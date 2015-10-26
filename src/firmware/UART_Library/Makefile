AVR_GCC_CONTROLLER           = atmega2560
BURNER						 = avrdude
AVRDUDE_CONTROLLER 	         = m2560
CC							 = avr-gcc
CSOURCE						 = uart.c dallas_paku_lib.c OneWire.c tests.c
OBJECTS  					 = ${CSOURCE:.c=.o}
EXECUTABLE					 = tempTest
COMPILER_PATH				 = /dev/cu.usbmodemfa131
COMPILER  					 = stk500v2
BAUDRATE					 = 115200
OBJECT_COPY					 = avr-objcopy
CFLAGS						 = -Wall -Wextra -Wimplicit -std=gnu99
PHASE_ONE					 = -Os -mmcu=${AVR_GCC_CONTROLLER} ${CFLAGS} -c
PHASE_TWO					 = -mmcu=${AVR_GCC_CONTROLLER} -o ${EXECUTABLE}.elf ${OBJECTS}


all :
	${CC} ${PHASE_ONE} ${CSOURCE}
	${CC} ${PHASE_TWO}
	${OBJECT_COPY} -j .text -j .data -O ihex ${EXECUTABLE}.elf ${EXECUTABLE}.hex
	${BURNER} -P ${COMPILER_PATH} -p ${AVRDUDE_CONTROLLER} -c ${COMPILER} -e -b ${BAUDRATE} -U flash:w:${EXECUTABLE}.hex

clean :
	rm -f ${OBJECTS} ${EXECUTABLE}.hex ${EXECUTABLE}.elf
