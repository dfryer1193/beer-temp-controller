//Only include once
#ifndef ds18b20_h
#define ds18b20_h

#include "dallas_one_wire.h"

#define TRUE 1
#define FALSE 0

#define TEMP_RESOLUTION = 12

#define CONVERT_T_COMMAND         0x44
#define READ_SCRATCHPAD_COMMAND   0xBE
#define WRITE_SCRATCHPAD_COMMAND  0x4E

#define make16(msb, lsb) ((msb << 8) | lsb)

// Reads the temperature from the thermometer.
float getTemp(void);

// Writes configuration data to device.
void ds18b20_configure(uint8_t tH, uint8_t tL, uint8_t conf);

#endif
