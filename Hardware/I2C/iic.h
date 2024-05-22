#ifndef __IIC_H
#define __IIC_H

#include "COMM/stc.h"		//����ͷ�ļ�
#include "COMM/stc32_stc8_usb.h"

#define  IIC_MODE     1   //0�����IIC   1;Ӳ��IIC
#define  SLAVE_24C02	0XA0

//------------------------------���Ŷ���------------------------------
#define  SCL P25
#define  SDA P24

//------------------------------��������------------------------------


//------------------------------��������------------------------------
void IIC_DELAY();				//@22.1184MHz 2US
void IIC_START(void);			//IIC��ʼ
void IIC_STOP(void);			//IIC����
void IIC_SENDACK(void);			//����ACK
void IIC_SENDNACK(void);		//����NACK
void IIC_WAITACK(void);			//�ȵ��ӻ�ACK
void IIC_SENDBYTE(u8 dat);		//����һ���ֽ�
u8 IIC_READBYTE(void);			//��ȡһ���ֽ�
void IIC_Init();				//IIC��ʼ��

void IIC_Write_NByte( u8 slave,u8 addr,u8 *p,u8 number );	//iicд�������ļ����ֽ�
void IIC_Read_NByte( u8 slave,u8 addr,u8 *p,u8 number );	//iic��ȡ�����ļ����ֽ�

#endif



