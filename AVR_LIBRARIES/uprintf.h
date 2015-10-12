/* CREATED BY SARGIS S YONAN - 12 OCT. 2015 */
/* A LIBRRY THAT MIMICS <stdio.h>'s PRINTF FUNCTION /*
/* FOR THE AVR ATMEGA 2560 MICROCONTROLLER */


#include "uart.h"
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>


#define OUTPUT_FUNCTION uartPutString
#define STRING_MULTIPLIER 4

void uprintf (char* input_string, ...);
