
#ifndef buzzer_H_
#define buzzer_H_

#include "std_types.h"


#define Buzzer_PORT_ID  PORTD_ID
#define Buzzer_PIN_ID	PIN6_ID


void buzzer_init(void);

void buzzer_ON(void);

void buzzer_OFF(void);

#endif
