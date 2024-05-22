#include "iic.h"

#if IIC_MODE == 0

	u8  ack = 0;

	void IIC_Init()				//IIC��ʼ��
	{
		SCL = 1;
		SDA	= 1;
	}

	void IIC_DELAY()		//@22.1184MHz 2US
	{
		unsigned long i;

		_nop_();
		_nop_();
		_nop_();
		i = 9UL;
		while (i) i--;
	}


	void IIC_START(void)	//IIC��ʼ
	{
		SCL = 1;
		SDA	= 1;
		IIC_DELAY();
		SDA	= 0;
		IIC_DELAY();
		SCL = 0;
		IIC_DELAY();
	}


	void IIC_STOP(void)		//IIC����
	{
		SCL = 0;
		SDA	= 0;
		IIC_DELAY();
		SCL = 1;
		IIC_DELAY();
		SDA	= 1;
		IIC_DELAY();
	}


	void IIC_SENDACK(void)	//����ACK
	{
		SDA	= 0;
		IIC_DELAY();
		SCL = 1;
		IIC_DELAY();
		SCL = 0;
		IIC_DELAY();
	}


	void IIC_SENDNACK(void)	//����NACK
	{
		SDA	= 1;
		IIC_DELAY();
		SCL = 1;
		IIC_DELAY();
		SCL = 0;
		IIC_DELAY();
	}

	void IIC_WAITACK(void)	//�ȵ��ӻ�ACK
	{
		SDA	= 1;
		IIC_DELAY();
		SCL = 1;
		IIC_DELAY();
		ack = SDA;
		IIC_DELAY();
		SCL = 0;
		IIC_DELAY();
	}

	void IIC_SENDBYTE(u8 dat)	//����һ���ֽ�
	{
		u8 i=8;
		do
		{
			if( dat& 0x80 )
				SDA	= 1;
			else
				SDA	= 0;
			IIC_DELAY();
			dat<<=1;
			SCL = 1;
			IIC_DELAY();	
			SCL = 0;
			IIC_DELAY();		
		}
		while(--i);
	}

	u8 IIC_READBYTE(void)	//��ȡһ���ֽ�
	{
		u8 i=8,dat=0;
		SDA	= 1;
		do
		{
			SCL = 1;
			IIC_DELAY();
			dat<<=1;
			if( SDA )
				dat |= 1;
			SCL = 0;
			IIC_DELAY();
		}
		while(--i);
		
		return dat;
	}


	//--------------------------------���ܺ���--------------------------------
	void IIC_Write_NByte( u8 slave,u8 addr,u8 *p,u8 number )	//iicд�������ļ����ֽ�
	{
		IIC_START();
		IIC_SENDBYTE(slave);
		IIC_WAITACK();
		if( !ack )
		{
			IIC_SENDBYTE(addr);
			IIC_WAITACK();
			if( !ack )
			{
				do
				{
					IIC_SENDBYTE(*p);
					p++;
					IIC_WAITACK();
					if( ack )
						break;
				}
				while(--number);
			}
		}
		IIC_STOP();
	}

	// ���Ϳ�ʼ���� ����������ַ��д�� �������ݵ�ַ ���Ϳ�ʼ���� ����������ַ������������
	void IIC_Read_NByte( u8 slave,u8 addr,u8 *p,u8 number )	//iic��ȡ�����ļ����ֽ�
	{
		IIC_START();
		IIC_SENDBYTE(slave);
		IIC_WAITACK();
		if( !ack )
		{
			IIC_SENDBYTE(addr);
			IIC_WAITACK();
			if( !ack )
			{
				IIC_START();
				IIC_SENDBYTE(slave+0x01);	
				IIC_WAITACK();
				if( !ack )
				{
					do
					{
						*p = IIC_READBYTE();	//P[0] => P[1]
						p++;
						if( number!=1 )
							IIC_SENDACK();
					}
					while(--number);
					IIC_SENDNACK();
				}
			}		
		}
		IIC_STOP();	
	}
#else
	void IIC_Init()				//IIC��ʼ��
	{
		I2C_S1 = 0;				//����IIC������
		I2C_S0 = 1;
		I2CCFG = 0XE0;			//�����ٶȺ�ʹ��IIC
		I2CMSST = 0;				//�������״̬
	}

	void IIC_DELAY()		//@22.1184MHz 2US
	{
		while( !(I2CMSST & 0X40) );
		I2CMSST &= (~0X40);
	}


	void IIC_START(void)	//IIC��ʼ
	{
		I2CMSCR = 0X01;			//��ʼ����
		IIC_DELAY();			//�ȴ�����
	}


	void IIC_STOP(void)		//IIC����
	{
		I2CMSCR = 0X06;			//��������
		IIC_DELAY();
	}


	void IIC_SENDACK(void)	//����ACK
	{
		I2CMSST = 0X00;			//����aCK
		I2CMSCR = 0X05;			//aCK����
		IIC_DELAY();
	}


	void IIC_SENDNACK(void)	//����NACK
	{
		I2CMSST = 0X01;			//����naCK
		I2CMSCR = 0X05;			//aCK����
		IIC_DELAY();
	}

	void IIC_WAITACK(void)	//�ȵ��ӻ�ACK
	{
		I2CMSCR = 0X03;			//����aCK
		IIC_DELAY();
	}

	void IIC_SENDBYTE(u8 dat)	//����һ���ֽ�
	{
		I2CTXD = dat;			//���͵�����д��Ĵ���
		I2CMSCR = 0X02;			//����
		IIC_DELAY();
	}

	u8 IIC_READBYTE(void)	//��ȡһ���ֽ�
	{
		I2CMSCR = 0X04;			//����
		IIC_DELAY();		
		return I2CRXD;
	}


	//--------------------------------���ܺ���--------------------------------
	void IIC_Write_NByte( u8 slave,u8 addr,u8 *p,u8 number )	//iicд�������ļ����ֽ�
	{
		IIC_START();
		IIC_SENDBYTE(slave);
		IIC_WAITACK();

		IIC_SENDBYTE(addr);
		IIC_WAITACK();

		do
		{
			IIC_SENDBYTE(*p);
			p++;
			IIC_WAITACK();

		}
		while(--number);
		IIC_STOP();
	}

	// ���Ϳ�ʼ���� ����������ַ��д�� �������ݵ�ַ ���Ϳ�ʼ���� ����������ַ������������
	void IIC_Read_NByte( u8 slave,u8 addr,u8 *p,u8 number )	//iic��ȡ�����ļ����ֽ�
	{
		IIC_START();
		IIC_SENDBYTE(slave);
		IIC_WAITACK();

		IIC_SENDBYTE(addr);
		IIC_WAITACK();

		IIC_START();
		IIC_SENDBYTE((u8)(slave+0x01));	
		IIC_WAITACK();

		do
		{
			*p = IIC_READBYTE();	//P[0] => P[1]
			p++;
			if( number!=1 )
				IIC_SENDACK();
		}
		while(--number);
		IIC_SENDNACK();
		IIC_STOP();	
	}

	
#endif