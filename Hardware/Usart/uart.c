#include "uart.h"
#include "ModBusDev.h"
u16 Rx1_Timer  = 0;

u8  TX1_Cnt;    //���ͼ���
u8  RX1_Cnt;    //���ռ���
bit B_TX1_Busy; //����æ��־

u8  xdata RX1_Buffer[MAX_LENGTH]; //���ջ���

u8  RecvBuf[UART_BUF_LEN] = {0};
u8  SendBuf[UART_BUF_LEN] = {0};


void UART1_config()
{
    /*********** ������ʹ�ö�ʱ��1 *****************/
    {
        TR1 = 0;
        AUXR &= ~0x01;      //S1 BRT Use Timer1;
        AUXR |=  (1<<6);    //Timer1 set as 1T mode
        TMOD &= ~(1<<6);    //Timer1 set As Timer
        TMOD &= ~0x30;      //Timer1_16bitAutoReload;
        TH1 = (u8)((65536UL - (MAIN_Fosc / 4) / Baudrate3) / 256);
        TL1 = (u8)((65536UL - (MAIN_Fosc / 4) / Baudrate3) % 256);
        ET1 = 0;    //��ֹ�ж�
        INTCLKO &= ~0x02;  //�����ʱ��
        TR1  = 1;
    }

    /*************************************************/
    //UART1ģʽ, 0x00: ͬ����λ���, 0x40: 8λ����,�ɱ䲨����, 
    //           0x80: 9λ����,�̶�������, 0xc0: 9λ����,�ɱ䲨���� 

    SCON = (SCON & 0x3f) | 0x40; 

    PS  = 0;    //�жϸ����ȼ�
    PSH = 1;

    ES  = 1;    //�����ж�
    REN = 1;    //�������
    
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


//UART1 ���ʹ�������
void UART1_SendData(char dat)
{
    ES=0;            //�ش����ж�
    SBUF=dat;            
    while(TI!=1);    //�ȴ����ͳɹ�
    TI=0;            //��������жϱ�־
    ES=1;            //�������ж�
}

//UART1 �����ַ���
void UART1_SendString(char *s)
{
    while(*s)//����ַ���������
    {
        UART1_SendData(*s++);//���͵�ǰ�ַ�
    }
}

//�ض���Printf
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


