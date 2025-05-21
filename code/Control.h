#ifndef CODE_CONTROL_H_
#define CODE_CONTROL_H_

#include "zf_common_headfile.h"

#define OPEN 1
#define CLOSE 0

/**************************************传统PID控制*****************************************/
typedef struct
{
    float    kp;
    float    ki;
    float    kd;
    float    imax;

    float    out_p;
    float    out_i;
    float    out_d;
    float    out;

    float    integrator;
    float    last_error;
    float    last_derivative;
    unsigned long      last_t;
}PID_struct;

extern PID_struct StraightDirectionPID;
extern PID_struct CurveDirectionPID;
extern PID_struct SpeedPID;

void Pid_Init(PID_struct * pid);
void PidValue(void);
float ConstrainFloat(float amt, float low, float high);
float Pid_LocCtrl(PID_struct * pid, float error);
float Pid_IncCtrl(PID_struct * pid, float error);
void Pid_update(PID_struct* pid ,float error);
void PidParameter_Init_AfterRunning(PID_struct * pid);
/**************************************传统PID控制*****************************************/



/**************************************前馈控制*****************************************/
typedef struct
{
    float KP;
    float KD;
    float Filter_Coe;
    float In;
    float Last_In;
    float Filter_In;
    float Out;
} FF_struct;

extern uint8 IfOpenFF;
extern FF_struct SpeedFF;

void FF_Init(FF_struct *FF,float KP,float KD,float Filter_Coe);
void FF_Calculate(FF_struct *FF,float Input);
/**************************************前馈控制*****************************************/



/***************************方向环模糊控制*****************************/
extern uint8 IfOpenFuzzy;
extern float EFF[7];
extern float DFF[7];
extern float Kp_UFF[7];
extern float Kd_UFF[7];

float KP_Fuzzy(float E,float EC);
float Kd_Fuzzy(float EC);
/***************************方向环模糊控制*****************************/



/***************************ADRC控制*****************************/
extern float r,h;
extern float b,delta,belta01,belta02,belta03;
extern float alpha1,alpha2,belta1,belta2,belta3;
float ADRC(float v,float y);
/***************************ADRC控制*****************************/



/***************************开闭环切换*****************************/
extern uint8 OpenOrClosedFlag;
/***************************开闭环切换*****************************/
#endif
