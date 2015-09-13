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

#include "contiki.h"
#include "spi-arch.h"

#include <stdint.h>
#include <io_macro.h>
#include <spi.h>

#define INT8_MAX 0xFF
#define INT16_MAX 0xFFFF
#define INT32_MAX 0xFFFFFFFF

static int32_t spi_delay = 0;
static uint32_t spi_mode = 0;
/*---------------------------------------------------------------------------*/
void
spi_init(void)
{
  spiSetPhase(&spi_mode, CPHA_FIRST);
  spiSetPolarity(&spi_mode, CPOL_LOW);
  spiSetBitOrder(&spi_mode, SPIORDER_MSB);
  spiSetSlaveSelect(&spi_mode, 7, 0x80, SS_ENABLE, SSPOL_LOW);
  spiSetBitRate(&spi_delay, 100);
  _spiInit();
}
/*---------------------------------------------------------------------------*/
void
spi_xmit(uint8_t *buffer, uint32_t length) {
  _spiXmit(buffer, length, spi_delay, spi_mode);
}
