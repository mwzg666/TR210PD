#ifndef _PDFILTER_H_
#define _PDFILTER_H_


#define PAN_FILTER 
#define BK_CPS  0.008   // ���ؼ���
#define NEU_AV_COUNT                  30   //���Ӷ�ʱ��ƽ��ֵ����

//#define FIRST_CPS_MEASURE_TIME        120  /* background meausre time in s *///ԭ����100
//#define LOW_DOSE_RATE_MODE_CPS_FITERBUF_SZIE     5
#define NEU_AV_COUNT                  30   //���Ӷ�ʱ��ƽ��ֵ����

typedef struct STU_CALC_DOSERATE
{
    double fDoseRate;    //���������� mSv/h
    double fDoseSumx;   //�ۻ�����   mSv
    
    //float    iLastDoseSum;        //�ϴμ�������,������,�ۻ���������
    //float    iMaxDoseRate;        //�����������ʣ�������
    
    uint32_t timerCnt;            // ����ʱ��
    uint32_t sumDoseCount;        //�ۼƼ�������
    uint32_t cpsDoseCount;        //CPS����
    uint32_t cpsDosePerCnt;        //30��CPS����
    
    int DoubtCnt;

    /* xhliu add 2013-10-18 */
    float afCPSBuf[FIRST_CPS_MEASURE_TIME];
    #ifdef PAN_FILTER
    float BdAvCps[LOW_DOSE_RATE_MODE_CPS_FITERBUF_SZIE];    // ����ƽ��CPS
    
    //float BigCPSBuf[MIN_FILTER_WIN_SIZE];
    //float SmCPSBuf[MIN_FILTER_WIN_SIZE];
    //BYTE  BigCnt;
    //BYTE  SmCnt;
    //BYTE  ConnCpsTime;       // �����м���ʱ��
    BYTE  NoCpsTime;         // �����޼���ʱ��
    
    float Av5sCps;    // 5��ƽ������
    float Av10sCps;   // 10��ƽ������
    float Av30sCps;   // 30��ƽ������
    float Av60sCps;   // 60��ƽ������
    float Av120sCps;  // 120��ƽ������
    float Av600sCps; // 600��ƽ������
    
    uint8_t  bBufFull;
    float    fCpsHis;
    float    fCpsMax;
    float    fCpsLast;  // ���һ��ͻ���ļ���
    #else
    float affilterCPSBuf[MIN_FILTER_WIN_SIZE];
    #endif
    
    uint16_t nCPSBuf;   // cps ��������
    
    
    // pgy add 2023.8.23  ���Ӷ�ʱ���ۼƼ�������
    float afNeuAvBuf[NEU_AV_COUNT];  
    uint16_t nNeuCpsTime;  // ���Ӽ���ʱ��
    uint16_t nNeuZeroTime;  // ����0����ʱ��
    
    
    uint8_t byDoseRateMode;  // 0: low dose rate 1: high dose rate
    uint8_t byVariWin;

    float fLowDRCpsBuf[LOW_DOSE_RATE_MODE_CPS_FITERBUF_SZIE];
    float fLowDRFiterBuf[5];
    int   iLowDRFiterBufIdx;
    int   iLowDRCPSBufIdx;
    float fLowDRLastCnt;
    
    uint8_t bLowDRFlage;
    uint8_t byLowDRCPSBufThre;    
    uint8_t  bSwitchFlag;           //0: ��û���л���1:���л���2:���л�
    uint8_t iGotoLowDRCnt;

    //int   iDR;
    //int   iGotoLDR;
    //float fGotoLDRSumCPS;
    
    //unsigned int idxFilterCPS;
    uint8_t nSwitchThr;
    uint8_t nLowCPS; 
    uint8_t nHighCSP;
    uint8_t timerCPSCnt;            //CPS, 30�����
    
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



