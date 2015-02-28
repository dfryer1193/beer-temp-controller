//Only include once
#ifndef ds18b20_h
#define ds18b20_h

//might not be needed
//#include <inttypes.h>

#define TRUE 1
#define FALSE 0

//Function prototypes

// Performs a reset cycle
// Returns 1 if a device returns a presence pulse
// Returns 0 if there is no device or the bus is shorted or low for >250us
uint8_t reset(void);

void skip(void);

// Write a byte. If power is 1, hold the device high at the end for
// parasitically powered devices
void write(uint8_t v, uint8_t power = 0);

void write_bytes(const uint8_t *buf, uint8_t count, bool power=0);

// Read a byte
uint8_t read(void);

void read_bytes(uint8_t *buf, uint16_t count);

// Write a bit
void write_bit(uint8_t v);

// Read a bit
uint8_t read_bit(void);

// Stop powering the bus
void depower(void);

//Sensor structure
struct DS18B20 {
  unsigned char ROM_NO[8];
  uint8_t LastDiscrepancy;
  uint8_t LastFamilyDiscrepancy;
  uint8_t LastDeviceFlag;
}

#endif
