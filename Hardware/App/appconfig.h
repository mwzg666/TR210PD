#ifndef _APPCONFIG_H_
#define _APPCONFIG_H_

#include "STC32G.h"
#include "stdio.h"
#include "intrins.h"
#include "string.h"
#include "stdlib.h"
#include "config.h"

#define     TRUE    1
#define     FALSE   0

typedef     unsigned char    BOOL;

typedef     unsigned char    BYTE;
typedef     unsigned short    WORD;
typedef     unsigned long    DWORD;

typedef     unsigned char    u8;
typedef     unsigned short    u16;
typedef     unsigned long    u32;

typedef     u32    uint32_t;
typedef     u8    uint8_t;
typedef     u8    uint16_t;

typedef     unsigned int    uint;

#define alt_u8      BYTE
#define alt_u16     WORD
#define alt_u32     DWORD  


#define bool        BYTE
#define true        1
#define false       0

#define BOOL        BYTE
#define TRUE        1
#define FALSE       0


#define CHANNEL_1  (1<<2)
#define CHANNEL_2  (1<<1)
#define CHANNEL_3  (1<<0)

#define ERR_SENSER (1<<7)        // 探测器故障
#define ERR_DATA   (1<<6)       // 数据故障
#define ERR_EEPROM (1<<5)       // 存储故障
#define ERR_DOSEOF (1<<4)       // 剂量当量过载
#define ERR_DROF   (1<<3)       // 剂量当量率过载
#define ERR_INIT   (1<<0)       // 正在初始化

#define DOSERATE_PRE_ALARM_BIT      (1<<7)            //剂量率一级报警位
#define DOSERATE_ALARM_BIT          (1<<6)            //剂量率二级报警位
#define DOSE_PRE_ALARM_BIT          (1<<5)            //累计剂量一级报警位
#define DOSER_ALARM_BIT             (1<<4)            //累计剂量二级报警位
#define OVER_RANGE_BIT              (1<<3)            //过载报警位
#define INVALID_ALARM1_BIT          (1<<2)            //失效二级报警位
#define INVALID_ALARM2_BIT          (1<<1)            //失效二级报警位
#define LOW_BACK_BIT                (1<<0           //低本底报警位

typedef struct
{
    BYTE ParaCheckSum;
    BYTE Channel;
    BYTE RepMode;
    BYTE AoutSw;
    BYTE Thres[3];
    BYTE Res;
    float Cr;      // 校准因子
    WORD  Sum;

}QPara;


typedef struct
{
    float DoseRate;   
    float DoseTotal;
    //float DoseMax;
    DWORD DevSt;
    float Cps1;
    float Cps2;
    float Cps3;
}STU_DOSERATE;

typedef struct
{
    float  DoseRatePreAlarm;        //剂量率一级报警(uSv/h)
    float  DoseRateAlarm;              //剂量率二级报警(uSv/h)
    float  DosePreAlarm;              //累计剂量一级报警(uSv)
    float  DoseAlarm;                  //累计剂量二级报警(uSv)
}SYS_ALARM;

typedef struct
{
    WORD DET_THR[3];                 //探测器阈值(0-255)
    WORD AnalogChannel;             //模拟通道
}STU_THR;


typedef struct
{
    BYTE Addr;
    BYTE Ver;
    BYTE Channel;
    BYTE RepMode;
    BYTE AoutSw;
    STU_DOSERATE s_DoseRate;
    SYS_ALARM s_Alarm;
    STU_THR s_Thr;
    float fix[16];
    WORD  Sum;
}SYS_PARAM;



#endif
