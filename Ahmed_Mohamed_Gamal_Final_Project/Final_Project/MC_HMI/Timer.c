#include <avr/io.h>
#include "Timer.h"
#include <avr/interrupt.h>
#include <util/delay.h>
/*******************************************************************************
 *                           Global Variables                                  *
 *******************************************************************************/

/* Global variables to hold the address of the call back function in the application */
static volatile void (*g_callBackPtr)(void) = NULL_PTR;

/*******************************************************************************
 *                       Interrupt Service Routines                            *
 *******************************************************************************/

ISR(TIMER0_COMP_vect)
{

	if(g_callBackPtr != NULL_PTR)
	{
		/* Call the Call Back function in the application after the edge is detected */
		(*g_callBackPtr)(); /* another method to call the function using pointer to function g_callBackPtr(); */
	}
}
ISR(TIMER0_OVF_vect)
{
	if(g_callBackPtr != NULL_PTR)
		{
			/* Call the Call Back function in the application after the edge is detected */
			(*g_callBackPtr)(); /* another method to call the function using pointer to function g_callBackPtr(); */
		}
}
/*******************************************************************************
 *                       Timer Functions                                       *
 *******************************************************************************/

void Timer_init(const Timers *Config_PTR)
{

	TCNT0 = Config_PTR->InitValueTimer;    // Set Timer initial value to 0


	if(Config_PTR->TimerMode == Compare && Config_PTR->TimerID == Timer0) //CompareMode
	{
		OCR0  = Config_PTR->CompareValueTimer; // Set Compare Value
		TIMSK |= (1<<OCIE0); // Enable Timer0 Compare Interrupt

	}

	else if(Config_PTR->TimerMode == Normal && Config_PTR->TimerID == Timer0)//NormalMode
	{


		TIMSK |= (1<<TOIE0);
	}
	/* Configure timer0 control register TCCR0
	 * 1. Non PWM mode FOC0=1
	 * 2. Config_PTR->TimerMode = 0 : NormalMode WGM01=0 & WGM00=0
	 * 3. Config_PTR->TimerMode = 1 : CompareMode WGM01=1 & WGM00=0
	 * 4. No need for OC0 in this example so COM00=0 & COM01=0
	 * 5. Set PreScaleValue CS02,CS01,CS00
	 */
	//TCCR0 = (1<<FOC0) | (1<<WGM01) | (1<<CS02) | (1<<CS00);
	TCCR0 = (1<<FOC0) | ((Config_PTR->TimerMode & (0x01)) << WGM01) | (Config_PTR->TimerPreScaler << CS00);

}

void Timer_setCallBack(void(*a_ptr)(void))
{

	/* Save the address of the Call back function in a global variable */
	g_callBackPtr = a_ptr;
}


void Timer_DeInit(const Timers *Config_PTR)
{
	TCNT0 = 0;
	OCR0  = 0;
	if(Config_PTR->TimerID == Timer0)
	{
		TIMSK &= 0xFC; //DeInit Compare/OverFlow Modes in Timer0 OCIE0,TOIE0
	}
	TCCR0 = 0 ;

}
