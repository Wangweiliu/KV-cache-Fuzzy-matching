#include<code/Communication.h>

USB_STRUCT usbStr;
uint8 IfReceived = 0;
uint8 Reset_flag = 1;
uint8 Rescue_stopFinish_flag = 0;
uint8 Run_flag = 0;
uint8 If_ReceivedInfo = 0;
uint8 If_ReceivedInfo_Frame = 0;
uint16 Receive_Frame = 0;
uint16 Init_If_ReceivedInfo = 0;

void Init_usbStr()
{
    usbStr.ReceiveStart=0;
    usbStr.ReceiveFinished=0;
}

void uart_receiveVerify(uint8 data)
{
    if(data == USB_FRAME_HEAD && !usbStr.ReceiveStart)//监测帧头
    {
        usbStr.ReceiveStart = 1;
        usbStr.ReceiveBuff[0] = data;
        usbStr.ReceiveIndex = 1;
    }
    else if(usbStr.ReceiveStart && usbStr.ReceiveIndex<USB_FRAME_LENGTH)//通信开始|接收队列数据
    {
        usbStr.ReceiveBuff[usbStr.ReceiveIndex] = data;
        usbStr.ReceiveIndex++;
    }

    if(usbStr.ReceiveIndex>=USB_FRAME_LENGTH)
    {
        if(usbStr.ReceiveBuff[USB_FRAME_LENGTH-1] == USB_FRAME_END)//帧尾
        {
            uint8 check = 0;
            for(int i=2;i<6;i++) check += usbStr.ReceiveBuff[i];

            if(check == usbStr.ReceiveBuff[6])//校验位
            {
                memcpy(usbStr.ReceiveBuffFinished,usbStr.ReceiveBuff,USB_FRAME_LENGTH);
                usbStr.ReceiveFinished = 1;

                //通信成功！！！速度和方向控制特殊处理----------------------------------------------------------------------
                uint8 Addr = (uint8)(usbStr.ReceiveBuffFinished[1] & 0xFF);//相当于取余
                uint8 BuffData[4] = {0};
                UNION_BIT16 UnionBit16_1;
                UNION_BIT16 UnionBit16_2;
                for(int i=0;i<4;i++)
                {
                    BuffData[i] = usbStr.ReceiveBuffFinished[2+i];
                }
                switch(Addr)
                {
                    case 0x01:
                        UnionBit16_1.U8_Buff[0] = BuffData[0];
                        UnionBit16_1.U8_Buff[1] = BuffData[1];
                        LastTrueDirError = TrueDirError;
                        TrueDirError = UnionBit16_1.SignedShort;


                        LastTrackState = TrackState;
                        UnionBit16_2.U8_Buff[0] = BuffData[2];
                        UnionBit16_2.U8_Buff[1] = BuffData[3];
                        TrackState = UnionBit16_2.SignedShort;

                        break;
                }
                //---------------------------------------------------------------------------------------------------
            }
        }

        usbStr.ReceiveIndex = 0;
        usbStr.ReceiveStart = 0;
    }
}

void USB_Edgeboard_CarSpeed(void)
{
    UNION_BIT16 bint16_Union;
    uint8 check = 0;
    uint8 buff[7] = {0};
    buff[0] = USB_FRAME_HEAD;     //帧头
    buff[1] = USB_ADDR_SPEEDBACK; //地址
    buff[2] = 0x06;               //帧长

    bint16_Union.SignedShort = (signed short)CarSpeed;
    buff[3] = bint16_Union.U8_Buff[0];
    buff[4] = bint16_Union.U8_Buff[1];

    for(int i=0;i<5;i++)
        check += buff[i];

    buff[6] = check;

    for(int i=0;i<7;i++)
        uart_write_byte(UART_0,buff[i]);
}


void USB_Edgeboard_Reset()
{
    uint8 check = 0;
    uint8 buff[7] = {0};
    buff[0] = USB_FRAME_HEAD;     //帧头
    buff[1] = USB_ADDR_RESETBACK; //地址
    buff[2] = 5;                  //帧长
    buff[3] = Reset_flag;
    for(int i=0;i<5;i++)
        check += buff[i];

    buff[4] = check;

    for(int i=0;i<5;i++)
        uart_write_byte(UART_0,buff[i]);
}

void USB_Edgeboard_Rescue()
{
    uint8 check = 0;
    uint8 buff[5] = {0};
    buff[0] = USB_FRAME_HEAD;      //帧头
    buff[1] = USB_ADDR_RESCUEBACK; //地址
    buff[2] = 5;                   //帧长
    buff[3] = rescuehandle_lrmain.StopFinishflag;//Rescue_stopFinish_flag;
    for(int i=0;i<5;i++)
        check += buff[i];

    buff[4] = check;

    for(int i=0;i<5;i++)
        uart_write_byte(UART_0,buff[i]);
}

void USB_Edgeboard_Run()
{
    uint8 check = 0;
    uint8 buff[7] = {0};
    buff[0] = USB_FRAME_HEAD;     //帧头
    buff[1] = USB_ADDR_RUNBACK;   //地址
    buff[2] = 0x05;               //帧长
    buff[3] = Run_flag;
    for(int i=0;i<5;i++)
        check += buff[i];

    buff[4] = check;

    for(int i=0;i<5;i++)
        uart_write_byte(UART_0,buff[i]);
}


void USB_Edgeboard_Elem()
{
    uint8 check = 0;
    uint8 buff[13] = {0};
    buff[0] = USB_FRAME_HEAD;      //帧头
    buff[1] = USB_ADDR_Elem;       //地址
    buff[2] = 13;                  //帧长 0,1,2 3~11 ,12

    for(uint8 i = 0; i<9; i++)
    {
        buff[i+3] = ElemLists_Array[i];
    }

    for(int i = 0; i < 12; i++)
        check += buff[i];

    buff[12] = check;

    for(int i = 0 ;i < 13; i++)
        uart_write_byte(UART_0,buff[i]);
}

void USB_Edgeboard_Anchor()
{
    uint8 check = 0;
    uint8 buff[8] = {0};
    buff[0] = USB_FRAME_HEAD;
    buff[1] = USB_ADDR_ANCHOR;
    buff[2] = 8;
    buff[3] = Anchor_Now;
    buff[4] = Anchor_Roundabout;
    buff[5] = Anchor_Blocks;
    buff[6] = Anchor_Bridge;

    for(int i=0;i<7;i++)
        check += buff[i];

    buff[7] = check;

    for(int i=0;i<8;i++)
        uart_write_byte(UART_0,buff[i]);
}
