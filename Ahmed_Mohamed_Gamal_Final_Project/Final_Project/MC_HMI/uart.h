 /******************************************************************************
 *
 * Module: UART
 *
 * File Name: uart.h
 *
 * Description: Header file for the UART AVR driver
 *
 * Author: Ahmed Mohamed Gamal
 *
 *******************************************************************************/

#ifndef UART_H_
#define UART_H_

#include "std_types.h"

typedef enum{
	baud_4800=4800,baud_9600 = 9600, baud_14400 = 144000
}UART_baud_rate;

typedef enum{
	FIVE_BIT,SIX_BIT,SEVEN_BIT,EIGHT_BIT,NINE_BIT=7
}UART_bit_data;

typedef enum{
	Disabled,Reserved,even,odd
}UART_parity;

typedef enum{
	ONE_StopBit,TWO_StopBit
}UART_stop_bit;


typedef struct{
	UART_baud_rate baud_rate;
	UART_bit_data bit_data;
	UART_parity parity;
	UART_stop_bit stop_bit;
}uart_ConfigType;
/*******************************************************************************
 *                      Functions Prototypes                                   *
 *******************************************************************************/

/*
 * Description :
 * Functional responsible for Initialize the UART device by:
 * 1. Setup the Frame format like number of data bits, parity bit type and number of stop bits.
 * 2. Enable the UART.
 * 3. Setup the UART baud rate.
 */
void UART_init(const uart_ConfigType *Config_PTR);

/*
 * Description :
 * Functional responsible for send byte to another UART device.
 */
void UART_sendByte(const uint8 data);

/*
 * Description :
 * Functional responsible for receive byte from another UART device.
 */
uint8 UART_recieveByte(void);

/*
 * Description :
 * Send the required string through UART to the other UART device.
 */
void UART_sendString(const uint8 *Str);

/*
 * Description :
 * Receive the required string until the '#' symbol through UART from the other UART device.
 */
void UART_receiveString(uint8 *Str); // Receive until #

#endif /* UART_H_ */
