
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

#include "millis.h"
#include "uart.h"
#include "HX711_ADC.h"

void init(void)
{
	uart::init();
	init_millis(F_CPU);
	sei();
}

int main(void)
{
	float raw = 0;
	char outbuf[10];
	init();

	HX711_ADC hx;
	hx.begin(128); // gain
	hx.start(1000, true); // stabilizing time, tare

	float calibrationValue = 2172.0;
	if (!hx.getTareTimeoutFlag()) hx.setCalFactor(calibrationValue);

	while (true)
	{
		if (hx.update())
		{
			raw = hx.getData();

			uart::putstring((char*) "mass: ", false);
			if (raw < 0) uart::putch('-');
			sprintf(outbuf, "%lu", (uint32_t) fabs(raw));
			uart::putstring(outbuf, false);
			uart::putch('.');
			sprintf(outbuf, "%03lu", ((uint32_t) (fabs(raw*1000.0)))%1000);
			uart::putstring(outbuf, false);
			uart::putstring((char*) " g", true);

			_delay_ms(1000);
		}

		// receive command from serial terminal, send 't' to initiate tare operation:
		if (uart::getch() == 't') hx.tareNoDelay();

		// check if last tare operation is complete:
		if (hx.getTareStatus())
		{
			uart::putstring((char*) "Tare complete", true);
		}
	}
}
