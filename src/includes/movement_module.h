
/*** CORE  ***/

void setupMovementManager();    // Setups global variables used by this module.
void movementManager();         // Brief description here <<
void setMotion(motion_t new_motion);
void moveRandomly();

/*** SUPPORT **/

void run();
void searchAndCatch();
void follow();

uint8_t collisionDetected();
void avoidCollisions();
void updateRunnerInfo();
void lookForARunner();
void updateDistance(uint8_t myrunner);



