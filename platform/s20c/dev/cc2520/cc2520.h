/*
 * Copyright (c) 2011, Swedish Institute of Computer Science.
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
 * This file is part of the Contiki operating system.
 */

/**
 * \file
 *         CC2520 driver header file
 * \author
 *         Adam Dunkels <adam@sics.se>
 *         Joakim Eriksson <joakime@sics.se>
 *         Henrik Juvonen <heolju@gmail.com>
 */

#ifndef CC2520_H_
#define CC2520_H_

#include "contiki.h"
#include "spi-arch.h"
#include "spi.h"
#include "radio.h"
#include "cc2520_const.h"

#include <stdio.h>
#include <stdlib.h>

int cc2520_init(void);

#define CC2520_MAX_PACKET_LEN      127

int cc2520_set_channel(int channel);
int cc2520_get_channel(void);

void cc2520_set_pan_addr(unsigned pan,
                         unsigned addr,
                         const uint8_t *ieee_addr);

extern signed char cc2520_last_rssi;
extern uint8_t cc2520_last_correlation;

int cc2520_rssi(void);

extern const struct radio_driver cc2520_driver;

/**
 * \param power Between 1 and 31.
 */
void cc2520_set_txpower(uint8_t power);
int cc2520_get_txpower(void);
#define CC2520_TXPOWER_MAX  31
#define CC2520_TXPOWER_MIN   0

/**
 * Interrupt function, called from the simple-cc2520-arch driver.
 *
 */
int cc2520_interrupt(void);

/* XXX hack: these will be made as Chameleon packet attributes */
extern rtimer_clock_t cc2520_time_of_arrival,
  cc2520_time_of_departure;
extern int cc2520_authority_level_of_sender;

int cc2520_on(void);
int cc2520_off(void);

void cc2520_set_cca_threshold(int value);

extern uint8_t cc2520_buf[];

/************************************************************************/
/* Additional SPI Macros for the CC2520 */
/************************************************************************/
/* Send a strobe to the CC2520 */
#define CC2520_STROBE(s)                                              \
  cc2520_buf[0]=(s);                                                  \
  SPI_XMIT(cc2520_buf,1);

/* Write to a register in the CC2520 */
#define CC2520_WRITE_REG(adr,data)                                    \
  cc2520_buf[0]=CC2520_INS_MEMWR|(((adr)>>8)&0xFF);                   \
  cc2520_buf[1]=(adr)&0xFF;                                           \
  cc2520_buf[2]=(data);                                               \
  SPI_XMIT(cc2520_buf,3);

/* Read a register in the CC2520 */
#define CC2520_READ_REG(adr,data)                                     \
  cc2520_buf[0]=CC2520_INS_MEMRD|(((adr)>>8)&0xFF);                   \
  cc2520_buf[1]=(adr)&0xFF;                                           \
  cc2520_buf[2]=0;                                                    \
  SPI_XMIT(cc2520_buf,3);                                             \
  (data)=cc2520_buf[2];

#define CC2520_READ_FIFO_BYTE(data)                                   \
  cc2520_buf[0]=CC2520_INS_RXBUF;                                     \
  cc2520_buf[1]=0;                                                    \
  SPI_XMIT(cc2520_buf,2);                                             \
  (data)=cc2520_buf[1];

#define CC2520_READ_FIFO_BUF(buffer,count)                            \
  cc2520_buf[0]=CC2520_INS_RXBUF;                                     \
  memset(cc2520_buf+1,0,(count));                                     \
  SPI_XMIT(cc2520_buf,1+(count));                                     \
  memcpy((buffer),cc2520_buf+1,(count));

#define CC2520_WRITE_FIFO_BUF(buffer,count)                           \
  cc2520_buf[0]=CC2520_INS_TXBUF;                                     \
  memcpy(cc2520_buf+1,(buffer),(count));                              \
  SPI_XMIT(cc2520_buf,1+(count));

/* Write to RAM in the CC2520 */
#define CC2520_WRITE_RAM(buffer,adr,count)                            \
  cc2520_buf[0]=CC2520_INS_MEMWR|(((adr)>>8)&0xFF);                   \
  cc2520_buf[1]=(adr)&0xFF;                                           \
  memcpy(cc2520_buf+2,(buffer),(count));                              \
  SPI_XMIT(cc2520_buf,2+(count));

/* Read from RAM in the CC2520 */
#define CC2520_READ_RAM(buffer,adr,count)                             \
  cc2520_buf[0]=CC2520_INS_MEMRD|(((adr)>>8)&0xFF);                   \
  cc2520_buf[1]=(adr)&0xFF;                                           \
  memset(cc2520_buf+2,0,(count));                                     \
  SPI_XMIT(cc2520_buf,2+(count));                                     \
  memcpy((buffer),cc2520_buf+2,(count));

/* Read status of the CC2520 */
#define CC2520_GET_STATUS(s)                                          \
  cc2520_buf[0]=CC2520_INS_SNOP;                                      \
  SPI_XMIT(cc2520_buf,1);                                             \
  (s)=cc2520_buf[0];

#endif /* CC2520_H_ */
