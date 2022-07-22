#ifndef __UART_H
#define __UART_H

#define UART_BAUD 9600
#define UBRR_VALUE (((F_CPU / (UART_BAUD * 16UL))) - 1)

namespace uart
{
    namespace
    {
        //ISR(USART_RX_vect);
    }

    void init(void);
    void putch(char c);
    void putstring(char* s, bool add_break);
    char getch(void);
}

#endif