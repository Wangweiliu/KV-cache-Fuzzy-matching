#include <code/Filter.h>

uint8 IfOpenKalman = 1;
KFP_Typedef CarSpeedAfterFilter;
KFP_Typedef GyroZAfterFilter;
KFP_Typedef GyroYAfterFilter;

void Kalman_Init(KFP_Typedef *kfp)
{
    kfp->Kg=0;//卡尔曼增益
    kfp->LastP=0.02;
    kfp->Now_P=0;
    kfp->Q=0.05;//过程噪声
    kfp->R=1.0;//观测噪声
    kfp->out=0;
}

float KalmanFilter(KFP_Typedef *kfp,float input)
{
    //预测协方差方程：k时刻系统估算协方差 = k-1时刻的系统协方差 + 过程噪声协方差
    kfp->Now_P = kfp->LastP + kfp->Q;

    //卡尔曼增益方程：卡尔曼增益 = k时刻系统估算协方差 / （k时刻系统估算协方差 + 观测噪声协方差）
    kfp->Kg = kfp->Now_P / (kfp->Now_P + kfp->R);

    //更新最优值方程：k时刻状态变量的最优值 = 状态变量的预测值 + 卡尔曼增益 * （测量值 - 状态变量的预测值）
    kfp->out = kfp->out + kfp->Kg * (input -kfp->out);//因为这一次的预测值就是上一次的输出值

    //更新协方差方程: 本次的系统协方差付给 kfp->LastP 为下一次运算准备。
    kfp->LastP = (1-kfp->Kg) * kfp->Now_P;

    return kfp->out;
}
