
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