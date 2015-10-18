//////////////////////////////////////////////////////
//     UART LIBRARY TO PROVIDE I/O FUNCTIONALITY    //
//   TO THE ATMEL AVR ATMEGA 2560 MICROCONTROLLER   //
// 	 WRITTEN BY: SARGIS S YONAN - 11 OCTOBER 2015   //
// CODE PARTIALLY BORROWED FROM THE ATMEL DATASHEET //
//////////////////////////////////////////////////////


#include "uart.h"

void uartInit(uint32_t baudrate)
{
	// Set uart baudrate register
	uint8_t ubrr  = ((F_CPU + baudrate * 8L) / (baudrate * 16L) - 1);
	UBRR0H = (unsigned int)(ubrr >> 8);
	UBRR0L = (unsigned int)(ubrr);

	// enable rx and tx for uart
	UCSR0B |= (1 << RXEN0) | (1 << TXEN0);

	// set mode asynch 8n1
	UCSR0C = (3 << UCSZ00);
}

unsigned char uartReceiveByte(void)
{
	while (!(UCSR0A & (1 << RXC0)));
	return UDR0;
}

void uartTransmitByte(unsigned char data)
{
	while (!(UCSR0A&(1 << UDRE0)));
	UDR0 = data;
}

// easy method for sending strings
void uartPutString(const char *str)
{
	unsigned char c;

	while ((c = *str++))
		uartTransmitByte(c);
}

// write data from programm memmory address
void uartPutString_P(const char *addr)
{
	unsigned char c;

	while((c = pgm_read_byte(addr++)))
		uartTransmitByte(c);
}


void UARTGrabString (char* stringBuffer, int maxStringSize)
{
    char c = NULL_CHAR;
    int i = 0;
    memset(stringBuffer, NULL_CHAR, maxStringSize);
    // RECEIVE A STRING FROM USART0 LOGIC
    for (i = 0; (c != NEWLINE_CHAR) && (i < maxStringSize); i++)
    {
        c = uartReceiveByte();

        // BACKSPACING LOGIC
        if (c == BACKSPACE_CHAR)
        {
            i--;
            if (i >= 0)
            {
                stringBuffer[i] = NULL_CHAR;
                if (!(i == 0))
                {
                    i--;
                }
            }
            continue;
        }
        // END BACKSPACING LOGIC
        stringBuffer[i] = c;
    }
    stringBuffer[i] = NULL_CHAR;
    // END STRING RECEIVING LOGIC
}


///* CREATED BY SARGIS S YONAN - 12 OCT. 2015 */
///* A FUNCTION THAT MIMICS <stdio.h>'s PRINTF FUNCTION */
///* FOR THE AVR ATMEGA 2560 MICROCONTROLLER */

void uprintf (char* input_string, ...)
{
    va_list valist;
    char* newString;
    int stringLength = 0;
    
    va_start(valist, input_string);
    
    for (stringLength = 0; input_string[stringLength] != '\0'; ++stringLength) { }
    
    newString = (char*)malloc(stringLength *  STRING_MULTIPLIER);
    vsprintf(newString, input_string, valist);
    
    ////////////////////////////
    // WRITING TO UART STREAM //
    ////////////////////////////
    OUTPUT_FUNCTION(newString);
    free(newString);
    va_end(valist);
}



