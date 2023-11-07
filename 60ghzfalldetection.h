#ifndef _60GHZFALLDETECTION_H__
#define _60GHZFALLDETECTION_H__

#define MESSAGE_HEAD1 0x53      //Data frame header1
#define MESSAGE_HEAD2 0x59      //Data frame header2

#define MESSAGE_END1  0x54      //End1 of data frame
#define MESSAGE_END2  0x43      //End2 of data frame

#define HUMAN_PSE_RADAR 0x80    //Human presence data

#define PRESENCE_INF 0x01       //Presence Information
#define SOMEONE_HERE 0x01       //Someone here
#define NOONE_HERE 0x00         //Noone here

#define MOVE_INF 0x02           //Campaign Information
#define PSE_NONE 0x00           //None 
#define STATIONARY 0x01         //A person is stationary
#define MOVEMENT 0x02           //A person in motion

#define BODY_SIG 0x03           //Body movement information

#define MOVE_DIS 0x0E           //Determine the distance at which the movement occurs

#define FALL_DETECTION 0x83     //Fall data markers

#define FALL_STATE 0x01         //Fall status marker 
#define NO_FALL 0x00            //No falls detected 
#define FALL_DOWN 0x01          //Fall detected 

#define RESIDENT_STATE 0x05     //Resident status
#define NO_RESIDENT 0x00        //No stationary residency
#define RESIDENT 0x01           //There is a stationary residency

#define reset_frame_len 10      //Reset data frame length
//Reset data frame
const unsigned char fall_reset_frame[10] = {0x53, 0x59, 0x01, 0x02, 0x00, 0x01, 0x0F, 0xBF, 0x54, 0x43};

//Return status, Use in arduino
#define NOONE 0x01
#define SOMEONE 0x02
#define NONEPSE 0x03
#define STATION 0x04
#define MOVE 0x05
#define BODYVAL 0x06
#define MOVEDIS 0x07

#define NOFALL 0x08
#define FALL 0x09
#define NORESIDENT 0x10
#define RESIDENCY 0x11


class FallDetection_60GHz{
    private:
        Stream *stream;               // e.g. SoftwareSerial or Serial1
        boolean newData;
        byte dataLen;
        unsigned char Msg[20] = {0};  //Storage of the data frames returned by the radar up to a maximum length of 20.
        int count = 0;
        int checkdata_len = 2;        //Without cyclic sending, number of frames sent
        void reset_val();
        void data_printf(const unsigned char* buff, int len);
    public:
        unsigned int sensor_report = 0, bodysign_val = 0, total_detect = 0, height_0005 = 0, height_0510 = 0, height_1015 = 0, height_1520 = 0;
        FallDetection_60GHz(Stream *s);
        void recvRadarBytes();
        void showData();
        void HumanExis_Func();
        void Fall_Detection();
        void send_func(const unsigned char* buff, int len, bool cyclic = false);
        void reset_func();
};

#endif
