/*
* WRITTEN BY SARGIS S YONAN ON 24 NOVEMBER 2015
* HEADER DECLARATIONS FOR AN XBEE RADIO
* USED FOR A MICRO GRID TEST BED PROJECT
* GITHUB.COM/SARGISYONAN
*/

// XBEE SPECIFIC DELIMITER, DELAY, BAUDRATE, FUNCTIONS, LENGTHS


#define RX_DELIMITER 							0x2D
#define XBEE_CHAR_MS_TIMEOUT					5
#define RX_TX_FUNCTION_getc 					uart1_getc
#define RX_TX_FUNCTION_init						uart1_init
#define RX_TX_FUNCTION_puts						uart1_puts
#define RX_TX_FUNCTION_flush					uart1_flush
#define RX_TX_FUNCTION_peek						uart1_peek
#define RX_TX_FUNCTION_available				uart1_available
#define BAUDRATE								19200

#define EIGHT_BIT_OFFSET						0x00FF

#define MAX_RECEIVE_LENGTH						10
#define MIN_RECEIVE_LENGTH						3
