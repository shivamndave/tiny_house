// fsm.h

/*
* WRITTEN BY SARGIS S YONAN ON 24 NOVEMBER 2015
* HEADER DECLARATIONS FOR A SENSOR DRIVER
* USED FOR A MICRO GRID TEST BED PROJECT
* GITHUB.COM/SARGISYONAN
*/

#define INITIAL_STATE 							IDLE_STATE
// FSM SPECS ///////
#define FSM_SUCCESS								true
#define FSM_ERROR 								false

/* ERROR MACRO DEFINITION */
#define PROGRAM_DIE() for(;;) \
{RX_TX_FUNCTION_puts("--FSM ERROR--");\
_RelayOff();}\

////////////////////
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
    uint8_t debugMode;      // if TRUE, the machine prints the current temp every itr.
};

typedef struct Machine_Status Status;
Status *status;	// allocated within ControllerInit()

/*              : setpoint        - contains the machines' current temperature setpoint
 *              : negativeOffset  - the deadband's lower limit = setpoint - negativeOffset
 *              : poitiveOffset   - the deadband's upper limit = setpoint + positiveOffset
 *              : current         - holds the current temperature
 */
struct TemperatureControl 
{
    float current;
    float setpoint;
    float negativeOffset;
    float positiveOffset;
};
typedef struct TemperatureControl Temperature;
Temperature *temperature;

