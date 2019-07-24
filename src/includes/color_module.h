
/*** CORE  ***/

void setupColors();	// Setups global variables used by this module.

void setColor(uint8_t rgbColor);										// Set bot Color
void blink(uint8_t off_delay, uint8_t on_delay, uint8_t rgbColor);		// Blinks (NB: must be called on every loop cycle!) NB.2: delays are in kiloticks (32 kiloticks = 1 sec)
void resetBlink();														// Reset Blink values (NB: call after done blinking!)
void setStableColor(uint8_t rgbColor);									// Reset to a steady color
uint8_t getColorFromMessage(uint8_t msg);								// Convert a  color message into an rgb color value	
uint8_t colorToMessage(uint8_t rgbColor);		
uint8_t numberToColor(uint8_t r_num);							// Convert a number into its colo

// ...


/*** SUPPORT **/

// ...
