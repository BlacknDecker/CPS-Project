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
	    case GAME_PHASE: mydata->game_state = gamephase(); break;
	    case WIN_PHASE: mydata->game_state = winphase(); break;
	    case LOSE_PHASE: mydata->game_state = losephase(); break;
		  case STANDBY_PHASE: mydata->game_state = standbyphase(); break;
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
		uint8_t randomGeneratedNumber = getRandomNumber(3,8);
		setColor(numberToColor(randomGeneratedNumber));
		mydata->my_color= numberToColor(randomGeneratedNumber);
		printf("> Bot Number %d - (1) Started! Picked Color Code: %d  \n" ,kilo_uid, randomGeneratedNumber); // DEBUG
	}
	return WAIT_PHASE;
}

game_state_t waitPhase(){
	if(kilo_uid == 0){					// Coordinator
		blink(16, 32, WHITE);
		if(waitTime(GAME_C, 160)){		// Wait 5 sec
			if(kilo_uid==0){ printf("> %d - (2) WAIT End. Flood!\n", kilo_uid); } // DEBUG
			setStableColor(BLUE);			 // Set a stable color and move on the next phase
			mydata->game_msg_state = START;  // Setup flooding
			//mydata->random_color = getRandomColor();	//Choose a random color
	    mydata->runner_color = getRandomNumber(3,8); //picking a color as the runner to flood it.
      return FLOOD_PHASE;
		}else{
			return WAIT_PHASE;
		}
	}else{								    // All the other
		if(mydata->new_message_arrived){	// Wait for a new message
			mydata->new_message_arrived = FALSE;	// Reset variable
			if(mydata->last_msg_payload >= BLUE_MSG || mydata->last_msg_payload <= CYAN_MSG){ 	// Checks to see if it has received a color
				//setReceivedColor();						// Set received color
				printf("> %d: RECEIVED COLOR CODE:%d!\n", kilo_uid, mydata->last_msg_payload); // DEBUG
               	
				mydata->runner_color = getColorFromMessage(mydata->last_msg_payload); // saves the color of the runner
				if (mydata->my_color == mydata->runner_color){       // Bot sets its self as runner if it has the same color as the picked color by the witch!
					mydata->my_role = RUNNER; //Updates its role as the Runner
					printf(">%d Is the Runner Now! [TEST, if Correct Value == 1] VALUE == %d \n ", kilo_uid , mydata->my_role); // DEBUG If Value is 1 it should be Correct
				}  
				else if  (kilo_uid != 0 && mydata->my_color != mydata->runner_color) {
					mydata->my_role = CATCHER; //Updates its role as catcher
					printf("> %d Is a Catcher Now! [TEST, if Correct Value == 2] VALUE == %d \n ", kilo_uid , mydata->my_role); // DEBUG If Value is 2 it should be Correct

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
		floodMessage(mydata->runner_color, &mydata->game_msg_state);	// Flood a message with a color
		if(mydata->game_msg_state == FINISH){
			setStableColor(GREEN);
			printf("> %d - (3)  Ended Flooding\n", kilo_uid);
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
			setStableColor(mydata->my_color);
			printf("> %d: Ended Flooding\n", kilo_uid);
			return GAME_PHASE;							// When finished flooding moves to the next phase
		}
		return FLOOD_PHASE;
	}
}


game_state_t gamephase(){

	setupMovementManager();

	if(kilo_uid == 0){
		return END_PHASE;
	}
	if (mydata->my_role == RUNNER){
 		pingMessage(RUNNER_MSG);}
 	if (mydata->my_role == CATCHER){
		pingMessage(CATCHER_MSG);}

	mydata->bot_state =  movementManager();

	if (mydata->bot_state == FALSE){
		return LOSE_PHASE;
	}
	else if (mydata->bot_state == TRUE){
		return WIN_PHASE;
	}

	else if (mydata->bot_state == -1) {
		return GAME_PHASE;
	}



 	 //DONT REMOVE THESE COMMENTS!
	 //DONT REMOVE THESE COMMENTS!
	 //DONT REMOVE THESE COMMENTS!
	 //DONT REMOVE THESE COMMENTS!

	// if (mydata->my_role == RUNNER){
 //  		pingMessage(RUNNER_MSG);
  		
 //  		mydata->bot_state =  movementManager();
 //  			if (mydata->bot_state != FALSE && mydata->bot_state != -1 ){
 //  				printf("# the bot number %d -Has WON Proudly! \n", kilo_uid);
 //  				return WIN_PHASE;
 //  			}
 //  			if (mydata->bot_state == FALSE){
 //  				printf("# the bot number %d -Has LOST Shamefully! \n", kilo_uid);
 //  				return LOSE_PHASE;
 //  			}

  		
	//   return GAME_PHASE;
	// }

	// else if (mydata->my_role == CATCHER){
	// 	pingMessage(CATCHER_MSG);
	// 	mydata->bot_state =  movementManager();
	// 	if ( mydata->bot_state = TRUE){
	// 		printf("# the bot number %d -Has WON Proudly! \n", kilo_uid);
	// 		return WIN_PHASE;
	// 	}
	// 	}

	// 	if(mydata->bot_state != TRUE && mydata->bot_state != -1){

	// 		return STANDBY_PHASE;
	// 	}

	// 	return GAME_PHASE;

	 //DONT REMOVE THESE COMMENTS!
	 //DONT REMOVE THESE COMMENTS!
	 //DONT REMOVE THESE COMMENTS!
	 //DONT REMOVE THESE COMMENTS!

	}


game_state_t winphase(){
	if(kilo_uid == 0){
		return END_PHASE;
	}
	else{						// the bot blinks green as it wins
		blink(16, 32, GREEN);
		return WIN_PHASE;
	}
}

game_state_t losephase(){
	if(kilo_uid == 0){
		return END_PHASE;
	}
	else{
		blink(16, 32, WHITE);     // the bot blinks white, as waving the flag of surrender! the white flag!
		printf("kilobot %d LOSES%d\n", kilo_uid);
		return LOSE_PHASE;
	}
}

game_state_t standbyphase(){
	if(kilo_uid == 0){ return END_PHASE; }  // Witch
 	if (mydata->my_role == RUNNER){         // if the runner is in the standbyphase and 
		if(waitTime(GAME_C, 300)){		        // Waits for time to run out		
			if(mydata->new_message_arrived == TRUE && mydata->last_msg_payload == CATCHER_MSG){  //checks to see if a meaningful message has arrived or not if true
				mydata->new_message_arrived = FALSE;												// resets the new msg flag
				return GAME_PHASE;																	// goes back to the game phasse
			}
			else {
				printf("# the bot number %d -Has WON Proudly! \n", kilo_uid);
				return WIN_PHASE;																	// if lost for a long time moves to win phase!
			}
		}
	}
	if (mydata->my_role == CATCHER){
		if(waitTime(GAME_C, 300)){		// Waits for time to run out
			if(mydata->new_message_arrived == TRUE && mydata->last_msg_payload == RUNNER_MSG){ //checks to see if a meaningful message has arrived or not if true
				mydata->new_message_arrived = FALSE;											// resets the new msg flag
				return GAME_PHASE;																// goes back to the game phasse
			}
			else {
				printf("# the bot number %d -Has LOST Shamefully! \n", kilo_uid);			
				return LOSE_PHASE;																// if lost for a long time moves to lose phase!
			}
		}
	}

}

game_state_t endPhase(){
    blink(16, 32, BLUE);
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

