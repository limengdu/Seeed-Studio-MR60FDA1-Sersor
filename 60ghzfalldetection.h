#ifndef _RADAR_H__
#define _RADAR_H__

#define MESSAGE_HEAD 0x53       //Data frame header
#define MESSAGE_TAIL 0x54       //Data frame tail

#define HUMAN_PSE_RADAR 0x80    //Human presence data

#define PRESENCE_INF 0x01       //Presence Information
#define SOMEONE_HERE 0x01       //Someone here
#define NOONE_HERE 0x00         //Noone here

#define MOVE_INF 0x02           //Campaign Information
#define NONE 0x00               //None
#define STATIONARY 0x01         //A person is stationary
#define MOVEMENT 0x02           //A person in motion

#define BODY_SIG 0x03           //Body movement information

#define DISTANCE 0x04           //Distance from the person being detected

#define MOVESPEED 0x06          //Speed of character movement

#define FALL_DETECTION 0x83     //Fall data markers

#define FALL_STATE 0x01         //Fall status marker
#define NO_FALL 0x00            //No falls detected
#define FALLING 0x01            //Fall detected

#define FALL_POTENTIAL 0x02     //Confidence level for falls

#define FALL_LOCATION 0x03      //Location of the fall

#define POINTCLOUD_DATA 0x04    //Point cloud data


class FallDetection_60GHz{
    private:
        
    public:
        const byte MsgLen = 12;
        byte dataLen = 12;
        byte Msg[12];
        boolean newData = false;
        void SerialInit();
        void recvRadarBytes();
        void Fall_Detection(byte inf[]);
        void Situation_judgment(byte inf[]);
        void ShowData(byte inf[]);
};

#endif
