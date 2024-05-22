#include "math.h"
#include "main.h"
#include "PdFilter.h"
#include "PDoseRateCalc.h"

extern float fAverCPS;

void InitArr(tag_CALC_DOSERATE* me, float cps)
{
    BYTE i;

    memset(me->afCPSBuf, 0, FIRST_CPS_MEASURE_TIME * 4);
    for (i = 0; i < FIRST_CPS_MEASURE_TIME; i++)
    {
        me->afCPSBuf[i] = cps;
    }
    me->bBufFull = TRUE;
    me->nCPSBuf = FIRST_CPS_MEASURE_TIME;
    me->fCpsHis = cps;
    me->fCpsMax = cps;
    me->fCpsLast = cps;

    //me->NoCpsTime = 0;         // 连续无计数时间

    me->timerCnt = FIRST_CPS_MEASURE_TIME * LOW_DOSE_RATE_MODE_CPS_FITERBUF_SZIE;

    for (i = 0; i < LOW_DOSE_RATE_MODE_CPS_FITERBUF_SZIE; i++)
    {
        me->BdAvCps[i] = cps;
    }

    me->Av5sCps = cps;
    me->Av10sCps = cps;
    me->Av30sCps = cps;
    me->Av60sCps = cps;
    me->Av120sCps = cps;
    me->Av600sCps = cps;
}


void FullArr(tag_CALC_DOSERATE* me, float cps)
{
    #define FULL_CNT  1
    BYTE i;

    if (cps < BK_CPS)
    {
        cps = BK_CPS;
    }

    memset(me->afCPSBuf, 0, FIRST_CPS_MEASURE_TIME * 4);
    for (i = 0; i < FULL_CNT; i++)
    {
        me->afCPSBuf[i] = cps;
    }
    me->bBufFull = FALSE;
    me->nCPSBuf = FULL_CNT;
    me->fCpsHis = cps;
    me->fCpsMax = cps;
    me->fCpsLast = cps;


    //me->NoCpsTime = 0;         // 连续无计数时间

    me->timerCnt = FULL_CNT;

    me->Av5sCps = cps;
    me->Av10sCps = cps;
    me->Av30sCps = cps;
    me->Av60sCps = cps;
    me->Av120sCps = cps;
    me->Av600sCps = cps;
}


void ClearArr(tag_CALC_DOSERATE* me)
{
    memset(me->afCPSBuf, 0, FIRST_CPS_MEASURE_TIME * 4);
 
    me->bBufFull = FALSE;
    me->nCPSBuf = 0;
    me->fCpsHis = 0;
    me->fCpsMax = 0;
    me->fCpsLast = 0;
    //me->NoCpsTime = 0;         // 连续无计数时间

    me->timerCnt = 0;

    me->Av5sCps = 0;
    me->Av10sCps = 0;
    me->Av30sCps = 0;
    me->Av60sCps = 0;
    me->Av120sCps = 0;
    me->Av600sCps = 0;
}



float LastTimeAv(tag_CALC_DOSERATE *me, BYTE time)
{
    float ret = 0.0;
    BYTE i;
    
    for (i=FIRST_CPS_MEASURE_TIME-time;i<FIRST_CPS_MEASURE_TIME;i++)
    {
        ret +=  me->afCPSBuf[i];
    }
    return (ret/time);
}

// 缓冲区满后：计算最近平均值
void CalcLastAv(tag_CALC_DOSERATE *me)
{
    me->Av120sCps = LastTimeAv(me, 120);
    me->Av60sCps  = LastTimeAv(me, 60);
    me->Av30sCps  = LastTimeAv(me, 30);
    me->Av10sCps  = LastTimeAv(me, 10);
    me->Av5sCps   = LastTimeAv(me, 5);
}

float NearTimeAv(tag_CALC_DOSERATE *me, BYTE time)
{
    float ret = 0.0;
    BYTE i;

    if (me->nCPSBuf < time)
    {
        // 取所有的数据
        for (i=0;i<me->nCPSBuf;i++)
        {
            ret +=  me->afCPSBuf[i];
        }

        return (ret/me->nCPSBuf);
    }
    else
    {
        // 取最后的数
        for (i=me->nCPSBuf-time;i<me->nCPSBuf;i++)
        {
            ret +=  me->afCPSBuf[i];
        }
        return (ret/time); 
    }
}


// 缓冲区没有满：计算最近平均值
void CalcNearAv(tag_CALC_DOSERATE *me)
{
    me->Av5sCps  = NearTimeAv(me, 5);
    me->Av10sCps = NearTimeAv(me, 10);
    me->Av30sCps = NearTimeAv(me, 30);
    me->Av60sCps = NearTimeAv(me, 60);
    me->Av120sCps = NearTimeAv(me, 120);
}


// 计数平滑 120秒
float CounterPH(tag_CALC_DOSERATE *me)
{    
    float Val = 0.0;
    BYTE i;
    
    Val = 0.0;
    if (me->bBufFull)
    {
        for (i=0;i<FIRST_CPS_MEASURE_TIME;i++)
        {
            Val += me->afCPSBuf[i];
        }
        Val = Val/FIRST_CPS_MEASURE_TIME;
    }
    else
    {
        if (me->nCPSBuf == 0)
        {
            me->fCpsHis = BK_CPS;
            return 0;
        }
        else
        {
            for (i=0;i<me->nCPSBuf;i++)
            {
                Val += me->afCPSBuf[i];
            }
            Val = Val/me->nCPSBuf;
        }
    }

    if (Val < BK_CPS)
    {
        Val = BK_CPS;
    }
    
    me->fCpsHis = Val;
    return Val;
}


// 本底平滑
float BdPH(tag_CALC_DOSERATE* me)
{
    float r = 0.0;
    BYTE i, c;
    c = me->timerCnt / FIRST_CPS_MEASURE_TIME;
    if (c > 0)
    {
        for (i = 0; i < c; i++)
        {
            r += me->BdAvCps[i];
        }

        // 把最近的值平滑进去，不然2分钟剂量率不动
        r += CounterPH(me);
        r = r / (c + 1);
    }
    else
    {
        r = CounterPH(me);
    }
    
    return r;
}


void PushCps(tag_CALC_DOSERATE* me, float cps)
{
    BYTE i, j;

    if (cps == 0)
    {
        me->NoCpsTime ++;
    }
    else
    {
        me->NoCpsTime = 0;         // 连续无计数时间
    }

    if (me->nCPSBuf >= FIRST_CPS_MEASURE_TIME)  // 缓冲区满了
    {
        me->bBufFull = TRUE;

        // 把数据往前移动
        for (i = 1; i < FIRST_CPS_MEASURE_TIME; i++)
        {
            me->afCPSBuf[i - 1] = me->afCPSBuf[i];
        }
        //新数据放在最后
        me->afCPSBuf[FIRST_CPS_MEASURE_TIME - 1] = cps;

        CalcLastAv(me);
    }
    else
    {
        me->afCPSBuf[me->nCPSBuf++] = cps;

        CalcNearAv(me);
    }

    me->timerCnt++;
    if (me->timerCnt > 0)
    {
        j = me->timerCnt / FIRST_CPS_MEASURE_TIME;
        if ((me->timerCnt % FIRST_CPS_MEASURE_TIME) == 0)
        {
            me->BdAvCps[j - 1] = CounterPH(me);
        }

        if (me->timerCnt >= FIRST_CPS_MEASURE_TIME * LOW_DOSE_RATE_MODE_CPS_FITERBUF_SZIE)
        {
            // 最后一个
            me->timerCnt -= FIRST_CPS_MEASURE_TIME;
            for (i = 1; i < LOW_DOSE_RATE_MODE_CPS_FITERBUF_SZIE; i++)
            {
                me->BdAvCps[i - 1] = me->BdAvCps[i];
            }
        }

        me->Av600sCps = BdPH(me);
    }
}


float GetLast(tag_CALC_DOSERATE* me)
{
    if (me->bBufFull)
    {
        return me->afCPSBuf[FIRST_CPS_MEASURE_TIME-1];
    }

    return me->afCPSBuf[me->nCPSBuf-1];
}



// 快速上升
void FastUp(tag_CALC_DOSERATE* me)
{
    float last;
    if (me->Av10sCps > (me->fCpsLast + 3*sqrt(me->fCpsLast)) )
    {
        last = GetLast(me);
        ClearArr(me);
        
        // 把最后1个计数存入缓存
        PushCps(me, last);
        me->fCpsLast = me->fCpsHis;
    }
}

// 小剂量慢速上升
void LowUp(tag_CALC_DOSERATE* me)
{
    if (me->Av60sCps > (me->fCpsLast * 3) )
    {
        InitArr(me, me->Av30sCps);
    }
}


void UpFilter(tag_CALC_DOSERATE* me)
{
    if (me->Av5sCps >= 3)  // 33uSv/h
    {
        FastUp(me);
        me->fCpsHis = me->Av60sCps;
    }
    if (me->Av5sCps >= 1)  // 11uSv/h
    {
        FastUp(me);
        me->fCpsHis = me->Av120sCps;
    }
    else
    {
        if (me->Av60sCps >= 0.1)  // 1uSv/h
        {
            LowUp(me);
            me->fCpsHis = me->Av120sCps;
        }
        else
        {
            // 本底用600s平滑
            me->fCpsHis =  me->Av600sCps;
        }
    }

    if (me->fCpsHis > me->fCpsMax)
    {
        me->fCpsMax = me->fCpsHis;
    }
}


void DownFilter(tag_CALC_DOSERATE* me)
{
    if (me->fCpsMax > 10)
    {
        if (me->Av5sCps < me->fCpsMax - 3*sqrt(me->fCpsMax))
        {
            FullArr(me, me->Av5sCps);
        }

        if (me->NoCpsTime >= 3)
        {
            InitArr(me, BK_CPS);
        }
    }
    else if (me->fCpsMax >= 2)
    {
        //FastDown(me);
        if (me->NoCpsTime > 5)
        {
            InitArr(me, BK_CPS);
        }
    }
    else if (me->fCpsMax >= 1)
    {
        if (me->Av10sCps == 0)
        {
            InitArr(me, BK_CPS);
        }
    }
    else if (me->fCpsMax >= 0.1)
    {
        if (me->Av30sCps == 0)
        {
            InitArr(me, BK_CPS);
        }
    }
    else if (me->fCpsMax >= 0.05)
    {
        if (me->Av60sCps == 0)
        {
            InitArr(me, BK_CPS);
        }
    }
}



void CpsFilter(tag_CALC_DOSERATE *me, DWORD Cps)
{
    PushCps(me,Cps);

    #if 1
    UpFilter(me);
    DownFilter(me);
    #else
    // 做温度试验，600秒
    me->fCpsHis =  me->Av600sCps;
    #endif
}


void QEpdDoseRateUpdate(QEpd *pEpd, tag_CALC_DOSERATE *me, 
                                        float fScaleFactor, bool getMaxAndSum)
{
    me->cpsDoseCount = (uint32_t)SysParam.s_DoseRate.Cps1;
    CpsFilter(me, me->cpsDoseCount);
    fAverCPS = me->fCpsHis;
    
    me->fDoseRate = QEpdDoseRateCalc();
    me->fDoseRate *= fScaleFactor;
    
    if(getMaxAndSum)
    {
        me->fDoseSumx += (me->fDoseRate/3600.0f);
       
        //pEpd->EpdPara.statusInfo.yLastDoseSum = me->fDoseSumx;
        
        if(me->fDoseRate > pEpd->EpdPara.statusInfo.iMaxDoseRate) 
        {
            pEpd->EpdPara.statusInfo.iMaxDoseRate = me->fDoseRate;

        }
    }
}
