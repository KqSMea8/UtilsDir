//------------------------------------------------------------------------------
//	���� Seismo.h
//------------------------------------------------------------------------------
#pragma once
//------------------------------------------------------------------------------
#include <Intel\ipp\ipps.h>
#include <mathZet.h>
#include <algorithm>
#include <ZetErrorCodes.h>
//------------------------------------------------------------------------------
enum Ssm_type_Sensor
{
	accsel,
	velosity,
	geofon,
};
//------------------------------------------------------------------------------
enum Ssm_type_Event
{// ����� ���� ������ � �����!!!
	te_Error = -1,			// ������ ��� ����������� ����
	te_No,					// ��� �������
	te_MinorEvent,			// ������ ������� (�������� ������� ���������)
	te_StrongEvent,			// ������� ������� (�������� ������� ���������)
	te_SmallTime,			// ������� � ����� �������������
	te_LargeTime,			// ������� �� ������� ������� �������������
	te_FalseEventPolar,		// ������ ������� �� ��������� �����������
	te_FalseEvent,			// ������ �������
    te_StartEvent,			// ������ ������������ �������
	te_EndEvent,			// ������� �����������
};
//------------------------------------------------------------------------------
enum Ssm_type_Error
{
	ssm_err_Ok,				// �� ������
	ssm_err_NotWork,		// ������� ������ �� ������
	ssm_err_FreqADC,		// �� ���������� ������� �������������
	ssm_err_NoChan,			// ��� ������
	ssm_err_OffChan,		// ������ �����
	ssm_err_ReadFile,		// ������ ������ �����
	ssm_err_ReadData,		// ������ ������ ������
	ssm_err_NaN,			// NaN � ������
	ssm_err_NoRange,		// �������� �� ������ � ���
	ssm_err_NoRangeMin,		// �������� ������ ��������
	ssm_err_NoRangeMax,		// �������� ������ ���������
	ssm_err_LessThreshold,	// �������� ������ ������ 
	ssm_err_MoreThreshold,	// �������� ������ ������ 
	ssm_err_NoChanX,		// ��� ������ X
	ssm_err_OffChanX,		// ������ ����� X
	ssm_err_ChanX,			// ����� X �� � �����
	ssm_err_NoChanY,		// ��� ������ Y
	ssm_err_OffChanY,		// ������ ����� Y
	ssm_err_ChanY,			// ����� Y �� � �����
	ssm_err_NoChanZ,		// ��� ������ Z
	ssm_err_OffChanZ,		// ������ ����� Z
	ssm_err_ChanZ,			// ����� Z �� � �����
	ssm_err_NoChanP,		// ��� ������ P
	ssm_err_OffChanP,		// ������ ����� P
	ssm_err_ChanP,			// ����� P �� � �����
	ssm_err_CreateDetector,	// ������ ��� �������� ���������
	ssm_err_SRV,			// ������ ��� ����������� � �������
};
//------------------------------------------------------------------------------
// ���� �������, ����������� �������������
enum Ssm_type_Event_Algr : DWORD
{
	tea_Event,			// ������� �����������  �������
	tea_ActualEvent,	// �������� �������
	tea_FalseEvent,	    // ������ �������
	tea_sizeType	    // ���-�� �������� �����
};
//------------------------------------------------------------------------------