/*
*	WRITTEN BY SARGIS S YONAN ON 21 OCTOBER 2015
*	- A HEADER DEFINITION FOR A SENSOR DRIVER -
*	-	 USED FOR A MICRO GRID TEST BED		  -
*	-		  GITHUB.COM/SARGISYONAN		  -
*/

#include "driver.h"
#include "../One_Wire_Library/OneWire.h"
#include "../UART_LIBRARY/uart.h"


#define INITIAL_STATE IDLE_STATE

void _RelayOn(void);
void _RelayOff(void);
void _Idle(void);

uint8_t _str_checksum(char *rxByteArray);
uint8_t _uint_checksum(uint8_t rxByte);

FSM_t FSM[] = {
		{&_Idle, {IDLE_STATE, IDLE_STATE, IDLE_STATE, IDLE_STATE, COOLING_STATE, COOLING_STATE, COOLING_STATE, COOLING_STATE}},
		{&_RelayOff, {IDLE_STATE, IDLE_STATE, IDLE_STATE, IDLE_STATE, COOLING_STATE, HEATING_STATE, COOLING_STATE, COOLING_STATE}},
		{&_RelayOn, {IDLE_STATE, IDLE_STATE, IDLE_STATE, IDLE_STATE, HEATING_STATE, HEATING_STATE, COOLING_STATE, HEATING_STATE}}
	};
		


int main (void)
{
	if (SystemInit())
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
	return FSM_ERROR;
}

void ProcessCommand(void)
{
	uint8_t rxByte = (uint8_t)uartReceiveByte();
	uint8_t receivedChecksum = (uint8_t)uartReceiveByte();
	char str[MAX_SEND_MESSAGE_LENGTH];


	if (!(receivedChecksum != _uint_checksum(rxByte)))
	{
		uartTransmitByte((unsigned char)RECEIVE_ERROR);
		return;
	}


	if ( (rxByte & ENABLE_MESSAGE) && (!(status->flags & EN_BIT)) )
	{
		status->flags ^= EN_BIT;
		return;
	}
	if ((rxByte & RECEIVE_MESSAGE_CHANGE_SETPOINT) == RECEIVE_MESSAGE_CHANGE_SETPOINT)
	{	if ((rxByte & ~(RECEIVE_MESSAGE_CHANGE_SETPOINT)) >= T_ABSOLUTE_MAX) 
		{
			uartTransmitByte((unsigned char)TEMPERATURE_MAX_ERROR);
			return;
		}
		status->setpoint = rxByte & (~(RECEIVE_MESSAGE_CHANGE_SETPOINT));
		return;
	}
	if ((rxByte & RECEIVE_MESSAGE_CHANGE_OFFSET) == RECEIVE_MESSAGE_CHANGE_OFFSET)
	{
		status->offset = rxByte & (!(RECEIVE_MESSAGE_CHANGE_OFFSET));
		return;
	}
	if ((rxByte & SEND_MESSAGE) == SEND_MESSAGE)
	{
		memset(str, '\0', MAX_SEND_MESSAGE_LENGTH);
		sprintf(str, "%.2f/%zu/%d/%.2f/%.2f",getTemperatureC(), (size_t)status->sysTime, status->currentState, status->setpoint, status->offset);
		uartPutString(str);	// send system status
		uartTransmitByte(_str_checksum(str));
		return;
	}
}

void PrintSystemStatusString(void)
{
	uprintf("\n---------SYSTEM STATUS---------\nTEMPERATURE (C): %.2f\nFLAGS: %d\nCURRENT STATE: %d\nSETPOINT: %.2f\nOFFSET: %.2f\n-------------------------------",getTemperatureC(), status->flags, status->currentState, status->setpoint, status->offset);
}

bool SystemInit(void)
{
	uartInit(BAUDRATE);
	RELAY_DDR = RELAY_DDR_SETTING;
	status = (Status*)malloc(sizeof(struct Machine_Status));
	if (status != NULL)
	{
		status->flags = 0x00;
		status->setpoint = T_SETPOINT_DEFAULT;
		status->offset = T_OFFSET_DEFAULT;
		status->currentState = INITIAL_STATE;
		status->initialized = true;
		status->sysTime = 0;
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
	status->flags &= 0x04;
	//1
	if (temp >= (status->setpoint + status->offset)) 
	{
			uprintf("\nSUB:1\n");
		return (status->flags ^ GT_BIT);
	}

	//2
	if (temp <= (status->setpoint - status->offset)) 
	{
			uprintf("\nSUB:2\n");

		return (status->flags ^ LT_BIT);
	}
//3
			uprintf("\nSUB:3\n");

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

uint8_t _str_checksum(char *rxByteArray)
{
	uint8_t checksum = 0;
	while (*rxByteArray != '\0') { checksum += (((*rxByteArray++) ^ 0xFF)); }
	checksum %= 0xFF;
	return checksum;
}

uint8_t _uint_checksum(uint8_t rxByte)
{
	return (rxByte ^ 0xFF);
}



