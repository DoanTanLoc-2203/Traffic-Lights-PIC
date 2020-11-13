#include "main.h"

#define     LED     PORTA
#define     ON      1
#define     OFF     0
#define PORTA_OUT   PORTA
#define TRISA_OUT   TRISA
#define     DEN_GT1         255
#define     DEN_GT2         254
#define     GT2ON           253
#define     GT2OFF          252
#define     CHINH_TG        251
#define     NHAP_MK         250
#define     KIEM_TRA        249
#define     DUNG            248
#define     SAI             247
#define     SET_GREEN       246
#define     SET_YELLOW      245
#define     MODEMK          244
#define     NHAPMKCAP1      243
#define     KIEMTRAMKCAP1   242
#define     SAICAP1         241
#define     NHAPMKCAP2      240
#define     KIEMTRAMKCAP2   239
#define     DOIMK           238
#define     DATHAYDOI       237
#define     R1G2            5
#define     R2G1            6
#define     Y1R2            7
#define     Y2R1            8
#define     MODE            4
const int   RED1 =  0;
const int     GREEN1 = 1;
const int     YELLOW1= 2;
const int     RED2 =  3;
const int     GREEN2 = 4;
const int     YELLOW2= 5;
unsigned char statusOfLight = DEN_GT1;
unsigned long timeDelay = 0;
unsigned long Yellowtime=3;
unsigned long Greentime=10;
unsigned long Redtime;
unsigned long  RedtimeDown=0;
unsigned long GreentimeDown=0;
unsigned long YellowtimeDown=0;
unsigned long Timeset=0;
unsigned long segmentime=0;
unsigned long segmentime2=0;
unsigned char checkchinhgio=0;
unsigned char arrayMapOfOutput [8] = {0x01,0x02,0x04,0x08,0x10,0x20,0x40,0x80};
unsigned char statusOutput[8] = {0,0,0,0,0,0,0,0};
unsigned char arrayMapOfNumber [16] = {1,2,3,'A',4,5,6,'B',
                                       7,8,9,'C','*',0,'E','D'};
unsigned char arrayMapOfPassword [1][4]={1,2,3,4};
unsigned char arrayMapOfPassword2 [1][4]={1,1,1,1};
unsigned char arrayPassword[4];
unsigned char indexOfNumber = 0;
unsigned char numberValue;
int num;
void init_output(void);
void init_system(void);
void delay_ms(int value);
void OpenOutput(int index);
void CloseOutput(int index);
void TestOutput(void);
void ReverseOutput(int index);
void StageR1G2();
void StageY2R1();
void StageR2G1();
void StageY1R2();
void YELLOWON();
void YELLOWOFF();
void OFFALL();
unsigned char CheckPassword();
unsigned char CheckPassword2();
unsigned char isButtonNumber();
void innum();
//Bai tap chong rung nut nhan
char numberOfPushButton = 0;
unsigned char isButtonUp();
unsigned char isButtonDown();
unsigned char isButtonEnter();
void AppTrafficLight();
void Test_KeyMatrix();
void Led7segmentA();
void Led7segmentB();
void main(void)
{
	init_system();
    LcdClearS();
    delay_ms(2000);
    
	while (1)
	{
        while (!flag_timer3);
        flag_timer3 = 0;
        if(checkchinhgio==1)
        {
            scan_key_button();
        }                
        scan_key_matrix();
        AppTrafficLight();
        DisplayLcdScreen();
	}
}

void delay_ms(int value)
{
	int i,j;
	for(i = 0; i < value; i++)
		for(j = 0; j < 160; j++);
}
void init_output(void)
{
    TRISA_OUT = 0x00;
    PORTA_OUT = 0x00;
}
void init_system(void)
{
    init_output();
    TRISB = 0x00;		//setup PORTB is output
    init_lcd();
    LcdClearS();
    init_interrupt();
    delay_ms(1000);
    init_timer0(4695);  //dinh thoi 1ms
    init_timer1(9390);  //dinh thoi 2ms
    init_timer3(46950); //dinh thoi 10ms
    SetTimer0_ms(2);
    SetTimer1_ms(10);
    SetTimer3_ms(60);   //Chu ky thuc hien viec xu ly input,proccess,output 50->60
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
	int i;
	for (i = 0; i <= 7 ; i++ )
	{
		OpenOutput(i);
		delay_ms(500);
		CloseOutput(i);
		delay_ms(500);
	}
}

unsigned char isButtonUp()
{
    if (key_code[0] == 1 || (key_code[0] >= 20 && key_code[0]%2 == 1))
        return 1;
    else
        return 0;
}
unsigned char isButtonDown()
{
    if (key_code[1] == 1)
        return 1;
    else
        return 0;
}
unsigned char isButton1()
{
    if (key_code[0] == 1)
        return 1;
    else
        return 0;
}
unsigned char isButton2()
{
    if (key_code[1] == 1)
        return 1;
    else
        return 0;
}
unsigned char isButtonEnter()
{
    if (key_code[11] == 1)
        return 1;
    else
        return 0;
}
void AppTrafficLight()
{
    switch (statusOfLight)
    {
        case DEN_GT1:
            LcdClearS();
            Redtime=Yellowtime+Greentime+1;
            RedtimeDown=0;
            GreentimeDown=0;
            YellowtimeDown=0;
            LcdPrintNumS(0,2,Redtime-RedtimeDown);
            LcdPrintStringS(1,0,"STAGE: MODE 1");
            LcdPrintCharS(0,0,'R');
            LcdPrintNumS(0,10,Greentime-GreentimeDown);
            LcdPrintCharS(0,8,'G');
            statusOfLight=R1G2;
            break;
        case R1G2:
            StageR1G2();
            timeDelay++; 
            segmentime=Redtime-RedtimeDown;
            Led7segmentA(segmentime);
            segmentime2=Greentime-GreentimeDown;
            Led7segmentB(segmentime2);
            if(GreentimeDown>Greentime)
            {
                LcdClearS();
                LcdPrintStringS(1,0,"STAGE: MODE 1");
                LcdPrintNumS(0,2,Redtime-RedtimeDown);
                LcdPrintCharS(0,0,'R');
                LcdPrintNumS(0,10,Yellowtime-YellowtimeDown);
                LcdPrintCharS(0,8,'Y');
                timeDelay=0;
                GreentimeDown=0;
                statusOfLight=Y2R1;
            }
            if(timeDelay>25)
            {
                LcdClearS();
                LcdPrintStringS(1,0,"STAGE: MODE 1");
                LcdPrintNumS(0,2,Redtime-RedtimeDown-1);
                LcdPrintCharS(0,0,'R');
                LcdPrintNumS(0,10,Greentime-GreentimeDown-1);
                LcdPrintCharS(0,8,'G');
                timeDelay=0;
                RedtimeDown++;
                GreentimeDown++;
            } 
            if(isButton1())
            {
                statusOfLight=DEN_GT2;
            }
            if(isButtonEnter())
            {
                statusOfLight=CHINH_TG;
            }
            break;
        case Y2R1:
            StageY2R1();
            timeDelay++;
            segmentime=Redtime-RedtimeDown;
            Led7segmentA(segmentime);
            segmentime2=Yellowtime-YellowtimeDown;
            Led7segmentB(segmentime2);
            if(YellowtimeDown>Yellowtime)
            {
                LcdClearS();
                timeDelay=0;
                RedtimeDown=0;
                YellowtimeDown=0;
                LcdPrintStringS(1,0,"STAGE: MODE 1");
                LcdPrintNumS(0,2,Greentime-GreentimeDown);
                LcdPrintCharS(0,0,'G');
                LcdPrintNumS(0,10,Redtime-RedtimeDown);
                LcdPrintCharS(0,8,'R');
                statusOfLight=R2G1;
            }
            if(timeDelay>25)
            {
                LcdClearS();
                timeDelay=0;
                LcdPrintStringS(1,0,"STAGE: MODE 1");
                LcdPrintNumS(0,2,Redtime-RedtimeDown-1);
                LcdPrintCharS(0,0,'R');
                LcdPrintNumS(0,10,Yellowtime-YellowtimeDown-1);
                LcdPrintCharS(0,8,'Y');
                RedtimeDown++;
                YellowtimeDown++;
            }
            if(isButton1())
            {
                statusOfLight=DEN_GT2;
            }
            if(isButtonEnter())
            {
                statusOfLight=CHINH_TG;
            }
            break;
        case R2G1:
            StageR2G1();
            timeDelay++;
            segmentime=Greentime-GreentimeDown;
            Led7segmentA(segmentime);
            segmentime2=Redtime-RedtimeDown;
            Led7segmentB(segmentime2);
            if(GreentimeDown>Greentime)
            {
                LcdClearS();
                LcdPrintStringS(1,0,"STAGE: MODE 1");
                LcdPrintNumS(0,2,Yellowtime-YellowtimeDown);
                LcdPrintCharS(0,0,'Y');
                LcdPrintNumS(0,10,Redtime-RedtimeDown);
               LcdPrintCharS(0,8,'R');
                timeDelay=0;
                GreentimeDown=0;
                statusOfLight=Y1R2;
            }
            if(timeDelay>25)
            {
                LcdClearS();
                LcdPrintStringS(1,0,"STAGE: MODE 1");
                LcdPrintNumS(0,2,Greentime-GreentimeDown-1);
                LcdPrintCharS(0,0,'G');
                LcdPrintNumS(0,10,Redtime-RedtimeDown-1);
                LcdPrintCharS(0,8,'R');
                timeDelay=0;
                RedtimeDown++;
                GreentimeDown++;
            }
            if(isButton1())
            {
                statusOfLight=DEN_GT2;
            }
            if(isButtonEnter())
            {
                statusOfLight=CHINH_TG;
            }
            break;
        case Y1R2:
            StageY1R2();
            segmentime=Yellowtime-YellowtimeDown;
            Led7segmentA(segmentime);
            segmentime2=Redtime-RedtimeDown;
            Led7segmentB(segmentime2);
            timeDelay++;
            LcdPrintStringS(1,0,"STAGE: MODE 1");
            if(YellowtimeDown>Yellowtime)
            {
                LcdClearS();
                timeDelay=0;
                RedtimeDown=0;
                YellowtimeDown=0;
                statusOfLight=DEN_GT1;
            }
            if(timeDelay>25)
            {
                LcdClearS();
                timeDelay=0;
                LcdPrintStringS(1,0,"STAGE: MODE 1");
                LcdPrintNumS(0,2,Yellowtime-YellowtimeDown-1);
                LcdPrintCharS(0,0,'Y');
                LcdPrintNumS(0,10,Redtime-RedtimeDown-1);
                LcdPrintCharS(0,8,'R');
                RedtimeDown++;
                YellowtimeDown++;
            }
            if(isButton1())
            {
                statusOfLight=DEN_GT2;
            }
            if(isButtonEnter())
            {
                statusOfLight=CHINH_TG;
            }
            break;
        case DEN_GT2:
            LcdClearS();
            timeDelay=0;
            statusOfLight=GT2ON;
            break;
        case GT2ON:
            timeDelay++;
            LcdPrintStringS(1,0,"STAGE: MODE 2");
            LcdPrintStringS(0,0,"PRESS 1-->MODE 1");
            YELLOWON();
            if(timeDelay>25)
            {
              timeDelay=0;
              statusOfLight=GT2OFF;
            }
            if(isButton1())
            {
                statusOfLight=DEN_GT1;
            }
            if(isButtonEnter())
            {
                statusOfLight=CHINH_TG;
            }
            break;
        case GT2OFF:
            timeDelay++;
            LcdPrintStringS(1,0,"STAGE: MODE 2");
            LcdPrintStringS(0,0,"PRESS 1-->MODE 1");
            YELLOWOFF();
            if(timeDelay>25)
            {
              timeDelay=0;
              statusOfLight=GT2ON;
            }
            if(isButton1())
            {
                statusOfLight=DEN_GT1;
            }
            if(isButtonEnter())
            {
                statusOfLight=CHINH_TG;
            }
            break;
        case CHINH_TG:
            LcdClearS();
            statusOfLight=MODEMK;
            OFFALL();
            break;
        case MODEMK:
            LcdClearS();
            LcdPrintStringS(0,0,"PRESS 1:ENTER!");
            LcdPrintStringS(1,0,"PRESS 2:CHANGE!");
            timeDelay ++;
            if(timeDelay>=100)
            {
                timeDelay=0;
                statusOfLight=DEN_GT1;
            }
            if(isButton1())
            {
                LcdClearS();
                statusOfLight=NHAP_MK;
            }
            if(isButton2())
            {
                LcdClearS();
                statusOfLight=NHAPMKCAP1;
            }
            break;
        case NHAPMKCAP1:
            LcdPrintStringS(0,0,"PASSWORD LV1!");
            timeDelay ++;
            if(timeDelay>=150)
            {
                timeDelay=0;
                statusOfLight=MODEMK;
            }
            if (isButtonNumber())
            {
                LcdPrintStringS(1,indexOfNumber,"*");
                arrayPassword [indexOfNumber] = numberValue;
                indexOfNumber ++;
                timeDelay = 0;
            }
            if(indexOfNumber>4)
            {
                indexOfNumber=0;
                timeDelay = 0;
                LcdClearS();
                statusOfLight=KIEMTRAMKCAP1;
            }
            break;
        case KIEMTRAMKCAP1:
            if (CheckPassword())
               statusOfLight=NHAPMKCAP2;
            else
               statusOfLight=SAICAP1;
            break;
        case SAICAP1:
            LcdPrintStringS(0,0,"WRONG PASSWORD!");
            timeDelay++;
            if(timeDelay>=50)
            {
                timeDelay=0;
                LcdClearS();
                statusOfLight=MODEMK;
            }
            break;
        case NHAPMKCAP2:
            LcdPrintStringS(0,0,"PASSWORD LV2!");
            timeDelay ++;
            if(timeDelay>=150)
            {
                timeDelay=0;
                statusOfLight=MODEMK;
            }
            if (isButtonNumber())
            {
                LcdPrintStringS(1,indexOfNumber,"*");
                arrayPassword [indexOfNumber] = numberValue;
                indexOfNumber ++;
                timeDelay = 0;
            }
            if(indexOfNumber>4)
            {
                indexOfNumber=0;
                timeDelay = 0;
                LcdClearS();
                statusOfLight=KIEMTRAMKCAP2;
            }
            break;
        case KIEMTRAMKCAP2:
            if (CheckPassword2())
               statusOfLight=DOIMK;
            else
               statusOfLight=SAICAP1;
            break;
        case DOIMK:
            LcdPrintStringS(0,0,"CHANGE PASSWORD!");
            timeDelay ++;
            if(timeDelay>=150)
            {
                timeDelay=0;
                statusOfLight=MODEMK;
            }
            if (isButtonNumber())
            {
               // LcdPrintStringS(1,indexOfNumber,"*");
                innum();
                arrayPassword [indexOfNumber] = numberValue;
                indexOfNumber ++;
                timeDelay = 0;
            }
            if(indexOfNumber>4)
            {
                indexOfNumber=0;
                timeDelay = 0;
                LcdClearS();
                statusOfLight=DATHAYDOI;
            }
            break;
        case DATHAYDOI:
            LcdPrintStringS(0,0,"SUCCESSFUL!");
            timeDelay++;
            if(timeDelay>=50)
            {
                int i;
                for(i =0 ;i<4;i++)
                {
                    arrayMapOfPassword[1][i]=arrayPassword[i];
                }
                LcdClearS();
                timeDelay=0;
                statusOfLight=MODEMK;
            }
            break;
        case NHAP_MK:
            LcdPrintStringS(0,0,"ENTER PASSWORD!");
            timeDelay ++;
            if(timeDelay>=100)
            {
                timeDelay=0;
                statusOfLight=DEN_GT1;
            }
            if (isButtonNumber())
            {
                LcdPrintStringS(1,indexOfNumber,"*");
                arrayPassword [indexOfNumber] = numberValue;
                indexOfNumber ++;
                timeDelay = 0;
            }
            if(indexOfNumber>=4)
            {
                indexOfNumber=0;
                timeDelay = 0;
                statusOfLight=KIEM_TRA;
            }
            break;
        case KIEM_TRA:
            if (CheckPassword())
               statusOfLight=DUNG ;
            else
                statusOfLight=SAI;
            break;
        case DUNG:
            statusOfLight=SET_GREEN;
            break;
        case SAI:
            LcdClearS();
            LcdPrintStringS(0,0,"WRONG PASSWORD!!");
            timeDelay++;
            if(timeDelay>50)
            {
                timeDelay=0;
                statusOfLight=NHAP_MK;
            }
            break;
        case SET_GREEN:
            LcdClearS();
            LcdPrintStringS(0,0,"TIME GREEN");
            LcdPrintNumS(1,0,Timeset);
            checkchinhgio=1;
            if(isButton1()&& Timeset>0)
            {
                Timeset--;
            }
            if(isButton2())
            {
                Timeset++;
            }
            if(isButtonEnter())
            {
                Greentime=Timeset;
                Timeset=0;
                statusOfLight=SET_YELLOW;
            }
            break;
        case SET_YELLOW:
            LcdClearS();
            LcdPrintStringS(0,0,"TIME YELLOW");
            LcdPrintNumS(1,0,Timeset);
            if(isButton1()&& Timeset>0)
            {
                Timeset--;
            }
            if(isButton2())
            {
                Timeset++;
            }
            if(isButtonEnter())
            {
                Yellowtime=Timeset;
                Timeset=0;
                checkchinhgio=0;
                statusOfLight=DEN_GT1;
            }
            break;
        default:
            break;
    }
}
void Led7segmentA(long num){
    int a=num%10;
    int b=(num-a)/10;
    int chuc,donvi;
    if(b==0)
    {
        chuc=0x00;
    }
    else if(b==1)
    {
        chuc=0x10;
    }
    else if(b==2)
    {
        chuc=0x20;
    }
    else if(b==3)
    {
        chuc=0x30;
    }
    else if(b==4)
    {
        chuc=0x40;
    }
    else if(b==5)
    {
       chuc=0x50;
    }
    else if(b==6)
    {
        chuc=0x60;
    }
    else if(b==7)
    {
        chuc=0x70;
    }
    else if(b==8)
    {
        chuc=0x80;
    }
    else if(b==9)
    {
        chuc=0x90;
    }//////////
    if(a==0)
    {
        donvi=0x00;
    }
    else if(a==1)
    {
        donvi=0x01;
    }
    else if(a==2)
    {
        donvi=0x02;
    }
    else if(a==3)
    {
        donvi=0x03;
    }
    else if(a==4)
    {
        donvi=0x04;
    }
    else if(a==5)
    {
       donvi=0x05;
    }
    else if(a==6)
    {
        donvi=0x06;
    }
    else if(a==7)
    {
        donvi=0x07;
    }
    else if(a==8)
    {
        donvi=0x08;
    }
    else if(a==9)
    {
        donvi=0x09;
    }
    
    TRISC=0x00;
    PORTC=chuc+donvi;
}
void Led7segmentB(long num){
    int a=num%10;
    int b=(num-a)/10;
    int chuc,donvi;
    if(b==0)
    {
        chuc=0x00;
    }
    else if(b==1)
    {
        chuc=0x10;
    }
    else if(b==2)
    {
        chuc=0x20;
    }
    else if(b==3)
    {
        chuc=0x30;
    }
    else if(b==4)
    {
        chuc=0x40;
    }
    else if(b==5)
    {
       chuc=0x50;
    }
    else if(b==6)
    {
        chuc=0x60;
    }
    else if(b==7)
    {
        chuc=0x70;
    }
    else if(b==8)
    {
        chuc=0x80;
    }
    else if(b==9)
    {
        chuc=0x90;
    }//////////
    if(a==0)
    {
        donvi=0x00;
    }
    else if(a==1)
    {
        donvi=0x01;
    }
    else if(a==2)
    {
        donvi=0x02;
    }
    else if(a==3)
    {
        donvi=0x03;
    }
    else if(a==4)
    {
        donvi=0x04;
    }
    else if(a==5)
    {
       donvi=0x05;
    }
    else if(a==6)
    {
        donvi=0x06;
    }
    else if(a==7)
    {
        donvi=0x07;
    }
    else if(a==8)
    {
        donvi=0x08;
    }
    else if(a==9)
    {
        donvi=0x09;
    }
    TRISD=0x00;
    PORTD=chuc+donvi;
}
void OFFALL()
{
   /* CloseOutput(YELLOW1);
    CloseOutput(RED1);
    CloseOutput(GREEN1);
    CloseOutput(YELLOW2);
    CloseOutput(RED2);
    CloseOutput(GREEN2);
    CloseOutput(6);
    CloseOutput(7);*/
    LED=0b00000000;
}
void YELLOWON()
{
    /*OpenOutput(YELLOW1);
    CloseOutput(RED1);
    CloseOutput(GREEN1);
    OpenOutput(YELLOW2);
    CloseOutput(RED2);
    CloseOutput(GREEN2);
        CloseOutput(6);
    CloseOutput(7);*/
    LED=0b00100100;
}
void YELLOWOFF()
{
    /*CloseOutput(YELLOW1);
    CloseOutput(RED1);
    CloseOutput(GREEN1);
    CloseOutput(YELLOW2);
    CloseOutput(RED2);
    CloseOutput(GREEN2);
        CloseOutput(6);
    CloseOutput(7);*/
    LED=0b00000000;
}
void StageR1G2()
{
    /*OpenOutput(RED1);
    CloseOutput(GREEN1);
    CloseOutput(YELLOW1);
    CloseOutput(RED2);
    OpenOutput(GREEN2);
    CloseOutput(YELLOW2);
        CloseOutput(6);
    CloseOutput(7);*/
    LED=0b00010001;
}
void StageY2R1()
{
    /*OpenOutput(RED1);
    CloseOutput(GREEN1);
    CloseOutput(YELLOW1);
    CloseOutput(RED2);
    CloseOutput(GREEN2);
    OpenOutput(YELLOW2);
    CloseOutput(6);
    CloseOutput(7);*/
    LED=0b00100001;
}
void StageR2G1()
{
    /*CloseOutput(RED1);
     OpenOutput(GREEN1);
    CloseOutput(YELLOW1);
    OpenOutput(RED2);
    CloseOutput(GREEN2);
    CloseOutput(YELLOW2);
    CloseOutput(6);
    CloseOutput(7);*/
    LED=0b00001010;
}
void StageY1R2()
{
    /*CloseOutput(RED1);
    CloseOutput(GREEN1);
    OpenOutput(YELLOW1);
    OpenOutput(RED2);
    CloseOutput(GREEN2);
    CloseOutput(YELLOW2);
        CloseOutput(6);
    CloseOutput(7);*/
    LED=0b00001100;
}
unsigned char CheckPassword()
{
    unsigned char i,j;
    unsigned result = 1;
    for (i=0;i<5;i++)
    {
        result = 1;
        for (j=0;j<4;j++)
        {
            if (arrayPassword[j] != arrayMapOfPassword[i][j])
                result = 0;
        }
        if (result == 1)
            return (i+1);
    }

}
unsigned char CheckPassword2()
{
    unsigned char i,j;
    unsigned result = 1;
    for (i=0;i<5;i++)
    {
        result = 1;
        for (j=0;j<4;j++)
        {
            if (arrayPassword[j] != arrayMapOfPassword2[i][j])
                result = 0;
        }
        if (result == 1)
            return (i+1);
    }

}
unsigned char isButtonNumber()
{
    unsigned char i;
    for (i = 0; i<=15; i++)
        if (key_code[i] == 1)
        {
            numberValue = arrayMapOfNumber[i];
            num=i;
            return 1;
        }
    return 0;
}
void innum()
{
    if(num==0)
    {
        LcdPrintStringS(1,indexOfNumber,"1");  
    }
    if(num==1)
    {
        LcdPrintStringS(1,indexOfNumber,"2");  
    }
    if(num==2)
    {
        LcdPrintStringS(1,indexOfNumber,"2"); 
    }
    if(num==3)
    {
        LcdPrintStringS(1,indexOfNumber,"A");  
    }
    if(num==4)
    {
        LcdPrintStringS(1,indexOfNumber,"4");  
    }
    if(num==5)
    {
        LcdPrintStringS(1,indexOfNumber,"5");  
    }
    if(num==6)
    {
        LcdPrintStringS(1,indexOfNumber,"6");  
    }
    if(num==7)
    {
        LcdPrintStringS(1,indexOfNumber,"B");  
    }
    if(num==8)
    {
        LcdPrintStringS(1,indexOfNumber,"7");  
    }
    if(num==9)
    {
        LcdPrintStringS(1,indexOfNumber,"8");  
    }
    if(num==10)
    {
        LcdPrintStringS(1,indexOfNumber,"9");  
    }
    if(num==11)
    {
        LcdPrintStringS(1,indexOfNumber,"C");  
    }
    if(num==12)
    {
        LcdPrintStringS(1,indexOfNumber,"*");  
    }
    if(num==13)
    {
        LcdPrintStringS(1,indexOfNumber,"0");  
    }
    if(num==14)
    {
        LcdPrintStringS(1,indexOfNumber,"#");  
    }
    if(num==15)
    {
        LcdPrintStringS(1,indexOfNumber,"D");  
    }
}