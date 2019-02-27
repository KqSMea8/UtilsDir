// Copyright (C) 2007 by ZET  http://www.zetms.ru
// Written by Krasovskiy Andrey, e-mail: krasovskiyAndrey@mail.ru
//										 info@zetms.ru
// All rights reserved
// ������ ���� ���������� � �������� �������� ��� �����������, 
// � ����������, ���������� � SCADA ������� �������� ZET.
// ������ ���� ����������� �� �������� ������ COleControl, � 
// ������������ ��� ���������� ����� ������.
// CZetBasicControl ���������� ��� ��������� �������, �������, �������
// � �������, ����������� ��� ��������� � SCADA ������� ZET, ��� 
// � ��������� ActiveX ���������.

#pragma once
#include "afxctl.h"
#include "C:\user\Projects\ChartEditor\user_mess.h"

class CZetBasicOle :
	public COleControl
{
public:
	CZetBasicOle(void);
public:
	~CZetBasicOle(void);

DECLARE_EVENT_MAP()
DECLARE_DISPATCH_MAP()
DECLARE_MESSAGE_MAP()

virtual void DoPropExchange(CPropExchange* pPX);
virtual int  OnCreate(LPCREATESTRUCT lpCreateStruct);
virtual void testMessage(float par);
virtual void OnClose(DWORD dwSaveOption);
virtual void OnMouseMove(UINT nFlags, CPoint point);
virtual void OnLButtonDown(UINT nFlags, CPoint point);
virtual void OnSize(UINT nType, int cx, int cy);
virtual void OnLButtonUp(UINT nFlags, CPoint point);

protected:
//�������� ��������� 
virtual LRESULT OnMouseNearComponent(WPARAM wParam, LPARAM lParam);
virtual LRESULT OnPlaceConnector(WPARAM wParam, LPARAM lParam);
virtual LRESULT OnSecondConnect(WPARAM wParam, LPARAM lParam);
//��������������� ��������� ActiveX� � ����� ��
virtual LRESULT SecondPointHWND(WPARAM wParam, LPARAM lParam);
//��������� �� ���������� �������
virtual LRESULT OnServerModify(WPARAM wParam, LPARAM lParam);	
//�������� ����������� ���������� - ������ ����� ��� ������������
virtual LRESULT OnChangeForm(WPARAM wParam, LPARAM lParam);
//��������� �� ���������� ���������� �� ����������
virtual LRESULT OnUpdateByConteiner(WPARAM wParam, LPARAM lParam);	

//��������� �� ������ ���������� �����, ����������� ���������� ������������ �������� �������� ����������
virtual void fillContactList(long contNum, CString nsme, float contDivX, float contDivY, long side, long connType, long disconnType, long pinType, bool multiconnect, bool isOut = false);
//������ �������� ������� ActiveX ����������
virtual void setLinearSizes(long cx, long cy);
//������ ������
virtual void drawRegion(CDC* pdc, CRect rcBounds);
//����������� ������������ CWnd*
virtual void ChNameFromID(long ID, CString* str, long* chNumber);

//�������
//���������� ���������� � �������������� ������
virtual LONG SaveConnectionLinkHandle(OLE_HANDLE ConnLinkHandle);
//������ ����� ���������� �� ����������, ������� � ���, ��� ��������� ��� ������� ������� ����������
virtual LONG ChangeLocation(LONG ind);
//�����. ��������� ����������� � ���������� ���������� � ����� ���������
virtual VARIANT ContInfo(LONG ContNum);
//����� ������� ������ ActiveX� � ���, ��� ������������ ������ ����� ����������
virtual LONG PutSecondPointHWND(OLE_HANDLE hwnd, LONG contNum);
//����������� ������������ CWnd*
virtual bool cwndValidate(CWnd* cwnd);

//��������
virtual LONG GetCompWidth(void);
virtual void SetCompWidth(LONG newVal);

virtual LONG GetCompHeight(void);
virtual void SetCompHeight(LONG newVal);

virtual LONG GetLeftX(void);
virtual void SetLeftX(LONG newVal);

virtual LONG GetLeftY(void);
virtual void SetLeftY(LONG newVal);

virtual LONG GetuniqueIdent(void);
virtual void SetuniqueIdent(LONG newVal);

virtual LONG GetSecondIdent(void);
virtual void SetSecondIdent(LONG newVal);

virtual BSTR GetCaption(void);
virtual void SetCaption(LPCTSTR newVal);

virtual LONG GethasLeftOffset(void);
virtual void SethasLeftOffset(LONG newVal);

virtual LONG GetisSimetric(void);
virtual void SetisSimetric(LONG newVal);

virtual LONG GetserverChanID(void);
virtual void SetserverChanID(LONG newVal);

virtual BSTR GetchannelName(void);
virtual void SetchannelName(LPCTSTR newVal);

virtual LONG GetchannelID(void);
virtual void SetchannelID(LONG newVal);

//������
virtual void CreateConnector(LONG cx_fromLeft, LONG cy_fromTop, OLE_HANDLE handle, long Position, long TypeOfContact, long Side, long ContNum)
	{
		FireEvent(eventidCreateConnector, EVENT_PARAM(VTS_I4 VTS_I4 VTS_HANDLE VTS_I4 VTS_I4 VTS_I4 VTS_I4), cx_fromLeft, cy_fromTop, handle, Position, TypeOfContact, Side, ContNum);
	}
virtual void PlaceConnector(LONG cx_fromLeft, LONG cy_fromTop, OLE_HANDLE handle, LONG Position, LONG TypeOfContact, long Side, long ContNum)
	{
		FireEvent(eventidPlaceConnector, EVENT_PARAM(VTS_I4 VTS_I4 VTS_HANDLE VTS_I4 VTS_I4 VTS_I4 VTS_I4), cx_fromLeft, cy_fromTop, handle, Position, TypeOfContact, Side, ContNum);
	}
virtual void MouseNearContact(LONG dimCX, LONG dimCY, LONG Position,  LONG ContNum, LONG LeftContact, long TypeOfContact, long Side)
	{
		FireEvent(eventidMouseNearContact, EVENT_PARAM(VTS_I4 VTS_I4 VTS_I4 VTS_I4 VTS_I4 VTS_I4 VTS_I4), dimCX, dimCY, Position, ContNum, LeftContact, TypeOfContact, Side);
	}
virtual void requestName(LONG ID, BSTR* name, long* number)
	{
		FireEvent(eventidrequestName, EVENT_PARAM(VTS_I4 VTS_PBSTR VTS_I4), ID, name, number);
	}
virtual void moveSize(LONG uniqueIdent, LONG leftX, LONG leftY, LONG cx, LONG cy)
	{
		FireEvent(eventidmoveSize, EVENT_PARAM(VTS_I4 VTS_I4 VTS_I4 VTS_I4 VTS_I4),uniqueIdent, leftX, leftY, cx, cy);
	}
public:
	enum {
		//�������������� �������
		eventidCreateConnector = 101,
		eventidPlaceConnector = 102L,
		eventidMouseNearContact = 105L,
		eventidrequestName = 107L,
		eventidmoveSize = 108L,

		//�������������� ������� � �������
		dispidChannelNames = 200L,
		dispidPutSecondPointHWND = 201L,
		dispidSaveConnectionLinkHandle = 202L,
		dispidChangeLocation = 204L,
		dispidContInfo = 205L,
		dispidCompWidth = 206,
		dispidCompHeight = 207,
		dispidLeftX = 208,
		dispidLeftY = 209,
		dispidCaption = 210,
		dispiduniqueIdent = 211,
		dispidSecondIdent = 212,
		dispidhasLeftOffset = 213,
		dispidisSimetric = 214,
		dispidserverChanID = 215,
		dispidchannelName = 216,
		dispidchannelID = 217
	};
//�������� ������� ����������� ����������� ActiveX ����������
long CompWidth;			//������
long CompHeight;		//������
long LeftX;				//���������� ������ �������� ����
long LeftY;				//-.-
long uniqueIdent;		//���������� ������������� ����������� � ������������
long SecondID;			//���������� ������������� ����������� � ������ ���������
long Simetric;			//�������������� ����������

//����������� ������
long m_hasLeftOffset;	//����� �� ������ ����� (��������, ���� � ���������� ���� ���������� ���)
long m_hasRightOffset;	//����� �� ������ ������
//�������� ��� ������� ������� ����� ������
virtual void setOffsets(bool left = false, bool right = false);

//��������� ActiveX ����������
CString caption;	//��������
HWND m_mainHandle;	//����� ����������
HWND m_thisHandle;  //����� ������� ActiveX�

//������� �����
struct SecondInfoS			//�������� �������� �����
{
	HWND hwnd;				//����� �������� �����
	long contact;			//����� �������� �������� �����
};
CArray <SecondInfoS, SecondInfoS> SecondPointHandles;		//������ �������� ����� ����������

CArray <HWND, HWND> m_connectionLinkHandles;				//������ �������������� �����

//��������� ����������
int i,j;				//������������ ��� �������
CString temp_str;		//�������� ������
long tempID;			//�������� ���������� long
CRect rect;				//�������� �������������
CPoint pt;				//�������� ���������� - CPoint
long oldContact;

HCURSOR default_Cursor;
virtual void SetDefaultCursor(LPCWSTR curParam);

//���������� � �������������� �������
struct chanInfoS
	{
		CString chanName;		//��� ������
		long chanID;			//ID ������
	};
long ID_chan;				//ID �������� ������
CString selected_channel;	//��� �������� ������
long m_nSelChanNumber;		//���������� ����� ���������� ������

//���������� ������� �������
long serverChanCount;
CArray <chanInfoS,chanInfoS> serverChanInfo;	//���� ������ ������� �������
bool haveServerInterface;
virtual void SetServerInterface();				//�������� ����� � ��������

//���������� � ���������� �����
struct contactPinInfoS
	{
		float pointDivX;	//X ���������� ��������
		float pointDivY;	//Y ���������� ��������
		long side;			//������� ��������� ��������
		long connType;		//��� ����������, ��������, WPARAM_IDCHAN_CONNECTED ��� WPARAM_ID_SWITCH_CONNECTED
		long disconnType;	//��� �����������, ��������,WPARAM_IDCHAN_DISCONNECTED
		bool connected;		//������������ ��� ���
		long number;		//����� ��������
		CString name;		//�������� ����
		long pinType;		//��� ���������, �������� - OUT_POLYSIGNAL
		bool multiConnect;  //��������� ����������������
		CRect contactRect;  //�������� ������� ���������� ��������
		bool isOut;			//�������� �� ������� - ��������
		long outValue;		//�������� �������� ��� �������� �� ����� ��������������� ����������
	};
long savedValueToSend;		//�������� ��� ����� ��������������� ����������, ������ ��� ���������� ���
							//��������, ������ �������� ����� �������� �������������� � ������������������
CArray <contactPinInfoS,contactPinInfoS> contactPinInfoArray;	//���� ������ ���������� ���������� �����
virtual void SetOutputValueforConnect(long newValue);		//������ ��������, ������� ���������� ��� ��������
															//����� ������� ��� ������ ��������
virtual void SetOutputValueforConnect(long contactNumber, long newValue);

//��������� ��������� ���������, ������� ����� ���� ������� ��� � ��������� 
struct InsideActiveXS
{
	LPOLESTR clsidStr;
	CLSID clsidActiveX;
	CWnd* ActiveXcwnd;
};
InsideActiveXS InsideActiveX;
virtual void InstallActiveX(LPOLESTR clsidStr, bool enable, CString name);

long m_bInOperate;	//����������, ���������������� ����������� ���������� (������ ����� ��� ������������)

};
