/* CREATED BY SARGIS S YONAN - 12 OCT. 2015 */
/* A LIBRRY THAT MIMICS <stdio.h>'s PRINTF FUNCTION /*
/* FOR THE AVR ATMEGA 2560 MICROCONTROLLER */


#include "uprintf.h"

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


