#include "iic.h"

#if IIC_MODE == 0

	u8  ack = 0;

	void IIC_Init()				//IIC初始化
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


	void IIC_START(void)	//IIC开始
	{
		SCL = 1;
		SDA	= 1;
		IIC_DELAY();
		SDA	= 0;
		IIC_DELAY();
		SCL = 0;
		IIC_DELAY();
	}


	void IIC_STOP(void)		//IIC结束
	{
		SCL = 0;
		SDA	= 0;
		IIC_DELAY();
		SCL = 1;
		IIC_DELAY();
		SDA	= 1;
		IIC_DELAY();
	}


	void IIC_SENDACK(void)	//发送ACK
	{
		SDA	= 0;
		IIC_DELAY();
		SCL = 1;
		IIC_DELAY();
		SCL = 0;
		IIC_DELAY();
	}


	void IIC_SENDNACK(void)	//发送NACK
	{
		SDA	= 1;
		IIC_DELAY();
		SCL = 1;
		IIC_DELAY();
		SCL = 0;
		IIC_DELAY();
	}

	void IIC_WAITACK(void)	//等到从机ACK
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

	void IIC_SENDBYTE(u8 dat)	//发送一个字节
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

	u8 IIC_READBYTE(void)	//读取一个字节
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


	//--------------------------------功能函数--------------------------------
	void IIC_Write_NByte( u8 slave,u8 addr,u8 *p,u8 number )	//iic写入连续的几个字节
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

	// 发送开始命令 发送器件地址（写） 发送数据地址 发送开始命令 发送器件地址（读）读数据
	void IIC_Read_NByte( u8 slave,u8 addr,u8 *p,u8 number )	//iic读取连续的几个字节
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
	void IIC_Init()				//IIC初始化
	{
		I2C_S1 = 0;				//设置IIC的引脚
		I2C_S0 = 1;
		I2CCFG = 0XE0;			//设置速度和使能IIC
		I2CMSST = 0;				//清空主机状态
	}

	void IIC_DELAY()		//@22.1184MHz 2US
	{
		while( !(I2CMSST & 0X40) );
		I2CMSST &= (~0X40);
	}


	void IIC_START(void)	//IIC开始
	{
		I2CMSCR = 0X01;			//开始命令
		IIC_DELAY();			//等待结束
	}


	void IIC_STOP(void)		//IIC结束
	{
		I2CMSCR = 0X06;			//结束命令
		IIC_DELAY();
	}


	void IIC_SENDACK(void)	//发送ACK
	{
		I2CMSST = 0X00;			//发送aCK
		I2CMSCR = 0X05;			//aCK命令
		IIC_DELAY();
	}


	void IIC_SENDNACK(void)	//发送NACK
	{
		I2CMSST = 0X01;			//发送naCK
		I2CMSCR = 0X05;			//aCK命令
		IIC_DELAY();
	}

	void IIC_WAITACK(void)	//等到从机ACK
	{
		I2CMSCR = 0X03;			//接收aCK
		IIC_DELAY();
	}

	void IIC_SENDBYTE(u8 dat)	//发送一个字节
	{
		I2CTXD = dat;			//发送的数据写入寄存器
		I2CMSCR = 0X02;			//发送
		IIC_DELAY();
	}

	u8 IIC_READBYTE(void)	//读取一个字节
	{
		I2CMSCR = 0X04;			//发送
		IIC_DELAY();		
		return I2CRXD;
	}


	//--------------------------------功能函数--------------------------------
	void IIC_Write_NByte( u8 slave,u8 addr,u8 *p,u8 number )	//iic写入连续的几个字节
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

	// 发送开始命令 发送器件地址（写） 发送数据地址 发送开始命令 发送器件地址（读）读数据
	void IIC_Read_NByte( u8 slave,u8 addr,u8 *p,u8 number )	//iic读取连续的几个字节
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