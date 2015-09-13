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

#ifndef __PLATFORM_CONF_H__
#define __PLATFORM_CONF_H__

#include "spi-arch.h"
#include "fc3180-gpio.h"

#define BV(x) (1 << x)
#define splhigh() 0
#define splx(sr)

void cc2520_arch_fifop_int_enable(void);
void cc2520_arch_fifop_int_disable(void);
void cc2520_arch_fifop_int_clear(void);
void cc2520_arch_interrupt(void);

extern unsigned char cc2520_levs[4];

/*
 * SPI bus configuration for FC3180
 */

#define CC2520_CONF_SYMBOL_LOOP_COUNT 2718

extern unsigned char cc2520_pins[5];

/* SPI bus - CC2520 pin configuration. */
#define CC2520_FIFO_PIN  cc2520_pins[0]
#define CC2520_FIFOP_PIN cc2520_pins[1]
#define CC2520_CCA_PIN   cc2520_pins[2]
#define CC2520_SFD_PIN   cc2520_pins[3]
#define CC2520_RESET_PIN cc2520_pins[4]

/* CC2520 Pin status. */
#define CC2520_FIFO_IS_1  ((portc_lev(CC2520_FIFO_PIN)) != LOW)
#define CC2520_FIFOP_IS_1 ((portc_lev(CC2520_FIFOP_PIN)) != LOW)
#define CC2520_CCA_IS_1   ((portc_lev(CC2520_CCA_PIN)) != LOW)
#define CC2520_SFD_IS_1   ((portc_lev(CC2520_SFD_PIN)) != LOW)

/* CC2520 reset pin. */
#define SET_RESET_INACTIVE() porth_write(CC2520_RESET_PIN, HIGH)
#define SET_RESET_ACTIVE()   porth_write(CC2520_RESET_PIN, LOW)

/* CC2520 voltage regulator enable pin. */
#define SET_VREG_ACTIVE()
#define SET_VREG_INACTIVE()

/* CC2520 rising edge trigger for external interrupt 0 (FIFOP). */
#define CC2520_FIFOP_INT_INIT() do { CC2520_ENABLE_FIFOP_INT(); CC2520_CLEAR_FIFOP_INT(); } while(0)

/* FIFOP on external interrupt 0. */
#define CC2520_ENABLE_FIFOP_INT()  cc2520_arch_fifop_int_enable()
#define CC2520_DISABLE_FIFOP_INT() cc2520_arch_fifop_int_disable()
#define CC2520_CLEAR_FIFOP_INT()   cc2520_arch_fifop_int_clear()

#endif
