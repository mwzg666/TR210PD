#include "main.h"
#ifdef USE_EPD_DOSE

#include "epd_dose.h"
#include "PDoseRateCalc.h"

DWORD PluseCnt = 0;
float fAverCPS=0;

static float LDtoHDBuf[3];
static BYTE  ldtohdidx = 0;
//char beep_increase_on_flag=0;//是否有累计计量增长报警
#define CPS_500ms_Threshold (3000)
#define CPS_1000ms_Threshold (60000)
//BOOL bGetPkuPer500ms = false;

//extern float CalcAlpha(unsigned long realCPS, float oldCPS);
extern DWORD Cps;
extern float CpsHis;

//为提高运算校率，校验采用查表法：
//--------------------------------------------------------
//DESCRIPTION:RTU CRC校验的高位字节表
//--------------------------------------------------------
const u8 auchCRCHi[] = {    
0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 
0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 
0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 
0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 
0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 
0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 
0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 
0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 
0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 
0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 
0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 
0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 
0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 
0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 
0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 
0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40
};

//--------------------------------------------------------
//DESCRIPTION:RTU CRC校验的低位字节表
//--------------------------------------------------------
const u8 auchCRCLo[]={
0x00, 0xC0, 0xC1, 0x01, 0xC3, 0x03, 0x02, 0xC2, 0xC6, 0x06, 0x07, 0xC7, 0x05, 0xC5, 0xC4, 0x04, 
0xCC, 0x0C, 0x0D, 0xCD, 0x0F, 0xCF, 0xCE, 0x0E, 0x0A, 0xCA, 0xCB, 0x0B, 0xC9, 0x09, 0x08, 0xC8, 
0xD8, 0x18, 0x19, 0xD9, 0x1B, 0xDB, 0xDA, 0x1A, 0x1E, 0xDE, 0xDF, 0x1F, 0xDD, 0x1D, 0x1C, 0xDC, 
0x14, 0xD4, 0xD5, 0x15, 0xD7, 0x17, 0x16, 0xD6, 0xD2, 0x12, 0x13, 0xD3, 0x11, 0xD1, 0xD0, 0x10, 
0xF0, 0x30, 0x31, 0xF1, 0x33, 0xF3, 0xF2, 0x32, 0x36, 0xF6, 0xF7, 0x37, 0xF5, 0x35, 0x34, 0xF4, 
0x3C, 0xFC, 0xFD, 0x3D, 0xFF, 0x3F, 0x3E, 0xFE, 0xFA, 0x3A, 0x3B, 0xFB, 0x39, 0xF9, 0xF8, 0x38, 
0x28, 0xE8, 0xE9, 0x29, 0xEB, 0x2B, 0x2A, 0xEA, 0xEE, 0x2E, 0x2F, 0xEF, 0x2D, 0xED, 0xEC, 0x2C, 
0xE4, 0x24, 0x25, 0xE5, 0x27, 0xE7, 0xE6, 0x26, 0x22, 0xE2, 0xE3, 0x23, 0xE1, 0x21, 0x20, 0xE0, 
0xA0, 0x60, 0x61, 0xA1, 0x63, 0xA3, 0xA2, 0x62, 0x66, 0xA6, 0xA7, 0x67, 0xA5, 0x65, 0x64, 0xA4, 
0x6C, 0xAC, 0xAD, 0x6D, 0xAF, 0x6F, 0x6E, 0xAE, 0xAA, 0x6A, 0x6B, 0xAB, 0x69, 0xA9, 0xA8, 0x68, 
0x78, 0xB8, 0xB9, 0x79, 0xBB, 0x7B, 0x7A, 0xBA, 0xBE, 0x7E, 0x7F, 0xBF, 0x7D, 0xBD, 0xBC, 0x7C, 
0xB4, 0x74, 0x75, 0xB5, 0x77, 0xB7, 0xB6, 0x76, 0x72, 0xB2, 0xB3, 0x73, 0xB1, 0x71, 0x70, 0xB0, 
0x50, 0x90, 0x91, 0x51, 0x93, 0x53, 0x52, 0x92, 0x96, 0x56, 0x57, 0x97, 0x55, 0x95, 0x94, 0x54, 
0x9C, 0x5C, 0x5D, 0x9D, 0x5F, 0x9F, 0x9E, 0x5E, 0x5A, 0x9A, 0x9B, 0x5B, 0x99, 0x59, 0x58, 0x98, 
0x88, 0x48, 0x49, 0x89, 0x4B, 0x8B, 0x8A, 0x4A, 0x4E, 0x8E, 0x8F, 0x4F, 0x8D, 0x4D, 0x4C, 0x8C, 
0x44, 0x84, 0x85, 0x45, 0x87, 0x47, 0x46, 0x86, 0x82, 0x42, 0x43, 0x83, 0x41, 0x81, 0x80, 0x40
};

//校验函数:
u16 CRC16(u8 *PuchMsg, u16 usDataLen)
{
    u8  uchCRCHi = 0xFF;       //high byte of CRC initialized
    u8  uchCRCLo = 0xFF;       //low byte of CRC initialized
    u32 uIndex;                //will index into CRC lookup table

    while (usDataLen--)
    {
        //calculate the CRC
        uIndex=uchCRCHi^(unsigned char)(*PuchMsg++);
        uchCRCHi=uchCRCLo^auchCRCHi[uIndex];
        uchCRCLo=auchCRCLo[uIndex];
    }
    return(uchCRCHi <<8|uchCRCLo);
}


int QEpdParamInit(QEpd * me)
{
    struct _QEpdParaInfo *pParaInfo;
    struct _QEpdBaseConfig *pBaseConfig;
    struct _QEpdDoseFactor *pDoseFactor;
    //u16 checkSum;
    //me->RecvSize = sizeof(QEpdFactoryInfo);        //厂家参数,只读,不允许更改
    //me->RecvSize = sizeof(QEpdAssignInfo);            //设备分配信息,40Bytes
    //me->RecvSize = sizeof(QEpdStatusInfo);            //设备状态信息,20Bytes
    me->RecvSize = sizeof(QEpdBaseConfig);            //设备配置信息,32Bytes
    me->RecvSize = sizeof(QEpdDoseFactor);        //拟合参数信息,68Bytes = 16x4 + 1 + 3(保留)
    
    me->RecvSize = sizeof(struct _QEpdParaInfo);    
    
    me->byVariWin = 120;
    me->updateBatCnt = 0;
    me->timerCPSCnt = 0;
    me->idxFilterCPS = 0;
  //  MSP430XX_FLASH_Erase(QEPD_PARAM_FLASH_BASE_ADDR);
    //同步数据到内存空间
    //MSP430XX_FLASH_Read(QEPD_PARAM_FLASH_BASE_ADDR, (void *)&me->EpdPara, sizeof(struct _QEpdParaInfo));
    //MSP430XX_FLASH_Read(QEPD_PARAM_FLASH_BASE_ADDR + sizeof(struct _QEpdParaInfo), (void *)&checkSum, 2);
    pParaInfo= (struct _QEpdParaInfo *)&me->EpdPara;
    //if((pParaInfo->dwSignature!=0x00103011)||(checkSum!=CRC16((u8 *)pParaInfo, sizeof(struct _QEpdParaInfo)))) 
    {
        //默认值
        pBaseConfig = (struct _QEpdBaseConfig *)&(me->EpdPara.baseConfig);
        pDoseFactor = (struct _QEpdDoseFactor *)&(me->EpdPara.tDoseFactor);
        //struct _QLocalTime     *pLocalTime  = (struct _QLocalTime *)&pBaseConfig->AdjustTime;
        //struct _QEpdStatusInfo *pStatusInfo = (struct _QEpdStatusInfo *)&me->EpdPara.statusInfo;
        
        memset((void *)me,0x00,sizeof *me);
        me->byVariWin           = 120;
        me->fDoseSumx                 = 0.0f;
        //pStatusInfo->iStorageUsed    = 0x0;
        //pStatusInfo->iStorageMax     = 4096;
        
        pParaInfo->dwSignature       = 0x00103011;
        //pParaInfo->dwDeviceNo        = 0x0b59fbf1;//0x88991234;
        //pParaInfo->DoseNum.tdwDeviceNo.ver = 12;
        //pParaInfo->DoseNum.tdwDeviceNo.unit = 5;
        //pParaInfo->DoseNum.tdwDeviceNo.MainID = 654321;
        //pBaseConfig->iDoseSumxMeddle = 3000;
        //pBaseConfig->iDoseRateMeddle = 3000;
        
        //pBaseConfig->iUsedTimeAlarm = 2;//2分钟时间报警
          
        //pBaseConfig->iDoseSumxAlarm = 4000;
        //pBaseConfig->iDoseRateAlarm = 4000;
        pBaseConfig->iScaleFactor   = 1.0f;
        //pBaseConfig->ucRunningMode   = 0x00;
        //pBaseConfig->ucRunningMode  |= QEPD_PARAM_ASSIGNED_BIT;//分配
        //pBaseConfig->ucRunningMode  |= QEPD_PARAM_WORKMODE_BIT;//手动模式
        //pBaseConfig->ucRunningMode  |= QEPD_PARAM_ACTIVE_BIT;//（激活）
        //pBaseConfig->ucRunningMode  |= QEPD_PARAM_ENERYUSED_BIT;//省电模式
        
        //pBaseConfig->ucDoseIncreaseAlarm = 3;//剂量增长1uSv报警

        //pBaseConfig->ucModeSetting   = 0x00;
        //pBaseConfig->ucModeSetting  |= QEPD_PARAM_CLEARALARM_BIT;
        //pBaseConfig->ucModeSetting  |= QEPD_PARAM_INTER_SAVE;
        //pBaseConfig->ucModeSetting  &= QEPD_PARAM_STORAGE_INTERVAL_MASK;
        //pBaseConfig->ucModeSetting  |= storageInterval_15Sec*QEPD_PARAM_STORAGE_INTERVAL;

        //pBaseConfig->ucModeSetting  &= QEPD_PARAM_BACKLIGHT_MODE_MASK;
        //pBaseConfig->ucModeSetting  |= backLightMode_5sec*QEPD_PARAM_BACKLIGHT_MODE;
        
        //pBaseConfig->ucDisplayMode  = 0x00;
        //pBaseConfig->ucDisplayMode  &= QEPD_PARAM_DISPLAY_UNIT_MASK;
        //pBaseConfig->ucDisplayMode  |= dispUnit_usv*QEPD_PARAM_DISPLAY_UNIT;
        
        pDoseFactor->experimentmode = 0;    
//        pDoseFactor->lowfactor[0]= 1;
//        pDoseFactor->lowfactor[1]= 1;
//        pDoseFactor->lowfactor[2]= 1;
//        pDoseFactor->lowfactor[3]= 1;
//        pDoseFactor->lowfactor[4]= 1;
//        pDoseFactor->lowfactor[5]= 1;
//        pDoseFactor->lowfactor[6]= 1;
//        pDoseFactor->lowfactor[7]= 1;
//        pDoseFactor->lowfactor[8]= 1;
//        pDoseFactor->lowfactor[9]= 1;
//        pDoseFactor->lowfactor[10]= 1;
//        pDoseFactor->lowfactor[11]= 1;
//        pDoseFactor->lowfactor[12]= 1;
//        pDoseFactor->lowfactor[13]= 1;
//        pDoseFactor->lowfactor[14]= 1;
//        pDoseFactor->lowfactor[15]= 1;
        
        /*
        pDoseFactor->midfactor0= 4.175559983;
        pDoseFactor->midfactor1= 13.59064852;
        pDoseFactor->midfactor2= 0.5482365533e-3;
        pDoseFactor->midfactor3= -0.1990246458e-6;

        pDoseFactor->highfactor0= 971.2739047;
        pDoseFactor->highfactor1= 12.99644062;
        pDoseFactor->highfactor2= 0.1530853217e-3;
        pDoseFactor->highfactor3= 0.1215235629e-8;

        pDoseFactor->highestfactor0=0;
        pDoseFactor->highestfactor1=0;
        pDoseFactor->highestfactor2=0;
        pDoseFactor->highestfactor3=0;*/            
        //MSP430XX_RTC_GetTime(pLocalTime);
        //checkSum = CRC16((u8 *)pParaInfo, sizeof(struct _QEpdParaInfo));
        //QEpdParamSaveAll(me);
    }
    me->fDoseRate      = me->EpdPara.statusInfo.iMaxDoseRate;
    me->fDoseSumx    = me->EpdPara.statusInfo.iLastDoseSum;
    me->fDoseSumIncrease = me->EpdPara.statusInfo.iLastDoseSum;
    //QEpdParamUpdateSaveTime(me);
    return true;
}





#ifdef DEBUG

const float TestCPSBuf[]={0,3,0,1,2,0,0,0,0,0,0,0,0,1,0,0,0,1,0,1,0,0,0,0,0,0,1,0,0,0,0,0,2,0,0,1,1,0,1,1,2,1,0,0,1,1,0,0,0,1,1,1,2,0,1,0,0,0,1,1,0,0,0,0,1,0,0,0,1,0,0,0,0,1,1,1,0,1,0,0,1,0,0,0,0,0,1,0,0,1,0,0,1,0,1,1,2,0,0,0,0,0,0,1}; //4us

float GetCPS()
{
    int  iCPS= 0;
    u32 cps = 0;
    cps = TestCPSBuf[iCPS];
    if (++iCPS >= (sizeof(TestCPSBuf) / sizeof (TestCPSBuf[0])))
    {
         iCPS = 0;
    }
    return (float)cps;
}
#endif


#if 0


unsigned long PreCPS = 0;
void QEpdDoseRateUpdate(QEpd *me)
{
    //unsigned short ir;
    static float SumCPS=0,AverCPS=0;
    
    static int i=0,k=0;
    static int /*iMaxCnt = -1,*/ iMinCnt = -1;
    static int DoubtCnt=0;
    static float fTmp = 0;
    static float Max = 0;      

    iMinCnt = -1;
    SumCPS=0;
    AverCPS=0;
    i=0;
    k=0;
    //fAverCPS = 0;
    DoubtCnt=0;    
    fTmp = 0;   
    Max = 0;      
    
    #if 0
        if (me->bGetPkuPer500ms && (PluseCnt < CPS_1000ms_Threshold))
        {
            me->bGetPkuPer500ms = 0;
        }
        // deta CPS_500ms_Threshold 或者邋CPS 大于CPS_1000ms_Threshold，切换到500ms
        if ( ((PluseCnt > PreCPS) && (PluseCnt - PreCPS) > CPS_500ms_Threshold) || PluseCnt > CPS_1000ms_Threshold )
        {
            me->bGetPkuPer500ms = 1;
        }
        
        PreCPS = PluseCnt;
       me->cpsDoseCount = PluseCnt;
       LockCritcialSection();   
       PluseCnt=0;
       UnLockCriticalSection();
    #else
      // me->cpsDoseCount = GetCPS();
       //PluseCnt = Cps;
       me->cpsDoseCount = SysParam.s_DoseRate.Cps1;//Cps; // PluseCnt;
       //LockCritcialSection();   
       //PluseCnt=0;
       //UnLockCriticalSection();
    #endif
       me->EpdPara.statusInfo.iUsedTime++;
       me->sumDoseCount += me->cpsDoseCount; 
       if(++me->timerCPSCnt<=30)
        {
          me->cpsDosePerCnt += me->cpsDoseCount;
        }
       else if(me->timerCPSCnt>30)
        {
          me->cpsDosePerCnt=0; 
          me->timerCPSCnt=0;
        }
    #if 1
         if (me->byDoseRateMode == LOW_DOSE_RATE_MODE)
         {

             if (me->cpsDoseCount >= 1)
             {
                 if (me->idxFilterCPS == 0)
                 {
                    void *pbuf = (void *)me->affilterCPSBuf;
                     memset(pbuf, 0, sizeof(me->affilterCPSBuf));
                     me->idxFilterCPS = 0;
                 }
                 me->affilterCPSBuf[me->idxFilterCPS ] = me->cpsDoseCount;
                 me->idxFilterCPS++;
             }

             if (me->cpsDoseCount < 1)
             {
                  if (me->idxFilterCPS) //检查滤波BUF里面有值
                  {
                      for (i = 0; i < me->idxFilterCPS; i++)
                      {
                           if (me->affilterCPSBuf[i] > 7)
                           {
                                /* affilterCPSBuf 里面的计数可能是噪声引起的退出 */
                                break;
                           }
                       }
                           
                       memcpy((void *)me->afCPSBuf, (void *)(me->afCPSBuf + me->idxFilterCPS), 
                              (FIRST_CPS_MEASURE_TIME - me->idxFilterCPS) * sizeof(me->afCPSBuf[0]));                           
          
               
                       for (k = 0; k < me->idxFilterCPS ; k++)
                       {
                           if ( i == me->idxFilterCPS)
                           {                                   
                                me->afCPSBuf[FIRST_CPS_MEASURE_TIME - me->idxFilterCPS  + k] = me->affilterCPSBuf[k];
                           }
                           else
                           {   
                                 /* affilterCPSBuf 噪声引起的计数,afCPSBuf 里面计数用0 */
                                me->afCPSBuf[FIRST_CPS_MEASURE_TIME - me->idxFilterCPS  + k] = 0; 
                           }
                       }
    
                               
                       if(me->nCPSBuf < FIRST_CPS_MEASURE_TIME)
                       {
                            if ((me->nCPSBuf = me->nCPSBuf + me->idxFilterCPS) > FIRST_CPS_MEASURE_TIME)
                            {
                                 me->nCPSBuf = FIRST_CPS_MEASURE_TIME;
                            }
                       }     
                        me->idxFilterCPS = 0;
                  }

                      
                  for(i=0;i<(FIRST_CPS_MEASURE_TIME-1);i++)
                  {
                       me->afCPSBuf[i] = me->afCPSBuf[i+1];
                  }
                  me->afCPSBuf[i]=me->cpsDoseCount;    
                  if(me->nCPSBuf<FIRST_CPS_MEASURE_TIME)
                  {
                       me->nCPSBuf++; 
                  }
                      
             }
             else
             {
                  if(me->idxFilterCPS >= sizeof(me->affilterCPSBuf) / sizeof(me->affilterCPSBuf[0]))
                  {
                          memcpy((void *)me->afCPSBuf, (void *)(me->afCPSBuf + me->idxFilterCPS), 
                                 (FIRST_CPS_MEASURE_TIME - me->idxFilterCPS) * sizeof me->afCPSBuf[0]);
                          
                          for (k = 0; k < me->idxFilterCPS ; k++)
                          {                                      
                               me->afCPSBuf[FIRST_CPS_MEASURE_TIME - me->idxFilterCPS  + k] = me->affilterCPSBuf[k];
                          }

                          
                          if(me->nCPSBuf < FIRST_CPS_MEASURE_TIME)
                          {
                               if ((me->nCPSBuf = me->nCPSBuf + me->idxFilterCPS) > FIRST_CPS_MEASURE_TIME)
                               {
                                    me->nCPSBuf = FIRST_CPS_MEASURE_TIME;
                               }
                          }     
                          me->idxFilterCPS = 0;
                    }
             }



             SumCPS = 0;
             for( i = 0; i < 10; i++)
             {
                 SumCPS += me->afCPSBuf[FIRST_CPS_MEASURE_TIME - 10 + i]; 
             }
             AverCPS = SumCPS / 10;

             if (AverCPS >= 1.2)  //大概17us
             {
                  DoubtCnt++;
             }
             else
             {
                 DoubtCnt = 0;
             }



            if (me->afCPSBuf[FIRST_CPS_MEASURE_TIME- 1] >= MIDDLE_DOSE_RATE_THRE 
                && me->afCPSBuf[FIRST_CPS_MEASURE_TIME- 2] >= MIDDLE_DOSE_RATE_THRE)
            {
                DoubtCnt = 0;
                me->byDoseRateMode = HIGH_DOSE_RATE_MODE;
                me->byLowDRCPSBufThre = 1;
                me->nSwitchThr =5;
                AverCPS = me->afCPSBuf[FIRST_CPS_MEASURE_TIME- 1];

                me->byVariWin = 2;
                memset((void *)me->fLowDRCpsBuf, 0, LOW_DOSE_RATE_MODE_CPS_FITERBUF_SZIE * sizeof(float));
                me->iLowDRCPSBufIdx=0;
                memset((void *)me->fLowDRFiterBuf, 0, sizeof(me->fLowDRFiterBuf)/ sizeof(me->fLowDRFiterBuf[0]));                       
                me->iLowDRFiterBufIdx = 0;
                
                me->fLowDRLastCnt = me->afCPSBuf[FIRST_CPS_MEASURE_TIME - 1];  
                
                me->nLowCPS = 0;
                me->nHighCSP = 0;
                

                
            }
            else
            {


             if(DoubtCnt >= 5)
             {
                
                 DoubtCnt = 0;
                 me->byDoseRateMode = HIGH_DOSE_RATE_MODE;

        #if 1                   
                 SumCPS = 0; 
                 for (i = 0; i < 10; i++) 
                 {
                      SumCPS += me->afCPSBuf[FIRST_CPS_MEASURE_TIME - 10 + i];
                 }
                 AverCPS = SumCPS / i;
                 
                 me->byLowDRCPSBufThre  = 5;
                 me->nSwitchThr = 2;
        #else
                 if (me->afCPSBuf[FIRST_CPS_MEASURE_TIME- 1] >= MIDDLE_DOSE_RATE_THRE)
                 {
                     me->byLowDRCPSBufThre = 1;
                     me->nSwitchThr =5;
                     AverCPS = me->afCPSBuf[FIRST_CPS_MEASURE_TIME- 1];
                 }
                 else
                 {
                     SumCPS = 0; 
                     for (i = 0; i < 1; i++)  // 原来 i < 5
                     {
                          SumCPS += me->afCPSBuf[FIRST_CPS_MEASURE_TIME - 1 + i];
                     }
                     AverCPS = SumCPS / i;

                     me->byLowDRCPSBufThre  = 5;
                     me->nSwitchThr = 2;
                 }

        #endif
                      
                 if (me->byDoseRateMode == HIGH_DOSE_RATE_MODE)
                 {
                     me->byVariWin = 1;
                     memset((void *)me->fLowDRCpsBuf, 0, LOW_DOSE_RATE_MODE_CPS_FITERBUF_SZIE * sizeof(float));
                     me->iLowDRCPSBufIdx=0;
                     memset((void *)me->fLowDRFiterBuf, 0, sizeof(me->fLowDRFiterBuf)/ sizeof(me->fLowDRFiterBuf[0]));                       
                     me->iLowDRFiterBufIdx = 0;
                     
                     me->afCPSBuf[FIRST_CPS_MEASURE_TIME - 1] = AverCPS;
                     me->fLowDRLastCnt = me->afCPSBuf[FIRST_CPS_MEASURE_TIME - 1];  
                     
                     me->nLowCPS = 0;
                     me->nHighCSP = 0;
                    // me->nSwitchThr = 2;
                    ldtohdidx = 0;
                 }    
                      
              }
            }
            
         }
         else if (me->byDoseRateMode == HIGH_DOSE_RATE_MODE)
         {

               
               if (me->cpsDoseCount < LOW_CPS_THRE && me->byLowDRCPSBufThre == 5)
               {
                    me->nHighCSP = 0;
                    if (++me->nLowCPS >= 3);
                    {
                         me->nSwitchThr = 4;
                         me->nLowCPS = 0;
                    }
                   
               }
               else if (me->cpsDoseCount >= LOW_CPS_THRE  && me->byLowDRCPSBufThre == 5)
               {   
                   me->nLowCPS = 0;
                   if (++me->nHighCSP >= 3)
                   {
                       me->nSwitchThr = 2;
                       me->nHighCSP = 0;
                   }
                   
               }
               else
               {
                    me->nLowCPS  = 0;
                    me->nHighCSP = 0;
               }

               
               if (me->cpsDoseCount < 1)
               {
               
                    if(++me->iGotoLowDRCnt >= 15)
                    {
                       /* 连续6秒CPS < 1 回到低剂量(每秒CPS < 1)先快速回到本地。把缓冲区清0*/
                        memset((void *)me->afCPSBuf, 0, sizeof(me->afCPSBuf));
                        me->byDoseRateMode = LOW_DOSE_RATE_MODE;  
                        DoubtCnt = 0;
                        //bFind = 0;
                        me->iGotoLowDRCnt  = 0;
                        me->idxFilterCPS = 0;
                        me->bSwitchFlag = 1;                        
                        me->byVariWin =120;

                    
                    }
                
                  
               }
               else
               {
                   me->iGotoLowDRCnt = 0;
                   me->bSwitchFlag = 0;
               }


               if (me->bSwitchFlag == 0)
               {
                      
                     if (me->byLowDRCPSBufThre == 1)
                     {

                         AverCPS = me->cpsDoseCount;
                         memcpy((void *)me->afCPSBuf, (void *)(&me->afCPSBuf[1]), (FIRST_CPS_MEASURE_TIME - 1) * sizeof me->afCPSBuf[0]);
                         me->afCPSBuf[FIRST_CPS_MEASURE_TIME - 1] = AverCPS;                           
                         if (me->byVariWin < LOW_DOSE_RATE_MODE_MAX_WIN)
                         {
                             me->byVariWin++;
                         }
                         
                        // me->fLowDRLastCnt = RawCPS;

                         if (AverCPS > (me->fLowDRLastCnt + me->fLowDRLastCnt * SWITCH_THR))
                         {                            
                             if (me->bLowDRFlage == 1)
                             {
                                 me->iLowDRFiterBufIdx = 0;
                             }
                             me->bLowDRFlage = 2;
                             me->fLowDRFiterBuf[me->iLowDRFiterBufIdx] = AverCPS;
                             me->iLowDRFiterBufIdx++;   
                         }
                         else if (AverCPS < (me->fLowDRLastCnt - me->fLowDRLastCnt * SWITCH_THR))
                         {
                              if (me->bLowDRFlage == 2)
                              {
                                   me->iLowDRFiterBufIdx = 0;
                              }
                              me->bLowDRFlage = 1;
                              me->fLowDRFiterBuf[me->iLowDRFiterBufIdx] = AverCPS;
                              me->iLowDRFiterBufIdx++;
                         }
                         else 
                         {
                              me->iLowDRFiterBufIdx = 0;
                              me->bLowDRFlage = 0;
                         }


                        if (me->iLowDRFiterBufIdx == me->nSwitchThr)
                        {
                            // printf("siwtch:%.3f,%.3f,%.3f\n", me->fLowDRLastCnt,me->afCPSBuf[FIRST_CPS_MEASURE_TIME - 1],AverCPS);
                            
                            SumCPS = 0;
                            for(i = 0; i <  me->nSwitchThr ; i++)
                            {
                                SumCPS += me->fLowDRFiterBuf[i];
                            }
                            AverCPS = SumCPS / i;

                            if (AverCPS < MIDDLE_DOSE_RATE_THRE)
                            {
                                me->byLowDRCPSBufThre = 5; // switch low dose

                                me->byVariWin = 1;
                                
                                memset((void *)me->fLowDRFiterBuf, 0, sizeof(me->fLowDRFiterBuf)/ sizeof(me->fLowDRFiterBuf[0]));                       
                                me->iLowDRFiterBufIdx = 0;

                                memset((void *)me->fLowDRCpsBuf, 0, LOW_DOSE_RATE_MODE_CPS_FITERBUF_SZIE * sizeof(float));
                                me->iLowDRCPSBufIdx=0;
                                
                                me->afCPSBuf[FIRST_CPS_MEASURE_TIME - 1] = AverCPS;
                                me->fLowDRLastCnt = me->afCPSBuf[FIRST_CPS_MEASURE_TIME - 1];  
                                
                                me->nLowCPS = 0;
                                me->nHighCSP = 0;
                                ldtohdidx =0;
                                
                                
                            }
                            else
                            {
                                me->byVariWin = 5;                                
   
                                memcpy((void *)&me->afCPSBuf[FIRST_CPS_MEASURE_TIME - me->nSwitchThr], &me->fLowDRFiterBuf[0], sizeof(me->fLowDRFiterBuf[0]) * me->nSwitchThr);
                                me->fLowDRLastCnt     = me->afCPSBuf[FIRST_CPS_MEASURE_TIME - 1];
                                memset((void *)me->fLowDRFiterBuf, 0, sizeof (me->fLowDRFiterBuf));
                                me->iLowDRFiterBufIdx = 0;
                                me->bLowDRFlage = 0;
                            }
                        
                         }
                         else
                         {
                              if(me->iLowDRFiterBufIdx == 0)
                              {
                                  fTmp = AverCPS;
                                  memcpy((void *)me->afCPSBuf, (void *)(&me->afCPSBuf[1]), (FIRST_CPS_MEASURE_TIME - 1) * sizeof me->afCPSBuf[0]);
                                  me->afCPSBuf[FIRST_CPS_MEASURE_TIME - 1] = fTmp;  
                                  me->fLowDRLastCnt = fTmp;
                              
                              }
                         }

                             
                     }
                     else  // me->byLowDRCPSBufThre == 5
                     {
                         me->fLowDRCpsBuf[me->iLowDRCPSBufIdx] = me->cpsDoseCount;

                         if (me->cpsDoseCount < MIDDLE_DOSE_RATE_THRE)
                         {
                            ldtohdidx = 0;
                         }

                         else
                         {
                            LDtoHDBuf[ldtohdidx++] = me->cpsDoseCount;
                         }

                         if (ldtohdidx >=3)
                         {
                            
                              me->byLowDRCPSBufThre = 1; //switch hig dose                                                                         
                              me->byVariWin = 3;
                              memcpy((void *)&me->afCPSBuf[FIRST_CPS_MEASURE_TIME - (sizeof(LDtoHDBuf) / sizeof(LDtoHDBuf[0]))], (void *)LDtoHDBuf, (sizeof(LDtoHDBuf) / sizeof(LDtoHDBuf[0])));
                              
                              me->fLowDRLastCnt = me->afCPSBuf[FIRST_CPS_MEASURE_TIME - 1];  
                              memset((void *)me->fLowDRFiterBuf, 0, sizeof(me->fLowDRFiterBuf)/ sizeof(me->fLowDRFiterBuf[0]));                       
                              me->iLowDRFiterBufIdx = 0;
                              me->nLowCPS = 0;
                              me->nHighCSP = 0;
                              ldtohdidx = 0;
                              
                         }
                         else
                         {
 
                            if (++me->iLowDRCPSBufIdx >=  me->byLowDRCPSBufThre )
                             {         
                                  if (me->byVariWin < LOW_DOSE_RATE_MODE_MAX_WIN)
                                  {
                                      me->byVariWin++;
                                  }
                                  SumCPS = 0;
                                  for(i = 0; i <  me->byLowDRCPSBufThre ; i++)
                                  {
                                      SumCPS += me->fLowDRCpsBuf[i];
                                  }
                                  AverCPS = SumCPS / me->byLowDRCPSBufThre;
                                  
                                //  printf("five aver:%.3f\n",AverCPS);
                                  
                                  if (AverCPS > (me->fLowDRLastCnt + me->fLowDRLastCnt * SWITCH_THR))
                                  {
                                       if (me->bLowDRFlage == 1)
                                       {
                                           me->iLowDRFiterBufIdx = 0;
                                       }
                                       me->bLowDRFlage = 2;
                                       me->fLowDRFiterBuf[me->iLowDRFiterBufIdx] = AverCPS;
                                       me->iLowDRFiterBufIdx++;
                                  }
                                  else if (AverCPS < (me->fLowDRLastCnt - me->fLowDRLastCnt * SWITCH_THR))
                                  {
                                       if (me->bLowDRFlage == 2)
                                       {
                                            me->iLowDRFiterBufIdx = 0;
                                       }
                                       me->bLowDRFlage = 1;
                                       me->fLowDRFiterBuf[me->iLowDRFiterBufIdx] = AverCPS;
                                       me->iLowDRFiterBufIdx++;
                                  }
                                  else 
                                  {
                                       me->iLowDRFiterBufIdx = 0;
                                       me->bLowDRFlage = 0;
                                  }
                                  
                                      
                                  if (me->iLowDRFiterBufIdx == me->nSwitchThr )
                                  {
                                     //  printf("siwtch:%.3f,%.3f,%.3f\n", me->fLowDRLastCnt,me->afCPSBuf[FIRST_CPS_MEASURE_TIME - 1],AverCPS);
   
                                       AverCPS =  (me->fLowDRFiterBuf[0] + me->fLowDRFiterBuf[1]) / 2;
                                       if (AverCPS >= MIDDLE_DOSE_RATE_THRE)
                                       {
                                            me->byLowDRCPSBufThre = 1; //switch hig dose                                                                         
                                            me->byVariWin = 2;
                                            memcpy((void *)&me->afCPSBuf[FIRST_CPS_MEASURE_TIME - 2], (void *)me->fLowDRFiterBuf, sizeof(me->fLowDRFiterBuf[0]) * 2);
   
                                            me->fLowDRLastCnt = me->afCPSBuf[FIRST_CPS_MEASURE_TIME - 1];  
   
                                            memset((void *)me->fLowDRFiterBuf, 0, sizeof(me->fLowDRFiterBuf)/ sizeof(me->fLowDRFiterBuf[0]));                       
                                            me->iLowDRFiterBufIdx = 0;
                                            me->nLowCPS = 0;
                                            me->nHighCSP = 0;
                                              
                                            
                                       }
                                       else
                                       {
                                            me->byVariWin = 1;
                                               
                                            me->afCPSBuf[FIRST_CPS_MEASURE_TIME - 1] = (me->fLowDRFiterBuf[0] + me->fLowDRFiterBuf[1]) / 2;            
                                            me->fLowDRLastCnt =  me->afCPSBuf[FIRST_CPS_MEASURE_TIME - 1];
                                               
                                            me->fLowDRFiterBuf[0] = 0;
                                            me->fLowDRFiterBuf[1] = 0;
                                            me->iLowDRFiterBufIdx = 0;
                                               
                                            me->bLowDRFlage = 0;
                                       }
                          
                                   }
                                   else
                                   {
                                        if(me->iLowDRFiterBufIdx == 0);
                                        {
                                            fTmp = AverCPS;
                                            memcpy((void *)me->afCPSBuf, (void *)(&me->afCPSBuf[1]), (FIRST_CPS_MEASURE_TIME - 1) * sizeof me->afCPSBuf[0]);
                                            me->afCPSBuf[FIRST_CPS_MEASURE_TIME - 1] = fTmp;  
                                            me->fLowDRLastCnt = fTmp;
                                        
                                        }
                                   }
                                  
                                  me->iLowDRCPSBufIdx = 0;  
                             }// if (++me->iLowDRCPSBufIdx >= 5)
                        }//if (ldtohdidx >=3)
                     }
     
              } 
          }
                 
       /* 计算平均CPS */   
       {
         /*晚上发个老毕的*/
         
         if (me->byDoseRateMode == HIGH_DOSE_RATE_MODE)
         {   
             
             SumCPS = 0;
             Max = 0;
           //  printf("buf win size:%d\n",me->byVariWin);
             
             for(i = 0; i< me->byVariWin;i++)
             {
                 if (me->afCPSBuf[FIRST_CPS_MEASURE_TIME - me->byVariWin + i] > Max)
                 {
                     Max = me->afCPSBuf[FIRST_CPS_MEASURE_TIME - me->byVariWin  + i];
                 }                     
                 SumCPS +=me->afCPSBuf[FIRST_CPS_MEASURE_TIME - me->byVariWin + i];
                 
             }
           //  printf("\n");


            if (me->nSwitchThr == 4 && me->byVariWin >= LOW_DOSE_RATE_MODE_MAX_WIN)
            {
                 iMinCnt = 0;
                 for(i = 0; i< LOW_DOSE_RATE_MODE_MAX_WIN; i++)
                 {
                      if (me->afCPSBuf[FIRST_CPS_MEASURE_TIME - LOW_DOSE_RATE_MODE_MAX_WIN + i] == Max)
                      {
                         iMinCnt++;
                      }
                 }
                 if (iMinCnt <= 3)//如果最大计数有3个以下，这个最大计数就有可能是一个异常值，所以需要去掉 2016.3.25
                 {
                     SumCPS = SumCPS - iMinCnt * Max;
                     fAverCPS = SumCPS / (LOW_DOSE_RATE_MODE_MAX_WIN - iMinCnt);
                 } 
                 else
                 {
                     fAverCPS = SumCPS / LOW_DOSE_RATE_MODE_MAX_WIN ;                        
                 }

                
                 
            }
            else
            {
                 if (me->byVariWin >= LOW_DOSE_RATE_MODE_MAX_WIN)
                 {
                       fAverCPS = SumCPS / LOW_DOSE_RATE_MODE_MAX_WIN;
                       fAverCPS = (SumCPS - Max + fAverCPS) /LOW_DOSE_RATE_MODE_MAX_WIN;
                 }
                 else
                 {
                      if (me->byVariWin > 0)
                      {       
                          fAverCPS = SumCPS / me->byVariWin;
                      }
                      else
                      {
                          fAverCPS = 0;
                      }
 
                 }
            }


            #if 1

                if(me->iLowDRCPSBufIdx > 0)
                {
                    int s;
                    s =rand()% 9;
                    if ( s > 4)
                    {
                        fAverCPS = fAverCPS + s/1000.0;
                    }
                    else
                    {
                        fAverCPS = fAverCPS - s/1000.0;
                    }

                }
            #endif                
         }
         else
         {    
            
              SumCPS = 0;
              for( i = 0; i < me->byVariWin; i++)
              {
                
                 SumCPS+= me->afCPSBuf[FIRST_CPS_MEASURE_TIME - me->byVariWin + i];
              }
              fAverCPS = SumCPS / i;

         }
       }

    //fAverCPS = 20.8;
    
    me->fDoseRate = QEpdDoseRateCalc();//me->cpsDoseCount/10000.0f;//QEpdDoseRateCalc(me,fAverCPS,0);
    me->fDoseRate *= me->EpdPara.baseConfig.iScaleFactor;
    me->fDoseSumx += (me->fDoseRate/3600.0f);
        
    me->EpdPara.statusInfo.iLastDoseSum = me->fDoseSumx;
    
    if(me->fDoseRate > me->EpdPara.statusInfo.iMaxDoseRate) 
    {
        me->EpdPara.statusInfo.iMaxDoseRate = me->fDoseRate;
    }

    //me->fDoseRate = 10.2;


    

    /*me->scaleDoseRate  = QEpdDoseRateCalc(me,fAverCPS,0);
    me->scaleDoseRate *= me->EpdPara.baseConfig.iScaleFactor;
    
    me->scaleSumDose   += (me->scaleDoseRate/3600);//msv/s
    me->scaleSumDoseCom = me->scaleSumDose*1000;//2012.10.22  μ
    me->scaleDoseRateCom= me->scaleDoseRate*1000;*/

    
    //me->fDoseRate = 0x00;//me->EpdPara.baseConfig.iScaleFactor;
    //me->fDoseSumx = 0x00;//(me->fDoseRate/3600);
    //me->EpdPara.statusInfo.iMaxDoseRate = me->fDoseRate;//导致数据不准    
    //me->EpdPara.statusInfo.iLastDoseSum = rand()/1000.0f;//me->fDoseSumx;//导致数据不准
    #endif    
}


#endif

#endif

