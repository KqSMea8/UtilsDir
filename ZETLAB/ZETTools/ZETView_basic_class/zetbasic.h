// Copyright (C) 2007 by ZET  http://www.zetms.ru
// Written by Krasovskiy Andrey, e-mail: krasovskiyAndrey@mail.ru
//										 info@zetms.ru
// All rights reserved
// ������ ���� ���������� � �������� �������� ��� �����������, 
// � ����������, ���������� � SCADA ������� �������� ZET.
// ������ ���� ����������� �� �������� ������ COleControl, � 
// ������������ ��� ���������� ����� ������.
// CZetBasicOle ���������� ��� ��������� �������, �������, �������
// � �������, ����������� ��� ��������� � SCADA ������� ZET, ��� 
// � ��������� ActiveX ���������.

#pragma once
#include "afxctl.h"
#include "stdint.h"
#include "user_mess.h"
#include "cdsrv.h"
#include "fModBusSlave.h"

#include "myGdiPlus.h"
using namespace Gdiplus;

#include <vector>
// #include <WideResult\WideResult.h>
#include <WideResult\WideResult.h>

// ��������� �������
#ifdef _TRANSLATE
#include <Dialog_ZET\Translate.h>
#endif

using namespace std;

// ������ ������ ���������� ����������� ������������� ������ ����������� ������ Active-X �����������
// TODO: ���� ������� ����� ������ ����� ��������, ��� ������� InstallActiveX ����� �� ����������
#define INSIDE_ACTIVEX

#define minWidth_bisizes		75
#define minWidth_Singlesizes	55

#define maxWidth_bisizes		180
#define maxWidth_SingleSizes	115		

#define INSIDEACTIVEX_ID		1001
#define MAX_INPUTCHAN			3

#define IDT_UPDATE_TIMER		100

//���������� � ��������� ����������� ������
struct classInformationS
{
	HWND handle;
	long number;
};

//���������� � ���������� �����
struct contactPinInfoSData
{
	contactPinInfoSData::contactPinInfoSData()
		: helpString(L"")
		, sData(L"")
	{
		fData = 0.f;
		dData = 0.;
		lData = 0;
		

		m_bDefined = false;
	}
	float fData;			//������ float ��� ��������
	double dData;			//������ double ��� ��������
	long lData;				//������ Long ��� ��������
	CString sData;			//������ CString ��� ��������
	CString helpString;		//������ �String ��� ��������, � ��� �����, ������ HelpString
	bool m_bDefined;		//����� ����, ���������� ��������� ���� ��� ���
};
struct contactPinInfoS
{
	float pointDivX;	//X ���������� ��������
	float pointDivY;	//Y ���������� ��������
	long side;			//������� ��������� ��������
	bool connected;		//������������ ��� ���
	long number;		//����� ��������
	CString name;		//�������� ����
	CString helpInfo;   //����������� � ������
	long pinType;		//��� ���������, �������� - OUT_POLYSIGNAL
	bool multiConnect;  //��������� ����������������
	//CRect contactRect;//�������� ������� ���������� ��������
	bool isOut;			//�������� �� ������� - ��������
	long outValue;		//�������� �������� ��� �������� �� ����� ��������������� ����������
	long outValueType;  //��������� �� ��� ������������� �������� TYPE_FLOAT ��� TYPE_LONG
	//COLORREF color;		//���� ��������� ��������, ���� �� ����
	//COLORREF pinColor;
	//COLORREF backColor;
	//COLORREF originalBackColor;
	//CString sLitera;
	bool connected_anotherV;

	contactPinInfoSData data;
};

//������� �����
struct SecondInfoS			//�������� �������� �����
{
	HWND hwnd;				//����� �������� �����
	IDispatch* pDisp;
	long myContact;			//���� ����� ��������
	long secContact;		//����� �������� �������� �����
	SecondInfoS()
		: pDisp(nullptr)
		, myContact(0)
		, secContact(0)
	{
		hwnd = nullptr;
	}
};

//���������� � �������������� �������
struct chanInfoS
{
	//����������� �� ���������
	chanInfoS()
		: Name(L"")
		, Reference(0.1f)
		, Freq(1.f)
		, ID(0)
		, typeADC(0)
	{
	}
	
	CString Name;		//��� ������
	long ID;			//ID ������
	//long Num;
	float Freq;				//������� ������������� ������
	CString Convertion;		//������� ���������
	float Reference;		//������� �������� ��� ������� ��
	long typeADC;			/* ������������ �������� (������ ������):
								0: ���
								1: ���
								2: �����������
								3: ��������
								4: ����������� ����� ���
								5: ����� ������������ ���	*/
};

//���������� � �������������� �����������
struct deviceInfoS
{
	//����������� �� ���������
	deviceInfoS()
	: typeDevice(0)
	, numberDSP(0)
	, serial(0)
	, Name(_T(""))
	{
	}
	bool operator < (const deviceInfoS& Var) const 
	{ 
		if (typeDevice == Var.typeDevice)
		{
			if (numberDSP == Var.numberDSP)
			{
				if (serial == Var.serial)
					return Name < Var.Name;
				else
					return serial < Var.serial;
			}
			else
				return numberDSP < Var.numberDSP;
		}
		else
			return typeDevice < Var.typeDevice;
	}
	
	long typeDevice;
	long numberDSP;
	long serial;
	CString Name;
};

struct ServerInfoS
{
	// ����� ������ ������ �� ��� �����
	long locate(const CString &name)
	{
		long ret(0);
		CString nameCur(name);
		int pos = nameCur.GetLength() - 1;
		if (pos >= 0)
		{
			if (L';' == nameCur.GetAt(pos))
				nameCur.Delete(pos, 1);
		}

		for (auto & it : channel)
		{
			if (it.Name == nameCur)
				return ret;
			++ret;
		}
		return -1;
	}

	// ����� ������ ������ �� ��� ID (long)
	long locateByID(const long id)
	{
		long ret(0);
		for (auto & it : channel)
		{
			if (it.ID == id)
				return ret;
			++ret;
		}
		return -1;
	}

	//long maxQuanDacs;		//���������� ��� �������
	//CArray <chanInfoS,chanInfoS> channel;		//���� ������ ������� �������
	//CArray <chanInfoS,chanInfoS> old_channel;	//������ ������ �������
	//CArray <chanInfoS,chanInfoS> lost_channel;	//��������� ������
	//CArray <chanInfoS,chanInfoS> new_channel;	//����������� ������ �������
	std::vector<chanInfoS> channel;
	std::vector<chanInfoS> old_channel;
	std::vector<chanInfoS> lost_channel;
	std::vector<chanInfoS> new_channel;

	//CArray <deviceInfoS,deviceInfoS> device;		//���� ������ ���������
	//CArray <deviceInfoS,deviceInfoS> old_device;	//������ ���������
	//CArray <deviceInfoS,deviceInfoS> lost_device;	//��������� ���������
	//CArray <deviceInfoS,deviceInfoS> new_device;	//����������� ����������
	std::vector<deviceInfoS> device;		//���� ������ ���������
	std::vector<deviceInfoS> old_device;	//������ ���������
	std::vector<deviceInfoS> lost_device;	//��������� ���������
	std::vector<deviceInfoS> new_device;	//����������� ����������
};

struct libInfoS
{
	libInfoS::libInfoS()
		: name(L"")
		, pDisp(NULL)
		, hwnd(NULL)
	{
		

	}
	CString name;
	IDispatch* pDisp;
	HWND hwnd;
};

struct InsideActiveXS
{
	InsideActiveXS()
		: ActiveXcwnd(nullptr)
		, InsideID(0)
		, clsidStr(0)
	{

		clsidActiveX = {0};
	}
	LPOLESTR clsidStr;
	CLSID clsidActiveX;
	CWnd* ActiveXcwnd;
	CString properties;
	long InsideID;
};

struct tempChanInfoS
{
	long ID;
	CString name;
	tempChanInfoS()
		: ID(0)
		, name(L"")
	{}
};

// ������������� ��������� ������ ������� ����� ������������ ZETVIEW
// sizeof(ZetViewTag) = 2*4 + 2*8 = 24 �����
typedef struct ZetViewTag
{
	long type;		// ��� ������ ��. user_mess.h
	long quality;	// ��������
	double time;	// ����� �������
	union			// �������� ������
	{
		bool	bVal;		// TYPE_BOOL
		long	lVal;		// TYPE_LONG
		float	fVal;		// TYPE_FLOAT
		double	dVal;		// TYPE_DOUBLE
		long long llVal;	// TYPE_LONG_LONG

		wchar_t	*pwText;			// TYPE_POINTER_STRING
		CString *pCText;			// TYPE_STRING
		std::wstring *pStdText;		// TYPE_STRING_STD

		std::vector<long> *pvlVal;		// TYPE_ARRAY_VECTOR_LONG
		std::vector<float> *pvfVal;		// TYPE_ARRAY_VECTOR_FLOAT
		std::vector<double>	*pvdVal;	// TYPE_ARRAY_VECTOR_DOUBLE
		std::vector<CString> *pvCText;	// TYPE_ARRAY_VECTOR_STRING
		std::vector<std::wstring> *pvStdText;	// TYPE_ARRAY_VECTOR_STRING_STD

		PVOID pVoid;					// TYPE_ARBITRARY_STRUCTURE
	} value;

	ZetViewTag()
		: time(-1.0)
		, quality(0)
		, type(-1)
	{
		value.dVal = 0.;
	}

	~ZetViewTag()
	{}

	template <class T>
	T* GetPointerOfArray(std::vector<T> *pVect)
	{
		T *ret = nullptr;
		if ((pVect != nullptr) && (!pVect->empty()))
			ret = vect.data();
		return ret;
	}

	void operator = (const ZetViewTag & oldTag)
	{
		time = oldTag.time;
		type = oldTag.type;
		quality = oldTag.quality;
		value.dVal = oldTag.value.dVal;
	}
} *LPZetViewTag;

class CZetBasicOle : public COleControl
{
public:
	CZetBasicOle(void);
	~CZetBasicOle(void);

	DECLARE_EVENT_MAP()
	DECLARE_DISPATCH_MAP()
	DECLARE_MESSAGE_MAP()

	virtual int  OnCreate(LPCREATESTRUCT lpCreateStruct, CString progid);
	virtual void DoPropExchange(CPropExchange* pPX, DWORD versionInfo);
	virtual void testMessage(float par);
	virtual void OnClose(DWORD dwSaveOption);
	//virtual void OnMouseMove(UINT nFlags, CPoint point);
	//	virtual void OnLButtonDown(UINT nFlags, CPoint point);
	virtual void OnSize(UINT nType, int cx, int cy);

protected:
	virtual LONG GetAllocHeapBl(void);
	virtual LONG GetAllocHeapSz(void);
	virtual LONG ResetMemoryLeak(void);
	virtual LONG GetMemoryLeak(void);
	//s1 - ������ ������
	//s2 - ������ ������
	//s3 - ��������� ��������� �������
	_CrtMemState s10, s20, s30;
	bool m_bFirstLEak;

	//�������� ��������� 
	//virtual LRESULT OnMouseNearComponent(WPARAM wParam, LPARAM lParam);
	//virtual LRESULT OnPlaceConnector(WPARAM wParam, LPARAM lParam);
	//virtual LRESULT OnPlaceConnectorDirect(WPARAM wParam, LPARAM lParam);
	//virtual LRESULT OnSecondConnect(WPARAM wParam, LPARAM lParam);
	//��������� �� ���������� �������
	//virtual LRESULT OnServerModify(WPARAM wParam, LPARAM lParam);	
	//�������� ����������� ���������� - ������ ����� ��� ������������
	//virtual LRESULT OnChangeForm(WPARAM wParam, LPARAM lParam);
	//��������� �� ���������� ���������� �� ����������
	virtual LRESULT OnUpdateByConteiner(WPARAM wParam, LPARAM lParam);
	virtual LRESULT OnStartAll(WPARAM wParam, LPARAM lParam);
	virtual LRESULT OnDebugMode(WPARAM wParam, LPARAM lParam);
	virtual LRESULT OnConnLineDisconnect(WPARAM wParam, LPARAM lParam);
	//��������� �� ������ ���������� �����, ����������� ���������� ������������ �������� �������� ����������
	virtual void fillContactList(long contNum, CString name, CString helpInfo, float contDivX, float contDivY, long side, long pinType, long outValueType, bool multiconnect, bool isOut = false, bool bNeedTranslateName = true);
	//������ �������� ������� ActiveX ����������
	virtual void setLinearSizes(long cx, long cy);
	//������ �������� ������� ActiveX ����������
public:
	virtual void setSizes(CSize size_design, CSize size_operate, bool update_standard);
	//	virtual void setOperateSizes(long cx, long cy);
	//	virtual void setDesignSizes(long cx, long cy);
	//������ ������
	//virtual void drawRegion(CDC* pdc, CRect rcBounds, bool RightTextHorizontal = false, bool haveContactPad = false);
	//void OnDrawMy1(Graphics &graphics, CRect rcBounds);
	//bool isAllowUpdateIcon;
	//void AllowUpdateIco();
	//void BanUpdateIcon();
	//����������� ������������ CWnd*
	virtual CString NameFromID(long ID/*, CString* str, long* chNumber, float* frequency, CString* Conv, float* Ref*/);
	//���������� ��� ��-���������
	virtual void SetDefaultCaption(LPCTSTR newVal);
	//void getServerChannels();
	//�������
	//���������� ���������� � �������������� ������
	//virtual LONG SaveConnectionLinkHandle(OLE_HANDLE ConnLinkHandle);
	//������ ����� ���������� �� ����������, ������� � ���, ��� ��������� ��� ������� ������� ����������
	//virtual LONG ChangeLocation(LONG ind);
	//�����. ��������� ����������� � ���������� ���������� � ����� ���������
	//virtual VARIANT ContInfo(LONG ContNum);
	//����������� ������������ CWnd*
	//virtual bool cwndValidate(CWnd* cwnd);
	//��������� ������ �������������� �����
	//virtual VARIANT readConnLinkHandles(LONG param);
	//�������� ��������� �� ��������� ����������
	virtual	LONG GetMethodsDisp(void);
	//�������� ������ � �������������� �������� �����������
	virtual	LONG RefreshData(OLE_HANDLE sourceWHND, LONG pinNum, LPCTSTR helpString, LONG parametr, LONG parametrType);
	//�������� ������ � �������������� �������� �����������
	virtual	LONG RefreshData2(OLE_HANDLE sourceWHND, LONG pinNum, LPCTSTR helpString, LONG pA, LONG uBound, LONG parametrType);
	//�������� ������ ���� double � �������������� �������� �����������
	virtual LONG RefreshData3(OLE_HANDLE sourceWHND, LONG pinNum, LPCTSTR helpString, LONGLONG pA, LONG uBound, LONG parametrType);
	virtual	LONG RefreshData5(OLE_HANDLE sourceWHND, LONG pinNum, LPCTSTR helpString, double parametr, LONG parametrType);
	virtual LONG RefreshDataArray1(LONG pinNum, VARIANT &theStrings, VARIANT &theArray);
	//virtual LONG RefreshDataU(OLE_HANDLE sourceWHND, ZetViewTag data);
	virtual LONG RefreshDataU(OLE_HANDLE sourceWHND, LONG pinNum, LPCTSTR helpString, LONG* lpdata);
	virtual void GetLeftData(/*contactPinInfoS& pinInfo*/long pin_number, /*SecondInfoS& secInfo*/long index);
	//virtual LONG RefreshDataArray2(LONG pinNum, LPCTSTR helpString, float* pA);
	//������ ���������� � �������� ������
	virtual LONG connectionInfo(OLE_HANDLE thisHWND, LONG thisContactNum, OLE_HANDLE secondHWND, LONG secondContactNum/*, LONG secondContactType*/);
	//��������� ����������� ��������� � ��������� ������ ����
	virtual BSTR ContactCaption(LONG contactNum);
	virtual LONG fillChannels(LONG id, LPCTSTR name, LPCTSTR conv, FLOAT freq, FLOAT refer, LONG typeADC);
	virtual LONG removeAllChannels(void);
	virtual	void applyChannels(void);

	virtual void GetLeftDataFromPin(LONG pinNum);
	virtual void OnPostRunFromScada(LONG message);
	virtual long OnGetUncertainData(long Handle, long secContNum, long myContNum);
	virtual void OnDrawInBuffer(LONG Handle);

	virtual LONG fillDevices(LONG typeADC, LONG numberDSP, LONG serial, LPCTSTR name);
	virtual LONG OnChangeGenParam(LONG param, FLOAT fValue, LPCTSTR sValue);
	//��������
	virtual ULONG GetCompWidth(void);
	virtual void SetCompWidth(ULONG newVal);

	virtual ULONG GetCompHeight(void);
	virtual void SetCompHeight(ULONG newVal);

	virtual LONG GetLeftX(void);
	virtual void SetLeftX(LONG newVal);

	virtual LONG GetLeftY(void);
	virtual void SetLeftY(LONG newVal);

	virtual LONG GetuniqueIdent(void);
	virtual void SetuniqueIdent(LONG newVal);

	virtual LONG GetSecondIdent(void);
	virtual void SetSecondIdent(LONG newVal);

	virtual BSTR GetCaption(void);
	virtual BSTR GetsHelpString(void);

	virtual BSTR GetConfiguration(void);
	virtual void SetConfiguration(LPCTSTR newVal);
public:
	virtual void SetCaption(LPCTSTR newVal);
	virtual void SetCaptionWithClassInfo(LPCTSTR newVal);
	virtual void SetsHelpString(LPCTSTR newVal);
protected:
	virtual LONG GethasLeftOffset(void);
	virtual void SethasLeftOffset(LONG newVal);

	virtual LONG GethasRightOffset(void);
	virtual void SethasRightOffset(LONG newVal);

	virtual LONG GetCompatibleDC(void);
	virtual void SetCompatibleDC(LONG newVal);

	//virtual LONG GetisSimetric(void);
	//virtual void SetisSimetric(LONG newVal);

	virtual LONG GetserverChanID(void);
	virtual void SetserverChanID(LONG newVal);

	//virtual BSTR GetchannelName(void);
	//virtual void SetchannelName(LPCTSTR newVal);

	//virtual LONG GetchannelID(void);
	//virtual void SetchannelID(LONG newVal);

	// 	virtual LONG GetInsideCWnd(void);
	// 	virtual void SetInsideCWnd(LONG newVal);

	virtual LONG GetsavedHwnd(void);
	virtual void SetsavedHwnd(LONG newVal);

public:
	virtual OLE_HANDLE GetLabelHWND(void);
	virtual void SetLabelHWND(OLE_HANDLE newVal);

	virtual	LONG GetIdentModify(void);
	virtual	void SetIdentModify(LONG newVal);

	virtual	BSTR GetToolTip(void);
	virtual	void SetToolTip(LPCTSTR newVal);

	virtual LONG GetIsSource(void);
	virtual	void SetIsSource(LONG newVal);

	virtual VARIANT_BOOL GetIsVisibleAtRunTime(void);
	virtual	void SetIsVisibleAtRunTime(VARIANT_BOOL newVal);

	virtual LONG Getqueue(void);
	virtual void Setqueue(LONG newVal);

	virtual	IDispatch* GetmeDispatch(void);
	virtual	void SetmeDispatch(IDispatch* pVal);

	virtual ULONG GetWidth(void);
	virtual void SetWidth(ULONG newVal);

	virtual ULONG GetHeight(void);
	virtual void SetHeight(ULONG newVal);

	virtual ULONG GetLeft(void);
	virtual void SetLeft(ULONG newVal);

	virtual ULONG GetTop(void);
	virtual void SetTop(ULONG newVal);

	long m_queOnRun;
	virtual LONG GetQueOnRun(void);
	virtual void SetQueOnRun(LONG newVal);

	//������
	virtual void CreateConnector(LONG cx_fromLeft, LONG cy_fromTop, OLE_HANDLE handle, long Position, long TypeOfContact, long Side, long ContNum)
	{
		FireEvent(eventidCreateConnector, EVENT_PARAM(VTS_I4 VTS_I4 VTS_HANDLE VTS_I4 VTS_I4 VTS_I4 VTS_I4), cx_fromLeft, cy_fromTop, handle, Position, TypeOfContact, Side, ContNum);
	}
	virtual void CreateConnectorDirect(LONG cx_fromLeft, LONG cy_fromTop, OLE_HANDLE handle, long Position, long TypeOfContact, long Side, long ContNum)
	{
		FireEvent(eventidCreateConnectorDirect, EVENT_PARAM(VTS_I4 VTS_I4 VTS_HANDLE VTS_I4 VTS_I4 VTS_I4 VTS_I4), cx_fromLeft, cy_fromTop, handle, Position, TypeOfContact, Side, ContNum);
	}
	virtual void PlaceConnector(LONG cx_fromLeft, LONG cy_fromTop, OLE_HANDLE handle, LONG Position, LONG TypeOfContact, long Side, long ContNum)
	{
		FireEvent(eventidPlaceConnector, EVENT_PARAM(VTS_I4 VTS_I4 VTS_HANDLE VTS_I4 VTS_I4 VTS_I4 VTS_I4), cx_fromLeft, cy_fromTop, handle, Position, TypeOfContact, Side, ContNum);
	}
	//virtual void MouseNearContact(LONG dimCX, LONG dimCY, LONG Position,  LONG ContNum, LONG LeftContact, long TypeOfContact, long Side)
	//{
	//	FireEvent(eventidMouseNearContact, EVENT_PARAM(VTS_I4 VTS_I4 VTS_I4 VTS_I4 VTS_I4 VTS_I4 VTS_I4), dimCX, dimCY, Position, ContNum, LeftContact, TypeOfContact, Side);
	//}
	/*
	virtual void requestName(LONG ID, BSTR* name, long* number, long* freq, BSTR* Conv, long* Ref)
	{
	FireEvent(eventidrequestName, EVENT_PARAM(VTS_I4 VTS_PBSTR VTS_PI4 VTS_PI4 VTS_PBSTR VTS_PI4), ID, name, number, freq, Conv, Ref);
	}
	*/
	virtual void moveSize(LONG uniqueIdent_, LONG leftX, LONG leftY, LONG cx, LONG cy)
	{
		FireEvent(eventidmoveSize, EVENT_PARAM(VTS_I4 VTS_I4 VTS_I4 VTS_I4 VTS_I4), uniqueIdent_, leftX, leftY, cx, cy);
	}
	virtual	void GetZetIDispatch(LONG Handle, IDispatch** pDisp, BSTR* pBSTR)
	{
		FireEvent(eventidGetZetIDispatch, EVENT_PARAM(VTS_I4 VTS_PDISPATCH VTS_PBSTR), Handle, pDisp, pBSTR);
	}
	virtual void ChangeComponentToolTip(BSTR text)
	{
		FireEvent(eventidComponentToolTip, EVENT_PARAM(VTS_BSTR), text);
	}
	virtual	void defineView(LONG* param)
	{
		FireEvent(eventiddefineView, EVENT_PARAM(VTS_PI4), param);
	}
	virtual	void requestType(LONG* conn_Type)
	{
		FireEvent(eventidrequestType, EVENT_PARAM(VTS_PI4), conn_Type);
	}
	virtual void MessageFromComponent(LPCTSTR string, long hWnd)
	{
		FireEvent(eventidMessageFromComponent, EVENT_PARAM(VTS_BSTR VTS_I4), string, hWnd);
	}
	void ContactPinInformation(LONG handle, LONG contNum, LPCTSTR name, LPCTSTR helpInfo, FLOAT contDivX, FLOAT contDivY, LONG side, LONG pinType, LONG outValueType, VARIANT_BOOL multiconnect, VARIANT_BOOL isOut)
	{
		FireEvent(eventidContactPinInformation, EVENT_PARAM(VTS_I4 VTS_I4 VTS_BSTR VTS_BSTR VTS_R4 VTS_R4 VTS_I4 VTS_I4 VTS_I4 VTS_BOOL VTS_BOOL), handle, contNum, name, helpInfo, contDivX, contDivY, side, pinType, outValueType, multiconnect, isOut);
	}
	/*void getSRVCWnd(LONG* srvCWnd)
		{
		FireEvent(eventidgetSRVCWnd, EVENT_PARAM(VTS_PI4), srvCWnd);
		}*/
	void requestChannels(long hWnd)
	{
		FireEvent(eventidrequestChannels, EVENT_PARAM(VTS_I4), hWnd);
	}
	void connectToServer(LONG wParam, LONG lParam)
	{
		FireEvent(eventidconnectToServer, EVENT_PARAM(VTS_I4 VTS_I4), wParam, lParam);
	}
	void connectToOPCClient(LONG wParam, LONG lParam)
	{
		FireEvent(eventidconnectToOPC_client, EVENT_PARAM(VTS_I4 VTS_I4), wParam, lParam);
	}
	void getMaxQuanDac(LONG* quanDAC)
	{
		FireEvent(eventidgetMaxQuanDac, EVENT_PARAM(VTS_PI4), quanDAC);
	}
	void serverCurrentTime(LONG chanNum, FLOAT* time)
	{
		FireEvent(eventidserverCurrentTime, EVENT_PARAM(VTS_I4 VTS_PR4), chanNum, time);
	}
	void serverGetData(LONG channel, LONG decade, DOUBLE time, LONG size, FLOAT* data)
	{
		FireEvent(eventidserverGetData, EVENT_PARAM(VTS_I4 VTS_I4 VTS_R8 VTS_I4 VTS_PR4), channel, decade, time, size, data);
	}
	void serverNumFileUsed(LONG channel)
	{
		FireEvent(eventidserverNumFileUsed, EVENT_PARAM(VTS_I4), channel);
	}
	void Registration(LPCTSTR sParam, LONG nParam, LONG* pRetParam)
	{
		FireEvent(eventidRegistration, EVENT_PARAM(VTS_BSTR VTS_I4 VTS_PI4), sParam, nParam, pRetParam);
	}
	void setCompPropertyBOOL(OLE_HANDLE handle, LONG id, VARIANT_BOOL parametr)
	{
		FireEvent(eventidsetCompPropertyBOOL, EVENT_PARAM(VTS_HANDLE VTS_I4 VTS_BOOL), handle, id, parametr);
	}
	void setCompPropertyLONG(OLE_HANDLE handle, LONG id, LONG parametr)
	{
		FireEvent(eventidsetCompPropertyLONG, EVENT_PARAM(VTS_HANDLE VTS_I4 VTS_I4), handle, id, parametr);
	}
	void setCompPropertyFLOAT(OLE_HANDLE handle, LONG id, FLOAT parametr)
	{
		FireEvent(eventidsetCompPropertyFLOAT, EVENT_PARAM(VTS_HANDLE VTS_I4 VTS_R4), handle, id, parametr);
	}
	void setCompPropertyBSTR(OLE_HANDLE handle, LONG id, LPCTSTR parametr)
	{
		FireEvent(eventidsetCompPropertyBSTR, EVENT_PARAM(VTS_HANDLE VTS_I4 VTS_BSTR), handle, id, parametr);
	}
	void getCompPropertyBOOL(OLE_HANDLE handle, LONG id, LONG* parametr)
	{
		FireEvent(eventidgetCompPropertyBOOL, EVENT_PARAM(VTS_HANDLE VTS_I4 VTS_PBOOL), handle, id, parametr);
	}
	void getCompPropertyLONG(OLE_HANDLE handle, LONG id, LONG* parametr)
	{
		FireEvent(eventidgetCompPropertyLONG, EVENT_PARAM(VTS_HANDLE VTS_I4 VTS_PI4), handle, id, parametr);
	}
	void getCompPropertyFLOAT(OLE_HANDLE handle, LONG id, FLOAT* parametr)
	{
		FireEvent(eventidgetCompPropertyFLOAT, EVENT_PARAM(VTS_HANDLE VTS_I4 VTS_PR4), handle, id, parametr);
	}
	void getCompPropertyBSTR(OLE_HANDLE handle, LONG id, BSTR* parametr)
	{
		FireEvent(eventidgetCompPropertyBSTR, EVENT_PARAM(VTS_HANDLE VTS_I4 VTS_PBSTR), handle, id, parametr);
	}
	void getCompNameProgID(OLE_HANDLE handle, BSTR* name, BSTR* progID)
	{
		FireEvent(eventidgetCompNameProgID, EVENT_PARAM(VTS_HANDLE VTS_PBSTR VTS_PBSTR), handle, name, progID);
	}
	void getProjectName(BSTR* name)
	{
		FireEvent(eventidgetProjectName, EVENT_PARAM(VTS_PBSTR), name);
	}
	void GetSCADAGuid(LONG handle, BSTR* GUID_Str)
	{
		FireEvent(eventidGetSCADAGuid, EVENT_PARAM(VTS_I4 VTS_PBSTR), handle, GUID_Str);
	}
	void CreateArray(LONG compHandle, VARIANT_BOOL bStatic, LONG size, LONG* IDStr_, LONG* pArrayStr_)
	{
		FireEvent(eventidCreateArray, EVENT_PARAM(VTS_I4 VTS_BOOL VTS_I4 VTS_PI4 VTS_PI4), compHandle, bStatic, size, IDStr_, pArrayStr_);
	}
	void UpdateArray(LONG compHandle, LONG pinNum, LONG size, LONG IDStr_)
	{
		FireEvent(eventidUpdateArray, EVENT_PARAM(VTS_I4 VTS_I4 VTS_I4 VTS_I4), compHandle, pinNum, size, IDStr_);
	}
	void CreateArrayStr(LONG compHandle, VARIANT_BOOL bStatic, LONG size, LONG* IDStr_, LONG* pArrayStr_)
	{
		FireEvent(eventidCreateArrayStr, EVENT_PARAM(VTS_I4 VTS_BOOL VTS_I4 VTS_PI4 VTS_PI4), compHandle, bStatic, size, IDStr_, pArrayStr_);
	}
	void CreateArrayDouble(LONG compHandle, VARIANT_BOOL bStatic, LONG size, LONG* IDStr_, LONGLONG* pArrayDouble)
	{
		FireEvent(eventidCreateArrayDouble, EVENT_PARAM(VTS_I4 VTS_BOOL VTS_I4 VTS_PI4 VTS_PI8), compHandle, bStatic, size, IDStr_, pArrayStr);
	}
	void refreshStandardSize(LONG handle, LONG cx, LONG cy, LONG hasLeftOffset, LONG hasRightOffset, LONG m_hasTopOffset_, LONG hasBottomOffset)
	{
		FireEvent(eventidrefreshStandardSize, EVENT_PARAM(VTS_I4 VTS_I4 VTS_I4 VTS_I4 VTS_I4 VTS_I4 VTS_I4), handle, cx, cy, hasLeftOffset, hasRightOffset, m_hasTopOffset_, hasBottomOffset);
	}
	void SetGenParam(LONG handle, LONG param, FLOAT fValue, BSTR sValue)
	{
		FireEvent(eventidSetGenParam, EVENT_PARAM(VTS_I4 VTS_I4 VTS_R4 VTS_BSTR), handle, param, fValue, sValue);
	}
	void ShowScadaConsole(void)
	{
		FireEvent(eventidShowScadaConsole, EVENT_PARAM(VTS_NONE));
	}
	void HideScadaConsole(void)
	{
		FireEvent(eventidHideScadaConsole, EVENT_PARAM(VTS_NONE));
	}
	void PrintScadaConsole(LPCTSTR string)
	{
		FireEvent(eventidPrintScadaConsole, EVENT_PARAM(VTS_BSTR), string);
	}
	void SetFilterParam(LONG handle, LONG param, FLOAT fValue, BSTR sValue)
	{
		FireEvent(eventidSetFilterParam, EVENT_PARAM(VTS_I4 VTS_I4 VTS_R4 VTS_BSTR), handle, param, fValue, sValue);
	}
	void DeleteArray(LONG compHandle, LONG ID, LONG pArray)
	{
		FireEvent(eventidDeleteArray, EVENT_PARAM(VTS_I4 VTS_I4 VTS_I4), compHandle, ID, pArray);
	}
	void GoTo(BSTR sValue)
	{
		FireEvent(eventidGoTo, EVENT_PARAM(VTS_BSTR), sValue);
	}
	void GetScrollPositionEvent(LONG* vertical, LONG* horizontal)
	{
		FireEvent(eventidGetScrollPositionEvent, EVENT_PARAM(VTS_PI4 VTS_PI4), vertical, horizontal);
	}
	void SetFormulaParam(LONG handle, LONG param, FLOAT fValue, BSTR sValue)
	{
		FireEvent(eventidSetFormulaParam, EVENT_PARAM(VTS_I4 VTS_I4 VTS_R4 VTS_BSTR), handle, param, fValue, sValue);
	}
	void UpdateContactData_FLOAT(LONG handle, long pinNum, FLOAT param)
	{
		FireEvent(eventidUpdateContactData_FLOAT, EVENT_PARAM(VTS_I4 VTS_I4 VTS_R4), handle, pinNum, param);
	}
	void UpdateContactData_LONG(LONG handle, long pinNum, LONG param)
	{
		FireEvent(eventidUpdateContactData_LONG, EVENT_PARAM(VTS_I4 VTS_I4 VTS_I4), handle, pinNum, param);
	}
	void UpdateContactData_STRING(LONG handle, long pinNum, BSTR param)
	{
		FireEvent(eventidUpdateContactData_STRING, EVENT_PARAM(VTS_I4 VTS_I4 VTS_BSTR), handle, pinNum, param);
	}
	void GetMouseCursorPosition(LONG* horisontal, LONG* vertical)
	{
		FireEvent(eventidGetMouseCursorPosition, EVENT_PARAM(VTS_PI4 VTS_PI4), horisontal, vertical);
	}

	long serverCWndLong;
public:
	enum {
		//�������������� �������
		eventidCreateConnector = 101,
		eventidPlaceConnector = 102L,
		eventiddefineView = 103L,
		eventidMouseNearContact = 105L,
		//eventidrequestName = 107L,
		eventidCreateConnectorDirect = 107L,
		eventidmoveSize = 108L,
		eventidGetZetIDispatch = 109L,
		eventidComponentToolTip = 110L,
		eventidrequestType = 111L,
		eventidMessageFromComponent = 112L,
		//eventidgetSRVCWnd = 113L,
		//������ ������� ���������� ZET.PageViewer.1
		eventidupdateCoordinateGrid = 114L,
		eventidupdateForeColor = 115L,
		eventidupdateBackColor = 116L,
		eventidupdatePageName = 117L,
		eventidrequestChannels = 118L,
		eventidconnectToServer = 119L,
		eventidgetMaxQuanDac = 120L,
		eventidserverCurrentTime = 121L,
		eventidserverGetData = 122L,
		eventidserverNumFileUsed = 123L,
		eventidshowByPageViewer = 124L,
		eventidsetCompPropertyBOOL = 125L,
		eventidsetCompPropertyLONG = 126L,
		eventidsetCompPropertyFLOAT = 127L,
		eventidsetCompPropertyBSTR = 128L,
		//������� ������ � MODBUS Ethernet
		eventidconnectToOPC_client = 129L,
		eventidInputDataChanged = 130L,
		//eventidMODBUS_SEND_DOUBLE = 130L,
		eventidMODBUS_SEND_INT = 131L,
		eventidMODBUS_SEND_STRING = 132L,
		eventidMODBUS_CHANGE_INTERFACE = 133L,
		//������� ������ � MODBUS Ethernet
		eventidOPC_SEND_FLOAT = 134L,
		eventidOPC_SEND_DOUBLE = 135L,
		eventidOPC_SEND_INT = 136L,
		eventidOPC_SEND_STRING = 137L,
		eventidOPC_CHANGE_INTERFACE = 138L,
		eventidRegistration = 139L,
		eventidCreateArray = 140L,
		eventidgetCompPropertyBOOL = 141L,
		eventidgetCompPropertyLONG = 142L,
		eventidgetCompPropertyFLOAT = 143L,
		eventidgetCompPropertyBSTR = 144L,
		eventidgetProjectName = 145L,
		eventidGetSCADAGuid = 146L,
		eventidgetCompNameProgID = 147L,
		eventidCreateArrayStr = 148L,
		eventidContactPinInformation = 149L,
		eventidrefreshStandardSize = 150L,
		eventidSetGenParam = 151L,
		eventidPrintScadaConsole = 154L,
		eventidHideScadaConsole = 153L,
		eventidShowScadaConsole = 152L,
		eventidSetFilterParam = 155L,
		eventidDeleteArray = 156L,
		eventidGoTo = 157L,
		eventidCreateArrayDouble = 158L,
		eventidGetScrollPositionEvent = 159L,
		eventidSetFormulaParam = 160L,
		eventidUpdateContactData_FLOAT = 161L,
		eventidUpdateContactData_LONG = 162L,
		eventidUpdateContactData_STRING = 163L,
		eventidUpdateArray = 164L,
		eventidGetMouseCursorPosition = 178L,
		eventidOpcUaDataChanged = 179L,

		//�������������� ������� � �������
		//dispidChannelNames = 200L,
		dispidPutSecondPointHWND = 201L,
		dispidSaveConnectionLinkHandle = 202L,
		//dispidChangeLocation = 204L,
		//dispidContInfo = 205L,
		dispidCompWidth = 206,
		dispidCompHeight = 207,
		dispidLeftX = 208,
		dispidLeftY = 209,
		dispidCaption = 210,
		dispiduniqueIdent = 211,
		dispidSecondIdent = 212,
		dispidhasLeftOffset = 213,
		dispidisSimetric = 214,
		dispidCompatibleDC = 215,	//
		dispidDrawInBuffer = 216,
		//dispidchannelID = 217,
		//dispidInsideCWnd = 218,
		dispidsavedHwnd = 219,
		//dispidreadConnLinkHandles = 220,
		dispidLabelHWND = 221,
		dispidhasRightOffset = 222,
		dispidIdentModify = 223,
		dispidToolTip = 224,
		dispidGetMethodsDisp = 225,
		dispidIsSource = 226,
		dispidRefreshData = 227,
		dispidconnectionInfo = 228,
		dispidContactCaption = 229,
		dispidIsVisibleAtRunTime = 230,
		dispidsHelpString = 231,
		dispidqueue = 232,
		dispidmeDispatch = 233,
		dispidRefreshDataArray = 234,
		dispidRefreshDataArray1 = 235L,
		dispidWidth = 236,
		dispidHeight = 237,
		dispidLeft = 238,
		dispidTop = 239,
		dispidfillChannels = 240L,
		dispidremoveAllChannels = 241L,
		dispidapplyChannels = 242L,
		//dispidRefreshDataArray2 = 243L,
		dispidRefreshData2 = 244L,
		dispidfillDevices = 245L,
		dispidOnChangeGenParam = 246L,
		dispidRefreshData3 = 247L,
		dispidConfiguration = 248,
		dispidRefreshData4 = 249,
		dospidGetPostRunFromScada = 250,
		dispidGetUncertainData = 251,
		dispidQueOnRun = 252,
		dispidRefreshData5 = 253,
		dispidResetMemoryLeak = 254L,
		dispidGetMemoryLeak = 255L,
		dispidRefreshDataU = 256L,
		dispidGetAllocHeapBl = 257L,
		dispidGetAllocHeapSz = 258L
	};
	virtual void CheckVisibleAtRunTime(WPARAM wParam);
	//�������� ������� ����������� ����������� ActiveX ����������
	long CompWidth, CompWidthStandard;//, OperateWidth, DesignWidthStandard;		//������
	long CompHeight, CompHeightStandard;//, OperateHeight, DesignHeightStandard;	//������
	long LeftX;				//���������� ������ �������� ����
	long LeftY;				//-.-
	long uniqueIdent;		//���������� ������������� ����������� � ������������
	long SecondID;			//���������� ������������� ����������� � ������ ���������
	//long Simetric;			//�������������� ����������

	//long m_nLabelVisible;	 //������ ��������� ������
	BOOL m_nVisibleAtRunTime;//������ ��������� � ������ �������

	//����������� ������
	long m_hasLeftOffset;	//����� �� ������ �����
	long m_hasRightOffset;	//����� �� ������ ������
	long m_hasTopOffset;	//����� �� ������ ������
	long m_hasBottomOffset;	//����� �� ������ �����

	//�������� ��� ������� ������� ����� ������
	virtual void setOffsets(bool left = false, bool right = false, bool top = false, bool bottom = false);
	virtual void setCaption(const CString &newCaption);
	virtual void SetServerInterface();				//�������� ����� � ��������
	//virtual void SetDefaultCursor(LPCWSTR curParam);

	//��������� ���������� ��������
	//CRect rcBounds;
	//CSize size;
	//CRect padRect;
	//virtual void InvalidateContactPad(long position, long side);
	//virtual void InvalidateContactString(long position, long side);

	//��������� ActiveX ����������
	CString caption;	//��������
	HWND m_mainHandle;	//����� ����������
	HWND m_thisHandle;  //����� ������� ActiveX�
	//HWND m_labelHandle; //����� ������

	//CArray <SecondInfoS, SecondInfoS&> SecondPointHandles;		//������ �������� ����� ����������
	std::vector <SecondInfoS> SecondPointHandles;		//������ �������� ����� ����������
	//CArray <HWND, HWND> m_connectionLinkHandles;				//������ �������������� �����

	//��������� ����������
	//int i,j;				//������������ ��� �������
	CString temp_str;		//�������� ������
	//long tempID;			//�������� ���������� long
	CRect rect;				//�������� �������������
	CPoint pt;				//�������� ���������� - CPoint
	//long oldContact;

	//HCURSOR default_Cursor;

	double ID_chan;				//ID �������� ������
	CString m_sSelected_channel;//��� �������� ������
	long m_nSelChanNumber;		//���������� ����� ���������� ������
	long m_nProjectNumber;		//����� �������
	CString m_sHelpString;
	CString m_sToolTip;
	CString m_sConfiguration;
	//���������� ������� �������
	//long serverChanCount;

	bool haveServerInterface;
	bool haveOPCClientInterface;
	void SetOPCClientInterface();
	bool FindChannelFromName(const CString &name, long &id);

	//CArray <contactPinInfoS,contactPinInfoS> contactPinInfoArray;	//���� ������ ���������� ���������� �����
	std::vector<contactPinInfoS> contactPinInfoArray;	//���� ������ ���������� ���������� �����
	//����� ������� ��� ������ ��������

	//�������� ���������� �������� �����������
	void doRefreshData1(int pinNum, const CString &helpString, long parametr, long parametrType);
	void doRefreshData2(int pinNum, const CString &helpString, long parametr, long arrSize, long parametrType);
	void doRefreshData3(int pinNum, const CString &helpString, long long parametr, long arrSize, long parametrType);
	void doRefreshData4(int pinNum, const CString &helpString, IDispatch* parametr, long parametrType);
	void doRefreshData5(int pinNum, const CString &helpString, double parametr, long parametrType);
	void doRefreshDataU(int pinNum, const CString &helpString, ZetViewTag *pData);
	//void doRefreshData(CString secondName, CString helpString ,long parametr, long parametrType);
	//long doRefreshDataArray(int pinNum, float* pArray, long firstElement, long arrSize, bool bAllowRefreshHelpInfo);

	//��������� ��������� ���������, ������� ����� ���� ������� ��� � ��������� 
#ifdef INSIDE_ACTIVEX
	InsideActiveXS InsideActiveX;
	virtual void InstallActiveX(LPOLESTR clsidStr, bool enable, CString name);
#endif

	long m_bInOperate;	//����������, ���������������� ����������� ���������� (������ ����� ��� ������������)
	//���������� ����� ��� ��������
	long queue;
	//����� ���������� ����������� ������\��������, ���� false
	virtual LRESULT OnUpdateBackColor(WPARAM wParam, LPARAM lParam);
	virtual LRESULT OnMinMax(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT onConnectToServer(WPARAM wParam, LPARAM lParam);

	//���������� ������ �������� � ���� ������
	//����������� �������� � ��������� �� ��������� ���������������
	//CArray <libInfoS,libInfoS> libInfo;
	std::vector<libInfoS> libInfo;
	virtual CString addLibs(const CString &name, CWnd* cwnd);
	virtual CString addLibs(const CString &name, IDispatch* pDisp);
	//virtual CString addLibsThis(CString name, CWnd* cwnd);
	//-------------------------------------------------------------
	bool b_isScript;
	CString MakeLowerUnicode(const CString &text);
	CString toTranslit(CString russian);
	CString toRemoveSymbols(CString text);

	bool m_bRunMode;
	bool m_bDebug;
	bool b_Refreshed;
	bool m_bHaveInsideActiveX;
	bool m_bMoveInsideActiveX;
	bool isSource;		//���� - ���������� ��� ������ ��������� �����������

	long IDFromName(CString name, long& num);
	void SetModifiedFlag(BOOL bModified = 1);	//���������� ���� ���������� �������� ���������� ���� �������������� �����������
	void StringSeparate(CString helpString, ULONG index = 0);	//�������� ������ �� ������
	//void GradientFillRect(HDC pDC, CRect ARect, COLORREF StartColor, COLORREF EndColor, int Colors);

	//CBrush backBrush;

	//CArray <tempChanInfoS,tempChanInfoS> tempChanInfo;
	std::vector<tempChanInfoS> tempChanInfo;
	tempChanInfoS tempChan_0_Array[MAX_INPUTCHAN];

	//long lastFillColor;
	//long lastAlign;
	//long lastSize;

	//��������� �� ��������� ��������������� ������ ���� 
	//������������ ��� �������� ������� ���������� ("���������� ���� �����������")
	//���� ���������� ����� lpDispatch ���� ����� - ��� ����� ��������
	IDispatch* meDisp;

	ServerInfoS serverInfo;
	int m_oldQuanChan;
	long m_nOffset;

	DWORD m_nCurrentVersion;
	DWORD m_nLoadingVersion;

	DISPPARAMS params_RefreshData;
	DISPPARAMS params_RefreshData2;
	DISPPARAMS params_RefreshData3;

	COLORREF m_scadaBackColor;
	WORD r_scadaBackColor;
	WORD g_scadaBackColor;
	WORD b_scadaBackColor;

	//Bitmap* basic_backBuffer;
	//CRect m_oldRect_basic;

	//����������� �������� �� ����� OPC � Long � String � �������
	TDataQuality  TDataQualityFromString(const CString &s_quality);
	LONG SignalTypeFromString(const CString &s_quality);
	LONG SignalTypeFromTDataQuality(long n_quality);

	void AddManagerContact();				//�������� ����� ��������� �������

	afx_msg void OnTimer(UINT_PTR nIDEvent);
	//	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnWindowPosChanging(WINDOWPOS* lpwndpos);

	// ������ � ��������� �����
	void ClearArrayStr(void);
	void CreateArrayStr(void);
	void fillDefArrayStr(void);
	long IDStr;
	CString* pArrayStr;
	// ------------------------

	void OnModifyValue(int pinIndex, float Val, const CString &helpString);
	void OnModifyValue(int pinIndex, double Val, const CString &helpString);
	void OnModifyValue(int pinIndex, long Val, const CString &helpString);
	void OnModifyValue(int pinIndex, const CString &Val, const CString &helpString);
	//void popClassInfo();

	virtual afx_msg LRESULT OnRemoveChannels(WPARAM wParam, LPARAM lParam);
	virtual afx_msg LRESULT OnAddChannels(WPARAM wParam, LPARAM lParam);

	// ������� ����
	CString m_strTranslateDst;
	CString m_nameOcx;

	/*������� �������� ����� ����� ���������� ZetDictionary
	��� �������� � ocx ������ ���� ��������� #define _TRANSLATE
	@return ������������ ������
	@param ID ������ � ��������*/
	CString Translate(_In_ const UINT & nID);
	/*������� �������� ����� ����� ���������� ZetDictionary
	��� �������� � ocx ������ ���� ��������� #define _TRANSLATE
	@return ������������ ������
	@param �������� ������*/
	CString Translate(_In_ const CString &str);

private:
	// ��������� ����������� ����� ���� BSTR
	void DisableBSTRCache();
	typedef int(*SETOANOCACHE)(void);

private:
	uint64_t m_lCurrComponentDeviceId;	// ������������� device_id ������� ����� ����������
	uint32_t m_lCurrComponentModuleId;	// ������������� module_id ������� ����� ����������
protected:
	uint64_t getCurrComponentDeviceId();	// ���������� ���������� ������������� device_id ������� ����� ����������
	uint32_t getCurrComponentModuleId();	// ���������� ���������� ������������� module_id ������� ����� ����������
};

//};

///////////////////////////////////