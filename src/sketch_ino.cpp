#include <unistd.h>
#include "core_simulation.h"

// la fonction d'initialisation d'arduino
void Board::setup(){
  // on configure la vitesse de la liaison
  Serial.begin(9600);

}

// la boucle de controle arduino
void Board::loop(){
  //Rajout d'un delai de 1s afin d'avoir les bonnes valeurs des capteurs au debut. 
  sleep(1);

}


