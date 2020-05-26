#include "core_simulation.h"
#include "mydevices.h"


int main(){
  // creation d'une board
  Board esp8266;
  // achat des senseurs et actionneurs
  DigitalActuatorLED led1(DELAY);
  I2CActuatorScreen screen;
  DigitalActuatorValve valve(DELAY);
  AnalogSensorUltrason proxSavon;
  AnalogSensorUltrason proxEau;
  
  // branchement des capteurs actionneurs
  esp8266.pin(0,led1);
  esp8266.pin(2,valve);
  esp8266.pin(1,proxSavon);
  esp8266.pin(3,proxEau);
  
  // allumage de la carte
  esp8266.run();
  return 0;
}


