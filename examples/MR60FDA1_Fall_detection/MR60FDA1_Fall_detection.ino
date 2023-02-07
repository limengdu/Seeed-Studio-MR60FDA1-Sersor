#include "Arduino.h"
#include <60ghzfalldetection.h>

//#include <SoftwareSerial.h>
// Choose any two pins that can be used with SoftwareSerial to RX & TX
//#define RX_Pin A2
//#define TX_Pin A3

//SoftwareSerial mySerial = SoftwareSerial(RX_Pin, TX_Pin);

// we'll be using software serial
//FallDetection_60GHz radar = FallDetection_60GHz(&mySerial);

// can also try hardware serial with
FallDetection_60GHz radar = FallDetection_60GHz(&Serial1);

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  Serial1.begin(115200);

  //  mySerial.begin(115200);

  while(!Serial);   //When the serial port is opened, the program starts to execute.

  Serial.println("Readly");
}

void loop()
{
  // put your main code here, to run repeatedly:
  radar.Fall_Detection();           //Receive radar data and start processing
  if(radar.sensor_report != 0x00){
    switch(radar.sensor_report){
        case NOFALL:
            Serial.println("The sensor detects this movement is not a fall.");
            Serial.println("----------------------------");
            break;
        case FALL:
            Serial.println("The sensor detects a fall.");
            Serial.println("----------------------------");
            break;
        case NORESIDENT:
            Serial.println("The sensors did not detect anyone staying in place.");
            Serial.println("----------------------------");
            break;
        case RESIDENCY:
            Serial.println("The sensor detects someone staying in place.");
            Serial.println("----------------------------");
            break;
    }
  }
  delay(200); //Add time delay to avoid program jam
}