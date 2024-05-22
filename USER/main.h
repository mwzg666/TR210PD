#ifndef __MAIN_H__
#define __MAIN_H__

//#include "STC32G.h"
//#include "stdio.h"
//#include "intrins.h"
//#include "string.h"
//#include "stdlib.h"
//#include "config.h"

#include "appconfig.h"
//===================================定义=================================


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
#define LOW_POWER_TIMER  10000  // 进入低功耗的时间，按毫秒计算
#define UPDATE_TIMER_TIMEOUT  1000  // in ms
#define ACTIVE_WAIT_TIME 15//in s,激活后等待时间
#define TIME_OUT_RETURN_VALUE  10//in s, when time out return mesure disp

#define DOSERATE_OVERLOAD_VALUE        10000
#define DOSESUMX_OVERLOAD_VALUE        10000

//#define INCREASE_ALARM_VALUE        20    //累积剂量率增长报警,uSv,暂时用宏定义，后期用读出仪给剂量计设置
/* 报警参数 */
#define ALARM_TYPE_BATTERY_ALARM     (1<<0)        //电量报警
#define ALARM_TYPE_SUMDOSE_ALARM     (1<<1)        //剂量当量报警
#define ALARM_TYPE_SUMDOSE_MEDDLE     (1<<2)        //剂量当量预报警
#define ALARM_TYPE_DOSERATE_ALARM     (1<<3)        //剂量当量率报警
#define ALARM_TYPE_DOSERATE_MEDDLE     (1<<4)        //剂量当量率预警
#define ALARM_TYPE_USEDTIME_ALARM    (1<<5)        //累积时间报警
#define ALARM_TYPE_SUMDOSE_OVERLOAD  (1<<6)        //剂量当量过载报警（Overflow）
#define ALARM_TYPE_DOSERATE_OVERLOAD (1<<7)        //剂量当量率过载报警（Overflow）
/* 故障参数 */
#define FAULT_TYPE_INITIALIZE          (1<<0)        //初始化故障
#define FAULT_TYPE_DETECTOR          (1<<1)        //探头故障
#define FAULT_TYPE_DATA              (1<<2)        //数据故障
#define FAULT_TYPE_STORAGE              (1<<3)        //存储故障

/* 运行模式 */
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

#pragma pack(1)    //按字节对齐
typedef struct _QEpdDoseFactor
{
    //u32 dwDeviceNo;
    u8  experimentmode;//实验模式开关(打开CPS等) 1:打开，0关闭
    //u8  resv[3];
    //float lowfactor[16];//长度保存与821一致，保证通信
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
    char    szSerialNumber[16];    //出厂编号
    char    szFactoryDate[8];    //出厂时间
    char    szFirmwareVerNo[16];//固件版本
    char    szHardwareVerNo[16];//硬件版本
}QEpdFactoryInfo,*PQEpdFactoryInfo;


//设备分配信息
typedef struct _QEpdAssignInfo{
    char      szUserID[18];            //人员ID号
    char      szUserName[8];            //人员姓名
    char      szTaskID[6];            //任务号
    u32 dwAssignTime;            //剂量计分配时间
}QEpdAssignInfo,*PQEpdAssignInfo;

#endif


//设备状态信息
typedef struct _QEpdStatusInfo
{
    //u16    iStorageMax;            //最大数据存储容量       
    //u16    iStorageUsed;            //使用存储容量        
    float    iLastDoseSum;                //上次剂量当量,浮点型,累积剂量当量
    float    iMaxDoseRate;                //最大剂量当量率，浮点型
    u32    iUsedTime;                    //单次使用时间，累计测量时间
    //u8    ucFaultFlag;
        /*
             0:    初始化故障
             1:    探头故障
             2:    数据故障
             3:    存储故障        
        */
    //u8    ucAlarmFlag;
        /*
             0:    电量报警
             1:    剂量当量报警
             2:    剂量当量预报警
             3:    剂量当量率报警
             4:    剂量当量率预报警
             5:    累积时间报警
             6:    剂量当量过载报警（Overflow）
             7: 剂量当量率过载报警（Overflow）        
        */
    //u8    resv[2];
}QEpdStatusInfo,*PQEpdStatusInfo;



#pragma pack()

#if 0
//运行模式
/*
     0: 剂量计是否分配
     1: 工作模式,0:联机 1: 手动
     2: 激活/关闭状态,0:未激活,1: 激活
     3: 节能模式, 1:睡眠模式 0:不是睡眠模式
 4 - 6: 监测声音间隔
      7: LED方式
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

//模式设置
/*
     0: 开始测量状态, 1:清除报警位，0:保持报警位
 1 - 2: 趋势存储模式, 00:关闭，01:报警触发，10:间隔存储
 3 - 5: 趋势存储间隔, 000:15秒，001:30秒         
 6 - 7: 背光方式
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
         0 - 2: 通讯速率
             3: 存储模式
             4: 默认测量
         5 - 7: 测量显示单位
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
//参数设置信息
typedef struct _QEpdBaseConfig{
    //报警参数信息
    //u32    iDoseSumxMeddle;            //剂量当量干预值 *0.1uSv
    //u32    iDoseSumxAlarm;                //剂量当量报警值 *0.1uSv
    //u32    iDoseRateMeddle;            //剂量当量率干预值 *0.1uSv/h
    //u32    iDoseRateAlarm;                //剂量当量率报警 *0.1uSv/h
    float        iScaleFactor;                //趋势数据校正因子
    //QLocalTime AdjustTime;                    //趋势数据校准时间
    //u16    iUsedTimeAlarm;                //使用时间报警值,分钟

    //float     iHolelifeSumDose;                //生命周期总计量
    //u8    ucRunningMode;
        /*
             0: 剂量计是否分配
             1: 工作模式,0:联机 1: 手动
             2: 激活/关闭状态,0:未激活,1: 激活
             3: 节能模式, 1:睡眠模式 0:不是睡眠模式
         4 - 6: 监测声音间隔
              7: LED方式
        */
    //u8    ucModeSetting;
        /*
             0: 开始测量状态, 1:清除报警位，0:保持报警位
         1 - 2: 趋势存储模式, 00:关闭，01:报警触发，10:间隔存储
         3 - 5: 趋势存储间隔, 000:15秒，001:30秒         
         6 - 7: 背光方式
        */
    //u8    ucDisplayMode;
        /*   
         0 - 2: 通讯速率
             3: 存储模式
             4: 默认测量
         5 - 7: 测量显示单位
       */
    //u16 ucDoseIncreaseAlarm;//剂量当量增长报警，0不报警; 5:4CPS; 4:8CPS; 3:1ucV; 3:10ucV; 1:100ucV;2016.4.18新增,必须两个字节，不然存储有对齐问题
    //u8 resv[3];
}QEpdBaseConfig,*PQEpdBaseConfig;

#if 0
//趋势数据
typedef struct _QEpdTrendData {
    float doseRate;        //剂量率
    float doseSumx;        //累计剂量
    u32 timeStamp;
    u8 doseType;    
}QEpdTrendData,*PQEpdTrendData;
#endif

typedef struct _QEpdParaInfo
{
    u32        dwSignature;

    #if 0
    //u32          dwDeviceNo;            //剂量计编号,   4Bytes    
    union _DWDOSENUM
    {
        unsigned long dwDeviceNo;
        struct _DW_DSOE_NUM
        {
             unsigned long MainID:20;
             unsigned long ver:4;        //仪器种类和版本号
             unsigned long unit:4;    //单位编号
        }tdwDeviceNo;
    }DoseNum;
    #endif
    
    //QEpdFactoryInfo factoryInfo;        //厂家参数,只读,不允许更改
    //QEpdAssignInfo    assignInfo;            //设备分配信息,40Bytes
    QEpdStatusInfo    statusInfo;            //设备状态信息,20Bytes
    QEpdBaseConfig    baseConfig;            //设备配置信息,32Bytes
    QEpdDoseFactor  tDoseFactor;        //拟合参数信息,68Bytes = 16x4 + 1 + 3(保留)
}QEpdParaInfo,QEpdPara;


#define HMI_FILE_SIGNATURE      0x00103011    //配置签名<4字节>

#define FLASH_SIGNATURE_OFFSET    0

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
            unsigned long ver:4;        //仪器种类和版本号
            unsigned long unit:4;    //单位编号
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
    //u8 ucDoseIncreaseAlarm;//剂量当量增长报警，0不报警; 1:4CPS; 2:8CPS; 3:1ucV; 4:10ucV; 5:100ucV;
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
    QEpdPara EpdPara;        //设备参数

    //u8 Buffer[512];//接受缓冲区

    u16 RecvSize;
    //u16 lastDoseState;        //前一次的状态

    //u16 doseAlarmCnt;
    //u16 doseWarnxCnt;
    
    float fDoseRate;    //剂量当量率 mSv/h
    float fDoseSumx; //累积剂量   mSv
    float fDoseSumIncrease;//累积剂量增长量，大于多少时报警
    //u32 sumDoseIncrease;//累计剂量计数增长，，大于多少时报警
    //u16 ActiveWaitTime;//激活后等待时间，时间为0了才能开始测量
    //u32 AlarmRestTime;//报警剩余时间 = (剂量累计预警-累积剂量）/剂量当量率
    //float fDoseRate;    //剂量当量率
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
    //u32 scaleTimerBCK;    //刻度用时间
    //u32 scaleTimer;    //刻度用时间
    //u32 iSumCPS;        //刻度总计数
   
    //菜单显示部分
    //u8 subItemDispIndex;//子菜单显示索引
    //u8 topItemDispIndex;//顶层菜单显示索引
    //u8 setParaDispIndex;//参数设置显示索引
    //u8 selParaDispIndex;//参数选择显示索引
    
    //u8 paraSelIndex;     //参数选择索引    
    //u8 dispParaEnable;  //联网模式,激活等待状态,参数显示使能
    //u8 bDispMenuType;
    //u8 alarmState;         //报警状态
    
    //u8 faultState;         //故障状态
    //u8 alarmFlags;         //报警标识
    
    //u8 quickWatchIndex; //快速查看参数索引
    
    //u8 dispMenuType;    /* top menu(dose rate item, dose item, disp par ect), disp par menu */
    //u8 bStartupMeasure;

    //u8 lastUnitIcoIndex;
    //u8 lastItemIcoIndex;
    //u8 lastTypeIcoIndex;
    //u8 lastvBatIcoIndex;
    
    //u8 isAlarmbeepOn;//蜂鸣器图标是否显示0不显示，1显示
    //u8 isAlarmMotorOn;//震动图标是否显示0不显示，1显示

    //超时参数
    u8 timerCPSCnt;            //CPS, 30秒计数
    u16 updateBatCnt;        //电池状态更新计数
    //u8 iCurAlarm;
    //u8 lastAlarm;
    
    //u32 syncbitmap;     //保存数据时用的同步bitmap
    //u32 iUsedTime;
    //u32 trendSaveCount;
    //u32 trendSaveTime;    //趋势数据保存时间

    //u32 timerCnt;
    u32 sumDoseCount;        //累计剂量计数
    u32 cpsDoseCount;        //CPS计数
    u32 cpsDosePerCnt;        //30秒CPS计数

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
    //u8  byPulseFlag;                      /* 0: 脉冲周期开始， 1: 脉冲周期结束 */
    //u8  byPulseMode;                      /* 0: 正常。 1:pre-change            */

    //u8  byPulseChFlg; 
    //u8  byPulseChCPS;
    //u8  KeyOffAlarm;
    //u8  alarmLedEnable;   //报警灯状态
    //u8  alarmIcoEnable;   //报警图标状态
    //u8  alarmBatEnable;
    //u16 dwPluseCircle;
    //u8  bGetPkuPer500ms;//1500MS读一次数据
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
    u8  bSwitchFlag;   //0: 还没有切换，1:低切换，2:高切换
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





//==================================变量声明=============================
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


//==================================函数声明=============================

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






