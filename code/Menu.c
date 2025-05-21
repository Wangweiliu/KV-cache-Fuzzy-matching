#include <code/Menu.h>
#include <stdlib.h>

MT Root[3];              // 根节点
MT FirstLevelMenu[24]; // 一级菜单
MT Iterator;

uint8 RowCursor = 0;
uint8 Elem_Cursor = 0;

uint8 Level = 0;
uint8 Page = 0;
uint8 PageNum = 3;

uint16 ServoDutyfactor = 1468;
uint16 MotorPWM_1 = 0;
uint16 MotorPWM_2 = 0;

uint8 IfOpenWirelessUart = 0;

void MainFunc()
{
    Get_Battery();                              // 显示电池电压
    oled_show_string(0, RowCursor, ">>");
    Change_Page();                              // 翻页
    Change_Level();                             // 进入次级菜单或返回根页面
    Iterator->PValue();                         // 显示界面
    Move_Cursor();                              // 移动光标
    RunorStopWhilePressing();                   // 一键发车或停车

    key_scanner();
    system_delay_ms(20);
}

void Change_Level()
{
    if (key_get_state(KEY_5) == KEY_LONG_PRESS && Iterator->Level == 1)
    {
        Level = 0;
        oled_clear();
        Iterator = Root[Page];
        key_clear_state(KEY_5);
    }

    if (key_get_state(KEY_5) == KEY_SHORT_PRESS && Iterator->Level == 0)
    {
        Level = 1;
        oled_clear();
        switch (RowCursor + Page * 8)
        {
            case 0:Iterator = FirstLevelMenu[0];break;
            case 1:Iterator = FirstLevelMenu[1];break;
            case 2:Iterator = FirstLevelMenu[2];break;
            case 3:Iterator = FirstLevelMenu[3];break;
            case 4:Iterator = FirstLevelMenu[4];break;
            case 5:Iterator = FirstLevelMenu[5];break;
            case 6:Iterator = FirstLevelMenu[6];break;
            case 7:Iterator = FirstLevelMenu[7];break;

            case 8:Iterator = FirstLevelMenu[8];break;
            case 9:Iterator = FirstLevelMenu[9];break;
            case 10:Iterator = FirstLevelMenu[10];break;
            case 11:Iterator = FirstLevelMenu[11];break;
            case 12:Iterator = FirstLevelMenu[12];break;
            case 13:Iterator = FirstLevelMenu[13];break;
            case 14:Iterator = FirstLevelMenu[14];break;
            case 15:Iterator = FirstLevelMenu[15];break;

            case 16:Iterator = FirstLevelMenu[16];break;
            case 17:Iterator = FirstLevelMenu[17];break;
            case 18:Iterator = FirstLevelMenu[18];break;
            default:break;
        }
        key_clear_state(KEY_5);
    }
}

void Move_Cursor()
{
    if (key_get_state(KEY_2) == KEY_SHORT_PRESS || key_get_state(KEY_2) == KEY_LONG_PRESS)
    {
        oled_show_string(0, RowCursor, "  ");
        RowCursor = (RowCursor + 1) % 8;
        key_clear_state(KEY_2);
    }
    if (key_get_state(KEY_1) == KEY_SHORT_PRESS || key_get_state(KEY_1) == KEY_LONG_PRESS)
    {
        oled_show_string(0, RowCursor, "  ");
        RowCursor = (RowCursor + 7) % 8;
        key_clear_state(KEY_1);
    }
}

void Change_Page()
{
    if (key_get_state(KEY_3) == KEY_SHORT_PRESS && Level == 0)
    {
        Page = (Page + 1) % PageNum;
        oled_clear();
        Iterator = Root[Page];
    }
    if (key_get_state(KEY_4) == KEY_SHORT_PRESS && Level == 0)
    {
        if(Page != 0)
            Page = (Page - 1) % PageNum;
        else
            Page = PageNum - 1;
        oled_clear();
        Iterator = Root[Page];
    }
}

void Menu_Init()
{
    for(uint8 i = 0;i < PageNum; i++)
    {
        Root[i] = (MT)malloc(sizeof(struct MenuTree));
    }
    Iterator = (MT)malloc(sizeof(struct MenuTree));
    for(uint8 i = 0; i < PageNum * 8; i++)
    {
        FirstLevelMenu[i] = (MT)malloc(sizeof(struct MenuTree));
    }
    for(uint8 i = 0;i < PageNum; i++)
    {
        Root[i]->Level = 0;
        Root[i]->Father = NULL;
        Root[0]->FirstChild = FirstLevelMenu[i*7 + 1];
        Root[i]->NextSibling = NULL;
    }
    Root[0]->PValue = &MainMenu1;
    Root[1]->PValue = &MainMenu2;
    Root[2]->PValue = &MainMenu3;

    Iterator = Root[0];
    for(uint8 j = 0; j < PageNum; j++)
    {
        for (uint8 i = 0 + j*8; i <= 7 + j*8; i++)
        {
            FirstLevelMenu[i]->Level = 1;
            if (i != 7 && i != 15 && i != 23)
            {
                FirstLevelMenu[i]->NextSibling = FirstLevelMenu[i + 1];
            }
            if (i == 0 || i== 8 || i== 16)
            {
                FirstLevelMenu[1]->Father = Root[0];
            }
            else
            {
                FirstLevelMenu[i]->Father = FirstLevelMenu[i - 1];
            }
        }
    }
    FirstLevelMenu[0]->PValue = &PID;
    FirstLevelMenu[1]->PValue = &InitParameter;
    FirstLevelMenu[2]->PValue = &SpeedStrategy;
    FirstLevelMenu[3]->PValue = &Roundabout_Small;
    FirstLevelMenu[4]->PValue = &DangerANDRescure;
    FirstLevelMenu[5]->PValue = &Bomp;
    FirstLevelMenu[6]->PValue = &Zebra;
    FirstLevelMenu[7]->PValue = &Roundabout_Big;


    FirstLevelMenu[8]->PValue = &Elem_Lists;
    FirstLevelMenu[9]->PValue = &Anchor_Adj;
    FirstLevelMenu[10]->PValue = &Fuzzy_Kp;
    FirstLevelMenu[11]->PValue = &Fuzzy_Kd;
    FirstLevelMenu[12]->PValue = &FuzzyPID_EF;
    FirstLevelMenu[13]->PValue = &FuzzyPID_DF;
    FirstLevelMenu[14]->PValue = &Test_Key;
    FirstLevelMenu[15]->PValue = &Test_Hardware;


    FirstLevelMenu[16]->PValue = &SLow_Down;
    FirstLevelMenu[17]->PValue = &CarState_Show;
    FirstLevelMenu[18]->PValue = &Kalman;
    //    FirstLevelMenu[12]->PValue = &ADRC_NLSEF;
    //    FirstLevelMenu[13]->PValue = &ADRC_TD_ESO;
}

void MainMenu1()
{
    oled_show_string(20, 0, "BasePID");
    oled_show_string(20, 1, "InitParameter");
    oled_show_string(20, 2, "BaseHandle");
    oled_show_string(20, 3, "RdSmall");
    oled_show_string(20, 4, "Rescue");
    oled_show_string(20, 5, "Danger");
    oled_show_string(20, 6, "Zebra&Ramp");
    oled_show_string(20, 7, "RdBig");

    oled_show_uint(80, 0, MotorFlag, 1);
    oled_show_float(97, 0, BatteryVoltage, 3, 2);
    oled_show_string(85, 5, "Ifr:");
    oled_show_uint(108, 5, If_ReceivedInfo, 1);

    oled_show_string(85, 6, "Err:");
    oled_show_int(108, 6, TrueDirError, 3);
    oled_show_string(85, 7, "Trk:");
    oled_show_int(108, 7, TrackState, 2);
}

void MainMenu2()
{
    oled_show_string(20, 0, "Elem_Lists");
    oled_show_string(20, 1, "Anchor");
    oled_show_string(20, 2, "Fuzzy_Kp");
    oled_show_string(20, 3, "Fuzzy_Kd");
    oled_show_string(20, 4, "FuzzyPID_EF");
    oled_show_string(20, 5, "FuzzyPID_DF");
    oled_show_string(20, 6, "TestKey");
    oled_show_string(20, 7, "Test_Hardware");
}

void MainMenu3()
{
    oled_show_string(20, 0, "SlowDown");
    oled_show_string(20, 1, "CarState");
    oled_show_string(20, 2, "Kalman");
}

uint8 SendResetFrame = 100;
void RunorStopWhilePressing()
{
    if (key_get_state(KEY_6) == KEY_SHORT_PRESS)
    {
        while(SendResetFrame != 0)
        {
            SendResetFrame --;
            IFSend[2] = OPEN;
        }
        if(SendResetFrame == 0)
        {
            IFSend[2] = CLOSE;
        }

        MotorFlag ++;
        Init_Parameter();
        oled_clear();
        if(MotorFlag == 1)
        {
            MotorPower = 0;
        }
        else if(MotorFlag == 2)
        {
            system_delay_ms(1000);
            Init_Parameter();
            MotorPower = 1;
        }

        if(MotorFlag > 2)
        {
            MotorFlag = 2;
        }

        if(MotorFlag != 0)
        {
            Iterator = FirstLevelMenu[17];
        }
    }
    else if (key_get_state(KEY_7) == KEY_SHORT_PRESS)
    {
        MotorFlag = 0;
        Run_flag = 0;
        SendResetFrame = 100;
    }
}

void PID()
{
    oled_show_string(20, 0, "CurveDirP:");
    oled_show_float(90, 0, CurveDirectionPID.kp, 3, 2);
    oled_show_string(20, 1, "CurveDirD:");
    oled_show_float(90, 1, CurveDirectionPID.kd, 3, 2);

    oled_show_string(20, 2, "SpeedP:");
    oled_show_float(90, 2, KpofCommon, 3, 2);
    oled_show_string(20, 3, "SpeedI:");
    oled_show_float(90, 3, KiofCommon, 3, 2);
    oled_show_string(20, 4, "SpeedD:");
    oled_show_float(90, 4, KdofCommon, 3, 2);

    oled_show_string(20, 5, "SFF_KP:");
    oled_show_float(90, 5, SpeedFF.KP, 2, 1);
    oled_show_string(20, 6, "SFF_KD:");
    oled_show_float(90, 6, SpeedFF.KD, 2, 1);
    oled_show_string(20, 7, "Filter_Coe:");
    oled_show_float(90, 7, SpeedFF.Filter_Coe, 2, 1);



    if (key_get_state(KEY_3) == KEY_LONG_PRESS || key_get_state(KEY_3) == KEY_SHORT_PRESS)
    {
        switch (RowCursor)
        {
            case 0:CurveDirectionPID.kp += 0.1;break;
            case 1:CurveDirectionPID.kd += 0.1;break;
            case 2:KpofCommon += 0.1;break;
            case 3:KiofCommon += 0.1;break;
            case 4:KdofCommon += 0.1;break;
            case 5:SpeedFF.KP += 0.1;break;
            case 6:SpeedFF.KD += 0.1;break;
            case 7:SpeedFF.Filter_Coe += 0.1;break;
            default:break;
        }
        key_clear_state(KEY_3);
    }
    if (key_get_state(KEY_4) == KEY_LONG_PRESS || key_get_state(KEY_4) == KEY_SHORT_PRESS)
    {
        switch (RowCursor)
        {
            case 0:CurveDirectionPID.kp -= 0.1;break;
            case 1:CurveDirectionPID.kd -= 0.1;break;
            case 2:KpofCommon -= 0.1;break;
            case 3:KiofCommon -= 0.1;break;
            case 4:KdofCommon -= 0.1;break;
            case 5:SpeedFF.KP -= 0.1;break;
            case 6:SpeedFF.KD -= 0.1;break;
            case 7:SpeedFF.Filter_Coe -= 0.1;break;
            default:break;
        }
        key_clear_state(KEY_4);
    }
}

void InitParameter()
{
//    oled_show_string(20, 0, "PWMOut:");
//    oled_show_int(90, 0, PWMOut, 4);
    oled_show_string(20, 1, "MotorPower:");
    oled_show_uint(90, 1, MotorPower, 1);
    oled_show_string(20, 2, "ServoMid:");
    oled_show_int(90, 2, ServoMid, 4);
    oled_show_string(20, 3, "ServoMin:");
    oled_show_int(100, 3, ServoMin, 4);
    oled_show_string(20, 4, "ServoMax:");
    oled_show_int(100, 4, ServoMax, 4);
//    oled_show_string(20, 5, "OpenOrClosed:");
//    oled_show_uint(100, 5, OpenOrClosedFlag, 1);
    oled_show_string(20, 6, "OpenWissUart:");
    oled_show_uint(100, 6, IfOpenWirelessUart, 1);
    oled_show_string(20, 7, "IfOpenFF:");
    oled_show_uint(100, 7, IfOpenFF, 1);


    if (key_get_state(KEY_3) == KEY_SHORT_PRESS || key_get_state(KEY_3) == KEY_LONG_PRESS)
    {
        switch (RowCursor)
        {
            //case 0:PWMOut += 100;break;
            case 1:MotorPower = 1;break;
            case 2:ServoMid += 1;break;
            case 3:ServoMin += 1;break;
            case 4:ServoMax += 1;break;
            //case 5:OpenOrClosedFlag = 1;break;
            case 6:IfOpenWirelessUart = 1;break;
            case 7:IfOpenFF = 1;break;

            default:break;
        }
        key_clear_state(KEY_3);
    }
    if (key_get_state(KEY_4) == KEY_SHORT_PRESS || key_get_state(KEY_4) == KEY_LONG_PRESS)
    {
        switch (RowCursor)
        {
            //case 0:PWMOut -= 100;break;
            case 1:MotorPower = 0;break;
            case 2:ServoMid -= 1;break;
            case 3:ServoMin -= 1;break;
            case 4:ServoMax -= 1;break;
            //case 5:OpenOrClosedFlag = 0;break;
            case 6:IfOpenWirelessUart = 0;break;
            case 7:IfOpenFF = 0;break;
            default:break;
        }
        key_clear_state(KEY_4);
    }
}

void SpeedStrategy()
{
    oled_show_string(20, 0, "CarSpeed:");
    oled_show_float(90, 0, CarSpeed, 3, 2);


    oled_show_string(20, 1, "min:");
    oled_show_float(90, 1, Speed_min, 3, 1);
    oled_show_string(20, 2, "BaseHandle:");
    oled_show_float(90, 2, SpeedOfBaseHandle, 3, 1);
    oled_show_string(20, 3, "BHspeedup:");
    oled_show_float(90, 3, SpeedOfBaseHandle_speedup, 3, 1);
    oled_show_string(20, 4, "wan:");
    oled_show_float(90, 4, SpeedOfBaseHandle_wan, 3, 1);
    oled_show_string(20, 6, "SLp:");
    oled_show_float(90, 6, SLp, 1, 2);
    oled_show_string(20, 7, "KLp:");
    oled_show_float(90, 7, KLp, 1, 2);

    oled_show_string(20, 5, "BigRCur:");
    oled_show_float(90, 5, SpeedOfBigRCurve, 3, 1);


    if (key_get_state(KEY_3) == KEY_LONG_PRESS || key_get_state(KEY_3) == KEY_SHORT_PRESS)
    {
        switch (RowCursor)
        {
            case 1:Speed_min += 1;break;
            case 2:SpeedOfBaseHandle += 1;break;
            case 3:SpeedOfBaseHandle_speedup += 1;break;
            case 4:SpeedOfBaseHandle_wan += 1;break;
            case 5:SpeedOfBigRCurve += 1;break;
            case 6:SLp += 0.01;break;
            case 7:KLp += 0.01;break;
            default:break;
        }
        key_clear_state(KEY_3);
    }
    if (key_get_state(KEY_4) == KEY_LONG_PRESS || key_get_state(KEY_4) == KEY_SHORT_PRESS)
    {
        switch (RowCursor)
        {
            case 1:Speed_min -= 1;break;
            case 2:SpeedOfBaseHandle -= 1;break;
            case 3:SpeedOfBaseHandle_speedup -= 1;break;
            case 4:SpeedOfBaseHandle_wan -= 1;break;
            case 5:SpeedOfBigRCurve -= 1;break;
            case 6:SLp -= 0.01;break;
            case 7:KLp -= 0.01;break;
            default:break;
        }
        key_clear_state(KEY_4);
    }
}


void Elem_Lists()//bomb(0), evil(1), thief(2), patient(3), tumble(4), RoundaboutSamll(5), Ramp(6)
{
    if(Elem_Cursor > 105) oled_show_string(Elem_Cursor % 126,3, "^");
    else oled_show_string(Elem_Cursor,1, "^");

    oled_show_string(0,   0, "b0");//0
    oled_show_string(21,  0, "e1");//21
    oled_show_string(42,  0, "t2");//42
    oled_show_string(63,  0, "p3");//63
    oled_show_string(84,  0, "t4");//84
    oled_show_string(105, 0, "rs5");//105

    oled_show_string(0,  2, "rp6");//126
    oled_show_string(21, 2, "rb7");//147
    oled_show_string(42, 2, "**");//168
    oled_show_string(63, 2, "**");//189
    oled_show_string(84, 2, "**");//210

    if (key_get_state(KEY_4) == KEY_SHORT_PRESS)
    {
        if(Elem_Cursor > 105) oled_show_string(Elem_Cursor - 126, 3, " ");
        else oled_show_string(Elem_Cursor, 1, " ");
        Elem_Cursor = (Elem_Cursor + 210) % 231;
        key_clear_state(KEY_4);
    }

    if (key_get_state(KEY_3) == KEY_SHORT_PRESS)
    {
        if(Elem_Cursor > 105) oled_show_string(Elem_Cursor - 126, 3, " ");
        else oled_show_string(Elem_Cursor, 1, " ");
        Elem_Cursor = (Elem_Cursor + 21) % 231;
        key_clear_state(KEY_3);
    }

    if (key_get_state(KEY_5) == KEY_SHORT_PRESS)
    {
        switch (Elem_Cursor)
        {
            case 0:ElemLists_Array[ElemLists_Array_Index]=0;break;
            case 21:ElemLists_Array[ElemLists_Array_Index]=1;break;
            case 42:ElemLists_Array[ElemLists_Array_Index]=2;break;
            case 63:ElemLists_Array[ElemLists_Array_Index]=3;break;
            case 84:ElemLists_Array[ElemLists_Array_Index]=4;break;
            case 105:ElemLists_Array[ElemLists_Array_Index]=5;break;
            case 126:ElemLists_Array[ElemLists_Array_Index]=6;break;
            case 147:ElemLists_Array[ElemLists_Array_Index]=7;break;
//            case 168:
//            case 189:
//            case 210:
        }
        ElemLists_Array_Index = (ElemLists_Array_Index + 1) % 9;
    }

    oled_show_uint(0,  7, ElemLists_Array[0], 1);
    oled_show_uint(15, 7, ElemLists_Array[1], 1);
    oled_show_uint(30, 7, ElemLists_Array[2], 1);
    oled_show_uint(45, 7, ElemLists_Array[3], 1);
    oled_show_uint(60, 7, ElemLists_Array[4], 1);
    oled_show_uint(75, 7, ElemLists_Array[5], 1);
    oled_show_uint(90, 7, ElemLists_Array[6], 1);
    oled_show_uint(105, 7, ElemLists_Array[7], 1);
    oled_show_uint(120, 7, ElemLists_Array[8], 1);

    oled_show_string(20, 6, "Index:");
    oled_show_uint(70, 6, ElemLists_Array_Index, 1);
    oled_show_string(20, 5, "IFSend:");
    oled_show_uint(70, 5, IFSend[0], 1);

    if (key_get_state(KEY_1) == KEY_SHORT_PRESS)
    {
        IFSend[0] = 1;
        key_clear_state(KEY_1);
    }
    if (key_get_state(KEY_2) == KEY_SHORT_PRESS)
    {
        IFSend[0] = 0;
        key_clear_state(KEY_2);
    }
}


void SLow_Down()
{
    oled_show_string(20, 0, "Slow_Res:");
    oled_show_float(90, 0, SpeedOfSlowDown, 2,1);

    oled_show_string(20, 1, "Slow_Dan:");
    oled_show_float(90, 1, SpeedOfSlowDown_Big, 2,1);

    if (key_get_state(KEY_3) == KEY_SHORT_PRESS || key_get_state(KEY_3) == KEY_SHORT_PRESS)
    {
       switch (RowCursor)
       {
           case 0:SpeedOfSlowDown += 1;break;
           case 1:SpeedOfSlowDown_Big += 1;break;
       }
       key_clear_state(KEY_3);
    }
    if (key_get_state(KEY_4) == KEY_SHORT_PRESS || key_get_state(KEY_4) == KEY_SHORT_PRESS)
    {
       switch (RowCursor)
       {
           case 0:SpeedOfSlowDown -= 1;break;
           case 1:SpeedOfSlowDown_Big -= 1;break;
       }
       key_clear_state(KEY_4);
    }
}

void Anchor_Adj()
{
    oled_show_string(20, 0, "General:");
    oled_show_uint(70, 0, Anchor_Now, 2);
    oled_show_string(20, 1, "Rd:");
    oled_show_uint(70, 1, Anchor_Roundabout, 2);
    oled_show_string(20, 2, "Danger:");
    oled_show_uint(70, 2, Anchor_Blocks, 2);
    oled_show_string(20, 3, "Ramp:");
    oled_show_uint(70, 3, Anchor_Bridge, 2);
    oled_show_string(20, 4, "IFSend:");
    oled_show_uint(70, 4, IFSend[1], 1);

    if (key_get_state(KEY_3) == KEY_SHORT_PRESS || key_get_state(KEY_3) == KEY_SHORT_PRESS)
    {
       switch (RowCursor)
       {
           case 0:Anchor_Now += 1;break;
           case 1:Anchor_Roundabout += 1;break;
           case 2:Anchor_Blocks += 1;break;
           case 3:Anchor_Bridge += 1;break;
           case 4:IFSend[1] = 1;break;
       }
       key_clear_state(KEY_3);
    }
    if (key_get_state(KEY_4) == KEY_SHORT_PRESS || key_get_state(KEY_4) == KEY_SHORT_PRESS)
    {
       switch (RowCursor)
       {
           case 0:Anchor_Now -= 1;break;
           case 1:Anchor_Roundabout -= 1;break;
           case 2:Anchor_Blocks -= 1;break;
           case 3:Anchor_Bridge -= 1;break;
           case 4:IFSend[1] = 0;break;
       }
       key_clear_state(KEY_4);
    }

}

void FuzzyPID_DF()
{
    oled_show_string(20, 0, "DFF-3:");
    oled_show_float(70, 0, DFF[0], 2, 1);
    oled_show_string(20, 1, "DFF-2:");
    oled_show_float(70, 1, DFF[1], 2, 1);
    oled_show_string(20, 2, "DFF-1:");
    oled_show_float(70, 2, DFF[2], 2, 1);
    oled_show_string(20, 3, "DFF1:");
    oled_show_float(70, 3, DFF[4], 2, 1);
    oled_show_string(20, 4, "DFF2:");
    oled_show_float(70, 4, DFF[5], 2, 1);
    oled_show_string(20, 5, "DFF3:");
    oled_show_float(70, 5, DFF[6], 2, 1);

    if (key_get_state(KEY_3) == KEY_SHORT_PRESS || key_get_state(KEY_3) == KEY_SHORT_PRESS)
    {
        switch (RowCursor)
        {
            case 0:DFF[0] += 1;break;
            case 1:DFF[1] += 1;break;
            case 2:DFF[2] += 1;break;
            case 3:DFF[4] += 1;break;
            case 4:DFF[5] += 1;break;
            case 5:DFF[6] += 1;break;
        }
        key_clear_state(KEY_3);
    }
    if (key_get_state(KEY_4) == KEY_SHORT_PRESS || key_get_state(KEY_4) == KEY_SHORT_PRESS)
    {
        switch (RowCursor)
        {
            case 0:DFF[0] -= 1;break;
            case 1:DFF[1] -= 1;break;
            case 2:DFF[2] -= 1;break;
            case 3:DFF[4] -= 1;break;
            case 4:DFF[5] -= 1;break;
            case 5:DFF[6] -= 1;break;
        }
        key_clear_state(KEY_4);
    }
}

void FuzzyPID_EF()
{
    oled_show_string(20, 0, "EFF-3:");
    oled_show_float(70, 0, EFF[0], 2, 1);
    oled_show_string(20, 1, "EFF-2:");
    oled_show_float(70, 1, EFF[1], 2, 1);
    oled_show_string(20, 2, "EFF-1:");
    oled_show_float(70, 2, EFF[2], 2, 1);
    oled_show_string(20, 3, "EFF1:");
    oled_show_float(70, 3, EFF[4], 2, 1);
    oled_show_string(20, 4, "EFF2:");
    oled_show_float(70, 4, EFF[5], 2, 1);
    oled_show_string(20, 5, "EFF3:");
    oled_show_float(70, 5, EFF[6], 2, 1);

    if (key_get_state(KEY_3) == KEY_LONG_PRESS || key_get_state(KEY_3) == KEY_SHORT_PRESS)
    {
        switch (RowCursor)
        {
            case 0:EFF[0] += 1;break;
            case 1:EFF[1] += 1;break;
            case 2:EFF[2] += 1;break;
            case 3:EFF[4] += 1;break;
            case 4:EFF[5] += 1;break;
            case 5:EFF[6] += 1;break;
        }
        key_clear_state(KEY_3);
    }
    if (key_get_state(KEY_4) == KEY_LONG_PRESS || key_get_state(KEY_4) == KEY_SHORT_PRESS)
    {
        switch (RowCursor)
        {
            case 0:EFF[0] -= 1;break;
            case 1:EFF[1] -= 1;break;
            case 2:EFF[2] -= 1;break;
            case 3:EFF[4] -= 1;break;
            case 4:EFF[5] -= 1;break;
            case 5:EFF[6] -= 1;break;
        }
        key_clear_state(KEY_4);
    }
}

void Fuzzy_Kp()
{
    oled_show_string(20, 0, "Kp0:");
    oled_show_float(70, 0, Kp_UFF[0], 2, 2);
    oled_show_string(20, 1, "Kp1:");
    oled_show_float(70, 1, Kp_UFF[1], 2, 2);
    oled_show_string(20, 2, "Kp2:");
    oled_show_float(70, 2, Kp_UFF[2], 2, 2);
    oled_show_string(20, 3, "Kp3:");
    oled_show_float(70, 3, Kp_UFF[3], 2, 2);
    oled_show_string(20, 4, "Kp4:");
    oled_show_float(70, 4, Kp_UFF[4], 2, 2);
    oled_show_string(20, 5, "Kp5:");
    oled_show_float(70, 5, Kp_UFF[5], 2, 2);
    oled_show_string(20, 6, "Kp6:");
    oled_show_float(70, 6, Kp_UFF[6], 2, 2);

    if (key_get_state(KEY_3) == KEY_LONG_PRESS || key_get_state(KEY_3) == KEY_SHORT_PRESS)
    {
        switch (RowCursor)
        {
            case 0:Kp_UFF[0] += 0.01;break;
            case 1:Kp_UFF[1] += 0.01;break;
            case 2:Kp_UFF[2] += 0.01;break;
            case 3:Kp_UFF[4] += 0.01;break;
            case 4:Kp_UFF[5] += 0.01;break;
            case 5:Kp_UFF[6] += 0.01;break;
        }
        key_clear_state(KEY_3);
    }
    if (key_get_state(KEY_4) == KEY_LONG_PRESS || key_get_state(KEY_4) == KEY_SHORT_PRESS)
    {
        switch (RowCursor)
        {
            case 0:Kp_UFF[0] -= 0.01;break;
            case 1:Kp_UFF[1] -= 0.01;break;
            case 2:Kp_UFF[2] -= 0.01;break;
            case 3:Kp_UFF[4] -= 0.01;break;
            case 4:Kp_UFF[5] -= 0.01;break;
            case 5:Kp_UFF[6] -= 0.01;break;
        }
        key_clear_state(KEY_4);
    }
}

void Fuzzy_Kd()
{
    oled_show_string(20, 0, "Kd0:");
    oled_show_float(70, 0, Kd_UFF[0], 2, 2);
    oled_show_string(20, 1, "Kd1:");
    oled_show_float(70, 1, Kd_UFF[1], 2, 2);
    oled_show_string(20, 2, "Kd2:");
    oled_show_float(70, 2, Kd_UFF[2], 2, 2);
    oled_show_string(20, 3, "Kd3:");
    oled_show_float(70, 3, Kd_UFF[3], 2, 2);
    oled_show_string(20, 4, "Kd4:");
    oled_show_float(70, 4, Kd_UFF[4], 2, 2);
    oled_show_string(20, 5, "Kd5:");
    oled_show_float(70, 5, Kd_UFF[5], 2, 2);
    oled_show_string(20, 6, "Kd6:");
    oled_show_float(70, 6, Kd_UFF[6], 2, 2);

    if (key_get_state(KEY_3) == KEY_LONG_PRESS || key_get_state(KEY_3) == KEY_SHORT_PRESS)
    {
        switch (RowCursor)
        {
            case 0:Kd_UFF[0] += 0.01;break;
            case 1:Kd_UFF[1] += 0.01;break;
            case 2:Kd_UFF[2] += 0.01;break;
            case 3:Kd_UFF[4] += 0.01;break;
            case 4:Kd_UFF[5] += 0.01;break;
            case 5:Kd_UFF[6] += 0.01;break;
        }
        key_clear_state(KEY_3);
    }
    if (key_get_state(KEY_4) == KEY_LONG_PRESS || key_get_state(KEY_4) == KEY_SHORT_PRESS)
    {
        switch (RowCursor)
        {
            case 0:Kd_UFF[0] -= 0.01;break;
            case 1:Kd_UFF[1] -= 0.01;break;
            case 2:Kd_UFF[2] -= 0.01;break;
            case 3:Kd_UFF[4] -= 0.01;break;
            case 4:Kd_UFF[5] -= 0.01;break;
            case 5:Kd_UFF[6] -= 0.01;break;
        }
        key_clear_state(KEY_4);
    }
}

void Kalman()
{
    oled_show_string(20, 0, "Q:");
    oled_show_float(70, 0, CarSpeedAfterFilter.Q, 1, 3);
    oled_show_string(20, 1, "R:");
    oled_show_float(70, 1, CarSpeedAfterFilter.R, 1, 3);
    if (key_get_state(KEY_3) == KEY_SHORT_PRESS || key_get_state(KEY_3) == KEY_LONG_PRESS)
    {
        switch (RowCursor)
        {
        case 0:CarSpeedAfterFilter.Q += 0.001;break;
        case 1:CarSpeedAfterFilter.R += 0.001;break;
        }
        key_clear_state(KEY_3);
    }
    if (key_get_state(KEY_4) == KEY_SHORT_PRESS || key_get_state(KEY_4) == KEY_LONG_PRESS)
    {
        switch (RowCursor)
        {
        case 0:CarSpeedAfterFilter.Q -= 0.001;break;
        case 1:CarSpeedAfterFilter.R -= 0.001;break;
        }
        key_clear_state(KEY_4);
    }
}

void ADRC_TD_ESO()
{
    oled_show_string(20, 0, "TD.r:");
    oled_show_float(70, 0, r, 3, 1);
    oled_show_string(20, 1, "TD.h:");
    oled_show_float(70, 1, h, 2, 3);
    oled_show_string(20, 2, "ESO.b:");
    oled_show_float(70, 2, b, 2, 1);
    oled_show_string(20, 3, "ESO.del:");
    oled_show_float(70, 3, delta, 2, 1);
    oled_show_string(20, 4, "ESO.b1:");
    oled_show_float(70, 4, belta01, 2, 1);
    oled_show_string(20, 5, "ESO.b2:");
    oled_show_float(70, 5, belta02, 2, 1);
    oled_show_string(20, 6, "ESO.b3:");
    oled_show_float(70, 6, belta03, 2, 1);

    if (key_get_state(KEY_3) == KEY_SHORT_PRESS || key_get_state(KEY_3) == KEY_LONG_PRESS)
    {
        switch (RowCursor)
        {
            case 0:r += 1;break;
            case 1:h += 0.001;break;
            case 2:b += 0.1;break;
            case 3:delta += 0.1;break;
            case 4:belta01 += 0.1;break;
            case 5:belta02 += 0.1;break;
            case 6:belta03 += 0.1;break;
        }
        key_clear_state(KEY_3);
    }
    if (key_get_state(KEY_4) == KEY_SHORT_PRESS || key_get_state(KEY_4) == KEY_LONG_PRESS)
    {
        switch (RowCursor)
        {
            case 0:r -= 1;break;
            case 1:h -= 0.001;break;
            case 2:b -= 0.1;break;
            case 3:delta -= 0.1;break;
            case 4:belta01 -= 0.1;break;
            case 5:belta02 -= 0.1;break;
            case 6:belta03 -= 0.1;break;
        }
        key_clear_state(KEY_4);
    }
}

void ADRC_NLSEF()
{
    oled_show_string(20, 0, "NLF.al1:");
    oled_show_float(70, 0, alpha1, 2, 1);
    oled_show_string(20, 1, "NLF.al2:");
    oled_show_float(70, 1, alpha2, 2, 1);
    oled_show_string(20, 2, "NLF.bel1:");
    oled_show_float(70, 2, belta1, 2, 1);
    oled_show_string(20, 3, "NLF.bel2:");
    oled_show_float(70, 3, belta2, 2, 1);
    oled_show_string(20, 4, "NLF.bel3:");
    oled_show_float(70, 4, belta3, 2, 1);

    if (key_get_state(KEY_3) == KEY_SHORT_PRESS || key_get_state(KEY_3) == KEY_LONG_PRESS)
    {
        switch (RowCursor)
        {
            case 0:alpha1 += 0.1;break;
            case 1:alpha2 += 0.1;break;
            case 2:belta1 += 0.1;break;
            case 3:belta2 += 0.1;break;
            case 4:belta3 += 0.1;break;
        }
        key_clear_state(KEY_3);
    }
    if (key_get_state(KEY_4) == KEY_SHORT_PRESS || key_get_state(KEY_4) == KEY_LONG_PRESS)
    {
        switch (RowCursor)
        {
            case 0:alpha1 -= 0.1;break;
            case 1:alpha2 -= 0.1;break;
            case 2:belta1 -= 0.1;break;
            case 3:belta2 -= 0.1;break;
            case 4:belta3 -= 0.1;break;
        }
        key_clear_state(KEY_4);
    }
}

void Zebra()
{
    oled_show_string(20, 0, "Kp:");
    oled_show_float(80, 0, KpofStop, 2, 1);
    oled_show_string(20, 1, "Ki:");
    oled_show_float(80, 1, KiofStop, 2, 1);
    oled_show_string(20, 2, "ZSpeed:");
    oled_show_float(80, 2, SpeedOfZebra, 3, 1);

    oled_show_string(20, 3, "Rampslow:");
    oled_show_float(80, 3, SpeedOfRamp, 2, 1);
    oled_show_string(20, 4, "Rampfly:");
    oled_show_float(80, 4, SpeedOfRamp_fly, 2, 1);

    oled_show_string(20, 5, "Distance:");
    oled_show_int(80, 5, Zebra_Distance, 3);

    if (key_get_state(KEY_3) == KEY_SHORT_PRESS || key_get_state(KEY_3) == KEY_LONG_PRESS)
    {
        switch (RowCursor)
        {
            case 0:KpofStop += 0.1;break;
            case 1:KiofStop += 0.1;break;
            case 2:SpeedOfZebra += 1;break;
            case 3:SpeedOfRamp += 1;break;
            case 4:SpeedOfRamp_fly += 1;break;
            case 5:Zebra_Distance += 1;break;
        }
        key_clear_state(KEY_3);
    }
    if (key_get_state(KEY_4) == KEY_SHORT_PRESS || key_get_state(KEY_4) == KEY_LONG_PRESS)
    {
        switch (RowCursor)
        {
            case 0:KpofStop -= 0.1;break;
            case 1:KiofStop -= 0.1;break;
            case 2:SpeedOfZebra -= 1;break;
            case 3:SpeedOfRamp -= 1;break;
            case 4:SpeedOfRamp_fly -= 1;break;
            case 5:Zebra_Distance -= 1;break;
        }
        key_clear_state(KEY_4);
    }
}

void DangerANDRescure()
{
    oled_show_string(20, 0, "S_IN:");
    oled_show_float(90, 0, SpeedOfRescueIN, 3, 1);
    oled_show_string(20, 1, "S_OFF:");
    oled_show_float(90, 1, SpeedOfRescueOFF, 3, 1);
    oled_show_string(20, 2, "LdjIN:");
    oled_show_int(90, 2, rescuehandle_lrmain.LeftDaJiaoIN, 4);
    oled_show_string(20, 3, "RdjIN:");
    oled_show_int(90, 3, rescuehandle_lrmain.RightDaJiaoIN, 4);
    oled_show_string(20, 4, "DistanceL:");
    oled_show_float(90, 4, rescuehandle_lrmain.ForwardDistance, 3, 2);
    oled_show_string(20, 5, "DistanceR:");
    oled_show_float(90, 5, rescuehandle_lrmain.ForwardDistance_Right, 3, 2);
    oled_show_string(20, 6, "flag:");
    oled_show_int(90, 6, rescuehandle_lrmain.StopFinishflag, 2);
    oled_show_string(20, 7, "StopFm");
    oled_show_int(90, 7, rescuehandle_lrmain.StopFrame, 4);


    if (key_get_state(KEY_3) == KEY_SHORT_PRESS || key_get_state(KEY_3) == KEY_LONG_PRESS)
    {
        switch (RowCursor)
        {
            case 0:SpeedOfRescueIN += 1;break;
            case 1:SpeedOfRescueOFF += 1;break;
            case 2:rescuehandle_lrmain.LeftDaJiaoIN += 1;break;
            case 3:rescuehandle_lrmain.RightDaJiaoIN += 1;break;
            case 4:rescuehandle_lrmain.ForwardDistance += 0.01;break;
            case 5:rescuehandle_lrmain.ForwardDistance_Right += 0.01;break;
            case 6:rescuehandle_lrmain.StopFinishflag += 1;break;
            case 7:rescuehandle_lrmain.StopFrame +=1;break;

        }
        key_clear_state(KEY_3);
    }
    if (key_get_state(KEY_4) == KEY_SHORT_PRESS || key_get_state(KEY_4) == KEY_LONG_PRESS)
    {
        switch (RowCursor)
        {
            case 0:SpeedOfRescueIN -= 1;break;
            case 1:SpeedOfRescueOFF -= 1;break;
            case 2:rescuehandle_lrmain.LeftDaJiaoIN -= 1;break;
            case 3:rescuehandle_lrmain.RightDaJiaoIN -= 1;break;
            case 4:rescuehandle_lrmain.ForwardDistance -= 0.01;break;
            case 5:rescuehandle_lrmain.ForwardDistance_Right -= 0.01;break;
            case 6:rescuehandle_lrmain.StopFinishflag -= 1;break;
            case 7:rescuehandle_lrmain.StopFrame -=1;break;
        }
        key_clear_state(KEY_4);
    }
}

void Bomp()
{
    oled_show_string(20, 0, "Speed:");
    oled_show_int(90, 0, SpeedOfDanger, 2);
    oled_show_string(20, 1, "DKp:");
    oled_show_float(90, 1, KpofBlocks, 2,1);
    oled_show_string(20, 2, "DKd:");
    oled_show_float(90, 2, KdofBlocks, 2,1);
    oled_show_string(20, 3, "SKp:");
    oled_show_float(90, 3, SpeedKpofBlocks, 3,1);
    oled_show_string(20, 4, "SKi:");
    oled_show_float(90, 4, SpeedKiofBlocks, 2,1);
    if (key_get_state(KEY_3) == KEY_SHORT_PRESS || key_get_state(KEY_3) == KEY_LONG_PRESS)
    {
        switch (RowCursor)
        {
            case 0:SpeedOfDanger += 1;break;
            case 1:KpofBlocks += 0.1;break;
            case 2:KdofBlocks += 0.1;break;
            case 3:SpeedKpofBlocks += 0.1;break;
            case 4:SpeedKiofBlocks += 0.1;break;
        }
        key_clear_state(KEY_3);
    }
    if (key_get_state(KEY_4) == KEY_SHORT_PRESS || key_get_state(KEY_4) == KEY_LONG_PRESS)
    {
        switch (RowCursor)
        {
            case 0:SpeedOfDanger -= 1;break;
            case 1:KpofBlocks -= 0.1;break;
            case 2:KdofBlocks -= 0.1;break;
            case 3:SpeedKpofBlocks -= 0.1;break;
            case 4:SpeedKiofBlocks -= 0.1;break;
        }
        key_clear_state(KEY_4);
    }
}

void Roundabout_Small()
{
    KdofRuHuan_Small = 4*KpofRuHuan_Small;
    oled_show_string(20, 0, "RH_Speed:");
    oled_show_float(90, 0, SpeedOfRuHuan_Small, 3, 1);
    oled_show_string(20, 1, "RH_Kp:");
    oled_show_float(90, 1, KpofRuHuan_Small, 3, 1);

    oled_show_string(20, 2, "HZ_Speed:");
    oled_show_float(90, 2, SpeedOfHuanZhong_Small, 3, 1);
    oled_show_string(20, 3, "HZ_Kp:");
    oled_show_float(90, 3, KpofHuanZhong_Small, 3, 1);
    oled_show_string(20, 4, "HZ_Kd:");
    oled_show_float(90, 4, KdofHuanZhong_Small, 3, 1);

    oled_show_string(20, 5, "CH_Speed:");
    oled_show_float(90, 5, SpeedOfChuHuan_Small, 3, 1);
    oled_show_string(20, 6, "CH_Kp:");
    oled_show_float(90, 6, KpofChuHuan_Small, 3, 1);
    oled_show_string(20, 7, "CH_Kd:");
    oled_show_float(90, 7, KdofChuHuan_Small, 3, 1);

    if (key_get_state(KEY_3) == KEY_SHORT_PRESS || key_get_state(KEY_3) == KEY_LONG_PRESS)
    {
        switch (RowCursor)
        {
            case 0:SpeedOfRuHuan_Small += 1;break;
            case 1:KpofRuHuan_Small += 0.1;break;
            case 2:SpeedOfHuanZhong_Small += 1;break;
            case 3:KpofHuanZhong_Small += 0.1;break;
            case 4:KdofHuanZhong_Small += 0.1;break;
            case 5:SpeedOfChuHuan_Small += 1;break;
            case 6:KpofChuHuan_Small += 0.1;break;
            case 7:KdofChuHuan_Small += 0.1;break;
        }
        key_clear_state(KEY_3);
    }
    if (key_get_state(KEY_4) == KEY_SHORT_PRESS || key_get_state(KEY_4) == KEY_LONG_PRESS)
    {
        switch (RowCursor)
        {
            case 0:SpeedOfRuHuan_Small -= 1;break;
            case 1:KpofRuHuan_Small -= 0.1;break;
            case 2:SpeedOfHuanZhong_Small -= 1;break;
            case 3:KpofHuanZhong_Small -= 0.1;break;
            case 4:KdofHuanZhong_Small -= 0.1;break;
            case 5:SpeedOfChuHuan_Small -= 1;break;
            case 6:KpofChuHuan_Small -= 0.1;break;
            case 7:KdofChuHuan_Small -= 0.1;break;
        }
        key_clear_state(KEY_4);
    }
}

void Roundabout_Big()
{
    KdofRuHuan_Big = 4*KpofRuHuan_Big;
    oled_show_string(20, 0, "RH_Speed:");
    oled_show_float(90, 0, SpeedOfRuHuan_Big, 3, 1);
    oled_show_string(20, 1, "RH_Kp:");
    oled_show_float(90, 1, KpofRuHuan_Big, 3, 1);

    oled_show_string(20, 2, "HZ_Speed:");
    oled_show_float(90, 2, SpeedOfHuanZhong_Big, 3, 1);
    oled_show_string(20, 3, "HZ_Kp:");
    oled_show_float(90, 3, KpofHuanZhong_Big, 3, 1);
    oled_show_string(20, 4, "HZ_Kd:");
    oled_show_float(90, 4, KdofHuanZhong_Big, 3, 1);

    oled_show_string(20, 5, "CH_Speed:");
    oled_show_float(90, 5, SpeedOfChuHuan_Big, 3, 1);
    oled_show_string(20, 6, "CH_Kp:");
    oled_show_float(90, 6, KpofChuHuan_Big, 3, 1);
    oled_show_string(20, 7, "CH_Kd:");
    oled_show_float(90, 7, KdofChuHuan_Big, 3, 1);

    if (key_get_state(KEY_3) == KEY_SHORT_PRESS || key_get_state(KEY_3) == KEY_LONG_PRESS)
    {
        switch (RowCursor)
        {
            case 0:SpeedOfRuHuan_Big += 1;break;
            case 1:KpofRuHuan_Big += 0.1;break;
            case 2:SpeedOfHuanZhong_Big += 1;break;
            case 3:KpofHuanZhong_Big += 0.1;break;
            case 4:KdofHuanZhong_Big += 0.1;break;
            case 5:SpeedOfChuHuan_Big += 1;break;
            case 6:KpofChuHuan_Big += 0.1;break;
            case 7:KdofChuHuan_Big += 0.1;break;
        }
        key_clear_state(KEY_3);
    }
    if (key_get_state(KEY_4) == KEY_SHORT_PRESS || key_get_state(KEY_4) == KEY_LONG_PRESS)
    {
        switch (RowCursor)
        {
            case 0:SpeedOfRuHuan_Big -= 1;break;
            case 1:KpofRuHuan_Big -= 0.1;break;
            case 2:SpeedOfHuanZhong_Big -= 1;break;
            case 3:KpofHuanZhong_Big -= 0.1;break;
            case 4:KdofHuanZhong_Big -= 0.1;break;
            case 5:SpeedOfChuHuan_Big -= 1;break;
            case 6:KpofChuHuan_Big -= 0.1;break;
            case 7:KdofChuHuan_Big -= 0.1;break;
        }
        key_clear_state(KEY_4);
    }
}

uint8 TestMotor_Flag = 0;

void Test_Hardware()
{
    oled_show_string(20, 0, "ServoPWM:");
    oled_show_int(90, 0, ServoDutyfactor, 4);
    Set_Angel(ServoDutyfactor);

    oled_show_string(20, 1, "MotorPWM:");
    oled_show_int(90, 1, PWMOut, 4);

    oled_show_string(20, 2, "TestMFlag:");
    oled_show_uint(90, 2, TestMotor_Flag, 1);

    oled_show_string(20, 3, "Buzz:");
    oled_show_uint(90, 3, gpio_get_level(BUZZ), 1);

    oled_show_string(20, 4, "LED0:");
    oled_show_uint(90, 4, gpio_get_level(LED0p), 1);
    oled_show_string(20, 5, "LED1:");
    oled_show_uint(90, 5, gpio_get_level(LED1p), 1);
    oled_show_string(20, 6, "LED2:");
    oled_show_uint(90, 6, gpio_get_level(LED2p), 1);
    oled_show_string(20, 7, "LED3:");
    oled_show_uint(90, 7, gpio_get_level(LED3p), 1);

    if (key_get_state(KEY_3) == KEY_SHORT_PRESS || key_get_state(KEY_3) == KEY_LONG_PRESS)
    {
        switch (RowCursor)
        {
            case 0:ServoDutyfactor += 1;break;
            case 1:PWMOut += 100;break;
            case 2:TestMotor_Flag = 1;break;
            case 3:Buzz_Ctrl(BUZZ0, ON);break;
            case 4:Led_Ctrl(LED0, ON);break;
            case 5:Led_Ctrl(LED1, ON);break;
            case 6:Led_Ctrl(LED2, ON);break;
            case 7:Led_Ctrl(LED3, ON);break;
        }
        key_clear_state(KEY_3);
    }
    if (key_get_state(KEY_4) == KEY_SHORT_PRESS || key_get_state(KEY_4) == KEY_LONG_PRESS)
    {
        switch (RowCursor)
        {
            case 0:ServoDutyfactor -= 1;break;
            case 1:PWMOut -= 100;break;
            case 2:TestMotor_Flag = 0;break;
            case 3:Buzz_Ctrl(BUZZ0, OFF);break;
            case 4:Led_Ctrl(LED0, OFF);break;
            case 5:Led_Ctrl(LED1, OFF);break;
            case 6:Led_Ctrl(LED2, OFF);break;
            case 7:Led_Ctrl(LED3, OFF);break;
        }
        key_clear_state(KEY_4);
    }
}

void Test_Key()
{
    uint8 key1 = 9, key2 = 9, key3 = 9, key4 = 9, key5 = 9, key6 = 9, key7 = 9;
    key1 = gpio_get_level(P33_4);
    key2 = gpio_get_level(P33_5);
    key3 = gpio_get_level(P33_6);
    key4 = gpio_get_level(P33_7);
    key5 = gpio_get_level(P33_8);
    key6 = gpio_get_level(P13_0);
    key7 = gpio_get_level(P13_1);
    oled_show_uint(0, 0, key1, 1);
    oled_show_uint(0, 1, key2, 1);
    oled_show_uint(0, 2, key3, 1);
    oled_show_uint(0, 3, key4, 1);
    oled_show_uint(0, 4, key5, 1);
    oled_show_uint(0, 5, key6, 1);
    oled_show_uint(0, 6, key7, 1);
    oled_show_string(60, 0, "P33_4");
    oled_show_string(60, 1, "P33_5");
    oled_show_string(60, 2, "P33_6");
    oled_show_string(60, 3, "P33_7");
    oled_show_string(60, 4, "P33_8");
    oled_show_string(60, 5, "P13_0");
    oled_show_string(60, 6, "P13_1");
}

void CarState_Show()
{
    oled_show_string(30, 5, "Err:");
    oled_show_int(60, 5, TrueDirError,2);
    oled_show_string(30, 6, "RoadType:");
    oled_show_int(88, 6, TrackState,1);

    oled_show_string(0, 0, "RelS:");
    oled_show_float(30, 0, CarSpeed, 3, 2);
    oled_show_string(70, 0, "TarS:");
    oled_show_int(110, 0, _Speed, 3);

    oled_show_string(0, 7, "Run:");
    oled_show_uint(24, 7, MotorFlag, 1);
    oled_show_string(70, 2, "DY:");
    oled_show_float(97, 2, BatteryVoltage, 3, 2);
    oled_show_string(0, 2, "ReFrm:");
    oled_show_int(40, 2, Receive_Frame, 5);

    oled_show_string(93, 7, "Ifr:");
    oled_show_uint(118, 7, If_ReceivedInfo, 1);

    oled_show_string(0, 1, "Servo:");
    oled_show_int(40, 1, (int)_ServoDutyfactor, 4);

    oled_show_string(0, 3, "Distance:");
    oled_show_float(80, 3, RoadLength, 3, 1);
}
