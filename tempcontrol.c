/*
 * tempcontrol.c
 * 
 * runs a temperature controller using an ATiny85 (Adafruit Trinket 3.3V)
 * the mcu for this is "avr25" (avr-gcc ... -mmcu=avr25 ...)
 *
 * See http://www.sourcecodebrowser.com/avr-libc/1.8.0/iotn85_8h.html for more
 * documentation
 *
 * Author: David Fryer <dfryer1193@gmail.com>
 *
 */

#include<avr/io.h>
#include<util/delay.h>

#include "ds18b20.h"

// Ales
#define TEMP_RANGE_MAX  70.0
#define TEMP_IDEAL      67.0
#define TEMP_RANGE_MIN  64.0

#define MINUTE_MS       60000

static float convert(float temperature);

int main(void){
  float temperature = 0.0;
  bool  heat_on;
  bool  cool_on;
  
  // Setup
  reset();
  
  while(1) { //main loop
    temperature = getTemp();
    temperature = convert(temperature);
    
    // For ales, 62 < temp < 72
    if (temperature < TEMP_IDEAL) {

      if (cool_on) {
        // turn off cooling
        cool_on = FALSE;
      }

      if (!heat_on) {
        // turn on lamp
        heat_on = TRUE;
      }

      // Wait a minute to avoid rapid flickering
      _delay_ms(MINUTE_MS);
    } else if (temperature > TEMP_IDEAL) {

      if (heat_on) {
        // turn off heating
        heat_on = FALSE;
      }

      if (!cool_on) {
        // Turn on cooling
        cool_on = TRUE;
      }

      // Wait a minute to avoid flickering
      _delay_ms(MINUTE_MS);
    }
  }
  
  return 0;
}

static float convert(float temperature) {
  return (temperature * (9.0/5.0) + 32.0);
}
