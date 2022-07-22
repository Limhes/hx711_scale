#ifndef _MILLIS_H_THISONE
#define _MILLIS_H_THISONE

#include <avr/io.h>
#include <avr/interrupt.h>

ISR(TIMER1_COMPA_vect);

void init_millis(unsigned long f_cpu);
unsigned long millis (void);

void delayMicroseconds(uint16_t us);

#endif
