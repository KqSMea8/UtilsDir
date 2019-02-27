//------------------------------------------------------------------------------
// ����� � ����������� � �������� ������ ���� � ������� "7777". ������ ��. �
// �������� ����������� ����.
//------------------------------------------------------------------------------
#pragma once
#include "ZetBasic.h"

#include <Dialog_ZET\ConnectToZetTools.h>
#include <Dialog_ZET\ConnectToZetLoggerClient.h>
//------------------------------------------------------------------------------
class CZetBasicLogger : public CZetBasicOle
{
private:
	bool CreateLogger();
	void RegisterBinary();

protected:
	SZetLoggerStructureId m_structWithAllID;
	CZETLoggerClient *m_pLogger = nullptr;
	std::vector<ZetHeader*> m_descriptors;

	// ����� ���������
	DECLARE_MESSAGE_MAP()

public:
	CZetBasicLogger(); 
	virtual ~CZetBasicLogger();

	virtual int OnCreate(LPCREATESTRUCT lpCreateStruct, CString progid);
	afx_msg LRESULT OnStartAll(WPARAM wParam, LPARAM lParam);
};
//------------------------------------------------------------------------------