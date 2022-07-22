#ifndef TIMER_H_
#define TIMER_H_

#include "std_types.h"


/**************************************************************************************************
 *                                      ENUMS & STRUCT
 * ************************************************************************************************
 */
typedef enum{
	Normal,Compare
}MODE;

typedef enum{
	NoClock,NoPreScaler,PreScaler8,PreScaler64,PreScaler256,PreScaler1024
}PreScaler;

typedef enum{
	Timer0,Timer1
}TimerID;
typedef struct{
	uint8 InitValueTimer;
	uint8 CompareValueTimer;
	MODE TimerMode;
	PreScaler TimerPreScaler;
	TimerID TimerID;
}Timers;
/**************************************************************************************************
 *                                        FUNCTIONS
 * ************************************************************************************************
 */



void Timer_init(const Timers *Config_PTR);

void Timer_setCallBack(void(*a_ptr)(void));

void Timer_DeInit(const Timers *Config_PTR);


#endif
