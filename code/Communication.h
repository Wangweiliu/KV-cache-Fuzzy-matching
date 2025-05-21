#ifndef CODE_COMMUNICATION_H_
#define CODE_COMMUNICATION_H_

#include "zf_common_headfile.h"

#define USB_FRAME_HEAD              0x42          //USB通信序列帧头
#define USB_FRAME_END               0x10          //USB通信序列帧尾
#define USB_FRAME_LENGTH            8             //USB通信序列字节长度

#define USB_ADDR_SPEEDBACK          0x05  //速度
#define USB_ADDR_RESETBACK          0x02  //初始化
#define USB_ADDR_RUNBACK            0x08  //发车，程序启动

#define USB_ADDR_RESCUEBACK         0x06  //救援区
#define USB_ADDR_Elem               0x09  //元素表
#define USB_ADDR_ANCHOR             0x10  //前瞻

extern uint8 IfReceived;
typedef struct
{
    bool ReceiveStart;                            //数据接收开始
    uint8 ReceiveIndex;                           //接收序列
    bool ReceiveFinished;                         //数据队列接收并校验完成
    uint8 ReceiveBuff[USB_FRAME_LENGTH];          //USB接收队列：临时接收
    uint8 ReceiveBuffFinished[USB_FRAME_LENGTH];  //USB接收队列：校验成功
}USB_STRUCT;
extern USB_STRUCT usbStr;

typedef union
{
    uint8 U8_Buff[2];
    int16 SignedShort;
}UNION_BIT16;

extern uint8 Reset_flag;
extern uint8 Rescue_stopFinish_flag;
extern uint8 Run_flag;
extern uint8 If_ReceivedInfo;
extern uint8 If_ReceivedInfo_Frame;
extern uint16 Receive_Frame;
extern uint16 Init_If_ReceivedInfo;

void uart_receiveVerify(uint8 data);
void Init_usbStr(void);
void USB_Edgeboard_Rescue(void);
void USB_Edgeboard_Reset(void);
void USB_Edgeboard_Run(void);
void USB_Edgeboard_Elem(void);
void USB_Edgeboard_Anchor(void);

#endif
