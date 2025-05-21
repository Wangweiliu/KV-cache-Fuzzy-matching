#include<code/TrackInformation.h>

RescueHandle_LRmain rescuehandle_lrmain;

uint8 DataFromEdgeBoard=0;
int16 TrueDirError=0;
int16 LastTrueDirError=0;
int16 TrackState=-1;
int16 LastTrackState=0;
int16 UpMissline=0;

uint8 ZebraBeforeRunning = 0;
uint8 ZebraBeforeStopping = 0;

uint8 Zebra_Distance = 20;

uint8 ElemLists_Array[9] = {0};
uint8 ElemLists_Array_Index = 0;
uint8 IFSend[3] = {0,0,0};

//快速浮点数平方根
float SqrtByCarmack( float number )
{
    int i;
    float x2, y;
    const float threehalfs = 1.5F;

    x2 = number * 0.5F;
    y  = number;
    i  = * ( int * ) &y;
    i  = 0x5f375a86 - ( i >> 1 );
    y  = * ( float * ) &i;
    y  = y * ( threehalfs - ( x2 * y * y ) );
    y  = y * ( threehalfs - ( x2 * y * y ) );
    y  = y * ( threehalfs - ( x2 * y * y ) );
    return number*y;
}

void ZebraCrossing_Process()
{
    _Speed = SpeedOfZebra;
    if(TrackState == ZebraCrossing && RoadLength >= Zebra_Distance)//可在距离斑马线10m内发车，此时正常驶过斑马线。当行驶距离超过10m，遇到斑马线停车
    {
        ZebraBeforeStopping = 1;
    }
}

void ZebraControl()
{
    if(ZebraBeforeStopping == 1 && TrackState != ZebraCrossing)
    {
        SpeedPID.kp = KpofStop;
        SpeedPID.ki = KiofStop;
        _Speed = 0;
        if(CarSpeed >= -2 && CarSpeed <= 2) {MotorPower = 0;ZebraBeforeStopping = 0;Init_Parameter();}
    }
}


void RescueInit()
{
    rescuehandle_lrmain.BeginDistance = 0;
    rescuehandle_lrmain.Direction = middle;
    rescuehandle_lrmain.step = Nonee;
    rescuehandle_lrmain.LeftDaJiaoIN = 1700;
    rescuehandle_lrmain.RightDaJiaoIN = 1225;
    rescuehandle_lrmain.ForwardDistance = 0.60;//磨砂蓝布0.70;
    rescuehandle_lrmain.ForwardDistance_Right = 0.56; //磨砂蓝布0.67;
    rescuehandle_lrmain.RescueHandle_LR_FLAG = 0;
    rescuehandle_lrmain.S_sec = 0;
    rescuehandle_lrmain.StopFrame = 50;//5ms/1,200=1000ms=1s
}

void Gddaojiao_judge()
{
    if(rescuehandle_lrmain.step == Nonee)
    {
        _Speed = SpeedOfRescueIN;
        rescuehandle_lrmain.S_sec = 0;
        rescuehandle_lrmain.StopFinishflag ++;
        if(TrackState == RescueHandle_L)
            rescuehandle_lrmain.Direction = left;
        else if(TrackState == RescueHandle_R)
            rescuehandle_lrmain.Direction = right;
        rescuehandle_lrmain.BeginDistance = RoadLength;
        rescuehandle_lrmain.step = Forwardd;
    }
    if(rescuehandle_lrmain.step == Forwardd)
    {
        _Speed = SpeedOfRescueIN;
        if(rescuehandle_lrmain.Direction == left)
            _ServoDutyfactor = rescuehandle_lrmain.LeftDaJiaoIN;
        else if(rescuehandle_lrmain.Direction == right)
            _ServoDutyfactor = rescuehandle_lrmain.RightDaJiaoIN;

        if(RoadLength - rescuehandle_lrmain.BeginDistance > rescuehandle_lrmain.ForwardDistance && rescuehandle_lrmain.Direction == left)
        {
            rescuehandle_lrmain.step = Stopp;
            _Speed = 0;
        }
        else if(RoadLength - rescuehandle_lrmain.BeginDistance > rescuehandle_lrmain.ForwardDistance_Right && rescuehandle_lrmain.Direction == right)
        {
            rescuehandle_lrmain.step = Stopp;
            _Speed = 0;
        }
    }
    if(rescuehandle_lrmain.step == Stopp)
    {
        _Speed = 0;
        rescuehandle_lrmain.RescueHandle_LR_FLAG = 1;
        if(rescuehandle_lrmain.Direction == left)
            _ServoDutyfactor = rescuehandle_lrmain.LeftDaJiaoIN;
        else if(rescuehandle_lrmain.Direction == right)
            _ServoDutyfactor = rescuehandle_lrmain.RightDaJiaoIN;
        if(CarSpeed > -1 && CarSpeed < 1 && (rescuehandle_lrmain.S_sec >= rescuehandle_lrmain.StopFrame))
        {
            rescuehandle_lrmain.step = Backk;
            rescuehandle_lrmain.BeginDistance = RoadLength;
        }
    }
    if(rescuehandle_lrmain.step == Backk)
    {
        _Speed = -SpeedOfRescueOFF;
        if(rescuehandle_lrmain.Direction == left)
            _ServoDutyfactor = rescuehandle_lrmain.LeftDaJiaoIN;
        else if(rescuehandle_lrmain.Direction == right)
            _ServoDutyfactor = rescuehandle_lrmain.RightDaJiaoIN;
        rescuehandle_lrmain.step = Waitt;
    }
    if(rescuehandle_lrmain.step == Waitt)
    {
        _Speed = -SpeedOfRescueOFF;
        if(rescuehandle_lrmain.Direction == left)
            _ServoDutyfactor = rescuehandle_lrmain.LeftDaJiaoIN;
        else if(rescuehandle_lrmain.Direction == right)
            _ServoDutyfactor = rescuehandle_lrmain.RightDaJiaoIN;
        USB_Edgeboard_Rescue();
        Led_Ctrl(LEDALL,RVS);
    }
}

void RescueReset()
{
    rescuehandle_lrmain.Direction=middle;
    rescuehandle_lrmain.BeginDistance = 0;
    rescuehandle_lrmain.step = Nonee;
    rescuehandle_lrmain.RescueHandle_LR_FLAG = 0;
    rescuehandle_lrmain.S_sec = 0;
}
