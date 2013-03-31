
#include <Wire.h>
#include "WiiMPlus.h"

#include "WProgram.h"
void setup();
void loop();
WiiMPlus wmp;

long temps = 0;


//see http://wiibrew.org/wiki/Wiimote/Extension_Controllers#Wii_Motion_Plus
//for info on what each byte represents
void setup(){
  // alimentation du nunchuck (pas n\u00e9cessaire de comprendre)
  DDRC |= _BV(PC3) | _BV(PC2);  // pattes Analog 2 et 3 en sortie
  PORTC &=~ _BV(PC2);           // patte Analog 2 \u00e0 0v
  PORTC |=  _BV(PC3);           // patte Analog 3 \u00e0 5v
  delay(100);  // une petite attente
  Serial.begin(115200);
  //Serial.println("WM+ tester");
  Wire.begin();
  wmp.wmpOn(); //turn WM+ on
  wmp.calibrateZeroes(false); //calibrate zeroes
  delay(1000);
}



void loop(){
  wmp.receiveData(); //receive data and calculate yaw pitch and roll
  Serial.print(millis()-temps);
  Serial.print(" ");
  temps = millis();
  Serial.print(wmp.getYaw()); //for info on which axis is which
  Serial.print(" ");
  Serial.print(wmp.getPitch());
  Serial.print(" ");
  Serial.println(wmp.getRoll());
  delay(10);
}

int main(void)
{
	init();

	setup();
    
	for (;;)
		loop();
        
	return 0;
}

