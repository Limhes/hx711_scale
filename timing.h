#ifndef __TIMING_H
#define __TIMING_H

#include <avr/io.h>
#include <avr/interrupt.h>

namespace timing
{
    void init(void);
    unsigned long millis (void);
    void delay_mss(double ms);
    void delay_us(uint16_t us);
}


#endif
