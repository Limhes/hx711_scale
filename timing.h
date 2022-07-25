#ifndef __TIMING_H
#define __TIMING_H

#include <avr/io.h>
#include <avr/interrupt.h>

namespace timing
{
    void init(void);
    unsigned long millis (void);
    void delay_mss(double ms);

    /**
    * Delay execution by us-ish amounts
    *
    * Delays by zero (F_CPU < 1 MHz), one (F_CPU > 1 MHz) or two (F_CPU > 2 MHz) clock cycles per "us", so use cautiously
    *
    * @param us Amount of "microseconds" to delay
    * @return nothing
    */
    void delay_us(uint16_t us);
}


#endif
