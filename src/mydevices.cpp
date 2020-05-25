
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
  while(1){
    if(ptrmem!=NULL)
      state=*ptrmem;
    if (state==LOW)
      cout << "((((eteint))))\n";
    else
    cout << "((((allume))))\n";
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
AnalogActuatorServo::AnalogActuatorServo():Device(),sens(0),angle(0){}

void AnalogActuatorServo::service() //à "déplacer" dans run()
{
  sens=1;
  angle=90;
  sleep(2);
  sens=0;
  angle=0;
}

int AnalogActuatorServo::AfficheEtat() //pour utiliser dans sketch_ino.cpp
{
  return(etatAngle);
}

void AnalogActuatorServo::run()
{
  while(1) //inclure un peu de "dynamique" dans la simulation
  {
    if (ptrmem!=NULL)
    {
      angle=*ptrmem;
      if (angle>etatAngle) //etatAngle=[-90,+90]
        etatAngle++;
      else if (angle<etatAngle)
        etatAngle--;
      sleep(0.1);
    }
  }
}

//classe AnalogSensorUltrason
AnalogSensorUltrason::AnalogSensorUltrason():Device(), prox(100){}

void AnalogSensorUltrason::run()
{
  while(1)
  {
    *ptrmem=prox;
    sleep(1);
  }
}

//classe DigitalActuatorValve
DigitalActuatorValve::DigitalActuatorValve(int t):Device(),refresh(t),flow(LOW){} 

void DigitalActuatorValve::run()
{
  while(1)
  {
    if(ptrmem!=NULL)
      flow=*ptrmem;
    if (flow)
      cout<<"(((Valve ouverte))"<<endl;
    else
      cout<<"(((Valve fermée)))"<<endl;    
    sleep(refresh);    
  }
}