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
#include "dev/eeprom.h"

#include <stdio.h>
#include <stdlib.h>
#include "io_macro.h"

#define WRITE_ENABLE() AndCRB(CRB12_index, 1)
#define WRITE_DISABLE() OrCRB(CRB12_index, 2)
#define SET_ADDRESS(addr) do { \
    if (addr & 0x100 != 0) OrCRB(CRB12_index, 1); \
    else AndCRB(CRB12_index, 2); \
    OutCRB(CRB14_index, addr & 0xFF); \
  } while(0)
#define WRITE(data) OutCRB(CRB13_index, data)
#define READ() InCRB(CRB13_index)

/*---------------------------------------------------------------------------*/
static void
eeprom_fill(eeprom_addr_t addr, unsigned char value, int size)
{
  int i;
  WRITE_ENABLE();
  for (i = 0; i < size; i++) {
    SET_ADDRESS(addr+i);
    WRITE(value);
  }
  WRITE_DISABLE();
}

/*---------------------------------------------------------------------------*/
static void
eeprom_test()
{
  unsigned char buf[0x1FF];
  eeprom_read(0, buf, 0x1FF);

  int i;
  for (i = 0; i < 0x1FF; i++) {
    printf("%02X\n", buf[i]);
  }

  eeprom_fill(0, 0x00, 0x1FF);
  eeprom_read(0, buf, 0x1FF);

  for (i = 0; i < 0x1FF; i++) {
    printf("%02X\n", buf[i]);
  }

  eeprom_fill(0, 0xFF, 0x1FF);
}

/*---------------------------------------------------------------------------*/
void
eeprom_init(void)
{
}

/*---------------------------------------------------------------------------*/
void
eeprom_write(eeprom_addr_t addr, unsigned char *buf, int size)
{
  int i;
  WRITE_ENABLE();
  for (i = 0; i < size; i++) {
    SET_ADDRESS(addr+i);
    WRITE(buf[i]);
  }
  WRITE_DISABLE();
}

/*---------------------------------------------------------------------------*/
void
eeprom_read(eeprom_addr_t addr, unsigned char *buf, int size)
{
  int i;
  WRITE_DISABLE();
  for (i = 0; i < size; i++) {
    SET_ADDRESS(addr+i);
    buf[i] = READ();
  }
}
