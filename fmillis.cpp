#include "esp_timer.h"
#include "sdkconfig.h"

#include "fmillis.h"

// return a millis() style timestamp, but uS resolution
// ARDUINO_ISR_ATTR
timestamp_t fmillis(void) {
  return (timestamp_t)(esp_timer_get_time() / 1000.0L);
}

timestamp_t fmicros(void) { return (timestamp_t)esp_timer_get_time(); }
