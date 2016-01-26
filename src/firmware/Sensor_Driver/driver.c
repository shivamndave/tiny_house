/*
* WRITTEN BY SARGIS S YONAN ON 21 OCTOBER 2015
* HEADER DEFINITIONS FOR A SENSOR DRIVER
* USED FOR A MICRO GRID TEST BED
* GITHUB.COM/SARGISYONAN -- SARGISY@GMAIL.COM
*/



#include "driver.h"
#include "fsm.h"
#include "../One_Wire_Library/OneWire.h"
#include "../UART_LIBRARY/uart.h"

static void uprintf (char* input_string, ...);


//* CREATED BY SARGIS S YONAN - 12 OCT. 2015 */
//* A FUNCTION THAT MIMICS <stdio.h>'s PRINTF FUNCTION */
//* FOR THE AVR MICROCONTROLLER */

void uprintf (char* input_string, ...)
{
    va_list valist;
    char* newString;
    uint8_t stringLength = 0;
    
    va_start(valist, input_string);
    
    for (stringLength = 0; input_string[stringLength] != '\0'; ++stringLength) {}
    
    newString = (char*)malloc(stringLength *  STRING_MULTIPLIER);
    vsprintf(newString, input_string, valist);
    
    // WRITING TO UART STREAM //
    
    RX_TX_FUNCTION_puts(newString);
    free(newString);
    va_end(valist);
}




bool SystemInit(void)
{
    sei();				/* enables AVR interrupts */
   	RX_TX_FUNCTION_init(UART_BAUD_SELECT(BAUDRATE,F_CPU));
	RELAY_DDR = RELAY_DDR_SETTING;
	status = (Status*)malloc(sizeof(struct Machine_Status));
	if (status != NULL)
	{
		status->flags = 0x00;
		status->currentState = INITIAL_STATE;
		temperature = (Temperature*)malloc(sizeof(struct TemperatureControl));
		if (temperature != NULL)
		{	
			temperature->setpoint = T_SETPOINT_DEFAULT;
			temperature->positiveOffset = T_OFFSET_DEFAULT;
			temperature->negativeOffset = T_OFFSET_DEFAULT;
			uprintf("/%d/", SYSTEM_INITIALIZED);
			return FSM_SUCCESS;
		}
	}
	return FSM_ERROR;
}




// the whole received message is checksummed and stored as a
// 16-bit word before the null term uint16_t with the _array_checksum algorithm
// the first 16-bits are the command, and the next bytes are the argument to that command
/*
/ example of packet to send
/  Rx    = | 0xXX 8-BIT COMMAND | 8-BIT UPPER ARGUMENT | 8-BIT DELIMITER ('-') = 0x002D |
/  index =                0                                 1                              2                               3
/ THE COMMAND -> Rx = {{RECEIVE_MESSAGE_CHANGE_SETPOINT}, {0x2D}} = changes the set point to 100 degrees Celsius
*/

void ProcessCommand(void)
{
	uint8_t rxByteArray[MAX_RECEIVE_LENGTH];
	
	for(int i = 0; i < MAX_RECEIVE_LENGTH; i++)
	{
		if (RX_TX_FUNCTION_available() < 1)
		{
			_delay_ms(XBEE_CHAR_MS_TIMEOUT);
		}
		rxByteArray[i] = RX_TX_FUNCTION_getc();
		if (rxByteArray[i] == RX_DELIMITER) 
		{
			rxByteArray[i] = '\0';
			break;
		}
	}
	/* TEST THIS */
	if (rxByteArray[MAX_RECEIVE_LENGTH - 1] != RX_DELIMITER) 
	{

		RX_TX_FUNCTION_flush();
		return;
	}


	switch (rxByteArray[0])
	{
		case ENABLE_STATE_MACHINE:
			uprintf("%d", STATE_MACHINE_ENABLE_SUCCESS);
			status->flags |= EN_BIT;
			break;


		case DISABLE_STATE_MACHINE:
			uprintf("%d", STATE_MACHINE_DISABLE_SUCCESS);
			status->flags &= ~(EN_BIT);
			break;


		case RECEIVE_MESSAGE_CHANGE_SETPOINT:
			if (rxByteArray[1] < TEMPERATURE_MAX)
			{
				temperature->setpoint = rxByteArray[1];
				uprintf("%d", CHANGE_SETPOINT_SUCCESS_CODE);
			} else {
				uprintf("%d", CHANGE_SETPOINT_ERROR_CODE);
			}
			break;


		case RECEIVE_MESSAGE_CHANGE_POSITIVE_OFFSET:
			if (rxByteArray[1] < POSITIVE_OFFSET_MAX)
			{
				temperature->positiveOffset = rxByteArray[1];
				uprintf("%d", CHANGE_POSITIVE_OFFSET_SUCCESS_CODE);
			} else {
				uprintf("%d", CHANGE_POSITIVE_OFFSET_ERROR_CODE);
			}
			break;


		case RECEIVE_MESSAGE_CHANGE_NEGATIVE_OFFSET:
			if (rxByteArray[1] < NEGATIVE_OFFSET_MAX)
			{
				temperature->negativeOffset = rxByteArray[1];
				uprintf("%d", CHANGE_NEGATIVE_OFFSET_SUCCESS_CODE);
			} else {
				uprintf("%d", CHANGE_NEGATIVE_OFFSET_ERROR_CODE);
			}
			break;


		case RECEIVE_MESSAGE_GET_SYSTEM_STATUS:
			uprintf("/%.2f/%d/%.2f/%.2f/%.2f/%d/", temperature->current, status->currentState, temperature->setpoint, temperature->positiveOffset, temperature->negativeOffset);
			break;

		case RECEIVE_DEBUG_SWITCH_COMMAND:
			if (status->debugMode == DEBUG_ON)
			{
				status->debugMode = DEBUG_OFF;
			} else {
				status->debugMode = DEBUG_ON;
			}	
			break;
			
		default:
			uprintf("%d", INVALID_COMMAND_ERROR_CODE);
			break;
	}

	RX_TX_FUNCTION_flush();
}
/*
void FreeMemory(void)
{
	if (!(status == NULL)) free(status);
	if (!(temperature == NULL)) free(temperature);
}
*/


uint8_t SensorResult(void)
{
	if ((status->flags & EN_BIT) != EN_BIT) return (status->flags &= 0); // not enabled

	if (temperature->current >= (temperature->setpoint + temperature->positiveOffset)) 
	{
		status->flags &= 0x04;
		return (status->flags ^ GT_BIT);
	}
	if (temperature->current <= (temperature->setpoint - temperature->negativeOffset)) 
	{
		status->flags &= 0x04;
		return (status->flags ^ LT_BIT);
	}
	return status->flags;
}

void _RelayOn(void)
{
	if (RELAY_PIN == OFF) RELAY_PORT |= ON;
}
void _RelayOff(void)
{
	if (RELAY_PIN == ON) RELAY_PORT &= OFF;
}