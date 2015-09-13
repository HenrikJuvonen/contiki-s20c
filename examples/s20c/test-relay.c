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
