/*
* WRITTEN BY SARGIS S YONAN ON 21 OCTOBER 2015
* HEADER DECLARATIONS FOR A SENSOR DRIVER
* USED FOR A MICRO GRID TEST BED PROJECT
* GITHUB.COM/SARGISYONAN
*/


#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

#include <avr/io.h>
#include <util/delay.h>

#include "../RX_TX/commands.h"
#include "../RX_TX/xbee_lib.h"


#ifndef DRIVER_H
#define DRIVER_H

/*
//\\//\\//\\//\\//\\//\\//\\
///// DEFAULT SETTINGS /////
//\\//\\//\\//\\//\\//\\//\\
*/
#define T_SETPOINT_DEFAULT 						25.5
#define T_OFFSET_DEFAULT 						0.25

// WATER HEATER SPECS //
#define TEMPERATURE_MAX							48
#define POSITIVE_OFFSET_MAX						(50 - temperature->setpoint)
#define NEGATIVE_OFFSET_MAX						1

///////// RELAY I/O ////////
#define RELAY_DDR 								DDRB
#define RELAY_DDR_SETTING						0x80
#define PIN_STATUS_BIT 							0x80
#define RELAY_PIN 								PINB
#define RELAY_STATUS 							(PINB & PIN_STATUS_BIT) 
#define RELAY_PORT 								PORTB
#define ON 										0x80
#define OFF 				    				0x00


void uprintf (char* input_string, ...);

                        
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
 * DESCRIPTION : This function allows the system status (set point, offsets, and state) to be configured
 *               via a list of possible commands (above). This function requires an array of the form:
                 the whole received message is checksummed and stored as a
                 16-bit word before the null term uint16_t with the _array_checksum algorithm
                 the first 16-bits are the command, and the next bytes are the argument to that command
*
/ example of packet to send
/  Rx    = | 0xXX 8-BIT COMMAND | 8-BIT UPPER ARGUMENT | 8-BIT DELIMITER ('-') = 0x002D |
/  index =                0                                 1                              2                               3
/ THE COMMAND -> Rx = {{RECEIVE_MESSAGE_CHANGE_SETPOINT}, {0x2D}} = changes the set point to 100 degrees Celsius
*
* ------------
 * PARAMETERS  : void
 * ------------
 * RETURNS     : void
 * ==============
 */
// PROTOTYPE
void ProcessCommand(void);	// DEFINED IN ../main.c


// RELAY COMMANDS //
/* PRIVATE FUNCTIONS THAT DEFINE REAL-WORLD OUTPUT */
void _RelayOn(void);		// TURNS ON RELAY PIN
void _RelayOff(void);		// TURNS OFF RELAY PIN
////////////////////
// FSM SPECIFIC COMMANDS TO GET MAX OFFSET //
/////////////////////////////////////////////
uint8_t SensorResult(void);	// DETERMINES WHAT THE OUTPUT OF THE SENSOR MEANS TO THE SYSTEM
//void FreeMemory(void);	// FREES SYSTEM STRUCTURE MEMORY

#endif


