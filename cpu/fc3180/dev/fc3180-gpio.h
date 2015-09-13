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

#ifndef _GPIO_H
#define _GPIO_H

#include <io_macro.h>

// Modes
#define INPT 0
#define OUTP 1

// Levels
#define LOW 0
#define HIGH 1

#define gpio_pin(pin) (1 << pin)

#define portc_lev(pin) (InDC() & pin)
#define portc_fsel(pin, mode) (mode == OUTP ? OrCC(pin) : AndCC(0xFF ^ pin))
#define portc_write(pin, level) (level == HIGH ? OrDC(pin) : AndDC(0xFF ^ pin))

#define porte_lev(pin) (InDE() & pin)
#define porte_fsel(pin, mode) (mode == OUTP ? OrCE(pin) : AndCE(0xFF ^ pin))
#define porte_write(pin, level) (level == HIGH ? OrDE(pin) : AndDE(0xFF ^ pin))

#define porth_lev(pin) (InDH() & pin)
#define porth_fsel(pin, mode) (mode == OUTP ? OrCH(pin) : AndCH(0xFF ^ pin))
#define porth_write(pin, level) (level == HIGH ? OrDH(pin) : AndDH(0xFF ^ pin))

#define porti_lev(pin) (InDI() & pin)
#define porti_fsel(pin, mode) (mode == OUTP ? OrCI(pin) : AndCI(0xFF ^ pin))
#define porti_write(pin, level) (level == HIGH ? OrDI(pin) : AndDI(0xFF ^ pin))

#define portj_lev(pin) (InDJ() & pin)
#define portj_fsel(pin, mode) (mode == OUTP ? OrCJ(pin) : AndCJ(0xFF ^ pin))
#define portj_write(pin, level) (level == HIGH ? OrDJ(pin) : AndDJ(0xFF ^ pin))

#define portn_lev(pin) (InDN() & pin)
#define portn_fsel(pin, mode) (mode == OUTP ? OrCN(pin) : AndCN(0xFF ^ pin))
#define portn_write(pin, level) (level == HIGH ? OrDN(pin) : AndDN(0xFF ^ pin))

#endif
