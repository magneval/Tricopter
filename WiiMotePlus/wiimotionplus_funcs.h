/*
 * Wiimotionplus functions  -- Talk to a Wii Wiimotionplus
 *
 * This library is from the Bionic Arduino course : 
 *                          http://todbot.com/blog/bionicarduino/
 *
 * 2007-11 Tod E. Kurt, http://todbot.com/blog/
 *
 * The Wii Wiimotionplus reading code originally from Windmeadow Labs
 *   http://www.windmeadow.com/node/42
 */

#if (ARDUINO >= 100)
#include <Arduino.h>
#else
#include <WProgram.h>
//#define Wire.write(x) Wire.send(x)
//#define Wire.read() Wire.receive()
#endif



byte wiimotionplus_buf[6];          //six data bytes

int yaw, pitch, roll;  //three axes
int yaw0, pitch0, roll0;  //calibration zeroes

// Uses port C (analog in) pins as power & ground for Wiimotionplus
static void i2c_setpowerpins()
{
#define pwrpin PORTC3
#define gndpin PORTC2
    DDRC |= _BV(pwrpin) | _BV(gndpin);
    PORTC &=~ _BV(gndpin);
    PORTC |=  _BV(pwrpin);
    delay(1000);  // wait for things to stabilize        
}

// initialize the I2C system, join the I2C bus,
// and tell the wiimotionplus we're talking to it
static void wiimotionplus_init()
{ 
  Wire.begin();                // join i2c bus as master
  /*
  Wire.send (0xF0);
  Wire.send (0x55);
if(Wire.endTransmission() == 0) {
  */
  Wire.beginTransmission(0x53);    //WM+ starts out deactivated at address 0x53
#if (ARDUINO >= 100)
    Wire.write((uint8_t)0xfe);// sends memory address
    //Wire.write((uint8_t)0x04);// sends sent a zero.
    Wire.write((uint8_t)0x05);// sends sent a zero.  
#else
    Wire.send((uint8_t)0xfe);// sends memory address                //send 0x04 to address 0xFE to activate WM+
    //Wire.send((uint8_t)0x04);// sends sent a zero.
    Wire.send((uint8_t)0x05);// sends sent a zero.
#endif
  Wire.endTransmission();          //WM+ jumps to address 0x52 and is now active
  //}
}

// Send a request for data to the wiimotionplus
// was "send_zero()"
static void wiimotionplus_send_request()
{
    Wire.beginTransmission(0x52);// transmit to device 0x52
#if (ARDUINO >= 100)
    Wire.write((uint8_t)0x00);// sends one byte
#else
    Wire.send((uint8_t)0x00);// sends one byte
#endif
   if(Wire.endTransmission() == 0) {
//  Serial.println("send request ok ");    
  // Wire.endTransmission();// stop transmitting
    return;
  }
  Serial.println("send request failure ");    
}

static int calibrateZeroes(){
  //wiimotionplus_send_request();
  for (int i=0;i<10;i++){
    wiimotionplus_send_request();  // send request for next data payload
    int cnt=0;
    Wire.requestFrom (0x52, 6);// request data from wiimotionplus
    while (Wire.available ()) {
        // receive byte as an integer
#if (ARDUINO >= 100)
    wiimotionplus_buf[cnt]=Wire.read();
#else
    wiimotionplus_buf[cnt]=Wire.receive();
#endif
        cnt++;
    }
    if (cnt <6) {
        Serial.println("failure ");
        Serial.print("boucle ");
        Serial.println(i, DEC); 
        Serial.print("cnt ");
        Serial.println(cnt, DEC); 
        return 0;
    }

    yaw0+=(((wiimotionplus_buf[3]>>2)<<8)+wiimotionplus_buf[0])/10;        //average 10 readings for each zero
    pitch0+=(((wiimotionplus_buf[4]>>2)<<8)+wiimotionplus_buf[1])/10;
    roll0+=(((wiimotionplus_buf[5]>>2)<<8)+wiimotionplus_buf[2])/10;
      delay(100);
    //wiimotionplus_send_request();  // send request for next data payload
  }
  wiimotionplus_send_request();
  Serial.print("Yaw0:");
  Serial.print(yaw0);
  Serial.print("  Pitch0:");
  Serial.print(pitch0);
  Serial.print("  Roll0:");
  Serial.println(roll0);
  return 1;
}

// Receive data back from the wiimotionplus, 
// returns 1 on successful read. returns 0 on failure
static int wiimotionplus_get_data()
{
    wiimotionplus_send_request();  // send request for next data payload 
    int cnt=0;
    Wire.requestFrom (0x52, 6);// request data from wiimotionplus
    while (Wire.available ()) {
        // receive byte as an integer
#if (ARDUINO >= 100)
    wiimotionplus_buf[cnt]=Wire.read();
#else
    wiimotionplus_buf[cnt]=Wire.receive();
#endif
        cnt++;
    }
      wiimotionplus_send_request();  // send request for next data payload
  yaw=((wiimotionplus_buf[3]>>2)<<8)+wiimotionplus_buf[0]-yaw0;        //see http://wiibrew.org/wiki/Wiimote/Extension_Controllers#Wii_Motion_Plus
  pitch=((wiimotionplus_buf[4]>>2)<<8)+wiimotionplus_buf[1]-pitch0;    //for info on what each byte represents
  roll=((wiimotionplus_buf[5]>>2)<<8)+wiimotionplus_buf[2]-roll0;      
   // If we recieved the 6 bytes, then go print them
    if (cnt <6) {
        return 0;   // failure
    }
    /*
    cnt=0;
    Wire.requestFrom (0x52, 6);// request data from wiimotionplus
    while (Wire.available ()) {
        // receive byte as an integer
#if (ARDUINO >= 100)
    wiimotionplus_buf[cnt]=Wire.read();
#else
    wiimotionplus_buf[cnt]=Wire.receive();
#endif
        cnt++;
    }
      wiimotionplus_send_request();  // send request for next data payload
    if (cnt <6) {
        return 0;   // failure
    }
    */
        return 1;   // success
}

// Print the input data we have recieved
// accel data is 10 bits long
// so we read 8 bits, then we have to add
// on the last 2 bits.  That is why I
// multiply them by 2 * 2
static void wiimotionplus_print_data()
{ 
    static int i=0;


    Serial.print(i,DEC);
    Serial.print("\t");

    Serial.print("yaw:");
    Serial.print(yaw, DEC);
    Serial.print("\t");
    Serial.print("pitch");
    Serial.print(pitch, DEC);
    Serial.print("\t");
    Serial.print("roll");
    Serial.print(roll, DEC);

    Serial.print("\r\n");  // newline
    i++;
}

static void wiimotionplus_print_csvData()
{ 
    static int i=0;


    Serial.print(i,DEC);
    Serial.print(";");
    Serial.print(yaw, DEC);
    Serial.print(";");
    Serial.print(pitch, DEC);
    Serial.print(";");
    Serial.print(roll, DEC);
    Serial.println("");  // newline
    i++;
}


// returns value of yaw gyroscope
static int wiimotionplus_yaw()
{
    return yaw;  
}

// returns value of pitch gyroscope
static int wiimotionplus_pitch()
{
    return pitch;   
}

// returns value of roll gyroscope
static int wiimotionplus_roll()
{
    return roll;   
}

