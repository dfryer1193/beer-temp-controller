#include "ds18b20.h"

/*
 * Reads the temperature from the device.
 * ( deg c )
 */
float getTemp(void) {
  uint8_t busy = 0;
  uint8_t temp1;
  uint8_t temp2;
  int16_t temp3;
  float result;

  // Address all devices
  skip_rom();
  write_byte(CONVERT_T_COMMAND); // Start temperature conversion

  while(!busy) {        // Wait while bus is low
    busy = read_byte();
  }

  reset();
  write_byte(READ_SCRATCHPAD_COMMAND);
  temp1 = read_byte();
  temp2 = read_byte();
  temp3 = make16(temp2, temp1);

  // Temp conversion
  result = (float) temp3 / 16.0;

  // Expire the time slot
  _delay_ms(200);
  return result; // This is in deg c
}

/*
 * Args: alarm trigger high, alarm trigger low, config
 */
void ds18b20_configure(uint8_t tH, uint8_t tL, uint8_t conf) {
  reset();
  skip_rom();
  write_byte(WRITE_SCRATCHPAD_COMMAND); // Write to scratchpad
  write_byte(tH);
  write_byte(tL);
  write_byte(conf);
}
