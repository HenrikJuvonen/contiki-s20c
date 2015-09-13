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
#include <stdint.h>

#include "hcsr04.h"
#include "fc3180-rtc.h"
#include "fc3180-gpio.h"

static uint8_t trig[2] = {gpio_pin(4), gpio_pin(6)};
static uint8_t echo[2] = {gpio_pin(5), gpio_pin(7)};

#define TIMEOUT 6500
/*---------------------------------------------------------------------------*/
static int
_read(unsigned char index)
{
  portc_write(trig[index], LOW);
  rtc_delay_us(5);
  portc_write(trig[index], HIGH);
  rtc_delay_us(10);
  portc_write(trig[index], LOW);

  uint16_t loopCntLOW = TIMEOUT;
  while (portc_lev(echo[index]) == LOW) {
    if (--loopCntLOW == 0) {
      return -1;
    }
  }

  uint16_t duration = 0;
  uint16_t loopCntHIGH = TIMEOUT;
  while (portc_lev(echo[index]) != LOW) {
    duration++;
    if (--loopCntHIGH == 0) {
      return -1;
    }
  }

  return duration / 58.2;
}
/*---------------------------------------------------------------------------*/
void
hcsr04_enable(void)
{
  portc_fsel(trig[0], OUTP);
  portc_fsel(echo[0], INPT);
  portc_fsel(trig[1], OUTP);
  portc_fsel(echo[1], INPT);
}
/*---------------------------------------------------------------------------*/
void
hcsr04_disable(void)
{
  portc_fsel(trig[0], INPT);
  portc_fsel(echo[0], INPT);
  portc_fsel(trig[1], INPT);
  portc_fsel(echo[1], INPT);
}
/*---------------------------------------------------------------------------*/
int
hcsr04_left_distance(void)
{
  return _read(0);
}
/*---------------------------------------------------------------------------*/
int
hcsr04_right_distance(void)
{
  return _read(1);
}
/*---------------------------------------------------------------------------*/
