#ifndef EPD_H
#define EPD_H
#ifdef __cplusplus
extern "C"
{
#endif
#include "config.h"
    
#include "appconfig.h"
//#include "qhsm.h"
//#include "list.h"
//#include "epd_alarm.h"  
//#include "MsgEngine.h"
//#include "msp430xx_hal.h"
  
//��������汾��
#define VERSION "V:VER1.986"  
  
#define LOW_POWER_TIMER  10000  // ����͹��ĵ�ʱ�䣬���������
#define UPDATE_TIMER_TIMEOUT  1000  // in ms
#define ACTIVE_WAIT_TIME 15//in s,�����ȴ�ʱ��
#define TIME_OUT_RETURN_VALUE  10//in s, when time out return mesure disp

#define DOSERATE_OVERLOAD_VALUE		10000
#define DOSESUMX_OVERLOAD_VALUE		10000

//#define INCREASE_ALARM_VALUE		20	//�ۻ���������������,uSv,��ʱ�ú궨�壬�����ö����Ǹ�����������
/* �������� */
#define ALARM_TYPE_BATTERY_ALARM	 (1<<0)		//��������
#define ALARM_TYPE_SUMDOSE_ALARM	 (1<<1)		//������������
#define ALARM_TYPE_SUMDOSE_MEDDLE	 (1<<2)		//��������Ԥ����
#define ALARM_TYPE_DOSERATE_ALARM	 (1<<3)		//���������ʱ���
#define ALARM_TYPE_DOSERATE_MEDDLE	 (1<<4)		//����������Ԥ��
#define ALARM_TYPE_USEDTIME_ALARM    (1<<5)		//�ۻ�ʱ�䱨��
#define ALARM_TYPE_SUMDOSE_OVERLOAD  (1<<6)		//�����������ر�����Overflow��
#define ALARM_TYPE_DOSERATE_OVERLOAD (1<<7)		//���������ʹ��ر�����Overflow��
/* ���ϲ��� */
#define FAULT_TYPE_INITIALIZE 		 (1<<0)		//��ʼ������
#define FAULT_TYPE_DETECTOR 		 (1<<1)		//̽ͷ����
#define FAULT_TYPE_DATA 			 (1<<2)		//���ݹ���
#define FAULT_TYPE_STORAGE 			 (1<<3)		//�洢����

/* ����ģʽ */
#define WORK_MODE_ALLOC				 ()
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

#pragma pack(1)	//���ֽڶ���
typedef struct _QEpdDoseFactor
{
	//uint32_t dwDeviceNo;
	uint8_t  experimentmode;//ʵ��ģʽ����(��CPS��) 1:�򿪣�0�ر�
	//uint8_t  resv[3];
	//float lowfactor[15];//���ȱ�����821һ�£���֤ͨ��
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
	char	szSerialNumber[16];	//�������
	char	szFactoryDate[8];	//����ʱ��
	char	szFirmwareVerNo[16];//�̼��汾
	char	szHardwareVerNo[16];//Ӳ���汾
}QEpdFactoryInfo,*PQEpdFactoryInfo;


//�豸������Ϣ
typedef struct _QEpdAssignInfo{
	char  	szUserID[18];			//��ԱID��
	char  	szUserName[8];			//��Ա����
	char  	szTaskID[6];			//�����
	uint32_t dwAssignTime;			//�����Ʒ���ʱ��
}QEpdAssignInfo,*PQEpdAssignInfo;

#endif


//�豸״̬��Ϣ
typedef struct _QEpdStatusInfo
{
	//uint16_t	iStorageMax;			//������ݴ洢����   	
	//uint16_t	iStorageUsed;			//ʹ�ô洢����		
	float	iLastDoseSum;				//�ϴμ�������,������,�ۻ���������
	float	iMaxDoseRate;				//�����������ʣ�������
	uint32_t	iUsedTime;					//����ʹ��ʱ�䣬�ۼƲ���ʱ��
	//uint8_t	ucFaultFlag;
		/*
			 0:	��ʼ������
			 1:	̽ͷ����
			 2:	���ݹ���
			 3:	�洢����		
		*/
	//uint8_t	ucAlarmFlag;
		/*
			 0:	��������
			 1:	������������
			 2:	��������Ԥ����
			 3:	���������ʱ���
			 4:	����������Ԥ����
			 5:	�ۻ�ʱ�䱨��
			 6:	�����������ر�����Overflow��
			 7: ���������ʹ��ر�����Overflow��		
		*/
	//uint8_t	resv[2];
}QEpdStatusInfo,*PQEpdStatusInfo;



#pragma pack()

//����ģʽ
/*
	 0: �������Ƿ����
	 1: ����ģʽ,0:���� 1: �ֶ�
	 2: ����/�ر�״̬,0:δ����,1: ����
	 3: ����ģʽ, 1:˯��ģʽ 0:����˯��ģʽ
 4 - 6: ����������
 	 7: LED��ʽ
*/
#define QEPD_PARAM_ASSIGNED_BIT		0x01
#define QEPD_PARAM_WORKMODE_BIT		0x02
#define QEPD_PARAM_ACTIVE_BIT		0x04
#define QEPD_PARAM_ENERYUSED_BIT	0x08
#define QEPD_PARAM_INTERVAL_BIT		0x10
#define QEPD_PARAM_LEDMODE_BIT		0x80

#define QEPD_PARAM_ASSIGNED_MASK	0xFE
#define QEPD_PARAM_WORKMODE_MASK	0xFD
#define QEPD_PARAM_ACTIVE_MASK		0xFB
#define QEPD_PARAM_ENERYUSED_MASK	0xF7
#define QEPD_PARAM_INTERVAL_MASK	0x8F
#define QEPD_PARAM_LEDMODE_MASK		0x7F

//ģʽ����
/*
	 0: ��ʼ����״̬, 1:�������λ��0:���ֱ���λ
 1 - 2: ���ƴ洢ģʽ, 00:�رգ�01:����������10:����洢
 3 - 5: ���ƴ洢���, 000:15�룬001:30��		 
 6 - 7: ���ⷽʽ
*/
		
#define QEPD_PARAM_CLEARALARM_BIT		0x01
#define QEPD_PARAM_TIGER_SAVE			0x02
#define QEPD_PARAM_INTER_SAVE			0x04
#define QEPD_PARAM_STORAGE_INTERVAL     0x80
#define QEPD_PARAM_BACKLIGHT_MODE		0x40

#define QEPD_PARAM_CLEARALARM_MASK		0x01
#define QEPD_PARAM_STORAGE_MODE_MASK	0x02
#define QEPD_PARAM_STORAGE_INTERVAL_MASK	0x08
#define QEPD_PARAM_BACKLIGHT_MODE_MASK		0x40

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

#define QEPD_PARAM_COMM_SPEED			0x01
#define QEPD_PARAM_STORAGE_MODE_BIT 	0x02
#define QEPD_PARAM_DEFAULT_MEASURE		0x08
#define QEPD_PARAM_DISPLAY_UNIT		    0x20

#define QEPD_PARAM_CLEARALARM_MASK		0x01
#define QEPD_PARAM_DEFAULT_MEASURE_MASK	0x02
#define QEPD_PARAM_DISPLAY_UNIT_MASK	0x1F

#pragma pack(1)
//����������Ϣ
typedef struct _QEpdBaseConfig{
	//����������Ϣ
	//uint32_t	iDoseSumxMeddle;			//����������Ԥֵ *0.1uSv
	//uint32_t	iDoseSumxAlarm;				//������������ֵ *0.1uSv
	//uint32_t	iDoseRateMeddle;			//���������ʸ�Ԥֵ *0.1uSv/h
	//uint32_t	iDoseRateAlarm;				//���������ʱ��� *0.1uSv/h
	float		iScaleFactor;				//��������У������
	//QLocalTime AdjustTime;					//��������У׼ʱ��
	//uint16_t	iUsedTimeAlarm;				//ʹ��ʱ�䱨��ֵ,����

	//float 	iHolelifeSumDose;				//���������ܼ���
	//uint8_t	ucRunningMode;
		/*
			 0: �������Ƿ����
			 1: ����ģʽ,0:���� 1: �ֶ�
			 2: ����/�ر�״̬,0:δ����,1: ����
			 3: ����ģʽ, 1:˯��ģʽ 0:����˯��ģʽ
		 4 - 6: ����������
		 	 7: LED��ʽ
		*/
	//uint8_t	ucModeSetting;
		/*
			 0: ��ʼ����״̬, 1:�������λ��0:���ֱ���λ
		 1 - 2: ���ƴ洢ģʽ, 00:�رգ�01:����������10:����洢
		 3 - 5: ���ƴ洢���, 000:15�룬001:30��		 
		 6 - 7: ���ⷽʽ
		*/
	//uint8_t	ucDisplayMode;
		/*   
		 0 - 2: ͨѶ����
			 3: �洢ģʽ
			 4: Ĭ�ϲ���
	     5 - 7: ������ʾ��λ
	   */
	//uint16_t ucDoseIncreaseAlarm;//������������������0������; 5:4CPS; 4:8CPS; 3:1ucV; 3:10ucV; 1:100ucV;2016.4.18����,���������ֽڣ���Ȼ�洢�ж�������
	//uint8_t resv[3];
}QEpdBaseConfig,*PQEpdBaseConfig;

#if 0
//��������
typedef struct _QEpdTrendData {
	float doseRate;		//������
	float doseSumx;		//�ۼƼ���
	uint32_t timeStamp;
	uint8_t doseType;	
}QEpdTrendData,*PQEpdTrendData;
#endif

typedef struct _QEpdParaInfo
{
	uint32_t        dwSignature;

    #if 0
	//uint32_t  		dwDeviceNo;			//�����Ʊ��,   4Bytes	
	union _DWDOSENUM
	{
		unsigned long dwDeviceNo;
		struct _DW_DSOE_NUM
		{
			 unsigned long MainID:20;
			 unsigned long ver:4;		//��������Ͱ汾��
			 unsigned long unit:4;	//��λ���
		}tdwDeviceNo;
	}DoseNum;
    #endif
    
	//QEpdFactoryInfo factoryInfo;		//���Ҳ���,ֻ��,���������
	//QEpdAssignInfo	assignInfo;			//�豸������Ϣ,40Bytes
	QEpdStatusInfo	statusInfo;			//�豸״̬��Ϣ,20Bytes
	QEpdBaseConfig	baseConfig;			//�豸������Ϣ,32Bytes
	QEpdDoseFactor  tDoseFactor;		//��ϲ�����Ϣ,68Bytes = 16x4 + 1 + 3(����)
}QEpdParaInfo,QEpdPara;


#define HMI_FILE_SIGNATURE      0x00103011	//����ǩ��<4�ֽ�>

#define FLASH_SIGNATURE_OFFSET	0

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
			unsigned long ver:4;		//��������Ͱ汾��
			unsigned long unit:4;	//��λ���
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
	//uint8_t ucDoseIncreaseAlarm;//������������������0������; 1:4CPS; 2:8CPS; 3:1ucV; 4:10ucV; 5:100ucV;
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
	QEpdPara EpdPara;		//�豸����

	//uint8_t Buffer[512];//���ܻ�����

	uint16_t RecvSize;
	//uint16_t lastDoseState;		//ǰһ�ε�״̬

	//uint16_t doseAlarmCnt;
	//uint16_t doseWarnxCnt;
	
	float fDoseRate;	//���������� mSv/h
	float fDoseSumx; //�ۻ�����   mSv
	float fDoseSumIncrease;//�ۻ����������������ڶ���ʱ����
	//uint32_t sumDoseIncrease;//�ۼƼ������������������ڶ���ʱ����
	//uint16_t ActiveWaitTime;//�����ȴ�ʱ�䣬ʱ��Ϊ0�˲��ܿ�ʼ����
	//uint32_t AlarmRestTime;//����ʣ��ʱ�� = (�����ۼ�Ԥ��-�ۻ�������/����������
	//float fDoseRate;	//����������
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
	//uint32_t scaleTimerBCK;	//�̶���ʱ��
	//uint32_t scaleTimer;	//�̶���ʱ��
	//uint32_t iSumCPS;		//�̶��ܼ���
   
	//�˵���ʾ����
	//uint8_t subItemDispIndex;//�Ӳ˵���ʾ����
	//uint8_t topItemDispIndex;//����˵���ʾ����
	//uint8_t setParaDispIndex;//����������ʾ����
	//uint8_t selParaDispIndex;//����ѡ����ʾ����
	
	//uint8_t paraSelIndex;	 //����ѡ������	
	//uint8_t dispParaEnable;  //����ģʽ,����ȴ�״̬,������ʾʹ��
	//uint8_t bDispMenuType;
	//uint8_t alarmState;	     //����״̬
	
	//uint8_t faultState;	     //����״̬
	//uint8_t alarmFlags;	     //������ʶ
	
	//uint8_t quickWatchIndex; //���ٲ鿴��������
	
	//uint8_t dispMenuType;    /* top menu(dose rate item, dose item, disp par ect), disp par menu */
	//uint8_t bStartupMeasure;

	//uint8_t lastUnitIcoIndex;
	//uint8_t lastItemIcoIndex;
	//uint8_t lastTypeIcoIndex;
	//uint8_t lastvBatIcoIndex;
	
	//uint8_t isAlarmbeepOn;//������ͼ���Ƿ���ʾ0����ʾ��1��ʾ
	//uint8_t isAlarmMotorOn;//��ͼ���Ƿ���ʾ0����ʾ��1��ʾ

	//��ʱ����
	uint8_t timerCPSCnt;		    //CPS, 30�����
	uint16_t updateBatCnt;		//���״̬���¼���
	//uint8_t iCurAlarm;
	//uint8_t lastAlarm;
	
	//uint32_t syncbitmap;	 //��������ʱ�õ�ͬ��bitmap
	//uint32_t iUsedTime;
	//uint32_t trendSaveCount;
	//uint32_t trendSaveTime;	//�������ݱ���ʱ��

	//uint32_t timerCnt;
	uint32_t sumDoseCount;		//�ۼƼ�������
	uint32_t cpsDoseCount;		//CPS����
	uint32_t cpsDosePerCnt;		//30��CPS����

	//uint32_t tempFlags;
	/* xhliu add for new dose rate algorithm 2013/08/15 */
	uint32_t  idxFilterCPS;
	//uint32_t  nCPSQue;
	//uint32_t  nScaleTOCircle;
	//uint32_t  nLowCount;
	//uint32_t  nHighCunt;   
	//uint32_t  dwCPSCircle;
	//uint16_t batlowcount;
   
   
	//uint32_t  dwPulseCirStartTime;              /* in ms                             */
	//uint32_t  dwPulseCirEndTime;                /* in ms                             */
	//uint32_t  dwSlideWinSize;
	//uint32_t SysStartIdelTicks;

	//uint8_t  byChangeDirct;              /* 1: CPS change to Up. 0: CPS change to down */
	//uint8_t  byCPSMeasureMode;
	//uint8_t  byPulseFlag;                      /* 0: �������ڿ�ʼ�� 1: �������ڽ��� */
	//uint8_t  byPulseMode;                      /* 0: ������ 1:pre-change            */

	//uint8_t  byPulseChFlg; 
	//uint8_t  byPulseChCPS;
	//uint8_t  KeyOffAlarm;
	//uint8_t  alarmLedEnable;   //������״̬
	//uint8_t  alarmIcoEnable;   //����ͼ��״̬
	//uint8_t  alarmBatEnable;
	//uint16_t dwPluseCircle;
	//uint8_t  bGetPkuPer500ms;//1500MS��һ������
	/* xhliu add 2013-10-18 */
	float afCPSBuf[FIRST_CPS_MEASURE_TIME];
	float affilterCPSBuf[MIN_FILTER_WIN_SIZE];
	uint16_t nCPSBuf;

	uint8_t byDoseRateMode;  // 0: low dose rate 1: high dose rate
	uint8_t byVariWin;

	float fLowDRCpsBuf[LOW_DOSE_RATE_MODE_CPS_FITERBUF_SZIE];
	float fLowDRFiterBuf[5];
	int   iLowDRFiterBufIdx;
	int   iLowDRCPSBufIdx;
	float fLowDRLastCnt;
	
	uint8_t bLowDRFlage;
 	uint8_t byLowDRCPSBufThre;	
	uint8_t  bSwitchFlag;   //0: ��û���л���1:���л���2:���л�
	uint8_t iGotoLowDRCnt;

	//int   iDR;
	//int   iGotoLDR;
	//float fGotoLDRSumCPS;
  
	
	// unsigned int idxFilterCPS;
	uint8_t nSwitchThr;
	uint8_t nLowCPS; 
	uint8_t nHighCSP;
   /*exit loop alrm or don't alram*/
    //uint8_t bExitLoopAlarm;

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

   //uint32_t backlighttime;
}QEpd;

//METHOD
//#pragma pack(pop)
#pragma pack()
//��EPD.c���õ������к�����������
  /* void QEPDCtor(QEpd * me);
   QSTATE QEpdSelCheck(WPARAM,LPARAM);
   QSTATE QEpdDispError(WPARAM,LPARAM);
   QSTATE QEpdOFF(WPARAM,LPARAM);
   QSTATE QEpdMeasure(WPARAM,LPARAM);
   QSTATE QEpdDispPara(WPARAM,LPARAM);
   QSTATE QEpdScale(WPARAM wParm,LPARAM lParm);
   QSTATE QEpdRFCOM(WPARAM wParm,LPARAM lParm);
   QSTATE QEpdTop(WPARAM wParm,LPARAM lParm);
   QSTATE QEpdStateSetPara(WPARAM wParm,LPARAM lParm);
   QSTATE QEpdSelectPara(WPARAM wParm,LPARAM lParm);
   QSTATE QEpdModifySavePower(WPARAM wParm,LPARAM lParm);
   
   void MsgPreProcess(QHsm *me,MSG *msg);
   void QEpdEnterSleep(QEpd *me);
   void QEpdWaken(QEpd *me);
   void QEpdSoftPowerOff(QEpd *me);
   void ExitExcuteFun(QEpd * me);
   
   void QEpdIdleMsg(QEpd *me);
   int MonDRSleepModeCallBackFun(void *p);
   int LedWinkTimerCallFun(void *p);
   void QEpdMeasureSleepMsg(QEpd *me);
   void QEpdMeasureWakenMsg(QEpd *me);
   void EpdSetParaInitMsg(QEpd *me);
   void EpdSelectParaInitMsg(QEpd *me);
   int EpdRevIRDAData(QEpd *me,LPARAM lparam);
   void EpdOFFRevRFData(QEpd *me,LPARAM lparam);
   BYTE EpdRevIRDAFirstPacket(QEpd *me,LPARAM lparam);
   BOOL EpdSelCheck(QEpd *me);
   void EpdOFFInitMsg(QEpd *me,LPARAM lparam);   
   void EpdSelCheckInitMsg(QEpd *me,LPARAM lparam);
   void EpdScaleInitMsg(QEpd *me,LPARAM lparam);
   void EpdRFComInitMsg(QEpd *me,LPARAM lparam);
   void EpdMeasureInitMsg(QEpd *me);
   void EpdDispParaInitMsg(QEpd *me,LPARAM lparam);
   void MeasureOnlineParaItem(QEpd *me);
   void EpdParaInit(QEpd * me);
   BOOL UpdateUIMsgProc(QEpd * me);
   void DisplayStateManu(QEpd *me);
   void DisplayStateAuto(QEpd *me);
   int MsgGenerate();
   void ComputeDoserate(QEpd * me);

   void SingleFrameSlaveAck(QEpd *me);
   void SingleFrameMasterWrite(QEpd *me);
   void ComplexFrameMasterWrite(QEpd *me,int Revlen);
   void ComplexFrameMasterRead(QEpd *me);
   void SaveSetTime(QEpd *me);
   void SingleFrameSCaleRead(QEpd *me);
   void SingleFrameSCaleAck(QEpd *me);
   void SingleFrameSCaleFactorRead(QEpd *me);
   void SaveFromScale(QEpd *me);
   BOOL UpdateBatState(QEpd *me);
   void DisplayDoserate(QEpd *me,double fdoserate);
   void DisplaySumDose(QEpd *me,double fSumdoserate);
   void DisplayUsedTime(QEpd *me);
   void SaveTrendData(QEpd *me);
   int ReadTrendData(unsigned char *addr,char *buf,unsigned int len);   
   void QEpdReadTrendData(QEpd *me);
   void UpdateWritePara(QEpd * me);
   void SaveSetThreshold(QEpd *me);
   unsigned long power(int x,int y);
   unsigned char StoreParaCheck(QEpd *me);
   void EpdRFIsrMsg(QEpd *me);
   void MeasureManuTopItem(QEpd *me);
   void MeasurEnterOhterMode(QEpd *me);
   void MeasureOnlineTopItem(QEpd *me);   
   void ParsetEnterSleep(QEpd *me);
   void SelectParEnterSleep(QEpd *me);
   void SavePowerEnterSleep(QEpd *me);
   void EpdReset(QEpd *me);
   void ComputeDoseRateEx3(QEpd *me);  // ��ԭ�������㷨�������޸�
   void ScaleComputedoseEX(QEpd * me);//�������㷨�Ŀ̶ȼ����ʼ��㺯��
   float GetCPS();
   */
ENDCLASS
   

#ifdef __cplusplus
}
#endif
#endif
