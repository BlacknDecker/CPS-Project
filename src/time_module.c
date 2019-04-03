#include <kilombo.h>
#include "includes/main.h"
#include "includes/time_module.h"

extern USERDATA * mydata;


/***  SETUP ***/

void setupTimeManager(){
	for(int i=0; i<TIMER_NUMBER; i++)
		mydata->timer[i] = 0;
}


/*** CORE FUNCTIONS ***/


uint8_t setTimer(uint8_t clock, uint8_t value){
	mydata->timer[clock] = value;
}

uint8_t isElapsed(uint8_t clock){
	if(mydata->timer[clock]){
		mydata->timer[clock] -= 1;
		return FALSE;
	}else{
		return TRUE;
	}
}



// insert functions that exposes services to the other modules here
// Don't forget to update the header file!


/*** SUPPORT FUNCTIONS ***/

// insert support ("private") functions here

