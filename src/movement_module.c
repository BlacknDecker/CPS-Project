#include <kilombo.h>
#include "includes/main.h"
#include "includes/color_module.h"
#include "includes/time_module.h"
#include "includes/movement_module.h"

extern USERDATA * mydata;


/***  SETUP ***/

void setupMovementManager(){
	printf("setupMovementManager\n");   // Stub
  // setup global variables
  mydata->collision = FALSE;
  runnerInfoSetup();
 }


/*** CORE FUNCTIONS ***/

/*
 It moves the kilobot depending on its role and checks if it is winner
 */
void movementManager(){
    if(mydata->play==FALSE || mydata->collision==TRUE){
      stop();
    } else if(mydata->my_role == RUNNER){
  		run();
  	} else if(mydata->my_role == CATCHER){
  		updateRunnerInfo();
  		searchAndCatch();
  	}
	avoidCollisions();
  checkIfWinner();
}

void setMotion(motion_t new_motion){
  switch(new_motion) {
  case STOP:
    set_motors(0,0);
    break;
  case FORWARD:
    set_motors(kilo_turn_left, kilo_turn_right);
    break;
  case LEFT:
    set_motors(kilo_turn_left, 0); 
    break;
  case RIGHT:
    set_motors(0, kilo_turn_right); 
    break;
  }
}

void stop(){
  setMotion(STOP);
}

void runnerInfoSetup(){
  mydata->runner.runner_id = 255;
  mydata->runner.last_distance = MAX_INT;
  mydata->runner.new_distance = MAX_INT;
  mydata->runner.last_direction = 0;
  mydata->runner.in_range = 0;
}

void moveRandomly(){
  if(waitTime(MOVE_C, 100)){
    uint8_t random = rand_soft() % 3; // 0 <= random < 3
    switch(random){
      case 0:
        setMotion(LEFT); break;
      case 1:
        setMotion(FORWARD); break;
      case 2:
        setMotion(RIGHT); break;
    }
  }
}

// insert functions that exposes services to the other modules here
// Don't forget to update the header file!


/*** SUPPORT FUNCTIONS ***/

void run(){
	moveRandomly();
}

void searchAndCatch(){

  if(mydata->runner.in_range == 0){    // the runner is out of range
    moveRandomly();
  } else{                              // the runner is in range
    follow();
  }              

}

// if the runner is getting far, the catcher changes direction
void follow(){
  if(mydata->runner.last_distance < mydata->runner.new_distance) {
  	mydata->runner.last_direction = (mydata->runner.last_direction + 1) % 3;
  }
  switch(mydata->runner.last_direction){
    case 0:
		setMotion(LEFT); break;
    case 1:
        setMotion(FORWARD); break;
    case 2:
        setMotion(RIGHT); break;
    }
}

uint8_t collisionDetected(){
	uint8_t ret = FALSE;
	for(int i=0; i<MAX_NEIGHBOURS; i++){
		if(mydata->distance[i] <= DANGER_D){
       // printf("> %d - my distance from %d is: %d\n", kilo_uid, i, mydata->distance[i]);
			ret = TRUE;
		}
	}
	return ret;
}

void avoidCollisions(){
	if(collisionDetected()){		// DANGER AREA --> STOP	
    	setMotion(STOP);
      mydata->collision = TRUE;
	}
}

void updateRunnerInfo(){
	uint8_t myrunner = mydata->runner.runner_id;
	if( myrunner == 255){
		lookForARunner();
	}else{
		updateDistance(myrunner);
	}
}

void lookForARunner(){
	uint8_t myrunner = 255;
	uint8_t d = MAX_INT;
	for(int i=0; i<MAX_NEIGHBOURS; i++){ 
		if(mydata->distance[i]<d && mydata->msg_payload[i]==RUNNER){	// the msg_payload contains the role of the bot in the game phase
			d = mydata->distance[i];
			myrunner = i;
		}
	}
	mydata->runner.runner_id = myrunner;
  mydata->runner.last_distance = d;
  mydata->runner.new_distance = d; 
  mydata->runner.last_direction = 0; 
  mydata->runner.in_range = 1;
}

void updateDistance(uint8_t myrunner){
	uint8_t distance = mydata->distance[myrunner];
	if(distance < MAX_INT){
		mydata->runner.in_range = 1;
		mydata->runner.last_distance = mydata->runner.new_distance;
		mydata->runner.new_distance = distance;
	}else{
		runnerInfoSetup();
	}
}

void checkIfWinner() {
  if(mydata->my_role == RUNNER) {
    // if the runner gets caught it loses
    //printf("I am runner %d\n", kilo_uid);
    if (mydata->collision && mydata->last_msg_payload==CATCHER){
      mydata->game_status = LOSER;
    }
  }else if (mydata->my_role == CATCHER) {
    // if the catcher has caught the runner it wins
    printf("> kilo %d collision=%d myrunner=%d runner_distance=%d\n", kilo_uid, mydata->collision, mydata->runner.runner_id, mydata->runner.last_distance);
    if (mydata->collision && mydata->runner.runner_id != 255 && mydata->runner.last_distance <= WIN_D){
      mydata->game_status = WINNER;
    }
  }
}
