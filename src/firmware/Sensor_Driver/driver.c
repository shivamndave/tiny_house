/*
* WRITTEN BY SARGIS S YONAN ON 21 OCTOBER 2015
* HEADER DEFINITIONS FOR A SENSOR DRIVER
* USED FOR A MICRO GRID TEST BED
* GITHUB.COM/SARGISYONAN -- SARGISY@GMAIL.COM
*/

#define PROGRAM_DIE() return(0)

#include "driver.h"
#include "../One_Wire_Library/OneWire.h"
#include "../UART_LIBRARY/uart.h"

void _RelayOn(void);
void _RelayOff(void);
void _Idle(void);
uint16_t _array_checksum(uint16_t* array);
uint8_t _str_checksum(char *rxByteArray);
uint8_t _uint_checksum(uint8_t rxByte);

FSM_t FSM[] = {
		{&_Idle, {IDLE_STATE, IDLE_STATE, IDLE_STATE, IDLE_STATE, COOLING_STATE, COOLING_STATE, HEATING_STATE, IDLE_STATE}},
		{&_RelayOff, {IDLE_STATE, IDLE_STATE, IDLE_STATE, IDLE_STATE, IDLE_STATE, COOLING_STATE, HEATING_STATE, IDLE_STATE}},
		{&_RelayOn, {IDLE_STATE, IDLE_STATE, IDLE_STATE, IDLE_STATE, IDLE_STATE, COOLING_STATE, HEATING_STATE, IDLE_STATE}}
	};
		


int main (void)
{


	if (SystemInit())
	{
	//enable for tests
	status->flags ^= EN_BIT;
		while(true)
		{

		//	ProcessCommand();
			PrintSystemStatusString();
			FSM[status->currentState].Output_Func_ptr();
			status->flags = SensorResult();
			status->currentState = FSM[status->currentState].nextState[status->flags];
		}
	}
	FreeMemory();
	return FSM_ERROR;
}


// the whole received message is checksummed and stored as a
// 16-bit word before the null term char with the _array_checksum algorithm
// the first 16-bits are the command, and the next bytes are the argument to that command
/*
/ example of packet to send
/  rx    = | 0x0000[16 COMMAND BITS] | 8-BIT UPPER ARGUMENT + 8-BIT LOWER ARGUMENT | 16-BIT CHECKSUM | COMMAND_TERMINATION 16-BIT COMMAND (NUL) = 0x0000 |
/  index =                0                                 1                              2                               3
/ THE COMMAND -> rx = {{RECIEVE_MESSAGE_CHANGE_SETPOINT}, {0x0064}, CHECKSUM, {0x0000}} = change setpoint to 100 degrees celsius
*/

void ProcessCommand(void)
{
	
	uint16_t rxByteArray[MAX_RECEIVE_LENGTH];
	int i = 0;
	char *txArray;


	for (i = 0; i < MAX_RECEIVE_LENGTH; i++)
	{
		rxByteArray[i] = (uint16_t)RX_TX_FUNCTION_getc();
		if ((rxByteArray[i] & EIGHT_BIT_OFFSET) == '\0')
		{
			break;
		}
	}

	if (_str_checksum(rxByteArray) != rxByteArray[i - 1])
	{
		uprintf(RX_TX_FUNCTION_puts, "//%d//TRANSMISSION_ERROR//", TRANSMISSION_ERROR_CODE);
		return;
	}

	switch (rxByteArray[0] & EIGHT_BIT_OFFSET)
	{


		case ENABLE_STATE_MACHINE:
			status->flags |= EN_BIT;
			return;
			break;


		case DISABLE_STATE_MACHINE:
			status->flags &= EN_BIT;
			return;
			break;


		case RECEIVE_MESSAGE_CHANGE_SETPOINT:
			if (rxByteArray[1] < TEMPERATURE_MAX)
			{
				status->setpoint = rxByteArray[1] & EIGHT_BIT_OFFSET;
			} else {
				uprintf(RX_TX_FUNCTION_puts, "//%d//CHANGE_SETPOINT_ERROR//", CHANGE_SETPOINT_ERROR_CODE);
			}
			return;
			break;


		case RECEIVE_MESSAGE_CHANGE_POSITIVE_OFFSET:
			if (rxByteArray[1] < POSITIVE_OFFSET_MAX)
			{
				status->positiveOffset = rxByteArray[1] & EIGHT_BIT_OFFSET;
			} else {
				uprintf(RX_TX_FUNCTION_puts, "//%d//CHANGE_POSITIVE_OFFSET_ERROR//", CHANGE_POSITIVE_OFFSET_ERROR_CODE);
			}
			return;
			break;


		case RECEIVE_MESSAGE_CHANGE_NEGATIVE_OFFSET:
			if (rxByteArray[1] < NEGATIVE_OFFSET_MAX)
			{
				status->negativeOffset = rxByteArray[1] & EIGHT_BIT_OFFSET;
			} else {
				uprintf(RX_TX_FUNCTION_puts, "//%d//CHANGE_NEGATIVE_OFFSET_ERROR//", CHANGE_NEGATIVE_OFFSET_ERROR_CODE);
			}
			return;
			break;


		case RECEIVE_MESSAGE_GET_SYSTEM_STATUS:
			uprintf(RX_TX_FUNCTION_puts, "//%.2f//%d//%d//%.2f//%.2f//%.2f//%d//", getTemperatureC(), status->currentState, status->sysTime, status->setpoint, status->positiveOffset, status->negativeOffset);
			return;
			break;


		default:
			uprintf(RX_TX_FUNCTION_puts, "//%d//UNKNOWN_ERROR//", UNKNOWN_ERROR_CODE);
			return;
			break;


	}
}



void PrintSystemStatusString(void)
{
	static uint8_t lastState = 0;
	if (lastState == 0)
	{
		lastState = status->currentState;
	}
	if (lastState != status->currentState)
	{
		// print to debug channel
		uprintf(uart1_puts, "\n---------SYSTEM STATUS---------\nTEMPERATURE (C): %.2f\nFLAGS: %d\nCURRENT STATE: %d\nSETPOINT: %.2f\nOFFSET: %.2f\n-------------------------------",getTemperatureC(), status->flags, status->currentState, status->setpoint, status->positiveOffset);
		lastState = status->currentState;
	} else {
		uprintf(uart1_puts, "TEMPERATURE (C): %.2f\n",getTemperatureC());

	}
}

bool SystemInit(void)
{
//    uart0_init(UART_BAUD_SELECT(BAUDRATE,F_CPU)); 
   	uart1_init(UART_BAUD_SELECT(BAUDRATE,F_CPU));
    
    //enable interupts
    sei();	

	RELAY_DDR = RELAY_DDR_SETTING;
	status = (Status*)malloc(sizeof(struct Machine_Status));
	if (status != NULL)
	{
		status->flags = 0x00;
		status->setpoint = T_SETPOINT_DEFAULT;
		status->positiveOffset = T_OFFSET_DEFAULT;
		status->negativeOffset = T_OFFSET_DEFAULT;
		status->currentState = INITIAL_STATE;
		status->sysTime = 0;
		uprintf(RX_TX_FUNCTION_getc_puts, "//%d//SYSTEM INITIALIZED//", SYSTEM_INITIALIZED);

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


uint8_t SensorResult(void)
{
	float temp = getTemperatureC();
	if (temp >= (status->setpoint + status->positiveOffset)) 
	{
		status->flags &= 0x04;
		return (status->flags ^ GT_BIT);
	}
	if (temp <= (status->setpoint - status->negativeOffset)) 
	{
		status->flags &= 0x04;
		return (status->flags ^ LT_BIT);
	}
	return status->flags;
}


void _RelayOn(void)
{
	if (RELAY_PIN == OFF)
	{
		RELAY_PORT |= ON;
	}
}
void _RelayOff(void)
{
	if (RELAY_PIN == ON)
	{
		RELAY_PORT &= OFF;
	}
}
void _Idle(void)
{
	_RelayOff();
}

uint8_t _str_checksum(char *rxByteArray)
{
	uint8_t checksum = 0;
	while (*rxByteArray != '\0') { checksum += (((*rxByteArray++) ^ 0xFF)); }
	checksum %= 0x1F;
	return checksum;
}

uint8_t _uint_checksum(uint8_t rxByte)
{
	return (rxByte ^ 0xFF);
}


uint16_t _array_checksum(uint16_t* array)
{
	int i = 1;
	uint16_t checksum = 0;
	for (i = 1; array[i] != '\0'; i++)
	{
		checksum += (array[i] % 64);
		if (i == 5)
		{
			break;
		}
	}
	return (checksum ^ 0xFFFF);
}


float getPositiveOffsetMax(void)
{
	return (50 - status->setpoint);
}



// ISR()
ISR(TIM0_OVF_vect)           // every 10ms
{
	static systick = 0;
	if ((systick++) = 100)
	{
		systick = 0;
		status->systime++;
	}
}

