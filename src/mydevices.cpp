
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


//classe AnalogSensorUltrason
AnalogSensorUltrason::AnalogSensorUltrason():Device(), prox(100){}

//classe DigitalActuatorValve
DigitalActuatorValve::DigitalActuatorValve(int t):Device(),refresh(t),flow(0){} 