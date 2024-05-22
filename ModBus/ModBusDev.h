#ifndef __MODBUS_DEV_H__
#define __MODBUS_DEV_H__
#include "main.h"

#define MODBUS_DEV_ADD          0xFFF0
#define MODBUS_DOSERATE_ADD     0x8000
#define MODBUS_ALARM_ADD        0x8010
#define MODBUS_THR_ADD          0x8020
#define MODBUS_FACTOR_ADD       0x9000

#pragma pack(1)

typedef struct
{   	   	    
    WORD Addr;	    
    //char Ver[16];
    WORD Ver;

}MODBUS_ADDR;


typedef struct
{
    float DoseRate;   
    float DoseTotal;
    //float DoseMax;
    DWORD DevSt;
    float Cps1;
    float Cps2;
    float Cps3;
}MODBUS_DOSERATE;

typedef struct
{
    float  DoseRatePreAlarm;    	//剂量率一级报警(uSv/h)
    float  DoseRateAlarm;  		    //剂量率二级报警(uSv/h)
    float  DosePreAlarm;  		    //累计剂量一级报警(uSv)
    float  DoseAlarm;  			    //累计剂量二级报警(uSv)

}MODBUS_ALARM;

typedef struct
{
    WORD DET_THR[3];      	   	    
    WORD AnalogChannel;	    

}MODBUS_THR;

typedef struct
{
    float Fix[16];	                    //修正系数
}MODBUS_FACTOR;

#pragma pack()
extern MODBUS_ADDR xdata ModBusAddr;
extern MODBUS_DOSERATE xdata ModBusDoserate;
extern MODBUS_ALARM xdata ModBusAlarm;
extern MODBUS_THR xdata ModBusThr;
extern MODBUS_FACTOR xdata ModBusFactor;



BYTE ReadAck( );
void HndModBusRecv(BYTE *buf, BYTE len);

#endif /*PROTOCOL_H_*/

