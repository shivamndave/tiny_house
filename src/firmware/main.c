/*
* A DRIVER BY SARGIS S YONAN ON 21 OCTOBER 2015
* USED FOR THE MICRO GRID TEST BED
* GITHUB.COM/SARGISYONAN -- SARGISY@GMAIL.COM
*/

#include "Sensor_Driver/driver.h"
#include "One_Wire_Library/OneWire.h"
#include "UART_LIBRARY/uart.h"

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
	//enable for tests
	status->flags ^= EN_BIT;
		while(true)
		{
			
			
			if (ProcessCommand() != PROCESS_COMMAND_ERROR){}
	/*
		
			PrintSystemStatusString();
			FSM[status->currentState].Output_Func_ptr();
			status->flags = SensorResult();
			status->currentState = FSM[status->currentState].nextState[status->flags];
	*/
		}
	}
	FreeMemory();
	PROGRAM_DIE();
}


	

// the whole received message is checksummed and stored as a
// 16-bit word before the null term char with the _array_checksum algorithm
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
	int i = 0;
	
	if (RX_TX_FUNCTION_peek() == UART_NO_DATA)
	{
		return;
	}

	rxByteArray = (uint16_t*)calloc(MAX_RECEIVE_LENGTH, (sizeof(uint16_t) * MAX_RECEIVE_LENGTH));
	
	if (rxByteArray != NULL)
	{
		for (i = 0; (i < MAX_RECEIVE_LENGTH) || (RX_TX_FUNCTION_peek() == UART_NO_DATA); i++)
		{
			rxByteArray[i] = RX_TX_FUNCTION_getc();
			if (rxByteArray[i] == '/')
			{
				rxByteArray[i] = '\0';
				break;
			}
		}
		for (int j = 0; rxByteArray[j] != '\0'; j++) {uart0_putc((uint8_t)rxByteArray[j]);}

		uprintf(RX_TX_FUNCTION_puts, "%d", _str_checksum(rxByteArray));


	//		uprintf(uart0_puts, "\nrxByteArray: %s", rxByteArray);
	//	uprintf(uart0_puts, "\n________\n");
	 	/*
		if (_str_checksum(rxByteArray) != rxByteArray[i - 1])
		{
			uprintf(RX_TX_FUNCTION_puts, "//%d//TRANSMISSION_ERROR//", TRANSMISSION_ERROR_CODE);
			return;
		}

		switch (rxByteArray[0] & EIGHT_BIT_OFFSET)
		{


			case ENABLE_STATE_MACHINE:
				status->flags |= EN_BIT;
				break;


			case DISABLE_STATE_MACHINE:
				status->flags &= EN_BIT;
				break;


			case RECEIVE_MESSAGE_CHANGE_SETPOINT:
				if (rxByteArray[1] < TEMPERATURE_MAX)
				{
					status->setpoint = rxByteArray[1] & EIGHT_BIT_OFFSET;
				} else {
					uprintf(RX_TX_FUNCTION_puts, "//%d//CHANGE_SETPOINT_ERROR//", CHANGE_SETPOINT_ERROR_CODE);
				}
				break;



			case RECEIVE_MESSAGE_CHANGE_POSITIVE_OFFSET:
				if (rxByteArray[1] < (uint16_t)POSITIVE_OFFSET_MAX)
				{
					status->positiveOffset = rxByteArray[1] & EIGHT_BIT_OFFSET;
				} else {
					uprintf(RX_TX_FUNCTION_puts, "//%d//CHANGE_POSITIVE_OFFSET_ERROR//", CHANGE_POSITIVE_OFFSET_ERROR_CODE);
				}
				break;


			case RECEIVE_MESSAGE_CHANGE_NEGATIVE_OFFSET:
				if (rxByteArray[1] < NEGATIVE_OFFSET_MAX)
				{
					status->negativeOffset = rxByteArray[1] & EIGHT_BIT_OFFSET;
				} else {
					uprintf(RX_TX_FUNCTION_puts, "//%d//CHANGE_NEGATIVE_OFFSET_ERROR//", CHANGE_NEGATIVE_OFFSET_ERROR_CODE);
				}
				break;


			case RECEIVE_MESSAGE_GET_SYSTEM_STATUS:
				uprintf(RX_TX_FUNCTION_puts, "//%.2f//%d//%d//%.2f//%.2f//%.2f//%d//", getTemperatureC(), status->currentState, status->sysTime, status->setpoint, status->positiveOffset, status->negativeOffset);
				break;


			default:
				uprintf(RX_TX_FUNCTION_puts, "//%d//UNKNOWN_ERROR//", UNKNOWN_ERROR_CODE);
				break;
		}

		*/


	} else {
		return PROCESS_COMMAND_ERROR;
	}
	return;
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
