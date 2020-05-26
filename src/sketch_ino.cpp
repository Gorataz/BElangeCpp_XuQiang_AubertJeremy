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
  pinMode(4,OUTPUT); //servo - analog
  analogWrite(4,0);
  digitalWrite(0,LOW);
}

// la boucle de controle arduino
void Board::loop()
{
  MyApplication myApp;
  //Rajout d'un delai de 1s afin d'avoir les bonnes valeurs des capteurs au debut. 
  sleep(1);
    
  float static proxSavon(100);
  float static proxEau(100);
  vector<int> commandTab;
  vector<float> returnSensor;
  unsigned int i;
  int static compteurLED(0);
  int static etatClignotage(0);

  proxSavon=analogRead(1);
  returnSensor.push_back(proxSavon);
  proxEau=analogRead(3);
  returnSensor.push_back(proxEau);

  returnSensor.push_back(compteurLED);
  returnSensor.push_back(etatClignotage);
  
  commandTab=myApp.toDo(returnSensor);

  //
  for (i=0;i<commandTab.size()-2;i++) //les deux derniers éléments du tableau seront utilisé pour compteurLED et etatClignotage
  {
    switch(commandTab[i])
    {
      case 1:
        analogWrite(4,90);
        break;
      case 3:
        digitalWrite(2,HIGH);
        break;
      case 4:
        digitalWrite(2,LOW);
        break;
      case 5:
        analogWrite(4,0);
        break;
      case 6:
        digitalWrite(0,HIGH);
        break;
      case 7:
        digitalWrite(0,LOW);
        break;
    }
  }
  compteurLED=commandTab[commandTab.size()-2];
  etatClignotage=commandTab[commandTab.size()-1];   
  cout<<" "<<endl;
}

