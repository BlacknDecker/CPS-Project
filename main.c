#include <kilombo.h>
#include "main.h"

#include "time_module.h"
#include "game_module.h"
#include "communication_module.h"
#include "movement_module.h"

#include <stdio.h> // for printf


REGISTER_USERDATA(USERDATA)


/******* SETUP,LOOP,MAIN *******************/

void loop(){
  gameManager();
  communicationManager();
  movementManager();
}


void setup(){
  setupTimeManager();
  setupGameManager();
  setupCommunicationManager();
  setupMovementManager();
}


int main() {
  kilo_init();
  kilo_start(setup, loop);
  return 0;
}


