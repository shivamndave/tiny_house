/*
* A DRIVER BY SARGIS S YONAN ON 21 OCTOBER 2015
* USED FOR THE MICRO GRID TEST BED
* GITHUB.COM/SARGISYONAN -- SARGISY@GMAIL.COM
*/

#include "Sensor_Driver/driver.h"
#include "One_Wire_Library/OneWire.h"
#include "UART_LIBRARY/uart.h"
#include <util/delay.h>
uint16_t *grabCommand(void);
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


	uint16_t *grabCommand(void)
	{
		uint16_t *c;

		c = (uint16_t*)malloc(11);
		c[0] = uart1_getc();
		if (c[0] == RX_DELIMITER) {
			c[0] = 0xFF;
			return c;
			}
		c[1] = uart1_getc();
		if (c[1] == RX_DELIMITER) {
			c[1] = 0xFF;
			return c;
			}
		c[2] = uart1_getc();
		if (c[2] == RX_DELIMITER) { 
			c[2] = 0xFF;
			return c;
			}
		c[3] = uart1_getc();
		if (c[3] == RX_DELIMITER) {
			c[3] = 0xFF;
			return c;
			}
		c[4] = uart1_getc();
		if (c[4] == RX_DELIMITER) {
			c[4] = 0xFF;
			return c;
			}
		c[5] = uart1_getc();
		if (c[5] == RX_DELIMITER) {
			c[5] = 0xFF;
			return c;
			}
		c[6] = uart1_getc();
		if (c[6] == RX_DELIMITER) {
			c[6] = 0xFF;
			return c;
			}
		c[7] = uart1_getc();
		if (c[7] == RX_DELIMITER) {
			c[7] = 0xFF;
			return c;
			}
		c[8] = uart1_getc();
		if (c[8] == RX_DELIMITER) {
			c[8] = 0xFF;
			return c;
			}
		c[9] = uart1_getc();
		if (c[9] == RX_DELIMITER) {
			c[9] = 0xFF;
			return c;
			}
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

	uint16_t *rxByteArray;
	rxByteArray = grabCommand();
	uart0_putc('C');
	for (int i = 0; rxByteArray[i] != 0x00FF; i++)
	{
		uart0_putc((char)(rxByteArray[i] & 0x00FF));
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
