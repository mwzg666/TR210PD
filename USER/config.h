#ifndef  _CONFIG_H_
#define _CONFIG_H_
#ifdef __cplusplus
extern "C" {
#endif

#if 0
//#define DEBUG   /* close debug when debug IrDA,open uart 19200  */ 
#define RELEASE  /* Release mode open the IrDA  */

#define EXPERIMENT_VER                                   0
#define FINISH_VER                                       1

//#define RELEASE_VER                                      EXPERIMENT_VER


//#define FIRST_PROGRAM   //第一次烧录代码下载参数则要打开此处
#define CC2500_REC_MODE  0
#define CC2500_SEND_MODE 1
#define KEEP_SLEEP_TIME  15   //单位:S     12.5


#define P1_INT_C_CODE 0
#define P1_NIT_ASM_CODE 1
#define P1_INT_CODE_TYPE P1_NIT_ASM_CODE
#define CC2500_WORK_MODE CC2500_SEND_MODE
#define TREND_RECORD_MAX  800
#ifdef RELEASE
#define CC2500_SWITCH_DELAY  1//500//60  //Ok: 3000 KK
#elif defined(DEBUG)
#define CC2500_SWITCH_DELAY 1
#endif

#endif

/* dose rate 计算策略参数配置  */
#define SMOOTH_LOW_LENGTH             120//原先是100,define low smooth window size
#define SMOOTH_MID_LENGTH             30//30//define midle smooth window size
#define SOMMTH_MAX_LENGTH             60
#define SMOOTH_HIGH_LENGTH            10//define high smooth window size
#define FIRST_CPS_MEASURE_TIME        120  /* background meausre time in s *///原先是100
#define FIRST_CPS_MEAUSRE_THSHOLD     5 //define the first cps measure thshold value
#define MIN_FILTER_WIN_SIZE           5
#define SWITCH_BUF_SZIE               10
#define HIGH_DOSE_REATE_MODE_MAX_WIN  30
#define LOW_CPS_THRE                  0.8
#define MIDDLE_DOSE_RATE_THRE         55// 55  // CPS 

#define LOW_DOSE_RATE_MODE_THR        10
#define LOW_DOSE_RATE_MODE_MAX_WIN    10//20
#define LOW_DOSE_RATE_MODE_CPS_FITERBUF_SZIE  5
#define FILTER_LIMIT_AMPLITUDE_THRESHOLD   (0.10)
#define SWITCH_THR                     0.50

//#define SECOND_CPS_MEASURE_TIME      30  /* in s */
#define SECOND_CPS_MEASURE_TIME       5   /* in s */
#define SECOND_CPS_MEASURE_THSHOLD  10

#define THIRD_CPS_MEASURE_TIME      1   /* in s */

#define DOSERATE_ALARM_CNT_THRESHOLD 2
#define DOSERATE_WARNNING_CNT_THRESHOLD 2


#define FILTER_HAKE_CPS_THRESHOLD 10

/* new dose rate algorithm */
#define CPS_SOOMTH_WINDOW_SIZE             5
#define BACKGROUND_TIME_PER_CPS            120
#define FILTER_CPS_QUE_SIZE                3

#define CPS_ALGORITHM_1                    1
#define CPS_ALGORITHM_2                    2
#define CPS_ALGORITHM                      CPS_ALGORITHM_1

#define CIRCLE_TO_TIME_CNT_THRESOLD        1
//#define CPS_SIMULATE 

/* 配置开机是否对探测器自检,TURE为进行自检，FALSE为不进行自检 */
#define DETECTOR_SELCHECK                 TRUE

#ifdef __cplusplus
}
#endif
#endif
