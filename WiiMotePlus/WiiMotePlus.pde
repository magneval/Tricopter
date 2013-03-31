/*
 * WiiChuckDemo -- 
 *
 * 2008 Tod E. Kurt, http://thingm.com/
 *
 */

#include <Wire.h>
#include "wiimotionplus_funcs.h"

int loop_cnt=0;

//byte accx,accy,zbut,cbut;
//int ledPin = 13;
int flag=0;

void setup(){
  Serial.begin(115200);
  i2c_setpowerpins();
  delay(1000);
  wiimotionplus_init(); // send the initilization handshake
  
  calibrateZeroes();              //calibrate zeroes
  delay(1000);

    Serial.print("n;");

    Serial.print("yaw;");
    Serial.print("pitch;");
    Serial.print("roll");
    Serial.println("");  // newline

}

void loop()
{
    if( loop_cnt > 100 ) { // every 100 msecs get new data
        loop_cnt = 0;



        flag=wiimotionplus_get_data();
    if (flag == 1) {
        //Serial.print("success ");
    } else {
        //Serial.print("failure "); 

    }
      wiimotionplus_print_csvData();
    }
    loop_cnt++;
    delay(1);
}

