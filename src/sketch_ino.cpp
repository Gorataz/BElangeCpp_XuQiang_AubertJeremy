#include <unistd.h>
#include "core_simulation.h"

// la fonction d'initialisation d'arduino
void Board::setup(){
  // on configure la vitesse de la liaison
  Serial.begin(9600);
  pinMode(0,OUTPUT); //LED - digital
  pinMode(2,OUTPUT); //valve - digital
  pinMode(1,INPUT); //ultrason savon - analog
  pinMode(3,INPUT); //ultrason eau - analog
}

// la boucle de controle arduino
void Board::loop()
{
  MyApplication myApp;

  //Rajout d'un delai de 1s afin d'avoir les bonnes valeurs des capteurs au debut. 
  sleep(1);

  float static proxSavon(1000);
  float static proxEau(1000);
  vector<int> commandTab;
  vector<float> returnSensor;
  int i(0);

  proxSavon=analogRead(1);
  returnSensor.push_back(proxSavon);
  proxEau=analogRead(3);
  returnSensor.push_back(proxEau);

  commandTab=myApp.toDo(returnSensor);

  //
  for (i;i<commandTab.size();i++)
  {
    switch(commandTab[i])
    {
      case 1:
        //servomoteur
        break;
      case 3:
        digitalWrite(2,HIGH);
        break;
      case 4:
        digitalWrite(2,LOW);
        break;
      case 5:
        //servomoteur
        break;
      case 6:
        digitalWrite(0,HIGH);
        break;
      case 7:
        digitalWrite(0,LOW);
        break;
    }
  }
  
}

