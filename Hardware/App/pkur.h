#ifndef __PKUR_H__
#define __PKUR_H__

void Pkur_IoInit(void);
void Pkur_HwInit(void);
void SGetCount(unsigned char *dat);
void SMeasreStart(unsigned char dat);
void SSetLimen(unsigned char *datain);
void SCLR(void);
void Pkur_Init(unsigned char Channel);



#endif

