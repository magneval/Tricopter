/*
 * WiiChuckDemo -- 
 *
 * 2008 Tod E. Kurt, http://thingm.com/
 *
 */

#include <Wire.h>
#include "nunchuck_funcs.h"

int loop_cnt=0;

//byte accx,accy,zbut,cbut;
//int ledPin = 13;
int flag=0;

void setup()
{
    Serial.begin(115200);
    nunchuck_setpowerpins();
    nunchuck_init(); // send the initilization handshake
    
    Serial.println("n\tjoy_x\tjoy_y\taccel_x\taccel_y\taccel_z\tb1\tb2");
}

void loop()
{
    if( loop_cnt > 100 ) { // every 100 msecs get new data
        loop_cnt = 0;



        flag=nunchuck_get_data();
    if (flag == 1) {
    //    Serial.print("success ");
    } else {
        Serial.print("failure "); 

    }

          /*  
        accx  = nunchuck_accelx(); // ranges from approx 70 - 182
        accy  = nunchuck_accely(); // ranges from approx 65 - 173
        zbut = nunchuck_zbutton();
        cbut = nunchuck_cbutton(); 
        Serial.print("accx: "); Serial.print((byte)accx,DEC);
        Serial.print("\taccy: "); Serial.print((byte)accy,DEC);
        Serial.print("\tzbut: "); Serial.print((byte)zbut,DEC);
        Serial.print("\tcbut: "); Serial.println((byte)cbut,DEC);
*/
      nunchuck_print_csvData();
    }
    loop_cnt++;
    delay(1);
}

