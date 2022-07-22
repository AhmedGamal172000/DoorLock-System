#include <avr/io.h>
#include <util/delay.h>
#include "lcd.h"
#include "keypad.h"
#include "gpio.h"
#include "uart.h"
#include "Timer.h"

#define error_7aramy '!'

uint8 STATE = 1;
boolean check = FALSE;
uint8 OPENDOORCOUNTER = 2;


void SendToCONTROL(uint8 key)
{

	UART_sendByte(key);

	if((key <= 9) && (key >= 0))
	{
		LCD_displayString("*"); /* display the pressed keypad switch */
	}
	else if(key == 13)
	{
		LCD_clearScreen();
		LCD_displayStringRowColumn(0,0,"Enter New PW");
		LCD_moveCursor(1,0);
	}
	else if(key == '=' || key == '+' || key == '-')
	{

	}
	else
	{
		LCD_displayCharacter(key); /* display the pressed keypad switch */
	}
	_delay_ms(2500);
}
void ScreenChecker()
{
	if(STATE ==1)
	{
		LCD_clearScreen();
		LCD_displayString("Enter NEW PW");
		LCD_moveCursor(1, 0);

	}
	else if(STATE == 2)
	{
		LCD_clearScreen();
		LCD_displayString("Re-Enter NEW PW");
		LCD_moveCursor(1, 0);

	}
	else if(STATE == 3)
	{
		LCD_clearScreen();
		LCD_displayString("+ : Open Door");
		LCD_moveCursor(1, 0);
		LCD_displayString("- : Change Password");
	}
	else if(STATE == 4) //Open door
	{
		LCD_clearScreen();
		LCD_displayString("Enter Your PW");
		LCD_moveCursor(1, 0);
	}
	else if(STATE == 5) // Change Password
	{
		LCD_clearScreen();
		LCD_displayString("Enter YOUR PW");
		LCD_moveCursor(1, 0);
	}
	check = FALSE;
}
void CheckCases(boolean check)
{
	switch(STATE)
	{
	case 1:
		if(check == TRUE) // to know which state we are in
		{
			STATE = UART_recieveByte();//current STATE FROM MC2
			ScreenChecker();//
		}
		break;
	case 2:
		if(check == TRUE) // to know which state we are in
		{
			STATE = UART_recieveByte();
			if(STATE == 1)
			{
				LCD_clearScreen();
				LCD_displayString("WRONG PASSWORD");
				_delay_ms(5000);
			}
			else if(STATE == 3)
			{
				LCD_clearScreen();
				LCD_displayString("ACCEPTED PASSWORD");
				_delay_ms(5000);
			}
			ScreenChecker();
		}
		break;
	case 3:
		if(check == TRUE) // to know which state we are in
		{
			STATE = UART_recieveByte();
			ScreenChecker();
		}
		break;
	case 4: //OPEN door
		if(check == TRUE) // to know which state we are in
		{
			STATE = UART_recieveByte();
			if(STATE == 3)
			{
				LCD_clearScreen();
				LCD_displayString("ACCEPTED PASSWORD");
				_delay_ms(5000);

				LCD_clearScreen();
				LCD_displayString("DOOR Opening..");
				UART_recieveByte();

				LCD_clearScreen();
				LCD_displayString("DOOR Closing..");
				UART_recieveByte();

			}
			else if(STATE == 6)
			{
				LCD_clearScreen();
				LCD_displayString("WRONG PASSWORD");
				_delay_ms(5000);
				LCD_clearScreen();

				STATE = 3;
			}
			ScreenChecker();
		}
		break;

	case 5:
		if(check == TRUE) // to know which state we are in
		{
			STATE = UART_recieveByte();
			if(STATE == 1)
			{
				LCD_clearScreen();
				LCD_displayString("ACCEPTED PASSWORD");
				_delay_ms(5000);
			}
			else if(STATE == 3)
			{
				LCD_clearScreen();
				LCD_displayString("WRONG PASSWORD");
				_delay_ms(5000);
			}
			ScreenChecker();
		}
		break;
	}
}

int main()
{

	/************************************************************************************
	 *                                 KeyPad Settings
	 ************************************************************************************/
	uint8 key = 0;
	uint8 EnterPasswordCounter = 0;

	/************************************************************************************
	 *                                  LCD Settings
	 ************************************************************************************/
	LCD_init();
	LCD_displayString("ENTER NEW PW");
	LCD_moveCursor(1,0);
	/************************************************************************************
	 *                                  UART Settings
	 ************************************************************************************/
	uart_ConfigType UART_Config_2 = {baud_9600, EIGHT_BIT, Disabled, ONE_StopBit};
	UART_init(&UART_Config_2);

	while(1)
	{
		key = KEYPAD_getPressedKey();
		if(key == 13)
		{
			SendToCONTROL(key);
			check = UART_recieveByte();
			_delay_ms(10);
			CheckCases(check);
		}
		_delay_ms(10);
		switch(STATE)
		{
		case 1:
			SendToCONTROL(key); //start sending the password byte by byte to the MC2
			check = UART_recieveByte(); // to know that user enter '='
			_delay_ms(10);
			CheckCases(check);// to knw
			break;
		case 2:
			SendToCONTROL(key);
			check = UART_recieveByte();
			_delay_ms(10);
			CheckCases(check);
			break;
		case 3:
			SendToCONTROL(key);
			check = UART_recieveByte();
			_delay_ms(10);
			CheckCases(check);
			break;
		case 4:
			SendToCONTROL(key);
			check = UART_recieveByte();
			_delay_ms(10);
			CheckCases(check);
			break;

		case 5:
			SendToCONTROL(key);
			check = UART_recieveByte();
			_delay_ms(10);
			CheckCases(check);
			break;
		default:
			break;
		}

	}
}
