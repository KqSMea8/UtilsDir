//-----------------------------------------------------------------------------
#pragma once
//-----------------------------------------------------------------------------
#ifdef ZETVIEWRESULT_EXPORTS
#define VIEWRESULT_API __declspec(dllexport)
#else
#define VIEWRESULT_API __declspec(dllimport)
#endif
//-----------------------------------------------------------------------------
#include <ZetErrorCodes.h>					// ���� ������
#include <chaninfo.h>						// CHANNEL_STRUCT
#include <ViewResult\StructViewResult.h>	// STRUCT_VIEW_RESALT
#include <ViewResult\TypeTransform.h>		// TypeTransform...
#include <ViewResult\StructUnit.h>			// ��������� ������ ���������
//-----------------------------------------------------------------------------
#define DEFAULT_VALUE_ALFA 0.5f

#define FLAG_START_ON	0x1
#define FLAG_POINT_ON	0x2
#define FLAG_INIT_ON	0x4
//-----------------------------------------------------------------------------
// ������������� ��������� ��� ����������� �������� ������-���� ������.
// pChanPar - ��������� �� ��������� ���������� ������;
// numberAver - ���-�� ��������, ������������ � ����������� ������ ��������;
// bindingPoint - ��������� ������������ ���������� �����;
// indxPoint - ������ ���������� ����� (���� 0, �� ������������ � DLL)
// pParam - ��������� �� ��������� ���������� �����������.
//
// ������������ ��������:
// ������������� �������� - ��� ������;
// 0 - ������������� ��������� �������.
_Check_return_ VIEWRESULT_API
long zvInitParam(_In_ LPCHANNEL_STRUCT pChanPar,
_In_ const DWORD numberAver,
_In_ const BYTE bindingPoint,
_In_ const BYTE indxPoint,
_Inout_ STRUCT_VIEW_RESALT * pParam);
//-----------------------------------------------------------------------------
// ������� ������ �������� � ��� �������������� � ������. ��������� �
// pParam->sValueCur.
//
// ������������ ��������:
// ������������� �������� - ��� ������;
// 0 - ��������� �������.
_Check_return_ VIEWRESULT_API
long zvNewValue(
_In_ const float newVal, _Inout_ STRUCT_VIEW_RESALT * pParam);
//-----------------------------------------------------------------------------
// �������������� ��������� ��� ������ ������ �������� ��� ��������� ����������.
//
// ������������ ��������:
// ������������� �������� - ��� ������;
// 0 - ��������� �������.
_Check_return_  VIEWRESULT_API
long zvReStart(_Inout_ STRUCT_VIEW_RESALT * pParam);
//-----------------------------------------------------------------------------
// ������� ������� (���� �� 0) ��������� ���������� �����.
//
// ������������ ��������:
// ������������� �������� - ��� ������;
// 0 - ��������� �������.
_Check_return_  VIEWRESULT_API
long zvSetIndxPoint(_In_ const BYTE indx, _Inout_ STRUCT_VIEW_RESALT * pParam);
//-----------------------------------------------------------------------------
// ������� ���� ������������ (����/���) ��������� ���������� �����
// ��� ���������� ����� � �������������.
// binding - 0 ������������ �� ���������, !0 ������������ ���������;
// pParam - ��������� �� ��������� ���������� �����������.
//
// ������������ ��������:
// ������������� �������� - ��� ������;
// 0 - ������������� ��������� �������.
_Check_return_ VIEWRESULT_API
long zvSetBindingPoint(
_In_ const BYTE binding,
_Inout_ STRUCT_VIEW_RESALT * pParam);
//-----------------------------------------------------------------------------
// �������������� ������
// code - ��� ��������������
// dt - ����� ����������, �������, ��� ����������������� �����. ��������������
// df - ��������� ���������� ��� ���������� �����. ��������������
// numPoints - ���-�� ����� ��������� ������, ��������� ��� ��������� ������ ������� ��������� ������
// lpwNewName - ������ �������� ������ (��� ttc_unit � ttc_preUnit) ��� nullptr
// lpwNewMeasure - ����� ����������� ������
// pChanDst - ��������� ��������� ������
// pChanSrc - ��������� ��������� ������
//
// ������������ ��������:
// ������������� �������� - ��� ������;
// 0 - ��������� �������.
_Check_return_  VIEWRESULT_API
long zvTransformChannel(
	_In_ const TypeTransformChannel code,
	_In_ const double dt,
	_In_ const double df,
	_In_ const uint64_t numPoints,
	_Inout_opt_ LPCWCH lpwNewName,
	_Inout_opt_ LPCWCH lpwNewMeasure,
	_Inout_ LPCHANNEL_STRUCT pChanDst,
	_Inout_ const LPCHANNEL_STRUCT pChanSrc);
//-----------------------------------------------------------------------------
// �������������� ������
// code - ��� ��������������
// dt - ����� ����������, �������, ��� ����������������� �����. ��������������
// df - ��������� ���������� ��� ���������� �����. ��������������
// numPoints - ���-�� ����� ��������� ������, ��������� ��� ��������� ������ ������� ��������� ������
// lpwNewName - ������ �������� ������ (��� ttc_unit � ttc_preUnit) ��� nullptr
// lpwNewMeasure - ����� ����������� ������
// pChanDst - ��������� ��������� ������
// pChanSrc - ��������� ��������� ������
//
// ������������ ��������:
// ������������� �������� - ��� ������;
// 0 - ��������� �������.
_Check_return_  VIEWRESULT_API
long zvTransformChannel_I(
	_In_ const TypeTransformChannel code,
	_In_ const double dt,
	_In_ const double df,
	_In_ const uint64_t numPoints,
	_Inout_opt_ LPCWCH lpwNewName,
	_Inout_opt_ LPCWCH lpwNewMeasure,
	_Inout_ LPCHANNEL_STRUCT pChanSrcDst);
//-----------------------------------------------------------------------------
// ��������� ���������� ����� ������ ���������, �������������� DLL
//
// ������������ ��������:
// ������������� �������� - ��� ������;
// 0 - ��������� �������.
_Check_return_  VIEWRESULT_API
long zvGetNumberGroupUnit(_Inout_ DWORD * pNumber);
//-----------------------------------------------------------------------------
// ��������� ���������� ����� ������ ���������, �������������� DLL
// ������������ ��������:
// ������������� �������� - ��� ������;
// 0 - ��������� �������.
_Check_return_  VIEWRESULT_API
long zvGetNumberTypeUnit(_Inout_ DWORD * pNumber);
//-----------------------------------------------------------------------------
// ��������� ����� ����� ������ ������ ���������.
// ndxGroup - ������ ������ (�� 0)
// lpwBuf - �����, � ������� ����� ������������ ���
// pSizeBuf - ������ ����� ������
//
// ���� lpwBuf = nullptr, �� �
// pSizeBuf ����� ������� ������, ����������� ��� ����������� ����� ���� ������.
//
// ������������ ��������:
// ������������� �������� - ��� ������;
// 0 - ��������� �������.
_Check_return_  VIEWRESULT_API
long zvGetNameGroupUnit(
	_In_ DWORD indxGroup,
	_Inout_ LPWCH lpwBuf,
	_Inout_ DWORD * pSizeBuf);
//-----------------------------------------------------------------------------
// ������������� �������������� ������:
// - ����������� �������� ������� (�� ��������� �����), � ����� �����
//   �������������� ������ DLL ������;
// - ����������� �������� ������� (�� ���� �������������� � ������� �������
//   ��� �� ��������� �����), � ����� �����
//   �������������� ������ DLL ������;
// - ����������� ���������� �������������� ������.
//
// ��������� �������:
// ttu - ��� ��������������;
// lpwBufSrc - ������������ ������� ������� (�����������);
// lpwBufDst - ������������ �������� ������� (��� nullptr ��� L"");
// psuc - ��������� ��������������.
//
// ������������ ��������:
// ������������� �������� - ��� ������;
// 0 - ��������� �������.
_Check_return_  VIEWRESULT_API
long zvInitTransformUnits(
	_In_ const TypeTransformUnit ttu,
	_Inout_ LPCWCH lpwBufSrc,
	_Inout_ LPCWCH lpwBufDst,
	_Inout_ STRUCT_UNIT_CONVERT * psuc);
//-----------------------------------------------------------------------------
// ��������� �������� ������� ��� �������������� ������. ����� ��������
// ������� ������������ �� ���� �������������� ��� �� ��������� �����.
//
// ��������� �������:
// ttu - ��� ��������������;
// lpwBufDstUnit - ������������ �������� ������� (��� 0);
// psuc - ��������� ��������������.
//
// ������������ ��������:
// ������������� �������� - ��� ������
// 0 - ��������� �������.
_Check_return_  VIEWRESULT_API
long zvChangeUnit(
	_In_ const TypeTransformUnit ttu,
	_Inout_ LPCWCH lpwBufDstUnit,
	_Inout_ STRUCT_UNIT_CONVERT * psuc);
//-----------------------------------------------------------------------------
// ���������� ��������� �������� ������� ����� ���������� �������
// zvInitTransformUnits ��� zvChangeUnit.
//
// ��������� �������:
// psuc - ��������� ��������������;
// pChanStruct - ��������� ��������� ������;
// pViewParam - ��������� �����������.
//
// ������������ ��������:
// ������������� �������� - ��� ������
// 0 - ��������� �������.
_Check_return_  VIEWRESULT_API
long zvRealizationUnit(
	_In_ const STRUCT_UNIT_CONVERT * psuc,
	_Inout_ CHANNEL_STRUCT * pChanStruct,
	_Inout_ STRUCT_VIEW_RESALT * pViewParam);
//-----------------------------------------------------------------------------
// ���������� �������������� �������� �� ����� ������� � ������. �
// ����������� ������� � �������������� �������/������� �������.
//
// ��������� �������:
// valSrc - �������� � �������� ��������;
// pValDst - �������� � �������� ��������;
// pbEventPreUnit - ����, ��� ���� �������������� �������/������� �������;
// pFactorEventPreUnit - ����������� �������������� ( = ValDst /valSrc );
// psuc - ��������� ��������������.
//
// ���� ���� �����������, ��� ��������� psuc ������� ���������, � ��������������
// �������/������ ������� �� ���������, �� ����� �������� ��� ���� �������, �������:
// *pValDst = (float)(psuc->convert.k * valSrc + psuc->convert.a);
//
// ������������ ��������:
// ������������� �������� - ��� ������
// 0 - ��������� �������.
_Check_return_  VIEWRESULT_API
long zvTransformValue(
	_In_ const float valSrc,
	_Inout_ float * pValDst,
	_Inout_ BYTE * pbEventPreUnit,
	_Inout_ double * pFactorEventPreUnit,
	_Inout_ STRUCT_UNIT_CONVERT * psuc);
//-----------------------------------------------------------------------------
// ������� ���������� �������������� �������/������� ������� �����
// ���������� ������� zvTransformValue ��� pbEvent = 1.
//
// ��������� �������:
// psuc - ��������� ��������������;
// pChanStruct - ��������� ��������� ������;
// pViewParam - ��������� �����������.
//
// ������������ ��������:
// ������������� �������� - ��� ������
// 0 - ��������� �������.
_Check_return_  VIEWRESULT_API
long zvRealizationPreUnit(
	_In_ const STRUCT_UNIT_CONVERT * psuc,
	_Inout_ CHANNEL_STRUCT * pChanStruct,
	_Inout_ STRUCT_VIEW_RESALT * pViewParam);
//-----------------------------------------------------------------------------
// ����������� ������������ �������� ���������� � ������������ (� �������� ������
// ������������� �������� �������) �� ������� ����� � ������������ � ���������
// ���� 8.417-2002 � ����� ������� ������� ����� ������ � ������������.
//
// ��������� �������:
// lpwBuf - ����� ��� �����������;
// sizeBuf - ������ ����� ������;
// psuc - ��������� ��������������;
// pViewParam - ��������� �����������.
// 
// ������������ ��������:
// ������������� �������� - ��� ������
// 0 - ��������� �������.
_Check_return_  VIEWRESULT_API
long zvCopyStringResult(
	_Inout_ LPWCH lpwBuf,
	_In_ const DWORD sizeBuf,
	_In_ const STRUCT_UNIT_CONVERT * psuc,
	_In_ const STRUCT_VIEW_RESALT * pViewParam);
//-----------------------------------------------------------------------------
// ��������� ������������������ ������������ �������������� ������, ���
// ���������� �������������� � ������������ � ���������, ����������� � �����
// "$ZetConfigDir\ConvertUnits.xml"
// ��������� �������:
// unitSrc - ����� � ������� ������������ �������;
// unitDst - ����� ��� �������� ����������� �������;
// sizeUnitDst - ������ ������ unitDst;
// k - ����������������� ����������� �������������� ������.
//
// ������ ���� ��� ������, ����� � ConvertUnits.xml ���� ������
// <Units	UnitSrc="���"	UnitDst="���"/>
// -------------------------------------------------
// WCHAR unitSrc[SIZE_NAME_UNIT] = { L"���" };
// WCHAR unitDst[SIZE_NAME_UNIT] = { 0 };
// float k(0.f);
// if (0 == zvConvertUnitByXML(unitSrc, unitDst, SIZE_NAME_UNIT, k))
//{		���������:
//		unitDst �������� ������ L"���"
//		k ����� 10.1971617
//}
_Check_return_  VIEWRESULT_API
long zvConvertUnitByXML(
	_Inout_ LPCWSTR unitSrc,
	_Inout_ LPWSTR unitDst,
	_In_ const DWORD sizeUnitDst,
	_Out_ float & k);
//-----------------------------------------------------------------------------