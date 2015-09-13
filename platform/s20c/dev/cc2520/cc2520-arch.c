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
#include "cc2520.h"
#include "clock.h"
#include "platform-conf.h"

extern volatile unsigned char cc2520_sfd_counter;
extern volatile unsigned short cc2520_sfd_start_time;
extern volatile unsigned short cc2520_sfd_end_time;

unsigned char cc2520_levs[4] = {0,0,0,0};
unsigned char cc2520_pins[5] = {gpio_pin(4),gpio_pin(5),gpio_pin(6),gpio_pin(7),gpio_pin(3)};

uint8_t cc2520_buf[256];

static unsigned char fifop_int = 0;
static unsigned char fifop_int_enabled = 0;

/*---------------------------------------------------------------------------*/
void
cc2520_arch_fifop_int_enable(void)
{
  fifop_int_enabled = 1;
}
/*---------------------------------------------------------------------------*/
void
cc2520_arch_fifop_int_disable(void)
{
  fifop_int_enabled = 0;
}
/*---------------------------------------------------------------------------*/
void
cc2520_arch_fifop_int_clear(void)
{
  fifop_int = 0;
}
/*---------------------------------------------------------------------------*/
void
cc2520_arch_interrupt(void)
{
  uint8_t levs[4];
  memcpy(levs, cc2520_levs, 4);

  uint8_t dc = InDC();
  cc2520_levs[0] = (dc & CC2520_FIFO_PIN) != LOW;
  cc2520_levs[1] = (dc & CC2520_FIFOP_PIN) != LOW;
  cc2520_levs[2] = (dc & CC2520_CCA_PIN) != LOW;
  cc2520_levs[3] = (dc & CC2520_SFD_PIN) != LOW;

  if (levs[3] != cc2520_levs[3])
  {
    if (cc2520_levs[3])
    {
      // printf("SFD INT 1\n");
      cc2520_sfd_counter++;
      cc2520_sfd_start_time = clock_time();
    }
    else
    {
      // printf("SFD INT 0\n");
      cc2520_sfd_counter = 0;
      cc2520_sfd_end_time = clock_time();
    }
  }
  if (levs[1] != cc2520_levs[1])
  {
    if (cc2520_levs[1])
    {
      // printf("FIFOP INT 1\n");
      if (!fifop_int && fifop_int_enabled) {
        fifop_int = 1;
        cc2520_interrupt();
      }
    }
  }
}
/*---------------------------------------------------------------------------*/
void
cc2520_arch_init(void)
{
  OutCC(0x00);
  
  uint8_t dc = InDC();
  cc2520_levs[0] = (dc & CC2520_FIFO_PIN) != LOW;
  cc2520_levs[1] = (dc & CC2520_FIFOP_PIN) != LOW;
  cc2520_levs[2] = (dc & CC2520_CCA_PIN) != LOW;
  cc2520_levs[3] = (dc & CC2520_SFD_PIN) != LOW;

  porth_fsel(CC2520_RESET_PIN, OUTP);

  spi_init();
}
/*---------------------------------------------------------------------------*/
