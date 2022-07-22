
#include <avr/io.h>
#include <util/delay.h>
#include "Buzzer.h"
#include "dc_motor.h"
#include "uart.h"
#include "twi.h"
#include "external_eeprom.h"
#include "gpio.h"
#include "Timer.h"
#include "std_types.h"


Timers Timer_x ={0,0,Normal,PreScaler1024,Timer0};
uint16 g_tick = 0;

uint8 MotorOpenCW = 1;
uint8 MotorOpenACW = 0;
uint8 rotate = 0;
uint8 incrementer = 0;
uint8 STATE=1;
uint8 Password[5];
uint8 index=0;
#define error_7aramy '!'

#define NUMBER_OF_COMPARE_MTACHES_PER_15_SECOND 450




/* global variable contain the ticks count of the timer */


/* Interrupt Service Routine for timer0 compare mode */
void WrongMessage()
{
	buzzer_ON();
	_delay_ms(7000);
	buzzer_OFF();
}
void callBackAction(void)
{
	g_tick++;
	if (MotorOpenCW == 1)
	{
		DcMotor_Rotate(DC_MOTOR_ACW);
		MotorOpenCW = 0;


	}
	if (MotorOpenACW == 1)
	{
		DcMotor_Rotate(DC_MOTOR_CW);
		MotorOpenACW = 0;

	}
	if(g_tick == NUMBER_OF_COMPARE_MTACHES_PER_15_SECOND)
	{
		DcMotor_Rotate(DC_MOTOR_STOP);
		g_tick = 0;
		rotate++;
		if (rotate == 1)
		{
			_delay_ms(3000);
			MotorOpenACW = 1;
			UART_sendByte(STATE);
		}
		else if ( rotate == 2)
		{
			Timer_DeInit(&Timer_x);
			rotate = 0;
			MotorOpenCW = 1;
			MotorOpenACW = 0;
			UART_sendByte(STATE);
		}
	}
}


void TimerStartWithMotor()
{
	Timer_setCallBack(callBackAction);
	Timer_init(&Timer_x);
	DcMotor_Init();
	g_tick = 0;

}
boolean CheckPassword(void)
{
	uint8 temp1;
	uint8 temp2;
	uint8 j = 5 ;
	for(uint8 i = 0; i<5 ; i++)
	{
		EEPROM_readByte(i,&temp1); // from 0 to 4 have password 1
		EEPROM_readByte(j, &temp2); // from 5 to 9 have password 2
		if( temp1 != temp2 ) // compare between them
		{
			WrongMessage();
			return FALSE;
		}
		else
		{
			j++;
		}

	}
	return TRUE;
}
uint8 ScreenOpenDoor(uint8 *value)
{

	if(*value == '=')
	{
		index = 0;
		uint8 temp=0;

		for(int i = 0 ; i <5 ; i++)
		{
			EEPROM_readByte(i, &temp);
			if(Password[i] != temp)
			{
				WrongMessage();
				UART_sendByte(TRUE);
				STATE = 6;
				UART_sendByte(STATE);
				STATE = 3;
				return 0;
			}
			else
			{
				continue;
			}
		}

		UART_sendByte(TRUE);
		STATE = 3;
		UART_sendByte(STATE);
		TimerStartWithMotor();

		//////////////////////////////////////////////////////////////////////////////////////
	}
	else
	{
		Password[index] = *value;
		index++;
		UART_sendByte(FALSE);
	}

}
uint8 ScreenChangePassword(uint8 *value)
{
	if(*value == '=')
	{
		index = 0;
		uint8 temp=0;

		for(int i = 0 ; i <5 ; i++)
		{
			EEPROM_readByte(i, &temp);
			if(Password[i] != temp)
			{
				WrongMessage();
				UART_sendByte(TRUE);
				STATE = 3;
				UART_sendByte(STATE);
				return 0;
			}
			else {

				continue;
			}
		}

		UART_sendByte(TRUE);
		STATE = 1;
		UART_sendByte(STATE);
	}
	else
	{
		Password[index] = *value;
		index++;
		UART_sendByte(FALSE);
	}
}
void ScreenPlusMinus (uint8 *value)
{
	if(*value == '-')
	{
		STATE = 5;
		UART_sendByte(TRUE);
		UART_sendByte(STATE);
	}
	else if(*value == '+')
	{
		STATE = 4;
		UART_sendByte(TRUE);
		UART_sendByte(STATE);
	}
}
void ScreenEnterNewPassword(uint8 *value)
{
	if(*value  == '=')
	{

		index = 0;
		STATE = 2;

		uint8 temp = 0;
		for(int i =0 ; i <5 ; i++)
		{
			EEPROM_readByte(i, &temp);
			_delay_ms(1000);
		}
		*value = 0;
		UART_sendByte(TRUE);
		_delay_ms(20);
		UART_sendByte(STATE);
	}
	else
	{
		EEPROM_writeByte(incrementer, *value);
		incrementer++;
		UART_sendByte(FALSE);
	}

}
void ScreenRe_EnterPassword(uint8 *value)
{
	boolean check = FALSE;
	if(*value  == '=')
	{
		uint8 temp = 0;
		for(int i =0 ; i <5 ; i++)
		{
			EEPROM_readByte(i, &temp);

			_delay_ms(1000);
		}
		check = CheckPassword();
		if(check == TRUE)
		{
			STATE = 3;
		}
		else if(check == FALSE)
		{
			WrongMessage();
			STATE = 1;
			incrementer = 0;
		}
		*value = 0;
		UART_sendByte(TRUE);
		_delay_ms(20);
		UART_sendByte(STATE);
	}
	else
	{
		EEPROM_writeByte(incrementer, *value);
		incrementer++;
		UART_sendByte(FALSE);
	}

	if(incrementer == 10)
	{
		incrementer = 0;
	}
}

int main()
{
	uint8 value = 0;
	uint8 temp;
	boolean check = FALSE;
	uint8 show =1;

	uint8 i=0;
	uint8 Password[5];

	GPIO_setupPinDirection(PORTD_ID, PIN7_ID,PIN_OUTPUT);
	GPIO_writePin(PORTD_ID,PIN7_ID, LOGIC_LOW);

	Config_twi twi = {ONE,SlaveAddress_ONE};
	/* Initialize the TWI/I2C Driver */
	TWI_init(&twi);


	uart_ConfigType UART_Config_2 = {baud_9600, EIGHT_BIT, Disabled, ONE_StopBit};
	UART_init(&UART_Config_2);

	buzzer_init();



	SREG  |= (1<<7);
	buzzer_init();
	while(1)
	{

		value = UART_recieveByte();
		if(value == 13)
		{
			STATE = 1;
		}
		switch(STATE)
		{
		case 1:
			ScreenEnterNewPassword(&value);
			break;
		case 2:
			ScreenRe_EnterPassword(&value);
			break;
		case 3:
			ScreenPlusMinus(&value);
			break;
		case 4:
			ScreenOpenDoor(&value);
			break;
		case 5:
			ScreenChangePassword(&value);
			break;

		}
	}
}

