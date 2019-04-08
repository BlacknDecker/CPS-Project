#include <kilombo.h>
#include "includes/main.h"
#include "includes/color_module.h"
#include "includes/time_module.h"

extern USERDATA * mydata;


/***  SETUP ***/

void setupMovementManager(){
	printf("setupMovementManager\n");   // Stub
	resetBlink(getColor(OFF));			// Test
    // setup global variables
}


/*** CORE FUNCTIONS ***/

/*
 Provide function description here
 */
void movementManager(){
	printf("movementManager\n"); 		//Stub
	blink(16, 32, getColor(WHITE));		// Test
    // do stuff
}

// insert functions that exposes services to the other modules here
// Don't forget to update the header file!


/*** SUPPORT FUNCTIONS ***/

// insert support ("private") functions here

