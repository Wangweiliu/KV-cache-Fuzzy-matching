#ifndef FILTER_H_
#define FILTER_H_

#include "zf_common_headfile.h"

typedef struct
{
    float LastP;//上次估算协方差 初始化值为0.02
    float Now_P;//当前估算协方差 初始化值为0
    float out;//卡尔曼滤波器输出 初始化值为0
    float Kg;//卡尔曼增益 初始化值为0
    float Q;//过程噪声协方差 初始化值为0.001
    float R;//观测噪声协方差 初始化值为0.543
}KFP_Typedef;//Kalman Filter parameter

extern uint8 IfOpenKalman;
extern KFP_Typedef CarSpeedAfterFilter;
extern KFP_Typedef GyroZAfterFilter;
extern KFP_Typedef GyroYAfterFilter;
void Kalman_Init(KFP_Typedef *kfp);
float KalmanFilter(KFP_Typedef *kfp,float input);

#endif


