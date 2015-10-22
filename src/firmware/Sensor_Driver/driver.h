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


#define NUMBER_OF_STATES 3

// STATE DEFINITIONS
#define IDLE 0
#define RELAY_ON 1
#define RELAY_OFF 2


#define SUCCESS_t bool;

struct FSM_STRUCTURE
{
	void(*func_ptr)(void);	// OUTPUT FUNCTION
	uint32_t waitTime;		// SAFE WAIT TIME
	uint8_t nextState[NUMBER_OF_STATES];	// NEXT STATE LOGIC
};

typedef const struct FSM_STRUCTURE FSM_t:
// CONST STATE DEFINITION - CONST MEMORY KEPT IN ROM
FSM_t FSM[NUMBER_OF_STATES]	=	
{
	{},
	{},
	{}
};

struct Status
{
	uint8_t currentState;	// HOLDS CURRENT STATE
	uint32_t input;			// RECIEVES INPUT
};

typedef struct Status Status;
Status *status = NULL;	// allocated within ControllerInit()

SUCCESS_t ControllerInit(void);
void _RelayOn(void);
void _RelayOff(void);
void _Idle(void);




