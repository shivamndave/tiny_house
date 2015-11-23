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



#ifndef DRIVER_H
#define DRIVER_H


#define PROGRAM_DIE() return(0)

// FSM SPECS //
#define INITIAL_STATE 							IDLE_STATE

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


/*
// RX CODES \\
*/
#define RX_DELIMITER 							0x2D
#define XBEE_CHAR_MS_TIMEOUT					5
#define RX_TX_FUNCTION_getc 					uart1_getc
#define RX_TX_FUNCTION_puts						uart1_puts
#define RX_TX_FUNCTION_flush					uart1_flush
#define RX_TX_FUNCTION_peek						uart1_peek
#define RX_TX_FUNCTION_available				uart1_available
#define BAUDRATE								19200

#define EIGHT_BIT_OFFSET						0x00FF

#define MAX_RECEIVE_LENGTH						10
#define MIN_RECEIVE_LENGTH						3



// SUCCESS/ERROR CODES //

#define PROCESS_COMMAND_SUCCESS					0x0001
#define CORRUPT_PACKET_TRANSMISSION				0x00FA
#define PROCESS_COMMAND_ERROR					0x0000
#define CHANGE_SETPOINT_ERROR_CODE				0x00F0
#define TRANSMISSION_ERROR_CODE					0x00F1	// sent if message was too large or too small in length
#define CHANGE_POSITIVE_OFFSET_ERROR_CODE		0x00F2
#define CHANGE_NEGATIVE_OFFSET_ERROR_CODE		0x00F4
#define UNKNOWN_ERROR_CODE						0x00F8

/////////////////////////

// RX/TX COMMANDS //

#define SYSTEM_INITIALIZED						0x0011
#define ENABLE_STATE_MACHINE					0x00FF
#define DISABLE_STATE_MACHINE					0x0000
#define RECEIVE_MESSAGE_CHANGE_SETPOINT			0x0001
#define RECEIVE_MESSAGE_CHANGE_POSITIVE_OFFSET	0x0002
#define RECEIVE_MESSAGE_CHANGE_NEGATIVE_OFFSET	0x0004
#define RECEIVE_MESSAGE_GET_SYSTEM_STATUS		0x0008

////////////////////

///////// RELAY I/O ////////
#define RELAY_DDR 								DDRB
#define RELAY_DDR_SETTING						0x80
#define PIN_STATUS_BIT 							0x80
#define RELAY_PIN 								PINB
#define RELAY_STATUS 							(PINB & PIN_STATUS_BIT) 
#define RELAY_PORT 								PORTB
#define ON 										0x80
#define OFF 									0x00


#define FSM_SUCCESS								true
#define FSM_ERROR 								false


#define GT_BIT 									0x01		
#define LT_BIT 									0x02		
#define EN_BIT 									0x04

// STATE DEFINITIONS
#define IDLE_STATE 								0
#define COOLING_STATE 							1
#define HEATING_STATE 							2



/** 
 * ==============
 * NAME         : FSM_STRUCTURE -- A Complex Data Type that contains the status machine states and output functions
 * -------------
 * MEMBERS      : void (*Output_Func_ptr)(void)   - the output function pointer with void arguments for the current state
 *              : nextState[8]                    - states for the FSM - 8 * NUMBER_OF_STATES instances
 *               
 * -------------
 * TYPE-DEFINED : FSM_t - a const struct FSM_STRUCTURE
 * ------------
 * GLOBAL-ID    : FSM[] - declared and initialized in driver.c
 * ==============
 */
struct FSM_STRUCTURE
{
	void(*Output_Func_ptr)(void);	// OUTPUT FUNCTION
	uint8_t nextState[8];	// NEXT STATE LOGIC
};
typedef const struct FSM_STRUCTURE FSM_t;



/** 
 * ==============
 * NAME         : Machine_Status Structure -- A Complex Data Type that contains the machine status variables
 * -------------
 * MEMBERS      : currentState    - holds the current state of the FSM
 *              : flags           - holds the flags set by the sensor drivers
 *              : sysTime         - holds a 32-bit counter of seconds past since system initialization
 *              : setpoint        - contains the machines' current temperature setpoint
 *              : negativeOffset  - the deadband's lower limit = setpoint - negativeOffset
 *              : poitiveOffset   - the deadband's upper limit = setpoint + positiveOffset
 *               
 * -------------
 * TYPE-DEFINED : Status
 * ------------
 * GLOBAL-ID    : status - a pointer to a type: Status - a pointer to a type: struct Machine_Status
 * ==============
 */
struct Machine_Status
{
	uint8_t currentState;	// HOLDS CURRENT STATE
	uint8_t flags;
	uint32_t sysTime;
	float setpoint;
	float negativeOffset;
	float positiveOffset;
};

typedef struct Machine_Status Status;
Status *status;	// allocated within ControllerInit()

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

