
/*** CORE  ***/

void setupColors();	// Setups global variables used by this module.


uint8_t getColor(uint8_t code); 										// Get RGB Color by Color Code
void blink(uint8_t off_delay, uint8_t on_delay, uint8_t rgb_color);		// Blinks (NB: must be called on every loop cycle!) NB.2: delays are in kiloticks (32 kiloticks = 1 sec)
void resetBlink(uint8_t rgb_color);										// Reset to a steady color

// ...


/*** SUPPORT **/

// ...
