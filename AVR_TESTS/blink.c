#define F_CPU 16000000UL
#define BAUDRATE 38400
#include <avr/io.h>
#include <util/delay.h>
#include <stdio.h>
#include <stdlib.h>
#include "uart.h"

#define BLINK_DELAY_MS 1000
char* IntToString(int word);

int main (void)
{
int i = 0;
char *string;
 /* set pin 5 of PORTB for output*/
	DDRB = 0b10000000;
	PORTB &= 0x00;
	uartInit(BAUDRATE);
 	while(1) 
 	{
 		string = IntToString(++i);
 		uartPutString(string);
 		uartTransmitByte(' ');
 		free(string);
	  	/* set pin 5 high to turn led on */
		PORTB |= 0b10000000;
	  	/* set pin 5 low to turn led off */
	   	}
return 0;
}

char *IntToString(int word)
{
	char *string = (char*)malloc(sizeof(int) + 1);
	sprintf(string, "%d", word);
	return string;
}
/*
char *DoubleToString (double word)
{
	char *string = (char*)malloc(sizeof(double) + 1);
	sprintf(s)
}
*



/



void printf (char* string, void* delimit...)
{
int i = 0;
while (fgetc(string) != '%')
{
	switch (fgetc(string))
	{
	case d:strncat(itoa((int)delimit[i]), string)
			
	case f:
	case s:
	}
}
*/
