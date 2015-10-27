/*
*	WRITTEN BY SARGIS S YONAN ON 21 OCTOBER 2015
*	- A HEADER DEFINITION FOR A SENSOR DRIVER -
*	-	 USED FOR A MICRO GRID TEST BED		  -
*	-		  GITHUB.COM/SARGISYONAN		  -
*/

#include "driver.h"
#include "../One_Wire_Library/OneWire.h"
#include "../UART_LIBRARY/uart.h"



#define INITIAL_STATE IDLE




int main (void)
{
	if (SystemInit())
	{
		while(true)
		{
			SensorInput();
		//	status->FSM[status->currentState].Output_Func_ptr();
		//	_ms_delay(status->FSM[status->currentState].waitTime);
		//	status->currentState = status->FSM[status->currentState].nextState[SensorInput()];
		}
	}
	return 0;
}




SUCCESS_t SystemInit(void)
{
	uartInit(BAUDRATE);
	if (RelayInit())
	{
		if (ControllerInit())
		{
			return FSM_SUCCESS;
		}
	}
	return FSM_ERROR;
}


SUCCESS_t RelayInit(void)
{
	RELAY_DDR = RELAY_STATUS_BIT;
	RELAY_PORT = OFF;
	return FSM_SUCCESS;
}

SUCCESS_t ControllerInit(void)
{
	status = (*Status)malloc(sizeof(struct Machine_Status));
	if (status != NULL)
	{
		// CONST STATE DEFINITION - CONST MEMORY KEPT IN ROM
		FSM =	
		{
			{&_Idle, IDLE_DELTA_T, {IDLE, IDLE, RELAY_OFF}},
			{&_RelayOff, RELAY_OFF_DELTA_T, {IDLE, RELAY_OFF, RELAY_ON}},
			{&_RelayOn, RELAY_ON_DELTA_T, {RELAY_ON, RELAY_OFF, IDLE}}
		};
		status->currentState = INITIAL_STATE;
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


void SensorInput(void)
{
	float temp = getTemperatureC();
	if (temp > HI)
	{
		_RelayOn();
	} else {
		_RelayOff();
	}
	return;
}


void _RelayOn(void)
{
	if (RELAY_PIN == OFF)
	{
		RELAY_PORT = ON;
	}
}
void _RelayOff(void)
{
	if (RELAY_PIN == ON)
	{
		RELAY_PORT = OFF;
	}
}
void _Idle(void)
{
	_RelayOff();
}




