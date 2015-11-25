/*
* WRITTEN BY SARGIS S YONAN ON 21 OCTOBER 2015
* HEADER DECLARATIONS FOR A SENSOR DRIVER
* USED FOR A MICRO GRID TEST BED PROJECT
* GITHUB.COM/SARGISYONAN
*/


#include "commands.h"
#include "xbee_lib.h"
#include "FSM/fsm.h"
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

#include <avr/io.h>
#include <util/delay.h>


#ifndef DRIVER_H
#define DRIVER_H

/* ERROR MACRO DEFINITION */
#define PROGRAM_DIE(x) while (1) \
{RX_TX_FUNCTION_puts("--FSM ERROR--");\
status->currentState = IDLE_STATE;\
_RelayOff();}\
return(x)\


/*
//\\//\\//\\//\\//\\//\\//\\
///// DEFAULT SETTINGS /////
//\\//\\//\\//\\//\\//\\//\\
*/
#define T_SETPOINT_DEFAULT 						25.5
#define T_OFFSET_DEFAULT 						0.25

// WATER HEATER SPECS //
#define TEMPERATURE_MAX							48.8889
#define POSITIVE_OFFSET_MAX						_getPositiveOffsetMax()
#define NEGATIVE_OFFSET_MAX						1

///////// RELAY I/O ////////
#define RELAY_DDR 								DDRB
#define RELAY_DDR_SETTING						0x80
#define PIN_STATUS_BIT 							0x80
#define RELAY_PIN 								PINB
#define RELAY_STATUS 							(PINB & PIN_STATUS_BIT) 
#define RELAY_PORT 								PORTB
#define ON 										0x80
#define OFF 									0x00




/** 
 * ==============
 * NAME        : SystemInit -- System Initialization
 * ------------
 * DESCRIPTION : This function initialized the system, by initializing the uart with a BAUDRATE, ISRs, 
 *               setting up directional DDR pins, allocating memory for the system structure, and initializing 
 *               the structure members. The system then send a success message
 * ------------
 * PARAMETERS  : void
 * ------------
 * RETURNS     : true if system initialized correctly; else, false
 * ==============
 */
// PROTOTYPE
bool SystemInit(void);


/** 
 * ==============
 * NAME        : ProcessCommand -- Processes RX & TX Commands 
 * ------------
 * DESCRIPTION : This function allows the system status (setpoint, offsets, and state) to be configured
 *               via a list of possible commands (above). This function requires an array of the form:
 *               | 16-BIT COMMAND | 16-BIT ARGUMENT | 16-BIT CHECKSUM | 0x0000 |    =
 *                       0                1                 2             3         =
 *               {{RECEIVE_MESSAGE_CHANGE_SETPOINT}, {0x0064}, CHECKSUM, {0x0000}} = change setpoint to 100 degrees celsius
 * ------------
 * PARAMETERS  : void
 * ------------
 * RETURNS     : void
 * ==============
 */
// PROTOTYPE
uint16_t ProcessCommand(void);	// DEFINED IN ../main.c


void FreeMemory(void);	// FREES SYSTEM STRUCTURE MEMORY
uint8_t SensorResult(void);	// DETERMINES WHAT THE OUTPUT OF THE SENSOR MEANS TO THE SYSTEM
void PrintSystemStatusString(void);	// DEBUGGING PRINT FUNCTION

float _getPositiveOffsetMax(void);	// PRIVATE FUNCTION TO DETERMINE +OFFSET

/* PRIVATE FUNCTIONS THAT DEFINE REAL-WORLD OUTPUT */
void _RelayOn(void);		// TURNS ON RELAY PIN
void _RelayOff(void);		// TURNS OFF RELAY PIN
void _Idle(void);			// AN EMPTY NO-OP
uint16_t _str_checksum(uint16_t *rxByteArray);	// CALCULATES THE CHECKSUM OF AN ARRAY

#endif

