#include "zf_common_headfile.h"
#pragma section all "cpu0_dsram"
// 将本语句与#pragma section all restore语句之间的全局变量都放在CPU0的RAM中


// **************************** 代码区域 ****************************
int core0_main(void)
{
    clock_init();                   // 获取时钟频率<务必保留>
    debug_init();                   // 初始化默认调试串口
    interrupt_global_disable();
    system_delay_init();

    /*************硬件始化**************/
    Motor_Init();
    Servo_Init();
    Encoder_Init();
    key_init(10);
    Led_Init();
    Buzz_Init();
    oled_init();
    wireless_uart_init();
    disable_Watchdog();
    /*************硬件初始化**************/

    /*************参数初始化**************/
    Init_usbStr();
    Pid_Init(&CurveDirectionPID);
    Pid_Init(&SpeedPID);
    FF_Init(&SpeedFF,38,0,0.2);
    PidValue();
    Kalman_Init(&CarSpeedAfterFilter);
    Kalman_Init(&GyroYAfterFilter);
    Kalman_Init(&GyroZAfterFilter);
    Menu_Init();
    RescueInit();
    /*************参数初始化**************/

    interrupt_global_enable(0);
    pit_ms_init(CCU60_CH0, 5);
    pit_start(CCU60_CH0);
    pit_ms_init(CCU60_CH1, 5);
    pit_start(CCU60_CH1);
    adc_init (ADC0_CH0_A0, ADC_10BIT);
    cpu_wait_event_ready();         // 等待所有核心初始化完毕

    /*************开机动画**************/
    Buzz_Ctrl(BUZZ0, ON);
    oled_set_font(1);
    oled_show_string(0,3,"Sir,I'm ready");
    system_delay_ms(700);
    Buzz_Ctrl(BUZZ0, OFF);
    oled_clear();
    oled_set_font(0);
    /*************开机动画**************/

    cpu_wait_event_ready();         // 等待所有核心初始化完毕
    while (TRUE)
    {
        MainFunc();
        //CarState_Show();
    }
}

#pragma section all restore
// **************************** 代码区域 ****************************
