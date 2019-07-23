
/*** CORE  ***/

void setupCommunicationManager();   // Setups global variables used by this module.
void communicationManager();        // Communication Routine

//Flood a message. Insert into flood_state the state of the message exchange
// NB: flood_state must be initialized to START
void floodMessage(uint8_t msg_payload, msg_flood_state_t * flood_state);


/*** SUPPORT **/


// FLOODING STATES
msg_flood_state_t startFlooding(uint8_t msg_payload);
msg_flood_state_t waitFlooding(uint8_t msg_payload);
msg_flood_state_t floodingProgress(uint8_t msg_payload);

// SEND
void sendingAlgorithm();
void setupPinging();
void setupColor();
void floodingMessage();
void setup_message(message_t * msg, uint8_t payload);
message_t * message_sender();

// RECEIVE
void message_receiver(message_t *m, distance_measurement_t *d);
uint8_t getSenderID(message_t m);
uint8_t getMessagePayload(message_t m);
uint8_t isNewMessage(uint8_t sender, uint8_t payload);
void notifyNewMessage(uint8_t sender);

// OUT QUEUE MANAGER
void pushRequest(uint8_t msg_payload);
void uploadMessage();

// DATA ARCHIVE MANAGER
void checkExpiredData();
uint8_t isExpired(uint8_t * lifetime);

// TIME WRAPPERS
void startCountdown();
uint8_t timeoutFires();

// PRETTY PRINT
void printMessageArchive();
void printMessage(uint8_t sender);
void printTwoDigitNumber(uint8_t num);
void printThreeDigitNumber(uint8_t num);
void printNeighbourList();



