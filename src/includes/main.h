
#include <stdio.h> // for printf
/************* CONSTANTS ********************/

// BOOLEAN
#define TRUE 1
#define FALSE 0

// COLORS (rgb)
#define OFF RGB(0,0,0)
// service
#define WHITE RGB(3,3,3)
#define GREEN RGB(0,3,0)
#define RED RGB(3,0,0)
// others
#define BLUE RGB(0,0,3)
#define PURPLE RGB(1,0,1)
#define YELLOW RGB(2,2,0)
#define ORANGE RGB(2,1,0)
#define CYAN RGB(0,1,1)

// DISTANCE ZONEs
#define DANGER_D 45
#define WARNING_D 60
#define SAFE_D 70

// GAME 
#define MAX_INT 255
#define MAX_NEIGHBOURS 10

// MESSAGE EXCHANGE
#define MAX_OUT_QUEUE 3
#define TIMEOUT 64
#define DATA_LIFETIME 48

// -- MESSAGE CODES --
#define VOID_MSG 0
#define PING_MSG 1 
#define ACK_MSG 2
//
#define BLUE_MSG 3
#define PURPLE_MSG 4
#define YELLOW_MSG 5
#define ORANGE_MSG 6
#define CYAN_MSG 7
//...
#define YES_MSG 11
#define NO_MSG 12
#define GO_MSG 13
#define RUNNER_MSG 14
// -- ------------- --

// TIME
#define TIMERS_NUMBER 4


/************* ENUMS ********************/

// CLOCKS TYPES
typedef enum {
    BLINK_C,
    MOVE_C,
    COMMUNICATION_C,
    GAME_C
} clock_type_t;


// MESSAGE MODULE STATES
typedef enum{
    PINGING,
    BUSY,
    DONE
} message_exchange_state_t;


// Return types for message sending function
typedef enum{
    ERROR,
    START,
    WAIT,
    IN_PROGRESS,
    FINISH
} msg_flood_state_t;


// Blink phases
typedef enum{
    BLINK_OFF,
    BLINK_ON,
    STEADY
} blink_state_t;


// Game States
typedef enum{
    START_PHASE,
    WAIT_PHASE,
    FLOOD_PHASE,
    END_PHASE
} game_state_t;


// MOTION TYPE
typedef enum {
    STOP,
    FORWARD,
    LEFT,
    RIGHT
} motion_t;

/************* GLOBAL VARIABLES ********************/

typedef struct {

    /*** TIME ***/
    
    uint8_t timer[TIMERS_NUMBER];

    /*** *** *** ***/

    /*** COLORS ***/

    blink_state_t blink_state;
    uint8_t my_color;                       // Code color of the color that i'm currently displaying

    /*** *** *** ***/

    /*** GAME ***/

    game_state_t game_state;                // Game state
    msg_flood_state_t game_msg_state;       // Flooding message state
    // Put variables here

    /*** *** *** ***/

    /*** DATA ARCHIVE ***/

    uint8_t neighbours[MAX_NEIGHBOURS];     // neighbours[ID] == 1 if kilobot_ID is in range, else == 0
    uint8_t distance[MAX_NEIGHBOURS];       // if neighbourss[ID] is in range, show distance, else == MAX_INT
    uint8_t msg_lifetime[MAX_NEIGHBOURS];   // Time from the last time a msg arrived (in kiloticks)
    uint8_t msg_payload[MAX_NEIGHBOURS];    // contains payload of the last msg received from the IDth bot

    uint8_t last_msg_payload;               // Contains the payload of the last msg with a meaningful content received
    
    /*** *** *** ***/

    /*** MESSAGE EXCHANGE ***/

    uint8_t new_message_arrived;                // Flag to notify new messages
    message_exchange_state_t msg_ex_state;      // Message Exchange State (to move in the finite state machine)
    uint8_t out_queue[MAX_OUT_QUEUE];           // Contains the messages waiting to be transmitted 
    message_t outgoing_message;                 // Message to transmit

    /*** *** *** ***/

    /*** MOVEMENT ***/

    // Put variables here

    /*** *** *** ***/


} USERDATA;

