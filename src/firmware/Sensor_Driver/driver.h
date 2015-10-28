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
#define RELAY_DDR DDRB
#define PIN_STATUS_BIT 0x80
#define RELAY_PIN PINB
#define RELAY_STATUS (PINB & PIN_STATUS_BIT) 
#define RELAY_PORT PORTB
#define ON 0x80
#define OFF 0x00
/////////////////////////////////////////////
					//
			//////////////////
			// SENSOR INPUT //
			//////////////////
                    //
/////////////////////////////////////////////


#define FSM_SUCCESS	true
#define FSM_ERROR false


#define T_SETPOINT_DEFAULT 30
#define T_OFFSSET_DEFAULT 2

#define T_ABSOLUTE_MAX 120 //farenhe.


#define LT_BIT 0x01
#define GT_BIT 0x02
#define EN_BIT 0x04

#define ENABLE_MESSAGE 0x0F
#define RECEIVE_MESSAGE_CHANGE_SETPOINT 0x80
#define RECEIVE_MESSAGE_CHANGE_OFFSET 0x40
#define SEND_MESSAGE 0x20
#define MAX_SEND_MESSAGE_LENGTH 32

// STATE DEFINITIONS
#define IDLE_STATE 0
#define COOLING_STATE 1
#define HEATING_STATE 2

struct FSM_STRUCTURE
{
	void(*Output_Func_ptr)(void);	// OUTPUT FUNCTION
	uint8_t nextState[8];	// NEXT STATE LOGIC
};
typedef const struct FSM_STRUCTURE FSM_t;


struct Machine_Status
{
	uint8_t currentState;	// HOLDS CURRENT STATE
	uint8_t setpoint;
	uint8_t offset;
	uint8_t flags;
};

typedef struct Machine_Status Status;
Status *status = NULL;	// allocated within ControllerInit()



bool SystemInit(void);
void FreeMemory(void);
uint8_t SensorResult(void);





