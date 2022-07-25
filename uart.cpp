
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include <avr/io.h>
#include <avr/interrupt.h>

#include "uart.h"

namespace uart
{
    namespace
    {
        volatile char received_char;
        char received_string[16], received_string_stored[16];
        bool string_returned;
        volatile uint8_t num_received;

        ISR(USART_RX_vect)
        {
            cli();
            while(!(UCSR0A & (1 << RXC0))) {};
            received_char = UDR0;  // clears the USART interrupt
            if (received_char == '\r')
            { /* silently ignore */ }
            else if (received_char == '\n')
            {
                received_string[num_received] = '\0';
                strcpy(received_string_stored, received_string);
                string_returned = false;
                num_received = 0;
            }
            else
            {
                received_string[num_received++] = received_char;
            }
            sei();
        }
    }

    void init(void)
    {
        num_received = 0;
        string_returned = false;

        UBRR0H = (uint8_t) (UBRR_VALUE >> 8);
        UBRR0L = (uint8_t) UBRR_VALUE;
        UCSR0B |= (1 << RXEN0) | (1 << TXEN0) | (1 << RXCIE0);   // Turn on the transmission, reception, and Receive interrupt
        UCSR0C |= (1 << UCSZ01) | (1 << UCSZ00);
    }

    void putch(char c)
    {
        loop_until_bit_is_set(UCSR0A, UDRE0);
        UDR0 = c;
    }

    void putstring(char* s, bool add_break)
    {
        uint8_t i = 0;
        while (s[i] != '\0')
            putch(s[i++]);
        if (add_break)
        {
            putch('\n');
            putch('\r');
        }
    }

    void putfloat(float f)
    {
        static char outbuf[10];

        // because sprintf for floats is disabled in AVR, we do it ourselves:
        if (f < 0) uart::putch('-');
        sprintf(outbuf, "%lu", (uint32_t) fabs(f));
        uart::putstring(outbuf, false);
        uart::putch('.');
        sprintf(outbuf, "%03lu", ((uint32_t) (fabs(f*1000.0)))%1000);
        uart::putstring(outbuf, false);
    }

    char* getstring(void)
    {
        if (!string_returned)
        {
            string_returned = true;
            return received_string_stored;
        }
        else
        {
            return (char*) "";
        }
    }
}
