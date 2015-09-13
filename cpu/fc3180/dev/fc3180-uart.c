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

#include "fc3180-uart.h"
#include <stdio.h>

#define pmxc_or(b) OrCRB(CRB6_index,b)
#define uctrl_out(port,b) OutB(0x24+0x4*port,b)
#define uibr_out(port,b) OutB(0x25+0x4*port,b)

struct uart_buf uart_bufs[3];
/*---------------------------------------------------------------------------*/
uint8_t uart_init(int port, uint8_t baudrate, uint8_t databits, uint8_t stopbits, uint8_t parity) {
  pmxc_or(1 << (7-port)); // enable port
  uctrl_out(port, 0 << 6 | 1 << 4 | databits << 3 | stopbits << 2 | parity); // set opts
  uibr_out(port, baudrate); // set opts
  memset(uart_bufs[port-1], 0, sizeof(uart_bufs[port-1]));
  return 0;
}
/*---------------------------------------------------------------------------*/
uint8_t uart_read(int port, char *buffer, uint16_t length) {
  uint8_t r;
  if (length < uart_bufs[port-1].in_pos) {
    r = length;
    memcpy(buffer, uart_bufs[port-1].in, r);
    memcpy(uart_bufs[port-1].in, uart_bufs[port-1].in+r, uart_bufs[port-1].in_pos-r);
    uart_bufs[port-1].in_pos -= r;
  } else {
    r = uart_bufs[port-1].in_pos;
    memcpy(buffer, uart_bufs[port-1].in, r);
    uart_bufs[port-1].in_pos = 0;
  }
  return r;
}
/*---------------------------------------------------------------------------*/
uint8_t uart_write(int port, char *buffer, uint16_t length) {
  uint8_t r;
  if (length < sizeof(uart_bufs[port-1].out)-uart_bufs[port-1].out_pos) {
    r = length;
    memcpy(uart_bufs[port-1].out+uart_bufs[port-1].out_pos, buffer, r);
    uart_bufs[port-1].out_pos += r;
  } else {
    r = sizeof(uart_bufs[port-1].out);
    memcpy(uart_bufs[port-1].out+uart_bufs[port-1].out_pos, buffer, r);
    uart_bufs[port-1].out_pos = r;
  }
  uart_write_ex(port);
  return r;
}
/*---------------------------------------------------------------------------*/
