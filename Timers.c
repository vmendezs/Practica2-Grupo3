#include "Timers.h"
#include <avr/interrupt.h>

void T_inicie_timer(){
// TIMER 0 for interrupt frequency 100.16025641025641 Hz:
cli(); // stop interrupts
TCCR0A = 0; // set entire TCCR0A register to 0
TCCR0B = 0; // same for TCCR0B
TCNT0  = 0; // initialize counter value to 0
// set compare match register for 100.16025641025641 Hz increments
OCR0A = limite; // = 16000000 / (1024 * 100.16025641025641) - 1 (must be <256)
// turn on CTC mode
TCCR0B |= (1 << WGM01);
// Set CS02, CS01 and CS00 bits for 1024 prescaler
TCCR0B |= (1 << CS02) | (0 << CS01) | (1 << CS00);
// enable timer compare interrupt
TIMSK0 |= (1 << OCIE0A);
sei(); // allow interrupts
}