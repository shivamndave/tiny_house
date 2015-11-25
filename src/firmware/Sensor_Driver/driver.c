/*
* WRITTEN BY SARGIS S YONAN ON 21 OCTOBER 2015
* HEADER DEFINITIONS FOR A SENSOR DRIVER
* USED FOR A MICRO GRID TEST BED
* GITHUB.COM/SARGISYONAN -- SARGISY@GMAIL.COM
*/



#include "driver.h"
#include "fsm.h"
#include "../One_Wire_Library/OneWire.h"
#include "../UART_LIBRARY/uart.h"





bool SystemInit(void)
{
	//enable interupts
    sei();	
   	uart1_init(UART_BAUD_SELECT(BAUDRATE,F_CPU));
    uart0_init(UART_BAUD_SELECT(BAUDRATE,F_CPU));

	RELAY_DDR = RELAY_DDR_SETTING;
	status = (Status*)malloc(sizeof(struct Machine_Status));
	if (status != NULL)
	{
		status->flags = 0x00;
		status->setpoint = T_SETPOINT_DEFAULT;
		status->positiveOffset = T_OFFSET_DEFAULT;
		status->negativeOffset = T_OFFSET_DEFAULT;
		status->currentState = INITIAL_STATE;
		uprintf(RX_TX_FUNCTION_puts, "//%d//SYSTEM INITIALIZED//", SYSTEM_INITIALIZED);

		return FSM_SUCCESS;
	}
	return FSM_ERROR;
}




// the whole received message is checksummed and stored as a
// 16-bit word before the null term uint16_t with the _array_checksum algorithm
// the first 16-bits are the command, and the next bytes are the argument to that command
/*
/ example of packet to send
/  rx    = | 0x0000[16 COMMAND BITS] | 8-BIT UPPER ARGUMENT + 8-BIT LOWER ARGUMENT | 16-BIT CHECKSUM | COMMAND_TERMINATION 16-BIT COMMAND ('-') = 0x002D |
/  index =                0                                 1                              2                               3
/ THE COMMAND -> rx = {{RECEIVE_MESSAGE_CHANGE_SETPOINT}, {0x0064}, CHECKSUM, {0x0000}} = change setpoint to 100 degrees celsius
*/

uint16_t ProcessCommand(void)
{
	uint8_t rxByteArray[MAX_RECEIVE_LENGTH];
	int i = 0;

	for(i = 0; i < MAX_RECEIVE_LENGTH; i++){
		if (uart1_available() < 1){
			_delay_ms (XBEE_CHAR_MS_TIMEOUT);
		}
	
		rxByteArray[i] = uart1_getc();
		if (rxByteArray[i] == RX_DELIMITER) 
		{
			rxByteArray[i] = '\0';
			break;
		}
	}
	uprintf(uart0_puts, "rxByteArray: %s\n", rxByteArray);
//	if (rxByteArray[i - 1] != _str_checksum(rxByteArray)) return CORRUPT_PACKET_TRANSMISSION;

		switch (rxByteArray[0] & EIGHT_BIT_OFFSET)
		{


			case ENABLE_STATE_MACHINE:
				uart0_putc('E');
				uprintf(RX_TX_FUNCTION_puts, "ENABLED\n");
				status->flags |= EN_BIT;
				break;


			case DISABLE_STATE_MACHINE:
				uart0_putc('O');
				uprintf(RX_TX_FUNCTION_puts, "DISABLED\n");
				status->flags &= ~(EN_BIT);
				break;


			case RECEIVE_MESSAGE_CHANGE_SETPOINT:
				uart0_putc('S');
				uprintf(RX_TX_FUNCTION_puts, "CHANGE SETPOINT\n");
				if (rxByteArray[1] < TEMPERATURE_MAX)
				{
					status->setpoint = rxByteArray[1] & EIGHT_BIT_OFFSET;
					uprintf(RX_TX_FUNCTION_puts, "new setpoint: %f\n", status->setpoint);
				}
				 else {
					uprintf(RX_TX_FUNCTION_puts, "//%d//CHANGE_SETPOINT_ERROR//", CHANGE_SETPOINT_ERROR_CODE);
				}
				break;



			case RECEIVE_MESSAGE_CHANGE_POSITIVE_OFFSET:
				uart0_putc('P');
				uprintf(RX_TX_FUNCTION_puts, "CHANGE POSITIVE OFFSET\n");
				if (rxByteArray[1] < POSITIVE_OFFSET_MAX)
				{
					status->positiveOffset = rxByteArray[1] & EIGHT_BIT_OFFSET;
				} else {
					uprintf(RX_TX_FUNCTION_puts, "//%d//CHANGE_POSITIVE_OFFSET_ERROR//", CHANGE_POSITIVE_OFFSET_ERROR_CODE);
				}
				break;


			case RECEIVE_MESSAGE_CHANGE_NEGATIVE_OFFSET:
				uart0_putc('C');
				uprintf(RX_TX_FUNCTION_puts, "CHANGE NEGATIVE OFFSET\n");
				if (rxByteArray[1] < NEGATIVE_OFFSET_MAX)
				{
					status->negativeOffset = rxByteArray[1] & EIGHT_BIT_OFFSET;
				} else {
					uprintf(RX_TX_FUNCTION_puts, "//%d//CHANGE_NEGATIVE_OFFSET_ERROR//", CHANGE_NEGATIVE_OFFSET_ERROR_CODE);
				}
				break;


			case RECEIVE_MESSAGE_GET_SYSTEM_STATUS:
				uart0_putc('G');
				uprintf(RX_TX_FUNCTION_puts, "GET SYSTEM STATUS\n");
				uprintf(RX_TX_FUNCTION_puts, "//%.2f//%d//%.2f//%.2f//%.2f//%d//", status->currentTemp, status->currentState, status->setpoint, status->positiveOffset, status->negativeOffset);
				break;


			default:
				uart0_putc('D');
				uprintf(RX_TX_FUNCTION_puts, "//%d//UNKNOWN_ERROR//", UNKNOWN_ERROR_CODE);
				break;
		}

	uart0_flush();
	uart1_flush();
	return PROCESS_COMMAND_SUCCESS;
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
		uprintf(uart1_puts, "\n---SYSTEM STATUS---\nTEMPERATURE (C): %.2f\nFLAGS: %d\nCURRENT STATE: %d\nSETPOINT: %.2f\nOFFSET: %.2f\n-------------------------------",status->currentTemp, status->flags, status->currentState, status->setpoint, status->positiveOffset);
		uprintf(uart0_puts, "%.2f\n",status->currentTemp);
		lastState = status->currentState;
	} else {
		uprintf(uart1_puts, "%.2f\n",status->currentTemp);
		uprintf(uart0_puts, "%.2f\n",status->currentTemp);

	}
}

void FreeMemory(void)
{
	if (!(status == NULL)) free(status);
}


uint8_t SensorResult(void)
{
	if ((status->flags & EN_BIT) != EN_BIT) return (status->flags &= 0); // not enabled

	if (status->currentTemp >= (status->setpoint + status->positiveOffset)) 
	{
		status->flags &= 0x04;
		return (status->flags ^ GT_BIT);
	}
	if (status->currentTemp <= (status->setpoint - status->negativeOffset)) 
	{
		status->flags &= 0x04;
		return (status->flags ^ LT_BIT);
	}
	return status->flags;
}


void _RelayOn(void)
{
	if (RELAY_PIN == OFF) RELAY_PORT |= ON;
}
void _RelayOff(void)
{
	if (RELAY_PIN == ON) RELAY_PORT &= OFF;
}
void _Idle(void)
{
	_RelayOff();
}

uint16_t _str_checksum(uint16_t *rxByteArray)
{
	uint8_t checksum = 0;
	while (*rxByteArray != RX_DELIMITER) { checksum += (((*rxByteArray++) ^ 0xFF)); }
	checksum %= 0x1F;
	return checksum;
}

float _getPositiveOffsetMax(void)
{
	return (50 - status->setpoint);
}
