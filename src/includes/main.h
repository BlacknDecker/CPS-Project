
#include <stdio.h> // for printf
/************* CONSTANTS ********************/

// BOOLEAN
#define TRUE 1
#define FALSE 0

// COLORS
#define OFF 0
#define RED 1
#define GREEN 2
#define BLUE 3
#define WHITE 4
#define PURPLE 5
#define YELLOW 6
#define ORANGE 7

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

// STANDARD MESSAGES
#define NO_MESSAGE 0
#define PING 1 

// TIME
#define TIMERS_NUMBER 4


/************* ENUMS ********************/

// MOTION TYPE
typedef enum {
    STOP,
    FORWARD,
    LEFT,
    RIGHT
} motion_t;


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


/************* GLOBAL VARIABLES ********************/

typedef struct {

    /*** TIME ***/
    
    uint8_t timer[TIMERS_NUMBER];

    /*** *** *** ***/

    /*** COLORS ***/

    uint8_t color_code[8];
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
    uint8_t msg_lifetime[MAX_NEIGHBOURS];   // Time from the last time a mex arrived (in kiloticks)
    uint8_t msg_payload[MAX_NEIGHBOURS];    // contains payload of the last mex received from the IDth bot

    /*** *** *** ***/

    /*** MESSAGES ***/

    uint8_t new_message_arrived;                // Flag to notify new messages
    message_exchange_state_t msg_ex_state;      // Message Exchange State (to move in the finite state machine)
    uint8_t out_queue[MAX_OUT_QUEUE];           // Contains the messages waiting to be transmitted 
    message_t outgoing_message;                 // Message to transmit

    /*** *** *** ***/

    /*** MOVEMENT ***/

    // Put variables here

    /*** *** *** ***/


} USERDATA;

