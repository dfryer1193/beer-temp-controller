#include "ds18b20.h"

uint8_t ds18b20::reset(void) { 
  uint16_t mask = bitmask;
  volatile uint8_t *reg IO_REG_ASM = baseReg;
  uint8_t r;
  uint8_t retries = 125;

  //TODO define macro for setting output
  do {
  } while (TRUE);
}
