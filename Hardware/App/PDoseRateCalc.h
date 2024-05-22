#ifndef PDOSERATECALC_H
#define PDOSERATECALC_H

/*
A�� 50 715  38762
B��54  841
C��52  764
D��53  800
*/
#define LOW_MID_THR  50
#define MID_HIGH_THR  715
#define HIGH_HIGHEST_THR  38762

float DoseRateCalc(float count);

//double CalcDoseRate(double count,int LowCoeffState,QComPara *me);
//double QEpdDoseRateCalc(QEpd * me, double count, int LowCoeffState); 
double QEpdDoseRateCalc();

#endif 
