
#ifndef _WII_MPLUS_H
#define _WII_MPLUS_H

#include "WProgram.h"
#include "Wire.h"

class WiiMPlus {

private:

  byte data[6]; //six data bytes

  int yaw0, pitch0, roll0; //calibration zeroes


public:

  int yaw, pitch, roll; //three axes
  
  int getYaw() {
      return yaw;
  }
  
  int getPitch() {
      return pitch;
  }
  
  int getRoll() {
      return roll;
  }

  void wmpOn(void);  
  void wmpSendZero(void);
  void calibrateZeroes(boolean);
  void receiveData(void);
  
};


#endif
