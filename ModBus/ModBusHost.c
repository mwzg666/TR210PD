#include <stdio.h>
#include <stdlib.h>
#include <string.h>


#include "main.h"
#include "Lcd.h"
#include "LcdApp.h"
#include "ModBus.h"
#include "ModBusHost.h"
#include "uart.h"


/*
ModBus ֡��ʽ
1. ����֡
��ַ           ����     �Ĵ���    �Ĵ�������    ����                                             CRC   
0A(�̶�ֵ)     Cmd(1)   RX(2)     n(2)          �����ݱ�ʾ��ȡ�������ݱ�ʾд��Ӧ�ļĴ��� 

���ݶ���:  ���� + ����
           n*2    dat(n*2)

2. Ӧ��֡ -- ��������
��ַ           ����   ���ݳ���    ����      CRC   
0A(�̶�ֵ)     Cmd    n(1)        dat(n)

3. Ӧ��֡ -- ����״̬
��ַ           ����   �Ĵ���   �Ĵ�������     CRC   
0A(�̶�ֵ)     Cmd    Rx(2)    n(2)                       
*/

HOST_SEND_FRAME xdata  HostSendFrame;   // ����֡
DEVICE_READ_ACK xdata  ReadAckFrame;    // ������֡
DEVICE_WRITE_ACK xdata WriteAckFrame;  // д����֡


BYTE MbId[2] = {0};


WORD HostBufIndex = 0;


WORD PopReg()
{
    WORD reg = 0;
    memcpy(&reg, &ReadAckFrame.Data[HostBufIndex], 2);
    HostBufIndex +=2;
    reg =  RegSw(reg);
    return reg;
}


BYTE HostSendCmd(BYTE Mode, BYTE Addr, BYTE Cmd, WORD Reg, WORD Count, BYTE * Data)
{
    WORD i = 0;
    WORD crc, SendLen;
    memset(&HostSendFrame, 0, sizeof(HOST_SEND_FRAME));
    
    HostSendFrame.Address = Addr; //Param.DevAddr;
    HostSendFrame.FunctionCode = Cmd;
    HostSendFrame.RegAddr = RegSw(Reg);
    HostSendFrame.RegCount =  RegSw(Count);

    SendLen = 6;

    if (((Count*2) > 0) && (Data != NULL))
    {
        HostSendFrame.Data[0] = Count*2;  // ���ݳ���
        SendLen ++;
        i++;
        memcpy(&HostSendFrame.Data[1], Data, Count*2);
        SendLen += Count*2;
        i+=2;
    }
    
    // ����CRC , ����ӵ����ݺ���
    crc = CRC16Calc((BYTE *)&HostSendFrame, SendLen);
    HostSendFrame.Data[i]  = (BYTE)(crc);
    HostSendFrame.Data[i+1] = (BYTE)(crc>>8);
    
    SendLen += 2; 
    SendBuffer(Mode, (BYTE *)&HostSendFrame, SendLen);

    return true;
}



// ����ԭ���ǰ�װMODBUS_RTUд�ģ�����MODBUS_TCPʱ���⴦��һ��
void SendTcpFrame(BYTE * Data, WORD Length)
{
    BYTE Head[6] = {0};
    WORD sLen = Length - 2;  // MODBUS_TCP û��CRC
    memcpy(Head,MbId, 2);
    Head[5] = (BYTE)sLen;
    
    //UnusedVar(Data);
    //Uart3Send(Head,6);
    //Uart3Send(Data, (BYTE)sLen);
}

void SendBuffer(BYTE Mode, BYTE * Data, WORD Length)
{
    switch(Mode)
    {
        case RJ45  : SendTcpFrame(Data, Length);        break;
        case RS485 : Uart3Send(Data, (BYTE)Length);     break;
    }
}

