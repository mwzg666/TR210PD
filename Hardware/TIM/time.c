#include "time.h"
#include "uart.h"
#include "led.h"
#include "Cmd.h"

u16  Timer0Cnt = 0;
u16  Timer1Cnt = 0;
u16  Time1s = 0;

void Timer0_Init(void)        //10毫秒@11.0592MHz
{

    AUXR = 0x00;    //Timer0 set as 12T, 16 bits timer auto-reload, 
    TH0 = (u8)(Timer0_Reload / 256);
    TL0 = (u8)(Timer0_Reload % 256);
    ET0 = 1;    //Timer0 interrupt enable
    TR0 = 1;    //Tiner0 run
    
    // 中断优先级3
    PT0  = 1;
    PT0H = 1;

}

// 10ms 中断一下
void Timer0ISR (void) interrupt 1
{
    Timer0Cnt ++;
}


//void delay_us(BYTE us)
//{
//    while(us--)
//    {
//        ;
//    }
//}

void delay_ms(unsigned int ms)
{
    unsigned int i;
    do{
        i = MAIN_Fosc / 6030;
        while(--i);
    }while(--ms);
}

void Task1s()
{
    static BYTE tm = 0;

    CLR_WDT = 1;  // 喂狗
    tm++;
    if(tm == 10)
    {
        SyncModBusDev();
        tm = 0;
    } 
    
    if((tm == 6)||(tm == 3))
    {
    }
    
    if(tm == 9)
    {
        
    }

}


void TimerTask()
{
    u16 delta = 0;
    static u16 Time1s = 0,Cnt = 0;
    
    if (Timer0Cnt)
    {
        delta = Timer0Cnt * 10;
        Timer0Cnt = 0;

        if (RX1_Cnt > 0)
        {
            Rx1_Timer += delta;
        }
        
        Time1s += delta;
        if (Time1s >= 100)
        {
            CLR_WDT = 1;  // 喂狗
            Time1s = 0;
            //Task1s();
        }

        Cnt += delta;
        if (Cnt >= CntTimer)
        {
            Cnt = 0;
            Task_Counter();
        }

        SleepCnt += delta;
        RunLed(delta);
    }
}

void Sleep(unsigned int ms)
{   
    SleepCnt = 0;
    
    while(1)
    {
        TimerTask();

        if (SleepCnt >= ms)
        {
            break;
        }        
    }
}


