#pragma once

//����������� �������������� ����� ������ ��� ������ ������ � �������� _COMMAND_INFO_USB.Command = 0x0113
#define NUM_SIGNALS				0x00		//��������� ���������� ����������� �������� � ������� (�������� ��������� ����������� �� ������� ������ ������ ������ (����� ��������� � ������ ������))


typedef struct _DEV_INFO					//���������, ������������ ��������� ������ �������
{
	short	num_ch_adc;						//���������� ������� �������
	time_t	start_time;						//����� ������ ������
	time_t	duration;						//����������������� ������, ���
	short	data_type;						//��� ������ (0 - 16 ���, 1 - 32 ����)
	float	frecuency;						//������� ������������� ������ ������
	
	float	abs_volt;
} DEV_INFO;

#define CANAL_NAME_LEN			128			//������������ ����� ������ ����� ������
#define CONV_NAME_LEN			16			//������������ ����� ������ ��� ������� ���������

typedef struct _CHAN_INFO					//���������, ������������ ��������� ����������
{
	char name[CANAL_NAME_LEN];				//�������� �������
	char conv[CONV_NAME_LEN];				//������� ���������
	float sense;							//���������������� ������� �/��.���.
	float DC;								//�������� ����
	float refer;							//������� �������� ��� ���������� �������� � ��
} CHAN_INFO;

#define COMMENT_LEN				128			//������������ ����� ������ �����������
#define FORMAT_LEN				4			//������������ ����� ������ ������� �����
#define MAX_STR_LEN_ANP_INF		256			//������������ ����� ������ � ��������� _CHAN_INFO_ANP


typedef struct _CHAN_INFO_ANP				//���������, ������������ ��������� ���������� ������ ��� ����������� ������� (������ ��������� ������������ � ���� ANP)
{
	char name[CANAL_NAME_LEN];				//�������� ������ ����� �������
	char comment[COMMENT_LEN];				//�����������
	float abs_volt;							//��� �������� ������� ��� � �������
	unsigned int frq;						//������� ������������� ������� � ��
	int tmi;								//(???) ���? int? ��������� ��������� ����� ������� ��� �����
	float frl;								//������� ����� ������� ������ ������
	float frh;								//������� ����� ������� ������� ������
	char format[FORMAT_LEN];				//������ �������������� ������ (�����/�������, 1�����/2�����)
	struct tm start;						//����� � ���� ������ �������
	short ch_idx;							//������ ������ �����
	float sense;							//���������������� ���������� ��������������� ���������� �������� � ����������, ����������� � �����/CONVERT
	char conv[CONV_NAME_LEN];				//�������� ������� ��������� ���������� ��������
	float ampl;								//����������� �������� ���������������� ��� �������������� ���������	
	float refer;							//��������� �������� ��� ������� ��. ��� ������������� ��� �������� ���������� 1 ���, ��� ������������� � 20 ����, ��� �������� � 300 ���/�^2
	int afch;								//���� �������� ��� �������������� ������
	float dc;								//�������� ���������� ������������ � �������
	float gain;								//��������

#ifdef WIN32
	_CHAN_INFO_ANP();	 						//����������� �� ���������		
	_CHAN_INFO_ANP(const _CHAN_INFO_ANP& inf);	//����������� �����������
#endif

} CHAN_INFO_ANP;

#ifdef WIN32
	bool ReadAnpFile(const CString& fname, CHAN_INFO_ANP* pChInfAnp);		//������ ��������� CHAN_INFO_ANP �� �����
	bool WriteAnpFile(CHAN_INFO_ANP *pChInfAnp, const char *fname);			//����� ��������� CHAN_INFO_ANP � ����
#endif
