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
    mydata->game_status = PLAYING;
    mydata->play = FALSE;
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
	    case GAME_PHASE: mydata->game_state = gamephase(); break;
	    case WIN_PHASE: mydata->game_state = winphase(); break;
	    case LOSE_PHASE: mydata->game_state = losephase(); break;
	    case END_PHASE:   mydata->game_state = endPhase();	break;

	}
}

// insert functions that exposes services to the other modules here
// Don't forget to update the header file!


/*** SUPPORT FUNCTIONS ***/

game_state_t startPhase(){
	if(kilo_uid == 0){
		mydata->my_role = WITCH;
		setColor(WHITE);
	}else{
		uint8_t randomGeneratedNumber = getRandomNumber(3,8);
		mydata->my_color= numberToColor(randomGeneratedNumber);
		setColor(mydata->my_color);
		printf("> Bot Number %d - (1) Started! Picked Color Code: %d  \n" ,kilo_uid, randomGeneratedNumber); // DEBUG
	}
	return WAIT_PHASE;
}

game_state_t waitPhase(){
	if(kilo_uid == 0){					// Coordinator
		blink(16, 32, WHITE);
		if(waitTime(GAME_C, 10)){		// Wait 5 sec
			printf("> %d - (2) WAIT End. Flood!\n", kilo_uid); // DEBUG
			setStableColor(WHITE);			 // Set a stable color and move on the next phase
			mydata->game_msg_state = START;  // Setup flooding
			//mydata->random_color = getRandomColor();	//Choose a random color
	    mydata->runner_color = numberToColor(getRandomNumber(3,8)); //picking a color as the runner to flood it.
	    printf("Witch says color %d\n", mydata->runner_color);
      return FLOOD_PHASE;
		}else{
			return WAIT_PHASE;
		}
	}else{								    // All the other
		if(mydata->new_message_arrived){	// Wait for a new message
			mydata->new_message_arrived = FALSE;	// Reset variable
			if(mydata->last_msg_payload >= BLUE_MSG || mydata->last_msg_payload <= CYAN_MSG){					// Set received color
				mydata->runner_color = getColorFromMessage(mydata->last_msg_payload); // saves the color of the runner
				//printf("> %d: RECEIVED COLOR CODE:%d!\n", kilo_uid, mydata->runner_color); // DEBUG
				if (mydata->my_color == mydata->runner_color){       // Bot sets its self as runner if it has the same color as the picked color by the witch!
					mydata->my_role = RUNNER; //Updates its role as the Runner
					printf("> %d: I am a runner!\n", kilo_uid);
				}  
				else if  (kilo_uid != 0 && mydata->my_color != mydata->runner_color) {
					mydata->my_role = CATCHER; //Updates its role as catcher
					printf("> %d: I am a catcher!\n", kilo_uid);

				}

				mydata->game_msg_state = START;  		// Setup flooding
				setStableColor(mydata->my_color);   
				return FLOOD_PHASE;						// move to the next phase
			}
		}
		return WAIT_PHASE;
	}	
}

game_state_t floodPhase(){
	if(kilo_uid == 0){									// Coordinator
		floodMessage(colorToMessage(mydata->runner_color), &mydata->game_msg_state);	// Flood a message with a color
		if(mydata->game_msg_state == FINISH){
			printf("> %d - (3)  Ended Flooding\n", kilo_uid);
			blink(16, 32, mydata->runner_color);		// the witch blinks the color of the runners
			return END_PHASE;							// When finished flooding moves to the next phase
		}
		return FLOOD_PHASE;
	} else {											// All the others
		blink(16, 8, mydata->my_color);
		floodMessage(colorToMessage(mydata->runner_color), &mydata->game_msg_state);	// Flood the color received
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
			printf("> %d: Ended Flooding\n", kilo_uid);
			flushLastMsgPayload();
			return GAME_PHASE;							// When finished flooding moves to the next phase
		}
		return FLOOD_PHASE;
	}
}


game_state_t gamephase(){
	if(kilo_uid == 0){
		return END_PHASE;
	}
	setStableColor(mydata->my_color);
	mydata->play = TRUE;
	setupPinging();
	pingMessage(mydata->my_role);
	if (mydata->game_status == WINNER){
		printf("kilobot %d WINS\n", kilo_uid);
		return WIN_PHASE;
	} else if (mydata->game_status == LOSER) {
		printf("kilobot %d LOSES\n", kilo_uid);
		return LOSE_PHASE;
	}
	return GAME_PHASE;
}

game_state_t winphase(){
	setMotion(STOP);
	setupPinging();
	pingMessage(255);		// notifies the other kilobots to avoid collisions, but telling that it's out of the game
	mydata->play = FALSE;	// stops the kilobot
	blink(16, 32, GREEN);	// the bot blinks green as it wins
	return WIN_PHASE;
}

game_state_t losephase(){
	setMotion(STOP);
	setupPinging();
	pingMessage(255);		// notifies the other kilobots to avoid collisions, but telling that it's out of the game
	mydata->play = FALSE;	// stops the kilobot
	blink(16, 32, WHITE);     // the bot blinks white, as waving the flag of surrender! the white flag!
	return LOSE_PHASE;
}

game_state_t endPhase(){
	blink(16,32,mydata->runner_color); // the witch blinks the color of the runners
	setupPinging();
	pingMessage(255);		// notifies the other kilobots to avoid collisions, but telling that it's out of the game
	return END_PHASE;
}

// insert support ("private") functions here

