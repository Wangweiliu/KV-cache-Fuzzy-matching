#include<code/Init.h>

void Init_Parameter()
{
    PWMOut = 0;
    PidParameter_Init_AfterRunning(&CurveDirectionPID);
    PidParameter_Init_AfterRunning(&SpeedPID);
    AngleofRoundabout=0;//环岛陀螺仪积分清零
    RoadLength=0;//编码器赛道长度积分清零

    TrackState = -1;//道路状况清零
    ZebraBeforeStopping = 0;//斑马线停车标志位清零

    SpeedPID.kp = KpofCommon;//速度环初始化
    SpeedPID.ki = KiofCommon;
    SpeedPID.kd = KdofCommon;
    flyFrame = 0;
    RescueReset();
}
