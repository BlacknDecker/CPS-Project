#include <kilombo.h>
#include "includes/main.h"
#include "includes/color_module.h"
#include "includes/communication_module.h"
#include "includes/time_module.h"
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
		setColor(WHITE);
	}else{

		for (uint8_t localBot = 1; localBot < 10; localBot++){
			if(kilo_uid == localBot){
				uint8_t randomGeneratedNumber = getRandomNumber(3,4);
				setColor(numberToColor(randomGeneratedNumber));
				printf("> %d - (1) Start! %d \n", kilo_uid, randomGeneratedNumber );
			}

		}

	}
	printf("> %d - (1) Start!\n", kilo_uid);  // DEBUG
	return WAIT_PHASE;
}

game_state_t waitPhase(){
	if(kilo_uid == 0){					// Coordinator
		blink(16, 32, WHITE);
		if(waitTime(GAME_C, 160)){		// Wait 5 sec
			if(kilo_uid==0){ printf("> %d - (2) WAIT End. Flood!\n", kilo_uid); } // DEBUG
			setStableColor(BLUE);			 // Set a stable color and move on the next phase
			mydata->game_msg_state = START;  // Setup flooding
			return FLOOD_PHASE;
		}else{
			return WAIT_PHASE;
		}
	}else{								    // All the other
		if(mydata->new_message_arrived){	// Wait for a new message
			mydata->new_message_arrived = FALSE;	// Reset variable
			if(mydata->last_msg_payload >= BLUE_MSG || mydata->last_msg_payload <= CYAN_MSG){ 	// Check if i received a color
				setReceivedColor();						// Set received color
				printf("> %d: RECEIVED COLOR CODE:%d!\n", kilo_uid, mydata->last_msg_payload); // DEBUG
				mydata->game_msg_state = START;  		// Setup flooding
				setStableColor(mydata->my_color);
				return FLOOD_PHASE;						// move to the next phase
			}
		}
		return WAIT_PHASE;
	}	
}

game_state_t floodPhase(){

	uint8_t randomGeneratedNumber = getRandomNumber(3,4); //picking a color as the runner to flood it.
	

	if(kilo_uid == 0){									// Coordinator


		floodMessage(colorToMessage(numberToColor(randomGeneratedNumber)), &mydata->game_msg_state);	// Flood a message with a color

		if(mydata->game_msg_state == FINISH){
			setStableColor(GREEN);
			printf("> %d - (3)  Ended Flooding\n", kilo_uid);
			return END_PHASE;							// When finished flooding moves to the next phase
		}
		return FLOOD_PHASE;
	} else {											// All the others
		blink(16, 8, mydata->my_color);
		floodMessage(colorToMessage(mydata->my_color), &mydata->game_msg_state);	// Flood the color received
		/*
		if(kilo_uid == 1){ 
	        printf(" 	> FLOODING PROGRESS - COMM CLOCK: %d\n", mydata->timer[COMMUNICATION_C]);
	        printf("    > GAME STATE: %d\n", mydata->game_state);
	        printf("    > FLOOD STATE:%d\n", mydata->game_msg_state);
	        printf("    > MSGEX STATE:%d\n", mydata->msg_ex_state);
	        printf("    > OUT QUEUE:  %d | %d | %d\n", mydata->out_queue[0],mydata->out_queue[1],mydata->out_queue[2]);
	        printf("    > OUT MESSAGE:%d\n", getMessagePayload(mydata->outgoing_message));
	    }
	    */
		if(mydata->game_msg_state == FINISH){
			setStableColor(mydata->my_color);
			printf("> %d: Ended Flooding\n", kilo_uid);
			return END_PHASE;							// When finished flooding moves to the next phase
		}
		return FLOOD_PHASE;
	}
}

game_state_t endPhase(){
	if(waitTime(GAME_C, 160)){		// Wait 5 sec
		printf("> %d - (4) Cooldown Ended\n", kilo_uid);
		return START_PHASE;
	}
	return END_PHASE;
}


// UTIL
void setReceivedColor(){
	uint8_t color_code = mydata->last_msg_payload;
	uint8_t translated_color = getColorFromMessage(color_code);
	setColor(translated_color);
	/*
	printf(" - COLOR DEBUG -\n");
	printf("Color Code Received:%d\n", color_code);
	printf("Translated Color: %d\n", translated_color);
	printf("Actual Color:%d\n\n", mydata->my_color);
	*/
}

// insert support ("private") functions here

