
#include <stdint.h>

#include <avr/io.h>
#include <avr/interrupt.h>

#include "uart.h"

namespace uart
{
    namespace
    {
        volatile char received_char;

        ISR(USART_RX_vect)
        {
            cli();
            while(!(UCSR0A & (1 << RXC0))) {};
            received_char = UDR0;  // clear the USART interrupt
            sei();
        }
    }

    void init(void)
    {
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

    char getch(void)
    {
        char r = received_char;
        received_char = '\0';
        return r;
    }
}
