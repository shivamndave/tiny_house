

#define F_CPU 16000000UL

#define BAUDRATE 57600
#include <stdlib.h>


#include "../UART_LIBRARY/uart.h"
#include "OneWire.h"
#define F_CPU 16000000UL


int main(void) 
{


	uprintf("\r\n* One-Wire Bus Master Monitor *\r\n");
	uartInit(BAUDRATE);
	while (1) 
	{
		uprintf("temp: %f\n", getTemperatureC());
	}

	return 0;

}
