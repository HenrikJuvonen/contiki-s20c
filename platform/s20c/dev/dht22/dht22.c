/*
 * Copyright (c) 2015, Henrik Juvonen.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. Neither the name of the Institute nor the names of its contributors
 *    may be used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE INSTITUTE AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE INSTITUTE OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 *
 */

#include <stdio.h>

#include "dht22.h"
#include "fc3180-rtc.h"
#include "fc3180-gpio.h"
/*---------------------------------------------------------------------------*/
#define TIMEOUT 255
/*---------------------------------------------------------------------------*/
static unsigned char pin = gpio_pin(4);
static unsigned char bits[5]; // Buffer to receive data
static double humidity = -1;
static double temperature = -1;
static unsigned long long tm = 0;
/*---------------------------------------------------------------------------*/
static int
_read() {
  if (!rtc_elapsed_ms(tm + 2000)) {
    return 1;
  }
  tm = rtc_time_ms();

  unsigned char i;

  // Init variables to receive data
  unsigned char mask = 128;
  unsigned char idx = 0;

  // Clear buffer
  memset(bits, 0, sizeof(bits));

  // Request sample
  portc_fsel(pin, OUTP);
  portc_write(pin, LOW);
  rtc_delay_ms(1);
  portc_write(pin, HIGH);
  rtc_delay_us(40);
  portc_fsel(pin, INPT);

  // Get acknowledge or timeout
  unsigned char loopCntLOW = TIMEOUT;
  while (portc_lev(pin) == LOW)
  {
    if (--loopCntLOW == 0) {
      return 0;
    }
  }

  unsigned char loopCntHIGH = TIMEOUT;
  while (portc_lev(pin) != LOW)
  {
    if (--loopCntHIGH == 0) {
      return 0;
    }
  }

  // Read output - 40 BITS => 5 BYTES
  for (i = 40; i != 0; i--) {
    loopCntLOW = TIMEOUT;
    while (portc_lev(pin) == LOW) {
      if (--loopCntLOW == 0) {
        return 0;
      }
    }

    unsigned char t = 0;

    loopCntHIGH = TIMEOUT;
    while (portc_lev(pin) != LOW) {
      t++;
      if (--loopCntHIGH == 0) {
        return 0;
      }
    }

    if (t > 30) {
      bits[idx] |= mask;
    }
    mask >>= 1;
    if (mask == 0) // Next byte?
    {
      mask = 128;
      idx++;
    }
  }
  portc_fsel(pin, OUTP);
  portc_write(pin, HIGH);

  return 1;
}
/*---------------------------------------------------------------------------*/
static unsigned char
_test_checksum() {
  return bits[4] == bits[0] + bits[1] + bits[2] + bits[3];
}
/*---------------------------------------------------------------------------*/
static void
_read_and_convert() {
  if (_read() && _test_checksum()) {
    humidity = (bits[0] * 256 + bits[1]) / 10;
    temperature = ((bits[2] & 0x7F) * 256 + bits[3]) / 10 * (bits[2] & 0x80 ? -1 : 1);
  } else {
    humidity = -1;
    temperature = -1;
  }
}
/*---------------------------------------------------------------------------*/
int
dht22_temp(void) {
  _read_and_convert();
  return temperature;
}
/*---------------------------------------------------------------------------*/
int
dht22_humidity(void) {
  _read_and_convert();
  return humidity;
}
/*---------------------------------------------------------------------------*/
