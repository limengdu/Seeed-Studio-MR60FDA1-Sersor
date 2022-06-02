#include "Arduino.h"
#include "60ghzfalldetection.h"

#ifdef __AVR__
    #include <SoftwareSerial.h>
    SoftwareSerial SSerial(2, 3); // RX, TX
    #define Serial1 SSerial
#endif

void FallDetection_60GHz::SerialInit(){
  Serial1.begin(115200);
}

// Receive data and process
void FallDetection_60GHz::recvRadarBytes(){
  static boolean recvInProgress = false;
  static byte ndx = 0;
  byte startMarker = MESSAGE_HEAD;            //Header frame
  byte endMarker = MESSAGE_TAIL;
  byte rb; // Each frame received
  while (Serial1.available() > 0 && newData == false)
  {
    rb = Serial1.read();
    if (recvInProgress == true)
    {                     // Received header frame
      if (rb != endMarker){ // Length in range
        Msg[ndx] = rb;
        ndx++;
      }
      else{
        recvInProgress = false;
        dataLen = ndx;
        ndx = 0;
        newData = true;
      }
    }
    else if (rb == startMarker){ // Waiting for the first frame to arrive
        recvInProgress = true;
    }
  }
}

//Radar transmits data frames for display via serial port
void FallDetection_60GHz::ShowData(byte inf[]){
  for (byte n = 0; n < dataLen+3; n++) {
    Serial.print(inf[n], HEX);
    Serial.print(' ');
  }
    Serial.println();
}


// Judgment of occupied and unoccupied, approach and distance
void FallDetection_60GHz::Situation_judgment(byte inf[]){
  switch(inf[2]){
    case HUMAN_PSE_RADAR:
      switch(inf[3]){
        case PRESENCE_INF:
          switch(inf[6]){
            case NOONE_HERE:
              ShowData(inf);
              Serial.println("Radar detects no one.");
              Serial.println("----------------------------");
              break;
            case SOMEONE_HERE:
              ShowData(inf);
              Serial.println("Radar detects somebody.");
              Serial.println("----------------------------");
              break;
          }
          break;
        case MOVE_INF:
          switch(inf[6]){
            case NONE:
              ShowData(inf);
              Serial.println("Radar detects None.");
              Serial.println("----------------------------");
              break;
            case STATIONARY:
              ShowData(inf);
              Serial.println("Radar detects somebody stationary.");
              Serial.println("----------------------------");
              break;
            case MOVEMENT:
              ShowData(inf);
              Serial.println("Radar detects somebody in motion.");
              Serial.println("----------------------------");
              break;
          }
          break;
        case BODY_SIG:
          ShowData(inf);
          Serial.print("The radar identifies the current motion feature value is: ");
          Serial.println(inf[6]);
          Serial.println("----------------------------");
          break;
        case DISTANCE:
          ShowData(inf);
          Serial.print("The distance of the radar from the monitored person is: ");
          Serial.print(inf[6]);
          Serial.print(" ");
          Serial.print(inf[7]);
          Serial.println(" cm");
          Serial.println("----------------------------");
          break;
        case MOVESPEED:
          ShowData(inf);
          Serial.print("The speed of movement of the monitored person is: ");
          Serial.print(inf[6]);
          Serial.print(" ");
          Serial.print(inf[7]);
          Serial.println(" cm/s");
          Serial.println("----------------------------");
          break;
      }
      break;
  }
}

//Respiratory sleep data frame decoding
void FallDetection_60GHz::Fall_Detection(byte inf[]){
  switch(inf[2]){
    case FALL_DETECTION:
      switch(inf[3]){
        case FALL_STATE:
          switch(inf[6]){
            case NO_FALL:
              ShowData(inf);
              Serial.println("Radar detects that the current no fall.");
              Serial.println("----------------------------");
              break;
            case FALLING:
              ShowData(inf);
              Serial.println("Radar detects current someone falling.");
              Serial.println("----------------------------");
              break;
          }
          break;
        case FALL_POTENTIAL:
          ShowData(inf);
          Serial.print("The confidence level for the current radar detection of a fall is: ");
          Serial.println(inf[6]);
          Serial.println("----------------------------");
          break;
        case FALL_LOCATION:
          ShowData(inf);
          Serial.print("The fall position is: ");
          Serial.print("x: ");
          Serial.print(inf[6]);
          Serial.print(" ");
          Serial.print(inf[7]);
          Serial.print(" ");
          Serial.print("y: ");
          Serial.print(inf[8]);
          Serial.print(" ");
          Serial.println(inf[9]);
          Serial.println("----------------------------");
          break;
        case POINTCLOUD_DATA:
          ShowData(inf);
          Serial.print("The point cloud data are: ");
          Serial.print("x: ");
          Serial.print(inf[6]);
          Serial.print(" ");
          Serial.print(inf[7]);
          Serial.print(" ");
          Serial.print("y: ");
          Serial.print(inf[8]);
          Serial.print(" ");
          Serial.print(inf[9]);
          Serial.print(" ");
          Serial.print("z: ");
          Serial.print(inf[10]);
          Serial.print(" ");
          Serial.println(inf[11]);
          Serial.println("----------------------------");
          break;
      }
      break;
  }
}




