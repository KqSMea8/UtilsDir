// Copyright (C) 2007 by ZET  http://www.zetms.ru
// Written by Krasovskiy Andrey, e-mail: krasovskiyAndrey@mail.ru
//										 info@zetms.ru
// All rights reserved
// Данный клас разработан в качестве базового для компонентов, 
// в дальнейшем, внедряемых в SCADA систему компании ZET.
// Данный клас унаследован от базового класса COleControl, и 
// поддерживает все интерфейсы этого класса.
// CZetBasicControl разработан для поддержки функций, методов, событий
// и свойств, необходимых для внедрения в SCADA систему ZET, как 
// в контейнер ActiveX элементов.

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
//пропишем сообщения 
virtual LRESULT OnMouseNearComponent(WPARAM wParam, LPARAM lParam);
virtual LRESULT OnPlaceConnector(WPARAM wParam, LPARAM lParam);
virtual LRESULT OnSecondConnect(WPARAM wParam, LPARAM lParam);
//регестрирование соседнего ActiveXа в своей БД
virtual LRESULT SecondPointHWND(WPARAM wParam, LPARAM lParam);
//сообщение об обновлении сервера
virtual LRESULT OnServerModify(WPARAM wParam, LPARAM lParam);	
//изменяет отображение компонента - дизайн режим или операторский
virtual LRESULT OnChangeForm(WPARAM wParam, LPARAM lParam);
//сообщение об обновлении компонента из контейнера
virtual LRESULT OnUpdateByConteiner(WPARAM wParam, LPARAM lParam);	

//заполняет БД списка контактных пинов, описывается координата относительно линейный размеров компонента
virtual void fillContactList(long contNum, CString nsme, float contDivX, float contDivY, long side, long connType, long disconnType, long pinType, bool multiconnect, bool isOut = false);
//задаем линейные размеры ActiveX компонента
virtual void setLinearSizes(long cx, long cy);
//рисуем регион
virtual void drawRegion(CDC* pdc, CRect rcBounds);
//определение актуальности CWnd*
virtual void ChNameFromID(long ID, CString* str, long* chNumber);

//функции
//знакомство компонента с соединительной линией
virtual LONG SaveConnectionLinkHandle(OLE_HANDLE ConnLinkHandle);
//Данный метод вызывается из контейнера, говорит о том, что контейнер уже изменил размеры компонента
virtual LONG ChangeLocation(LONG ind);
//метод. контейнер запрашивает у компонента информацию о точке крепления
virtual VARIANT ContInfo(LONG ContNum);
//метод говорит нашему ActiveXу о том, что прикрепилась вторая точка соединения
virtual LONG PutSecondPointHWND(OLE_HANDLE hwnd, LONG contNum);
//определение актуальности CWnd*
virtual bool cwndValidate(CWnd* cwnd);

//свойства
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

//Евенты
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
		//идентификаторы событий
		eventidCreateConnector = 101,
		eventidPlaceConnector = 102L,
		eventidMouseNearContact = 105L,
		eventidrequestName = 107L,
		eventidmoveSize = 108L,

		//идентификаторы методов и функций
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
//линейные размеры отображения конкретного ActiveX компонента
long CompWidth;			//ширина
long CompHeight;		//высота
long LeftX;				//координата левого верхнего угла
long LeftY;				//-.-
long uniqueIdent;		//уникальный идентификатор отображения в дизайнрежиме
long SecondID;			//уникальный идентификатор отображения в режиме оператора
long Simetric;			//симметричность компонента

//определение границ
long m_hasLeftOffset;	//имеет ли отступ слева (например, если у компонента есть контактный пин)
long m_hasRightOffset;	//имеет ли отступ справа
//добавить или удалить отступы слева справа
virtual void setOffsets(bool left = false, bool right = false);

//параметры ActiveX компонента
CString caption;	//название
HWND m_mainHandle;	//хэндл контейнера
HWND m_thisHandle;  //хэндл данного ActiveXа

//внешние связи
struct SecondInfoS			//описание соседних точек
{
	HWND hwnd;				//хэндл соседней точки
	long contact;			//номер контакта соседней точки
};
CArray <SecondInfoS, SecondInfoS> SecondPointHandles;		//хэндлы соседних точек соединения

CArray <HWND, HWND> m_connectionLinkHandles;				//хэндлы соединительных линий

//временные переменные
int i,j;				//используется как счетчик
CString temp_str;		//буферная строка
long tempID;			//буферная переменная long
CRect rect;				//буферный прямоугольник
CPoint pt;				//буферная переменная - CPoint
long oldContact;

HCURSOR default_Cursor;
virtual void SetDefaultCursor(LPCWSTR curParam);

//информация о подсоединенных каналах
struct chanInfoS
	{
		CString chanName;		//имя канала
		long chanID;			//ID канала
	};
long ID_chan;				//ID текцщего канала
CString selected_channel;	//имя текущего канала
long m_nSelChanNumber;		//порядковый номер выбранного канала

//количество каналов сервера
long serverChanCount;
CArray <chanInfoS,chanInfoS> serverChanInfo;	//база данных каналов сервера
bool haveServerInterface;
virtual void SetServerInterface();				//добавить связь с сервером

//информация о контактных пинах
struct contactPinInfoS
	{
		float pointDivX;	//X координаты контакта
		float pointDivY;	//Y координаты контакта
		long side;			//сторона крепления контакта
		long connType;		//тип соединения, например, WPARAM_IDCHAN_CONNECTED или WPARAM_ID_SWITCH_CONNECTED
		long disconnType;	//тип дисконнекта, например,WPARAM_IDCHAN_DISCONNECTED
		bool connected;		//подсоединено или нет
		long number;		//номер контакта
		CString name;		//название пина
		long pinType;		//вид крепления, например - OUT_POLYSIGNAL
		bool multiConnect;  //поддержка мультисоединений
		CRect contactRect;  //линейные размеры контактной площадки
		bool isOut;			//является ли контакт - выходным
		long outValue;		//выходное значение для мессаджа ко вновь подсоединенному компоненту
	};
long savedValueToSend;		//значение для вновь подсоединенного компонента, которе ему передается при
							//коннекте, данное значение имеет свойство автосохранения и автовосстановления
CArray <contactPinInfoS,contactPinInfoS> contactPinInfoArray;	//база данных информации контактных пинов
virtual void SetOutputValueforConnect(long newValue);		//задаем значение, которое передается при коннекте
															//можно вызвать для любого контакта
virtual void SetOutputValueforConnect(long contactNumber, long newValue);

//структура описывает компонент, который может быть внедрен как в контейнер 
struct InsideActiveXS
{
	LPOLESTR clsidStr;
	CLSID clsidActiveX;
	CWnd* ActiveXcwnd;
};
InsideActiveXS InsideActiveX;
virtual void InstallActiveX(LPOLESTR clsidStr, bool enable, CString name);

long m_bInOperate;	//переменная, идентифицирующая отображение компонента (дизайн режим или операторский)

};
