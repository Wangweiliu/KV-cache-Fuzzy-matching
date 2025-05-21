#include<code/ALLvariable.h>

/**************************舵机**************************/
uint16 ServoMid = 1468;
uint16 ServoMax = 1796;
uint16 ServoMin = 1096;
uint8 Anchor_Now = 38;
uint8 Anchor_Roundabout = 36;
uint8 Anchor_Blocks = 9;
uint8 Anchor_Bridge = 3;
/**************************舵机**************************/



/**************************小车各个阶段速度**************************/
float SpeedOfBaseHandle = 80;
float SpeedOfBaseHandle_speedup = 105;
float SpeedOfBaseHandle_wan = 72;
float Speed_min = 70;

float SpeedOfBigRCurve = 100;
float SpeedOfRamp = 60;
float SpeedOfRamp_fly = 70;
float SpeedOfZebra = 100;

float SpeedOfHuanZhong_Small = 80;
float SpeedOfChuHuan_Small = 80;
float SpeedOfRuHuan_Small = 80;

float SpeedOfHuanZhong_Big = 90;
float SpeedOfChuHuan_Big = 100;
float SpeedOfRuHuan_Big = 90;

float SpeedOfRescueIN= 60;
float SpeedOfRescueOFF= 40;
float SpeedOfDanger = 50;
float SpeedOfSlowDown_Big = 15;
float SpeedOfSlowDown = 50;

float _Speed=0;
/**************************小车各个阶段速度**************************/


/**************************速度策略参数**************************/
double flyFrame = 0;   //记录加速帧数
float  SpeedAdd = 20;
float  SLp = 0.031;      //减速系数
float  KLp = 0.07;
/**************************速度策略参数**************************/


/*****************************PID*****************************/
float KpofRuHuan_Small = 2.81;
float KdofRuHuan_Small = 10.41;//10.41;
float KpofHuanZhong_Small = 2.31;
float KdofHuanZhong_Small = 11.51;
float KpofChuHuan_Small = 1.81;
float KdofChuHuan_Small = 9.01;

float KpofRuHuan_Big = 3.01;
float KdofRuHuan_Big = 10.41;//10.41;
float KpofHuanZhong_Big = 2.2;
float KdofHuanZhong_Big = 8.8;
float KpofChuHuan_Big = 2.0;
float KdofChuHuan_Big = 8.1;

float KpofBlocks = 3.3;
float KdofBlocks = 15.2;
float SteerKp = 2.4;
float SteerKd = 9.6;


float KpofStop = 60;//斑马线停车Kp
float KiofStop = 1.0;//斑马线停车Ki
float SpeedKpofBlocks = 200;
float SpeedKiofBlocks = 6;
float KiofCommon = 5.8;
float KpofCommon = 200;//115;
float KdofCommon = 0.0;
/*****************************PID*****************************/
