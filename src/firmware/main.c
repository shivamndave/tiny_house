/*
* A DRIVER BY SARGIS S YONAN ON 21 OCTOBER 2015
* USED FOR THE MICRO GRID TEST BED
* GITHUB.COM/SARGISYONAN -- SARGISY@GMAIL.COM
*/

#include "Sensor_Driver/driver.h"
#include "One_Wire_Library/OneWire.h"
#include "UART_LIBRARY/uart.h"
#include <util/delay.h>
uint8_t *grabCommand(void);
ISR(TIM0_OVF_vect);

FSM_t FSM[] = {
		{&_Idle, {IDLE_STATE, IDLE_STATE, IDLE_STATE, IDLE_STATE, COOLING_STATE, COOLING_STATE, HEATING_STATE, IDLE_STATE}},
		{&_RelayOff, {IDLE_STATE, IDLE_STATE, IDLE_STATE, IDLE_STATE, IDLE_STATE, COOLING_STATE, HEATING_STATE, IDLE_STATE}},
		{&_RelayOn, {IDLE_STATE, IDLE_STATE, IDLE_STATE, IDLE_STATE, IDLE_STATE, COOLING_STATE, HEATING_STATE, IDLE_STATE}}
	};
		


int main (void)
{
	if (SystemInit())	// DEFINED IN driver.h
	{
	//	status->flags ^= EN_BIT;
		while(true)
		{
			//uprintf(RX_TX_FUNCTION_puts, "hello");
			if (uart1_available() >= 1) (ProcessCommand());
			if (status->flags & EN_BIT)
				{
				PrintSystemStatusString();

				FSM[status->currentState].Output_Func_ptr();
				status->flags = SensorResult();
				status->currentState = FSM[status->currentState].nextState[status->flags];
				}
		}
	}
	FreeMemory();
	PROGRAM_DIE();
}


// Takes a byte from the Xbee at uart1 and places it into variable c
// if a valid delimiter is found, the delimiter is replaced with 0xFF
// and c is returned.
	uint8_t *grabCommand(void)
	{
		uint8_t *c;
		c = (uint8_t*)malloc(8);

		int temp = 0;
		for(temp = 0; temp < 8; temp++){
			//if (temp == 8)return NULL;
			if (uart1_available() < 1){
				_delay_ms(5);
				//if nothing in buffer, wait 5ms.
				if (uart1_available() < 1)return c;
				//if still nothing available, return c.
			}

			uint8_t temp2 = uart1_getc();
			c[temp] = temp2;


//			if (c[temp] == '\0')return c;
			uprintf(RX_TX_FUNCTION_puts, "gC() = %d - %c\n", temp, c[temp]); //testing
			if (c[temp] == RX_DELIMITER) {
				c[temp] = '\0';

				return c;
			}
		}
		free(c);
		return NULL;
	}

// the whole received message is checksummed and stored as a
// 16-bit word before the null term uint16_t with the _array_checksum algorithm
// the first 16-bits are the command, and the next bytes are the argument to that command
/*
/ example of packet to send
/  rx    = | 0x0000[16 COMMAND BITS] | 8-BIT UPPER ARGUMENT + 8-BIT LOWER ARGUMENT | 16-BIT CHECKSUM | COMMAND_TERMINATION 16-BIT COMMAND (NUL) = 0x0000 |
/  index =                0                                 1                              2                               3
/ THE COMMAND -> rx = {{RECIEVE_MESSAGE_CHANGE_SETPOINT}, {0x0064}, CHECKSUM, {0x0000}} = change setpoint to 100 degrees celsius
*/

uint16_t ProcessCommand(void)
{
	uprintf(RX_TX_FUNCTION_puts, "IN pCommand()\n");
	uint8_t *rxByteArray;
	rxByteArray = grabCommand();
	//uart0_putc('C');
	if (rxByteArray != NULL){
		//for (int i = 0; rxByteArray[i] != RX_DELIMITER; i++)
		{
			uart0_putc((rxByteArray[i]));
			uprintf(RX_TX_FUNCTION_puts, "%s", rxByteArray);
		}
	}

	 	/*
		if (_str_checksum(rxByteArray) != rxByteArray[i - 1])
		{
			uprintf(RX_TX_FUNCTION_puts, "//%d//TRANSMISSION_ERROR//", TRANSMISSION_ERROR_CODE);
			return;
		}
		
		switch (rxByteArray[1] & EIGHT_BIT_OFFSET)
		{


			case ENABLE_STATE_MACHINE:
				uart0_putc('E');
				status->flags |= EN_BIT;
				break;


			case DISABLE_STATE_MACHINE:
				uart0_putc('O');
				status->flags &= EN_BIT;
				break;


			case RECEIVE_MESSAGE_CHANGE_SETPOINT:
				uart0_putc('S');
				if (rxByteArray[1] < TEMPERATURE_MAX)
				{
					status->setpoint = rxByteArray[1] & EIGHT_BIT_OFFSET;
				} else {
					uprintf(RX_TX_FUNCTION_puts, "//%d//CHANGE_SETPOINT_ERROR//", CHANGE_SETPOINT_ERROR_CODE);
				}
				break;



			case RECEIVE_MESSAGE_CHANGE_POSITIVE_OFFSET:
				uart0_putc('P');
				if (rxByteArray[1] < POSITIVE_OFFSET_MAX)
				{
					status->positiveOffset = rxByteArray[1] & EIGHT_BIT_OFFSET;
				} else {
					uprintf(RX_TX_FUNCTION_puts, "//%d//CHANGE_POSITIVE_OFFSET_ERROR//", CHANGE_POSITIVE_OFFSET_ERROR_CODE);
				}
				break;


			case RECEIVE_MESSAGE_CHANGE_NEGATIVE_OFFSET:
				uart0_putc('C');
				if (rxByteArray[1] < NEGATIVE_OFFSET_MAX)
				{
					status->negativeOffset = rxByteArray[1] & EIGHT_BIT_OFFSET;
				} else {
					uprintf(RX_TX_FUNCTION_puts, "//%d//CHANGE_NEGATIVE_OFFSET_ERROR//", CHANGE_NEGATIVE_OFFSET_ERROR_CODE);
				}
				break;


			case RECEIVE_MESSAGE_GET_SYSTEM_STATUS:
				uart0_putc('G');
				uprintf(RX_TX_FUNCTION_puts, "//%.2f//%d//%d//%.2f//%.2f//%.2f//%d//", getTemperatureC(), status->currentState, status->sysTime, status->setpoint, status->positiveOffset, status->negativeOffset);
				break;


			default:
				uart0_putc('D');
				uprintf(RX_TX_FUNCTION_puts, "//%d//UNKNOWN_ERROR//", UNKNOWN_ERROR_CODE);
				break;
		}
*/
	uart0_flush();
//	} else {
//		return PROCESS_COMMAND_ERROR;
//	}
	free(rxByteArray);
	return PROCESS_COMMAND_SUCCESS;
}
/*
// ISR()
ISR(TIM0_OVF_vect)           // every 10ms
{
	static uint8_t systick = 0;
	if ((systick++) == 100)
	{
		systick = 0;
		status->sysTime++;
	}
}
*/
