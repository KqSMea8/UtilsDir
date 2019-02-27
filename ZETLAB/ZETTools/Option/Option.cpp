//----------------------------------------------------------------------
// ���� Option.cpp
// ������ �� 6.06.2012
// ��� "����", ����������, ������� �.�.
//----------------------------------------------------------------------
#include "stdafx.h"
#include "Option\Option.h"
//----------------------------------------------------------------------
#define STD_QUANTITY										9
#define SOFTWARE_OPTION_QUANTITY							30
#define HARDWARE_OPTION_QUANTITY							11

#define ZETLAB_DEMO											0x3			
#define ZETLAB_ANALIZ										0x2f1f97
#define ZETLAB_VIBRO										0x2f9f9f	
#define ZETLAB_NOISE										0x1e93		
#define ZETLAB_TENZO										0x273e17	
#define ZETLAB_SEISMO										0x2f9f93	
#define ZETLAB_SEISMO_DAC									ZETLAB_SEISMO | 0x4
#define ZETLAB_BASE											0x7			
#define ZETLAB_SENSOR										0x1a60c03	
#define RESERVE_MASK										0x7f024060

//����������� ����������� ��������
const long lStdSoftwareMasks[STD_QUANTITY] =			   {ZETLAB_DEMO,
															ZETLAB_BASE,
															ZETLAB_NOISE,
															ZETLAB_SENSOR,
															ZETLAB_SEISMO,
															ZETLAB_SEISMO_DAC,
															ZETLAB_TENZO,
															ZETLAB_ANALIZ,
															ZETLAB_VIBRO};
//�������� ����������� ����������� ��������
const CString sStdSoftwareOptions[STD_QUANTITY] =		   {L"ZETLabDemo",
															L"ZETLabBase",
															L"ZETLabNoise",
															L"ZETLabSensor",
															L"ZETLabSeismo",
															L"ZETLabSeismo",
															L"ZETLabTenzo",
															L"ZETLabAnaliz",
															L"ZETLabVibro"};
//�������� ��������� ����������� �����
const CString sSoftwareOptions[SOFTWARE_OPTION_QUANTITY] = {L"������� ���������",
															L"����������",
															L"���",
															L"��������, ����������, �������",
															L"",
															L"",
															L"������������ �������, ���������, ���",
															L"�������� ���. � ���. �������",
															L"�����. ���., ���. ������, ������ ���. ���.",
															L"�����������",
															L"����������",
															L"ZETLabStudio",
															L"����������, ���������",
															L"",
															L"ZETView",
															L"���������, �������",
															L"���������� ��������",
															L"����������� � �����������",
															L"ZETView (exe)",
															L"������� �������",
															L"�����������, STA-LTA, �������",
															L"��������������� ���������",
															L"������������ ���������",
															L"����������� � ZET7176",
															L"",
															L"",
															L"",
															L"",
															L"",
															L""};
//�������� ��������� ���������� �����
const CString sHardwareOptions[HARDWARE_OPTION_QUANTITY] = {L"PrUs",
															L"ICP",
															L"Atten",
															L"SD-card",
															L"Ethernet",
															L"Bluetooth",
															L"���",
															L"���������� ����������",
															L"TEDs",
															L"GPS",
															L"PTP"};
//----------------------------------------------------------------------
COption::COption()
{
	
}
//----------------------------------------------------------------------
COption::~COption()
{
	
}
//----------------------------------------------------------------------
CString COption::GetSoftware(long lMask)
{
	CString sOption(L"");

	//������� ��� ��������� ����
	lMask &= (~RESERVE_MASK);
	
	for (int i = 0; i < STD_QUANTITY; ++i)
	{
		//����������� �����
		if (((lMask & lStdSoftwareMasks[i]) == lStdSoftwareMasks[i]) && ((lMask & (~lStdSoftwareMasks[i])) == 0))
		{
			return sStdSoftwareOptions[i];
		}
	}
	
	for (int i = STD_QUANTITY - 1; i >= 0; --i)
	{
		//����������� ����� � ���-��
		if ((lMask & lStdSoftwareMasks[i]) == lStdSoftwareMasks[i])
		{
			sOption = sStdSoftwareOptions[i];
			long lOptions = lMask & (~lStdSoftwareMasks[i]);
			for (int j = 1; j < sizeof(long) * 8 - 1; ++j)
			{
				if ((lOptions >> j) & 0x1)
				{
					sOption.Append(L" + ");
					sOption.Append(sSoftwareOptions[j - 1]);
				}
			}
			return sOption;
		}
	}
	//����� ��������� �����
	for (int j = 1; j < SOFTWARE_OPTION_QUANTITY + 1; ++j)
	{
		if ((lMask >> j) & 0x1)
		{
			if(sOption.GetLength() > 0)
				sOption.Append(L" + ");
						
			sOption.Append(sSoftwareOptions[j - 1]);
		}
	}
	return sOption;
}
//----------------------------------------------------------------------
CString COption::GetHardware(long lMask)
{
	CString sOption(L"");
	//����� ��������� �����
	for (int i = 0; i < HARDWARE_OPTION_QUANTITY; ++i)
	{
		if ((lMask >> i) & 0x1)
		{
			if(sOption.GetLength() > 0)
				sOption.Append(L" + ");

			sOption.Append(sHardwareOptions[i]);
		}
	}
	return sOption;
}
//----------------------------------------------------------------------