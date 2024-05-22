#ifndef __CMD_H__
#define __CMD_H__

#if 0
#define CMD_SET_CHANNEL   0x11
#define CMD_REPORT_MODE   0x12
#define CMD_GET_COUNTER   0x13
#define CMD_GET_PARAM     0x14
#define CMD_GET_TOTAL     0x15
#define CMD_SET_THRES     0x16
#define CMD_GET_THRES     0x17
#define CMD_SET_SW        0x18
#else
#define CMD_SHARK_HAND    0x1     // ��������
#define CMD_GET_DR        0x2     // ����ǰ����������
#define CMD_GET_DOSE      0x3     // ����ǰ��������
#define CMD_GET_MAX       0x4     // ��������������
#define CMD_GET_STATUS    0x5    // ����ǰ״̬
#define CMD_CLR_DOSE      0x6    // �����������
#define CMD_GET_FACTOR    0x7   // ��У׼��������
#define CMD_SET_FACTOR    0x8   // дУ׼��������
#define CMD_GET_MODEID    0x9   // ��ģ����
#define CMD_GET_VER       0xA   // ��ͨ��Э��汾
#define CMD_GET_CPS       0xB   // ��CPS
#define CMD_GET_CPS_30S   0xC    // ��30�����
#define CMD_SET_THRES     0xD    // ������ֵ
#endif


void Handle_Uart_Rec();
void HandCmd();
void ReportCounter();


#endif

