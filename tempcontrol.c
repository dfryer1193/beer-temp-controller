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

int main(void){
  //Set clock to 16 MHz
  //Other setup
  //Main loop
    //Get temp
    //If out of range, turn on heat lamp
    //If in range, turn output off
    //Sleep for 60 s
  return 0;
}
