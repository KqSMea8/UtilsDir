#pragma once

#ifndef __AFXWIN_H__
#   error Включить stdafx.h до включения этого файла в PCH
#endif

#include "../../resource/resource.h"

class CTestOpcClientApp : public CWinApp
{
public:
	CTestOpcClientApp();

public:
	virtual BOOL InitInstance();

protected:
	DECLARE_MESSAGE_MAP()
};

extern CTestOpcClientApp theApp;
