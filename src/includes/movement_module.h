
/*** CORE  ***/

void setupMovementManager();    		// Setups global variables used by this module.
void movementManager();         		// Makes the kilobot move depending on its roles and checks if it is winner
void setMotion(motion_t new_motion);	// sets the motors pf the kilobot
void runnerInfoSetup();
void moveRandomly();					// makes the kilobot moving randomly

/*** SUPPORT **/

void run();						// runner behaviour
void searchAndCatch();			// catcher behaviour, looking for a runner and following it
void follow();					// follows a runner kilobot

uint8_t collisionDetected();	// returns true if two kilobots are too close
uint8_t avoidCollisions();		// if two kilobots are too close, it forces them to stop
void updateRunnerInfo();		// updates the catcher infos about the runner it's following
void lookForARunner();			// if no runner is in range, it check the messages to look for one
void updateDistance(uint8_t myrunner);	// updates the distance from the current runner
void checkIfWinner(uint8_t collision);		// Returns true if a catcher catches a runner, false if a runner get caught, -1 otherwie (meaning that the game is going on) 


