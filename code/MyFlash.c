#include "MyFlash.h"

void WriteDatatoFlash()
{
    if(key_get_state(KEY_7) == KEY_LONG_PRESS)
    {
        flash_erase_page(0, 11);
        flash_buffer_clear();
        flash_union_buffer[0].uint32_type = ServoMid;
        flash_union_buffer[1].uint32_type = ServoMin;
        flash_union_buffer[2].uint32_type = ServoMax;
        flash_write_page_from_buffer(0, 11);

        flash_erase_page(0, 10);
        flash_buffer_clear();
        flash_union_buffer[0].float_type = SpeedOfBaseHandle;
        flash_union_buffer[1].float_type = SpeedOfBaseHandle_speedup;
        flash_union_buffer[2].float_type = SpeedOfBaseHandle_wan;
        flash_union_buffer[3].float_type = Speed_min;
        flash_union_buffer[4].float_type = SpeedOfHuanZhong_Small;
        flash_union_buffer[5].float_type = SpeedOfChuHuan_Small;
        flash_union_buffer[6].float_type = SpeedOfRuHuan_Small;
        flash_union_buffer[7].float_type = SpeedOfHuanZhong_Big;
        flash_union_buffer[8].float_type = SpeedOfChuHuan_Big;
        flash_union_buffer[9].float_type = SpeedOfRuHuan_Big;
        flash_union_buffer[10].float_type = SpeedOfRescueIN;
        flash_union_buffer[11].float_type = SpeedOfRescueOFF;
        flash_union_buffer[12].float_type = SpeedOfDanger;
        flash_union_buffer[13].float_type = SpeedOfSlowDown_Big;
        flash_union_buffer[14].float_type = SpeedOfSlowDown;
        flash_union_buffer[15].float_type = SpeedOfBigRCurve;
        flash_union_buffer[16].float_type = SpeedOfRamp;
        flash_union_buffer[17].float_type = SpeedOfRamp_fly;
        flash_union_buffer[18].float_type = SpeedOfZebra;
        flash_write_page_from_buffer(0, 10);

        flash_erase_page(0, 9);
        flash_buffer_clear();
        flash_union_buffer[0].float_type = KpofRuHuan_Small;
        flash_union_buffer[1].float_type = KdofRuHuan_Small;
        flash_union_buffer[2].float_type = KpofHuanZhong_Small;
        flash_union_buffer[3].float_type = KdofHuanZhong_Small;
        flash_union_buffer[4].float_type = KpofChuHuan_Small;
        flash_union_buffer[5].float_type = KdofChuHuan_Small;
        flash_union_buffer[6].float_type = KpofRuHuan_Big;
        flash_union_buffer[7].float_type = KdofRuHuan_Big;
        flash_union_buffer[8].float_type = KpofHuanZhong_Big;
        flash_union_buffer[9].float_type = KdofHuanZhong_Big;
        flash_union_buffer[10].float_type = KpofChuHuan_Big;
        flash_union_buffer[11].float_type = KdofChuHuan_Big;
        flash_union_buffer[12].float_type = KpofBlocks;
        flash_union_buffer[13].float_type= KdofBlocks;
        flash_union_buffer[14].float_type = SteerKp;
        flash_union_buffer[15].float_type = SteerKd;
        flash_union_buffer[16].float_type = KpofStop;
        flash_union_buffer[17].float_type = KiofStop;
        flash_union_buffer[18].float_type = SpeedKpofBlocks;
        flash_union_buffer[19].float_type = SpeedKiofBlocks;
        flash_union_buffer[20].float_type = KiofCommon;
        flash_union_buffer[21].float_type = KpofCommon;
        flash_union_buffer[22].float_type = KdofCommon;
        flash_union_buffer[23].float_type = SLp;
        flash_union_buffer[24].float_type = KLp;
        flash_write_page_from_buffer(0, 9);

        flash_erase_page(0, 8);
        flash_buffer_clear();
        flash_union_buffer[0].uint32_type = ElemLists_Array[0];
        flash_union_buffer[1].uint32_type = ElemLists_Array[1];
        flash_union_buffer[2].uint32_type = ElemLists_Array[2];
        flash_union_buffer[3].uint32_type = ElemLists_Array[3];
        flash_union_buffer[4].uint32_type = ElemLists_Array[4];
        flash_union_buffer[5].uint32_type = ElemLists_Array[5];
        flash_union_buffer[6].uint32_type = ElemLists_Array[6];
        flash_union_buffer[7].uint32_type = ElemLists_Array[7];
        flash_union_buffer[8].uint32_type = ElemLists_Array[8];
        flash_write_page_from_buffer(0, 8);
    }
}

void Flash_Init()
{
//    if(flash_check(0,11) == 1)
//    {
//        flash_read_page_to_buffer(0, 11);
//        ServoMid = read_buf;
//        read_buf = flash_read_page(0,0, uint16);
//        ServoMax = read_buf;
//        read_buf = flash_read_page(0,0, uint16);
//        ServoMin = read_buf;
//    }
//    if(flash_check(0,10) == 1)
//    {
//        read_buf = flash_read_page(0,0, uint16);
//        ServoMid = read_buf;
//        read_buf = flash_read_page(0,0, uint16);
//        ServoMax = read_buf;
//        read_buf = flash_read_page(0,0, uint16);
//        ServoMin = read_buf;
//    }
//    if(flash_check(0,9) == 1)
//    {
//        read_buf = flash_read_page(0,0, uint16);
//        ServoMid = read_buf;
//        read_buf = flash_read_page(0,0, uint16);
//        ServoMax = read_buf;
//        read_buf = flash_read_page(0,0, uint16);
//        ServoMin = read_buf;
//    }
}
