/*************************************************************************
ͨ��Э��

��:  M47ADS-PD����̽��ģ��˵��-20160802.docx
***************************************************************************/
#include "Cmd.h"
#include "uart.h"
#include "main.h"
#include "pkur.h"
extern const char Ver[];
extern const char Id[];
extern QPara Sys_Para;
extern unsigned char RecvBuf[UART_BUF_LEN];
extern unsigned char SendBuf[UART_BUF_LEN];
extern int RecLen;
extern WORD UartCnt;
extern BYTE PrukData[6];

extern float DoseRate;
extern float DoseMax;
extern float DoseTotal;
extern DWORD Cps;
extern DWORD Cps30s;
extern float CpsHis;
extern BYTE DevSt;
extern BOOL bIniting;

BYTE RecData[16];
BYTE SendData[16];

BYTE Cmd,Len;

BYTE GetCs(BYTE b, BYTE len)
{
    BYTE i,ret;
    ret = 0;

    for (i=b;i<b+len;i++)
    {
        ret += RecvBuf[i];
    }

    return ret;
}

BYTE SetCs(BYTE len)
{
    BYTE i,ret;
    ret = 0;
    for (i=0;i<len;i++)
    {
        ret += SendBuf[i];
    }
    return ret;
}

void Handle_Uart_Rec()
{
    BYTE i,j;

    // 1����û�����������buf
    if (Rx1_Timer < 20) 
    {
        RX1_Cnt = 0;
        ClearUart1Buf();
    }
    
    if (RX1_Cnt < 5)   // һ֡�����ֽ�
    {
        return;
    }

    for (i=0;i<RX1_Cnt; i++)
    {
        if (RX1_Buffer[i] == 0x16)
        {
            // ������ʼ��
            j = 0;
            while(RX1_Buffer[j] != 0x68)
            {
                j++;
                if (j >= RX1_Cnt) 
                {
                    return ;
                }
            }

            Cmd = RX1_Buffer[j+1];
            Len = RX1_Buffer[j+2];
            

            //  У�鳤��
            if (Len == (i - j - 4))
            {
                //  ����У��
                if (RX1_Buffer[i-1]  == GetCs(j,(BYTE)(Len+3)))
                {
                    if (Len > 0)
                    {
                        memcpy(RecData,&RX1_Buffer[j+3],Len);
                    }
                    HandCmd();
                    
                    RX1_Cnt = 0;
                    ClearUart1Buf();
                }
            }
        }
    }

    
}


void SendCmd(BYTE Cmd, BYTE Len, BYTE *Dat)
{
    SendBuf[0] = 0x68;
    SendBuf[1] = Cmd;
    SendBuf[2] = Len;
    if (Len > 0)
    {
        memcpy(&SendBuf[3],Dat,Len);
    }
    SendBuf[Len+3] = SetCs((BYTE)(Len+3));
    SendBuf[Len+4] = 0x16;

    Uart1Send(SendBuf,(u8)(Len+5));
}


void SkHand()
{
    SendCmd(CMD_SHARK_HAND,0,NULL);
}

void GetDr()
{
    memset(SendData,0,16);
    memcpy(SendData,&DoseRate,4);
    SendCmd(CMD_GET_DR,4,SendData);
}

void GetDt()
{
    memset(SendData,0,16);
    memcpy(SendData,&DoseTotal,4);
    SendCmd(CMD_GET_DOSE,4,SendData);
}

void ClrDt()
{
    DoseTotal = 0.0;
    
    memset(SendData,0,16);
    SendData[0] = 0;
    SendCmd(CMD_CLR_DOSE,1,SendData);
}

void GetMax()
{
    memset(SendData,0,16);
    memcpy(SendData,&DoseMax,4);
    SendCmd(CMD_GET_MAX,4,SendData);
}

void GetSt()
{
    // ���ﻹ�д�����
    
    memset(SendData,0,16);
    SendData[0] = DevSt;
    SendCmd(CMD_GET_STATUS,1,SendData);
}

void GetFct()
{
    memset(SendData,0,16);
    memcpy(SendData,&Sys_Para.Cr,4);
    SendCmd(CMD_GET_FACTOR,4,SendData);
}

void SetFct()
{
    if (Len != 4)
    {
        return;
    }
    memcpy(&Sys_Para.Cr,RecData,4);
    WriteParam();
    
    memset(SendData,0,16);
    SendData[0] = 0;
    SendCmd(CMD_SET_FACTOR,1,SendData);
}

void GetId()
{
    memset(SendData,0,16);
    memcpy(SendData,&Id,4);
    SendCmd(CMD_GET_MODEID,4,SendData);
}

void GetVer()
{
    memset(SendData,0,16);
    memcpy(SendData,&Ver,4);
    SendCmd(CMD_GET_VER,4,SendData);
}

void GetCps()
{   
    float cps;
    memset(SendData,0,16);
    #if 1
    // ����ʵʱCPS

    cps = (float)Cps;
    memcpy(SendData,&cps,4);
    #else
    // ����ƽ�����CPS
    memcpy(SendData,&CpsHis,4);
    #endif
    SendCmd(CMD_GET_CPS,4,SendData);
}

void GetCps30s()
{
    memset(SendData,0,16);
    memcpy(SendData,&Cps30s,4);
    SendCmd(CMD_GET_CPS,4,SendData);
}


void SetThres()
{
    if (Len != 1)
    {
        return;
    }

    Sys_Para.Thres[0] = 255 - RecData[0];

    // �µ�ϵͳֻ��һ��ͨ��
    //Sys_Para.Thres[1] = 255 - RecData[1];
    //Sys_Para.Thres[2] = 255 - RecData[2];
    
    Pkur_Init(Sys_Para.Channel);
    WriteParam();

    memset(SendData,0,16);
    SendData[0] = 0;
    SendCmd(CMD_SET_THRES,1,SendData);
}



void HandCmd()
{
    switch(Cmd)
    {
        case CMD_SHARK_HAND    : SkHand();     break;     // ��������
        case CMD_GET_DR        : GetDr();      break;     // ����ǰ����������
        case CMD_GET_DOSE      : GetDt();      break;     // ����ǰ��������
        case CMD_GET_MAX       : GetMax();     break;     // ��������������
        case CMD_GET_STATUS    : GetSt();      break;     // ����ǰ״̬
        case CMD_CLR_DOSE      : ClrDt();      break;     // �����������
        case CMD_GET_FACTOR    : GetFct();     break;     // ��У׼��������
        case CMD_SET_FACTOR    : SetFct();     break;     // дУ׼��������
        case CMD_GET_MODEID    : GetId();      break;     // ��ģ����
        case CMD_GET_VER       : GetVer();     break;     // ��ͨ��Э��汾
        case CMD_GET_CPS       : GetCps();     break;     // ��CPS
        case CMD_GET_CPS_30S   : GetCps30s();  break;     // ��30�����
        case CMD_SET_THRES     : SetThres();   break;     // ������ֵ
    }
}

