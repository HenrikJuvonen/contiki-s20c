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

#ifndef _RTC_H
#define _RTC_H

#include <io_macro.h>
#include <stdint.h>

extern uint64_t rtc_tm;

#define rtc_out(b) OutCRB(RTC_index,b)
#define rtc_clear(b) AndCRB(RTC_index,b^0xFF)
#define rtc_read() InCRB(RTC_index)

#define rtc_time_ex() do { \
    rtc_out(0); \
    rtc_tm = rtc_read(); \
    rtc_out(1); \
    rtc_tm += rtc_read() << 8; \
    rtc_out(2); \
    rtc_tm += rtc_read() << 16; \
    rtc_out(3); \
    rtc_tm += rtc_read() << 24; \
    rtc_out(4); \
    rtc_tm += rtc_read() << 32; \
    rtc_out(5); \
    rtc_tm += rtc_read() << 40; \
  } while(0)

#define rtc_time_ms() ((rtc_tm & 0xFFFFFFFF)/100.0)
#define rtc_elapsed_ms(tm) (rtc_time_ms() >= tm)

#define rtc_init() do { \
    rtc_tm = 0; \
    rtc_out(1 << 7); \
    rtc_clear(1 << 7); \
    rtc_time_ex(); \
  } while(0)

#define rtc_delay_us(us) delay(DLY12_1US*us)
#define rtc_delay_ms(ms) delay(DLY12_1MS*ms)

#endif
