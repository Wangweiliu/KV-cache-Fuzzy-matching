#include "zf_common_headfile.h"
#pragma section all "cpu1_dsram"
// 将本语句与#pragma section all restore语句之间的全局变量都放在CPU1的RAM中


void core1_main(void)
{
    disable_Watchdog();                     // 关闭看门狗
    interrupt_global_enable(0);             // 打开全局中断

    uart_init(UART_0,115200,UART0_TX_P15_2,UART0_RX_P15_3);
    uart_rx_interrupt(UART_0, 1);

    cpu_wait_event_ready();                 // 等待所有核心初始化完毕
    while (TRUE)
    {
        if(IfOpenWirelessUart == 1)
            printf("%lf,%lf\n",_Speed,CarSpeed);
        if(TrackState == RescueHandle_L || TrackState == RescueHandle_R)
        {
            Gddaojiao_judge();
        }

        if((LastTrackState == RescueHandle_L || LastTrackState == RescueHandle_R) && TrackState != RescueHandle_R && TrackState != RescueHandle_L)
        {
            RescueReset();
        }

        if(IFSend[0] == OPEN)
        {
            USB_Edgeboard_Elem();
        }
        if(IFSend[1] == OPEN)
        {
            USB_Edgeboard_Anchor();
        }
        if(IFSend[2] == OPEN)
        {
            USB_Edgeboard_Reset();
        }
    }
}
#pragma section all restore

