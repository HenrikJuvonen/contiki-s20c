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
