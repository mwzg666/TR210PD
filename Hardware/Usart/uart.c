#include "uart.h"
#include "ModBusDev.h"
u16 Rx1_Timer  = 0;

u8  TX1_Cnt;    //发送计数
u8  RX1_Cnt;    //接收计数
bit B_TX1_Busy; //发送忙标志

u8  xdata RX1_Buffer[MAX_LENGTH]; //接收缓冲

u8  RecvBuf[UART_BUF_LEN] = {0};
u8  SendBuf[UART_BUF_LEN] = {0};


void UART1_config()
{
    /*********** 波特率使用定时器1 *****************/
    {
        TR1 = 0;
        AUXR &= ~0x01;      //S1 BRT Use Timer1;
        AUXR |=  (1<<6);    //Timer1 set as 1T mode
        TMOD &= ~(1<<6);    //Timer1 set As Timer
        TMOD &= ~0x30;      //Timer1_16bitAutoReload;
        TH1 = (u8)((65536UL - (MAIN_Fosc / 4) / Baudrate3) / 256);
        TL1 = (u8)((65536UL - (MAIN_Fosc / 4) / Baudrate3) % 256);
        ET1 = 0;    //禁止中断
        INTCLKO &= ~0x02;  //不输出时钟
        TR1  = 1;
    }

    /*************************************************/
    //UART1模式, 0x00: 同步移位输出, 0x40: 8位数据,可变波特率, 
    //           0x80: 9位数据,固定波特率, 0xc0: 9位数据,可变波特率 

    SCON = (SCON & 0x3f) | 0x40; 

    PS  = 0;    //中断高优先级
    PSH = 1;

    ES  = 1;    //允许中断
    REN = 1;    //允许接收
    
    //UART1 switch to, 0x00: P3.0 P3.1, 0x40: P3.6 P3.7, 
    //                 0x80: P1.6 P1.7, 0xC0: P4.3 P4.4
    P_SW1 &= 0x3f;
    P_SW1 |= 0x40;  
    
    B_TX1_Busy = 0;
    TX1_Cnt = 0;
    RX1_Cnt = 0;
}

void UART1_ISR (void) interrupt 4
{
    if(RI)
    {
        RI = 0;
        Rx1_Timer = 0;
        RX1_Buffer[RX1_Cnt] = SBUF;
        
        if(++RX1_Cnt >= MAX_LENGTH)   
        {
            RX1_Cnt = 0;
        }
    }

    if(TI)
    {
        TI = 0;
        B_TX1_Busy = 0;
    }
}


//UART1 发送串口数据
void UART1_SendData(char dat)
{
    ES=0;            //关串口中断
    SBUF=dat;            
    while(TI!=1);    //等待发送成功
    TI=0;            //清除发送中断标志
    ES=1;            //开串口中断
}

//UART1 发送字符串
void UART1_SendString(char *s)
{
    while(*s)//检测字符串结束符
    {
        UART1_SendData(*s++);//发送当前字符
    }
}

//重定向Printf
char putchar(char c )
{
    SBUF = c;
    while(!TI);
    TI = 0;
    return c;
}


void Uart1Send(u8 *buf, u8 len)
{
    u8 i;
    for (i=0;i<len;i++)     
    {
        SBUF = buf[i];
        B_TX1_Busy = 1;
        while(B_TX1_Busy);
    }
}


void ClearUart1Buf()
{
    memset(RX1_Buffer,0,UART_BUF_LEN);
    RX1_Cnt = 0;
}


void Uart1Hnd()
{
    if (Rx1_Timer > 20)
    {
        Rx1_Timer = 0;
        //Uart1Send(RX1_Buffer,RX1_Cnt);
        HndModBusRecv(RX1_Buffer,RX1_Cnt);
        ClearUart1Buf();
    }
}


