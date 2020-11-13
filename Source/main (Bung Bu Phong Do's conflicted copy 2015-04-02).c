#include "main.h"
// Noi khai bao hang so
#define     LED     PORTD
#define     ON      1
#define     OFF     0

#define     INIT_SYSTEM     0
#define     MOVING_DOWN     1
#define     MOVING_UP       2
#define     BOTTOM_POSITION     3
#define     TOP_POSITION     4
#define     STOP_MOVING     5

#define     INIT_SYSTEM     0
#define     XI_NHAN_STOP     4
#define     XI_NHAN_LEFT     120
#define     XI_NHAN_RIGHT    130

// Noi khai bao bien toan cuc
unsigned char arrayMapOfOutput [8] = {0x01,0x02,0x04,0x08,0x10,0x20,0x40,0x80};
unsigned char statusOutput[8] = {0,0,0,0,0,0,0,0};
unsigned char AdLed_12[8] =
{0x81,0xc3,0xe7,0xff,0x7e,0x3c,0x18,0x00};
unsigned char AdLed_34[16] =
{0x01,0x03,0x07,0x0f,0x1f,0x3f,0x7f,0xff,
0xfe,0xfc,0xf8,0xf0,0xe0,0xc0,0x80,0x00,};
unsigned char indexOfAdLed = 0;

unsigned char statusFlag = INIT_SYSTEM;
unsigned char statusLedAds = INIT_SYSTEM;
unsigned char statusChinhThoiGian = INIT_SYSTEM;

// Khai bao cac ham co ban IO
void init_system(void);
void delay_ms(int value);
void OpenOutput(int index);
void CloseOutput(int index);
void TestOutput(void);
void ReverseOutput(int index);
unsigned char isButtonMotorOn();
unsigned char isButtonMotorOff();
void MotorOn();
void MotorOff();
void BaiTap_Motor();
void Test_KeyMatrix();
//Chuong trinh con cho bai tap Flag
unsigned char isButtonUp();
unsigned char isButtonDown();
unsigned char isButtonStop();
unsigned char isBottomSwitch();
unsigned char isTopSwitch();
void FlagMovingDown();
void FlagMovingUp();
void FlagStopMoving();
//Chuong trinh con cho bai tap Xi-nhan
void XinhanLeftOn();
void XinhanLeftOff();
void XinhanRightOn();
void XinhanRightOff();
unsigned char isButtonLeft();
unsigned char isButtonRight();
unsigned char isButtonStopXinhan();
void BaiTap_Xinhan();
unsigned char statusXinhan = INIT_SYSTEM;
unsigned char timeXinhanDelay = 0;


////////////////////////////////////////////////////////////////////
//Hien thuc cac chuong trinh con, ham, module, function duoi cho nay
////////////////////////////////////////////////////////////////////
void main(void)
{
	unsigned int k = 0;
        unsigned char a,b,c;
        char d,e,f;
        a = -12;
        d = -12;
	init_system();
	while (1)
	{
            while (!flag_timer3);
            flag_timer3 = 0;
            // thuc hien cac cong viec duoi day
            scan_key_matrix();
//            BaiTap_Xinhan();
            k = (k+1);
            LcdPrintNumS(1,0,a);
            LcdPrintNumS(1,4,d);
            LcdPrintNumS(1,13,k/20);
            DisplayLcdScreen();
	}
}
// Hien thuc cac module co ban cua chuong trinh
void delay_ms(int value)
{
	int i,j;
	for(i=0;i<value;i++)
		for(j=0;j<238;j++);
}

void init_system(void)
{
        TRISB = 0x00;		//setup PORTB is output
        TRISD = 0x00;
        init_lcd();
        LcdClearS();
//        LED = 0x00;
	init_interrupt();
        delay_ms(1000);
        init_timer0(4695);//dinh thoi 1ms sai so 1%
        init_timer1(9390);//dinh thoi 2ms
	init_timer3(46950);//dinh thoi 10ms
	SetTimer0_ms(2);
        SetTimer1_ms(10);
	SetTimer3_ms(50); //Chu ky thuc hien viec xu ly input,proccess,output
        init_key_matrix();
}

void OpenOutput(int index)
{
	if (index >= 0 && index <= 7)
	{
		LED = LED | arrayMapOfOutput[index];
	}

}

void CloseOutput(int index)
{
	if (index >= 0 && index <= 7)
	{
		LED = LED & ~arrayMapOfOutput[index];
	}
}

void ReverseOutput(int index)
{
    if (statusOutput[index]  == ON)
    {
        CloseOutput(index);
        statusOutput[index] = OFF;
    }
    else
    {
        OpenOutput(index);
        statusOutput[index] = ON;
    }
}

void TestOutput(void)
{
	int k;
	for (k=0;k<=7 ;k++ )
	{
		OpenOutput(k);
		delay_ms(500);
		CloseOutput(k);
		delay_ms(500);
	}
}
unsigned char isButtonMotorOn()
{
    if (key_code[2] == 1)
        return 1;
    else
        return 0;
}

unsigned char isButtonMotorOff()
{
    if (key_code[3] == 1)
        return 1;
    else
        return 0;
}

void MotorOn()
{
    OpenOutput(7);
}

void MotorOff()
{
    CloseOutput(7);
}

void BaiTap_Motor()
{
    if (isButtonMotorOn())
        MotorOn();
    if (isButtonMotorOff())
        MotorOff();
}

void Test_KeyMatrix()
{
    int temp_i;
    for (temp_i=0; temp_i<=15; temp_i++)
    {
        if(key_code[temp_i] != 0)
            LED = temp_i;
    }
}
//Noi hien thuc cac chuong trinh con cua bai tap Xinhan
void XinhanLeftOn()
{
    timeXinhanDelay = (timeXinhanDelay + 1) % 20;
    if (timeXinhanDelay < 10)
        OpenOutput(6);
    else
        CloseOutput(6);
}
void XinhanLeftOff()
{
    CloseOutput(6);
}
void XinhanRightOn()
{
    timeXinhanDelay = (timeXinhanDelay + 1) % 20;
    if (timeXinhanDelay < 15)
        OpenOutput(7);
    else
        CloseOutput(7);
}
void XinhanRightOff()
{
    CloseOutput(7);
}
unsigned char isButtonLeft()
{
    if (key_code[8] == 1)
        return 1;
    else
        return 0;
}
unsigned char isButtonRight()
{
    if (key_code[10] == 1)
        return 1;
    else
        return 0;
}
unsigned char isButtonStopXinhan()
{
    if (key_code[9] == 1)
        return 1;
    else
        return 0;
}

void BaiTap_Xinhan()
{
    switch (statusXinhan)
    {
        case INIT_SYSTEM:
            statusXinhan = XI_NHAN_STOP;
            break;
        case XI_NHAN_STOP:
            LcdPrintStringS(0,0,"XI_NHAN_STOP ");
            XinhanLeftOff();
            XinhanRightOff();
            if (isButtonLeft())
                statusXinhan = XI_NHAN_LEFT;
            if (isButtonRight())
                statusXinhan = XI_NHAN_RIGHT;
            break;
        case XI_NHAN_LEFT:
            LcdPrintStringS(0,0,"XI_NHAN_LEFT ");
            XinhanLeftOn();
            XinhanRightOff();
            if (isButtonStopXinhan())
                statusXinhan = XI_NHAN_STOP;
            if (isButtonRight())
                statusXinhan = XI_NHAN_RIGHT;
            break;
        case XI_NHAN_RIGHT:
            LcdPrintStringS(0,0,"XI_NHAN_RIGHT");
            XinhanRightOn();
            XinhanLeftOff();
            if (isButtonStopXinhan())
                statusXinhan = XI_NHAN_STOP;
            if (isButtonLeft())
                statusXinhan = XI_NHAN_LEFT;
            break;
        default:
            statusXinhan = INIT_SYSTEM;
            break;

    }
}
