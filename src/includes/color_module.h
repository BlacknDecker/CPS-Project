
/*** CORE  ***/

void setupColors();	// Setups global variables used by this module.


uint8_t getColor(uint8_t colorCode); 									// Get RGB Color by Color Code
void setColor(uint8_t colorCode);										// Give a given color to the bot (by color code)
void blink(uint8_t off_delay, uint8_t on_delay, uint8_t colorCode);		// Blinks (NB: must be called on every loop cycle!) NB.2: delays are in kiloticks (32 kiloticks = 1 sec)
void resetBlink(uint8_t colorCode);										// Reset to a steady color

// ...


/*** SUPPORT **/

// ...
