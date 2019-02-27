#include "StdAfx.h"
#include "MyOle.h"

BEGIN_MESSAGE_MAP(CZetBasicOle, COleControl)
	//Это сообщение посылает контейнер, говоря о том, что мышь находится над компонентом
	ON_MESSAGE(WM_MOUSE_ONCOMPONENT,OnMouseNearComponent)

	//необходимо обновить компонент
	ON_MESSAGE(WM_USER_UPDATECOMP,OnUpdateByConteiner)

	//Это сообщение посылает контейнер, говоря о том, что контейнер хочет создать проводник
	ON_MESSAGE(WM_USER_WANT_PLACE_CONNECTOR,OnPlaceConnector)

	//сообщение о подтверждении соединения по проводнику
	ON_MESSAGE(WM_USER_SECOND_POINT_CONNECTED,OnSecondConnect)

	//сообщение об обновлении сервера
	ON_MESSAGE(WM_USER_SERVER_MODIFY,OnServerModify)

	//сообщение об обновлении сервера
	ON_MESSAGE(WM_USER_PUT_SECOND_HWND,SecondPointHWND)

	//сообщение посылается компоненту, говорит о том, какой вид ему принять, в зависимости от того, в каком окне он находится
	ON_MESSAGE(WM_USER_CHANGE_FORM,OnChangeForm)

	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONDOWN()
	ON_WM_SIZE()
	ON_WM_LBUTTONUP()
END_MESSAGE_MAP()

BEGIN_DISPATCH_MAP(CZetBasicOle, COleControl)
	//функции и методы
	DISP_FUNCTION_ID(CZetBasicOle, "PutSecondPointHWND", dispidPutSecondPointHWND, PutSecondPointHWND, VT_I4, VTS_HANDLE VTS_I4)
	DISP_FUNCTION_ID(CZetBasicOle, "SaveConnectionLinkHandle", dispidSaveConnectionLinkHandle, SaveConnectionLinkHandle, VT_I4, VTS_HANDLE)
	DISP_FUNCTION_ID(CZetBasicOle, "ChangeLocation", dispidChangeLocation, ChangeLocation, VT_I4, VTS_I4)
	DISP_FUNCTION_ID(CZetBasicOle, "ContInfo", dispidContInfo, ContInfo, VT_VARIANT, VTS_I4)
	
	//свойства
	DISP_PROPERTY_EX_ID(CZetBasicOle, "CompWidth", dispidCompWidth, GetCompWidth, SetCompWidth, VT_I4)
	DISP_PROPERTY_EX_ID(CZetBasicOle, "CompHeight", dispidCompHeight, GetCompHeight, SetCompHeight, VT_I4)
	DISP_PROPERTY_EX_ID(CZetBasicOle, "LeftX", dispidLeftX, GetLeftX, SetLeftX, VT_I4)
	DISP_PROPERTY_EX_ID(CZetBasicOle, "LeftY", dispidLeftY, GetLeftY, SetLeftY, VT_I4)
	DISP_PROPERTY_EX_ID(CZetBasicOle, "uniqueIdent", dispiduniqueIdent, GetuniqueIdent, SetuniqueIdent, VT_I4)
	DISP_PROPERTY_EX_ID(CZetBasicOle, "SecondIdent", dispidSecondIdent, GetSecondIdent, SetSecondIdent, VT_I4)
	DISP_PROPERTY_EX_ID(CZetBasicOle, "Caption", dispidCaption, GetCaption, SetCaption, VT_BSTR)
	DISP_PROPERTY_EX_ID(CZetBasicOle, "hasLeftOffset", dispidhasLeftOffset, GethasLeftOffset, SethasLeftOffset, VT_I4)
	DISP_PROPERTY_EX_ID(CZetBasicOle, "isSimetric", dispidisSimetric, GetisSimetric, SetisSimetric, VT_I4)
	DISP_PROPERTY_EX_ID(CZetBasicOle, "channelName", dispidchannelName, GetchannelName, SetchannelName, VT_BSTR)
	DISP_PROPERTY_EX_ID(CZetBasicOle, "channelID", dispidchannelID, GetchannelID, SetchannelID, VT_I4)

END_DISPATCH_MAP()


BEGIN_EVENT_MAP(CZetBasicOle, COleControl)
	EVENT_CUSTOM_ID("CreateConnector", eventidCreateConnector, CreateConnector, VTS_I4 VTS_I4 VTS_HANDLE VTS_I4 VTS_I4 VTS_I4)
	EVENT_CUSTOM_ID("MouseNearContact", eventidMouseNearContact, MouseNearContact, VTS_I4 VTS_I4 VTS_I4)
	EVENT_CUSTOM_ID("PlaceConnector", eventidPlaceConnector, PlaceConnector, VTS_I4 VTS_I4 VTS_HANDLE VTS_I4 VTS_I4 VTS_I4)
	EVENT_CUSTOM_ID("requestName", eventidrequestName, requestName, VTS_I4 VTS_PBSTR VTS_I4)
END_EVENT_MAP()

CZetBasicOle::CZetBasicOle(void)
{
	//инициализация начальных значений параметров
	//надо помнить о том, что значения могут быть изменены в блоке ...Ctrl::DoPropExchange()
	ID_chan = 0;
	CompWidth = 0;
	CompHeight = 0;
	LeftX = 0;
	LeftY = 0;
	uniqueIdent = 0;
	SecondID = 0;
	Simetric = 0;
	oldContact = 65535;
	m_bInOperate = 0;

	serverChanCount = 0;
    
	//обнуляем указатель на внедренный ActiveX, т.к. его пока нет
	InsideActiveX.ActiveXcwnd = NULL;

	caption = L"";
}

void CZetBasicOle::DoPropExchange(CPropExchange* pPX)
{
	COleControl::DoPropExchange(pPX);

	//обязательне свойства
	PX_String(pPX,L"Название",caption);
	PX_Long(pPX,L"Ширина",CompWidth);
	PX_Long(pPX,L"Высота",CompHeight);
	PX_Long(pPX,L"Лево",LeftX,0);
	PX_Long(pPX,L"Верх",LeftY,0);
	PX_Long(pPX,L"Идентификатор",uniqueIdent,0);
	PX_Long(pPX,L"Идент.2",SecondID,0);
	PX_Long(pPX,L"Левый отступ",m_hasLeftOffset,0);
	PX_Long(pPX,L"Правый отступ",m_hasRightOffset,0);
	PX_Long(pPX,L"Симметричность",Simetric,1);
	PX_Long(pPX,L"В окне",m_bInOperate,0);
	//посылается выходным пином при загрузке компонента
	PX_Long(pPX,L"Сохраненное значение",savedValueToSend,0);
	//---------------------
	//для работы с сервером (обязательные)
	PX_String(pPX,L"Имя канала",selected_channel,L"№ канала");		//сохранение названия канала
	PX_Long(pPX,L"уникальный ID канала",ID_chan);		//сохранение ID канала
}

CZetBasicOle::~CZetBasicOle(void)
{
	if (InsideActiveX.ActiveXcwnd!=NULL)
	{
		delete InsideActiveX.ActiveXcwnd;
		InsideActiveX.ActiveXcwnd = nullptr;
	}
}

void CZetBasicOle::OnMouseMove(UINT nFlags, CPoint point)
{
	MouseNearContact(0,0,uniqueIdent,0,1, 0, 0); 

	SetCursor(default_Cursor);

	COleControl::OnMouseMove(nFlags, point);
}

void CZetBasicOle::OnLButtonDown(UINT nFlags, CPoint point)
{
	MouseNearContact(0,0,uniqueIdent,0,1, 0, 3); 
	SetCursor(default_Cursor);
	COleControl::OnLButtonDown(nFlags, point);
}

void CZetBasicOle::OnSize(UINT nType, int cx, int cy)
{
	COleControl::OnSize(nType, cx, cy);

	CompWidth = cx;
	CompHeight = cy;

	//даем знать главному окну о первоначальных размерах компонента
	this->MoveWindow(LeftX,LeftY,CompWidth,CompHeight);	

	for (i=0; i<contactPinInfoArray.GetSize();i++)
	{
		if (contactPinInfoArray[i].side == LEFT_SIDE)
			contactPinInfoArray[i].contactRect = CRect(0,
													   CompHeight*contactPinInfoArray[i].pointDivY-10,
													   10,
													   CompHeight*contactPinInfoArray[i].pointDivY+10); 
	    //если контакт - с правой сторны, то
		if (contactPinInfoArray[i].side == RIGHT_SIDE)
			contactPinInfoArray[i].contactRect = CRect(CompWidth*contactPinInfoArray[i].pointDivX-10,
													   CompHeight*contactPinInfoArray[i].pointDivY-10,
													   CompWidth*contactPinInfoArray[i].pointDivX,
													   CompHeight*contactPinInfoArray[i].pointDivY+10);
	}

	if (InsideActiveX.ActiveXcwnd != NULL)
	if (cwndValidate(InsideActiveX.ActiveXcwnd) == true)
	{
		if (m_hasLeftOffset == 1 && m_hasRightOffset == 1)
			InsideActiveX.ActiveXcwnd->MoveWindow(PIN_LENGTH,0,CompWidth-2*PIN_LENGTH,CompHeight);
		else 
		if (m_hasLeftOffset == 1 && m_hasRightOffset == 0)
			InsideActiveX.ActiveXcwnd->MoveWindow(PIN_LENGTH,0,CompWidth-PIN_LENGTH,CompHeight);
		else 
		if (m_hasLeftOffset == 0 && m_hasRightOffset == 1)
			InsideActiveX.ActiveXcwnd->MoveWindow(0,0,CompWidth-PIN_LENGTH,CompHeight);
		else 
		if (m_hasLeftOffset == 0 && m_hasRightOffset == 0)
			InsideActiveX.ActiveXcwnd->MoveWindow(0,0,CompWidth,CompHeight);
	}
	
}

int CZetBasicOle::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (COleControl::OnCreate(lpCreateStruct) == -1)
		return -1;

	//даем знать главному окну о первоначальных размерах компонента
	this->MoveWindow(LeftX,LeftY,CompWidth,CompHeight);

	//сохраняем хэндл главного окна
	m_mainHandle = this->GetParent()->m_hWnd;
	//сохраняем хэндл данного ActiveXа
	m_thisHandle = this->m_hWnd;

	if (haveServerInterface == true)
		::PostMessage(this->GetParent()->m_hWnd,
					  WM_USER_CONNECTTOSERVER,
					  1, //подсоединен
					  LPARAM(this->m_hWnd));
	return 0;
}

void CZetBasicOle::SetDefaultCursor(LPCWSTR curParam)
{
	default_Cursor = LoadCursor(NULL,curParam);
}

void CZetBasicOle::setOffsets(bool left, bool right)
{
	left == true?m_hasLeftOffset=1:m_hasLeftOffset=0;
	right == true?m_hasRightOffset=1:m_hasRightOffset=0;
}

void CZetBasicOle::SetServerInterface()
{
	haveServerInterface = true;
}

void CZetBasicOle::SetOutputValueforConnect(long newValue)
{
	savedValueToSend = newValue;
}
void CZetBasicOle::SetOutputValueforConnect(long contactNumber, long newValue)
{
	for (i=0; i<contactPinInfoArray.GetSize(); i++)
		if (contactPinInfoArray[i].number == contactNumber)
			contactPinInfoArray[i].outValue = newValue;
}

void CZetBasicOle::testMessage(float par)
{
	CString str;
	str.Format(L"%f",par);
	MessageBox(str);
}

void CZetBasicOle::OnClose(DWORD dwSaveOption)
{
	//при уничтожении объекта он должен послать всем соединенным с ним ActiveXам сообщение о том, что соединения нет 
	for (i = 0;i < SecondPointHandles.GetSize(); i++)
	{
		if (contactPinInfoArray.GetSize()>0)
		{
			for (j = 0; j< contactPinInfoArray.GetSize(); j++)
				::PostMessage(SecondPointHandles[i].hwnd,				//хэндл того, кому посылаем
							  WM_USER_SECOND_POINT_CONNECTED,			//сообщение о коннекте - дисконнекте
							  WPARAM(m_thisHandle),						//свой хэндл - как идентификатор
							  contactPinInfoArray[j].disconnType);		//тип отсоединения
		}
	}
	//посылается соединительной линии
	for (i=0;i<m_connectionLinkHandles.GetSize();i++)
		::PostMessage(m_connectionLinkHandles[i],
					  WM_USER_IS_BEING_DELETING,
					  0,
					  0);

	//сообщение о том, что компонент должен отсоединиться от сервера контейнера
	::PostMessage(m_mainHandle,WM_USER_CONNECTTOSERVER,0,uniqueIdent);

	COleControl::OnClose(dwSaveOption);
}

void CZetBasicOle::drawRegion(CDC* pdc, CRect rcBounds)
{
	pdc->SetBkMode(TRANSPARENT);

/*
	данныя функция рисует регион, примерный вид которого изображен на рисунке снизу:
	     _____________
	____|вход1	  	  |
	    |		  	  |
	____|...	 выход|____
	    |			  |
	____|входN	  	  |
	    |_____________|
	   параметры данного региона задаются в onCreate функцией fillContactList,
	   в которой описываются параметры контактных пинов данного ActiveXа

*/
	//идентификация пинов
	PLOGFONT plf = (PLOGFONT) LocalAlloc(LPTR, sizeof(LOGFONT)); //инициализация структуры 
	CFont pins_font;						//шрифт подписи пинов
	plf->lfHeight = rcBounds.Width()/8;		//определяем высоту символов
	pins_font.CreateFontIndirectW(plf);		//создаем шрифт

	//пин Индикатора
	CFont out_font;
	plf->lfHeight = rcBounds.Width()/8;		//определяем высоту символов
	plf->lfEscapement = -2700;				//угол наклона
	out_font.CreateFontIndirectW(plf);
	//----------------------


	//формируем регион
	CRgn boundRagion;
	boundRagion.CreateRectRgn(0,0,rcBounds.Width(),rcBounds.Height());
	
	CRgn leftRgn;
	leftRgn.CreateRectRgn(0,0,PIN_LENGTH,rcBounds.Height());

	CRgn rightRgn;
	rightRgn.CreateRectRgn(rcBounds.Width()-PIN_LENGTH,0,rcBounds.Width(),rcBounds.Height());

	if (m_hasLeftOffset == 1)
		boundRagion.CombineRgn(&boundRagion,&leftRgn,RGN_XOR);
	if (m_hasRightOffset == 1)
	boundRagion.CombineRgn(&boundRagion,&rightRgn,RGN_XOR);

	CRgn pinRgn;
	for (i=0;i<contactPinInfoArray.GetSize();i++)
	{
		if (m_hasRightOffset == 1)
		if (contactPinInfoArray[i].side == RIGHT_SIDE)
		{
			pinRgn.CreateRectRgn(contactPinInfoArray[i].pointDivX*rect.Width()-PIN_LENGTH,
								 contactPinInfoArray[i].pointDivY*rect.Height()-1,
								 contactPinInfoArray[i].pointDivX*rect.Width(),
								 contactPinInfoArray[i].pointDivY*rect.Height()+1); 
			pdc->SelectObject(&out_font);
			float fSize = plf->lfHeight;
			pdc->TextOutW(rcBounds.Width()-PIN_LENGTH-plf->lfHeight,
						  rcBounds.Height()*contactPinInfoArray[i].pointDivY+plf->lfHeight*1.5 + contactPinInfoArray[i].name.GetLength()*fSize/9,
						  contactPinInfoArray[i].name);

		}
		if (m_hasLeftOffset == 1)
		if (contactPinInfoArray[i].side == LEFT_SIDE)
		{
			pinRgn.CreateRectRgn(0,
								 contactPinInfoArray[i].pointDivY*rect.Height()-1,
								 PIN_LENGTH,
								 contactPinInfoArray[i].pointDivY*rect.Height()+1);
			pdc->SelectObject(&pins_font);			//выбираем шрифт в контекст
			pdc->TextOutW(PIN_LENGTH,
						  rcBounds.Height()*contactPinInfoArray[i].pointDivY-plf->lfHeight/2,
						  contactPinInfoArray[i].name);
		}
		boundRagion.CombineRgn(&boundRagion,&pinRgn,RGN_OR);

		pinRgn.DeleteObject();
	}
	
	CBrush blackBr(RGB(0,0,0));
	if (m_hasLeftOffset == true)
		pdc->FillRect(CRect(0,0,PIN_LENGTH,rcBounds.Height()),&blackBr);
	if (m_hasRightOffset == true)
		pdc->FillRect(CRect(rcBounds.Width()-PIN_LENGTH,0,rcBounds.Width(),rcBounds.Height()),&blackBr);
	//---------------------
	this->SetWindowRgn(boundRagion,true);

	DeleteObject(pins_font);
	DeleteObject(out_font);

}

void CZetBasicOle::InstallActiveX(LPOLESTR clsidStr, bool enable, CString name)
{
	long result = CLSIDFromString(clsidStr,&InsideActiveX.clsidActiveX);
	if (result == NOERROR)
	{
		InsideActiveX.ActiveXcwnd = new(CWnd);
		
		if (InsideActiveX.ActiveXcwnd->CreateControl(InsideActiveX.clsidActiveX,L"",WS_VISIBLE,CRect(0,0,CompWidth-PIN_LENGTH,CompHeight),this,1000))
			InsideActiveX.ActiveXcwnd->EnableWindow(enable); else
				MessageBox(L"В данный ActiveX компонент, "+caption+L", внедрен объект - "+name+L", который не обнаружен в реестре, возможно, компонент не установлен в системе.");
	} else 
		if (result == CO_E_CLASSSTRING)
			MessageBox(L"В данный ActiveX компонент, "+caption+L", внедрен объект - "+name+L", у которого строка классового идентификатора CLSID задана неверно.");
}

void CZetBasicOle::ChNameFromID(long ID, CString* str, long* chNumber)
{
	//выделяем память под переменные
//	CString tempStr;
	long number;

	//посылаем контейнеру запрос на имя канала
	BSTR names = SysAllocString(*str);
	requestName(ID,&names,&number);
	
	*str = names;			//возвращаем имя канала
	*chNumber = number;		//возвращаем номер канала
}

void CZetBasicOle::fillContactList(long contNum, CString name, float contDivX, float contDivY, long side, long connType, long disconnType, long pinType, bool multiconnect, bool isOut)
{
	//необходимо набросать шаблон контактных пинов, расположения, типа крепления и т.д.
	contactPinInfoS info;				//создаем переменную структурного типа

	info.number = contNum;				//порядковый номер контакта, нумеровать с 1
	info.name = name;
	info.pointDivX = contDivX;			//задаем соотношение ширины
	info.pointDivY = contDivY;			//задаем соотношение высоты
	info.side = side;					//сторона крепления
	info.connType = connType;			//сообщение, подтверждающее соединение
	info.disconnType = disconnType;		//сообщение, подтверждающее разъединение
	info.multiConnect = multiconnect;   //поддержка мультисоединения
	info.pinType = pinType;				//тип контакта
	info.isOut = isOut;					//является ли канал выходным
	info.outValue = 65535;
	info.connected = false;				//подсоединен или нет				

	if (side == LEFT_SIDE)
		info.contactRect = CRect(0,
								 CompHeight*contDivY-10,
								 10,
								 CompHeight*contDivY+10); 
    //если контакт - с правой сторны, то
	if (side == RIGHT_SIDE)
		info.contactRect = CRect(CompWidth*contDivX-10,
								 CompHeight*contDivY-10,
								 CompWidth*contDivX,
								 CompHeight*contDivY+10);

	//добавляем информацию об описанном пине в БД
	contactPinInfoArray.Add(info);
}

void CZetBasicOle::setLinearSizes(long cx, long cy)
{
	CompWidth = cx;
	CompHeight = cy;

	this->MoveWindow(LeftX,LeftY,CompWidth,CompHeight);

}

LONG CZetBasicOle::SaveConnectionLinkHandle(OLE_HANDLE ConnLinkHandle)
   {
	   AFX_MANAGE_STATE(AfxGetStaticModuleState());

	   m_connectionLinkHandles.Add(HWND(ConnLinkHandle));

	   return 0;
   }

bool CZetBasicOle::cwndValidate(CWnd* cwnd)
{
	bool result = false;
	if (cwnd!=NULL)
	{
		IUnknown* iUnkn = NULL;
		iUnkn = cwnd->GetControlUnknown();
		if (iUnkn != NULL)
			result = true; else 
				result = false;
	} else
		result = false;

	return result;
}

LONG CZetBasicOle::ChangeLocation(LONG ind)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	if (m_connectionLinkHandles.GetSize() > 0)
		for (i = 0; i<= m_connectionLinkHandles.GetSize()-1; i++)
			//посылаем сообщения каждой соединительной линии, связанной с компонентом
			::PostMessage(m_connectionLinkHandles[i],WM_USER_ONE_OF_COMPONENTS_CHANGE_DIM,0,0);
		
	return 0;
}

VARIANT CZetBasicOle::ContInfo(LONG ContNum)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	GetClientRect(rect);

	VARIANT vaResult;
	VariantInit(&vaResult);
	//VT_ARRAY это SAFEARRAY*
	vaResult.vt = VT_ARRAY;
	
	//создаем указатель на SAFEARRAY
	SAFEARRAY* pSA = NULL;
	//границы
	SAFEARRAYBOUND bound[1];
	bound[0].lLbound = 0;		//номер нижнего элемента
	bound[0].cElements = 3;		//количество элементов
	pSA = SafeArrayCreate(VT_I4,1,bound);	//создаем SAFEARRAY

	//координаты точки креплений
	long PointX = contactPinInfoArray[ContNum-1].pointDivX*rect.Width();
	long PointY = contactPinInfoArray[ContNum-1].pointDivY*rect.Height();
	long Side   = contactPinInfoArray[ContNum-1].side;

	//загоняем в массив SAFEARRAY значение координаты
	long index1 = 0;			//индекс координаты Х
	long index2 = 1;			//индекс координаты Y
	long index3 = 2;			//индекс стороны

	SafeArrayPutElement(pSA,&index1,&PointX);	//запись элементов в массив
	SafeArrayPutElement(pSA,&index2,&PointY);	//-.-
	SafeArrayPutElement(pSA,&index3,&Side);	    //-.-
	
	vaResult.parray = pSA;		//формируем возвращиемое значение

	return vaResult;
}

LRESULT CZetBasicOle::SecondPointHWND(WPARAM wParam, LPARAM lParam)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
    SecondInfoS secondInfo;
    secondInfo.hwnd = HWND(wParam);			//сохраняем хэндл второй точки
    secondInfo.contact = lParam;			//сохраняем порядковый номер контакта самого себя
	//добавляем запись в БД
	SecondPointHandles.Add(secondInfo);
	
    //сообщение ныне подсоединенному компоненту о том, что соединение прошло успешно
    //посылается, непосредственно, второй точке - участкнику соединения
	if (contactPinInfoArray.GetSize()>0)
	{
		long b = contactPinInfoArray.GetSize();
		if (lParam <= contactPinInfoArray.GetSize())
		::PostMessage(SecondPointHandles[SecondPointHandles.GetSize() - 1].hwnd,
					 WM_USER_SECOND_POINT_CONNECTED,
					 long(this->m_hWnd),
					 contactPinInfoArray[lParam-1].connType); else
			MessageBox(L"Указан не тот номер пина");
	}
	long b = 0;
	return 0;
}

LONG CZetBasicOle::PutSecondPointHWND(OLE_HANDLE hwnd, LONG contNum)
{

	return 0;

}

//свойства
LONG CZetBasicOle::GetCompWidth(void)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	return CompWidth;
}
void CZetBasicOle::SetCompWidth(LONG newVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	CompWidth = newVal;

	SetModifiedFlag();
}
LONG CZetBasicOle::GetCompHeight(void)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	return CompHeight;
}
void CZetBasicOle::SetCompHeight(LONG newVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	CompHeight = newVal;

	SetModifiedFlag();
}
LONG CZetBasicOle::GetLeftX(void)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	return LeftX;
}
void CZetBasicOle::SetLeftX(LONG newVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	LeftX = newVal;

	SetModifiedFlag();
}
LONG CZetBasicOle::GetLeftY(void)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	return LeftY;
}
void CZetBasicOle::SetLeftY(LONG newVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	LeftY = newVal;

	SetModifiedFlag();
}

LONG CZetBasicOle::GetuniqueIdent(void)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	return uniqueIdent;
}
void CZetBasicOle::SetuniqueIdent(LONG newVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	uniqueIdent = newVal;

	SetModifiedFlag();
}
LONG CZetBasicOle::GetSecondIdent(void)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	return SecondID;
}
void CZetBasicOle::SetSecondIdent(LONG newVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	SecondID = newVal;

	SetModifiedFlag();
}
BSTR CZetBasicOle::GetCaption(void)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	CString strResult = caption;

	return strResult.AllocSysString();
}
void CZetBasicOle::SetCaption(LPCTSTR newVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	caption = newVal;

	SetModifiedFlag();
}
LONG CZetBasicOle::GethasLeftOffset(void)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	return m_hasLeftOffset;
}
void CZetBasicOle::SethasLeftOffset(LONG newVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	SetModifiedFlag();
}
LONG CZetBasicOle::GetisSimetric(void)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	return Simetric;
}
void CZetBasicOle::SetisSimetric(LONG newVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	SetModifiedFlag();
}
LONG CZetBasicOle::GetserverChanID(void)
 {
	 AFX_MANAGE_STATE(AfxGetStaticModuleState());

	 return 0;
 }
void CZetBasicOle::SetserverChanID(LONG newVal)
 {
	 AFX_MANAGE_STATE(AfxGetStaticModuleState());
	 SetModifiedFlag();
 }
//запрос списка каналов
 BSTR CZetBasicOle::GetchannelName(void)
 {
	 AFX_MANAGE_STATE(AfxGetStaticModuleState());
	 CString strResult;
	 return strResult.AllocSysString();
 }
 void CZetBasicOle::SetchannelName(LPCTSTR newVal)
 {
	 AFX_MANAGE_STATE(AfxGetStaticModuleState());
	 chanInfoS info;
	 info.chanName = newVal;
	 serverChanInfo.Add(info);
	 
	 SetModifiedFlag();
 }
  LONG CZetBasicOle::GetchannelID(void)
 {
	 AFX_MANAGE_STATE(AfxGetStaticModuleState());
	 return 0;
 }
 void CZetBasicOle::SetchannelID(LONG newVal)
 {
	 AFX_MANAGE_STATE(AfxGetStaticModuleState());
	 serverChanInfo[serverChanInfo.GetSize()-1].chanID = newVal;
	 if (ID_chan == newVal)
	 {
		 m_nSelChanNumber = serverChanInfo.GetSize()-1;
		 if (selected_channel != serverChanInfo[serverChanInfo.GetSize()-1].chanName)
		 {
			selected_channel = serverChanInfo[serverChanInfo.GetSize()-1].chanName;
			InvalidateControl();
			//если мы изменили номер канала, то дадим знать об этом приемнику
			for (j = 0; j< SecondPointHandles.GetSize(); j++ )
				::PostMessage(SecondPointHandles[j].hwnd,
							  WM_USER_REFRASH_SOURCE,
							  long(this->m_hWnd),
							  ID_chan);
		 }
	 }
	 SetModifiedFlag();
 }


//мессаджы

 LRESULT CZetBasicOle::OnChangeForm(WPARAM wParam, LPARAM lParam)
{
	if (lParam == 1)
	{
		//убираем отступы
		setOffsets(false,false);

		//проверим, не установлен ли уже флаг о нахождении в окне оперетора
		//если да, это говорт о том, что компонент восстанавливается из хранилища
		if (m_bInOperate == 1)
		{
			setLinearSizes(CompWidth,CompHeight);
		} else
			{
				m_bInOperate = 1;


				//подгоням размер компонента под дизайн режим или операторский
				setLinearSizes(CompWidth-PIN_LENGTH,CompHeight);

		}
		//обновим отображение компонента
		CZetBasicOle::OnSize(0,CompWidth,CompHeight);
		
		moveSize(uniqueIdent, LeftX, LeftY, CompWidth, CompHeight);
	}

	return 0;
}

 //сообщение об обновлении компонента из контейнера
LRESULT CZetBasicOle::OnUpdateByConteiner(WPARAM wParam, LPARAM lParam)
{
	GetClientRect(rect);

	this->OnDraw(this->GetDC(),rect,rect);
	return 0;
}

//сообщение об обновлении компонента из контейнера
LRESULT CZetBasicOle::OnServerModify(WPARAM wParam, LPARAM lParam)
{
	//wParam - количество каналов сервера
	serverChanCount = wParam;
	serverChanInfo.RemoveAll();
	for (int i=0; i<serverChanCount; i++)
		::PostMessage(this->GetParent()->m_hWnd,WM_USER_REQUEST_CHANLIST,WPARAM(this->m_hWnd),i);

	return 0;
}

//получаем уведомление от контейнера, что мышь находится над ActiveX компонентом
LRESULT CZetBasicOle::OnMouseNearComponent(WPARAM wParam, LPARAM lParam)
{
	//команды смена цвета соединительной линии
	if (lParam == 2)
		for (i=0;i<m_connectionLinkHandles.GetSize();i++)
			::PostMessage(m_connectionLinkHandles[i],WM_COMPONENT_CHANGELINECOLOR,0,lParam);
	 else
		if (lParam == 3)
			for (i=0;i<m_connectionLinkHandles.GetSize();i++)
				::PostMessage(m_connectionLinkHandles[i],WM_COMPONENT_CHANGELINECOLOR,0,0);
	//----------------------------------------

	GetCursorPos(&pt);
	ScreenToClient(&pt);

	GetClientRect(rect);
	
	if (contactPinInfoArray.GetSize()>0)
	{
		//в ответ запрос, компонент посылает контейнеру сообщение, о том, что 
		//мышь находится над контактным пином
		for (i=0; i<contactPinInfoArray.GetSize(); i++)
			//если мышь попадает в зону одного из контактов, то сообщить об этом контейнеру
			if (PtInRect(contactPinInfoArray[i].contactRect,pt))
			{
				if (oldContact != contactPinInfoArray[i].number)
				{
					oldContact = contactPinInfoArray[i].number;
	
					long type;		//тип крепления - либо запрет либо значение одного из допустимых состояний

					if (contactPinInfoArray[i].multiConnect == false && 
						contactPinInfoArray[i].connected == true)
							//если соединение одинарное и оно уже подключено, то запретить соединение
							type = FORBIDDEN; else
								//иначе приравнять к типу соединения текущего контакта
								type = contactPinInfoArray[i].pinType;

					//сообщаем контейнеру, что надо подсветить контактную площадку и задаем её параметры
					MouseNearContact(rect.Width()*contactPinInfoArray[i].pointDivX, 
									 rect.Height()*contactPinInfoArray[i].pointDivY, 
									 uniqueIdent, 
									 contactPinInfoArray[i].number, 
									 0, 
									 type, 
									 contactPinInfoArray[i].side);
					afxDump<<contactPinInfoArray[i].number<< " - number\n";
				}
			}
		for (i = 0; i<contactPinInfoArray.GetSize();i++)
		{
			if (contactPinInfoArray[i].number == oldContact)
				if (!PtInRect(contactPinInfoArray[i].contactRect,pt))
				{
					MouseNearContact(0,0,uniqueIdent,0,1, 0, 4); 
					oldContact = 65535;
				}
		}
	}
	return 0;
}

LRESULT CZetBasicOle::OnPlaceConnector(WPARAM wParam, LPARAM lParam)
{
	//wParam говорит о том, что контейнер хочет создать или разместить проводник
	//lParam передает номер контакта
	GetCursorPos(&pt);
	ScreenToClient(&pt);

	GetClientRect(rect);

	//смотрим, над камим именно контактом находилась мышь во время щелчка
	//над выбором канала
	for (i = 0; i < contactPinInfoArray.GetSize(); i++)
	{
		if (contactPinInfoArray[i].number == lParam)
		{
			long type;
			//проверяем, поддерживается ли мультиконнект или нет
			//и если нет, то не подсоединен ли еще контакт
			if (contactPinInfoArray[i].multiConnect == false &&
				contactPinInfoArray[i].connected == true)
				type = FORBIDDEN; else
					type = contactPinInfoArray[i].pinType;

			long b = contactPinInfoArray[i].side;
			//от контейнера могут прийти 2 типа сообщений - создание и размещение созданного проводника
			switch (wParam)
			{
				// 1. размещение уже созданного проводника
				case WANT_PLACE:
						PlaceConnector(rect.Width()*contactPinInfoArray[i].pointDivX, 
									   rect.Height()*contactPinInfoArray[i].pointDivY, 
									   OLE_HANDLE(this->m_hWnd),
									   uniqueIdent, 
									   type,
									   contactPinInfoArray[i].side, 
									   contactPinInfoArray[i].number);
						break;

				case WANT_CREATE:
						CreateConnector(rect.Width()*contactPinInfoArray[i].pointDivX, 
									   rect.Height()*contactPinInfoArray[i].pointDivY, 
									   OLE_HANDLE(this->m_hWnd),
									   uniqueIdent, 
									   type,
									   contactPinInfoArray[i].side, 
									   contactPinInfoArray[i].number);
						break;

				default: break;
			}
		}
	}
	
	return 0;
}

//Сообщение приходит от соединительной линии, говорит о том, что подсоединен второй конец линии
LRESULT CZetBasicOle::OnSecondConnect(WPARAM wParam, LPARAM lParam)
{
	//Сообщение о том, что соединение прошло успешно и проводник создан
	for (i = 0;i<SecondPointHandles.GetSize();i++)
	{
		//определяем, для какого контакта предназначается сообщение
		if (HWND(wParam) == SecondPointHandles[i].hwnd)
			for (j=0; j<contactPinInfoArray.GetSize(); j++)
				if (contactPinInfoArray[j].number == SecondPointHandles[i].contact)
				{
					if (lParam == contactPinInfoArray[j].connType)
					{
						contactPinInfoArray[j].connected = true;

						//проверяем, если мы задвали какое то значение на выход по-умолчанию для пина
						//то зададим его, иначе - зададим значение из savedValueToSend
						long output = savedValueToSend;
						if (contactPinInfoArray[j].outValue != 65535)
							output= contactPinInfoArray[j].outValue;

						//посылаем подсоединенному компоненту параметр
						if (contactPinInfoArray[j].isOut == true)
							::PostMessage(HWND(wParam),
										  WM_USER_REFRASH_SOURCE,
										  WPARAM(this->m_hWnd),
										  output);

						InvalidateControl();
					}
					if (lParam == contactPinInfoArray[j].disconnType)
					{
						contactPinInfoArray[j].connected = false;
						InvalidateControl();
					}
				}
	}
	return 0;
}

void CZetBasicOle::OnLButtonUp(UINT nFlags, CPoint point)
{
	SetCursor(default_Cursor);
	COleControl::OnLButtonUp(nFlags, point);
}
