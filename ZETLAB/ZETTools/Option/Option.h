//----------------------------------------------------------------------
// ���� Option.h
// ������ �� 06.06.2012
// ��� "����", ����������, ������� �.�.
//----------------------------------------------------------------------
// ����� ��� ������ ����� ���������
//----------------------------------------------------------------------
#pragma once

class COption
{
public:
	COption();
	virtual ~COption();

	CString GetSoftware(long lMask);
	CString GetHardware(long lMask);
};
//----------------------------------------------------------------------

