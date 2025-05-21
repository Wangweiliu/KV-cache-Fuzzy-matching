#ifndef CODE_MENU_H_
#define CODE_MENU_H_

#include "zf_common_headfile.h"

typedef struct MenuTree* MT;
struct MenuTree
{
     uint8 Level;
     void(*PValue)(void);
     MT Father;
     MT FirstChild;
     MT NextSibling;
};
extern MT Root[3];//¸ù½Úµã
extern MT FirstLevelMenu[24];
extern MT Iterator;
extern uint8 IfOpenWirelessUart;

extern uint8 TestMotor_Flag;

void Change_Level(void);
void Change_Page(void);
void RunorStopWhilePressing(void);
void Move_Cursor(void);
void MainFunc(void);
void Menu_Init(void);

void MainMenu1(void);
void MainMenu2(void);
void MainMenu3(void);

void PID(void);
void InitParameter(void);
void SpeedStrategy(void);
void FuzzyPID_DF(void);
void FuzzyPID_EF(void);
void Fuzzy_Kp(void);
void Fuzzy_Kd(void);
void Kalman(void);
void Test_Hardware(void);
void Test_Key(void);
void ADRC_TD_ESO(void);
void ADRC_NLSEF(void);
void Zebra(void);
void DangerANDRescure(void);
void Bomp(void);
void Roundabout_Big(void);
void Roundabout_Small(void);
void Elem_Lists(void);
void Anchor_Adj(void);
void SLow_Down(void);
void CarState_Show(void);

#endif
