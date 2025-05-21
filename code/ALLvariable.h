#ifndef CODE_ALLVARIABLE_H_
#define CODE_ALLVARIABLE_H_

#include "zf_common_headfile.h"


extern uint16 ServoMid;
extern uint16 ServoMax;
extern uint16 ServoMin;
extern uint8 Anchor_Now;
extern uint8 Anchor_Roundabout;
extern uint8 Anchor_Blocks;
extern uint8 Anchor_Bridge;


extern float SpeedOfBaseHandle;
extern float SpeedOfBaseHandle_speedup;
extern float SpeedOfBaseHandle_wan;
extern float SpeedOfLeaveCurve;
extern float Speed_min;
extern float SpeedOfHuanZhong_Small;
extern float SpeedOfChuHuan_Small;
extern float SpeedOfRuHuan_Small;
extern float SpeedOfHuanZhong_Big;
extern float SpeedOfChuHuan_Big;
extern float SpeedOfRuHuan_Big;
extern float SpeedOfBigRCurve;
extern float SpeedOfRamp;
extern float SpeedOfRamp_fly;
extern float SpeedOfZebra;
extern float SpeedOfRescueIN;
extern float SpeedOfRescueOFF;
extern float SpeedOfDanger;
extern float SpeedOfSlowDown_Big;
extern float SpeedOfSlowDown;
extern float _Speed;


extern float  SLp;
extern double flyFrame;
extern float  KLp;


extern float KpofRuHuan_Small;
extern float KdofRuHuan_Small;
extern float KpofHuanZhong_Small;
extern float KdofHuanZhong_Small;
extern float KpofChuHuan_Small;
extern float KdofChuHuan_Small;
extern float KpofRuHuan_Big;
extern float KdofRuHuan_Big;
extern float KpofHuanZhong_Big;
extern float KdofHuanZhong_Big;
extern float KpofChuHuan_Big;
extern float KdofChuHuan_Big;
extern float KpofBlocks;
extern float KdofBlocks;
extern float SteerKp;
extern float SteerKd;

extern float KpofStop;//斑马线停车Kp
extern float KiofStop;//斑马线停车Ki
extern float SpeedKpofBlocks;
extern float SpeedKiofBlocks;
extern float KiofCommon;
extern float KpofCommon;
extern float KdofCommon;

#endif
