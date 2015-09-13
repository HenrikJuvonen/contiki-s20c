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
#include "relay.h"
#include "fc3180-uart.h"
#include "no-sensors.h"
#include <stdio.h>
#include <string.h>

/*---------------------------------------------------------------------------*/
NO_SENSORS();
/*---------------------------------------------------------------------------*/
PROCESS(test_relay_process, "Test relay process");
AUTOSTART_PROCESSES(&test_relay_process);
/*---------------------------------------------------------------------------*/
PROCESS_THREAD(test_relay_process, ev, data)
{
  static char buf[32];
  static struct etimer et;

  PROCESS_BEGIN();

  relay_init();

  etimer_set(&et, 250);

  while (1) {
    PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&et));

    if (uart_available(3)) {
      memset(buf, 0, sizeof(buf));
      uart_read(3, buf, sizeof(buf));
      printf(">>> %s\n", buf);
      if (strstr(buf, "on") == buf) {
        relay_on();
      }
      if (strstr(buf, "off") == buf) {
        relay_off();
      }
    }

    etimer_reset(&et);
  }

  PROCESS_END();
}
/*---------------------------------------------------------------------------*/
