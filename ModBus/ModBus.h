#ifndef __MODBUS_H__
#define __MODBUS_H__
#include "main.h"

#define RJ45    0
#define RS485   1


#define DATA_BUF_LENGTH     256         // һ�����256���ֽ�


#define CMD_READ_REG                0x03     // ��ȡ�Ĵ���ֵ
#define CMD_WRITE_REG               0x10     // д�Ĵ���ֵ


#pragma pack(1)

// ������д���������ʱû��data�ֶ�
typedef struct 
{
    BYTE Address;         //��ַ��
    BYTE FunctionCode;    //���ܴ���
    WORD RegAddr;     // ��ַΪ���ģʽ
    WORD RegCount;
    
    BYTE Data[DATA_BUF_LENGTH + 2];   // ������ʱ�������ڵ�һ���ֽ�
}HOST_SEND_FRAME;

// �豸��Ӧ��
typedef struct 
{
    BYTE Address;        //��ַ��
    BYTE FunctionCode;   //���ܴ���

    // ���Ĵ���ʱû�����������ֶ�
    BYTE DataLen;         // �������ݵĳ���
    BYTE Data[DATA_BUF_LENGTH + 2];
}DEVICE_READ_ACK;

// �豸дӦ��
typedef struct 
{
    BYTE Address;         //��ַ��
    BYTE FunctionCode;   //���ܴ���
    WORD RegAddr;     // ��ַΪ���ģʽ
    WORD RegCount;
    WORD Crc;
}DEVICE_WRITE_ACK;
  
#pragma pack()


WORD CRC16Calc(BYTE *dataBuff, DWORD dataLen);
void FloatToBytes(float dat, BYTE *res);
float BytesToFloat(BYTE *dat);
DWORD BytesToDWord(BYTE *dat);
WORD BytesToWord(BYTE *dat);
DWORD SwEndian(DWORD input);
WORD SwEndianW(WORD input);
WORD RegSw(WORD dat);

WORD WordToSmall(WORD dat);
float FloatToSmall(float dat);
DWORD DwordToSmall(DWORD dat);
float SwEndianF(float input);


BYTE ValidRtuFrame(BYTE *abyRdBuf,BYTE len);
BYTE ValidTcpFrame(BYTE *abyRdBuf,BYTE len);

#endif

