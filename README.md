# tiny_house

This project is intended for the Micro-Grid Testbed Project and may be implemented
into The University of California at Santa Cruz's Tiny House Competition.

The subdirectories are as follows:
  - src:
    - firmware: (see examples and instructions below)
      - One-Wire Library: adds support for the temperature probe
      - UART-Library    : adds support for UART communication with the uC
      - Sensor_Driver   : a main driver for the project that includes an FSM for temperature control of a water heater
    - uC_Schematics:
      - several views of a PCB design that will house an AVR Atmega 328P uC that will be used as a controller for the driver
    - app:
      - tiny.html	: Basic frontend html page of the web application. Allows for displays of graphs
      - js		: Holds javascript source files, including the main date parser and JSON highchart files
        - lib		: Contains libraries used for the project, such as timepicker, for the date/time selection
      - css		: Styling for the frontend and media queres for responsive design. Also lib/jQuery stylesheets



	- FIRMWARE INSTRUCTIONS AND EXAMPLES
	
THIS FIRMWARE REPO IS INTENDED TO SERVE AS THE CONTROLLING SOFTWARE FOR A SMART REMOTE CONTROLLED WATER HEATER WITH VARIABLE SETPOINT, OFFSETS, AND MANIPULATEABLE STATES (DISABLE/ENABLE)

- TESTED ON AN ATMEGA2560, XBEE REV 2, AVR-GCC, AND AVR-DUDE USING AN ARDUINO PROGRAMER. CURRENTLY TESTING ON AN AVR ATMEGA 88PA-PU

- USART Library for serial I/O with the an AVR uC
	- formatted print functonality through uprintf()
- Dallas One Wire Library
	- Used for the temperature sensor probe and serial communication and floating point conversions for the sensor
GNU Makefile
	- supports builds with multiple source files, specify device, location, ISP model, and sources

DIRECTIONS FOR WIRELESS COMMUNICATIONS TRANSMISSION BAUDRATE = 19200

FRAME DETAILS:
	- 16-BIT ADDRESS
	- DEFAULT RECEIVER ADDRESS = 0x0001

CHECK COMMANDS LIST FOR AVAILABLE COMMANDS
APPEND THE DELIMITER DEFINED IN XBEE_LIB.h ~ 0x2D
	- EXAMPLE:
		- IN ORDER TO CHANGE THE SETPOINT TEMPERATURE TO 5 DEGREES CELSIUS
			- BYTE 1: 0xBB = CHANGE SETPOINT COMMAND --> COMMAND TO BE SENT
			- BYTE 2: 0x05 = 5 DEGREES CELSIUS --> ARGUMENT TO FIRST BYTE
			- BYTE 3: 0x2D = EOP -->'END OF PACKET' MESSAGE



- REPURPOSING INSTRUCTIONS
	- REDEFINE STATES IN main.c
	- REDEFINE THE PRIVATE driver LIBRARY FUNCTIONS FOR CORRECT REAL-WORLD OUTPUT
	- REDFINE STATE DEFINITIONS AND STRUCTURES IN fsm.h
	- REDEFINE SensorResult() IN driver.c/h TO PROVIDE THE CORRECT NEXT STATE

