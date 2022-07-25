
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

#include "timing.h"
#include "uart.h"
#include "HX711_ADC.h"

void init(void)
{
	uart::init();
	timing::init();
	sei(); // re-enable interrupts
}

int main(void)
{
	float mass_measured = 0;
	char outbuf[16];
	char* inbuf;
	uint32_t timestamp = 0;

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
			mass_measured = hx.getData();

			// print CSV data: timestamp, mass measured, debug
			sprintf(outbuf, "%lu, ", timestamp++);
			uart::putstring(outbuf, false);
			uart::putfloat(mass_measured);
			uart::putch('\n');

			_delay_ms(1000);
		}

		// handle incoming commands: t, m xxx.xxx
		inbuf = uart::getstring();
		if (inbuf[0] == 't')
		{
			hx.tareNoDelay();
		}
		else if (inbuf[0] == 'm')
		{
			hx.getNewCalibration(atof(&inbuf[2]));
		}
		//uart::putstring(inbuf, true);

		//if (hx.getTareStatus())
		//	uart::putstring((char*) "Tare complete", true);

	}
}
