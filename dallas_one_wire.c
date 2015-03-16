#include <avr/io.h>
#include <stdint.h>

#include <util/atomic.h>
#include <util/delay.h>

#include "dallas_one_wire.h"

/* Global Variables */
IDENTIFIER_LIST_t identifier_list;

/* Identifier routine return codes */
#define DALLAS_IDENTIFIER_NO_ERROR      0x00
#define DALLAS_IDENTIFIER_DONE          0x01
#define DALLAS_IDENTIFIER_SEARCH_ERROR  0x02

/* Private function prototypes */
static uint8_t discover_identifier(DALLAS_IDENTIFIER_t *, DALLAS_IDENTIFIER_t *);

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

    if ((DALLAS_PORT_IN & (1 << PIN)) == 0x00) {
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

// TODO: finish according to http://www.dietfig.org/avronewire.html
