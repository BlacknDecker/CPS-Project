
/*** CORE  ***/

void setupGameManager();    // Setups global variables used by this module.
void gameManager();         // Brief description here <<
// ...


/*** SUPPORT **/

game_state_t startPhase();
game_state_t waitPhase();
game_state_t floodPhase();
game_state_t endPhase();

void setReceivedColor();
