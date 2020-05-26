
#include <thread>
#include <unistd.h>
#include <string.h>

#include "core_simulation.h"

// class BoardException
int BoardException::get(){return num;}

string BoardException::text(){
  string s;
  switch(num){
  case SPEED : s=string("mauvaise vitesse de la laison terminal");break;
  case INOUT : s=string("mauvaise utilisation du sens de l'entree/sortie"); break;
  case ADDRESS : s=string("mauvaise adresse de la pin"); break;
  case SIZEXC : s=string("taille erronee"); break;
  case EMPTY: s=string("zone vide"); break;
  default: s=string("erreur inconnue");
  }
  return s;
}

// classe terminale
void Terminal::begin(int speed){
  if (speed!=9600){
    cerr << "wrong speed\n";
    throw BoardException(SPEED);
  }
}
void Terminal::println(string s){
  cout <<"Serial: "<< s<<endl;
}

// representatoin du bus I2C
I2C::I2C(){
  for(int i=0;i<MAX_I2C_DEVICES;i++){
    registre[i]=new char[I2C_BUFFER_SIZE];
    vide[i]=true;
  }
}

bool I2C::isEmptyRegister(int addr){
  bool result=true;
  if ((addr>=0)&&(addr<MAX_I2C_DEVICES))
    result=vide[addr];
  else
    throw BoardException(ADDRESS);
  return result;
}

int I2C::write(int addr, char* bytes, int size){
  if ((addr<0)||(addr>=MAX_I2C_DEVICES))
    throw BoardException(ADDRESS);
  if ((size<0)||(size>I2C_BUFFER_SIZE))
    throw BoardException(SIZEXC);
  tabmutex[addr].lock();
  memcpy(registre[addr],bytes,size*sizeof(char));
  vide[addr]=false;
  tabmutex[addr].unlock();
  return size;
}

int I2C::requestFrom(int addr, char* bytes, int size){
  int result =0;
  if ((addr<0)||(addr>=MAX_I2C_DEVICES))
    throw BoardException(ADDRESS);
  if ((size<0)||(size>I2C_BUFFER_SIZE))
    throw BoardException(SIZEXC);
  if (vide[addr]==false){
    tabmutex[addr].lock();
    memcpy(bytes,registre[addr],size*sizeof(char));
    vide[addr]=true;
    tabmutex[addr].unlock();
    result =size;
  }
  return result;
}

char * I2C::getRegistre(int addr){
  if ((addr<0)||(addr>=MAX_I2C_DEVICES))
    throw BoardException(ADDRESS);
  return (registre[addr]);
}

bool* I2C::getVide(int addr){
  if ((addr<0)||(addr>=MAX_I2C_DEVICES))
    throw BoardException(ADDRESS);
  return (&vide[addr]);
}

// classe generique reprenstant un capteur/actionneur
Device::Device(){
  ptrtype=NULL;
  ptrmem=NULL;
  i2caddr=-1;
  i2cbus=NULL;
}

void Device::run(){
  while(1){
    cout << "empty device\n";
    sleep(3);
  }
}

void Device::setPinMem(unsigned short* ptr,enum typeio *c){
  ptrtype=c;
  ptrmem=ptr;
}

void Device::setI2CAddr(int addr, I2C * bus){
  i2caddr=addr;
  i2cbus=bus;
}

// classe representant une carte arduino
void Board::run(){
  try{
    setup();
    while(1) loop();
  }
  catch(BoardException e){
    cout <<"exception: "<<e.get() <<endl;
  }
}

void Board::pin(int p, Device& s){
  s.setPinMem(&io[p], &stateio[p]);
  tabthreadpin[p]=new thread(&Device::run,&s);
  
}

void Board::pinMode(int p,enum typeio t){
  stateio[p]=t;
}

void Board::digitalWrite(int i, int l){
  if (stateio[i]==OUTPUT)
    io[i]=l;
  else
    throw BoardException(INOUT);
}

int Board::digitalRead(int i){
  int   result=0;
  if (stateio[i]==INPUT)
    result= io[i];
  else
    throw BoardException(INOUT);
  return result;
}

void Board::analogWrite(int i, int l){
  if (stateio[i]==OUTPUT)
    io[i]=l;
  else
    throw BoardException(INOUT);
}

int Board::analogRead(int i){
  int   result=0;
  if (stateio[i]==INPUT)
    result= io[i];
  else
    throw BoardException(INOUT);
  return result;
}

void Board::i2c(int addr,Device& dev){
  if ((addr<0)||(addr>=MAX_I2C_DEVICES))
    throw BoardException(ADDRESS);
  dev.setI2CAddr(addr,&bus);
  tabthreadbus[addr]=new thread(&Device::run,&dev);
}

//Classe MyApplication
MyApplication::MyApplication():nbSensor(0),returnSensor(0),etatProxEau(0),etatProxSavon(0),etatClignotage(0),cptLED(0){}

vector<int> MyApplication::toDo(vector<float> const& returnSensor) //Dans cette version, on ne prends pas en compte 
{
  vector<int> commandTab;

  if (returnSensor[0]<10 && !etatProxSavon) //activer la distribution de savon
  {
    commandTab.push_back(1);
    etatProxSavon=1; //1 signifie qu'il qu'on a été à une distance <10cm
    etatClignotage=1; //on démarre la séquence de clignotage de la LED
  }
  if (returnSensor[1]<10 && !etatProxEau) //démarrer l'écoulement de l'eau
  {
    commandTab.push_back(3);
    etatProxEau=1;
  }
  if (returnSensor[1]>10 && etatProxEau) //arrêter l'écoulement de l'eau
  {
    commandTab.push_back(4);
    etatProxEau=0;
  }
  if (returnSensor[0]>10 && etatProxSavon) //arrêter la distribution de savon
  {
    commandTab.push_back(5);
    etatProxSavon=0;
  }
  if (etatClignotage && cptLED==40) //arrêt du clignotage et la LED reste allumée
  {
    commandTab.push_back(6);
    etatClignotage=0;
    cptLED=0;
  }
  if (etatClignotage) //gestion du clignotage
  {
    if (!cptLED%2)
      commandTab.push_back(6);
    if (cptLED%2)
      commandTab.push_back(7);
    cptLED++;
  }

  return (commandTab);
}

/*
Tableau des commandes :
1 -> DebutDistributionSavon     (=sens 1, angle 90)
3 -> OuvertureEau               (=flow HIGH)
4 -> FermetureEau               (=flow LOW)
5 -> FinDistributionSavon       (=sens 0, angle 0)
6 -> LED allummée
7 -> LED éteinte
*/
