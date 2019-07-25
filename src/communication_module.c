#include <kilombo.h>
#include "includes/main.h"
#include "includes/time_module.h"
#include "includes/communication_module.h"

extern USERDATA * mydata;


/***  SETUP ***/

void setupCommunicationManager(){
    mydata->msg_ex_state = PINGING;
    mydata->new_message_arrived = FALSE;
    // lifetime
    for(int i=0; i<MAX_NEIGHBOURS; i++){ mydata->msg_lifetime[i] = 0; }
    // neighbours
    for(int i=0; i<MAX_NEIGHBOURS; i++){ mydata->neighbours[i] = FALSE; }
    // distance
    for(int i=0; i<MAX_NEIGHBOURS; i++){ mydata->distance[i] = MAX_INT; }
    // last message
    for(int i=0; i<MAX_NEIGHBOURS; i++){ mydata->msg_payload[i] = VOID_MSG; }
    // out_queue
    for(int i=0; i<MAX_OUT_QUEUE; i++){ mydata->out_queue[i] = VOID_MSG; }
    // last message received
    mydata->last_msg_payload = 0;
    // outgoing message
    setup_message(&mydata->outgoing_message, PING_MSG);
    
    /** CALLBACKS **/
    kilo_message_tx = message_sender;
    kilo_message_rx = message_receiver; 
}


/*** MESSAGE MANAGEMENT ROUTINE ***/
void communicationManager(){
    if(mydata->msg_ex_state == PINGING){  // Just pinging
      uploadMessage();                    // Check if i have a message to send in the queue and in affirmative case take it
    }
    sendingAlgorithm();                   // Perform a step in the sending algorithm, based on the state
    checkExpiredData();                   // Check if data are still consistent
}


/*** CORE FUNCTIONS ***/

//Flood a message. Insert into flood_state the state of the message exchange
// NB: flood_state must be initialize to START
void floodMessage(uint8_t msg_payload, msg_flood_state_t * flood_state){
    switch(*flood_state){
        case START: *flood_state = startFlooding(msg_payload); break;   // Put message in queue
        case WAIT:  *flood_state = waitFlooding(msg_payload); break;    // Wait the message to be retrieved from out queue
        case IN_PROGRESS: *flood_state = floodingProgress(msg_payload); break;
        case FINISH: *flood_state = FINISH; break;
        default: *flood_state = ERROR;
    }
}

// Set a new message to ping. 
// NB: no need to be called over time, just call it the first time and will start pinging until a new message is sent!
void pingMessage(uint8_t msg_payload){
    // printf("> %d PINGING\n", kilo_uid);
    mydata->msg_ex_state = PINGING;                            // Change state
    setup_message(&mydata->outgoing_message, msg_payload);     // Change the transmitting msg
}


/*** SUPPORT FUNCTIONS ***/

/************ FLOODING STATES ************************/

msg_flood_state_t startFlooding(uint8_t msg_payload){
    //if(kilo_uid == 1){ printf(" > START FLOODING\n\n"); }
    pushRequest(msg_payload);                           // Push message in out queue
    if( mydata->msg_ex_state == PINGING ){              // Check if i will wait in queue
        return IN_PROGRESS;
    }else{
        return WAIT;
    }
}

msg_flood_state_t waitFlooding(uint8_t msg_payload){
    //if(kilo_uid == 1){ printf(" > WAIT FLOODING\n\n"); }
    uint8_t out_payload = getMessagePayload(mydata->outgoing_message);  // Get Outgoing message payload
    if(out_payload == msg_payload){ 
        return IN_PROGRESS;
    }else{
        return WAIT;
    }
}

msg_flood_state_t floodingProgress(uint8_t msg_payload){
    if(mydata->msg_ex_state == DONE){
        //if(kilo_uid == 1){ printf(" > FINISH FLOODING\n\n"); }
        return FINISH;
    }else{
        return IN_PROGRESS;
    }
}


/************ SEND ************************/

// Decide which action to do based on the status
void sendingAlgorithm(){
    switch(mydata->msg_ex_state){
        case PINGING: break;                    // Pings 
        case COLOR: break;                      // send my color
        case BUSY: floodingMessage(); break;    // Flood a message
        default: setupPinging();                // Goes back to pinging
    }
}

void setupPinging(){
    // printf("> %d PINGING\n", kilo_uid);
    mydata->msg_ex_state = PINGING;                         // Change state
    setup_message(&mydata->outgoing_message, PING_MSG);     //Change the transmitting msg
}

// Flood a message for a while
void floodingMessage(){
    if(timeoutFires()){             // Send until timeout expires
       mydata->msg_ex_state = DONE; // Change state
    }
}


// Generate a message
void setup_message(message_t * msg, uint8_t payload){
    msg->type = NORMAL;                     // Message type
    uint8_t sender = kilo_uid << 4;         // Add ID on the most significant 4 bits (on the left)
    msg->data[0] = sender + payload;        // Payload
    msg->crc = message_crc(msg);            // Checksum
}


// Callback to send a message
message_t * message_sender(){  
    return &mydata->outgoing_message;
}


/************ RECEIVE ************************/

// Callback to Receive messages
void message_receiver(message_t *m, distance_measurement_t *d){
    /* Store message in the archive */
    uint8_t snd = getSenderID(*m);                      // Get Sender ID
    mydata->msg_lifetime[snd] = 0;                      // Reset lifetime
    mydata->neighbours[snd] = 1;                        // Update neighbours
    mydata->distance[snd] = estimate_distance(d);       // Update distance
    uint8_t tmp_payload = getMessagePayload(*m);
    if(isNewMessage(snd, tmp_payload)){                 // If is a new message
        mydata->msg_payload[snd] = tmp_payload;         // Get Payload
        notifyNewMessage(snd);                          // Notify only if is a relevant message
    }
    // printMessageArchive();                           // DEBUG only
}

// Get the sender uid
uint8_t getSenderID(message_t m){
    uint8_t snd_id = m.data[0];   // Raw message
    snd_id >>= 4;                 // Clear the payload data
    return snd_id;
}

// Get the message payload
uint8_t getMessagePayload(message_t m){
    uint8_t payload = m.data[0];  // Raw message
    payload <<= 4;                // Clear the ID data
    payload >>= 4;                // Make the message readable again
    return payload;
}

// To avoid duplicates
uint8_t isNewMessage(uint8_t sender, uint8_t payload){
    if(payload == mydata->msg_payload[sender]){
        return FALSE;
    }else{
        return TRUE;
    }
}

void notifyNewMessage(uint8_t sender){
    if(mydata->msg_payload[sender] != VOID_MSG){
        if(mydata->msg_payload[sender] != PING_MSG){
            mydata->last_msg_payload = mydata->msg_payload[sender]; // Update 
            mydata->new_message_arrived = TRUE;                     // Set flag to notify 
        }
    }
}


/************ OUT QUEUE MANAGER ************************/
// Insert a message in out_queue (if space available)
void pushRequest(uint8_t msg_payload){
    for(int i=0; i<MAX_OUT_QUEUE; i++){
        if(mydata->out_queue[i] == 0){            // If the slot is free
            mydata->out_queue[i] = msg_payload;   // Insert message
            // printf("> %d PUSHING REQUEST\n", kilo_uid);
            // printMessageArchive();                // Debug
            return;                               // End
        }
    }
  // If no free slot found, ignore request
}

// Remove a message from out queue and put it in out_msg
void uploadMessage(){
    for(int i=0; i<MAX_OUT_QUEUE; i++){
        if(mydata->out_queue[i]){                                            // Found a message in the queue
            // printf("> %d UPLOADING WORK!\n", kilo_uid);
            setup_message(&mydata->outgoing_message, mydata->out_queue[i]);  // Copy msg in the message to transmit
            startCountdown();                                                // Reset timeout timer
            mydata->out_queue[i] = VOID_MSG;                                 // Remove message from the queue
            mydata->msg_ex_state = BUSY;
            return;                                                          // Get only a message
        }
    }
}


/************ DATA ARCHIVE MANAGER ************************/

void checkExpiredData(){
    for(int i=0; i<MAX_NEIGHBOURS; i++){
        if(mydata->neighbours[i]){                      // Check only neighbours
            if(isExpired(&mydata->msg_lifetime[i])){    // If data have lifetime has reached the end -> reset!
                if(kilo_uid==0){ printf("> %d: My neighbour %d is expired -> lifetime: %d\n", kilo_uid, i, mydata->msg_lifetime[i]); } // DEBUG
                mydata->neighbours[i]   = FALSE;
                mydata->distance[i]     = MAX_INT;
                mydata->msg_payload[i]  = VOID_MSG;
                if(kilo_uid==0){ printf("Updated neighbours list: "); }                         // DEBUG
                if(kilo_uid==0){ printNeighbourList(); }                                        // DEBUG          
            }
        }
    }
}

// Check if data has become too old otherwise ages it
uint8_t isExpired(uint8_t * lifetime){
    if(*lifetime > DATA_LIFETIME){
        return TRUE; 
    } else {
        *lifetime += 1;
        return FALSE;
    }
}


/************ TIME WRAPPERS ************************/

// Starts timeout timer
void startCountdown(){
    setTimer(COMMUNICATION_C, TIMEOUT);
}

// Check if timeout fires
uint8_t timeoutFires(){
    return isElapsed(COMMUNICATION_C);
}


/************ PRETTY PRINT ************************/

// Prints all non-void messages
void printMessageArchive(){
    printf("+++ MESSAGES +++\n");
    for(int i=0; i<MAX_NEIGHBOURS; i++){
        if(mydata->msg_payload[i] != 0){
            printMessage(i);
        }
    }
    printf("+++ +++ +++\n");
    fflush(NULL);
}

// Prints a "log" message
void printMessage(uint8_t sender){
    printf("Bot ");
    printTwoDigitNumber(kilo_uid);
    printf(" -> | sender: ");
    printTwoDigitNumber(sender);
    printf(" | msg: ");
    printThreeDigitNumber(mydata->msg_payload[sender]);
    //  printf(" | state: ");
    //  printTwoDigitNumber(mydata->state);
    printf(" |\n");
}

// util function
void printTwoDigitNumber(uint8_t num){
    if(num < 10)
        printf("0");
    printf("%d", num);
}

// util function
void printThreeDigitNumber(uint8_t num){
    if(num < 100)
        printf("0");
    if(num < 10)
        printf("0");
    printf("%d", num);
}

// Debug
void printNeighbourList(){
  printf("NEIGHBOURS: ");
  for(int i=0; i<MAX_NEIGHBOURS; i++){
      if(mydata->neighbours[i]){
          printf("%d->life:%d | ", i, mydata->msg_lifetime[i]);
      }
  }
  printf("\n");
  fflush(NULL);
}
