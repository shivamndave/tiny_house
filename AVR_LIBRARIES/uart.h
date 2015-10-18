#define F_CPU 16000000UL
#include <string.h>

#define NULL_CHAR '\0'
#define NEWLINE_CHAR '\n'
#define BACKSPACE_CHAR 0x08


#ifndef _UART_H_
#define _UART_H_

void uartInit(uint32_t);
unsigned char uartReceiveByte(void);
void uartTransmitByte(unsigned char);
void uartPutString(const char*);
void uartPutString_P(const char*);

/* @param:
	INPUT:	stringBuffer: a C style char* for a string
 maxStringSize: a maximum size for the char* buffer. At most = (int)sizeof(stringBuffer)
	RETURN: void; string from UART stored at the beginning of stringBuffer - '\0'
 */
//////////////////////////////////////////////////////
// 	 WRITTEN BY: SARGIS S YONAN - 18 OCTOBER 2015   //
//////////////////////////////////////////////////////

void UARTGrabString (char* stringBuffer, int maxStringSize);


#endif /* _UART_H_ */
