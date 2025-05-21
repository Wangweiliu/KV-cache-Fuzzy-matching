#include "isr_config.h"
#include "isr.h"

#define _e 2.71828
int temp = 0;

float f(float x) {
    float exponential_numerator = exp(-fabs(x / 16.0)) - 1.0;
    float exponential_denominator = exp(-fabs(x)) + 1.0;
    float result = (fabs((exponential_numerator / exponential_denominator)) / 2.0 + 0.4) * 118.0;

    return result;
}

// 对于TC系列默认是不支持中断嵌套的，希望支持中断嵌套需要在中断内使用 interrupt_global_enable(0); 来开启中断嵌套
// 简单点说实际上进入中断后TC系列的硬件自动调用了 interrupt_global_disable(); 来拒绝响应任何的中断，因此需要我们自己手动调用 interrupt_global_enable(0); 来开启中断的响应。

// **************************** PIT中断函数 ****************************
IFX_INTERRUPT(cc60_pit_ch0_isr, 0, CCU6_0_CH0_ISR_PRIORITY)
{
    interrupt_global_enable(0);                     // 开启中断嵌套
    pit_clear_flag(CCU60_CH0);

    EncoderValue_Get();
    if(MotorFlag != 0)
    {
        Cal_RoadLength(0.005);

        switch(TrackState)
        {
            /***************************传统赛道*****************************/
            case BaseHandle:
                flyFrame ++;
                _Speed = SpeedOfBaseHandle;
            break;

            case BaseHandle_speedup:
                flyFrame += 2;
                _Speed = SpeedOfBaseHandle_speedup;
            break;

            case BaseHandle_down:
                if (SpeedOfBaseHandle > 85)
                {
                    if (flyFrame > 10)
                    {
                        _Speed = SpeedOfBaseHandle_wan - flyFrame * SLp;
                    }
                }
                break;
            case BaseHandle_wan:
                flyFrame = 0;
                _Speed = SpeedOfBaseHandle_wan - func_abs(TrueDirError) * KLp;
                if (_Speed < Speed_min) { _Speed = Speed_min; }
                break;

            case BigRCurve:
                _Speed = SpeedOfBigRCurve;
            break;

            case RuHuan_Small:
                _Speed = SpeedOfRuHuan_Small;
                CurveDirectionPID.kp = KpofRuHuan_Small;
                CurveDirectionPID.kd = KpofRuHuan_Small;
            break;

            case HuanZhong_Small:
                _Speed = SpeedOfHuanZhong_Small;
                CurveDirectionPID.kp = KpofHuanZhong_Small;
                CurveDirectionPID.kd = KpofHuanZhong_Small;
            break;

            case ChuHuan_Small:
                _Speed = SpeedOfChuHuan_Small;
                CurveDirectionPID.kp = KpofChuHuan_Small;
                CurveDirectionPID.kd = KpofChuHuan_Small;
            break;

            case RuHuan_Big:
                _Speed = SpeedOfRuHuan_Big;
                CurveDirectionPID.kp = KpofRuHuan_Big;
                CurveDirectionPID.kd = KpofRuHuan_Big;
            break;

            case HuanZhong_Big:
                _Speed = SpeedOfHuanZhong_Big;
                CurveDirectionPID.kp = KpofHuanZhong_Big;
                CurveDirectionPID.kd = KpofHuanZhong_Big;
            break;

            case ChuHuan_Big:
                _Speed = SpeedOfChuHuan_Big;
                CurveDirectionPID.kp = KpofChuHuan_Big;
                CurveDirectionPID.kd = KpofChuHuan_Big;
            break;

            case Ramp:
                _Speed = SpeedOfRamp;
            break;

            case Ramp_fly:
                _Speed = SpeedOfRamp_fly;
            break;

            case Stop:
                _Speed = 0;
            break;

            case ZebraCrossing:
                ZebraCrossing_Process();
            break;
            /***************************传统赛道*****************************/


            /***************************模型部分*****************************/
            case RescueScene:
                _Speed = SpeedOfSlowDown_Big;
            break;

            case DangerScene:
                _Speed = SpeedOfDanger;
                CurveDirectionPID.kp = KpofBlocks;
                CurveDirectionPID.kd = KdofBlocks;
                SpeedPID.kp = SpeedKpofBlocks;
                SpeedPID.ki = SpeedKiofBlocks;
            break;

            case SlowDown:
                _Speed = SpeedOfSlowDown;
            break;

            case SlowDown_big:
                _Speed = SpeedOfSlowDown_Big;
            break;
            /***************************模型部分*****************************/

            case -1:
                _Speed = 0;
            break;

            default:
            break;
        }

        if(!If_ReceivedInfo)//未接受到上位机信息
        {
            _Speed = 0;
        }

        ElemBuzz();
        ZebraControl();

        if( TrackState != DangerScene      && TrackState != RuHuan_Small   &&
            TrackState != HuanZhong_Small  && TrackState != ChuHuan_Small  &&
            TrackState != ChuHuan_Big      && TrackState != RuHuan_Big     &&
            TrackState != HuanZhong_Big                                        )
        {
            if(IfOpenFuzzy == OPEN)
            {
                CurveDirectionPID.kp = KP_Fuzzy((float)TrueDirError,(float)(TrueDirError - LastTrueDirError));
                CurveDirectionPID.kd = Kd_Fuzzy((float)(TrueDirError - LastTrueDirError));
            }
            else
            {
                CurveDirectionPID.kp = SteerKp;
                CurveDirectionPID.kd = SteerKd;
            }
        }
        if(ZebraBeforeStopping == 0 && TrackState != DangerScene)
        {
            SpeedPID.kp = KpofCommon;
            SpeedPID.ki = KiofCommon;
            SpeedPID.kd = KdofCommon;
            Pid_update(&SpeedPID ,(float)( _Speed - CarSpeed ));
        }


        if(IfOpenFF == OPEN){ FF_Calculate(&SpeedFF,(float)_Speed );PWMOut += SpeedFF.Out; }

        if(TrackState != RescueHandle_L && TrackState != RescueHandle_R && TrackState != Ramp)
            _ServoDutyfactor=ServoMid - 2*(Pid_LocCtrl(&CurveDirectionPID, (float)TrueDirError));
        Servo_Constrain(&_ServoDutyfactor);
        Set_Angel((uint16)_ServoDutyfactor);
        PWMOut = (int)Pid_IncCtrl(&SpeedPID, (float)( _Speed - CarSpeed ));

        if(MotorPower == 1)
            Set_MoterSpeed(PWMOut);
        else
            Set_MoterSpeed(0);
    }
    else
    {
        if(TestMotor_Flag ==0 )
        {
            Set_MoterSpeed(0);
        }
        else if(TestMotor_Flag == 1)
        {
            Set_MoterSpeed(PWMOut);
        }
    }

}


IFX_INTERRUPT(cc60_pit_ch1_isr, 0, CCU6_0_CH1_ISR_PRIORITY)
{
    interrupt_global_enable(0);                     // 开启中断嵌套
    pit_clear_flag(CCU60_CH1);

    if(rescuehandle_lrmain.RescueHandle_LR_FLAG == 1)
    {
        rescuehandle_lrmain.S_sec ++;
    }

    Init_If_ReceivedInfo ++;
    if(Init_If_ReceivedInfo > 10)
    {
        Init_If_ReceivedInfo = 0;
        If_ReceivedInfo = 0;
    }

}

IFX_INTERRUPT(cc61_pit_ch0_isr, 0, CCU6_1_CH0_ISR_PRIORITY)
{
    interrupt_global_enable(0);                     // 开启中断嵌套
    pit_clear_flag(CCU61_CH0);




}

IFX_INTERRUPT(cc61_pit_ch1_isr, 0, CCU6_1_CH1_ISR_PRIORITY)
{
    interrupt_global_enable(0);                     // 开启中断嵌套
    pit_clear_flag(CCU61_CH1);




}
// **************************** PIT中断函数 ****************************


// **************************** 外部中断函数 ****************************
IFX_INTERRUPT(exti_ch0_ch4_isr, 0, EXTI_CH0_CH4_INT_PRIO)
{
    interrupt_global_enable(0);                     // 开启中断嵌套
    if(exti_flag_get(ERU_CH0_REQ0_P15_4))           // 通道0中断
    {
        exti_flag_clear(ERU_CH0_REQ0_P15_4);

    }

    if(exti_flag_get(ERU_CH4_REQ13_P15_5))          // 通道4中断
    {
        exti_flag_clear(ERU_CH4_REQ13_P15_5);




    }
}

IFX_INTERRUPT(exti_ch1_ch5_isr, 0, EXTI_CH1_CH5_INT_PRIO)
{
    interrupt_global_enable(0);                     // 开启中断嵌套

    if(exti_flag_get(ERU_CH1_REQ10_P14_3))          // 通道1中断
    {
        exti_flag_clear(ERU_CH1_REQ10_P14_3);

        tof_module_exti_handler();                  // ToF 模块 INT 更新中断

    }

    if(exti_flag_get(ERU_CH5_REQ1_P15_8))           // 通道5中断
    {
        exti_flag_clear(ERU_CH5_REQ1_P15_8);


    }
}

// 由于摄像头pclk引脚默认占用了 2通道，用于触发DMA，因此这里不再定义中断函数
// IFX_INTERRUPT(exti_ch2_ch6_isr, 0, EXTI_CH2_CH6_INT_PRIO)
// {
//  interrupt_global_enable(0);                     // 开启中断嵌套
//  if(exti_flag_get(ERU_CH2_REQ7_P00_4))           // 通道2中断
//  {
//      exti_flag_clear(ERU_CH2_REQ7_P00_4);
//  }
//  if(exti_flag_get(ERU_CH6_REQ9_P20_0))           // 通道6中断
//  {
//      exti_flag_clear(ERU_CH6_REQ9_P20_0);
//  }
// }

IFX_INTERRUPT(exti_ch3_ch7_isr, 0, EXTI_CH3_CH7_INT_PRIO)
{
    interrupt_global_enable(0);                     // 开启中断嵌套
    if(exti_flag_get(ERU_CH3_REQ6_P02_0))           // 通道3中断
    {
        exti_flag_clear(ERU_CH3_REQ6_P02_0);
        camera_vsync_handler();                     // 摄像头触发采集统一回调函数
    }
    if(exti_flag_get(ERU_CH7_REQ16_P15_1))          // 通道7中断
    {
        exti_flag_clear(ERU_CH7_REQ16_P15_1);




    }
}
// **************************** 外部中断函数 ****************************


// **************************** DMA中断函数 ****************************
IFX_INTERRUPT(dma_ch5_isr, 0, DMA_INT_PRIO)
{
    interrupt_global_enable(0);                     // 开启中断嵌套
    camera_dma_handler();                           // 摄像头采集完成统一回调函数
}
// **************************** DMA中断函数 ****************************


// **************************** 串口中断函数 ****************************
// 串口0默认作为调试串口
IFX_INTERRUPT(uart0_tx_isr, 0, UART0_TX_INT_PRIO)
{
    interrupt_global_enable(0);                     // 开启中断嵌套



}
IFX_INTERRUPT(uart0_rx_isr, 0, UART0_RX_INT_PRIO)
{
    interrupt_global_enable(0);                     // 开启中断嵌套

    If_ReceivedInfo = uart_query_byte(UART_0,&DataFromEdgeBoard);
    uart_receiveVerify(DataFromEdgeBoard);
    Receive_Frame ++;
    if(Receive_Frame >= 65534)
    {
        Receive_Frame = 0;
    }
    Init_If_ReceivedInfo = 0;
//    if(!If_ReceivedInfo) {
//        If_ReceivedInfo_Frame ++;
//    }
//    else{
//        If_ReceivedInfo_Frame = 0;
//    }
//
//    if(If_ReceivedInfo_Frame >= 50) {
//        If_ReceivedInfo_Frame = 50;
//    }

#if DEBUG_UART_USE_INTERRUPT                        // 如果开启 debug 串口中断
        debug_interrupr_handler();                  // 调用 debug 串口接收处理函数 数据会被 debug 环形缓冲区读取
#endif                                              // 如果修改了 DEBUG_UART_INDEX 那这段代码需要放到对应的串口中断去
}


// 串口1默认连接到摄像头配置串口
IFX_INTERRUPT(uart1_tx_isr, 0, UART1_TX_INT_PRIO)
{
    interrupt_global_enable(0);                     // 开启中断嵌套




}
IFX_INTERRUPT(uart1_rx_isr, 0, UART1_RX_INT_PRIO)
{
    interrupt_global_enable(0);                     // 开启中断嵌套
    camera_uart_handler();                          // 摄像头参数配置统一回调函数
}

// 串口2默认连接到无线转串口模块
IFX_INTERRUPT(uart2_tx_isr, 0, UART2_TX_INT_PRIO)
{
    interrupt_global_enable(0);                     // 开启中断嵌套



}

IFX_INTERRUPT(uart2_rx_isr, 0, UART2_RX_INT_PRIO)
{
    interrupt_global_enable(0);                     // 开启中断嵌套
    wireless_module_uart_handler();                 // 无线模块统一回调函数



}
// 串口3默认连接到GPS定位模块
IFX_INTERRUPT(uart3_tx_isr, 0, UART3_TX_INT_PRIO)
{
    interrupt_global_enable(0);                     // 开启中断嵌套



}

IFX_INTERRUPT(uart3_rx_isr, 0, UART3_RX_INT_PRIO)
{
    interrupt_global_enable(0);                     // 开启中断嵌套
    gnss_uart_callback();                           // GNSS串口回调函数



}

// 串口通讯错误中断
IFX_INTERRUPT(uart0_er_isr, 0, UART0_ER_INT_PRIO)
{
    interrupt_global_enable(0);                     // 开启中断嵌套
    IfxAsclin_Asc_isrError(&uart0_handle);
}
IFX_INTERRUPT(uart1_er_isr, 0, UART1_ER_INT_PRIO)
{
    interrupt_global_enable(0);                     // 开启中断嵌套
    IfxAsclin_Asc_isrError(&uart1_handle);
}
IFX_INTERRUPT(uart2_er_isr, 0, UART2_ER_INT_PRIO)
{
    interrupt_global_enable(0);                     // 开启中断嵌套
    IfxAsclin_Asc_isrError(&uart2_handle);
}
IFX_INTERRUPT(uart3_er_isr, 0, UART3_ER_INT_PRIO)
{
    interrupt_global_enable(0);                     // 开启中断嵌套
    IfxAsclin_Asc_isrError(&uart3_handle);
}
