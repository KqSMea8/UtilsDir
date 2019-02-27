// ����� ���������������� ������ � ���������� ��� �������� ���������������� ������

#pragma once
#include <afx.h>
#include <vector>

// ����� ��� ������ �� ������� ���������, ��������� � ���������������� ������

class CDeviceFinder
{
private:
	// ������ ��������� � ��������� ���������� ���������������� ������
	std::vector<CString> m_vDevices;
	// ��������� ���������� ���������������� ������
	CString m_sDevicesPath = L"";

public:
	CDeviceFinder(CString path_ = L"");
	~CDeviceFinder();

public:
	unsigned long getDevicesCount();                       // ������ ���������� ���������
	CString getDeviceName(unsigned long deviceNumber_);    // ������ ����� ����������
	CString getDevicePath(unsigned long deviceNumber_);    // ������ ���������� ����������
	CString getDeviceCfgAdr(unsigned long deviceNumber_);  // ������ ������ ����������������� ����� ����������
};
