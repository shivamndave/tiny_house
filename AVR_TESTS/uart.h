#define F_CPU 16000000UL

#ifndef _UART_H_
#define _UART_H_

void uartInit(uint32_t);
unsigned char uartReceiveByte(void);
void uartTransmitByte(unsigned char);
void uartPutString(const char*);
void uartPutString_P(const char*);

#endif /* _UART_H_ */
