#include "main.h"
#include "pkur.h"
#include "time.h"
//PUK_RST  -- P3.0
//PKU_AE1 -- P3.7
//PKU_AE2 -- P1.0
//PUK_CONT_EN -- P3.6
//PKU_CLK -- P3.3
//PKU_MOSI -- P3.1
//PKU_MISO -- P3.2


//#define Spi_Sck1   (P3OUT |=  BIT3)
//#define Spi_Sck0   (P3OUT &=(~BIT3))
//#define Spi_Si1    (P3OUT |=  BIT1)
//#define Spi_Si0    (P3OUT &=(~BIT1))   
//#define Spi_So     ((P3IN>>2) & 0X01) 
//
//#define PUK_RST1   (P3OUT |= BIT0)
//#define PUK_RST0   (P3OUT &= ~BIT0)
//#define PUK_AE1_1   (P3OUT |= BIT7)
//#define PUK_AE1_0   (P3OUT &= ~BIT7)
//#define PUK_AE2_1   (P1OUT |= BIT0)
//#define PUK_AE2_0   (P1OUT &= ~BIT0)


#define PUK_EN1    (P3OUT |= BIT6)
#define PUK_EN0    (P3OUT &= ~BIT6)


extern QPara Sys_Para;




/*******************************************************************************************************************
** 函数名称: void SD_HardWareInit()                Name:      void SD_HardWareInit()
** 功能描述: 初始化访问SD卡的硬件条件            Function: initialize the hardware condiction that access sd card
** 输　入: 无                                    Input:      NULL
** 输　出: 无                                    Output:      NULL
********************************************************************************************************************/
void Pkur_HwInit(void)
{
    //P5DIR |= BIT6;
    //P3OUT &= (~BIT1)&(~BIT3);//IO设置
    //P3DIR |=  BIT0+BIT1+BIT3;//IO设置
    PKU_RST(1);       // high reset
    delay_ms(10);
    PKU_RST(0);      
    
    PKU_AE1(1);  
    PKU_AE2(0);   
        
    PKU_MOSI(0);
    
    COUNT(1);

    #if 0
    Spi_Sck0;
    Spi_Sck1;
    Spi_Sck0;
    Spi_Sck1;
    Spi_Sck0;
    PUK_EN0;
    #endif
}
/*******************************************************************************************************************
** 函数名称: void SPI_SendByte()                Name:      void SPI_SendByte()
** 功能描述: 通过SPI接口发送一个字节            Function: send a byte by SPI interface
** 输　入: INT8U byte: 发送的字节                Input:      INT8U byte: the byte that will be send
** 输　出: 无                                    Output:      NULL
********************************************************************************************************************/
unsigned char SPI_SendByte(unsigned char byte)
{

    unsigned char i,j;
    j=0;
    //P3IN |=BIT2;
    PKU_CLK(0);
    for(i=0;i<8;i++)           //"发数据
    { 
        if(byte&0x80)
        {
            PKU_MOSI(1);;
        }
        else 
        {
            PKU_MOSI(0);;
        }
        byte=byte<<1;

        #if 0
        for(int z = 0;z < 60;z++)//60
        {
            _NOP();                 //"根据芯片速度与单片机时钟调整延时
        }
        #endif
        
        PKU_CLK(1);
        //_NOP();
        //_NOP();
        PKU_CLK(0);//下降沿读数据
        j=j<<1;
        if(PKU_MOSO())
        {
            j=j|0x01;     //"读数据
        }
    }
    return j;
}
/*******************************************************************************************************************
** 函数名称: INT8U SPI_RecByte()                Name:      INT8U SPI_RecByte()
** 功能描述: 从SPI接口接收一个字节                Function: receive a byte from SPI interface
** 输　入: 无                                    Input:      NULL
** 输　出: 收到的字节                            Output:      the byte that be received
********************************************************************************************************************/
unsigned char SPI_RecByte(void)
{
    return SPI_SendByte(0XFF);
}
//2.2   芯片驱动
/************************************************************************************************/
/*指令编码                                                                                      */
/************************************************************************************************/
//设置通道开关
const unsigned char  tunnel_SW[2] = {0XD8,0X00};
//设置通道阈值
const unsigned char  tunnel_WriteLM[] = {0XC0,0XC8,0XD0};
//将三个通道计数器清零
const unsigned char  tunnel_CLR[] = {0XE0,0XE8,0XF0};
//读取三个通道计数
const unsigned char  tunnel_READ[4][2] = {{0X20,0X00},{0X24,0X00},{0X28,0X00},{0X2C,0X00}};
/************************************************************************************************/
//读取三个通道计数
//data:读到的计数值所在内存区首地址,大小为6字节；
//第0和1字节为第一通道的计数值，计数值为16位无符号整型数，
//                                 第0字节为计数值的低8位，第1字节为高8位；
//第2和3字节为第二通道的计数值，格式同上；
//第4和5字节为第三通道的计数值，格式同上；
/************************************************************************************************/
void SGetCount(unsigned char *dat)
{
    int i;
    unsigned char databuf[2];
    for( i = 0;i < 3;i++)
    {
        SPI_SendByte(tunnel_READ[i][0]);
        SPI_SendByte(tunnel_READ[i][1]);
        databuf[0] = SPI_SendByte(0X00);
        databuf[1] = SPI_SendByte(0X00);
        SPI_SendByte(0X00);
        SPI_SendByte(0X00);
        dat[i*2] = (databuf[1]);
        dat[i*2+1] = (databuf[0]);        
    }    
}

/************************************************************************************************/
//将三个通道计数器清零
/************************************************************************************************/
void SCLR(void)
{
    int i;
    for(i = 0;i < 3;i++)
    {
        SPI_SendByte(tunnel_CLR[i]);
        SPI_SendByte(tunnel_READ[i][0]);
        SPI_SendByte(tunnel_READ[i][1]);
        SPI_SendByte(0X00);
        SPI_SendByte(0X00);
        SPI_SendByte(0X00);
        SPI_SendByte(0X00);
    }
}

/************************************************************************************************/
//打开通道
//data:要打开的通道号
//04第一通道；BIT2
//02第二通道；BIT1
//01第三通道；BIT0
/************************************************************************************************/
void SMeasreStart(unsigned char dat)
{
    SPI_SendByte(tunnel_SW[0]|(dat&0X07));
    SPI_SendByte(tunnel_SW[1]);
}

/************************************************************************************************/
//关闭所有通道
/************************************************************************************************/
void SMeasreStop(void)
{
    SPI_SendByte(tunnel_SW[0]);
    SPI_SendByte(tunnel_SW[1]);
}

/************************************************************************************************/
//设置阈值
//datain:要设置阈值的所在的内存首地址，长度为3个字节
//第0、1、2字节分别为第一、二、三通道阈值
/************************************************************************************************/
void SSetLimen(unsigned char *datain)
{
    int i;
    for(i = 0;i < 3;i++)//设置阈值
    {
        SPI_SendByte(tunnel_WriteLM[i]|(datain[i]>>5));
        SPI_SendByte(datain[i]<<3);
    }

}


void Pkur_Init(BYTE Channel)
{
    //BYTE dd[3] = {10,10,150};
    Pkur_HwInit();
    PkurSw(SysParam.AoutSw);

    SMeasreStart(Channel);
    SSetLimen((BYTE *)SysParam.s_Thr.DET_THR);

    SCLR();
}

