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
  
//定义软件版本号
#define VERSION "V:VER1.986"  
  
#define LOW_POWER_TIMER  10000  // 进入低功耗的时间，按毫秒计算
#define UPDATE_TIMER_TIMEOUT  1000  // in ms
#define ACTIVE_WAIT_TIME 15//in s,激活后等待时间
#define TIME_OUT_RETURN_VALUE  10//in s, when time out return mesure disp

#define DOSERATE_OVERLOAD_VALUE		10000
#define DOSESUMX_OVERLOAD_VALUE		10000

//#define INCREASE_ALARM_VALUE		20	//累积剂量率增长报警,uSv,暂时用宏定义，后期用读出仪给剂量计设置
/* 报警参数 */
#define ALARM_TYPE_BATTERY_ALARM	 (1<<0)		//电量报警
#define ALARM_TYPE_SUMDOSE_ALARM	 (1<<1)		//剂量当量报警
#define ALARM_TYPE_SUMDOSE_MEDDLE	 (1<<2)		//剂量当量预报警
#define ALARM_TYPE_DOSERATE_ALARM	 (1<<3)		//剂量当量率报警
#define ALARM_TYPE_DOSERATE_MEDDLE	 (1<<4)		//剂量当量率预警
#define ALARM_TYPE_USEDTIME_ALARM    (1<<5)		//累积时间报警
#define ALARM_TYPE_SUMDOSE_OVERLOAD  (1<<6)		//剂量当量过载报警（Overflow）
#define ALARM_TYPE_DOSERATE_OVERLOAD (1<<7)		//剂量当量率过载报警（Overflow）
/* 故障参数 */
#define FAULT_TYPE_INITIALIZE 		 (1<<0)		//初始化故障
#define FAULT_TYPE_DETECTOR 		 (1<<1)		//探头故障
#define FAULT_TYPE_DATA 			 (1<<2)		//数据故障
#define FAULT_TYPE_STORAGE 			 (1<<3)		//存储故障

/* 运行模式 */
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

#pragma pack(1)	//按字节对齐
typedef struct _QEpdDoseFactor
{
	//uint32_t dwDeviceNo;
	uint8_t  experimentmode;//实验模式开关(打开CPS等) 1:打开，0关闭
	//uint8_t  resv[3];
	//float lowfactor[15];//长度保存与821一致，保证通信
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
//设备出厂信息
typedef struct _QEpdFactoryInfo{
	//厂家参数,只读,不允许更改
	char	szSerialNumber[16];	//出厂编号
	char	szFactoryDate[8];	//出厂时间
	char	szFirmwareVerNo[16];//固件版本
	char	szHardwareVerNo[16];//硬件版本
}QEpdFactoryInfo,*PQEpdFactoryInfo;


//设备分配信息
typedef struct _QEpdAssignInfo{
	char  	szUserID[18];			//人员ID号
	char  	szUserName[8];			//人员姓名
	char  	szTaskID[6];			//任务号
	uint32_t dwAssignTime;			//剂量计分配时间
}QEpdAssignInfo,*PQEpdAssignInfo;

#endif


//设备状态信息
typedef struct _QEpdStatusInfo
{
	//uint16_t	iStorageMax;			//最大数据存储容量   	
	//uint16_t	iStorageUsed;			//使用存储容量		
	float	iLastDoseSum;				//上次剂量当量,浮点型,累积剂量当量
	float	iMaxDoseRate;				//最大剂量当量率，浮点型
	uint32_t	iUsedTime;					//单次使用时间，累计测量时间
	//uint8_t	ucFaultFlag;
		/*
			 0:	初始化故障
			 1:	探头故障
			 2:	数据故障
			 3:	存储故障		
		*/
	//uint8_t	ucAlarmFlag;
		/*
			 0:	电量报警
			 1:	剂量当量报警
			 2:	剂量当量预报警
			 3:	剂量当量率报警
			 4:	剂量当量率预报警
			 5:	累积时间报警
			 6:	剂量当量过载报警（Overflow）
			 7: 剂量当量率过载报警（Overflow）		
		*/
	//uint8_t	resv[2];
}QEpdStatusInfo,*PQEpdStatusInfo;



#pragma pack()

//运行模式
/*
	 0: 剂量计是否分配
	 1: 工作模式,0:联机 1: 手动
	 2: 激活/关闭状态,0:未激活,1: 激活
	 3: 节能模式, 1:睡眠模式 0:不是睡眠模式
 4 - 6: 监测声音间隔
 	 7: LED方式
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

//模式设置
/*
	 0: 开始测量状态, 1:清除报警位，0:保持报警位
 1 - 2: 趋势存储模式, 00:关闭，01:报警触发，10:间隔存储
 3 - 5: 趋势存储间隔, 000:15秒，001:30秒		 
 6 - 7: 背光方式
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
		 0 - 2: 通讯速率
			 3: 存储模式
			 4: 默认测量
	     5 - 7: 测量显示单位
	   */

#define QEPD_PARAM_COMM_SPEED			0x01
#define QEPD_PARAM_STORAGE_MODE_BIT 	0x02
#define QEPD_PARAM_DEFAULT_MEASURE		0x08
#define QEPD_PARAM_DISPLAY_UNIT		    0x20

#define QEPD_PARAM_CLEARALARM_MASK		0x01
#define QEPD_PARAM_DEFAULT_MEASURE_MASK	0x02
#define QEPD_PARAM_DISPLAY_UNIT_MASK	0x1F

#pragma pack(1)
//参数设置信息
typedef struct _QEpdBaseConfig{
	//报警参数信息
	//uint32_t	iDoseSumxMeddle;			//剂量当量干预值 *0.1uSv
	//uint32_t	iDoseSumxAlarm;				//剂量当量报警值 *0.1uSv
	//uint32_t	iDoseRateMeddle;			//剂量当量率干预值 *0.1uSv/h
	//uint32_t	iDoseRateAlarm;				//剂量当量率报警 *0.1uSv/h
	float		iScaleFactor;				//趋势数据校正因子
	//QLocalTime AdjustTime;					//趋势数据校准时间
	//uint16_t	iUsedTimeAlarm;				//使用时间报警值,分钟

	//float 	iHolelifeSumDose;				//生命周期总计量
	//uint8_t	ucRunningMode;
		/*
			 0: 剂量计是否分配
			 1: 工作模式,0:联机 1: 手动
			 2: 激活/关闭状态,0:未激活,1: 激活
			 3: 节能模式, 1:睡眠模式 0:不是睡眠模式
		 4 - 6: 监测声音间隔
		 	 7: LED方式
		*/
	//uint8_t	ucModeSetting;
		/*
			 0: 开始测量状态, 1:清除报警位，0:保持报警位
		 1 - 2: 趋势存储模式, 00:关闭，01:报警触发，10:间隔存储
		 3 - 5: 趋势存储间隔, 000:15秒，001:30秒		 
		 6 - 7: 背光方式
		*/
	//uint8_t	ucDisplayMode;
		/*   
		 0 - 2: 通讯速率
			 3: 存储模式
			 4: 默认测量
	     5 - 7: 测量显示单位
	   */
	//uint16_t ucDoseIncreaseAlarm;//剂量当量增长报警，0不报警; 5:4CPS; 4:8CPS; 3:1ucV; 3:10ucV; 1:100ucV;2016.4.18新增,必须两个字节，不然存储有对齐问题
	//uint8_t resv[3];
}QEpdBaseConfig,*PQEpdBaseConfig;

#if 0
//趋势数据
typedef struct _QEpdTrendData {
	float doseRate;		//剂量率
	float doseSumx;		//累计剂量
	uint32_t timeStamp;
	uint8_t doseType;	
}QEpdTrendData,*PQEpdTrendData;
#endif

typedef struct _QEpdParaInfo
{
	uint32_t        dwSignature;

    #if 0
	//uint32_t  		dwDeviceNo;			//剂量计编号,   4Bytes	
	union _DWDOSENUM
	{
		unsigned long dwDeviceNo;
		struct _DW_DSOE_NUM
		{
			 unsigned long MainID:20;
			 unsigned long ver:4;		//仪器种类和版本号
			 unsigned long unit:4;	//单位编号
		}tdwDeviceNo;
	}DoseNum;
    #endif
    
	//QEpdFactoryInfo factoryInfo;		//厂家参数,只读,不允许更改
	//QEpdAssignInfo	assignInfo;			//设备分配信息,40Bytes
	QEpdStatusInfo	statusInfo;			//设备状态信息,20Bytes
	QEpdBaseConfig	baseConfig;			//设备配置信息,32Bytes
	QEpdDoseFactor  tDoseFactor;		//拟合参数信息,68Bytes = 16x4 + 1 + 3(保留)
}QEpdParaInfo,QEpdPara;


#define HMI_FILE_SIGNATURE      0x00103011	//配置签名<4字节>

#define FLASH_SIGNATURE_OFFSET	0

#if 0
/*********核电版本的EPD参数，需要同步到本地参数****/
/*定义不带星期的时间结构体*/
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
			unsigned long ver:4;		//仪器种类和版本号
			unsigned long unit:4;	//单位编号
		}tDoseNum;
	}DoseNum;
	char IDCardNum[18];
	char UserName[8];
	char TaskNum[6];
	unsigned int DataMemorySize; //数据存储容量
	unsigned int UsedMemorySize; //使用存储容量
	float SumDose; //上次剂量当量
	float MaxDoseRate; //最大剂量率
	unsigned long UsedTime;//累计测量时间

	unsigned long MeddleSumDose; //剂量当量干预值      单位: 100nSv
	unsigned long SumDoseAlarm; //剂量当量报警值       单位: 100nSv 
	unsigned long  MeddleDoseRate; //剂量率干预值      单位: 100nSv  
	unsigned long DoseRateAlarmValue; //剂量率报警值   单位: 100nSv
	unsigned int UsedTimeAlarm;//使用时间报警     单位分钟
	float ScaleFactor;
	QTrenddataTime AdjustTime;
	char FailedInfor;
	char alarmInfor;
	char workmode;//bit0(剂量计是否分配),bit1(0:联机 1: 手动),bit2(0:未激活,1: 激活) bit3(1:睡眠模式 0:不是睡眠模式 )剂量计是否分配  工作模式   激活关闭状态 睡眠模式标志
	char ModeSet;//bit0(1:清除报警位，0:保持报警位), bit2~bit1 (00:关闭，01:报警触发，10:间隔存储),bit5~bit3趋势数据存储时间间隔(000:15秒，001:30秒....)，bit6: (0:清除，1:保持；配合bit0使用)
	char Communicate;//监测声音间隔 led方式  背光方式 
	char Display; //通信速率  存储模式 默认测量 测量显示单位
	//uint8_t ucDoseIncreaseAlarm;//剂量当量增长报警，0不报警; 1:4CPS; 2:8CPS; 3:1ucV; 4:10ucV; 5:100ucV;
	float iHolelifeSumDose;//生命周期总计量
	QEpdDoseFactor tDoseFactor;
}pOLD_PARA_INFOR,OLDQComPara;
#endif

#pragma pack()
/*********核电版本的EPD参数，需要同步到本地参数****/
//将于工版本的参数写入到老版本的参数当中，目的是为了把参数发送到读出仪
//void SetOldEpdPara(pOLD_PARA_INFOR *oldParam, QEpdParaInfo *epdParam);
//将老版本的参数写入到于工版本的参数当中，目的是为了接收到的参数写入EPD
//void SetEpdPara(QEpdParaInfo *epdParamp, pOLD_PARA_INFOR *oldParam);
//extern OLDQComPara gs_OldParam;
//定义参数的枚举类型
enum {MID_DOSE_STATE=0,LOW_DOSE_STATE};
enum _DOSE_MODE{LOW_DOSE_RATE_MODE,MIDDLE_DOSE_RATE_MODE,HIGH_DOSE_RATE_MODE ,BEN_DI_MODE};

typedef struct
{

//SUBCLASS(QHsm,QEpd)
	QEpdPara EpdPara;		//设备参数

	//uint8_t Buffer[512];//接受缓冲区

	uint16_t RecvSize;
	//uint16_t lastDoseState;		//前一次的状态

	//uint16_t doseAlarmCnt;
	//uint16_t doseWarnxCnt;
	
	float fDoseRate;	//剂量当量率 mSv/h
	float fDoseSumx; //累积剂量   mSv
	float fDoseSumIncrease;//累积剂量增长量，大于多少时报警
	//uint32_t sumDoseIncrease;//累计剂量计数增长，，大于多少时报警
	//uint16_t ActiveWaitTime;//激活后等待时间，时间为0了才能开始测量
	//uint32_t AlarmRestTime;//报警剩余时间 = (剂量累计预警-累积剂量）/剂量当量率
	//float fDoseRate;	//剂量当量率
	//float fDoseSumx; //累积剂量率


	//uint32 PluseCnt;
	//报警参数部分
	//double fDoseSumxMeddle; //剂量当量干预值 0.1uSv
	//double fDoseRateMeddle; //剂量率干预值 0.1uSv
	//double fDoseSumxAlarm;  //剂量当量报警值 0.1uSv/h
	//double fDoseRateAlarm;  //剂量率报警值 0.1uSv/h

	//float scaleSumDose;     //刻度用累计剂量
	//float scaleDoseRate;    //刻度用剂量率,刻度仪发送过来的
	//float scaleSumDoseCom;  //2012.3.8
	//float scaleDoseRateCom; //2012.3.8
	//uint32_t scaleTimerBCK;	//刻度用时间
	//uint32_t scaleTimer;	//刻度用时间
	//uint32_t iSumCPS;		//刻度总计数
   
	//菜单显示部分
	//uint8_t subItemDispIndex;//子菜单显示索引
	//uint8_t topItemDispIndex;//顶层菜单显示索引
	//uint8_t setParaDispIndex;//参数设置显示索引
	//uint8_t selParaDispIndex;//参数选择显示索引
	
	//uint8_t paraSelIndex;	 //参数选择索引	
	//uint8_t dispParaEnable;  //联网模式,激活等待状态,参数显示使能
	//uint8_t bDispMenuType;
	//uint8_t alarmState;	     //报警状态
	
	//uint8_t faultState;	     //故障状态
	//uint8_t alarmFlags;	     //报警标识
	
	//uint8_t quickWatchIndex; //快速查看参数索引
	
	//uint8_t dispMenuType;    /* top menu(dose rate item, dose item, disp par ect), disp par menu */
	//uint8_t bStartupMeasure;

	//uint8_t lastUnitIcoIndex;
	//uint8_t lastItemIcoIndex;
	//uint8_t lastTypeIcoIndex;
	//uint8_t lastvBatIcoIndex;
	
	//uint8_t isAlarmbeepOn;//蜂鸣器图标是否显示0不显示，1显示
	//uint8_t isAlarmMotorOn;//震动图标是否显示0不显示，1显示

	//超时参数
	uint8_t timerCPSCnt;		    //CPS, 30秒计数
	uint16_t updateBatCnt;		//电池状态更新计数
	//uint8_t iCurAlarm;
	//uint8_t lastAlarm;
	
	//uint32_t syncbitmap;	 //保存数据时用的同步bitmap
	//uint32_t iUsedTime;
	//uint32_t trendSaveCount;
	//uint32_t trendSaveTime;	//趋势数据保存时间

	//uint32_t timerCnt;
	uint32_t sumDoseCount;		//累计剂量计数
	uint32_t cpsDoseCount;		//CPS计数
	uint32_t cpsDosePerCnt;		//30秒CPS计数

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
	//uint8_t  byPulseFlag;                      /* 0: 脉冲周期开始， 1: 脉冲周期结束 */
	//uint8_t  byPulseMode;                      /* 0: 正常。 1:pre-change            */

	//uint8_t  byPulseChFlg; 
	//uint8_t  byPulseChCPS;
	//uint8_t  KeyOffAlarm;
	//uint8_t  alarmLedEnable;   //报警灯状态
	//uint8_t  alarmIcoEnable;   //报警图标状态
	//uint8_t  alarmBatEnable;
	//uint16_t dwPluseCircle;
	//uint8_t  bGetPkuPer500ms;//1500MS读一次数据
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
	uint8_t  bSwitchFlag;   //0: 还没有切换，1:低切换，2:高切换
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
   //alarm alarmControler[TIMER_COUNT];//所有的定时器都一起管理
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
//在EPD.c中用到的所有函数进行声明
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
   void ComputeDoseRateEx3(QEpd *me);  // 在原来最早算法基础上修改
   void ScaleComputedoseEX(QEpd * me);//采用新算法的刻度剂量率计算函数
   float GetCPS();
   */
ENDCLASS
   

#ifdef __cplusplus
}
#endif
#endif
