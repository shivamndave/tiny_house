/*
* A DRIVER BY SARGIS S YONAN ON 21 OCTOBER 2015
* USED FOR THE MICRO GRID TEST BED
* GITHUB.COM/SARGISYONAN -- SARGISY@GMAIL.COM
*/

#include "One_Wire_Library/OneWire.h"
#include "UART_LIBRARY/uart.h"
#include "Sensor_Driver/driver.h"
#include "Sensor_Driver/fsm.h"


FSM_t FSM[] = 
{
	{&_Idle, {IDLE_STATE, IDLE_STATE, IDLE_STATE, IDLE_STATE, COOLING_STATE, COOLING_STATE, HEATING_STATE, IDLE_STATE}},
	{&_RelayOff, {IDLE_STATE, IDLE_STATE, IDLE_STATE, IDLE_STATE, IDLE_STATE, COOLING_STATE, HEATING_STATE, IDLE_STATE}},
	{&_RelayOn, {IDLE_STATE, IDLE_STATE, IDLE_STATE, IDLE_STATE, IDLE_STATE, COOLING_STATE, HEATING_STATE, IDLE_STATE}}
};


int main (void)
{
	if (SystemInit())	// DEFINED IN driver.h
	{
		while(true)		// embedded system; does not return from main()
		{
			status->currentTemp = getTemperatureC();
			if (RX_TX_FUNCTION_available() >= 1) ProcessCommand();								// if commands are in the receiving buffer
			FSM[status->currentState].Output_Func_ptr();								// executes proper state function
			status->flags = SensorResult();												// changes next state for the FSM
			status->currentState = FSM[status->currentState].nextState[status->flags];
		}
	}
	FreeMemory();
	PROGRAM_DIE(0);	// if (error)
}


