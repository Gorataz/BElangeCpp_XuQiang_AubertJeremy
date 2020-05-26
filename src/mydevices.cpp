
#include "mydevices.h"

using namespace std;

//classe DigitalActuatorLED
DigitalActuatorLED::DigitalActuatorLED(int t):Device(),state(LOW),temps(t){
}

int DigitalActuatorLED::StateLED()
{
  return(state);
}

void DigitalActuatorLED::run(){
  while(1)
  {
    if(ptrmem!=NULL)
      state=*ptrmem;
    if (state==LOW)
      cout << "LED OFF  ○○○○○○○○○○○○"<<endl;
    else
      cout << "LED ON •••••••••••"<<endl;
    sleep(temps);
    }
}

// classe I2CActuatorScreen
I2CActuatorScreen::I2CActuatorScreen ():Device(){
  }

void I2CActuatorScreen::run(){
  while(1){
    if ( (i2cbus!=NULL)&&!(i2cbus->isEmptyRegister(i2caddr))){
      Device::i2cbus->requestFrom(i2caddr, buf, I2C_BUFFER_SIZE);
      cout << "---screen :"<< buf << endl;
    }
    sleep(1);
    }
}

//classe AnalogActuatorServo
AnalogActuatorServo::AnalogActuatorServo(int t):Device(),sens(0),angle(0),etatAngle(0),refresh(t){}

void AnalogActuatorServo::run() // Question : on a eu un problème avec la valeur dans angle, ça a été résolu en écrivant UNE FOIS un analogWrite, puis on l'a supprimé et on avait bien au final l'angle à 0...
{
  while(1) //inclure un peu de "dynamique" dans la simulation
  {
    if (ptrmem!=NULL)
    {
      angle=*ptrmem;
    }
    if (angle>etatAngle && etatAngle<90) //etatAngle=[-90,+90]
    {
      etatAngle+=10;
      sens=1;
    }
    else if (angle<etatAngle && etatAngle>0)
    {
      etatAngle-=10;
      sens=0;
    }
    cout<<"Sens servo : "<<sens<<endl;
    cout<<" "<<endl;
    cout<<"Angle servo : "<<etatAngle<<endl;
    sleep(refresh);
  }
}

//classe AnalogSensorUltrason
AnalogSensorUltrason::AnalogSensorUltrason(string nomFichierUser):Device(), prox(100), fichier(nomFichierUser){}

void AnalogSensorUltrason::run()
{
  while(1)
  {
    if(ifstream(fichier))
      prox=5;
    else
      prox=100;
    *ptrmem=prox;
    sleep(1);
  }
}

//classe DigitalActuatorValve
DigitalActuatorValve::DigitalActuatorValve(int t):Device(),flow(LOW),refresh(t){} 

void DigitalActuatorValve::run()
{
  while(1)
  {
    if(ptrmem!=NULL) //Permet le DigitalWrite
      flow=*ptrmem;
    if (flow==HIGH) 
      cout<<"Valve open ▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓"<<endl;
    else if (flow==LOW)
      cout<<"Valve close ░░░░░░░░░░░░░░░"<<endl;    
    sleep(refresh);    
  }
}

