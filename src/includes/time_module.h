
/*** CORE  ***/

void setupTimeManager();	// Setups global variables used by this module.


uint8_t setTimer(uint8_t clock, uint8_t value); 	// Set the given timer to a given value
uint8_t isElapsed(uint8_t clock);					// Return TRUE when the given clock is elapsed. Decrease the timer and return false otherwise
// ...


/*** SUPPORT **/

// ...
