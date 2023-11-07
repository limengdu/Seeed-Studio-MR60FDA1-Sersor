#include "Arduino.h"
#include "60ghzfalldetection.h"

FallDetection_60GHz::FallDetection_60GHz(Stream *s)
    : stream(s){
  this->newData = false;
}

// Receive data and process
void FallDetection_60GHz::recvRadarBytes(){
  while (stream->available()) {
    if(stream->read() == MESSAGE_HEAD1){           //Receive header frame 1
      if(stream->read() == MESSAGE_HEAD2){         //Receive header frame 2
        dataLen = stream->readBytesUntil(MESSAGE_END2, Msg, 20);
        if (dataLen > 0 && dataLen < 20){
          Msg[dataLen] = MESSAGE_END2;
          this->newData = true;
        }
      }
    }
  }
}

//Radar transmits data frames for display via serial port
void FallDetection_60GHz::showData(){
  if(this->newData){
    Serial.print(MESSAGE_HEAD1, HEX);
    Serial.print(' ');
    Serial.print(MESSAGE_HEAD2, HEX);
    Serial.print(' ');
    char charVal[4];
    for (byte n = 0; n < dataLen+1; n++) {
      sprintf(charVal, "%02X", Msg[n]);
      Serial.print(charVal);
      Serial.print(' ');
    }
    Serial.println();
    this->newData = false;
    Msg[dataLen] = {0};
  }
}

// Judgment of occupied and unoccupied, approach and distance
void FallDetection_60GHz::HumanExis_Func(){
  recvRadarBytes();
  reset_val();
  if(this->newData){
    switch(Msg[0]){
      case HUMAN_PSE_RADAR:
        switch(Msg[1]){
          case PRESENCE_INF:
            switch(Msg[4]){
              case NOONE_HERE:
                showData();
                sensor_report = NOONE;
                break;
              case SOMEONE_HERE:
                showData();
                sensor_report = SOMEONE;
                break;
            }
            break;
          case MOVE_INF:
            switch(Msg[4]){
              case PSE_NONE:
                showData();
                sensor_report = NONEPSE;
                break;
              case STATIONARY:
                showData();
                sensor_report = STATION;
                break;
              case MOVEMENT:
                showData();
                sensor_report = MOVE;
                break;
            }
            break;
          case BODY_SIG:
            showData();
            sensor_report = BODYVAL;
            bodysign_val = Msg[4];
            break;
          // case MOVE_DIS:  //This data is only used as an interface and is not normally required by users
          //   showData();
          //   sensor_report = MOVEDIS;
          //   total_detect = (Msg[4] << 8 | Msg[5]);
          //   height_0005 = Msg[6];
          //   height_0510 = Msg[7];
          //   height_1015 = Msg[8];
          //   height_1520 = Msg[9];
          //   break;
        }
        break;
    }
  }
}

//Respiratory sleep data frame decoding
void FallDetection_60GHz::Fall_Detection(){
  recvRadarBytes();
  reset_val();
  if(this->newData){
    switch(Msg[0]){
      case FALL_DETECTION:
        switch(Msg[1]){
          case FALL_STATE:
            switch(Msg[4]){
              case NO_FALL:
                showData();
                sensor_report = NOFALL;
                break;
              case FALL_DOWN:
                showData();
                sensor_report = FALL;
                break;
            }
            break;
          case RESIDENT_STATE:
            switch(Msg[4]){
              case NO_RESIDENT:
                showData();
                sensor_report = NORESIDENT;
                break;
              case RESIDENT:
                showData();
                sensor_report = RESIDENCY;
                break;
            }
            break;
        }
        break;
    }
  }
}

//Send data frame
void FallDetection_60GHz::send_func(const unsigned char* buff, int len, bool cyclic /*=false*/){
  if(cyclic || count < checkdata_len){
    if(cyclic || count < 1){
      stream->write(buff, len);
      stream->flush();
    }
    do{
      recvRadarBytes();
      delay(20);
    }while(!(this->newData));
    if(cyclic || count < 1){
      Serial.print("  Sent  ---> ");
      data_printf(buff, len);
    }
    if(count%2 == 1){
      Serial.print("Receive <--- ");
      showData();
    }
    this->newData = false;
  }
  count++;
}

//Reset radar
void FallDetection_60GHz::reset_func(){
  stream->write(fall_reset_frame, reset_frame_len);
  stream->flush();
  Serial.println("Radar reset!");
}

//print redirect
void FallDetection_60GHz::data_printf(const unsigned char* buff, int len){
  char charVal[4];
  for(int i=0; i<len; i++){
    sprintf(charVal, "%02X", buff[i]);
    Serial.print(charVal);
    Serial.print(' ');
  }
  Serial.println();
}

//reset the radar values
void FallDetection_60GHz::reset_val(){
  sensor_report = 0x00;

  bodysign_val = 0x00;
  total_detect = 0x00;
  height_0005 = 0x00;
  height_0510 = 0x00;
  height_1015 = 0x00;
  height_1520 = 0x00;
}


