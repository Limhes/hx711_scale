#ifndef __UART_H
#define __UART_H

#define UART_BAUD 9600
#define UBRR_VALUE (((F_CPU / (UART_BAUD * 16UL))) - 1)

namespace uart
{
    void init(void);
    void putch(char c);
    void putstring(char* s, bool add_break);
    void putfloat(float f);
    char* getstring(void);
}

#endif
