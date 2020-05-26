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
  //Rajout d'un delai de 1s afin d'avoir les bonnes valeurs des capteurs au debut. 
  sleep(1);

  float static proxSavon(1000);
  float static proxEau(1000);
  int etatValve(0); //0 si fermer, 1 sinon

  proxSavon=analogRead(1);
  proxEau=analogRead(3);

  if (proxSavon<10)
  {
    cout<<"Nous sommes dans la boucle 1"<<endl;
    //commande 1 du servomoteur
    sleep(2);
    //comande 2 du servomoteur
  }
  else if (proxEau<10)
  {
    digitalWrite(2,HIGH);
    etatValve=1;
    sleep(5);
  }
  else if (proxEau>10 && etatValve)
  {
    digitalWrite(2,LOW);
    etatValve=0;
  }
  else
    cout<<"R.A.S"<<endl;
  
}

