#include "ModBus.h"
#include "ModBusDev.h"
#include "uart.h"

MODBUS_ADDR xdata ModBusAddr;
MODBUS_DOSERATE xdata ModBusDoserate;
MODBUS_ALARM xdata ModBusAlarm;
MODBUS_THR xdata ModBusThr;
MODBUS_FACTOR xdata ModBusFactor;



/*
ModBus 帧格式
1. 发送帧
地址           命令     寄存器    寄存器数量    数据                                             CRC   
0A(固定值)     Cmd(1)   RX(2)     n(2)          无数据表示读取、有数据表示写对应的寄存器 

数据定义:  长度 + 数据
           n*2    dat(n*2)

2. 应答帧 -- 返回数据
地址           命令   数据长度    数据      CRC   
0A(固定值)     Cmd    n(1)        dat(n)

3. 应答帧 -- 返回状态
地址           命令   寄存器   寄存器数量     CRC   
0A(固定值)     Cmd    Rx(2)    n(2)                       
*/


DEVICE_READ_ACK xdata  DevReadAck;   
DEVICE_WRITE_ACK xdata DevWriteAck;

HOST_SEND_FRAME xdata RecvFrame;   


// 把浮点数转换为大端打包到发送数据区
void PackageFloatValue(WORD Offset, float val)
{
    BYTE temp[4] = {0};
    FloatToBytes(val,temp);
    memcpy(&DevReadAck.Data[Offset], temp, 4);  
}

void PackageDWordValue(WORD Offset, DWORD val)
{
    DWORD temp;
    temp = SwEndian(val);
    memcpy(&DevReadAck.Data[Offset], &temp, 4);  
}


void PackageWordValue(WORD Offset, WORD val)
{
    BYTE temp[2] = {0};
    temp[0] = (BYTE)(val >> 8);
    temp[1] = (BYTE)val;
    memcpy(&DevReadAck.Data[Offset], temp, 2);  
}

// 把寄存器值包装到发送缓存
bool PackageReg(WORD Reg, WORD Count)
{
    DWORD offset;
    BYTE *p;
    SyncModBusDev();
    if (Count > 128)
    {
        return false;
    }

    if (Reg >= MODBUS_DEV_ADD)
    {
        offset = (Reg - MODBUS_DEV_ADD)*2;
        if (offset >= sizeof(MODBUS_ADDR))
        {
            return false;
        }
        p = (BYTE *)&ModBusAddr;
        memcpy(DevReadAck.Data, &p[offset], Count*2);  
    }

     else if (Reg >= MODBUS_FACTOR_ADD)
    {
        offset = (Reg - MODBUS_FACTOR_ADD)*2;
        if (offset >= sizeof(MODBUS_FACTOR))
        {
            return false;
        }
        
        p = (BYTE *)&ModBusFactor;
        memcpy(DevReadAck.Data, &p[offset], Count*2); 
    }

    else if (Reg >= MODBUS_THR_ADD)
    {
        offset = (Reg - MODBUS_THR_ADD)*2;
        if (offset >= sizeof(MODBUS_THR))
        {
            return false;
        }
        
        p = (BYTE *)&ModBusThr;
        memcpy(DevReadAck.Data, &p[offset], Count*2); 
    }

    else if (Reg >= MODBUS_ALARM_ADD)
    {
        offset = (Reg - MODBUS_ALARM_ADD)*2;
        if (offset >= sizeof(MODBUS_ALARM))
        {
            return false;
        }
        
        p = (BYTE *)&ModBusAlarm;
        memcpy(DevReadAck.Data, &p[offset], Count*2);  
    }
    else if (Reg >= MODBUS_DOSERATE_ADD)
    {
        offset = (Reg - MODBUS_DOSERATE_ADD)*2;
        if (offset >= sizeof(MODBUS_DOSERATE))
        {
            return false;
        }
        p = (BYTE *)&ModBusDoserate;
        memcpy(DevReadAck.Data, &p[offset], Count*2);  
    }
    
    else
    {
        return false;
    }

    return true;
}

BYTE ReadAck( )
{
    WORD i = 0;
    WORD crc,SendLen;
    memset(&DevReadAck, 0, sizeof(DEVICE_READ_ACK));
    
    DevReadAck.Address = RecvFrame.Address; 
    DevReadAck.FunctionCode = RecvFrame.FunctionCode;
    SendLen = 2;
    DevReadAck.DataLen = RecvFrame.RegCount*2; 
    SendLen ++;
    PackageReg(RecvFrame.RegAddr, RecvFrame.RegCount);
    SendLen += DevReadAck.DataLen;
    
    // 计算CRC , 并添加到数据后面
    i = DevReadAck.DataLen;
    crc = CRC16Calc((BYTE *)&DevReadAck, SendLen);
    DevReadAck.Data[i]  = (BYTE)(crc);
    DevReadAck.Data[i+1] = (BYTE)(crc>>8);
    SendLen += 2; 
    Uart1Send((BYTE *)&DevReadAck, (BYTE)SendLen);
    return true;
}


// 发送写命令应答
void WriteAck()
{
    WORD crc;
    memset(&DevWriteAck, 0, sizeof(DEVICE_WRITE_ACK));
    
    DevWriteAck.Address = RecvFrame.Address;  
    DevWriteAck.FunctionCode = RecvFrame.FunctionCode;
    DevWriteAck.RegAddr = RegSw(RecvFrame.RegAddr);
    DevWriteAck.RegCount = RegSw(RecvFrame.RegCount);

    crc = CRC16Calc((BYTE *)&DevWriteAck, 6);
    DevWriteAck.Crc = crc;

    Uart1Send((BYTE *)&DevWriteAck, sizeof(DEVICE_WRITE_ACK));

}


void ModBusSaveParam()
{
    BYTE i = 0,j = 0;
    SysParam.Addr = ModBusAddr.Addr;
    
//    SysParam.s_DoseRate.DoseRate = ModBusDoserate.DoseRate;
//    SysParam.s_DoseRate.DoseTotal = ModBusDoserate.DoseTotal;
//    SysParam.s_DoseRate.DevSt = ModBusDoserate.DevSt;
//    SysParam.s_DoseRate.Cps1 = ModBusDoserate.Cps1;
//    SysParam.s_DoseRate.Cps2 = ModBusDoserate.Cps2;
//    SysParam.s_DoseRate.Cps3 = ModBusDoserate.Cps3;

    SysParam.s_Alarm.DoseRatePreAlarm = ModBusAlarm.DoseRatePreAlarm;
    SysParam.s_Alarm.DoseRateAlarm= ModBusAlarm.DoseRateAlarm;
    SysParam.s_Alarm.DosePreAlarm = ModBusAlarm.DosePreAlarm;
    SysParam.s_Alarm.DoseAlarm = ModBusAlarm.DoseAlarm;
    for(i = 0;i < 3;i++)
    {
        SysParam.s_Thr.DET_THR[3] = ModBusThr.DET_THR[3];
    }
    SysParam.s_Thr.AnalogChannel = ModBusThr.AnalogChannel ;
    
    for(i = 0;i < 16;i++)
    {
        SysParam.fix[i] = ModBusFactor.Fix[i];
    }

    
    WriteParam();
}

// 把接收到的数据加载到寄存器中
bool WriteRegValue(WORD Reg, WORD Count)
{
    BYTE *p;
    int len,offset;
    if ((Reg ==MODBUS_DEV_ADD)&& (Count == 1))
    {
        SysParam.Addr = RecvFrame.Data[2];
        WriteParam();
        return true;
    }
    else if(Reg >= MODBUS_FACTOR_ADD) 
    {
        len = sizeof(MODBUS_FACTOR);
        offset = (Reg - MODBUS_FACTOR_ADD) * 2;
        if ( (offset + Count * 2) > len )
        {
            return false;
        }
        p = (BYTE *)&ModBusFactor;        
        memcpy(&p[offset], &RecvFrame.Data[1], Count*2); //RecvFrame.Data[1]
        ModBusSaveParam();
        return true;
    }
    else if(Reg >= MODBUS_THR_ADD) 
    {
        len = sizeof(MODBUS_THR);
        offset = (Reg - MODBUS_THR_ADD) * 2;
        if ( (offset + Count * 2) > len )
        {
            return false;
        }
        p = (BYTE *)&ModBusThr;
        memcpy(&p[offset], &RecvFrame.Data[1], Count*2);
        ModBusSaveParam();
        return true;
    }
    else  if (Reg >= MODBUS_ALARM_ADD) 
    {
        len = sizeof(MODBUS_ALARM);
        offset = (Reg - MODBUS_ALARM_ADD) * 2;
        if ( (offset + Count * 2) > len )
        {
            return false;
        }
        p = (BYTE *)&ModBusAlarm;
        memcpy(&p[offset], &RecvFrame.Data[1], Count*2);
        ModBusSaveParam();
        return true;
    }
    return false;
}


// 设寄存器值
void SetRegValue()
{
    if (RecvFrame.Data[0] == 0)
    {
        return;
    }

    if (WriteRegValue(RecvFrame.RegAddr, RecvFrame.RegCount))
    {
        WriteAck();
    }
}


void HndModBusRecv(BYTE *buf, BYTE len)
{
    if (!ValidRtuFrame(buf, len))
    {

        return;
    }

    memset(&RecvFrame, 0, sizeof(HOST_SEND_FRAME));
    memcpy(&RecvFrame, buf, len);
    if (RecvFrame.Address != SysParam.Addr)
    {
        return;
    }

    switch(RecvFrame.FunctionCode)
    {
        case CMD_READ_REG: ReadAck();  break;
        case CMD_WRITE_REG: SetRegValue();  break;
    }
}



