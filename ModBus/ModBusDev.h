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
    float  DoseRatePreAlarm;    	//������һ������(uSv/h)
    float  DoseRateAlarm;  		    //�����ʶ�������(uSv/h)
    float  DosePreAlarm;  		    //�ۼƼ���һ������(uSv)
    float  DoseAlarm;  			    //�ۼƼ�����������(uSv)

}MODBUS_ALARM;

typedef struct
{
    WORD DET_THR[3];      	   	    
    WORD AnalogChannel;	    

}MODBUS_THR;

typedef struct
{
    float Fix[16];	                    //����ϵ��
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

