#define F_CPU 16000000UL
#define BAUDRATE 57600

#include <string.h>
#include <avr/io.h>
#include <util/delay.h>
#include "uart.h"

#define STRING_MAX 100
#define BLINK_DELAY_MS 1000

#define NULL_CHAR '\0'
#define NEWLINE_CHAR '\n'
#define BACKSPACE_CHAR 0x08

#define ZERO_VOLTS 0x00

#define PORTB5_HI 0b10000000


void AVRInit (void);


int main (void)
{
    char buf[STRING_MAX];
    AVRInit();
    while(1) 
	{
        UARTGrabString(buf, STRING_MAX);
        uprintf("\n\nFEEDBACK >> %s", buf);
    }	
    return 0;
}

void AVRInit (void)
{
    DDRB = PORTB5_HI;
    PORTB &= ZERO_VOLTS;
    uartInit(BAUDRATE);
}
