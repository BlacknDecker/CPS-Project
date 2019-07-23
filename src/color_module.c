#include <kilombo.h>
#include "includes/main.h"
#include "includes/time_module.h"
#include "includes/color_module.h"

extern USERDATA * mydata;


/***  SETUP ***/

void setupColors(){
	mydata->blink_state = STEADY;	// Blink Phases
	mydata->my_color = OFF;			// Need to manually setup
	setColor(OFF);					// Set OFF color
}


/*** CORE FUNCTIONS ***/

void setColor(uint8_t rgbColor){
	if(rgbColor != OFF){
		mydata->my_color = rgbColor;	// Update variable	
	}
	set_color(rgbColor);			// Actually set the color
}

// NB: delays are in kiloticks (32 kiloticks = 1 sec)
void blink(uint8_t off_delay, uint8_t on_delay, uint8_t rgbColor){
	switch(mydata->blink_state){
		case STEADY: 
			setColor(OFF); 						// Set OFF color
			setTimer(BLINK_C, off_delay); 		// Set OFF phase timer
			mydata->blink_state = BLINK_OFF;	// Go to OFF state
			break;
		case BLINK_OFF:
			if(isElapsed(BLINK_C)){
				setColor(rgbColor);					// Set ON color
				setTimer(BLINK_C, on_delay);		// Set ON phase timer
				mydata->blink_state = BLINK_ON;		// Go to ON state
			}
			break;
		case BLINK_ON:	
			if(isElapsed(BLINK_C)){
				setColor(OFF); 						// Set OFF color
				setTimer(BLINK_C, off_delay);	  	// Set OFF phase timer
				mydata->blink_state = BLINK_OFF;  	// Go to OFF state
			}
			break;
		default: 							// In case of error, restart timer
			mydata->blink_state = STEADY;	// set stable
			setColor(OFF);					// Set OFF color
	}
}

// Reset Blink Values
void resetBlink(){
	mydata->blink_state = STEADY;
	resetClock(BLINK_C);
}

void setStableColor(uint8_t rgbColor){
	mydata->blink_state = STEADY;			// set stable
	resetClock(BLINK_C);
	setColor(rgbColor);						// Set given color
}


// Convert a  color message into an rgb color value
uint8_t getColorFromMessage(uint8_t msg){
	switch(msg){
		case BLUE_MSG: return BLUE;
		case PURPLE_MSG: return PURPLE;
		case YELLOW_MSG: return YELLOW;
		case ORANGE_MSG: return ORANGE;
		default: return OFF;
	}
}

// Convert a color into its message code
uint8_t colorToMessage(uint8_t rgbColor){
	switch(rgbColor){
		case BLUE: return BLUE_MSG;
		case PURPLE: return PURPLE_MSG;
		case YELLOW: return YELLOW_MSG;
		case ORANGE: return ORANGE_MSG;
		default: return VOID_MSG;
	}
}

// changes the random number to color
void colorToNum(uint8_t r_num){
	switch(r_num){
		case 0: return setColor(BLUE);
		case 1: return setColor(PURPLE);
		case 2: return setColor(YELLOW);
		case 3: return setColor(ORANGE);
		}
}


// insert functions that exposes services to the other modules here
// Don't forget to update the header file!


/*** SUPPORT FUNCTIONS ***/

// insert support ("private") functions here

