# tiny_house

This project is intended for the Micro-Grid Testbed Project and may be implemented
into The University of California at Santa Cruz's Tiny House Competition.

The subdirectories are as follows:
  - src:
    - firmware:
      - One-Wire Library: adds support for the temperature probe
      - UART-Library    : adds support for UART communication with the uC
      - Sensor_Driver   : a main driver for the project that includes an FSM for temperature control of a water heater
    - uC_Schematics:
      - several views of a PCB design that will house an AVR Atmega 328P uC that will be used as a controller for the driver
    
