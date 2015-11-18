/*
* WRITTEN BY SARGIS S YONAN ON 21 OCTOBER 2015
* HEADER DEFINITIONS FOR A SENSOR DRIVER
* USED FOR A MICRO GRID TEST BED
* GITHUB.COM/SARGISYONAN -- SARGISY@GMAIL.COM
*/


#include "driver.h"
#include "../One_Wire_Library/OneWire.h"
#include "../UART_LIBRARY/uart.h"




void PrintSystemStatusString(void)
{
	static uint8_t lastState = 0;
	if (lastState == 0)
	{
		lastState = status->currentState;
	}
	if (lastState != status->currentState)
	{
		// print to debug channel
		uprintf(uart1_puts, "\n---SYSTEM STATUS---\nTEMPERATURE (C): %.2f\nFLAGS: %d\nCURRENT STATE: %d\nSETPOINT: %.2f\nOFFSET: %.2f\n-------------------------------",getTemperatureC(), status->flags, status->currentState, status->setpoint, status->positiveOffset);
		uprintf(uart0_puts, "%.2f\n",getTemperatureC());
		lastState = status->currentState;
	} else {
		uprintf(uart1_puts, "%.2f\n",getTemperatureC());
		uprintf(uart0_puts, "%.2f\n",getTemperatureC());

	}
}

bool SystemInit(void)
{
//    uart0_init(UART_BAUD_SELECT(BAUDRATE,F_CPU)); 
   	uart1_init(UART_BAUD_SELECT(BAUDRATE,F_CPU));
    uart0_init(UART_BAUD_SELECT(BAUDRATE,F_CPU));
    //enable interupts
    sei();	

	RELAY_DDR = RELAY_DDR_SETTING;
	status = (Status*)malloc(sizeof(struct Machine_Status));
	if (status != NULL)
	{
		status->flags = 0x00;
		status->setpoint = T_SETPOINT_DEFAULT;
		status->positiveOffset = T_OFFSET_DEFAULT;
		status->negativeOffset = T_OFFSET_DEFAULT;
		status->currentState = INITIAL_STATE;
		status->sysTime = 0;
		uprintf(RX_TX_FUNCTION_puts, "//%d//SYSTEM INITIALIZED//", SYSTEM_INITIALIZED);

		return FSM_SUCCESS;
	}
	return FSM_ERROR;
}

void FreeMemory(void)
{
	if (!(status == NULL))
	{
		free(status); 
	}
}


uint8_t SensorResult(void)
{
	float temp = getTemperatureC();
	if (temp >= (status->setpoint + status->positiveOffset)) 
	{
		status->flags &= 0x04;
		return (status->flags ^ GT_BIT);
	}
	if (temp <= (status->setpoint - status->negativeOffset)) 
	{
		status->flags &= 0x04;
		return (status->flags ^ LT_BIT);
	}
	return status->flags;
}


void _RelayOn(void)
{
	if (RELAY_PIN == OFF)
	{
		RELAY_PORT |= ON;
	}
}
void _RelayOff(void)
{
	if (RELAY_PIN == ON)
	{
		RELAY_PORT &= OFF;
	}
}
void _Idle(void)
{
	_RelayOff();
}

uint16_t _str_checksum(uint16_t *rxByteArray)
{
	uint8_t checksum = 0;
	while (*rxByteArray != '\0') { checksum += (((*rxByteArray++) ^ 0xFF)); }
	checksum %= 0x1F;
	return checksum;
}

float _getPositiveOffsetMax(void)
{
	return (50 - status->setpoint);
}
