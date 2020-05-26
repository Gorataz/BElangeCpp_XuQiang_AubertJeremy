#include "core_simulation.h"
#include "mydevices.h"


int main(){
  // creation d'une board
  Board esp8266;
  // achat des senseurs et actionneurs
  DigitalActuatorLED led1(TIMER_SENSOR);
  I2CActuatorScreen screen;
  DigitalActuatorValve valve(TIMER_SENSOR);
  AnalogSensorUltrason proxSavon("presenceSavon.txt");
  AnalogSensorUltrason proxEau("presenceEau.txt");
  AnalogActuatorServo servo(TIMER_SENSOR);
  
  // branchement des capteurs actionneurs
  esp8266.pin(0,led1);
  esp8266.pin(2,valve);
  esp8266.pin(1,proxSavon);
  esp8266.pin(3,proxEau);
  esp8266.pin(4,servo);
  
  // allumage de la carte
  esp8266.run();
  return 0;
}


