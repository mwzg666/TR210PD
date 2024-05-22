#ifndef __MAIN_H__
#define __MAIN_H__

//#include "STC32G.h"
//#include "stdio.h"
//#include "intrins.h"
//#include "string.h"
//#include "stdlib.h"
//#include "config.h"

#include "appconfig.h"
//===================================����=================================


//#define DEBUG
#define USE_EPD_DOSE


#define MIB_CST_SETPARA_CNT 6
#define MIB_CST_SELPARA_NUM    13
#define MIB_CST_MAX_DISP_PARA     14

#define MAIN_Fosc        11059200UL    // 11.0592M
//#define MAIN_Fosc        35000000UL    // 35M

#define VERSION 101  

#define COUNTER_THRES  50000

#if 0
#define LOW_POWER_TIMER  10000  // ����͹��ĵ�ʱ�䣬���������
#define UPDATE_TIMER_TIMEOUT  1000  // in ms
#define ACTIVE_WAIT_TIME 15//in s,�����ȴ�ʱ��
#define TIME_OUT_RETURN_VALUE  10//in s, when time out return mesure disp

#define DOSERATE_OVERLOAD_VALUE        10000
#define DOSESUMX_OVERLOAD_VALUE        10000

//#define INCREASE_ALARM_VALUE        20    //�ۻ���������������,uSv,��ʱ�ú궨�壬�����ö����Ǹ�����������
/* �������� */
#define ALARM_TYPE_BATTERY_ALARM     (1<<0)        //��������
#define ALARM_TYPE_SUMDOSE_ALARM     (1<<1)        //������������
#define ALARM_TYPE_SUMDOSE_MEDDLE     (1<<2)        //��������Ԥ����
#define ALARM_TYPE_DOSERATE_ALARM     (1<<3)        //���������ʱ���
#define ALARM_TYPE_DOSERATE_MEDDLE     (1<<4)        //����������Ԥ��
#define ALARM_TYPE_USEDTIME_ALARM    (1<<5)        //�ۻ�ʱ�䱨��
#define ALARM_TYPE_SUMDOSE_OVERLOAD  (1<<6)        //�����������ر�����Overflow��
#define ALARM_TYPE_DOSERATE_OVERLOAD (1<<7)        //���������ʹ��ر�����Overflow��
/* ���ϲ��� */
#define FAULT_TYPE_INITIALIZE          (1<<0)        //��ʼ������
#define FAULT_TYPE_DETECTOR          (1<<1)        //̽ͷ����
#define FAULT_TYPE_DATA              (1<<2)        //���ݹ���
#define FAULT_TYPE_STORAGE              (1<<3)        //�洢����

/* ����ģʽ */
#define WORK_MODE_ALLOC                 ()
enum _ALARM_TYPE{
    MonDRSleepMode = 0,
    UpdateDoseRateTimer,
    UpdatePkuCountTimer,
    UpdatePkuCountTimer500ms,
    TimeOutTimmer,
    TimerA_,
    KeyTimer_,
    KeyAlarmDelay,
    TIMER_COUNT
};
#endif

#pragma pack(1)    //���ֽڶ���
typedef struct _QEpdDoseFactor
{
    //u32 dwDeviceNo;
    u8  experimentmode;//ʵ��ģʽ����(��CPS��) 1:�򿪣�0�ر�
    //u8  resv[3];
    //float lowfactor[16];//���ȱ�����821һ�£���֤ͨ��
    /*float lowfactor1;
    float lowfactor2;
    float lowfactor3;

    float midfactor0;
    float midfactor1;
    float midfactor2;
    float midfactor3;

    float highfactor0;    
    float highfactor1;
    float highfactor2;
    float highfactor3;

    float highestfactor0;    
    float highestfactor1;
    float highestfactor2;
    float highestfactor3; */ 
}QEpdDoseFactor,*PQEpdDoseFactor;

#if 0
//�豸������Ϣ
typedef struct _QEpdFactoryInfo{
    //���Ҳ���,ֻ��,���������
    char    szSerialNumber[16];    //�������
    char    szFactoryDate[8];    //����ʱ��
    char    szFirmwareVerNo[16];//�̼��汾
    char    szHardwareVerNo[16];//Ӳ���汾
}QEpdFactoryInfo,*PQEpdFactoryInfo;


//�豸������Ϣ
typedef struct _QEpdAssignInfo{
    char      szUserID[18];            //��ԱID��
    char      szUserName[8];            //��Ա����
    char      szTaskID[6];            //�����
    u32 dwAssignTime;            //�����Ʒ���ʱ��
}QEpdAssignInfo,*PQEpdAssignInfo;

#endif


//�豸״̬��Ϣ
typedef struct _QEpdStatusInfo
{
    //u16    iStorageMax;            //������ݴ洢����       
    //u16    iStorageUsed;            //ʹ�ô洢����        
    float    iLastDoseSum;                //�ϴμ�������,������,�ۻ���������
    float    iMaxDoseRate;                //�����������ʣ�������
    u32    iUsedTime;                    //����ʹ��ʱ�䣬�ۼƲ���ʱ��
    //u8    ucFaultFlag;
        /*
             0:    ��ʼ������
             1:    ̽ͷ����
             2:    ���ݹ���
             3:    �洢����        
        */
    //u8    ucAlarmFlag;
        /*
             0:    ��������
             1:    ������������
             2:    ��������Ԥ����
             3:    ���������ʱ���
             4:    ����������Ԥ����
             5:    �ۻ�ʱ�䱨��
             6:    �����������ر�����Overflow��
             7: ���������ʹ��ر�����Overflow��        
        */
    //u8    resv[2];
}QEpdStatusInfo,*PQEpdStatusInfo;



#pragma pack()

#if 0
//����ģʽ
/*
     0: �������Ƿ����
     1: ����ģʽ,0:���� 1: �ֶ�
     2: ����/�ر�״̬,0:δ����,1: ����
     3: ����ģʽ, 1:˯��ģʽ 0:����˯��ģʽ
 4 - 6: ����������
      7: LED��ʽ
*/
#define QEPD_PARAM_ASSIGNED_BIT        0x01
#define QEPD_PARAM_WORKMODE_BIT        0x02
#define QEPD_PARAM_ACTIVE_BIT        0x04
#define QEPD_PARAM_ENERYUSED_BIT    0x08
#define QEPD_PARAM_INTERVAL_BIT        0x10
#define QEPD_PARAM_LEDMODE_BIT        0x80

#define QEPD_PARAM_ASSIGNED_MASK    0xFE
#define QEPD_PARAM_WORKMODE_MASK    0xFD
#define QEPD_PARAM_ACTIVE_MASK        0xFB
#define QEPD_PARAM_ENERYUSED_MASK    0xF7
#define QEPD_PARAM_INTERVAL_MASK    0x8F
#define QEPD_PARAM_LEDMODE_MASK        0x7F

//ģʽ����
/*
     0: ��ʼ����״̬, 1:�������λ��0:���ֱ���λ
 1 - 2: ���ƴ洢ģʽ, 00:�رգ�01:����������10:����洢
 3 - 5: ���ƴ洢���, 000:15�룬001:30��         
 6 - 7: ���ⷽʽ
*/
        
#define QEPD_PARAM_CLEARALARM_BIT        0x01
#define QEPD_PARAM_TIGER_SAVE            0x02
#define QEPD_PARAM_INTER_SAVE            0x04
#define QEPD_PARAM_STORAGE_INTERVAL     0x80
#define QEPD_PARAM_BACKLIGHT_MODE        0x40

#define QEPD_PARAM_CLEARALARM_MASK        0x01
#define QEPD_PARAM_STORAGE_MODE_MASK    0x02
#define QEPD_PARAM_STORAGE_INTERVAL_MASK    0x08
#define QEPD_PARAM_BACKLIGHT_MODE_MASK        0x40

enum storageMode{
    storageMode_close = 0,
    storageMode_Trigger,
    storageMode_Interval
};

enum storageInterval{
    storageInterval_15Sec = 0,
    storageInterval_30Sec,
    storageInterval_1min,
    storageInterval_10min,
    storageInterval_30min,
    storageInterval_60min,
    storageInterval_90min,
    storageInterval_24hour
};

enum backLightMode{
    backLightMode_Close = 0,
    backLightMode_2sec,
    backLightMode_5sec,
    backLightMode_10sec
};

enum dispUnit{
    dispUnit_usv = 0,
    dispUnit_msv,
    dispUnit_sv
};

enum doseIncreaseAlarmType{
    noAlarm = 0,
    cps_4,
    cps_8,
    ucv_1,
    ucv_10,
    ucv_100
};

        /*   
         0 - 2: ͨѶ����
             3: �洢ģʽ
             4: Ĭ�ϲ���
         5 - 7: ������ʾ��λ
       */

#define QEPD_PARAM_COMM_SPEED            0x01
#define QEPD_PARAM_STORAGE_MODE_BIT     0x02
#define QEPD_PARAM_DEFAULT_MEASURE        0x08
#define QEPD_PARAM_DISPLAY_UNIT            0x20

#define QEPD_PARAM_CLEARALARM_MASK        0x01
#define QEPD_PARAM_DEFAULT_MEASURE_MASK    0x02
#define QEPD_PARAM_DISPLAY_UNIT_MASK    0x1F

#endif

#pragma pack(1)
//����������Ϣ
typedef struct _QEpdBaseConfig{
    //����������Ϣ
    //u32    iDoseSumxMeddle;            //����������Ԥֵ *0.1uSv
    //u32    iDoseSumxAlarm;                //������������ֵ *0.1uSv
    //u32    iDoseRateMeddle;            //���������ʸ�Ԥֵ *0.1uSv/h
    //u32    iDoseRateAlarm;                //���������ʱ��� *0.1uSv/h
    float        iScaleFactor;                //��������У������
    //QLocalTime AdjustTime;                    //��������У׼ʱ��
    //u16    iUsedTimeAlarm;                //ʹ��ʱ�䱨��ֵ,����

    //float     iHolelifeSumDose;                //���������ܼ���
    //u8    ucRunningMode;
        /*
             0: �������Ƿ����
             1: ����ģʽ,0:���� 1: �ֶ�
             2: ����/�ر�״̬,0:δ����,1: ����
             3: ����ģʽ, 1:˯��ģʽ 0:����˯��ģʽ
         4 - 6: ����������
              7: LED��ʽ
        */
    //u8    ucModeSetting;
        /*
             0: ��ʼ����״̬, 1:�������λ��0:���ֱ���λ
         1 - 2: ���ƴ洢ģʽ, 00:�رգ�01:����������10:����洢
         3 - 5: ���ƴ洢���, 000:15�룬001:30��         
         6 - 7: ���ⷽʽ
        */
    //u8    ucDisplayMode;
        /*   
         0 - 2: ͨѶ����
             3: �洢ģʽ
             4: Ĭ�ϲ���
         5 - 7: ������ʾ��λ
       */
    //u16 ucDoseIncreaseAlarm;//������������������0������; 5:4CPS; 4:8CPS; 3:1ucV; 3:10ucV; 1:100ucV;2016.4.18����,���������ֽڣ���Ȼ�洢�ж�������
    //u8 resv[3];
}QEpdBaseConfig,*PQEpdBaseConfig;

#if 0
//��������
typedef struct _QEpdTrendData {
    float doseRate;        //������
    float doseSumx;        //�ۼƼ���
    u32 timeStamp;
    u8 doseType;    
}QEpdTrendData,*PQEpdTrendData;
#endif

typedef struct _QEpdParaInfo
{
    u32        dwSignature;

    #if 0
    //u32          dwDeviceNo;            //�����Ʊ��,   4Bytes    
    union _DWDOSENUM
    {
        unsigned long dwDeviceNo;
        struct _DW_DSOE_NUM
        {
             unsigned long MainID:20;
             unsigned long ver:4;        //��������Ͱ汾��
             unsigned long unit:4;    //��λ���
        }tdwDeviceNo;
    }DoseNum;
    #endif
    
    //QEpdFactoryInfo factoryInfo;        //���Ҳ���,ֻ��,���������
    //QEpdAssignInfo    assignInfo;            //�豸������Ϣ,40Bytes
    QEpdStatusInfo    statusInfo;            //�豸״̬��Ϣ,20Bytes
    QEpdBaseConfig    baseConfig;            //�豸������Ϣ,32Bytes
    QEpdDoseFactor  tDoseFactor;        //��ϲ�����Ϣ,68Bytes = 16x4 + 1 + 3(����)
}QEpdParaInfo,QEpdPara;


#define HMI_FILE_SIGNATURE      0x00103011    //����ǩ��<4�ֽ�>

#define FLASH_SIGNATURE_OFFSET    0

#if 0
/*********�˵�汾��EPD��������Ҫͬ�������ز���****/
/*���岻�����ڵ�ʱ��ṹ��*/
typedef struct _date
{   
  char second;
  char minute;
  char hour;  
  char day;
  char month;
  unsigned int year;
}QTrenddataTime;

typedef struct OLD_PARA_INFOR
{
    union _DOSENUM
    {
        unsigned long nDoseNum;
        struct _DSOE_NUM
        {
            unsigned long MainID:20;
            unsigned long ver:4;        //��������Ͱ汾��
            unsigned long unit:4;    //��λ���
        }tDoseNum;
    }DoseNum;
    char IDCardNum[18];
    char UserName[8];
    char TaskNum[6];
    unsigned int DataMemorySize; //���ݴ洢����
    unsigned int UsedMemorySize; //ʹ�ô洢����
    float SumDose; //�ϴμ�������
    float MaxDoseRate; //��������
    unsigned long UsedTime;//�ۼƲ���ʱ��

    unsigned long MeddleSumDose; //����������Ԥֵ      ��λ: 100nSv
    unsigned long SumDoseAlarm; //������������ֵ       ��λ: 100nSv 
    unsigned long  MeddleDoseRate; //�����ʸ�Ԥֵ      ��λ: 100nSv  
    unsigned long DoseRateAlarmValue; //�����ʱ���ֵ   ��λ: 100nSv
    unsigned int UsedTimeAlarm;//ʹ��ʱ�䱨��     ��λ����
    float ScaleFactor;
    QTrenddataTime AdjustTime;
    char FailedInfor;
    char alarmInfor;
    char workmode;//bit0(�������Ƿ����),bit1(0:���� 1: �ֶ�),bit2(0:δ����,1: ����) bit3(1:˯��ģʽ 0:����˯��ģʽ )�������Ƿ����  ����ģʽ   ����ر�״̬ ˯��ģʽ��־
    char ModeSet;//bit0(1:�������λ��0:���ֱ���λ), bit2~bit1 (00:�رգ�01:����������10:����洢),bit5~bit3�������ݴ洢ʱ����(000:15�룬001:30��....)��bit6: (0:�����1:���֣����bit0ʹ��)
    char Communicate;//���������� led��ʽ  ���ⷽʽ 
    char Display; //ͨ������  �洢ģʽ Ĭ�ϲ��� ������ʾ��λ
    //u8 ucDoseIncreaseAlarm;//������������������0������; 1:4CPS; 2:8CPS; 3:1ucV; 4:10ucV; 5:100ucV;
    float iHolelifeSumDose;//���������ܼ���
    QEpdDoseFactor tDoseFactor;
}pOLD_PARA_INFOR,OLDQComPara;
#endif

#pragma pack()
/*********�˵�汾��EPD��������Ҫͬ�������ز���****/
//���ڹ��汾�Ĳ���д�뵽�ϰ汾�Ĳ������У�Ŀ����Ϊ�˰Ѳ������͵�������
//void SetOldEpdPara(pOLD_PARA_INFOR *oldParam, QEpdParaInfo *epdParam);
//���ϰ汾�Ĳ���д�뵽�ڹ��汾�Ĳ������У�Ŀ����Ϊ�˽��յ��Ĳ���д��EPD
//void SetEpdPara(QEpdParaInfo *epdParamp, pOLD_PARA_INFOR *oldParam);
//extern OLDQComPara gs_OldParam;
//���������ö������
enum {MID_DOSE_STATE=0,LOW_DOSE_STATE};
enum _DOSE_MODE{LOW_DOSE_RATE_MODE,MIDDLE_DOSE_RATE_MODE,HIGH_DOSE_RATE_MODE ,BEN_DI_MODE};

typedef struct
{

//SUBCLASS(QHsm,QEpd)
    QEpdPara EpdPara;        //�豸����

    //u8 Buffer[512];//���ܻ�����

    u16 RecvSize;
    //u16 lastDoseState;        //ǰһ�ε�״̬

    //u16 doseAlarmCnt;
    //u16 doseWarnxCnt;
    
    float fDoseRate;    //���������� mSv/h
    float fDoseSumx; //�ۻ�����   mSv
    float fDoseSumIncrease;//�ۻ����������������ڶ���ʱ����
    //u32 sumDoseIncrease;//�ۼƼ������������������ڶ���ʱ����
    //u16 ActiveWaitTime;//�����ȴ�ʱ�䣬ʱ��Ϊ0�˲��ܿ�ʼ����
    //u32 AlarmRestTime;//����ʣ��ʱ�� = (�����ۼ�Ԥ��-�ۻ�������/����������
    //float fDoseRate;    //����������
    //float fDoseSumx; //�ۻ�������


    //uint32 PluseCnt;
    //������������
    //double fDoseSumxMeddle; //����������Ԥֵ 0.1uSv
    //double fDoseRateMeddle; //�����ʸ�Ԥֵ 0.1uSv
    //double fDoseSumxAlarm;  //������������ֵ 0.1uSv/h
    //double fDoseRateAlarm;  //�����ʱ���ֵ 0.1uSv/h

    //float scaleSumDose;     //�̶����ۼƼ���
    //float scaleDoseRate;    //�̶��ü�����,�̶��Ƿ��͹�����
    //float scaleSumDoseCom;  //2012.3.8
    //float scaleDoseRateCom; //2012.3.8
    //u32 scaleTimerBCK;    //�̶���ʱ��
    //u32 scaleTimer;    //�̶���ʱ��
    //u32 iSumCPS;        //�̶��ܼ���
   
    //�˵���ʾ����
    //u8 subItemDispIndex;//�Ӳ˵���ʾ����
    //u8 topItemDispIndex;//����˵���ʾ����
    //u8 setParaDispIndex;//����������ʾ����
    //u8 selParaDispIndex;//����ѡ����ʾ����
    
    //u8 paraSelIndex;     //����ѡ������    
    //u8 dispParaEnable;  //����ģʽ,����ȴ�״̬,������ʾʹ��
    //u8 bDispMenuType;
    //u8 alarmState;         //����״̬
    
    //u8 faultState;         //����״̬
    //u8 alarmFlags;         //������ʶ
    
    //u8 quickWatchIndex; //���ٲ鿴��������
    
    //u8 dispMenuType;    /* top menu(dose rate item, dose item, disp par ect), disp par menu */
    //u8 bStartupMeasure;

    //u8 lastUnitIcoIndex;
    //u8 lastItemIcoIndex;
    //u8 lastTypeIcoIndex;
    //u8 lastvBatIcoIndex;
    
    //u8 isAlarmbeepOn;//������ͼ���Ƿ���ʾ0����ʾ��1��ʾ
    //u8 isAlarmMotorOn;//��ͼ���Ƿ���ʾ0����ʾ��1��ʾ

    //��ʱ����
    u8 timerCPSCnt;            //CPS, 30�����
    u16 updateBatCnt;        //���״̬���¼���
    //u8 iCurAlarm;
    //u8 lastAlarm;
    
    //u32 syncbitmap;     //��������ʱ�õ�ͬ��bitmap
    //u32 iUsedTime;
    //u32 trendSaveCount;
    //u32 trendSaveTime;    //�������ݱ���ʱ��

    //u32 timerCnt;
    u32 sumDoseCount;        //�ۼƼ�������
    u32 cpsDoseCount;        //CPS����
    u32 cpsDosePerCnt;        //30��CPS����

    //u32 tempFlags;
    /* xhliu add for new dose rate algorithm 2013/08/15 */
    u32  idxFilterCPS;
    //u32  nCPSQue;
    //u32  nScaleTOCircle;
    //u32  nLowCount;
    //u32  nHighCunt;   
    //u32  dwCPSCircle;
    //u16 batlowcount;
   
   
    //u32  dwPulseCirStartTime;              /* in ms                             */
    //u32  dwPulseCirEndTime;                /* in ms                             */
    //u32  dwSlideWinSize;
    //u32 SysStartIdelTicks;

    //u8  byChangeDirct;              /* 1: CPS change to Up. 0: CPS change to down */
    //u8  byCPSMeasureMode;
    //u8  byPulseFlag;                      /* 0: �������ڿ�ʼ�� 1: �������ڽ��� */
    //u8  byPulseMode;                      /* 0: ������ 1:pre-change            */

    //u8  byPulseChFlg; 
    //u8  byPulseChCPS;
    //u8  KeyOffAlarm;
    //u8  alarmLedEnable;   //������״̬
    //u8  alarmIcoEnable;   //����ͼ��״̬
    //u8  alarmBatEnable;
    //u16 dwPluseCircle;
    //u8  bGetPkuPer500ms;//1500MS��һ������
    /* xhliu add 2013-10-18 */
    float afCPSBuf[FIRST_CPS_MEASURE_TIME];
    float affilterCPSBuf[MIN_FILTER_WIN_SIZE];
    u16 nCPSBuf;

    u8 byDoseRateMode;  // 0: low dose rate 1: high dose rate
    u8 byVariWin;

    float fLowDRCpsBuf[LOW_DOSE_RATE_MODE_CPS_FITERBUF_SZIE];
    float fLowDRFiterBuf[5];
    int   iLowDRFiterBufIdx;
    int   iLowDRCPSBufIdx;
    float fLowDRLastCnt;
    
    u8 bLowDRFlage;
     u8 byLowDRCPSBufThre;    
    u8  bSwitchFlag;   //0: ��û���л���1:���л���2:���л�
    u8 iGotoLowDRCnt;

    //int   iDR;
    //int   iGotoLDR;
    //float fGotoLDRSumCPS;
  
    
    // unsigned int idxFilterCPS;
    u8 nSwitchThr;
    u8 nLowCPS; 
    u8 nHighCSP;
   /*exit loop alrm or don't alram*/
    //u8 bExitLoopAlarm;

    //float fLimitAmpTh;
   
   
   /* timer of cycle enter  */
   //alarm EnterSleepModeTimer;
   //alarm alarmControler[TIMER_COUNT];//���еĶ�ʱ����һ�����
   /*timer monitor dose rate when sleep mode*/
   //alarm MonDRSleepMode;
  
   /* timer update dose rate,sum dose rate etc.*/
  // alarm UpdateDoseRateTimer;
   /* timer update pku counter*/
   //alarm UpdatePkuCountTimer;
   //alarm UpdatePkuCountTimer500ms;
  
   //alarm LedWinkTimer;

   //alarm TimeOutTimmer; 

   //u32 backlighttime;
}QEpd;



#define PKUR(x)        (x)?(P0 |= (1<<6)):(P0 &= ~(1<<6))      
#define PKU_AE1(x)     (x)?(P2 |= (1<<6)):(P2 &= ~(1<<6))      
#define PKU_AE2(x)     (x)?(P2 |= (1<<7)):(P2 &= ~(1<<7))      
#define PKU_RST(x)     (x)?(P4 |= (1<<6)):(P4 &= ~(1<<6))   
#define COUNT(x)       (x)?(P4 |= (1<<5)):(P4 &= ~(1<<5))   
#define PKU_MOSI(x)    (x)?(P2 |= (1<<3)):(P2 &= ~(1<<3))  
#define PKU_CLK(x)     (x)?(P2 |= (1<<5)):(P2 &= ~(1<<5))  
#define PKU_MOSO()    (P2 & (1<<4))
#define KEY1()    (P3 & (1<<2))





//==================================��������=============================
extern SYS_PARAM xdata SysParam;
extern WORD  CntTimer;
extern WORD  TimerACnt;
extern WORD  SleepCnt;
extern WORD  UartCnt;

extern BYTE PrukData[6];
extern float DoseRate;
extern float DoseMax;
extern float DoseTotal;
extern DWORD Cps;

extern DWORD Cps30s;

extern WORD  CntTimer;

extern QPara Sys_Para;
extern BYTE DevSt;


//==================================��������=============================

void Error();
WORD GetCounter();
//void Delay(WORD ms);

void PkurSw(BYTE ch);
float CpsTouSv_h(float count);
void AddTotal();
void Task_Counter();
void WriteParam();
void SyncModBusDev();



#endif






