
#include "WiiMPlus.h"


void WiiMPlus::wmpOn(){
  Wire.beginTransmission(0x53); //WM+ starts out deactivated at address 0x53
  Wire.send(0xfe); //send 0x04 to address 0xFE to activate WM+
  Wire.send(0x04);
  Wire.endTransmission(); //WM+ jumps to address 0x52 and is now active
}

void WiiMPlus::wmpSendZero(){
  Wire.beginTransmission(0x52); //now at address 0x52
  Wire.send(0x00); //send zero to signal we want info
  Wire.endTransmission();
}

void WiiMPlus::calibrateZeroes(boolean debug){
  float accY, accP, accR;
  
  for (int i=0;i<100;i++){
    wmpSendZero();
    Wire.requestFrom(0x52,6);
    for (int i=0;i<6;i++){
      data[i]=Wire.receive();
    }
    accY += ((data[3]>>2)<<8) + data[0]; //average 10 readings
    accP += ((data[4]>>2)<<8) + data[1];
    accR += ((data[5]>>2)<<8) + data[2];
  }
  
  yaw0 = accY / 100;
  pitch0 = accP / 100;
  roll0 = accR / 100;

  if (debug) 
  {
    Serial.print("Yaw0:");  
    Serial.print(yaw0);
    Serial.print(" Pitch0:");
    Serial.print(pitch0);
    Serial.print(" Roll0:");
    Serial.println(roll0);
  }
}


void WiiMPlus::receiveData(){
  wmpSendZero(); //send zero before each request (same as nunchuck)
  Wire.requestFrom(0x52,6); //request the six bytes from the WM+
  for (int i=0;i<6;i++){
    data[i]=Wire.receive();
  }
  yaw=((data[3]>>2)<<8)+data[0]-yaw0; 
  pitch=((data[4]>>2)<<8)+data[1]-pitch0; 
  roll=((data[5]>>2)<<8)+data[2]-roll0; 
}

