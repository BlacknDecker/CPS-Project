
/*** CORE  ***/

void setupGameManager();    // Setups global variables used by this module.
void gameManager();         // Brief description here <<
// ...


/*** SUPPORT **/

uint8_t gameTimerElapsed();
game_state_t startPhase();
game_state_t waitPhase();
game_state_t floodPhase();
game_state_t endPhase();
game_state_t winphase();
game_state_t losephase();
game_state_t gamephase();
void stopTheGame();
