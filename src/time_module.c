#include <kilombo.h>
#include "includes/main.h"
#include "includes/time_module.h"

extern USERDATA * mydata;


/***  SETUP ***/

void setupTimeManager(){
	for(int i=0; i<TIMERS_NUMBER; i++)
		mydata->timer[i] = 0;
}


/*** CORE FUNCTIONS ***/


uint8_t setTimer(uint8_t clock, uint8_t value){
	mydata->timer[clock] = value;
	return value;
}

uint8_t isElapsed(uint8_t clock){
	// if(kilo_uid == 0){ printf(" > CLOCK %d IS ELAPSED? ", clock); }
	if(mydata->timer[clock]){
		mydata->timer[clock] -= 1;
		// if(kilo_uid == 0){ printf(" > NO, VALUE DECRESED: %d\n", mydata->timer[clock]); }
		return FALSE;
	}else{
		// if(kilo_uid == 0){ printf(" > YES, VALUE: %d\n\n", mydata->timer[clock]); }
		return TRUE;
	}
}

// Wait for a given time. Return TRUE when done, FALSE otherwise
uint8_t waitTime(uint8_t clock, uint8_t kiloTicks){
	if(mydata->timer[clock] && mydata->timer[clock] <= kiloTicks){	// If timer has a value > 0 and less than the given timeout means that the timer has already started
		isElapsed(clock);
		if(mydata->timer[clock])										// Check timer after decrease!
			return FALSE;
		return TRUE;
	}else{																// Otherwise i have to set my timer
		setTimer(clock, kiloTicks);
		return FALSE;
	}
}


// Set a clock to zero
void resetClock(uint8_t clock){
	mydata->timer[clock] = 0;
}
