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
		//	ProcessCommand();
			PrintSystemStatusString();
			FSM[status->currentState].Output_Func_ptr();
			status->flags = SensorResult();
			status->currentState = FSM[status->currentState].nextState[status->flags];
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

void ProcessCommand(void)
{

	uint16_t rxByteArray[MAX_RECEIVE_LENGTH];
	int i = 0;

	if (RX_TX_FUNCTION_available() <= 0)
	{
		return;
	}

	for (i = 0; i < MAX_RECEIVE_LENGTH; i++)
	{
		rxByteArray[i] = (uint16_t)RX_TX_FUNCTION_getc();
		if ((rxByteArray[i] & EIGHT_BIT_OFFSET) == '\0')
		{
			break;
		}
	}
 
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
	RX_TX_FUNCTION_flush();
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
