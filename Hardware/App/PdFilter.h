#ifndef _PDFILTER_H_
#define _PDFILTER_H_


#define PAN_FILTER 
#define BK_CPS  0.008   // 本地计数
#define NEU_AV_COUNT                  30   //中子定时长平均值数量

//#define FIRST_CPS_MEASURE_TIME        120  /* background meausre time in s *///原先是100
//#define LOW_DOSE_RATE_MODE_CPS_FITERBUF_SZIE     5
#define NEU_AV_COUNT                  30   //中子定时长平均值数量

typedef struct STU_CALC_DOSERATE
{
    double fDoseRate;    //剂量当量率 mSv/h
    double fDoseSumx;   //累积剂量   mSv
    
    //float    iLastDoseSum;        //上次剂量当量,浮点型,累积剂量当量
    //float    iMaxDoseRate;        //最大剂量当量率，浮点型
    
    uint32_t timerCnt;            // 计数时间
    uint32_t sumDoseCount;        //累计剂量计数
    uint32_t cpsDoseCount;        //CPS计数
    uint32_t cpsDosePerCnt;        //30秒CPS计数
    
    int DoubtCnt;

    /* xhliu add 2013-10-18 */
    float afCPSBuf[FIRST_CPS_MEASURE_TIME];
    #ifdef PAN_FILTER
    float BdAvCps[LOW_DOSE_RATE_MODE_CPS_FITERBUF_SZIE];    // 本地平均CPS
    
    //float BigCPSBuf[MIN_FILTER_WIN_SIZE];
    //float SmCPSBuf[MIN_FILTER_WIN_SIZE];
    //BYTE  BigCnt;
    //BYTE  SmCnt;
    //BYTE  ConnCpsTime;       // 连续有计数时间
    BYTE  NoCpsTime;         // 连续无计数时间
    
    float Av5sCps;    // 5秒平均计数
    float Av10sCps;   // 10秒平均计数
    float Av30sCps;   // 30秒平均计数
    float Av60sCps;   // 60秒平均计数
    float Av120sCps;  // 120秒平均计数
    float Av600sCps; // 600秒平均计数
    
    uint8_t  bBufFull;
    float    fCpsHis;
    float    fCpsMax;
    float    fCpsLast;  // 最后一次突变后的计数
    #else
    float affilterCPSBuf[MIN_FILTER_WIN_SIZE];
    #endif
    
    uint16_t nCPSBuf;   // cps 缓冲数量
    
    
    // pgy add 2023.8.23  中子定时长累计计数缓存
    float afNeuAvBuf[NEU_AV_COUNT];  
    uint16_t nNeuCpsTime;  // 中子计数时间
    uint16_t nNeuZeroTime;  // 中子0计数时间
    
    
    uint8_t byDoseRateMode;  // 0: low dose rate 1: high dose rate
    uint8_t byVariWin;

    float fLowDRCpsBuf[LOW_DOSE_RATE_MODE_CPS_FITERBUF_SZIE];
    float fLowDRFiterBuf[5];
    int   iLowDRFiterBufIdx;
    int   iLowDRCPSBufIdx;
    float fLowDRLastCnt;
    
    uint8_t bLowDRFlage;
    uint8_t byLowDRCPSBufThre;    
    uint8_t  bSwitchFlag;           //0: 还没有切换，1:低切换，2:高切换
    uint8_t iGotoLowDRCnt;

    //int   iDR;
    //int   iGotoLDR;
    //float fGotoLDRSumCPS;
    
    //unsigned int idxFilterCPS;
    uint8_t nSwitchThr;
    uint8_t nLowCPS; 
    uint8_t nHighCSP;
    uint8_t timerCPSCnt;            //CPS, 30秒计数
    
    /* xhliu add for new dose rate algorithm 2013/08/15 */
    uint32_t  idxFilterCPS;
    //uint32_t  nCPSQue;
    //uint32_t  nScaleTOCircle;
    //uint32_t  nLowCount;
    //uint32_t  nHighCunt;
    //uint32_t  dwCPSCircle;
    float LDtoHDBuf[3];
    uint8_t  ldtohdidx;
}tag_CALC_DOSERATE;


void QEpdDoseRateUpdate(QEpd *pEpd, tag_CALC_DOSERATE *me,
                                        float fScaleFactor, bool getMaxAndSum);
void InitArr(tag_CALC_DOSERATE* me, float cps);


#endif



