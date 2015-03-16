#include <avr/io.h>
#include <stdint.h>

#include <util/atomic.h>
#include <util/delay.h>

#include "dallas_one_wire.h"

/* Global Variables */
IDENTIFIER_LIST_t identifier_list;

/* Identifier routine return codes */
#define IDENTIFIER_NO_ERROR      0x00
#define IDENTIFIER_DONE          0x01
#define IDENTIFIER_SEARCH_ERROR  0x02

/* Private function prototypes */
static uint8_t discover_identifier(IDENTIFIER_t *, IDENTIFIER_t *);

/* functions */
void write(uint8_t bit) {
  if (bit == 0x00) {
    ATOMIC_BLOCK(ATOMIC_RESTORESTATE) {
      // configure pin as output
      DDR |= (1 << PIN);
      
      // Pull the bus low
      PORT &= ~(1 << PIN);

      // Wait requ'd time
      _delay_us(90);

      // Release the bus
      PORT |= (1 << PIN);

      // Expire time slot
      _delay_us(90);
    }
  } else {
    ATOMIC_BLOCK(ATOMIC_RESTORESTATE) {
      // Configure pin as output.
      DDR |= (1 << PIN);

      // Pull bus low.
      PORT &= ~(1 << PIN);

      // wait req'd time
      _delay_us(10);

      // Release bus.
      PORT |= (1 << PIN);

      // Expire time slot
      _delay_us(30);
    }
  }
}

uint8_t read(void) {
  uint8_t reply;
  ATOMIC_BLOCK(ATOMIC_RESTORESTATE) {
    // Configure pin as output
    DDR |= (1 << PIN);

    // Pull bus low
    PORT &= ~(1 << PIN);

    // Wait requ'd time
    _delay_us(2);

    // Configure for input
    DDR &= ~(1 << PIN);

    // Wait for a while
    _delay_us(11);

    if ((PORT_IN & (1 << PIN)) == 0x00) {
      reply = 0x00;
    } else {
      reply = 0x01;
    }

    // Expire time slot
    _delay_us(47);
  }
  return reply;
}

uint8_t reset(void) {
  uint8_t reply;

  //reset reply var
  reply = 0x00;
  ATOMIC_BLOCK(ATOMIC_RESTORESTATE) {
    // Configure pin for output
    DDR |= (1 << PIN);

    // Pull bus low
    PORT &= ~(1 << PIN);

    // Wait requ'd time
    _delay_us(500);

    //Switch to input, enable pin change interrupt, wait.
    DDR &= ~(1 << PIN);

    _delay_us(70);

    if ((PORT_IN & (1 << PIN)) == 0x00) {
      reply = 0x01;
    }

    _delay_us(420);
  }

  return reply;
}

void write_byte(uint8_t byte) {
  uint8_t position;

  for (position = 0x00; position < 0x08; position++) {
    write(byte & 0x01);

    byte = (byte >> 1);
  }
}

uint8_t read_byte(void) {
  uint8_t byte;
  uint8_t position;

  byte = 0x00;

  for (position = 0x00; position < 0x08; position++) {
    byte += (read() << position);
  }
  return byte;
}

void drive_bus(void) {
  // configure pin as output
  DDR |= (1 << PIN);

  // Set bus high
  PORT |= (1 << PIN);
}

void match_rom(IDENTIFIER_t * identifier) {
  uint8_t identifier_bit;
  uint8_t current_byte;
  uint8_t current_bit;
  
  reset();
  write_byte(MATCH_ROM_COMMAND);

  for (identifier_bit = 0x00; identifier_bit < NUM_IDENTIFIER_BITS; identifier_bit++) {
    current_byte = identifier_bit / 8;
    current_bit = identifier_bit - (current_byte * 8);

    write(identifier->identifier[current_byte] & (1 << current_bit));
  }
}

void skip_rom(void) {
  reset();
  write_byte(SKIP_ROM_COMMAND);
}

uint8_t search_identifiers(void) {
  uint8_t current_device;
  uint8_t return_code;

  for (current_device = 0x00; current_device < NUM_DEVICES; current_device++) {
    if (current_device == 0x00) {
      return_code = discover_identifier(&identifier_list.identifiers[current_device], 0x00);
    } else {
      return_code = discover_identifier(&identifier_list.identifiers[current_device], \
          &identifier_list.identifiers[current_device-1]);
    }

    if (return_code == IDENTIFIER_DONE) {
      identifier_list.num_devices = current_device + 0x01;
      return 0x00;
    } else if (return_code == IDENTIFIER_SEARCH_ERROR) {
      return 0x01;
    }
  }
  return 0x02;
}

IDENTIFIER_LIST_t * get_identifier_list(void) {
  return &identifier_list;
}

static uint8_t discover_identifier(IDENTIFIER_t * current_identifier, IDENTIFIER_t * last_identifier) {
  uint8_t identifier_bit;
  uint8_t recieved_two_bits;
  uint8_t current_bit;
  uint8_t current_byte;
  uint8_t identifier_diverged;

  identifier_diverged = 0x00;
  identifier_bit = 0x00;

  reset();
  write_byte(SEARCH_ROM_COMMAND);

  for (identifier_bit = 0; identifier_bit < NUM_IDENTIFIER_BITS; identifier_bit++) {
    recieved_two_bits = (read() << 1);
    recieved_two_bits += read();

    current_byte = identifier_bit / 8;
    current_bit = identifier_bit - (current_byte * 8);

    if (recieved_two_bits == 0x02) {
      // All devices have a 1 at this position.
      current_identifier->identifier[current_byte] += (1 << current_bit);

      write(0x01);
    } else if (recieved_two_bits == 0x01) {
      // All devices have a 0 at this position.

      write(0x00);
    } else if (recieved_two_bits == 0x00) {
      if ((identifier_diverged == 0x00) && (last_identifier != 0x00)) {
        identifier_diverged = 0x01;

        if ((last_identifier->identifier[current_byte] & (1 << current_bit)) == 0x00) {
          // Then we choose 1.

          current_identifier->identifier[current_byte] += (1 << current_bit);

          write(0x01);
        } else {
          // 0 otherwise.

          write(0x00);
        }
      } else {
        // We'll go with 0.
        write(0x00);
      }
    } else {
      // ERROR!
      return IDENTIFIER_SEARCH_ERROR;
    }
  }

  if (identifier_diverged == 0x00) {
    return IDENTIFIER_DONE;
  } else {
    return IDENTIFIER_NO_ERROR;
  }
}

void write_buffer(uint8_t * buffer, uint8_t buffer_length) {
  uint8_t i;

  for (i = 0x00; i < buffer_length; i++) {
    write_byte(buffer[i]);
  }
}

void read_buffer(uint8_t * buffer, uint8_t buffer_length) {
  uint8_t i;

  for (i = 0x00; i < buffer_length; i++) {
    buffer[i] = read_byte();
  }
}

