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
#include "dht22-sensor.h"
#include <stdio.h>

/*---------------------------------------------------------------------------*/
SENSORS(&dht22_sensor);
/*---------------------------------------------------------------------------*/
PROCESS(test_dht22_process, "Test dht22 process");
AUTOSTART_PROCESSES(&test_dht22_process);
/*---------------------------------------------------------------------------*/
PROCESS_THREAD(test_dht22_process, ev, data)
{
  static struct etimer et;
  static const struct sensors_sensor *sensor;
  static int temp;
  static int humidity;

  PROCESS_BEGIN();

  etimer_set(&et, 2000);

  while (1) {
    PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&et));

    sensor = sensors_find("dht22");
    if (sensor) {
      temp = sensor->value(DHT22_SENSOR_TEMP);
      humidity = sensor->value(DHT22_SENSOR_HUMIDITY);

      printf("%d ", clock_time());
      if (temp == -1 || humidity == -1) {
        printf("Temperature=?? Humidity=??\n");
      } else {
        printf("Temperature=%d Humidity=%d\n", temp, humidity);
      }
    }

    etimer_reset(&et);
  }

  PROCESS_END();
}
/*---------------------------------------------------------------------------*/
