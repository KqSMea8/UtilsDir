#include "StdAfx.h"
#include "MyOle.h"

BEGIN_MESSAGE_MAP(CZetBasicOle, COleControl)
	//��� ��������� �������� ���������, ������ � ���, ��� ���� ��������� ��� �����������
	ON_MESSAGE(WM_MOUSE_ONCOMPONENT,OnMouseNearComponent)

	//���������� �������� ���������
	ON_MESSAGE(WM_USER_UPDATECOMP,OnUpdateByConteiner)

	//��� ��������� �������� ���������, ������ � ���, ��� ��������� ����� ������� ���������
	ON_MESSAGE(WM_USER_WANT_PLACE_CONNECTOR,OnPlaceConnector)

	//��������� � ������������� ���������� �� ����������
	ON_MESSAGE(WM_USER_SECOND_POINT_CONNECTED,OnSecondConnect)

	//��������� �� ���������� �������
	ON_MESSAGE(WM_USER_SERVER_MODIFY,OnServerModify)

	//��������� �� ���������� �������
	ON_MESSAGE(WM_USER_PUT_SECOND_HWND,SecondPointHWND)

	//��������� ���������� ����������, ������� � ���, ����� ��� ��� �������, � ����������� �� ����, � ����� ���� �� ���������
	ON_MESSAGE(WM_USER_CHANGE_FORM,OnChangeForm)

	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONDOWN()
	ON_WM_SIZE()
	ON_WM_LBUTTONUP()
END_MESSAGE_MAP()

BEGIN_DISPATCH_MAP(CZetBasicOle, COleControl)
	//������� � ������
	DISP_FUNCTION_ID(CZetBasicOle, "PutSecondPointHWND", dispidPutSecondPointHWND, PutSecondPointHWND, VT_I4, VTS_HANDLE VTS_I4)
	DISP_FUNCTION_ID(CZetBasicOle, "SaveConnectionLinkHandle", dispidSaveConnectionLinkHandle, SaveConnectionLinkHandle, VT_I4, VTS_HANDLE)
	DISP_FUNCTION_ID(CZetBasicOle, "ChangeLocation", dispidChangeLocation, ChangeLocation, VT_I4, VTS_I4)
	DISP_FUNCTION_ID(CZetBasicOle, "ContInfo", dispidContInfo, ContInfo, VT_VARIANT, VTS_I4)
	
	//��������
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
	//������������� ��������� �������� ����������
	//���� ������� � ���, ��� �������� ����� ���� �������� � ����� ...Ctrl::DoPropExchange()
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
    
	//�������� ��������� �� ���������� ActiveX, �.�. ��� ���� ���
	InsideActiveX.ActiveXcwnd = NULL;

	caption = L"";
}

void CZetBasicOle::DoPropExchange(CPropExchange* pPX)
{
	COleControl::DoPropExchange(pPX);

	//����������� ��������
	PX_String(pPX,L"��������",caption);
	PX_Long(pPX,L"������",CompWidth);
	PX_Long(pPX,L"������",CompHeight);
	PX_Long(pPX,L"����",LeftX,0);
	PX_Long(pPX,L"����",LeftY,0);
	PX_Long(pPX,L"�������������",uniqueIdent,0);
	PX_Long(pPX,L"�����.2",SecondID,0);
	PX_Long(pPX,L"����� ������",m_hasLeftOffset,0);
	PX_Long(pPX,L"������ ������",m_hasRightOffset,0);
	PX_Long(pPX,L"��������������",Simetric,1);
	PX_Long(pPX,L"� ����",m_bInOperate,0);
	//���������� �������� ����� ��� �������� ����������
	PX_Long(pPX,L"����������� ��������",savedValueToSend,0);
	//---------------------
	//��� ������ � �������� (������������)
	PX_String(pPX,L"��� ������",selected_channel,L"� ������");		//���������� �������� ������
	PX_Long(pPX,L"���������� ID ������",ID_chan);		//���������� ID ������
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

	//���� ����� �������� ���� � �������������� �������� ����������
	this->MoveWindow(LeftX,LeftY,CompWidth,CompHeight);	

	for (i=0; i<contactPinInfoArray.GetSize();i++)
	{
		if (contactPinInfoArray[i].side == LEFT_SIDE)
			contactPinInfoArray[i].contactRect = CRect(0,
													   CompHeight*contactPinInfoArray[i].pointDivY-10,
													   10,
													   CompHeight*contactPinInfoArray[i].pointDivY+10); 
	    //���� ������� - � ������ ������, ��
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

	//���� ����� �������� ���� � �������������� �������� ����������
	this->MoveWindow(LeftX,LeftY,CompWidth,CompHeight);

	//��������� ����� �������� ����
	m_mainHandle = this->GetParent()->m_hWnd;
	//��������� ����� ������� ActiveX�
	m_thisHandle = this->m_hWnd;

	if (haveServerInterface == true)
		::PostMessage(this->GetParent()->m_hWnd,
					  WM_USER_CONNECTTOSERVER,
					  1, //�����������
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
	//��� ����������� ������� �� ������ ������� ���� ����������� � ��� ActiveX�� ��������� � ���, ��� ���������� ��� 
	for (i = 0;i < SecondPointHandles.GetSize(); i++)
	{
		if (contactPinInfoArray.GetSize()>0)
		{
			for (j = 0; j< contactPinInfoArray.GetSize(); j++)
				::PostMessage(SecondPointHandles[i].hwnd,				//����� ����, ���� ��������
							  WM_USER_SECOND_POINT_CONNECTED,			//��������� � �������� - �����������
							  WPARAM(m_thisHandle),						//���� ����� - ��� �������������
							  contactPinInfoArray[j].disconnType);		//��� ������������
		}
	}
	//���������� �������������� �����
	for (i=0;i<m_connectionLinkHandles.GetSize();i++)
		::PostMessage(m_connectionLinkHandles[i],
					  WM_USER_IS_BEING_DELETING,
					  0,
					  0);

	//��������� � ���, ��� ��������� ������ ������������� �� ������� ����������
	::PostMessage(m_mainHandle,WM_USER_CONNECTTOSERVER,0,uniqueIdent);

	COleControl::OnClose(dwSaveOption);
}

void CZetBasicOle::drawRegion(CDC* pdc, CRect rcBounds)
{
	pdc->SetBkMode(TRANSPARENT);

/*
	������ ������� ������ ������, ��������� ��� �������� ��������� �� ������� �����:
	     _____________
	____|����1	  	  |
	    |		  	  |
	____|...	 �����|____
	    |			  |
	____|����N	  	  |
	    |_____________|
	   ��������� ������� ������� �������� � onCreate �������� fillContactList,
	   � ������� ����������� ��������� ���������� ����� ������� ActiveX�

*/
	//������������� �����
	PLOGFONT plf = (PLOGFONT) LocalAlloc(LPTR, sizeof(LOGFONT)); //������������� ��������� 
	CFont pins_font;						//����� ������� �����
	plf->lfHeight = rcBounds.Width()/8;		//���������� ������ ��������
	pins_font.CreateFontIndirectW(plf);		//������� �����

	//��� ����������
	CFont out_font;
	plf->lfHeight = rcBounds.Width()/8;		//���������� ������ ��������
	plf->lfEscapement = -2700;				//���� �������
	out_font.CreateFontIndirectW(plf);
	//----------------------


	//��������� ������
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
			pdc->SelectObject(&pins_font);			//�������� ����� � ��������
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
				MessageBox(L"� ������ ActiveX ���������, "+caption+L", ������� ������ - "+name+L", ������� �� ��������� � �������, ��������, ��������� �� ���������� � �������.");
	} else 
		if (result == CO_E_CLASSSTRING)
			MessageBox(L"� ������ ActiveX ���������, "+caption+L", ������� ������ - "+name+L", � �������� ������ ���������� �������������� CLSID ������ �������.");
}

void CZetBasicOle::ChNameFromID(long ID, CString* str, long* chNumber)
{
	//�������� ������ ��� ����������
//	CString tempStr;
	long number;

	//�������� ���������� ������ �� ��� ������
	BSTR names = SysAllocString(*str);
	requestName(ID,&names,&number);
	
	*str = names;			//���������� ��� ������
	*chNumber = number;		//���������� ����� ������
}

void CZetBasicOle::fillContactList(long contNum, CString name, float contDivX, float contDivY, long side, long connType, long disconnType, long pinType, bool multiconnect, bool isOut)
{
	//���������� ��������� ������ ���������� �����, ������������, ���� ��������� � �.�.
	contactPinInfoS info;				//������� ���������� ������������ ����

	info.number = contNum;				//���������� ����� ��������, ���������� � 1
	info.name = name;
	info.pointDivX = contDivX;			//������ ����������� ������
	info.pointDivY = contDivY;			//������ ����������� ������
	info.side = side;					//������� ���������
	info.connType = connType;			//���������, �������������� ����������
	info.disconnType = disconnType;		//���������, �������������� ������������
	info.multiConnect = multiconnect;   //��������� ����������������
	info.pinType = pinType;				//��� ��������
	info.isOut = isOut;					//�������� �� ����� ��������
	info.outValue = 65535;
	info.connected = false;				//����������� ��� ���				

	if (side == LEFT_SIDE)
		info.contactRect = CRect(0,
								 CompHeight*contDivY-10,
								 10,
								 CompHeight*contDivY+10); 
    //���� ������� - � ������ ������, ��
	if (side == RIGHT_SIDE)
		info.contactRect = CRect(CompWidth*contDivX-10,
								 CompHeight*contDivY-10,
								 CompWidth*contDivX,
								 CompHeight*contDivY+10);

	//��������� ���������� �� ��������� ���� � ��
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
			//�������� ��������� ������ �������������� �����, ��������� � �����������
			::PostMessage(m_connectionLinkHandles[i],WM_USER_ONE_OF_COMPONENTS_CHANGE_DIM,0,0);
		
	return 0;
}

VARIANT CZetBasicOle::ContInfo(LONG ContNum)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	GetClientRect(rect);

	VARIANT vaResult;
	VariantInit(&vaResult);
	//VT_ARRAY ��� SAFEARRAY*
	vaResult.vt = VT_ARRAY;
	
	//������� ��������� �� SAFEARRAY
	SAFEARRAY* pSA = NULL;
	//�������
	SAFEARRAYBOUND bound[1];
	bound[0].lLbound = 0;		//����� ������� ��������
	bound[0].cElements = 3;		//���������� ���������
	pSA = SafeArrayCreate(VT_I4,1,bound);	//������� SAFEARRAY

	//���������� ����� ���������
	long PointX = contactPinInfoArray[ContNum-1].pointDivX*rect.Width();
	long PointY = contactPinInfoArray[ContNum-1].pointDivY*rect.Height();
	long Side   = contactPinInfoArray[ContNum-1].side;

	//�������� � ������ SAFEARRAY �������� ����������
	long index1 = 0;			//������ ���������� �
	long index2 = 1;			//������ ���������� Y
	long index3 = 2;			//������ �������

	SafeArrayPutElement(pSA,&index1,&PointX);	//������ ��������� � ������
	SafeArrayPutElement(pSA,&index2,&PointY);	//-.-
	SafeArrayPutElement(pSA,&index3,&Side);	    //-.-
	
	vaResult.parray = pSA;		//��������� ������������ ��������

	return vaResult;
}

LRESULT CZetBasicOle::SecondPointHWND(WPARAM wParam, LPARAM lParam)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
    SecondInfoS secondInfo;
    secondInfo.hwnd = HWND(wParam);			//��������� ����� ������ �����
    secondInfo.contact = lParam;			//��������� ���������� ����� �������� ������ ����
	//��������� ������ � ��
	SecondPointHandles.Add(secondInfo);
	
    //��������� ���� ��������������� ���������� � ���, ��� ���������� ������ �������
    //����������, ���������������, ������ ����� - ���������� ����������
	if (contactPinInfoArray.GetSize()>0)
	{
		long b = contactPinInfoArray.GetSize();
		if (lParam <= contactPinInfoArray.GetSize())
		::PostMessage(SecondPointHandles[SecondPointHandles.GetSize() - 1].hwnd,
					 WM_USER_SECOND_POINT_CONNECTED,
					 long(this->m_hWnd),
					 contactPinInfoArray[lParam-1].connType); else
			MessageBox(L"������ �� ��� ����� ����");
	}
	long b = 0;
	return 0;
}

LONG CZetBasicOle::PutSecondPointHWND(OLE_HANDLE hwnd, LONG contNum)
{

	return 0;

}

//��������
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
//������ ������ �������
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
			//���� �� �������� ����� ������, �� ����� ����� �� ���� ���������
			for (j = 0; j< SecondPointHandles.GetSize(); j++ )
				::PostMessage(SecondPointHandles[j].hwnd,
							  WM_USER_REFRASH_SOURCE,
							  long(this->m_hWnd),
							  ID_chan);
		 }
	 }
	 SetModifiedFlag();
 }


//��������

 LRESULT CZetBasicOle::OnChangeForm(WPARAM wParam, LPARAM lParam)
{
	if (lParam == 1)
	{
		//������� �������
		setOffsets(false,false);

		//��������, �� ���������� �� ��� ���� � ���������� � ���� ���������
		//���� ��, ��� ������ � ���, ��� ��������� ����������������� �� ���������
		if (m_bInOperate == 1)
		{
			setLinearSizes(CompWidth,CompHeight);
		} else
			{
				m_bInOperate = 1;


				//�������� ������ ���������� ��� ������ ����� ��� ������������
				setLinearSizes(CompWidth-PIN_LENGTH,CompHeight);

		}
		//������� ����������� ����������
		CZetBasicOle::OnSize(0,CompWidth,CompHeight);
		
		moveSize(uniqueIdent, LeftX, LeftY, CompWidth, CompHeight);
	}

	return 0;
}

 //��������� �� ���������� ���������� �� ����������
LRESULT CZetBasicOle::OnUpdateByConteiner(WPARAM wParam, LPARAM lParam)
{
	GetClientRect(rect);

	this->OnDraw(this->GetDC(),rect,rect);
	return 0;
}

//��������� �� ���������� ���������� �� ����������
LRESULT CZetBasicOle::OnServerModify(WPARAM wParam, LPARAM lParam)
{
	//wParam - ���������� ������� �������
	serverChanCount = wParam;
	serverChanInfo.RemoveAll();
	for (int i=0; i<serverChanCount; i++)
		::PostMessage(this->GetParent()->m_hWnd,WM_USER_REQUEST_CHANLIST,WPARAM(this->m_hWnd),i);

	return 0;
}

//�������� ����������� �� ����������, ��� ���� ��������� ��� ActiveX �����������
LRESULT CZetBasicOle::OnMouseNearComponent(WPARAM wParam, LPARAM lParam)
{
	//������� ����� ����� �������������� �����
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
		//� ����� ������, ��������� �������� ���������� ���������, � ���, ��� 
		//���� ��������� ��� ���������� �����
		for (i=0; i<contactPinInfoArray.GetSize(); i++)
			//���� ���� �������� � ���� ������ �� ���������, �� �������� �� ���� ����������
			if (PtInRect(contactPinInfoArray[i].contactRect,pt))
			{
				if (oldContact != contactPinInfoArray[i].number)
				{
					oldContact = contactPinInfoArray[i].number;
	
					long type;		//��� ��������� - ���� ������ ���� �������� ������ �� ���������� ���������

					if (contactPinInfoArray[i].multiConnect == false && 
						contactPinInfoArray[i].connected == true)
							//���� ���������� ��������� � ��� ��� ����������, �� ��������� ����������
							type = FORBIDDEN; else
								//����� ���������� � ���� ���������� �������� ��������
								type = contactPinInfoArray[i].pinType;

					//�������� ����������, ��� ���� ���������� ���������� �������� � ������ � ���������
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
	//wParam ������� � ���, ��� ��������� ����� ������� ��� ���������� ���������
	//lParam �������� ����� ��������
	GetCursorPos(&pt);
	ScreenToClient(&pt);

	GetClientRect(rect);

	//�������, ��� ����� ������ ��������� ���������� ���� �� ����� ������
	//��� ������� ������
	for (i = 0; i < contactPinInfoArray.GetSize(); i++)
	{
		if (contactPinInfoArray[i].number == lParam)
		{
			long type;
			//���������, �������������� �� ������������� ��� ���
			//� ���� ���, �� �� ����������� �� ��� �������
			if (contactPinInfoArray[i].multiConnect == false &&
				contactPinInfoArray[i].connected == true)
				type = FORBIDDEN; else
					type = contactPinInfoArray[i].pinType;

			long b = contactPinInfoArray[i].side;
			//�� ���������� ����� ������ 2 ���� ��������� - �������� � ���������� ���������� ����������
			switch (wParam)
			{
				// 1. ���������� ��� ���������� ����������
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

//��������� �������� �� �������������� �����, ������� � ���, ��� ����������� ������ ����� �����
LRESULT CZetBasicOle::OnSecondConnect(WPARAM wParam, LPARAM lParam)
{
	//��������� � ���, ��� ���������� ������ ������� � ��������� ������
	for (i = 0;i<SecondPointHandles.GetSize();i++)
	{
		//����������, ��� ������ �������� ��������������� ���������
		if (HWND(wParam) == SecondPointHandles[i].hwnd)
			for (j=0; j<contactPinInfoArray.GetSize(); j++)
				if (contactPinInfoArray[j].number == SecondPointHandles[i].contact)
				{
					if (lParam == contactPinInfoArray[j].connType)
					{
						contactPinInfoArray[j].connected = true;

						//���������, ���� �� ������� ����� �� �������� �� ����� ��-��������� ��� ����
						//�� ������� ���, ����� - ������� �������� �� savedValueToSend
						long output = savedValueToSend;
						if (contactPinInfoArray[j].outValue != 65535)
							output= contactPinInfoArray[j].outValue;

						//�������� ��������������� ���������� ��������
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
