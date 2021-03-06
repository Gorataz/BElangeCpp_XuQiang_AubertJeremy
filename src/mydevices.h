#ifndef MYDEVICES_H
#define MYDEVICES_H
#define luminosite_environnement 200 //200 lux
#define distance_ultrason 5.0 //5cm

#include <iostream>
#include <thread>
#include <unistd.h>
#include <string.h>
#include "core_simulation.h"



// exemple d'actionneur digital : une led, ne pas oublier d'heriter de Device
class DigitalActuatorLED: public Device {
private:
  // etat de la LED
  int state;
  // temps entre 2 affichage de l etat de la led
  int temps;
  
public:
    // initialisation du temps de rafraichissement
  DigitalActuatorLED(int t);
  //Permet de renvoyer l'etat de la LED à une autre fonction
  int StateLED();
  //thread permettant l'attente des 20 secondes et le signalement par LED
  void Attente20sec();
  // thread representant l'actionneur et permettant de fonctionner independamment de la board
  virtual void run();
};

// exemple d'actionneur sur le bus I2C permettant d'echanger des tableaux de caracteres : un ecran, ne pas oublier d'heriter de Device
class I2CActuatorScreen : public Device{
protected:
    // memorise l'affichage de l'ecran
  char buf[I2C_BUFFER_SIZE];
  
public:
  // constructeur
  I2CActuatorScreen ();
  // thread representant le capteur et permettant de fonctionner independamment de la board
  virtual void run();
};

class AnalogActuatorServo: public Device //Oulala c'est compliqué..........................
{
  private:
    //sens de rotation du servomoteur
    int sens;
    //angle de rotation du servomoteur - commande
    int angle; 
    //angle de rotation du servomoteur - etat
    int etatAngle;
    //
    int refresh;
  public:
    //constructeur
    AnalogActuatorServo(int t);
    //thread representant l'actionneur et permettant de fonctionner independamment de la board
    virtual void run();
};

class AnalogSensorUltrason: public Device 
{
  protected:
    float barriere=distance_ultrason;
  private:
    float prox;
    int serial; //distinguer les deux ultrasons -> 1 : savon, 2 : eau
    string fichier;
  public:
    //Constructeur
    AnalogSensorUltrason(string nomFichierUser);
    //Methode permettant le retour de distance
    int AfficheDistance();
    //thread representant le capteur et permettant de fonctionner independamment de la board - va faire un refresh toutes les .5sec de la distance enregistrée par les ultrasons
    virtual void run();
};

class DigitalActuatorValve: public Device //it's over DigitalActuatorValve, I have the high pointer
{
  private:
    //debit sortant (LOW : pas d'ecoulement, HIGH : ecoulement max)
    int flow;
    //temps de refresh / delay
    int refresh;
  public:
    //Constructeur
    DigitalActuatorValve(int t);
    //thread representant l'actionneur et permettant de fonctionner independamment de la board - va permettre l'ouverture/la fermeture de la valve (pour la distribution d'eau)
    virtual void run();
};


#endif
