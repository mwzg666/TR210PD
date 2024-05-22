#ifndef _TIME_H_
#define _TIME_H_


//===================================����=================================

#define SysTick         9216                        // 10ms,ϵͳ�δ�Ƶ��, ��4000~16000֮��
#define Timer0_Reload   (65536UL - SysTick)         //Timer 0 �ж�Ƶ��


//==================================��������=============================



//==================================��������=============================

void Timer0_Init(void);
void Timer1_Init(void);
void delay_ms(unsigned int ms);
void delay_us(unsigned char us);    
void TimerTask();
void Sleep(unsigned int ms);



#endif

