//-----------------------------------------------------------------------------
// ���� �������� ����� ���������� � ���������,
// ������������ � �� ZetLab � ZetWiew
//-----------------------------------------------------------------------------
#pragma once
#include "afxstr.h"
#include "WTypes.h"
#include <WinDef.h>
#include <WinError.h>	// ���������, ������������ � ���������� WinLog
#include <DefineWM.h>	// ������� ���������
//------------------------------------------------------------------------------
// ����������� ������ ��� ���� � ����������� ���������
#ifndef Width_Min
#define Width_Min		320
#endif

// ����������� ������ ��� ���� � ����������� ���������
#ifndef Height_Min
#define Height_Min		256
#endif

// ������ ���� � �� ����������� ���������
#ifndef WidthWin
#define WidthWin 492	// 492 = 512-20
#endif

// ������������ ���-�� �������� � ������� �� GridGl
#ifndef MaxSizeForGridGl
#define MaxSizeForGridGl 500000
#endif

// ������������ ���-�� �������� � ����� ����������� ������� �� Granna
#ifndef MaxSizePointInLineForGramma
#define MaxSizePointInLineForGramma 50000
#endif

//------------------------------------------------------------------------------
// ���� ����� ����������. ������������ � CDialog_ZET::OnStructureNew.
// ����� ���� ����� ��������� ������ � �����.
enum TypeNewParam : BYTE
{
	param_no,			// ��������� �� ������
	param_default,		// �� ���������
	param_cfg_file,		// �� cfg-�����
	param_win_param,	// �� ���� ������� ����������
	param_zet_project,	// �� ZET-������ ��� ������� � ������� Z-�������
	param_modify_srv,	// ����� ��������� ������� �������
	param_unit_1,		// �� UNIT-1
	param_unit_2,		// �� UNIT-2
	param_unit_3,		// �� UNIT-3
	param_cfg_change,	// �� ���������� cfg-�����
	param_cfg_struct,	// �� cfg-���������
};
//------------------------------------------------------------------------------