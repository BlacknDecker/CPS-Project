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
    if(mydata->my_role == CATCHER){
    	mydata->runner = (runner_t){-1, MAX_INT, MAX_INT, 0, 0};
    }
 }


/*** CORE FUNCTIONS ***/

/*
 It moves the kilobot depending on its role and checks if it is winner
 */
uint8_t movementManager(){
	if(waitTime(MOVE_C, 200)){
		uint8_t myrole = mydata->my_role;
    	if(myrole == RUNNER){
    		run();
    	} else if(myrole == CATCHER){
    		updateRunnerInfo();
    		searchAndCatch();
    	}
    
    	//update();
	}
	uint8_t collision = avoidCollisions();
  return checkIfWinner(collision);
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

void moveRandomly(){
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
	uint8_t ret = 0;
	for(int i=0; i<MAX_NEIGHBOURS; i++){ 
		if(mydata->distance[i] <= DANGER_D){
			ret = 1;
		}
	}
	return ret;
}

uint8_t avoidCollisions(){
	if(collisionDetected()){		// DANGER AREA --> STOP	
    	setMotion(STOP);
      return TRUE;
	}
  return FALSE;
}

void updateRunnerInfo(){
	uint8_t myrunner = mydata->runner.runner_id;
	if( myrunner == -1){
		lookForARunner();
	}else{
		updateDistance(myrunner);
	}
}

void lookForARunner(){
	uint8_t myrunner = -1;
	uint8_t d = MAX_INT;
	for(int i=0; i<MAX_NEIGHBOURS; i++){ 
		if(mydata->distance[i]<d && mydata->msg_payload[i]==RUNNER){	// the msg_payload contains the role of the bot in the game phase
			d = mydata->distance[i];
			myrunner = i;
		}
	}
	mydata->runner = (runner_t){myrunner, d, d, 0, 1};
}

void updateDistance(uint8_t myrunner){
	uint8_t distance = mydata->distance[myrunner];
	if(distance < MAX_INT){
		mydata->runner.in_range = 1;
		mydata->runner.last_distance = mydata->runner.new_distance;
		mydata->runner.new_distance = distance;
	}else{
		mydata->runner = (runner_t){-1, MAX_INT, MAX_INT, 0, 0};
	}
}

uint8_t checkIfWinner(uint8_t collision) {

  if(mydata->my_role == RUNNER) {
    // if the runner gets caught it loses
    if (collision){
      return FALSE;
    }
  }else if (mydata->my_role == CATCHER) {
    // if the catcher has caught the runner it wins
    if (mydata->runner.runner_id != -1 && mydata->runner.last_distance < DANGER_D){
      return TRUE;
    }
  }
  // Otherwise the game is going on
  return -1;
}
