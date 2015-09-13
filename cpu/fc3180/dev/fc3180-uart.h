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

#ifndef _UART_H
#define _UART_H

#include <io_macro.h>
#include <stdint.h>

// helpers
#define usts_read(port) InB(0x26+0x4*port)
#define udata_out(port,b) OutB(0x27+0x4*port,b)
#define udata_read(port) InB(0x27+0x4*port)

// stop bits
#define STOPBITS_1   0
#define STOPBITS_2   1

// data bits
#define DATABITS_8   0
#define DATABITS_7   1

// parity
#define PARITY_NONE  0
#define PARITY_SPACE 1
#define PARITY_ODD   2
#define PARITY_EVEN  3

// baudrates
#define B300    0x7
#define B600    0x6
#define B1200   0x5
#define B2400   0x4
#define B4800   0x3
#define B9600   0x2
#define B19200  0x1
#define B38400  0x0
#define B7200   0xF
#define B14400  0xE
#define B28800  0xD
#define B57600  0xC
#define B115200 0xB
#define B234400 0xA
#define B460800 0x9
#define B921600 0x8

// conversions
#define BAUDRATE(b) (b <= 0x7 ? 300 * (1 << (0x7-b)) : 921600/(1 << (b-0x8)))
#define BDELAY(b) (1000000 / (BAUDRATE(b)/8))

// struct for input and output buffers
struct uart_buf {
  uint8_t in[256];
  uint8_t in_pos;
  uint8_t out[256];
  uint8_t out_pos;
};

extern struct uart_buf uart_bufs[3];

uint8_t uart_init(int port, uint8_t baudrate, uint8_t databits, uint8_t stopbits, uint8_t parity);
uint8_t uart_read(int port, char *buffer, uint16_t length);
uint8_t uart_write(int port, char *buffer, uint16_t length);

#define uart_available(port) uart_bufs[port-1].in_pos

#define uart_read_ex(port) do { \
    uint8_t r = usts_read(port); \
    while (0x40 & r) { \
      uart_bufs[port-1].in[uart_bufs[port-1].in_pos++] = udata_read(port); \
      r = usts_read(port); \
    } \
  } while(0)

#define uart_write_wait(port) do { \
    uint8_t r = usts_read(port); \
    if (0x20 & r) continue; \
    if (0x80 & r) break; \
  } while (1)

#define uart_write_ex(port) do { \
    uint8_t i = 0; \
    for (i = 0; i < uart_bufs[port-1].out_pos; i++) { \
      udata_out(port, uart_bufs[port-1].out[i]); \
      uart_write_wait(port); \
    } \
    uart_bufs[port-1].out_pos = 0; \
  } while(0)

#endif
