#ifndef _LED_H_
#define _LED_H_



//===================================定义=================================

#define RUN_LED(x) (x)?(P5 |= (1<<3)):(P5 &= ~(1<<3))    // 板载LED
#define DEVRUN_LED(x) (x)?(P2 |= (1<<2)):(P2 &= ~(1<<2))    // 板载LED



//==================================变量声明=============================



//==================================函数声明=============================

//void LED_Init(void);
void RunLed(u16 dt);


#endif


