/* An AVR library for Dallas OneWire devices. */

#ifndef DALLAS_ONE_WIRE_H
#define DALLAS_ONE_WIRE_H

/* defines */

// clock freq
#define F_CPU 8000000UL //1MHz

// pins
#define PORT     PORTB
#define PORT_IN  PINB
#define DDR      DDRB
#define PIN      0

// count of devices on bus
#define NUM_DEVICES  1

// num bits in identifier
#define NUM_IDENTIFIER_BITS  64

// OneWire commands
#define MATCH_ROM_COMMAND   0x55
#define SKIP_ROM_COMMAND    0xCC
#define SEARCH_ROM_COMMAND  0xF0

/* Structures */
typedef struct {
  uint8_t identifier[NUM_IDENTIFIER_BITS / 8];
} IDENTIFIER_t;

typedef struct {
  IDENTIFIER_t identifiers[NUM_DEVICES];
  uint8_t num_devices;
} IDENTIFIER_LIST_t;

/* Functions */
// Writes lsb of arg to bus.
void write(uint8_t);

// Write a byte to the bus.
void write_byte(uint8_t);

// Write bytes from specified buffer
void write_buffer(uint8_t * buffer, uint8_t buffer_len);

// Read a bit from the bus and returns it as LSB
uint8_t read(void);

// Read a byte from the bus,
uint8_t read_byte(void);

// Reads bytes into specified buffer
read_buffer(uint8_t * buffer, uint8_t buffer_len);

// Resets the bus. Returns:
//  TRUE - if a device indicates presence
//  FALSE - else
uint8_t reset(void);

// Powers the bus from the AVR (max 40A)
void drive_bus(void);

// Sends a MATCH_ROM_COMMAND to spec'd device, auto reset.
void match_rom(DALLAS_IDENTIFIER_t *);

// Sends a SKIP_ROM_COMMAND. Automatically resets the bus.
void skip_rom(void);

// Populates identifier list. Returns:
//  0 - if devices were found and there was no error
//  1 - if there was a bus error
//  2 - if there were more devices than spec'd by DALLAS_NUM_DEVICES
uint8_t search_identifiers(void);

#endif
