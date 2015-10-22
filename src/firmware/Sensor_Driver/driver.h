/*
*	WRITTEN BY SARGIS S YONAN ON 21 OCTOBER 2015
*	- A HEADER DEFINITION FOR A SENSOR DRIVER -
*	-	 USED FOR A MICRO GRID TEST BED		  -
*	-		  GITHUB.COM/SARGISYONAN		  -
*/

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

#include <avr/io.h>
#include <util/delay.h>


			///////////////
			// RELAY I/O //
			///////////////
// RELAY ON PB5 = PORTB & 0b10000000
#define PIN_STATUS_BIT 0x80
#define RELAY_PIN PINB
#define RELAY_STATUS (PINB & PIN_STATUS_BIT) 
#define RELAY_PORT PORTB
#define ON 0x80
#define OFF 0x00
/////////////////////////////////////////////


			//////////////////
			// SENSOR INPUT //
			//////////////////
#define SENSOR_IN temperatureFunction()
/////////////////////////////////////////////


98741




#define FSM_SUCCESS	true
#define FSM_ERROR false


#define NUMBER_OF_STATES 3

// STATE DEFINITIONS
#define IDLE 0
#define RELAY_OFF 1
#define RELAY_ON 2

#define IDLE_DELTA_T
#define RELAY_ON_DELTA_T
#define RELAY_OFF_DELTA_T


#define INITIAL_STATE IDLE

#define SUCCESS_t bool;

struct FSM_STRUCTURE
{
	void(*Output_Func_ptr)(void);	// OUTPUT FUNCTION
	uint32_t waitTime;		// SAFE WAIT TIME
	uint8_t nextState[NUMBER_OF_STATES];	// NEXT STATE LOGIC
};
typedef const struct FSM_STRUCTURE FSM_t:

struct Machine_Status
{
	uint8_t currentState;	// HOLDS CURRENT STATE
	FSM_t FSM[NUMBER_OF_STATES];
};

typedef struct Machine_Status Status;
Status *status = NULL;	// allocated within ControllerInit()

SUCCESS_t ControllerInit(void);
void FreeMemory(void);


void SensorInput(void);

void _RelayOn(void);
void _RelayOff(void);
void _Idle(void);




