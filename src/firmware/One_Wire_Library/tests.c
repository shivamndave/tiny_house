

#define F_CPU 16000000UL

#define BAUDRATE 57600
#include <stdlib.h>


#include "../UART_LIBRARY/uart.h"
#include "OneWire.h"
#define F_CPU 16000000UL


int main(void) {

	/*
	 * UI variables
	 */

//	char ticker_[8]={'|','/','-','\\','|','/','-','\\'};
//	uint8_t ticker=0;
	int count = 0;
	char line[30];

	float conversion = 0.0;
	uartInit(BAUDRATE);
	/*
	 * OneWire Variables
	 */
	uint8_t chip_scratchpad[9];
	DALLAS_TEMPERATURE temp;

	uprintf("\r\n* One-Wire Bus Master Monitor *\r\n");

	_delay_ms(1000);

	while (1) {

		if (dallas_command(SKIP_ROM_COMMAND, 1)) {

			dallas_command(CONVERT_TEMP__COMMAND, 0);
			_delay_ms(500); //wait for conversion result.

			if (dallas_command(SKIP_ROM_COMMAND, 1)) {
				dallas_command(READ_SCRATCHPAD_COMMAND, 0);
				dallas_read_buffer(chip_scratchpad, 9);

				temp=getDallasTemp(chip_scratchpad[1],chip_scratchpad[0]);

				//create output string
				sprintf(line, "%d.%d",temp.integer,temp.fraction);

				conversion = DTtof(temp);

				uprintf("\n%d:  float: %f -- string: %s  -- Farenheight: %f", count++, conversion, line, CelsiusToFahrenheit(conversion));
				//uprintf("%c Temp: 0x%02X%02Xh %c%d.%04d\r\n",ticker_[ticker%8],scratchpad[1],scratchpad[0],temp.sign,temp.integer,temp.fraction);
				//remove '\n' to get one line output

				//send it over USART/stdout

				//send to LCD as well

				//backslash as user defined char for non-european LCD ROMS.
				if (line[0]=='\\'){
					//put the user defined character 0 into first position = backslash
					//backslash is defined inside LCDinit() by default (under code 0x07)
					line[0]=0x07;
				}



			}else
				uprintf("BUS Error: READ_SCRATCHPAD\r\n");
		} else
			uprintf("BUS Error: No slaves found\r\n");

		_delay_ms(100);

	}

	return 0;

}
