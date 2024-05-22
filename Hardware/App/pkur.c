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
** ��������: void SD_HardWareInit()                Name:      void SD_HardWareInit()
** ��������: ��ʼ������SD����Ӳ������            Function: initialize the hardware condiction that access sd card
** �䡡��: ��                                    Input:      NULL
** �䡡��: ��                                    Output:      NULL
********************************************************************************************************************/
void Pkur_HwInit(void)
{
    //P5DIR |= BIT6;
    //P3OUT &= (~BIT1)&(~BIT3);//IO����
    //P3DIR |=  BIT0+BIT1+BIT3;//IO����
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
** ��������: void SPI_SendByte()                Name:      void SPI_SendByte()
** ��������: ͨ��SPI�ӿڷ���һ���ֽ�            Function: send a byte by SPI interface
** �䡡��: INT8U byte: ���͵��ֽ�                Input:      INT8U byte: the byte that will be send
** �䡡��: ��                                    Output:      NULL
********************************************************************************************************************/
unsigned char SPI_SendByte(unsigned char byte)
{

    unsigned char i,j;
    j=0;
    //P3IN |=BIT2;
    PKU_CLK(0);
    for(i=0;i<8;i++)           //"������
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
            _NOP();                 //"����оƬ�ٶ��뵥Ƭ��ʱ�ӵ�����ʱ
        }
        #endif
        
        PKU_CLK(1);
        //_NOP();
        //_NOP();
        PKU_CLK(0);//�½��ض�����
        j=j<<1;
        if(PKU_MOSO())
        {
            j=j|0x01;     //"������
        }
    }
    return j;
}
/*******************************************************************************************************************
** ��������: INT8U SPI_RecByte()                Name:      INT8U SPI_RecByte()
** ��������: ��SPI�ӿڽ���һ���ֽ�                Function: receive a byte from SPI interface
** �䡡��: ��                                    Input:      NULL
** �䡡��: �յ����ֽ�                            Output:      the byte that be received
********************************************************************************************************************/
unsigned char SPI_RecByte(void)
{
    return SPI_SendByte(0XFF);
}
//2.2   оƬ����
/************************************************************************************************/
/*ָ�����                                                                                      */
/************************************************************************************************/
//����ͨ������
const unsigned char  tunnel_SW[2] = {0XD8,0X00};
//����ͨ����ֵ
const unsigned char  tunnel_WriteLM[] = {0XC0,0XC8,0XD0};
//������ͨ������������
const unsigned char  tunnel_CLR[] = {0XE0,0XE8,0XF0};
//��ȡ����ͨ������
const unsigned char  tunnel_READ[4][2] = {{0X20,0X00},{0X24,0X00},{0X28,0X00},{0X2C,0X00}};
/************************************************************************************************/
//��ȡ����ͨ������
//data:�����ļ���ֵ�����ڴ����׵�ַ,��СΪ6�ֽڣ�
//��0��1�ֽ�Ϊ��һͨ���ļ���ֵ������ֵΪ16λ�޷�����������
//                                 ��0�ֽ�Ϊ����ֵ�ĵ�8λ����1�ֽ�Ϊ��8λ��
//��2��3�ֽ�Ϊ�ڶ�ͨ���ļ���ֵ����ʽͬ�ϣ�
//��4��5�ֽ�Ϊ����ͨ���ļ���ֵ����ʽͬ�ϣ�
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
//������ͨ������������
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
//��ͨ��
//data:Ҫ�򿪵�ͨ����
//04��һͨ����BIT2
//02�ڶ�ͨ����BIT1
//01����ͨ����BIT0
/************************************************************************************************/
void SMeasreStart(unsigned char dat)
{
    SPI_SendByte(tunnel_SW[0]|(dat&0X07));
    SPI_SendByte(tunnel_SW[1]);
}

/************************************************************************************************/
//�ر�����ͨ��
/************************************************************************************************/
void SMeasreStop(void)
{
    SPI_SendByte(tunnel_SW[0]);
    SPI_SendByte(tunnel_SW[1]);
}

/************************************************************************************************/
//������ֵ
//datain:Ҫ������ֵ�����ڵ��ڴ��׵�ַ������Ϊ3���ֽ�
//��0��1��2�ֽڷֱ�Ϊ��һ��������ͨ����ֵ
/************************************************************************************************/
void SSetLimen(unsigned char *datain)
{
    int i;
    for(i = 0;i < 3;i++)//������ֵ
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

