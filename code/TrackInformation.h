#ifndef CODE_TRACKINFORMATION_H_
#define CODE_TRACKINFORMATION_H_

#include "zf_common_headfile.h"

enum roadType
{
    BaseHandle = 0,     // 弯道和短直道
    BaseHandle_speedup, // 中直道和长直道
    BaseHandle_down,
    BaseHandle_wan,
    Stop,               // 停车
    RuHuan_Small,       // 入环
    HuanZhong_Small,    // 环中
    ChuHuan_Small,      // 出环
    RuHuan_Big,         // 入环
    HuanZhong_Big,      // 环中
    ChuHuan_Big,        // 出环
    BigRCurve,          // 小曲率弯
    Ramp,               // 坡道
        Ramp_fly,       // 下坡道
    ZebraCrossing,      // 斑马线

    RescueScene,        //救援区
        RescueHandle_L, // 救援区左转
        RescueHandle_R, // 救援区右转
    DangerScene,        // 危险区
    RacingScene,        // 追逐区
    SlowDown,
    SlowDown_big,
};


typedef struct
{
    uint16 LeftDaJiaoIN;
    uint16 RightDaJiaoIN;
    uint16 LeftDaJiaoOUT;
    uint16 RightDaJiaoOUT;
    float BeginDistance;
    float ForwardDistance;
    float ForwardDistance_Right;
    uint8  StopFinishflag;
    uint8 step;
    uint8 Direction;
    uint8 RescueHandle_LR_FLAG;
    uint16 StopFrame;
    uint16 S_sec;

    void(*Gddaojiao_judge)(void);
    void(*RescueReset)(void);
    void(*RescueInit)(void);

    enum RescueHandle_LR_Step
    {
        Nonee = 0,
        Forwardd,
        Stopp,
        Backk,
        Waitt,
    };
    enum Dir
    {
        middle = 0,
        left,
        right,
    };
}RescueHandle_LRmain;
extern RescueHandle_LRmain rescuehandle_lrmain;
void RescueInit(void);
void Gddaojiao_judge(void);
void RescueReset(void);


extern uint8 DataFromEdgeBoard;
extern int16 TrueDirError;
extern int16 LastTrueDirError;
extern int16 TrackState;
extern int16 LastTrackState;
extern int16 UpMissline;

extern uint8 ElemLists_Array[9];
extern uint8 ElemLists_Array_Index;
extern uint8 IFSend[3];

extern uint8 ZebraBeforeRunning;
extern uint8 ZebraBeforeStopping;
extern uint8 Zebra_Distance;

float SqrtByCarmack( float number );
void ZebraControl(void);
void ZebraCrossing_Process(void);

#endif
