#include "core_simulation.h"
#include "mydevices.h"


int main(){
  // creation d'une board
  Board esp8266;
  // achat des senseurs et actionneurs
  DigitalActuatorLED led1(DELAY);
  I2CActuatorScreen screen;
  
  // branchement des capteurs actionneurs
  esp8266.pin(0,led1);
  
  // allumage de la carte
  esp8266.run();
  return 0;
}


