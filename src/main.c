#include <kilombo.h>
#include "includes/main.h"

#include "includes/time_module.h"
#include "includes/game_module.h"
#include "includes/communication_module.h"
#include "includes/movement_module.h"

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


