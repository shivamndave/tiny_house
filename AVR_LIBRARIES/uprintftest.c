#define F_CPU 16000000UL
#define BAUDRATE 38400
#include <avr/io.h>
#include <util/delay.h>
#include "uart.h"
#include "uprintf.h"

#define BLINK_DELAY_MS 1000


int main (void)
{
int i = 0;

 /* set pin 5 of PORTB for output*/
	DDRB = 0b10000000;
	PORTB &= 0x00;
	uartInit(BAUDRATE);
	while(1) 
	{
		for(i=0;i<3;i++)
		{
			uprintf("CURRENT ITTERATION: %d\n", i);
		/* set pin 5 high to turn led on */
//		PORTB ^= 0b10000000;
		/* set pin 5 low to turn led off */
		}
PORTB ^= 0b10000000;
	}
return 0;
}
