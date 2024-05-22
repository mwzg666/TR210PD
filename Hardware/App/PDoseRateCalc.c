
#include "main.h"

#if 1

//#include "Epd.h"
#include "PDoseRateCalc.h"

#if 0
#define LK4  0.3109221812e-4
#define LK3  -0.4082361904e-2
#define LK2  0.1426728048
#define LK1  10.11181609
#define LK0  -0.2014007739

#define MK3  0.1277080700e-8
#define MK2  0.9200505397e-4
#define MK1  10.46742896
#define MK0  50.06881742

#define HK3  0.1135803289e-7 
#define HK2  -0.6820468170e-3
#define HK1  34.01547122
#define HK0  -152106.7571
#else
#define LK4  0.3109221812e-4
#define LK3  0.2187706170e-3
#define LK2  -0.3012745075e-1
#define LK1  11.33073355
#define LK0  0.9305976888

#define MK3  -0.3547875997e-6
#define MK2  0.3027908355e-2
#define MK1  8.903399278
#define MK0  97.58780370

#define HK3  0.1723497784e-8 
#define HK2  0.1595624034e-4
#define HK1  10.80373834
#define HK0  6803.227777
#endif

#define HighDR(x)  HK0 + x * HK1 + x * x * HK2 + x * x * x * HK3
#define MidDR(x)   MK0 + x * MK1 + x * x * MK2 + x * x * x * MK3
#define LowDR(x)   LK0 + x * LK1 + x * x * LK2 + x * x * x * LK3 // + x * x * x * x * LK4

extern float fAverCPS;
extern float CpsHis;

float DoseRateCalc(float count)
{
    static float DRreturn = 0.0f;
    //static float count;
    //double count2 = count * count;
    //double count3 = count2 * count;
    unsigned char State;

    //count = fAverCPS;
    //CpsHis = fAverCPS;

    if(count >= 8398)
    {
        State = 2;  //high
    }
    else if(count >= 75)
    {   
        State = 1;    //mid
    }
    else //if (count < 75)
    {
        State = 0;  //low
    }


    if (count >= 0.14) // 1.3uSv/h
    {
        switch(State)
        {
            case 0:  //低剂量                
                DRreturn = LowDR(count);
            break;
                
            case 1:
                DRreturn = MidDR(count);
            break;
                
            case 2:
                DRreturn = HighDR(count);
            break;
        } 

        //DRreturn = DRreturn / 1000.0; // to mSv
    }
    else
    {
        #if 0
        TRACE(("background dose rate mode......\n"));
        //由于2014年下半年探测器一致性不好，有些探测器在本底时候计数偏大，
        //导致本底情况下有些EPD剂量率在400-500nSv/h,这时的CPS在0.031034左右
        // 如果把这个计数带入 DRreturn = 0.000099 + count * 0.01131; 计算结果在450nSv/h,导致本底显示偏大
        // 通过修改count <= 0.053138。 CPS在0.031034左右 带入 DRreturn = 0.000099 + count * 0.001; 计算
        // 结果在130nSv/h.能接受。 0.053138 CPS剂量率在152nSv;
        // EPD测量范围在1uSv以上，所以1uSv以下没有多大影响,152 到 700 nSv中间不用考虑
        // if (count <= 0.03)
        //if (count <= 0.053138)  
        if (count <= 0.09734)
        {
           //计算结果在99nSv 到 152 nSv
           DRreturn = 0.000099 + count * 0.001;  //mSv
        }
        else
        {
            //计算结果700nSv 到 1.68uSv/h
           //DRreturn = 0.000099 + count * 0.01131;  // mSv 
            DRreturn = 0.000099 + count * 0.0099347;  // mSv 
        }
        #else
        // count <0.14(0.001473mSv/h) ,敏感度很低，使用下面计算
        // 在count =0.139, DRreturn = 0.000169 mSv
        //DRreturn = 0.000099 + count * 0.0005;  //mSv
        DRreturn = 0.099 + count * 0.0005;  //uSv
        #endif
    }
    return (DRreturn <= 0 ? 0:DRreturn);
}

//double CalcDoseRate(double count,int LowCoeffState, QComPara *me)
double QEpdDoseRateCalc()
{
  static float dosrate=0;
  dosrate=(double)DoseRateCalc(fAverCPS);
  return dosrate;
}

#endif

