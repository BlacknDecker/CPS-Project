#include <kilombo.h>
#include "includes/main.h"
#include "includes/time_module.h"
#include "includes/color_module.h"

extern USERDATA * mydata;


/***  SETUP ***/

void setupColors(){
	mydata->color_code[OFF] = RGB(0,0,0);
	mydata->color_code[RED] = RGB(3,0,0);
	mydata->color_code[GREEN] = RGB(0,3,0);
	mydata->color_code[BLUE] = RGB(0,0,3);
	mydata->color_code[WHITE] = RGB(3,3,3);
	mydata->color_code[PURPLE] = RGB(1,0,1);
	mydata->color_code[YELLOW] = RGB(2,2,0);
	mydata->color_code[ORANGE] = RGB(2,1,0);

	// Blink Phases
	mydata->blink_state = STEADY;
}


/*** CORE FUNCTIONS ***/

uint8_t getColor(uint8_t colorCode){
	return mydata->color_code[code];
}

void setColor(uint8_t colorCode){
	set_color(getColor(colorCode));				// Set color
	mydata->my_color = colorCode;				// Update variable
}

// NB: delays are in kiloticks (32 kiloticks = 1 sec)
void blink(uint8_t off_delay, uint8_t on_delay, uint8_t colorCode){
	switch(mydata->blink_state){
		case STEADY: 
			setColor(OFF); 			// Set OFF color
			setTimer(BLINK_C, off_delay); 		// Set OFF phase timer
			mydata->blink_state = BLINK_OFF;	// Go to OFF state
			break;
		case BLINK_OFF:
			if(isElapsed(BLINK_C)){
				setColor(colorCode);				// Set ON color
				setTimer(BLINK_C, on_delay);		// Set ON phase timer
				mydata->blink_state = BLINK_ON;		// Go to ON state
			}
			break;
		case BLINK_ON:
			if(isElapsed(BLINK_C)){
				setColor(colorCode); 				// Set OFF color
				setTimer(BLINK_C, off_delay);	  	// Set OFF phase timer
				mydata->blink_state = BLINK_OFF;  	// Go to OFF state
			}
			break;
		default: 
			mydata->blink_state = STEADY;	// set stable
			setColor(OFF);					// Set OFF color
	}
}

void resetBlink(uint8_t colorCode){
	mydata->blink_state = STEADY;			// set stable
	setColor(colorCode);					// Set given color
}



// insert functions that exposes services to the other modules here
// Don't forget to update the header file!


/*** SUPPORT FUNCTIONS ***/

// insert support ("private") functions here

