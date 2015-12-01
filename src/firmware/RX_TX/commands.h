/*
* WRITTEN BY SARGIS S YONAN ON 24 NOVEMBER 2015
* HEADER DEFINITIONS FOR A COMMANDS LIBRARY
* USED FOR A MICRO GRID TEST BED PROJECT
* GITHUB.COM/SARGISYONAN
*/


// LIST OF ALL POSSIBLE BINARY TRANSMISSION CODES


#ifndef COMMANDS_H
#define COMMANDS_H


// SUCCESS/ERROR CODES //
#define PROCESS_COMMAND_SUCCESS					0x01
#define STATE_MACHINE_ENABLE_SUCCESS			0xDA
#define STATE_MACHINE_DISABLE_SUCCESS			0xDB
#define CHANGE_SETPOINT_SUCCESS_CODE			0xDC
#define CHANGE_POSITIVE_OFFSET_SUCCESS_CODE		0xDD
#define CHANGE_NEGATIVE_OFFSET_SUCCESS_CODE		0xDF

#define PROCESS_COMMAND_ERROR					0x00
#define CHANGE_SETPOINT_ERROR_CODE				0xF0
#define TRANSMISSION_ERROR_CODE					0xF1	// sent if message was too large or too small in length
#define CHANGE_POSITIVE_OFFSET_ERROR_CODE		0xF2
#define INVALID_COMMAND_ERROR_CODE				0xF8
#define CHANGE_NEGATIVE_OFFSET_ERROR_CODE		0xFA

// DEBUG MODE //
#define DEBUG_ON 10
#define DEBUG_OFF 0
#define RECEIVE_MESSAGE_GET_SYSTEM_STATUS       0x15
/////////////////////////

// RX/TX COMMANDS //

#define SYSTEM_INITIALIZED						0x11
#define ENABLE_STATE_MACHINE					0xFF
#define DISABLE_STATE_MACHINE					0xAA
#define RECEIVE_MESSAGE_CHANGE_SETPOINT			0xBB
#define RECEIVE_MESSAGE_CHANGE_POSITIVE_OFFSET	0xCC
#define RECEIVE_MESSAGE_CHANGE_NEGATIVE_OFFSET	0x22
#define RECEIVE_MESSAGE_GET_SYSTEM_STATUS		0x33

////////////////////

#endif