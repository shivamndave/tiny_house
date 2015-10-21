AVR_GCC_CONTROLLER           = atmega2560
BURNER						 = avrdude
AVRDUDE_CONTROLLER 	         = m2560
CC							 = avr-gcc
CSOURCE						 = One_Wire_Library/OneWire.c UART_Library/uart.c UART_Library/uprintftest.c
#${CSOURCE:.c=.o}
OBJECTS  					 = OneWire.o uart.o	uprintftest.o
EXECUTABLE					 = uartTestEx
COMPILER_PATH				 = /dev/cu.usbmodemfa131
COMPILER  					 = stk500v2
BAUDRATE					 = 115200
OBJECT_COPY					 = avr-objcopy
CFLAGS						 = -Wall -Wextra -Wimplicit -std=gnu99
PHASE_ONE					 = -Os -mmcu=${AVR_GCC_CONTROLLER} ${CFLAGS} -c
PHASE_TWO					 = -mmcu=${AVR_GCC_CONTROLLER} -o${EXECUTABLE}.elf ${OBJECTS}


all:
	${CC} ${PHASE_ONE} ${CSOURCE}
	${CC} ${PHASE_TWO}
	${OBJECT_COPY} -j .text -j .data -O ihex ${EXECUTABLE}.elf ${EXECUTABLE}.hex
	${BURNER} -P ${COMPILER_PATH} -p ${AVRDUDE_CONTROLLER} -c ${COMPILER} -e -b ${BAUDRATE} -U flash:w:${EXECUTABLE}.hex
	rm -f ${OBJECTS} ${EXECUTABLE}.elf
clean:
	rm -f ${OBJECTS} ${EXECUTABLE}.hex ${EXECUTABLE}.elf
push:
	git push -u origin master