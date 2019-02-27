// calibr.h
#ifndef __calibr_h__
#define __calibr_h__

typedef struct _CALIBR		//��������� �������� �� ������ ��� ��������� �������
{
	unsigned long sizeSelf;		// ������ ���� ���������
	char Signature[16];			// ���������
	short ICP;					// 0-ICP ��������, 1-ICP �������,
	short TestCode[3];			// �������� ��� ��� �������� ����������� + ������������
	char name[128];				// �������� ������
	char conv[16];				// ������� ���������
	float sense;				// ��� �������� �������
	float DC;					// �������� ����
	float refer;				// ������� �������� ��� ���������� �������� � ��
	float ExtAmp;				// �� �������� ���������
	_CALIBR() 
		: sizeSelf(2500)
		, ICP(0)
		, sense(0.f)
		, DC(0.f)
		, refer(0.f)
		, ExtAmp(0.f)
	{
		ZeroMemory(Signature, sizeof(Signature));
		ZeroMemory(TestCode, sizeof(TestCode));
		ZeroMemory(name, sizeof(name));
		ZeroMemory(conv, sizeof(conv));

	}
} CALIBR, *PCALIBR;

#endif __calibr_h__