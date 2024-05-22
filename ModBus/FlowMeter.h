#ifndef __FLOWMETER_H__
#define __FLOWMETER_H__


#define FLOW_METER_ADDR   0x01       // ������Ĭ�ϵ�ַ��255
#define FLOW_VALUE_REG    0x003A




typedef struct
{
    float val;    // L/min
    float Totol;   // ������
    float Alarm;
}FLOW_VALUE; 

void SendReadFlow(BYTE ch);
void ReadFlow(BYTE ch);
void FlowMeterAck(BYTE *Buf, BYTE Len);

#endif

