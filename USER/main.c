#include "main.h"
#include "led.h"
#include "time.h"
#include "uart.h"
#include "eeprom.h"
#include "PDoseRateCalc.h"
#include "ModBus.h"
#include "ModBusDev.h"
#include "Pkur.h"
#include "Cmd.h"
#include "PdFilter.h"

#ifdef USE_EPD_DOSE
//#include "Epd.h"
#include "epd_dose.h"
#endif

SYS_PARAM xdata SysParam = {0};


const char Ver[] = "V2.0";
const char Id[] =  "M47";

WORD  TimerACnt = 0;
WORD  SleepCnt  = 0;
WORD  UartCnt = 0;

BYTE PrukData[6] = {0};
float DoseRate=0.0;
float DoseMax=0.0;
float DoseTotal=0.0;
DWORD Cps = 0;

DWORD Cps30s = 0;

WORD  CntTimer = 1000;

QPara Sys_Para;

BYTE DevSt = 0x00;
BOOL bIniting = TRUE;

// 平滑后的CPS值
float CpsHis = 0.0;

#ifdef USE_EPD_DOSE
QEpd Epd;
tag_CALC_DOSERATE Calc_doserate;

#endif

void Error()
{
    while(1)
    {
        RUN_LED(1);
        delay_ms(50);
        RUN_LED(0);
        delay_ms(50);
    }
    
}


void SysInit()
{
    HIRCCR = 0x80;           // 启动内部高速IRC
    while(!(HIRCCR & 1));
    CLKSEL = 0;              
}

void IoInit()
{
    EAXFR = 1;
    WTST = 0;                                           //设置程序指令延时参数，赋值为0可将CPU执行指令的速度设置为最快

    P0M1 = 0x00;   P0M0 |= (1<<6) ;                     // P0.0 P0.1 P0.4 推挽输出
    P1M1 = 0x00;   P1M0 = 0x00;                         //设置为准双向口
    P2M1 = (1<<4);   P2M0 = 0x00; //(1<<2)|(1<<3)|(1<<5)|(1<<6)|(1<<7);                      // P2.2 推挽输出
    P3M1 = (1<<2);   P3M0 = 0x00;        //设置为准双向口
    P4M1 = 0x00;   P4M0 = 0x00;//(1<<5)|(1<<6);                         //设置为准双向口
    P5M1 = 0x00;   P5M0 |= (1<<3);             //设置为准双向口

}

void LedInit()
{
    // 初始状态都为0
    //RUN_LED(0);
    DEVRUN_LED(0);
}


//unsigned char CheckSum(char *buf,int len)
//{
//    BYTE Sum=0;
//    //unsigned char verify=0;
//    unsigned int i=0;
//    
//    for(i=1;i<len;i++)
//    {
//        Sum+=*(buf+i);
//    }
//    //verify=(char)(Sum%256);
//    return Sum%234;
//}
WORD ParamCheck(BYTE *buf, WORD len)
{
    WORD dwSum = 0;
    WORD i;

    for (i = 0; i < len; i++)
    {
        dwSum += buf[i];
    }

    return dwSum;
}

void ParamDef()
{
    BYTE i;
//    Sys_Para.Channel = CHANNEL_1; 
//    Sys_Para.RepMode = 1; // 自动汇报

    
//    Sys_Para.Thres[0] = 255-30;
//    Sys_Para.Thres[1] = 10;
//    Sys_Para.Thres[2] = 150;

//    Sys_Para.AoutSw = 0;
    Sys_Para.Cr = 1;
    SysParam.Addr = 1;
    SysParam.Ver = VERSION;
    SysParam.Channel = CHANNEL_1;
    SysParam.RepMode = 1;
    SysParam.s_Thr.DET_THR[0] = 255-30;
    SysParam.s_Thr.DET_THR[1] = 10;
    SysParam.s_Thr.DET_THR[2] = 150;
    SysParam.AoutSw = 0;
    for(i = 0;i<16;i++)
    {
        SysParam.fix[i] = 1.0;
    }
    
}

void WriteParam()
{
    EA = 0;    
    
    EEPROM_SectorErase(0);
    EEPROM_SectorErase(512);
    SysParam.Sum = ParamCheck((BYTE *)&SysParam, sizeof(SYS_PARAM)-2);
    if (!EEPROM_write(0, (BYTE *)&SysParam, sizeof(SYS_PARAM)))
    {
        Error();
    }
    EA = 1;     //打开总中断
}


void ReadParam()
{
    EEPROM_read(0, (BYTE *)&SysParam, sizeof(SYS_PARAM));

    #if 0
    memset(StrTmp,0,32);
    sprintf((char *)StrTmp,"%d \r\n",sizeof(SYS_PARAM));
    DebugMsg((char *)StrTmp);

    memset(StrTmp,0,32);
    sprintf((char *)StrTmp,"%d \r\n",sizeof(HOST_SENSOR_PARAM));
    DebugMsg((char *)StrTmp);

    memset(StrTmp,0,32);
    sprintf((char *)StrTmp,"%d \r\n",sizeof(HOST_ALRAM_PARA));
    DebugMsg((char *)StrTmp);

    memset(StrTmp,0,32);
    sprintf((char *)StrTmp,"%d \r\n",sizeof(float));
    DebugMsg((char *)StrTmp);
    
    //Rs485Send((BYTE *)&SysParam, sizeof(SYS_PARAM));
    
    
    if (SysParam.Sign != PARAM_SIGN)
    {
        DebugMsg("Sign error. \r\n");
    }

    if (SysParam.Sum != ParamCheck((BYTE *)&SysParam, sizeof(SYS_PARAM)-2))
    {
        DebugMsg("Param Check error. \r\n");
    }
    #endif

   
    if (SysParam.Sum != ParamCheck((BYTE *)&SysParam, sizeof(SYS_PARAM)-2))
    {
        ParamDef();
        WriteParam();
        //DebugMsg("Def Param. \r\n");
    }
}


WORD GetCounter()
{
    #if 1 
    WORD c;
    memset(PrukData,0,6);
    SGetCount(PrukData);
    SCLR();  

   
    c = PrukData[1];
    c <<= 8;
    c |= PrukData[0];
    return c;
    #else
    static BYTE i = 0;
    static WORD j = 0;

    i++;
    //j = 0;
    if (i>6)
    {
        i = 0;
        return 1; // j++;
    }
    else
    {
        return 0; //j;
    }
    #endif
}

void PkurSw(BYTE ch)
{
    if (ch & 1)
    {
        PKU_AE1(1);
    }
    else
    {
        PKU_AE1(0);
    }

    if (ch & 2)
    {
        PKU_AE2(1);
    }
    else
    {
        PKU_AE2(0);
    }
}

void Dev_Check()
{
    BYTE i;

    i = 10;     // 4S    
    while(i--)
    {
        DEVRUN_LED(1);
        Sleep(200);
        DEVRUN_LED(0);
        Sleep(200);
    }

    if (GetCounter() < 10)
    {
        //DevSt |= ERR_SENSER;
    }

    
    // wait 10 s
    i = 10;
    while(i--)
    {
        Sleep(1000);
        GetCounter();
    }
    
    bIniting = FALSE;
    //DevSt &= ~ERR_INIT;
}

#ifndef USE_EPD_DOSE

float CpsTouSv_h(float count)
{
    #if 0
    return count;
    #else
    float DRreturn;
    float count2;
    float count3;
    unsigned char State;

    const float lowfactor0 = -0.1150896707;
    const float lowfactor1 = 10.48925651;
    const float lowfactor2 = -0.3584771495;
    const float lowfactor3 = 0.3935024371e-1;

    const float midfactor0 = 4.175559983;
    const float midfactor1 = 13.59064852;
    const float midfactor2 = 0.5482365533e-3;
    const float midfactor3 = -0.1990246458e-6;

    const float highfactor0 = 971.2739047;
    const float highfactor1 = 12.99644062;
    const float highfactor2 = 0.1530853217e-3;
    const float highfactor3 = 0.1215235629e-8;

    DRreturn = 0.0f;
    count2 = count * count;
    count3 = count2 * count;

    #if 1
    if(count >= 2540)
    {
        State=2;  //high
    }
    else if(count >= 8.6)
    {   
        State=1;    //mid
    }
    else
    {
        State=0;  //low
    }
    #else
    State=0;  //low
    #endif

    
    if (count >= 0.14) /* cps :0.068333333 对应剂量率是:0.885738234uSv/h */
    {
        switch(State)
        {
            case 0:  //低剂量     
            {
                #if 1      
                DRreturn = lowfactor0 
                           + count * lowfactor1 
                           + count2 * lowfactor2 
                           + count3 * lowfactor3;
                #else

                DRreturn =    -1.387933839 
                            + count * 21.46101642
                            + count2 * -7.696467625 
                            + count3 * 2.613823815
                            + count3 * count *-0.3595228660 
                            + count3 * count2 *0.1713606062e-1;
                #endif
                break;
            }


            case 1:
            {
                DRreturn =    midfactor0 
                            + count * midfactor1 
                            + count2 * midfactor2 
                            + count3 * midfactor3;                
                break;
            }
            
            case 2:
            {
                DRreturn =    highfactor0 
                            + count * highfactor1 
                            + count2 * highfactor2 
                            + count3 * highfactor3;
                break;
            }


        } 
        //DRreturn = DRreturn / 1000.0; // to mSv
    }
    else
    {
        
        if (count <= 0.03)
        {
            //DRreturn = 0.000099 + count * 0.001;  //mSv
            DRreturn = 0.099 + count * 0.001;  //uSv
        }
        else
        {
            //DRreturn = 0.000099 + count * 0.01131;  // mSv
            DRreturn = 0.099 + count * 0.01131;  // uSv
        }
    }

    if (DRreturn > 99999000000)
    {
        DRreturn = 99999000000;
    }

    // 保留两位小数，后面的数据直接舍弃，不四舍五入
    //DRreturn *= Sys_Para.Cr;
    
    return (DRreturn <= 0 ? 0:DRreturn);
    
    #endif
}
#endif

void AddTotal()
{
    SysParam.s_DoseRate.DoseTotal = (float)Calc_doserate.fDoseSumx;
}

#ifndef USE_EPD_DOSE

// 取平均值
#define CT 120   
static DWORD CpsArr[CT] = {0};
static BOOL Full = FALSE;
static BYTE ArrCnt = 0;


// 将计数压入堆栈
void PushCps(DWORD cps)
{
    BYTE i;
    if (ArrCnt >= CT)
    {
        Full = TRUE;
        for (i=1;i<CT;i++)
        {
            CpsArr[i-1] = CpsArr[i];
        }
        CpsArr[CT-1] = cps;
    }
    else
    {
        CpsArr[ArrCnt++] = cps;
    }
}

void ClearCounter()
{
    memset(CpsArr,0,sizeof(CpsArr));
    Full = FALSE;
    ArrCnt = 0;
    CpsHis = 0;
}


// 计数平滑
float CounterPH()
{
    float Val = 0.0;
    BYTE i;
    
    
    Val = 0.0;
    if (Full)
    {
        for (i=0;i<CT;i++)
        {
            Val += CpsArr[i];
        }
        Val = Val/CT;
    }
    else if (ArrCnt > 0)
    {
        for (i=0;i<ArrCnt;i++)
        {
            Val += CpsArr[i];
        }
        Val = Val/ArrCnt;
    }
    else
    {
        Val = SysParam.s_DoseRate.Cps1;
    }

    CpsHis = Val;
    return Val;
}



float GetHis(BYTE time)
{
    BYTE i;
    float ret = 0.0;
    if (Full)
    {
        for (i=CT-time;i<CT;i++)
        {
            ret += CpsArr[i];
        }

        return (ret/time);
    }
    else
    {
        if (time > ArrCnt)
        {
            for (i=0;i<ArrCnt;i++)
            {
                ret += CpsArr[i];
            }

            return (ret/ArrCnt);
        }
        else
        {
            for (i=ArrCnt-time;i<ArrCnt;i++)
            {
                ret += CpsArr[i];
            }

            return (ret/time);
        }
    }

    //return 0.0;
}


// time :  保留最后计数的时间
void ResvCps(BYTE time)
{
    BYTE i;
    if (Full)
    {
        for (i=0;i<CT;i++)
        {
            if (i<time)
            {
                CpsArr[i] = CpsArr[CT-time+i];
            }
            else
            {
                CpsArr[i] = 0;
            }
        }
        ArrCnt = time;
        Full = FALSE;
    }
    else
    {
        if (time < ArrCnt)
        {
            for (i=0;i<ArrCnt;i++)
            {
                if (i<time)
                {
                    CpsArr[i] = CpsArr[ArrCnt-time+i];
                }
                else
                {
                    CpsArr[i] = 0;
                }
            }

            ArrCnt = time;
        }
        else
        {
            // 全部保留，就不做处理了
        }
    }
}
// 计数压入队列
void AddCounter(DWORD cps)
{
    #if 0
    static float His120S = 0.0;
    //static float His60S  = 0.0;
    static float His30S  = 0.0;
    static float His10S  = 0.0;
    static float His5S   = 0.0;
    static float His3S   = 0.0;

    float M1,M2,M3;
    #endif
    
    PushCps(cps);

    #if 0
    
    His120S = GetHis(120);
    //His60S  = GetHis(60);
    His30S  = GetHis(30);
    His10S  = GetHis(10);
    His5S   = GetHis(5);
    His3S   = GetHis(3);

    
    M1 = 1000; // 0.8mSv/h
    M2 = 80;   // 50uSv/h
    M3 = 1.6;  // 1uSv/h
    

    if (His3S > M1)   
    {
        if ((His3S > His120S * 1.03) || (His3S < His120S / 1.03))
        {
            ResvCps(3);
        }
    }
    else if (His5S > M2)  
    {
        if ((His5S > His120S * 1.1) || (His5S < His120S / 1.1))
        {
            ResvCps(5);
        }
    }
    else if (His10S > M3)  
    {
        if ((His10S > His120S * 1.2) || (His10S < His120S / 1.2))
        {
            ResvCps(10);
        }
    }
    else if (His30S > 0.0)  
    {
        if ((His30S > His120S * 1.3) || (His30S < His120S / 1.3))
        {
            ResvCps(30);
        }
    }

    #if 0
    else if (His60S > 0.0)  // 0.0u
    {
        if ((His60S > His120S * 1.1) || (His60S < His120S / 1.1))
        {
            ResvCps(60);
        }
    }
    #endif

    #endif
    
}


void CpsFilter(DWORD cps)
{
    static DWORD BigCpsErr[3] = {0};
    static BYTE BigErrCnt = 0;

    static DWORD SmCpsErr[3] = {0};
    static BYTE SmErrCnt = 0;
    
    if (CpsHis > 0.02)
    {
        //if (cps > 0)
        {
            if (cps > (CpsHis * 1.2))  // 大计数
            {
                SmErrCnt = 0;
                BigCpsErr[BigErrCnt++] = cps;
                if (BigErrCnt >= 3)
                {
                    // 连续两个大计数则不算抖动
                    ClearCounter();
                    AddCounter(BigCpsErr[0]);
                    AddCounter(BigCpsErr[1]);
					AddCounter(BigCpsErr[2]);
                    BigErrCnt = 0;
                }
            }
            else if (cps < (CpsHis / 1.2))  // 小计数
            {
                BigErrCnt = 0;
                SmCpsErr[SmErrCnt++] = cps;
                if (SmErrCnt >= 3)
                {
                    // 连续两个小计数则不算抖动
                    ClearCounter();
                    AddCounter(SmCpsErr[0]);
                    AddCounter(SmCpsErr[1]);
					AddCounter(SmCpsErr[2]);
                    SmErrCnt = 0;
                }
            }
            else
            {
                AddCounter(cps);
                BigErrCnt = 0;
                SmErrCnt = 0;
            }
        }
        //else
        //{
        //    AddCounter(cps);
        //    BigErrCnt = 0;
        //    SmErrCnt = 0;
        //}
    }
    else
    {
        AddCounter(cps);
        BigErrCnt = 0;
        SmErrCnt = 0;
    }
}

#endif

double DrFix(QEpd *me, double dr)
{
    QEpdDoseFactor *pDoseFactor = (QEpdDoseFactor *)&me->EpdPara.tDoseFactor;
    
    if (dr < 3.0)
    {
        return (dr*SysParam.fix[0]);//pDoseFactor->lowfactor[0]
    }
    else if (dr < 10.0)
    {
        return (dr*SysParam.fix[1]);
    }
    else if (dr < 50.0)
    {
        return (dr*SysParam.fix[2]);
    }
    else if (dr < 100.0)
    {
        return (dr*SysParam.fix[3]);
    }
    else if (dr < 500.0)
    {
        return (dr*SysParam.fix[4]);
    }
    else if (dr < 1000.0)
    {
        return (dr*SysParam.fix[5]);
    }
    else if (dr < 5000.0)  
    {
        return (dr*SysParam.fix[6]);
    }
    else if (dr < 10000.0)
    {
        return (dr*SysParam.fix[7]);
    }
    else if (dr < 50000.0)
    {
        return (dr*SysParam.fix[8]);
    }
    else if (dr < 100000.0)
    {
        return (dr*SysParam.fix[9]);
    }
    else if (dr < 500000.0)
    {
        return (dr*SysParam.fix[10]);
    }
    else if (dr < 1000000.0)
    {
        return (dr*SysParam.fix[11]);
    }
    else if (dr < 5000000.0)
    {
        return (dr*SysParam.fix[12]);
    }
    else if (dr < 10000000.0)
    {
        return (dr*SysParam.fix[13]);
    }
    else if (dr < 50000000.0)
    {
        return (dr*SysParam.fix[14]);
    }
    else if (dr < 100000000.0)
    {
        return (dr*SysParam.fix[15]);
    }

    return dr;
}


void CaluDoseRate()
{
    #ifndef USE_EPD_DOSE
    static BYTE Cnt = 0;
    static DWORD C30 = 0;
    float  c;
    
    
    Cnt++;
    if (Cnt<=30)
    {
        C30 += SysParam.s_DoseRate.Cps1;
    }
    else
    {
        Cps30s = C30;
        C30 = 0;
        Cnt = 0;
    }

    CpsFilter(SysParam.s_DoseRate.Cps1);    
    c = CounterPH();
    
    //DoseRate = CpsTouSv_h(c) * Sys_Para.Cr;
    //DoseRate = DoseRateCalc(c) * Sys_Para.Cr;
    SysParam.s_DoseRate.DoseRate = DrFix(&Epd,    DoseRateCalc(c)); //  DoseRateCalc(c) * Sys_Para.Cr;
    #else
    
    QEpdDoseRateUpdate(&Epd,&Calc_doserate,1,true);
    SysParam.s_DoseRate.DoseRate = (float)DrFix(&Epd, Calc_doserate.fDoseRate);   // Epd.fDoseRate * Sys_Para.Cr;
    #endif
    
    if (DoseMax < SysParam.s_DoseRate.DoseRate)
    {
        DoseMax = SysParam.s_DoseRate.DoseRate;
    }
    
    AddTotal();
    
}


void Task_Counter()
{
    static DWORD CpsTmp = 0;
    static BYTE  Cnt = 0;
    static BOOL  bCps1s = TRUE;

    if (bIniting)
    {
        return;
    }

    if (bCps1s)
    {
        CpsTmp = 0;
        Cnt    = 0;
        
        SysParam.s_DoseRate.Cps1 = GetCounter();
        CaluDoseRate();

 
        if (SysParam.s_DoseRate.Cps1 >= COUNTER_THRES)
        {
            CntTimer = 500;
            bCps1s = FALSE;
        }
    }
    else
    {
        CpsTmp += GetCounter();
        Cnt ++;
        if (Cnt >= 2)
        {
            Cnt    = 0;
            
            SysParam.s_DoseRate.Cps1 = CpsTmp;
            CaluDoseRate();

            CpsTmp = 0;
            if (SysParam.s_DoseRate.Cps1 < COUNTER_THRES)
            {
                CntTimer = 1000;
                bCps1s = TRUE;
            }
        }
    }
}

void SyncModBusDev()
{
    BYTE i;
    ModBusAddr.Addr = SysParam.Addr;
    ModBusAddr.Ver = SysParam.Ver;
    //sprintf(ModBusAddr.Ver,"V%d.%d.%d", SysParam.Ver/100, SysParam.Ver%100/10, SysParam.Ver%10 );
    memset(&ModBusDoserate, 0, sizeof(MODBUS_DOSERATE));
    ModBusDoserate.DoseRate = SysParam.s_DoseRate.DoseRate;
    ModBusDoserate.DoseTotal = SysParam.s_DoseRate.DoseTotal;
    ModBusDoserate.DevSt= SysParam.s_DoseRate.DevSt;
    ModBusDoserate.Cps1 = SysParam.s_DoseRate.Cps1;
    ModBusDoserate.Cps2 = SysParam.s_DoseRate.Cps2;
    ModBusDoserate.Cps3 = SysParam.s_DoseRate.Cps3;

    memset(&ModBusAlarm, 0, sizeof(MODBUS_ALARM));
    ModBusAlarm.DoseRatePreAlarm = SysParam.s_Alarm.DoseRatePreAlarm;
    ModBusAlarm.DoseRateAlarm= SysParam.s_Alarm.DoseRateAlarm;
    ModBusAlarm.DosePreAlarm = SysParam.s_Alarm.DosePreAlarm;
    ModBusAlarm.DoseAlarm = SysParam.s_Alarm.DoseAlarm;

    memset(&ModBusThr, 0, sizeof(MODBUS_THR));
    for(i = 0;i<3;i++)
    {
        ModBusThr.DET_THR[i] = SysParam.s_Thr.DET_THR[i];
    }
    ModBusThr.AnalogChannel = SysParam.s_Thr.AnalogChannel;

    memset(&ModBusFactor, 0, sizeof(MODBUS_FACTOR));
    for(i = 0;i < 16;i++)
    {
        ModBusFactor.Fix[i] = SysParam.fix[i];
    }

}


void main(void)
{
    SysInit();
    IoInit();
    LedInit();
    RUN_LED(1); 
    delay_ms(200);
    Timer0_Init();
    delay_ms(200); 
    
    PKUR(1);    
    delay_ms(500); 
    Pkur_Init(SysParam.Channel);
    delay_ms(500); 
    
    UART1_config();
    ClearUart1Buf();
    delay_ms(500);  
    
    ReadParam();
    delay_ms(200);
    

    // 待CPU稳定了再读参数
    #ifdef USE_EPD_DOSE
    QEpdParamInit(&Epd);
    #endif

    SyncModBusDev();
    delay_ms(200);
    
    Pkur_Init(SysParam.Channel); 
    delay_ms(500);   
    Dev_Check();
    
    RUN_LED(0);
    InitArr(&Calc_doserate,BK_CPS);
    EA = 1;                     //打开总中断

    WDT_CONTR |= (1<<5) |  7;   // 启动开门狗，约8秒
    while(1)
    {
        TimerTask();
        Uart1Hnd();
   
    }
}


