#include<code/ExternalDevice.h>

/***************************电池****************************/
float BatteryVoltage=0.0;

void Get_Battery()
{
    BatteryVoltage = adc_mean_filter_convert(ADC0_CH0_A0,5)*0.0129+1.0812;  //172 3.3    884 12.5
}
/***************************电池****************************/

/***************************舵机****************************/
float _ServoDutyfactor=0;

void Servo_Init()
{
    pwm_init(ServoMotorPWM,ServoMotorFREQ,ServoMid);
}

void Servo_Constrain(float* angle)
{
    if(*angle > ServoMax) *angle=ServoMax;
    if(*angle < ServoMin) *angle=ServoMin;
}

void Set_Angel(uint16 angle)
{
    pwm_set_duty(ServoMotorPWM,angle);
}
/***************************舵机****************************/

/***************************电机****************************/
uint8 MotorFlag=0;
int32 PWMMax=9900;
int32 PWMMin=-9900;
uint32 PWMOut= 0;
uint8 MotorPower = 1;

void Motor_Init()
{
    pwm_init( PWMAdvance , 17000, 0);
    pwm_init( PWMRetreat , 17000, 0);
}

void Set_MoterSpeed(int32 MotorSpeed)
{
    if(MotorSpeed > PWMMax) MotorSpeed = PWMMax;
    else if(MotorSpeed < PWMMin) MotorSpeed = PWMMin;

    if(MotorSpeed>=0)
    {
        pwm_set_duty(PWMAdvance,MotorSpeed);
        pwm_set_duty(PWMRetreat,0);
    }
    else
    {
        pwm_set_duty(PWMAdvance,0);
        pwm_set_duty(PWMRetreat,-MotorSpeed);
    }

}
/***************************电机****************************/

/***************************编码器****************************/
int16 EncoderValue=0;
float CarSpeed=0;
float RoadLength=0;

void Encoder_Init()
{
    encoder_dir_init(TIM6_ENCODER, TIM6_ENCODER_CH1_P20_3, TIM6_ENCODER_CH2_P20_0);
}

void EncoderValue_Get()
{
    EncoderValue = encoder_get_count(TIM6_ENCODER);
    if(IfOpenKalman == 1)
        CarSpeed = KalmanFilter(&CarSpeedAfterFilter,(float)EncoderValue) * EncoderValue_to_RealSpeed;
    else
        CarSpeed = EncoderValue * EncoderValue_to_RealSpeed;
    encoder_clear_count(TIM6_ENCODER);
}

void Cal_RoadLength(float T)
{
    RoadLength += CarSpeed * T / 30.0;
}
/***************************编码器****************************/

/***************************陀螺仪****************************/
const float RAD_TO_DEG = 57.29578;

float Gyro_X = 0;
float Gyro_Y = 0;
float Gyro_Z = 0;
float Acc_X = 0;
float Acc_Y = 0;
float Acc_Z = 0;

float alpha = 0.10; // 互补滤波系数，可根据需要调整
float compAngleY = 0.0; // 初始化互补滤波后的角度值
float AngleofRoundabout = 0;
float CourseAngle = 0;

void Get_GyroValue()
{
    imu963ra_get_gyro();//获取陀螺仪角速度数值
    imu963ra_get_acc();//获取加速度记数值
    Gyro_X = imu963ra_gyro_transition(imu963ra_gyro_x) - 0.1;
    //Gyro_Y = KalmanFilter(&GyroYAfterFilter, imu963ra_gyro_transition(imu963ra_gyro_y) + 0.2);
    Gyro_Y = KalmanFilter(&GyroYAfterFilter,imu963ra_gyro_transition(imu963ra_gyro_y) + 0.2);
    Gyro_Z = KalmanFilter(&GyroZAfterFilter,imu963ra_gyro_transition(imu963ra_gyro_z) + 0.0);
//    Acc_X = imu963ra_acc_transition(imu963ra_acc_x);
//    Acc_Y = imu963ra_acc_transition(imu963ra_acc_y);
//    Acc_Z = imu963ra_acc_transition(imu963ra_acc_z);
}

void Cal_Angel(float T)
{
    // 使用互补滤波器计算角度
    float gyroRateY = Gyro_Y;
//    float accAngleY = atan2(Acc_Y, SqrtByCarmack(Acc_X * Acc_X + Acc_Z * Acc_Z)) * RAD_TO_DEG;
//
//    // 互补滤波器公式
//    compAngleY = alpha * (compAngleY + gyroRateY * T) + (1.0 - alpha) * accAngleY;
//
//    AngleofRoundabout += compAngleY * T;
    AngleofRoundabout += gyroRateY * T;
}

int Ramp_Detection()
{
    if( Gyro_Z >= 40 ) return 1;
    else return 0;
}
/***************************陀螺仪****************************/

/***************************蜂鸣器****************************/
void Buzz_Init(void)
{
      gpio_init(BUZZ,GPO,1,GPO_PUSH_PULL);
}

void Buzz_Ctrl(BUZZn_e BUZZno, BUZZs_e sta)
{
    switch(BUZZno)
    {
        case BUZZ0:
          if(sta==BUZZ_ON)       gpio_set_level(BUZZ,0);
          else if(sta==BUZZ_OFF) gpio_set_level(BUZZ,1);
          else if(sta==BUZZ_RVS) gpio_toggle_level(BUZZ);
        break;

        default:
        break;
    }
}

void ElemBuzz()
{
    if(  TrackState != RuHuan_Small      &&
         TrackState != HuanZhong_Small   &&
         TrackState != ChuHuan_Small     &&
         TrackState != RuHuan_Big        &&
         TrackState != HuanZhong_Big     &&
         TrackState != ChuHuan_Big       &&
         TrackState != ZebraCrossing     &&
         TrackState != RescueScene       &&
         TrackState != DangerScene       &&
         TrackState != RescueHandle_R    &&
         TrackState != RescueHandle_L    &&
         TrackState != Ramp                 )
    {
        Buzz_Ctrl(BUZZ0, OFF);
    }

    if(  TrackState == RuHuan_Small      ||
         TrackState == HuanZhong_Small   ||
         TrackState == ChuHuan_Small     ||
         TrackState == RuHuan_Big        ||
         TrackState == HuanZhong_Big     ||
         TrackState == ChuHuan_Big       ||
         TrackState == ZebraCrossing     ||
         TrackState == RescueScene       ||
         TrackState == DangerScene       ||
         TrackState == RescueHandle_R    ||
         TrackState == RescueHandle_L    ||
         TrackState == Ramp                 )
    {
        Buzz_Ctrl(BUZZ0, ON);
    }
}
/***************************蜂鸣器****************************/

/***************************LED****************************/
void Led_Init(void)
{
    gpio_init(LED0p, GPO, 0, GPO_PUSH_PULL);
    gpio_init(LED1p, GPO, 0, GPO_PUSH_PULL);
    gpio_init(LED2p, GPO, 0, GPO_PUSH_PULL);
    gpio_init(LED3p, GPO, 0, GPO_PUSH_PULL);
}

void Led_Ctrl(LEDn_e LEDno, LEDs_e sta)
{
    switch(LEDno)
    {
       case LED0:
            if(sta==ON)       gpio_set_level(LED0p, 0);
            else if(sta==OFF) gpio_set_level(LED0p, 1);
            else if(sta==RVS) gpio_toggle_level(LED0p);
       break;
       case LED1:
            if(sta==ON)       gpio_set_level(LED1p,0);
            else if(sta==OFF) gpio_set_level(LED1p,1);
            else if(sta==RVS) gpio_toggle_level(LED1p);
       break;
       case LED2:
            if(sta==ON)       gpio_set_level(LED2p,0);
            else if(sta==OFF) gpio_set_level(LED2p,1);
            else if(sta==RVS) gpio_toggle_level(LED2p);
       break;
       case LED3:
            if(sta==ON)       gpio_set_level(LED3p,0);
            else if(sta==OFF) gpio_set_level(LED3p,1);
            else if(sta==RVS) gpio_toggle_level(LED3p);
       break;

       case LEDALL:
            if(sta==ON)
            {
                 gpio_set_level(LED0p,0);
                 gpio_set_level(LED1p,0);
                 gpio_set_level(LED2p,0);
                 gpio_set_level(LED3p,0);
            }
            else if(sta==OFF)
            {
                 gpio_set_level(LED0p,1);
                 gpio_set_level(LED1p,1);
                 gpio_set_level(LED2p,1);
                 gpio_set_level(LED3p,1);
            }
            else if(sta==RVS)
            {
                 gpio_toggle_level(LED0p);
                 gpio_toggle_level(LED1p);
                 gpio_toggle_level(LED2p);
                 gpio_toggle_level(LED3p);
            }
       break;
       default:
       break;
    }
}
/***************************LED****************************/
