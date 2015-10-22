/*
*	WRITTEN BY SARGIS S YONAN ON 21 OCTOBER 2015
*	- A HEADER DEFINITION FOR A SENSOR DRIVER -
*	-	 USED FOR A MICRO GRID TEST BED		  -
*	-		  GITHUB.COM/SARGISYONAN		  -
*/

#include "driver.h"

#define INITIAL_STATE IDLE





int main (void)
{
	while(true)
	{
		status->FSM[status->currentState].Output_Func_ptr();
		_ms_delay(status->FSM[status->currentState].waitTime);
		status->currentState = status->FSM[status->currentState].nextState[SensorInput()];
	}

	return 0;
}




SUCCESS_t ControllerInit(void)
{
	status = (*Status)malloc(sizeof(struct Machine_Status));
	if (status)
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
	free(status); 
}


void SensorInput(void)
{
	return SENSOR_IN;
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




