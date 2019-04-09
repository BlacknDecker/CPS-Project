#include <kilombo.h>
#include "includes/main.h"
#include "includes/game_module.h"

extern USERDATA * mydata;


/***  SETUP ***/

void setupGameManager(){
	printf("SetupGameManager\n"); //Stub
	mydata->game_state = START_PHASE;
	mydata->game_msg_state = START;
    // setup global variables
}


/*** CORE FUNCTIONS ***/

/*
 Provide function description here
 */
void gameManager(){
	// Test Communication
	switch(mydata->game_state){
		case START_PHASE: mydata->game_state = startPhase(); break;
	    case WAIT_PHASE:  mydata->game_state = waitPhase();	break;
	    case FLOOD_PHASE: mydata->game_state = floodPhase(); break;
	    case END_PHASE:   mydata->game_state = endPhase();	break;
	}
}

// insert functions that exposes services to the other modules here
// Don't forget to update the header file!


/*** SUPPORT FUNCTIONS ***/

game_state_t startPhase(){
	if(kilo_uid == 0){
		setColor(RED);
	}else{
		setColor(ORANGE);
	}
	return WAIT_PHASE;
}

game_state_t waitPhase(){
	if(kilo_uid == 0){					// Coordinator
		blink(16, 8, BLUE);
		if(waitTime(GAME_C, 160)){		// Wait 5 sec
			resetBlink(RED);			// Go back to red and move on the next phase
			return FLOOD_PHASE;
		}else{
			return WAIT_PHASE;
		}
	}else{								    // All the other
		if(mydata->new_message_arrived){	// Wait for a new message
			mydata->new_message_arrived = FALSE;	// Reset variable
			setReceivedColor();						// Set received color
			return FLOOD_PHASE;						// move to the next phase
		}else{
			return WAIT_PHASE;
		}
	}	
}

game_state_t floodPhase(){
	if(kilo_uid == 0){									// Coordinator
		floodMessage(BLUE, &mydata->game_msg_state);	// Flood a message with a color
		if(mydata->game_msg_state == FINISH){
			return END_PHASE;							// When finished flooding moves to the next phase
		}
		return FLOOD_PHASE;
	} else {											// All the others
		floodMessage(mydata->my_color, &mydata->game_msg_state);	// Flood the color received
		if(mydata->game_msg_state == FINISH){
			return END_PHASE;							// When finished flooding moves to the next phase
		}
		return FLOOD_PHASE;
	}
}

game_state_t endPhase(){
	if(waitTime(GAME_C, 160)){		// Wait 5 sec
		return START_PHASE;
	}
}


// UTIL
void setReceivedColor(){
	for(int i=0; i<MAX_NEIGHBOURS; i++){			// Check archive for new messages
		if(mydata->neighbours[i]){
			if(mydata->msg_payload[i] != mydata->my_color){
				setColor(mydata->msg_payload);		// Set the new color
			}
		}
	}
}

// insert support ("private") functions here

