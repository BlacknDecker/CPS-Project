#include <kilombo.h>
#include "includes/main.h"
#include "includes/communication_module.h"

extern USERDATA * mydata;


/***  SETUP ***/

void setupCommunicationManager(){
    mydata->mex_ex_state = PINGING;
    mydata->new_message_arrived = 0;
    mydata->attempts_counter = 0;
    // lifetime
    for(int i=0; i<MAX_NEIGHBOURS; i++){ mydata->lifetime[i] = 0; }
    // neighbours
    for(int i=0; i<MAX_NEIGHBOURS; i++){ mydata->neighbours[i] = 0; }
    // distance
    for(int i=0; i<MAX_NEIGHBOURS; i++){ mydata->distance[i] = MAX_INT; }
    // last message
    for(int i=0; i<MAX_NEIGHBOURS; i++){ mydata->last_message[i] = 0; }
    // out_queue
    for(int i=0; i<MAX_OUT_QUEUE; i++){ setup_message(&mydata->out_queue[i], NO_MESSAGE); }
    // outgoing message
    setup_message(&mydata->outgoing_mex, NO_MESSAGE);
    // transmitting
    setup_message(&mydata->transmitting_mex, NO_MESSAGE);
    // ping message
    setup_message(&mydata->ping_mex, PING);
    // ack message
    setup_message(&mydata->ack_mex, ACK);
    
    /** CALLBACKS **/
    kilo_message_tx = message_tx;
    kilo_message_rx = message_rx; 
}


/*** CORE FUNCTIONS ***/


/*** MESSAGE MANAGEMENT ROUTINE ***/
void communicationManager(){
    if(mydata->msg_ex_state == PINGING){  // Just pinging
      uploadWork();                       // Check if i have a message to send in the queue and in affirmative case take it
    }
    sendingAlgorithm();                   // Perform a step in the sending algorithm, based on the state
    checkExpiredData();                   // Check if data are still consistent
}


/*** SUPPORT FUNCTIONS ***/


/************ SEND ************************/

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

void pinging(){
    // printf("> %d PINGING\n", kilo_uid);
    mydata->attempts_counter = 0;                 // Reset Counter
    mydata->transmitting_mex = mydata->ping_mex;  // Change the transmitting msg
}

// Flood Strategy
void floodStrategy(){
    if(timeoutFires()){                   // Send until timeout expires
        mydata->msg_ex_state = PINGING;   // When expired go back to pinging
    }
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
    if(payload == mydata->last_message[sender]){
        return 0;
    }else{
        return 1;
    }
}

void notifyNewMessage(uint8_t sender){
    if(mydata->last_message[sender] != NO_MESSAGE){
        if(mydata->last_message[sender] != PING){
            mydata->new_message_arrived = 1;              // Set flag to notify 
        }
    }
}


/************ OUT QUEUE MANAGER ************************/
void pushRequest(uint8_t msg){
    for(int i=0; i<MAX_OUT_QUEUE; i++){
        if(mydata->out_queue[i] == 0){    // If the slot is free
            mydata->out_queue[i] = msg;   // Insert message
            // printf("> %d PUSHING REQUEST\n", kilo_uid);
            printMessageArchive();
            return;                                         // end
        }
    }
  // If no free slot found, ignore request
}

void uploadWork(){
    for(int i=0; i<MAX_OUT_QUEUE; i++){
        if(mydata->out_queue[i]){          // New message in the queue
            // printf("> %d UPLOADING WORK!\n", kilo_uid);
            mydata->outgoing_mex = mydata->out_queue[i];          // Move msg in the current working msg
            mydata->out_queue[i] =  NO_MESSAGE;                   // Remove message from the queue
            return;                                               // Get only a message
        }
    }
}


/************ DATA ARCHIVE MANAGER ************************/

void checkExpiredData(){
    for(int i=0; i<MAX_NEIGHBOURS; i++){
        if(mydata->neighbours[i]){                      // Check only neighbours
            if(isExpired(&mydata->msg_lifetime[i])){    // If data have lifetime has reached the end -> reset!
                if(kilo_uid==0){ printf("> %d: My neighbour %d is expired -> lifetime: %d\n", kilo_uid, i, mydata->lifetime[i]); } // DEBUG
                mydata->neighbours[i]   = 0;
                mydata->distance[i]     = MAX_INT;
                mydata->msg_payload[i] = 0;
                if(kilo_uid==0){ printf("Updated neighbours list: "); }                         // DEBUG
                if(kilo_uid==0){ printNeighbourList(); }                                        // DEBUG          
            }
        }
    }
}

// Check if data has become too old otherwise ages it
uint8_t isExpired(uint8_t * lifetime){
    if(*lifetime > DATA_LIFETIME){
        return 1; 
    } else {
        *lifetime += 1;
        return 0;
    }
}


/************ TIME WRAPPERS ************************/

// Starts timeout timer
void startTimer(){
    setTimer(COMMUNICATION_C, TIMEOUT);
}

// Check if timeout fires
uint8_t timeoutFires(){
    return isElapsed(COMMUNICATION_C);
}


/************ PRETTY PRINT ************************/
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

// Prints a "log" message
void printMessage(uint8_t sender){
    printf("Bot ");
    printTwoDigitNumber(kilo_uid);
    printf(" -> | sender: ");
    printTwoDigitNumber(sender);
    printf(" | msg: ");
    printThreeDigitNumber(mydata->last_message[sender]);
    //  printf(" | state: ");
    //  printTwoDigitNumber(mydata->state);
    printf(" |\n");
}

// Prints all non-void messages
void printMessageArchive(){
    printf("+++ MESSAGES +++\n");
    for(int i=0; i<MAX_NEIGHBOURS; i++){
        if(mydata->last_message[i] != 0){
            printMessage(i);
        }
    }
    printf("+++ +++ +++\n");
    fflush(NULL);
}

// Debug
void printNeighbourList(){
  printf("NEIGHBOURS: ");
  for(int i=0; i<MAX_NEIGHBOURS; i++){
      if(mydata->neighbours[i]){
          printf("%d->life:%d | ", i, mydata->lifetime[i]);
      }
  }
  printf("\n");
  fflush(NULL);
}