#include "StdAfx.h"
#include "ZetBasic.h"
#include <afxmt.h>
#include <Dialog_ZET/ConnectToZetPath.h>

// � 4 ����������� ������ ������� �� �������� (����� ����������� SeismoDetector, SeismoRegion, SeismoStation � ZETPSH3610)
// #ifndef NOT_CREATE_SCADA_COMPONENTS_DUMP
// 	#include <ZetViewLeakDetector/zetViewLeakDetector.h>
// #endif

//using namespace BasicClass;
long globalRunMode;

GdiplusStartupInput gdiplusStartupInput; 
ULONG_PTR gdiplusToken;

// *******************************************************
// �������� ������������������ ������ ����������� � ����
// ��������������� ������� � ����� ZetViewLeakDetector/zetViewLeakDetector.h
volatile LONG gCurrComponentHeapBl = 0;
volatile LONG gCurrComponentHeapSz = 0;
// *******************************************************

// ����� ���������� (������ ���������� ���������� ��� ��������� ����������� ��������� ������ ����)
uint32_t gComponentNumber = 0;

//CArray <classInformationS,classInformationS> classInformation;
std::vector<classInformationS> classInformation;

//������ ������������� ��� ������, ��� ���� ��� �� ������ ��� �� ������������� ��������
BEGIN_MESSAGE_MAP(CZetBasicOle, COleControl)
	//��� ��������� �������� ���������, ������ � ���, ��� ���� ��������� ��� �����������
//	ON_MESSAGE(WM_MOUSE_ONCOMPONENT,OnMouseNearComponent)

	//���������� �������� ���������
	ON_MESSAGE(WM_USER_UPDATECOMP, OnUpdateByConteiner)

	//��� ��������� �������� ���������, ������ � ���, ��� ��������� ����� ������� ���������
	//ON_MESSAGE(WM_USER_WANT_PLACE_CONNECTOR,OnPlaceConnector)

	//��� ��������� �������� ���������, ������ � ���, ��� ��������� ����� ������� ���������
	//ON_MESSAGE(WM_USER_WANT_PLACE_CONNECTORDIRECT,OnPlaceConnectorDirect)

	//��������� � ������������� ���������� �� ����������
	//ON_MESSAGE(WM_USER_SECOND_POINT_CONNECTED,OnSecondConnect)

	//��������� �� �������������� ����� �� ������������ �����
	ON_MESSAGE(WM_USER_SECOND_POINT_DISCONNECTED,OnConnLineDisconnect)

	//��������� �� ���������� �������
	//ON_MESSAGE(WM_USER_SERVER_MODIFY,OnServerModify)

	//��������� ���������� ����������, ������� � ���, ����� ��� ��� �������, � ����������� �� ����, � ����� ���� �� ���������
	//ON_MESSAGE(WM_USER_CHANGE_FORM,OnChangeForm)

	//���������� ��� ������� ���� ��������
	ON_MESSAGE(WM_USER_START_ALL,OnStartAll)

	//������ ������� ��� �������� � ����� �����
	ON_MESSAGE(WM_USER_DEBUG_MODE,OnDebugMode)

	//������� � ����� �������������� ������� �������� ����������
	//ON_MESSAGE(WM_USER_QUEUE_ALLOW,OnQueueAllow)
	
	//ZETView �������� ���� ����
	ON_MESSAGE(WM_USER_ZETVIEWUPDATEBACKCOLOR, OnUpdateBackColor)

	ON_MESSAGE(WM_USER_MINMAX, OnMinMax)
	//ZETView �������� ���� ����
	//ON_MESSAGE(WM_USER_REDRAW_REGION, OnUpdateRedrawRegion)
	
//	ON_WM_MOUSEMOVE()
//	ON_WM_LBUTTONDOWN()
	ON_WM_SIZE()
	ON_WM_SHOWWINDOW()
//	ON_WM_LBUTTONUP()
	ON_WM_TIMER()
	ON_WM_WINDOWPOSCHANGING()
END_MESSAGE_MAP()

BEGIN_DISPATCH_MAP(CZetBasicOle, COleControl)
	
	//������� � ������
	DISP_FUNCTION_ID(CZetBasicOle, "GetAllocHeapBl", dispidGetAllocHeapBl, GetAllocHeapBl, VT_I4, VTS_NONE)
	DISP_FUNCTION_ID(CZetBasicOle, "GetAllocHeapSz", dispidGetAllocHeapSz, GetAllocHeapSz, VT_I4, VTS_NONE)

	DISP_FUNCTION_ID(CZetBasicOle, "ResetMemoryLeak", dispidResetMemoryLeak, ResetMemoryLeak, VT_I4, VTS_NONE)
	DISP_FUNCTION_ID(CZetBasicOle, "GetMemoryLeak", dispidGetMemoryLeak, GetMemoryLeak, VT_I4, VTS_NONE)
//	DISP_FUNCTION_ID(CZetBasicOle, "SaveConnectionLinkHandle", dispidSaveConnectionLinkHandle, SaveConnectionLinkHandle, VT_I4, VTS_HANDLE)
//	DISP_FUNCTION_ID(CZetBasicOle, "ChangeLocation", dispidChangeLocation, ChangeLocation, VT_I4, VTS_I4)
//	DISP_FUNCTION_ID(CZetBasicOle, "ContInfo", dispidContInfo, ContInfo, VT_VARIANT, VTS_I4)
//	DISP_FUNCTION_ID(CZetBasicOle, "readConnLinkHandles", dispidreadConnLinkHandles, readConnLinkHandles, VT_VARIANT, VTS_I4)
	DISP_FUNCTION_ID(CZetBasicOle, "GetMethodsDisp", dispidGetMethodsDisp, GetMethodsDisp, VT_I4, VTS_NONE)
	
	DISP_FUNCTION_ID(CZetBasicOle, "RefreshData", dispidRefreshData, RefreshData, VT_I4, VTS_HANDLE VTS_I4 VTS_BSTR VTS_I4 VTS_I4)
	DISP_FUNCTION_ID(CZetBasicOle, "RefreshData2", dispidRefreshData2, RefreshData2, VT_I4, VTS_HANDLE VTS_I4 VTS_BSTR VTS_I4 VTS_I4 VTS_I4)
	DISP_FUNCTION_ID(CZetBasicOle, "RefreshData3", dispidRefreshData3, RefreshData3, VT_I4, VTS_HANDLE VTS_I4 VTS_BSTR VTS_I8 VTS_I4 VTS_I4)
	DISP_FUNCTION_ID(CZetBasicOle, "RefreshData5", dispidRefreshData5, RefreshData5, VT_I4, VTS_HANDLE VTS_I4 VTS_BSTR VTS_R8 VTS_I4)
	DISP_FUNCTION_ID(CZetBasicOle, "RefreshDataU", dispidRefreshDataU, RefreshDataU, VT_I4, VTS_HANDLE VTS_I4 VTS_BSTR VTS_PI4)

	DISP_FUNCTION_ID(CZetBasicOle, "PostRunFromScada", dospidGetPostRunFromScada, OnPostRunFromScada, VT_EMPTY, VTS_I4)
	DISP_FUNCTION_ID(CZetBasicOle, "GetUncertainData", dispidGetUncertainData, OnGetUncertainData, VT_I4, VTS_I4 VTS_I4 VTS_I4)
	DISP_FUNCTION_ID(CZetBasicOle, "DrawInBuffer", dispidDrawInBuffer, OnDrawInBuffer, VT_EMPTY, VTS_I4)
	
	DISP_FUNCTION_ID(CZetBasicOle, "RefreshDataArray1", dispidRefreshDataArray1, RefreshDataArray1, VT_I4, VTS_I4 VTS_VARIANT VTS_VARIANT)
	DISP_FUNCTION_ID(CZetBasicOle, "connectionInfo", dispidconnectionInfo, connectionInfo, VT_I4, VTS_HANDLE VTS_I4 VTS_HANDLE VTS_I4)
	DISP_FUNCTION_ID(CZetBasicOle, "ContactCaption", dispidContactCaption, ContactCaption, VT_BSTR, VTS_I4)
	DISP_FUNCTION_ID(CZetBasicOle, "fillChannels", dispidfillChannels, fillChannels, VT_I4, VTS_I4 VTS_BSTR VTS_BSTR VTS_R4 VTS_R4 VTS_I4)
	DISP_FUNCTION_ID(CZetBasicOle, "removeAllChannels", dispidremoveAllChannels, removeAllChannels, VT_I4, VTS_NONE)
	DISP_FUNCTION_ID(CZetBasicOle, "applyChannels", dispidapplyChannels, applyChannels, VT_EMPTY, VTS_NONE)
	DISP_FUNCTION_ID(CZetBasicOle, "fillDevices", dispidfillDevices, fillDevices, VT_I4, VTS_I4 VTS_I4 VTS_I4 VTS_BSTR)
	DISP_FUNCTION_ID(CZetBasicOle, "OnChangeGenParam", dispidOnChangeGenParam, OnChangeGenParam, VT_I4, VTS_I4 VTS_R4 VTS_BSTR)
	
	//��������
	DISP_PROPERTY_EX_ID(CZetBasicOle, "CompWidth", dispidCompWidth, GetCompWidth, SetCompWidth, VT_I4)
	DISP_PROPERTY_EX_ID(CZetBasicOle, "CompHeight", dispidCompHeight, GetCompHeight, SetCompHeight, VT_I4)
	DISP_PROPERTY_EX_ID(CZetBasicOle, "LeftX", dispidLeftX, GetLeftX, SetLeftX, VT_I4)
	DISP_PROPERTY_EX_ID(CZetBasicOle, "LeftY", dispidLeftY, GetLeftY, SetLeftY, VT_I4)
	DISP_PROPERTY_EX_ID(CZetBasicOle, "uniqueIdent", dispiduniqueIdent, GetuniqueIdent, SetuniqueIdent, VT_I4)
	DISP_PROPERTY_EX_ID(CZetBasicOle, "SecondIdent", dispidSecondIdent, GetSecondIdent, SetSecondIdent, VT_I4)
	DISP_PROPERTY_EX_ID(CZetBasicOle, "Caption", dispidCaption, GetCaption, SetCaption, VT_BSTR)
	DISP_PROPERTY_EX_ID(CZetBasicOle, "sHelpString", dispidsHelpString, GetsHelpString, SetsHelpString, VT_BSTR)
	DISP_PROPERTY_EX_ID(CZetBasicOle, "hasLeftOffset", dispidhasLeftOffset, GethasLeftOffset, SethasLeftOffset, VT_I4)
	DISP_PROPERTY_EX_ID(CZetBasicOle, "CompatibleDC", dispidCompatibleDC, GetCompatibleDC, SetCompatibleDC, VT_I4)
	//DISP_PROPERTY_EX_ID(CZetBasicOle, "isSimetric", dispidisSimetric, GetisSimetric, SetisSimetric, VT_I4)
	//DISP_PROPERTY_EX_ID(CZetBasicOle, "channelName", dispidchannelName, GetchannelName, SetchannelName, VT_BSTR)
	//DISP_PROPERTY_EX_ID(CZetBasicOle, "channelID", dispidchannelID, GetchannelID, SetchannelID, VT_I4)
	//DISP_PROPERTY_EX_ID(CZetBasicOle, "InsideCWnd", dispidInsideCWnd, GetInsideCWnd, SetInsideCWnd, VT_I4)
	DISP_PROPERTY_EX_ID(CZetBasicOle, "savedHwnd", dispidsavedHwnd, GetsavedHwnd, SetsavedHwnd, VT_I4)
	DISP_PROPERTY_EX_ID(CZetBasicOle, "LabelHWND", dispidLabelHWND, GetLabelHWND, SetLabelHWND, VT_HANDLE)
	DISP_PROPERTY_EX_ID(CZetBasicOle, "hasRightOffset", dispidhasRightOffset, GethasRightOffset, SethasRightOffset, VT_I4)
	DISP_PROPERTY_EX_ID(CZetBasicOle, "IdentModify", dispidIdentModify, GetIdentModify, SetIdentModify, VT_I4)
	DISP_PROPERTY_EX_ID(CZetBasicOle, "ToolTip", dispidToolTip, GetToolTip, SetToolTip, VT_BSTR)
	DISP_PROPERTY_EX_ID(CZetBasicOle, "IsSource", dispidIsSource, GetIsSource, SetIsSource, VT_I4)
	DISP_PROPERTY_EX_ID(CZetBasicOle, "IsVisibleAtRunTime", dispidIsVisibleAtRunTime, GetIsVisibleAtRunTime, SetIsVisibleAtRunTime, VT_BOOL)
	DISP_PROPERTY_EX_ID(CZetBasicOle, "queue", dispidqueue, Getqueue, Setqueue, VT_I4)
	DISP_PROPERTY_EX_ID(CZetBasicOle, "meDispatch", dispidmeDispatch, GetmeDispatch, SetmeDispatch, VT_DISPATCH)
	//����� �������� ��� ��������� ZETView2
	DISP_PROPERTY_EX_ID(CZetBasicOle, "Width", dispidWidth, GetWidth, SetWidth, VT_UI4)
	DISP_PROPERTY_EX_ID(CZetBasicOle, "Height", dispidHeight, GetHeight, SetHeight, VT_UI4)
	DISP_PROPERTY_EX_ID(CZetBasicOle, "Left", dispidLeft, GetLeft, SetLeft, VT_UI4)
	DISP_PROPERTY_EX_ID(CZetBasicOle, "Top", dispidTop, GetTop, SetTop, VT_UI4)
	DISP_PROPERTY_EX_ID(CZetBasicOle, "Configuration", dispidConfiguration, GetConfiguration, SetConfiguration, VT_BSTR)
	DISP_PROPERTY_EX_ID(CZetBasicOle, "QueOnRun", dispidQueOnRun, GetQueOnRun, SetQueOnRun, VT_I4)
END_DISPATCH_MAP()


BEGIN_EVENT_MAP(CZetBasicOle, COleControl)
	EVENT_CUSTOM_ID("defineView", eventiddefineView, defineView, VTS_PI4)
	EVENT_CUSTOM_ID("CreateConnector", eventidCreateConnector, CreateConnector, VTS_I4 VTS_I4 VTS_HANDLE VTS_I4 VTS_I4 VTS_I4)
	EVENT_CUSTOM_ID("MouseNearContact", eventidMouseNearContact, MouseNearContact, VTS_I4 VTS_I4 VTS_I4)
	EVENT_CUSTOM_ID("PlaceConnector", eventidPlaceConnector, PlaceConnector, VTS_I4 VTS_I4 VTS_HANDLE VTS_I4 VTS_I4 VTS_I4)
	//EVENT_CUSTOM_ID("requestName", eventidrequestName, requestName, VTS_I4 VTS_PBSTR VTS_PI4 VTS_PI4 VTS_PBSTR VTS_PI4)
	EVENT_CUSTOM_ID("requestType", eventidrequestType, requestType, VTS_PI4)
	EVENT_CUSTOM_ID("MessageFromComponent", eventidMessageFromComponent, MessageFromComponent, VTS_BSTR VTS_I4)
	//EVENT_CUSTOM_ID("getSRVCWnd", eventidgetSRVCWnd, getSRVCWnd, VTS_PI4)
	EVENT_CUSTOM_ID("requestChannels", eventidrequestChannels, requestChannels, VTS_I4)
	EVENT_CUSTOM_ID("connectToServer", eventidconnectToServer, connectToServer, VTS_I4 VTS_I4)
	EVENT_CUSTOM_ID("getMaxQuanDac", eventidgetMaxQuanDac, getMaxQuanDac, VTS_PI4)
	EVENT_CUSTOM_ID("serverCurrentTime", eventidserverCurrentTime, serverCurrentTime, VTS_I4 VTS_PR4)
	EVENT_CUSTOM_ID("serverGetData", eventidserverGetData, serverGetData, VTS_I4 VTS_I4 VTS_R8 VTS_I4 VTS_PR4)
	EVENT_CUSTOM_ID("serverNumFileUsed", eventidserverNumFileUsed, serverNumFileUsed, VTS_I4)
	EVENT_CUSTOM_ID("Registration", eventidRegistration, Registration, VTS_BSTR VTS_I4 VTS_PI4)
	EVENT_CUSTOM_ID("ContactPinInformation", eventidContactPinInformation, ContactPinInformation, VTS_I4 VTS_I4 VTS_BSTR VTS_BSTR VTS_R4 VTS_R4 VTS_I4 VTS_I4 VTS_I4 VTS_BOOL VTS_BOOL)
	EVENT_CUSTOM_ID("refreshStandardSize", eventidrefreshStandardSize, refreshStandardSize, VTS_I4 VTS_I4 VTS_I4 VTS_I4 VTS_I4 VTS_I4 VTS_I4)

	EVENT_CUSTOM_ID("getProjectName", eventidgetProjectName, getProjectName, VTS_PBSTR)

	EVENT_CUSTOM_ID("GetGuid", eventidGetSCADAGuid, GetSCADAGuid, VTS_I4 VTS_PBSTR)

	EVENT_CUSTOM_ID("ShowScadaConsole", eventidShowScadaConsole, ShowScadaConsole, VTS_NONE)
	EVENT_CUSTOM_ID("HideScadaConsole", eventidHideScadaConsole, HideScadaConsole, VTS_NONE)
	EVENT_CUSTOM_ID("PrintScadaConsole", eventidPrintScadaConsole, PrintScadaConsole, VTS_BSTR)
END_EVENT_MAP()

CZetBasicOle::CZetBasicOle(void)
: m_bRunMode(false)
, m_bDebug(false)
, m_oldQuanChan(0)
, m_nCurrentVersion(0)
, b_isScript(false)
, m_scadaBackColor(RGB(192,192,192))
, b_Refreshed(false)
, m_bHaveInsideActiveX(false)
, m_bMoveInsideActiveX(true)
, m_queOnRun(3)
, m_nVisibleAtRunTime(TRUE)
, m_bFirstLEak(false)
, CompWidthStandard(0)
, CompHeightStandard(0)
, ID_chan(0.)
, CompWidth(0)
, CompHeight(0)
, LeftX(0)
, LeftY(0)
, uniqueIdent(0)
, SecondID(0)
, m_bInOperate(0)
, queue(20)
, caption(L"")
, m_nOffset(2)
, isSource(true)
, m_nProjectNumber(0)
, m_hasLeftOffset(0)
, m_hasRightOffset(0)
, m_hasTopOffset(0)
, m_hasBottomOffset(0)
, m_mainHandle(0)
, m_thisHandle(0)
, haveServerInterface(false)
, haveOPCClientInterface(false)
, m_nSelChanNumber(0)
, serverCWndLong(0)
, m_nLoadingVersion(0)
, IDStr(0)
, m_lCurrComponentDeviceId(0)
, m_lCurrComponentModuleId(0)
, meDisp(nullptr)
, pArrayStr(nullptr)
{
	//������������� ��������� �������� ����������
	//���� ������� � ���, ��� �������� ����� ���� �������� � ����� ...Ctrl::DoPropExchange()

	m_sConfiguration = L"Release";
	params_RefreshData.rgvarg = 0;
	params_RefreshData.rgdispidNamedArgs = 0;
	params_RefreshData.cArgs = 3;
	params_RefreshData.cNamedArgs = 0;

	params_RefreshData2.rgvarg = 0;
	params_RefreshData2.rgdispidNamedArgs = 0;
	params_RefreshData2.cArgs = 3;
	params_RefreshData2.cNamedArgs = 0;

	params_RefreshData3.rgvarg = 0;
	params_RefreshData3.rgdispidNamedArgs = 0;
	params_RefreshData3.cArgs = 3;
	params_RefreshData3.cNamedArgs = 0;

	r_scadaBackColor = m_scadaBackColor & 255;
	g_scadaBackColor = (m_scadaBackColor >> 8) & 255;
	b_scadaBackColor = (m_scadaBackColor >> 16) & 255;
	s10 = { 0 };
	s20 = { 0 };
	s30 = { 0 };
	
	// ������� ZetDictionary.dll ���������������� � ���� ZetView.
	// ���� ��������� ��� ��������, �� ���� ���������������� ������� ZetDictionaryD.dll
#ifdef _DEBUG
#ifdef _TRANSLATE
	// ��� ������������� �� ������� InitLanguageDst ����� "Translate.cpp"
	wchar_t sLanguageDst[LOCALE_NAME_MAX_LENGTH] = { 0 };

	// �� ������� ��������� ����
	if (0 != zpQueryLanguage(sLanguageDst, LOCALE_NAME_MAX_LENGTH))
	{	// �������� ���� �� ������� �� �������, ������� ��������� �� ������
		GetUserDefaultLocaleName(sLanguageDst, LOCALE_NAME_MAX_LENGTH);
	}
	if (sLanguageDst[0] != 0)
	{// ���� ������� ������� (�� ������� ��� �� ������)
		// ������������� � ������� ���� ����
		long err = zdSetLanguageDst(sLanguageDst);
	}
	// � ������ � ������ ������ ������ ������!
#endif
#endif
}

void CZetBasicOle::SetModifiedFlag(BOOL bModified )
{
	COleControl::SetModifiedFlag(bModified);
}

void CZetBasicOle::DoPropExchange(CPropExchange* pPX, DWORD versionInfo)
{
	//���������� ������� ������
	m_nCurrentVersion = versionInfo;
	//������ ������� ������������ ���������� �������� ������� 
	//��� ������ ������� ������������ ����������
	ExchangeVersion(pPX, versionInfo);
	m_nLoadingVersion = pPX->GetVersion();
	if (isSource == true) 
	{
		bool is_load;
		if (pPX->IsLoading())
			is_load = true;
		else
			is_load = false;

		if(((is_load == true) && (uniqueIdent == 0)) || (is_load == false)) 
		{
			COleControl::DoPropExchange(pPX);

			//����������� ��������
			PX_String(pPX,L"�����z���",caption);
			
			PX_Long(pPX,L"������",CompWidth);
			PX_Long(pPX,L"������",CompHeight);
			
			PX_Long(pPX,L"����",LeftX,0);
			PX_Long(pPX,L"����",LeftY,0);
			PX_Long(pPX,L"�������������",uniqueIdent,0);
			PX_Long(pPX,L"�����.2",SecondID,0);
			long b = 0;
			PX_Long(pPX,L"����� ��.�������",b,0);
			
			long temp_m_hasLeftOffset;
			long temp_m_hasRightOffset;
			PX_Long(pPX,L"����� ������",temp_m_hasLeftOffset,0);
			PX_Long(pPX,L"������ ������",temp_m_hasRightOffset,0);
			
			//PX_Long(pPX,L"��������������",Simetric,1);
			PX_Long(pPX,L"���������� ����� ��������",queue);
			//PX_Long(pPX,L"����� �� �����",m_nLabelVisible,1);
			//PX_Long(pPX,L"����� � ������ RUN",m_nVisibleAtRunTime,1);
			
			//---------------------
			//��� ������ � �������� (������������)
			PX_String(pPX,L"��� ������",m_sSelected_channel,L"� ������");	//���������� �������� ������
			PX_Double(pPX,L"���������� ID ������",ID_chan);		//���������� ID ������

			PX_String(pPX,L"���������",m_sHelpString,L"");		//���������� �������� ������

		}
	}
//	PX_String(pPX,L"���������",m_sHelpString,L"");		//���������� �������� ������
}

CZetBasicOle::~CZetBasicOle(void)
{
}

void CZetBasicOle::OnSize(UINT nType, int cx, int cy)
{
	COleControl::OnSize(nType, cx, cy);

	CompWidth = cx;
	CompHeight = cy;


	//if (m_bMoveInsideActiveX == true)
	#ifdef INSIDE_ACTIVEX
		if (InsideActiveX.ActiveXcwnd != NULL)
		if (InsideActiveX.ActiveXcwnd->m_hWnd != NULL)
		//if (cwndValidate(InsideActiveX.ActiveXcwnd))
		if (InsideActiveX.ActiveXcwnd->IsWindowVisible())
		{
			InsideActiveX.ActiveXcwnd->MoveWindow(m_nOffset, m_nOffset, CompWidth - 2 * m_nOffset, CompHeight - 2 * m_nOffset);	
			if ((m_hasLeftOffset == 1) && (m_hasRightOffset == 1) && (m_hasTopOffset == 1) && (m_hasBottomOffset == 1))
				InsideActiveX.ActiveXcwnd->MoveWindow(PIN_LENGTH + m_nOffset, PIN_LENGTH + m_nOffset, CompWidth - 2 * PIN_LENGTH - 2 * m_nOffset, CompHeight - 2 * PIN_LENGTH - 2 * m_nOffset);
			else 
			if ((m_hasLeftOffset == 1) && (m_hasRightOffset == 1) && (m_hasTopOffset == 1) && (m_hasBottomOffset == 0))
				InsideActiveX.ActiveXcwnd->MoveWindow(PIN_LENGTH + m_nOffset, PIN_LENGTH + m_nOffset, CompWidth - 2 * PIN_LENGTH - 2 * m_nOffset, CompHeight - PIN_LENGTH - 2 * m_nOffset);
			else 
			if ((m_hasLeftOffset == 1) && (m_hasRightOffset == 1) && (m_hasTopOffset == 0) && (m_hasBottomOffset == 1))
				InsideActiveX.ActiveXcwnd->MoveWindow(PIN_LENGTH + m_nOffset, m_nOffset, CompWidth - 2 * PIN_LENGTH - 2 * m_nOffset, CompHeight - PIN_LENGTH - 2 * m_nOffset);
			else 
			if ((m_hasLeftOffset == 1) && (m_hasRightOffset == 1) && (m_hasTopOffset == 0) && (m_hasBottomOffset == 0))
				InsideActiveX.ActiveXcwnd->MoveWindow(PIN_LENGTH + m_nOffset, m_nOffset, CompWidth - 2 * PIN_LENGTH - 2 * m_nOffset, CompHeight - 2 * m_nOffset);
			else
			if ((m_hasLeftOffset == 1) && (m_hasRightOffset == 0) && (m_hasTopOffset == 1) && (m_hasBottomOffset == 1))
				InsideActiveX.ActiveXcwnd->MoveWindow(PIN_LENGTH + m_nOffset, PIN_LENGTH + m_nOffset, CompWidth - PIN_LENGTH - 2 * m_nOffset, CompHeight - 2 * PIN_LENGTH - 2 * m_nOffset);
			else 
			if ((m_hasLeftOffset == 1) && (m_hasRightOffset == 0) && (m_hasTopOffset == 1) && (m_hasBottomOffset == 0))
				InsideActiveX.ActiveXcwnd->MoveWindow(PIN_LENGTH + m_nOffset, PIN_LENGTH + m_nOffset, CompWidth - PIN_LENGTH - 2 * m_nOffset, CompHeight - PIN_LENGTH - 2 * m_nOffset);
			else 
			if ((m_hasLeftOffset == 1) && (m_hasRightOffset == 0) && (m_hasTopOffset == 0) && (m_hasBottomOffset == 1))
				InsideActiveX.ActiveXcwnd->MoveWindow(PIN_LENGTH + m_nOffset, m_nOffset, CompWidth - PIN_LENGTH - 2 * m_nOffset, CompHeight - PIN_LENGTH - 2 * m_nOffset);
			else 
			if ((m_hasLeftOffset == 1) && (m_hasRightOffset == 0) && (m_hasTopOffset == 0) && (m_hasBottomOffset == 0))
				InsideActiveX.ActiveXcwnd->MoveWindow(PIN_LENGTH + m_nOffset, m_nOffset, CompWidth - PIN_LENGTH - 2 * m_nOffset, CompHeight - 2 * m_nOffset);
			else
			if ((m_hasLeftOffset == 0) && (m_hasRightOffset == 1) && (m_hasTopOffset == 1) && (m_hasBottomOffset == 1))
				InsideActiveX.ActiveXcwnd->MoveWindow(m_nOffset, PIN_LENGTH + m_nOffset, CompWidth - PIN_LENGTH - 2 * m_nOffset, CompHeight - 2 * PIN_LENGTH - 2 * m_nOffset);
			else 
			if ((m_hasLeftOffset == 0) && (m_hasRightOffset == 1) && (m_hasTopOffset == 1) && (m_hasBottomOffset == 0))
				InsideActiveX.ActiveXcwnd->MoveWindow(m_nOffset, PIN_LENGTH + m_nOffset, CompWidth - PIN_LENGTH - 2 * m_nOffset, CompHeight - PIN_LENGTH - 2 * m_nOffset);
			else 
			if ((m_hasLeftOffset == 0) && (m_hasRightOffset == 1) && (m_hasTopOffset == 0) && (m_hasBottomOffset == 1))
				InsideActiveX.ActiveXcwnd->MoveWindow(m_nOffset, m_nOffset, CompWidth - PIN_LENGTH - 2 * m_nOffset, CompHeight - PIN_LENGTH - 2 * m_nOffset);
			else 
			if ((m_hasLeftOffset == 0) && (m_hasRightOffset == 1) && (m_hasTopOffset == 0) && (m_hasBottomOffset == 0))
				InsideActiveX.ActiveXcwnd->MoveWindow(m_nOffset, m_nOffset, CompWidth - PIN_LENGTH - 2 * m_nOffset, CompHeight - 2 * m_nOffset);
			else
			if ((m_hasLeftOffset == 0) && (m_hasRightOffset == 0) && (m_hasTopOffset == 1) && (m_hasBottomOffset == 1))
				InsideActiveX.ActiveXcwnd->MoveWindow(m_nOffset, PIN_LENGTH + m_nOffset, CompWidth - 2 * m_nOffset, CompHeight - 2 * PIN_LENGTH - 2 * m_nOffset);
			else 
			if ((m_hasLeftOffset == 0) && (m_hasRightOffset == 0) && (m_hasTopOffset == 1) && (m_hasBottomOffset == 0))
				InsideActiveX.ActiveXcwnd->MoveWindow(m_nOffset, PIN_LENGTH + m_nOffset, CompWidth - 2 * m_nOffset, CompHeight - PIN_LENGTH - 2 * m_nOffset);
			else 
			if ((m_hasLeftOffset == 0) && (m_hasRightOffset == 0) && (m_hasTopOffset == 0) && (m_hasBottomOffset == 1))
				InsideActiveX.ActiveXcwnd->MoveWindow(m_nOffset, m_nOffset, CompWidth - 2 * m_nOffset, CompHeight - PIN_LENGTH - 2 * m_nOffset);
			else 
			if ((m_hasLeftOffset == 0) && (m_hasRightOffset == 0) && (m_hasTopOffset == 0) && (m_hasBottomOffset == 0))
				InsideActiveX.ActiveXcwnd->MoveWindow(m_nOffset, m_nOffset, CompWidth - 2 * m_nOffset, CompHeight - 2 * m_nOffset);									
		}
	#endif

}


void CZetBasicOle::DisableBSTRCache()
{
	HINSTANCE hLib = LoadLibrary(L"OLEAUT32.DLL");
	if (hLib != NULL)
	{
		SETOANOCACHE SetOaNoCache = (SETOANOCACHE)GetProcAddress(hLib, "SetOaNoCache");
		if (SetOaNoCache != NULL)
			SetOaNoCache();
		FreeLibrary(hLib);

	}
}

int CZetBasicOle::OnCreate(LPCREATESTRUCT lpCreateStruct, CString progid)
{
	// 1. ������������� ������� � ����� �������
	// 2. progid ����� �� ������������
// #ifdef _TRANSLATE
// 	progid = Translate(progid);
// #endif

	if (COleControl::OnCreate(lpCreateStruct) == -1)
		return -1;

	//	TODO :
	//DisableBSTRCache();	//	���� �� ��������� � ������������� ������ BSTR

	defineView(&m_bInOperate);
	
	//TRACE_SCADA(progid+L" ����������� � ������ DEBUG");
	if ( !m_bInOperate )
	{
		classInformationS temp;
		temp.handle = this->m_hWnd;
		temp.number = classInformation.size();
		classInformation.push_back(temp);
		//TRACE("CZetBasicOle::OnCreate %d %d\n", temp.number, temp.handle);
	}
	else
	{
		GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL);
	}

	//��������� ����� �������� ����
	m_mainHandle = this->GetParent()->m_hWnd;
	//��������� ����� ������� ActiveX�
	m_thisHandle = this->m_hWnd;

	//������� ��������� � �� �� �������� ���������� ��������
	if (haveServerInterface == true)
	{
		connectToServer(1, long(m_thisHandle));
	}

	meDisp = GetIDispatch(false);
	
	// ������� ���� -----------------------------------------------------
	//m_strTranslateDst.Preallocate(128);
	m_nameOcx = (CString)AfxGetAppName() + L".ocx";

	// ���������� ����������� ������ ����������
	m_lCurrComponentModuleId = ++gComponentNumber;

	return 0;
}

CString CZetBasicOle::Translate(_In_ const CString &str)
{
#ifdef _TRANSLATE
	DWORD size(0);
	if (2 == zdTranslate(str, m_nameOcx, nullptr, &size))
	{
		if (0 == zdGetResultTranslate(m_strTranslateDst.GetBuffer(size), size))
		{
			m_strTranslateDst.ReleaseBuffer();

			CString s(str);
			int numAll = str.GetLength();
			s.TrimLeft();
			int numLeft = numAll - s.GetLength();
			s.TrimRight();
			int numRight = numAll - numLeft - s.GetLength();

			if (numLeft > 0)
				m_strTranslateDst = str.Left(numLeft) + m_strTranslateDst;
			if (numRight > 0)
				m_strTranslateDst += str.Right(numRight);

			return m_strTranslateDst;
		}
	}
#endif
	return str;
}

CString CZetBasicOle::Translate(_In_ const UINT & nID)
{// ��������� ������ �� ��������
	CString Stroka(L"");
	if (Stroka.LoadString(nID) != FALSE)
	{
#ifdef _TRANSLATE
		// � ������ ���� � ������� ��������� ����������, ��������� ������, ����������� �� ��������
		if (!Stroka.IsEmpty())
			Stroka = Translate(Stroka);
#endif
	}
	return Stroka;
}

void CZetBasicOle::StringSeparate(CString helpString, ULONG index)
{
	//������������� ������� ������ �������
	if (!tempChanInfo.empty())
		tempChanInfo.clear();
	//�������, � ������ �� ���-������?
	if (!helpString.IsEmpty())
	{
		//���� ������ �� ...

		tempChanInfoS temp;
		//���������� �������������� ������ ������ � ����������� �������� �������
		//���� � ����� ������ �� �������� ����� � �������, �� ���������� ��
		if (helpString[helpString.GetLength()-1] != L';')
			helpString += L";";
		//�� ��� ���, ���� �� ����������� ; ���������� ������
		while (helpString.Find(L";") != -1)
		{
			temp.name = helpString.Mid(0, helpString.Find(L";"));
			helpString.Delete(0, temp.name.GetLength()+1);
			if (temp.name.GetLength() > 0)
			{
				long b;
				temp.ID = IDFromName(temp.name, b);
				//��������� ������ �������
				tempChanInfo.push_back(temp);
			}
		}
		if (!tempChanInfo.empty())
		if (index < MAX_INPUTCHAN)
		{
			tempChan_0_Array[index] = tempChanInfo[0];
		}
	}
}

long CZetBasicOle::IDFromName(CString name, long& num)
{
	long retVal = 0;
	num = 0;
	
	name.MakeLower();
	if ( !name.IsEmpty() )
	{
		UINT m = serverInfo.channel.size();
		if (m > 0)
		{
			for (unsigned int i = 0; i < m; ++i)
			{
				CString temp = serverInfo.channel[i].Name;
				temp.MakeLower();
				if (temp == name)
				{
					retVal = serverInfo.channel[i].ID;
					num = i;	//���������� ����� ������
					break;
				}
			}
		}
	}
	return retVal;
}

void CZetBasicOle::setCaption(const CString &newCaption)
{
	caption = newCaption;
}

void CZetBasicOle::setOffsets(bool left, bool right, bool top, bool bottom)
{
	left == true?m_hasLeftOffset=1:m_hasLeftOffset=0;
	right == true?m_hasRightOffset=1:m_hasRightOffset=0;
	top == true?m_hasTopOffset=1:m_hasTopOffset=0;
	bottom == true?m_hasBottomOffset=1:m_hasBottomOffset=0;
}

void CZetBasicOle::SetServerInterface()
{
	haveServerInterface = true;
}

void CZetBasicOle::SetOPCClientInterface()
{
	haveOPCClientInterface = true;
}

void CZetBasicOle::testMessage(float par)
{
	CString str(L"");
	str.Format(L"%f",par);
	MessageBox(str);
}

void CZetBasicOle::OnClose(DWORD dwSaveOption)
{
	//for (UINT i = 0, sz = classInformation.size(); i < sz; ++i)

	for (auto &it : classInformation)
	{
		if (it.handle == this->m_hWnd)
		{
			it.handle = 0;
		}
	}

	globalRunMode = 0;

	//��� ����������� ������� �� ������ ������� ���� ����������� � ��� ActiveX�� ��������� � ���, ��� ���������� ��� 
	for (UINT i = 0, sz = SecondPointHandles.size(); i < sz; ++i)
	{
		if (!contactPinInfoArray.empty())
		{
			for (UINT j = 0, szpin = contactPinInfoArray.size(); j< szpin; ++j)
			{
				HWND hw = SecondPointHandles[i].hwnd;
				::PostMessage(SecondPointHandles[i].hwnd,				//����� ����, ���� ��������
							  WM_USER_SECOND_POINT_CONNECTED,			//��������� � �������� - �����������
							  WPARAM(m_thisHandle),						//���� ����� - ��� �������������
							  0);		
			}
			//��� ������������
		}
	}

	if (haveServerInterface == true)
		connectToServer(0,uniqueIdent);

	COleControl::OnClose(dwSaveOption);
}

#ifdef INSIDE_ACTIVEX
void CZetBasicOle::InstallActiveX(LPOLESTR clsidStr, bool enable, CString name)
{
	AfxEnableControlContainer();
	HRESULT result = CLSIDFromString(clsidStr,&InsideActiveX.clsidActiveX);
	if (result == NOERROR)
	{
		//LPOLESTR progID;
		//ProgIDFromCLSID(InsideActiveX.clsidActiveX,&progID);
		InsideActiveX.ActiveXcwnd = new(CWnd);
		
		if (InsideActiveX.ActiveXcwnd->CreateControl(InsideActiveX.clsidActiveX, L"", WS_VISIBLE | WS_CHILD, CRect(0,0,CompWidth-PIN_LENGTH,CompHeight),this,INSIDEACTIVEX_ID))
		{
			m_bHaveInsideActiveX = true;
			InsideActiveX.ActiveXcwnd->EnableWindow(enable); 
		} else
			{
				InsideActiveX.InsideID = 0;
			}
	} else 
		if (result == CO_E_CLASSSTRING)
		{
			InsideActiveX.InsideID = 0;
		}
}
#endif

CString CZetBasicOle::NameFromID(long ID)
{

	for (UINT i = 0, sz = serverInfo.channel.size(); i < sz; ++i)
		if (serverInfo.channel[i].ID == ID)
			return serverInfo.channel[i].Name;

	return L"-.-";
}

void CZetBasicOle::fillContactList(long contNum, CString name, CString helpInfo, float contDivX, float contDivY, long side, long pinType, long outValueType, bool multiconnect, bool isOut, bool bNeedTranslateName)
{
	//���������� ��������� ������ ���������� �����, ������������, ���� ��������� � �.�.
	contactPinInfoS info;				//������� ���������� ������������ ����

	info.number		= contNum;			//���������� ����� ��������, ���������� � 1
#ifdef _TRANSLATE
	info.name		= bNeedTranslateName ? Translate(name)	   : name;
	info.helpInfo	= bNeedTranslateName ? Translate(helpInfo) : helpInfo;		//����������� � ������
#else
	info.name		= name;
	info.helpInfo	= helpInfo;			//����������� � ������
#endif
	info.pointDivX	= contDivX;			//������ ����������� ������
	info.pointDivY	= contDivY;			//������ ����������� ������
	info.side		= side;				//������� ���������
	info.multiConnect = multiconnect;   //��������� ����������������
	info.pinType	= pinType;			//��� ��������
	info.isOut		= isOut;			//�������� �� ����� ��������
	info.outValue	= 65535;
	info.outValueType = outValueType;   //��������� ��� ������������� ��������
	info.connected	= false;			//����������� ��� ���			
	info.data.m_bDefined = false;
	//��������� ���������� �� ��������� ���� � ��
	contactPinInfoArray.push_back(info);
}

void CZetBasicOle::setLinearSizes(long cx, long cy)
{
	CompWidth = cx;
	CompHeight = cy;

	this->MoveWindow(LeftX,LeftY,CompWidth,CompHeight);
}

void CZetBasicOle::setSizes(CSize size_design, CSize size_operate,bool update_standard)
{
	//������� ������������� ������� ������������� ����
	if (m_bInOperate == 1)
	{
		setOffsets(false, false);	
		setLinearSizes(size_operate.cx, size_operate.cy);
		if (update_standard == true)
		{
			CompWidthStandard = CompWidth;
			CompHeightStandard = CompHeight;
		}
	}
	else
	//����� ������������� ������� ������ ����
	if (m_bInOperate == 0)
	{
		if (update_standard == true)
		{
			CompWidthStandard = size_design.cx;
			CompHeightStandard = size_design.cy;
		}
	}

}

LONG CZetBasicOle::GetMethodsDisp(void)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	return 0;
}

LONG CZetBasicOle::connectionInfo(OLE_HANDLE thisHWND, LONG thisContactNum, OLE_HANDLE secondHWND, LONG secondContactNum)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	SecondInfoS secondInfo;
	secondInfo.hwnd = HWND(secondHWND);				//��������� ����� ������ �����
	secondInfo.myContact = thisContactNum;			//��������� ���������� ����� �������� ������ ����
	secondInfo.secContact = secondContactNum;		//��������� ���������� ����� �������� �������� �����
	//��������� ������ � ��
	
	//�������� ��������� �� ������ �������������� ������
	IDispatch* pDisp(nullptr);
	BSTR bstr;
	GetZetIDispatch(long(secondInfo.hwnd),&pDisp, &bstr);
	CString ResStr(bstr);
	::SysFreeString(bstr);

	if (pDisp != NULL)
	{
		//������� � ����� ���� ��������� ������������� ���������
		//� ��������� � ��� ����� ����������
		CWnd* cwnd = FromHandle((HWND)secondHWND);
		if (cwnd)
		{
			if (b_isScript)
			{
				ResStr = toTranslit(ResStr);
				try
				{
					if (cwnd->GetParent() == this->GetParent())
						ResStr += L"_D";
					else
						ResStr += L"_L";
				}
				catch (...){};

				//			secondInfo.name = str;
				secondInfo.pDisp = pDisp;
				addLibs(ResStr, pDisp);
			}
			else
			{
				secondInfo.pDisp = pDisp;
				addLibs(L"", pDisp);
			}
			UINT m = libInfo.size();
			if (m > 0)
				libInfo[m - 1].hwnd = cwnd->m_hWnd;
		}

	} 
	else
	{
		secondInfo.pDisp = NULL;
	}
	//��������� ���������� � ��
	SecondPointHandles.push_back(secondInfo);

	//��������� � ���, ��� ���������� ������ ������� � ��������� ������
	//����������, ��� ������ �������� ��������������� ���������
	for (UINT j = 0, sz = contactPinInfoArray.size(); j < sz; ++j)
		if (contactPinInfoArray[j].number == thisContactNum)
		{
			CWnd* cw = FromHandle(HWND(secondHWND))->GetParent()->GetParent();
			CWnd* cw2 = this->GetParent()->GetParent();
			if (cw != cw2)		
				contactPinInfoArray[j].connected_anotherV = true;
			
			contactPinInfoArray[j].connected = true;
		}


	return 0;
}

void CZetBasicOle::OnModifyValue(int pinIndex, float Val, const CString &helpString)
{
	if ((UINT)pinIndex < contactPinInfoArray.size())
	{
		auto x = &contactPinInfoArray[pinIndex].data;
		x->fData = Val;
		x->helpString = helpString;
		x->m_bDefined = true;
		//contactPinInfoArray[pinIndex].data.fData = Val;
		//contactPinInfoArray[pinIndex].data.helpString = helpString;
		//contactPinInfoArray[pinIndex].data.m_bDefined = true;
	}
}
void CZetBasicOle::OnModifyValue(int pinIndex, double Val, const CString &helpString)
{
	if ((UINT)pinIndex < contactPinInfoArray.size())
	{
		auto x = &contactPinInfoArray[pinIndex].data;
		x->dData = Val;
		x->helpString = helpString;
		x->m_bDefined = true;
		//contactPinInfoArray[pinIndex].data.dData = Val;
		//contactPinInfoArray[pinIndex].data.helpString = helpString;
		//contactPinInfoArray[pinIndex].data.m_bDefined = true;
	}
}
void CZetBasicOle::OnModifyValue(int pinIndex, long Val, const CString &helpString)
{
	if ((UINT)pinIndex < contactPinInfoArray.size())
	{
		auto x = &contactPinInfoArray[pinIndex].data;
		x->lData = Val;
		x->helpString = helpString;
		x->m_bDefined = true;
		//contactPinInfoArray[pinIndex].data.lData = Val;
		//contactPinInfoArray[pinIndex].data.helpString = helpString;
		//contactPinInfoArray[pinIndex].data.m_bDefined = true;
	}
}
void CZetBasicOle::OnModifyValue(int pinIndex, const CString &Val, const CString &helpString)
{
	if ((UINT)pinIndex < contactPinInfoArray.size())
	{
		auto x = &contactPinInfoArray[pinIndex].data;
		x->sData = Val;
		x->helpString = helpString;
		x->m_bDefined = true;
		//contactPinInfoArray[pinIndex].data.sData = Val;
		//contactPinInfoArray[pinIndex].data.helpString = helpString;
		//contactPinInfoArray[pinIndex].data.m_bDefined = true;
	}
}

long CZetBasicOle::OnGetUncertainData(long Handle, long secContNum, long myContNum)
{
	for (UINT i = 0, sz = contactPinInfoArray.size(); i < sz; ++i)
	{
		auto x = contactPinInfoArray.data() + i;
		if (x->number == myContNum)
		if (x->data.m_bDefined)
		//if (contactPinInfoArray[i].number == myContNum)
		//	if (contactPinInfoArray[i].data.m_bDefined == true)
		{
			if (contactPinInfoArray[i].outValueType == TYPE_FLOAT)
			{
				doRefreshData1(myContNum, contactPinInfoArray[i].data.helpString, *(long*)&contactPinInfoArray[i].data.fData, TYPE_FLOAT);
			}
			else
			if (contactPinInfoArray[i].outValueType == TYPE_LONG)
			{
				doRefreshData1(myContNum, contactPinInfoArray[i].data.helpString, contactPinInfoArray[i].data.lData, TYPE_LONG);
			}
			else
			if (contactPinInfoArray[i].outValueType == TYPE_STRING)
			{
				doRefreshData1(myContNum, contactPinInfoArray[i].data.sData, 0, TYPE_STRING);
			}
		}
	}

	return 0;
}

void CZetBasicOle::OnDrawInBuffer(LONG Handle)
{

}

void CZetBasicOle::GetLeftData(long pin_number, long index)
{
	VARIANT mVars[3];	
	VARIANT vaResult_RefreshData;
	VariantInit(&vaResult_RefreshData);

	//VariantInit(&mVars[2]);							
	mVars[2].vt = VT_I4;
	mVars[2].uintVal = OLE_HANDLE(m_thisHandle);

	//VariantInit(&mVars[1]);
	mVars[1].vt = VT_I4;
	mVars[1].lVal = pin_number;

	//VariantInit(&mVars[0]);
	mVars[0].vt = VT_I4;
	mVars[0].lVal = SecondPointHandles[index].secContact;

	params_RefreshData.rgvarg = mVars;
	params_RefreshData.rgdispidNamedArgs = 0;
	params_RefreshData.cArgs = 3;
	params_RefreshData.cNamedArgs = 0;

	//�������� ����� ����������
	try
	{
		SecondPointHandles[index].pDisp->Invoke(dispidGetUncertainData,       //ID ������� ���������� ������ 
							  IID_NULL,				 //���������
							  LOCALE_SYSTEM_DEFAULT,   //���� �� ���������
							  DISPATCH_METHOD,		 //��� �����
							  &params_RefreshData,	 //���������
							  &vaResult_RefreshData,	 //���������
							  NULL,					 
							  NULL);
	}
	//catch (CMemoryException*)
	//{

	//}
	//catch (CFileException*)
	//{
	//}
	//catch (CException*)
	//{
	//}
	catch(...)
	{

	}

	//VariantClear(&vaResult_RefreshData);

	VariantClear(&mVars[2]);
	VariantClear(&mVars[1]);
	VariantClear(&mVars[0]);
}

void CZetBasicOle::GetLeftDataFromPin(LONG pinIndex)
{
	//���� ������ �������
	if (m_bRunMode)
		if ((UINT)pinIndex < contactPinInfoArray.size())
	{
		//���������� ��� ���������� � ���� ��, ������� ���������� � ������ �����
		for (UINT c = 0, sz1 = SecondPointHandles.size(); c < sz1; ++c)
		{
			if (SecondPointHandles[c].myContact == contactPinInfoArray[pinIndex].number)
			{
				if (SecondPointHandles[c].hwnd != NULL)
				{
					if (SecondPointHandles[c].pDisp != NULL)
					{
						try
						{
							GetLeftData(contactPinInfoArray[pinIndex].number, /*SecondPointHandles[c]*/c);
						}
						//catch (CMemoryException*){}
						//catch (CFileException*){}
						//catch (CException*){}
						catch(...){}
					}
				}
			}
		}
	}
}

void CZetBasicOle::OnPostRunFromScada(LONG message)
{
	//���� ������ �������
	{
		//���������� ���������� �����
		for (UINT i = 0, sz = contactPinInfoArray.size(); i < sz; ++i)
		{
			//���� ����� ��������� �����
			if (contactPinInfoArray[i].side == LEFT_SIDE)
			{
				//���� ����� �� ���� �������������������, �� ���������� ��������� � ���������� ����� ����������
				if (contactPinInfoArray[i].connected)
				if (contactPinInfoArray[i].data.m_bDefined == false)
				if (contactPinInfoArray[i].pinType != IN_MANAGE)
				if (contactPinInfoArray[i].pinType != OUT_MANAGE)
				{
					//�������� ��������� ������ � �����
					try
					{
						//���������� ��� ���������� � ���� ��, ������� ���������� � ������ �����
						for (UINT c = 0, sz1 = SecondPointHandles.size(); c < sz1; ++c)
						{
							if (SecondPointHandles[c].myContact == contactPinInfoArray[i].number)
							{
								if (SecondPointHandles[c].hwnd != NULL)
								{
									if (SecondPointHandles[c].pDisp != NULL)
									{
										GetLeftData(contactPinInfoArray[i].number, c);
									}
								}
							}
						}
					}
					//catch (CMemoryException*)
					//{
					//
					//}
					//catch (CFileException*)
					//{
					//}
					//catch (CException*)
					//{
					//}
					catch(...)
					{

					}
				}
			}
		}
	}
}

LONG CZetBasicOle::RefreshData(OLE_HANDLE sourceWHND, LONG pinNUM, LPCTSTR helpString, LONG parametr, LONG parametrType)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	if (m_bDebug)
	{
		//�������� � �����, ��� ������ �� �������� ����������
		switch (parametrType)
		{
		case TYPE_FLOAT :
		case TYPE_FLOAT_BAD :
		case TYPE_FLOAT_UNCERTAIN :
			{
				UpdateContactData_FLOAT((long)m_mainHandle, pinNUM, *((float*)&parametr));
				break;
			}
		case TYPE_LONG :
			{
				UpdateContactData_LONG((long)m_mainHandle, pinNUM, parametr);
				break;
			}
		case TYPE_STRING :
		case TYPE_CHANNEL :
			{
				if (!CString(helpString).IsEmpty())
				{
					BSTR tmpbStr = SysAllocString(helpString);
					UpdateContactData_STRING((long)m_mainHandle, pinNUM, tmpbStr);
					::SysFreeString(tmpbStr);
				}
				break;
			}
		}

	}
	return 0;
}
LONG CZetBasicOle::RefreshData2(OLE_HANDLE sourceWHND, LONG pinNum, LPCTSTR helpString, LONG pA, LONG uBound, LONG parametrType)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	//��������� �� ������������ � ���������� ��������
	long arrSize = uBound + 1;
	if (arrSize > MAXPOINTS)
		arrSize = MAXPOINTS;

	if (m_bDebug)
	{
		if (parametrType == TYPE_POINTER)
		{
			UpdateArray((long)m_mainHandle, pinNum, arrSize, pA);
		}
	}

	return 0;
}
LONG CZetBasicOle::RefreshData3(OLE_HANDLE sourceWHND, LONG pinNum, LPCTSTR helpString, LONGLONG pA, LONG uBound, LONG parametrType)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	return 0;
}
LONG CZetBasicOle::RefreshData5(OLE_HANDLE sourceWHND, LONG pinNUM, LPCTSTR helpString, double parametr, LONG parametrType)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	return 0;
}
LONG CZetBasicOle::RefreshDataArray1(LONG pinNum, VARIANT &theStrings, VARIANT &theArray)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	return 0;
}

LONG CZetBasicOle::RefreshDataU(OLE_HANDLE sourceWHND, LONG pinNum, LPCTSTR helpString, LONG* lpdata)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	BSTR tmpbStr;
	if (m_bDebug)
	{
		if (lpdata != nullptr)
		{
			ZetViewTag* tag = (ZetViewTag*)lpdata;
			switch (tag->type)
			{
			case TYPE_STRING:
				{
					if (tag->value.pCText != NULL)
					{
						tmpbStr = SysAllocString(*tag->value.pCText);
						UpdateContactData_STRING((long)m_mainHandle, pinNum, tmpbStr);
					}
					break;
				}
			}
		}

	}	
	return 0;
}

//��������
ULONG CZetBasicOle::GetCompWidth(void)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	return (ULONG)CompWidth;
}
void CZetBasicOle::SetCompWidth(ULONG newVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	CompWidth = (LONG)newVal;
}

ULONG CZetBasicOle::GetCompHeight(void)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	return (ULONG)CompHeight;
}
void CZetBasicOle::SetCompHeight(ULONG newVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	CompHeight = (LONG)newVal;
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

	CString strResult(L"");

	strResult = caption;

	return strResult.AllocSysString();
}
void CZetBasicOle::SetCaptionWithClassInfo(LPCTSTR newVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

#ifdef _TRANSLATE
	caption.Format(L"%s_%u", (Translate(newVal)).GetString(), classInformation.size());
#else
	caption.Format(L"%s_%u", newVal, classInformation.size());
#endif

	//���� ����� ����� ���������� �������, �� 
	SetsHelpString(caption);

	SetModifiedFlag();
}
void CZetBasicOle::SetCaption(LPCTSTR newVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	//���� ����� ����� ���������� �������, �� 
	caption = newVal;

	::PostMessage(m_mainHandle,WM_USER_CHANGE_ACTIVEX_NAME,WPARAM(this->m_hWnd),0);

	SetModifiedFlag();
}

LONG CZetBasicOle::Getqueue(void)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	return queue;
}
void CZetBasicOle::Setqueue(LONG newVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	queue = newVal;

	SetModifiedFlag();
}

BSTR CZetBasicOle::GetToolTip(void)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	CString strResult(m_sToolTip);

	return strResult.AllocSysString();
}
void CZetBasicOle::SetToolTip(LPCTSTR newVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	m_sToolTip = newVal;
	BSTR tmpbStr = SysAllocString(m_sToolTip);
	ChangeComponentToolTip(tmpbStr);
	::SysFreeString(tmpbStr);

	SetModifiedFlag();
}

BSTR CZetBasicOle::GetsHelpString(void)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	CString strResult(m_sHelpString);

	return strResult.AllocSysString();
}
void CZetBasicOle::SetsHelpString(LPCTSTR newVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	m_sHelpString = newVal;
	
	SetModifiedFlag();
}

BSTR CZetBasicOle::GetConfiguration(void)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	CString strResult(m_sConfiguration);

	return strResult.AllocSysString();
}
void CZetBasicOle::SetConfiguration(LPCTSTR newVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	m_sConfiguration = newVal;
	
	::PostMessage(m_mainHandle, WM_USER_CHANGE_CONFIGURATION, WPARAM(this->m_hWnd), 0);

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

LONG CZetBasicOle::GethasRightOffset(void)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	return m_hasRightOffset;
}
void CZetBasicOle::SethasRightOffset(LONG newVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	SetModifiedFlag();
}

LONG CZetBasicOle::GetCompatibleDC(void)
{
	return 0;
}
void CZetBasicOle::SetCompatibleDC(LONG newVal)
{
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

//���� ������� � ���������� ID ����������
LONG CZetBasicOle::GetsavedHwnd(void)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	return 0;
}
void CZetBasicOle::SetsavedHwnd(LONG newVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	SetModifiedFlag();
}
//���� ���������� ����� � ������ ������
OLE_HANDLE CZetBasicOle::GetLabelHWND(void)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	return NULL;
}
void CZetBasicOle::SetLabelHWND(OLE_HANDLE newVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	SetModifiedFlag();
}

LONG CZetBasicOle::GetIdentModify(void)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	return IsModified();
}
void CZetBasicOle::SetIdentModify(LONG newVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	SetModifiedFlag(newVal);
}

VARIANT_BOOL CZetBasicOle::GetIsVisibleAtRunTime(void)
 {
	 AFX_MANAGE_STATE(AfxGetStaticModuleState());

	 return (m_nVisibleAtRunTime != FALSE) ? VARIANT_TRUE : VARIANT_FALSE;
 }
void CZetBasicOle::SetIsVisibleAtRunTime(VARIANT_BOOL newVal)
 {
	 AFX_MANAGE_STATE(AfxGetStaticModuleState());
	 BOOL valNew = (newVal != VARIANT_FALSE) ? TRUE : FALSE;
	 if (m_nVisibleAtRunTime != valNew)
	 {
		 m_nVisibleAtRunTime = valNew;
		 if (m_bInOperate && m_bRunMode)
		 {
			 if (m_nVisibleAtRunTime)
				 ShowWindow(SW_SHOW);
			 else
				 ShowWindow(SW_HIDE);
		 }
	 
		 SetModifiedFlag();
	 }
 }

ULONG CZetBasicOle::GetWidth(void)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	return CompWidth;
}
void CZetBasicOle::SetWidth(ULONG newVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	if (CompWidth != newVal)
	{
		CompWidth = newVal;
		setSizes(CSize(CompWidth,CompHeight),CSize(CompWidth,CompHeight),false);
		moveSize(uniqueIdent, LeftX, LeftY, CompWidth, CompHeight);
		SetModifiedFlag();
		//OnUpdateByConteiner(0,0);
	}
}

ULONG CZetBasicOle::GetHeight(void)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	return CompHeight;
}
void CZetBasicOle::SetHeight(ULONG newVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	if (CompHeight != newVal)
	{
		CompHeight = newVal;
		setSizes(CSize(CompWidth,CompHeight),CSize(CompWidth,CompHeight),false);
		moveSize(uniqueIdent, LeftX, LeftY, CompWidth, CompHeight);
		SetModifiedFlag();
	}
}

ULONG CZetBasicOle::GetLeft(void)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	return LeftX;
}
void CZetBasicOle::SetLeft(ULONG newVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	if (LeftX != newVal)
	{
		LeftX = newVal;
		this->MoveWindow(LeftX,LeftY,CompWidth,CompHeight);
		moveSize(uniqueIdent, LeftX, LeftY, CompWidth, CompHeight);
		SetModifiedFlag();
		//OnUpdateByConteiner(0,0);
	}
}

ULONG CZetBasicOle::GetTop(void)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	return LeftY;
}
void CZetBasicOle::SetTop(ULONG newVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	if (LeftY != newVal)
	{
		LeftY = newVal;
		this->MoveWindow(LeftX,LeftY,CompWidth,CompHeight);
		moveSize(uniqueIdent, LeftX, LeftY, CompWidth, CompHeight);
		SetModifiedFlag();
		//OnUpdateByConteiner(0,0);
	}
}

LONG CZetBasicOle::GetQueOnRun(void)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	// TODO: Add your dispatch handler code here

	return m_queOnRun;
}
void CZetBasicOle::SetQueOnRun(LONG newVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	m_queOnRun = newVal;

	SetModifiedFlag();
}

//��������
//��������� �� ���������� ���������� �� ����������
LRESULT CZetBasicOle::OnUpdateByConteiner(WPARAM wParam, LPARAM lParam)
{
	if (m_bInOperate)
	{
		GetClientRect(rect);
		OnSize(0,CompWidth,CompHeight);
	}
//	m_bCanRedraw = true;
	else
	{

		if (uniqueIdent != NULL)
		{
			for (UINT i = 0, size = contactPinInfoArray.size(); i < size; ++i)
			{
				ContactPinInformation(long(uniqueIdent), 
									  contactPinInfoArray[i].number, 
									  contactPinInfoArray[i].name,
									  contactPinInfoArray[i].helpInfo,
									  contactPinInfoArray[i].pointDivX,
									  contactPinInfoArray[i].pointDivY,
									  contactPinInfoArray[i].side,
									  contactPinInfoArray[i].pinType,
									  contactPinInfoArray[i].outValueType,
									  (contactPinInfoArray[i].multiConnect != false) ? VARIANT_TRUE : VARIANT_FALSE,
									  (contactPinInfoArray[i].isOut != false) ? VARIANT_TRUE : VARIANT_FALSE);

			}
			refreshStandardSize(long(uniqueIdent),CompWidthStandard,CompHeightStandard,m_hasLeftOffset,m_hasRightOffset,m_hasTopOffset,m_hasBottomOffset);

		}
	}

	return 0;
}

LONG CZetBasicOle::GetMemoryLeak(void)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

#ifndef DEBUG
	return 0;
#else

	if (m_bFirstLEak == true)
	{
		m_bFirstLEak = false;
		//������ ������ ������
		_CrtMemCheckpoint( &s10 );
	}
	//������ ������ ������
	_CrtMemCheckpoint( &s20 );

	//������ ���������� �������
	//bool ret = 
	_CrtMemDifference( &s30, &s10, &s20);

	long currMemLeak = long(s30.lSizes[1]);

	return currMemLeak;

#endif

}

LONG CZetBasicOle::GetAllocHeapBl(void)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	return gCurrComponentHeapBl;
}

LONG CZetBasicOle::GetAllocHeapSz(void)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	return gCurrComponentHeapSz;
}

LONG CZetBasicOle::ResetMemoryLeak(void)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

#ifdef DEBUG
	m_bFirstLEak = true;
	//������ ������ ������
	_CrtMemCheckpoint( &s10 );
	//������ ������ ������
	_CrtMemCheckpoint( &s20 );
#endif

	return 0;
}

LRESULT CZetBasicOle::OnMinMax(WPARAM wParam, LPARAM lParam)
{
	long minWidth = minWidth_Singlesizes;
	long maxWidth = maxWidth_SingleSizes;

	if (m_hasRightOffset && m_hasLeftOffset)
	{
		minWidth = minWidth_bisizes;
		maxWidth = maxWidth_bisizes;
	}
	else
	if ((m_hasRightOffset && !m_hasLeftOffset) ||
		(!m_hasRightOffset && m_hasLeftOffset))
	{
		minWidth = minWidth_Singlesizes;
		maxWidth = maxWidth_SingleSizes;
	}

	if (CompWidth == minWidth)
	{
		SetWidth(maxWidth);
	}
	else
	{
		SetWidth(minWidth);
	}

	return 0;
}

LRESULT CZetBasicOle::OnUpdateBackColor(WPARAM wParam, LPARAM lParam)
{
	m_scadaBackColor = wParam;
	r_scadaBackColor = m_scadaBackColor & 255;
	g_scadaBackColor = (m_scadaBackColor >> 8) & 255;
	b_scadaBackColor = (m_scadaBackColor >> 16) & 255;
	Invalidate(FALSE);
	return 0;
}

LRESULT CZetBasicOle::OnConnLineDisconnect(WPARAM wParam, LPARAM lParam)
{
	//� ��������� wParam �������� ����� �������������� �����
	//� ��������� lParam �������� ����� ���������� - ��������� ����������
	for (int i = SecondPointHandles.size() - 1; i >= 0; i--)
	{	
		if (HWND(wParam) == SecondPointHandles[i].hwnd)
			if (SecondPointHandles[i].myContact == lParam)
			{
			//	CString name = caption;
				SecondPointHandles.erase(SecondPointHandles.begin() + i);
				if (lParam - 1 < (long)contactPinInfoArray.size())
					contactPinInfoArray[lParam-1].connected = false;
			}
	}

	for (int i = libInfo.size() - 1; i >= 0; i--)
	{	
		if (HWND(wParam) == libInfo[i].hwnd)
		{
			//libInfo.RemoveAt(i);	
			libInfo.erase(libInfo.begin()+i);
			i = 0;
		}
	}
	return 0;
}

LRESULT CZetBasicOle::OnStartAll(WPARAM wParam, LPARAM lParam)
 {	//���������� ��� ���������� ������� �����
	for (auto it = contactPinInfoArray.begin(), ie = contactPinInfoArray.end();
				it != ie; ++it)
	{
		if (it->side == LEFT_SIDE)
		{
			auto x = &it->data;
			x->m_bDefined = false;
			x->dData = 0.;
			x->fData = 0.f;
			x->helpString = L"";
			x->lData = 0;
		}
	}

	m_bRunMode = wParam != 0;
	m_nProjectNumber = lParam;

	if (m_bInOperate)
	{
		if (m_bRunMode)
		{// ������ �������
			if (m_nVisibleAtRunTime == 0)
			{
				ShowWindow(SW_HIDE);
			}
		}
		else
		{// ������ �� �������
			ShowWindow(SW_SHOW);
		}
	}
	return 0;
 }

LRESULT CZetBasicOle::OnDebugMode(WPARAM wParam, LPARAM lParam)
{
	m_bDebug = lParam != 0;
	return 0;
}

void CZetBasicOle::SetDefaultCaption(LPCTSTR newVal)
{
//	if (caption == L"")
	if ( ((CString)caption).IsEmpty() )
		SetCaption(newVal);
}

//���������� ������ �������� � ���� ������, ����������� �������� � ��������� �� ��������� ���������������
CString CZetBasicOle::addLibs(const CString &name, CWnd* cwnd)
{
	CString tempName(L"");
	IUnknown* pUnkn(nullptr);
	if (cwnd != NULL)
	if (cwnd->m_hWnd != NULL)
	{
		pUnkn = cwnd->GetControlUnknown();
		if (pUnkn != NULL)
		{
			IDispatch* pDisp(nullptr);
			pUnkn->QueryInterface(IID_IDispatch, (void**)&pDisp);
			if (pDisp != NULL)
			{
				tempName.Format(L" %s ", name.GetString());

				libInfoS lib;
				lib.name = name;
				lib.pDisp = pDisp;
				lib.hwnd = cwnd->m_hWnd;
				libInfo.push_back(lib);
				//pDisp->Release();
			}
		}
	}
	return tempName;
}

//��� ��������� ���������� �� ��������� IDispatch ��������, � �������, �������� ����� ��� ���������, �� �� ������������ ����������� �� �������� ������
CString CZetBasicOle::addLibs(const CString &name, IDispatch* pDisp)
{
	CString tempName(L"");
	if (pDisp != NULL)
	{
		tempName.Format(L" %s ", name.GetString());
		
		libInfoS lib;
		lib.name = name;
		lib.pDisp = pDisp;

		libInfo.push_back(lib);

		//pDisp->Release();
	}

	return tempName;
}

CString CZetBasicOle::MakeLowerUnicode(const CString &text)
{
	CString lowtext(text);
	lowtext.MakeLower();
	lowtext.Replace(L'�', L'�');
	lowtext.Replace(L'�', L'�');
	lowtext.Replace(L'�', L'�');
	lowtext.Replace(L'�', L'�');
	lowtext.Replace(L'�', L'�');
	lowtext.Replace(L'�', L'�');
	lowtext.Replace(L'�', L'�');
	lowtext.Replace(L'�', L'�');
	lowtext.Replace(L'�', L'�');
	lowtext.Replace(L'�', L'�');
	lowtext.Replace(L'�', L'�');
	lowtext.Replace(L'�', L'�');
	lowtext.Replace(L'�', L'�');
	lowtext.Replace(L'�', L'�');
	lowtext.Replace(L'�', L'�');
	lowtext.Replace(L'�', L'�');
	lowtext.Replace(L'�', L'�');
	lowtext.Replace(L'�', L'�');
	lowtext.Replace(L'�', L'�');
	lowtext.Replace(L'�', L'�');
	lowtext.Replace(L'�', L'�');
	lowtext.Replace(L'�', L'�');
	lowtext.Replace(L'�', L'�');
	lowtext.Replace(L'�', L'�');
	lowtext.Replace(L'�', L'�');
	lowtext.Replace(L'�', L'�');
	lowtext.Replace(L'�', L'�');
	lowtext.Replace(L'�', L'�');
	lowtext.Replace(L'�', L'�');
	lowtext.Replace(L'�', L'�');
	lowtext.Replace(L'�', L'�');
	lowtext.Replace(L'�', L'�');

	return lowtext;
}

CString CZetBasicOle::toRemoveSymbols(CString text)
{
	//CString str = L""; �� ������������
	//CStringA russian_;
	//russian_ = text;
	//russian_.MakeLower();
	//str = russian_;
	//str.Replace(L" ", L"_");
	//return str;
	//text.MakeLower();
	text = MakeLowerUnicode(text);
	text.Replace(L" ", L"_");
	return text;
}

CString CZetBasicOle::toTranslit(CString russian)
{
	if (b_isScript == true)
	{
		CString english(L"");
		/*CStringA russian_;
		russian_ = russian;
		russian_.MakeLower();
		russian = russian_;*/
		russian = MakeLowerUnicode(russian);
		for (int i = 0; i < russian.GetLength(); ++i)
		{
			wchar_t sign1 = russian[i];
			long sign = russian[i];
			switch (sign)
			{
				case 32:
					if (i != 0)
					{
						english = english + L"_";				//������
					}
				break;		
				case 1040: case 1072: english = english + L"a"; break;		//�
				case 1041: case 1073: english = english + L"b"; break;		//�
				case 1042: case 1074: english = english + L"v"; break;		//�
				case 1043: case 1075: english = english + L"g"; break;		//�
				case 1044: case 1076: english = english + L"d"; break;		//�
				case 1045: case 1077: english = english + L"e"; break;		//�
				case 1025: case 1105: english = english + L"e"; break;		//�
				case 1046: case 1078: english = english + L"zh"; break;		//�
				case 1047: case 1079: english = english + L"z"; break;		//�
				case 1048: case 1080: english = english + L"i"; break;		//�
				case 1049: case 1081:
					if (english.GetLength() > 0)
					{
						if (english[english.GetLength() - 1] != 'i')
						{
							english = english + L"i";			//�								
						}
					}
				break;
				case 1050: case 1082: english = english + L"k"; break;		//�
				case 1051: case 1083: english = english + L"l"; break;		//�
				case 1052: case 1084: english = english + L"m"; break;		//�
				case 1053: case 1085: english = english + L"n"; break;		//�
				case 1054: case 1086: english = english + L"o"; break;		//�
				case 1055: case 1087: english = english + L"p"; break;		//�
				case 1056: case 1088: english = english + L"r"; break;		//�
				case 1057: case 1089: english = english + L"s"; break;		//�
				case 1058: case 1090: english = english + L"t"; break;		//�
				case 1059: case 1091: english = english + L"u"; break;		//�
				case 1060: case 1092: english = english + L"f"; break;		//�
				case 1061: case 1093: english = english + L"h"; break;		//�
				case 1062: case 1094: english = english + L"ts"; break;	//�
				case 1063: case 1095: english = english + L"ch"; break;	//�
				case 1064: case 1096: english = english + L"sh"; break;	//�
				case 1065: case 1097: english = english + L"sh"; break;	//�
				case 1066: case 1098: english = english + L"";   break;	//�
				case 1067: case 1099:
					if (english.GetLength() > 0)
					{
						if (english[english.GetLength() - 1] != 'i')
						{
							english = english + L"i";			//�
						}
					}
				break;
				case 1068: case 1100: english = english + L"";   break;	//�
				case 1069: case 1101: english = english + L"e";  break;	//�
				case 1070: case 1102: english = english + L"yu"; break;	//�
				case 1071: case 1103: english = english + L"ya"; break;	//�
				case 48:
					if (i == 0)
						english = english + L"m";
					english = english + L"0";					//0
				break;		
				case 49:
					if (i == 0)
						english = english + L"m";
					english = english + L"1";					//1
				break;
				case 50:
					if (i == 0)
						english = english + L"m";
					english = english + L"2";					//2
				break;
				case 51:
					if (i == 0)
						english = english + L"m";
					english = english + L"3";					//3
				break;	
				case 52:
					if (i == 0)
						english = english + L"m";
					english = english + L"4";					//4
				break;
				case 53:
					if (i == 0)
						english = english + L"m";
					english = english + L"5";					//5
				break;
				case 54:
					if (i == 0)
						english = english + L"m";
					english = english + L"6";					//6
				break;
				case 55:
					if (i == 0)
						english = english + L"m";
					english = english + L"7";					//7
				break;
				case 56:
					if (i == 0)
						english = english + L"m";
					english = english + L"8";					//8
				break;
				case 57:
					if (i == 0)
						english = english + L"m";
					english = english + L"9";					//9
				break;
				default:
					english += russian[i];
				break;
			}
		}
		return english;
	}

	return L"b_isScript not Supported";
}

void CZetBasicOle::SetIsSource(LONG newVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	isSource = newVal != 0;
	SetModifiedFlag();
}

LONG CZetBasicOle::GetIsSource(void)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	return 0;
}

void CZetBasicOle::doRefreshData1(int pinNum, const CString &helpString, long parametr, long parametrType)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	BSTR tmpbStr;
	if (m_bDebug)
	{
		//�������� � �����, ��� ������ �� �������� ����������
		switch (parametrType)
		{
		case TYPE_FLOAT :
		case TYPE_FLOAT_BAD :
		case TYPE_FLOAT_UNCERTAIN :
			{
				UpdateContactData_FLOAT((long)m_mainHandle, pinNum, *((float*)&parametr));
				break;
			}
		case TYPE_LONG :
			{
				UpdateContactData_LONG((long)m_mainHandle, pinNum, parametr);
				break;
			}
		case TYPE_STRING :
		case TYPE_CHANNEL :
			{
				if (!helpString.IsEmpty())
				{
					tmpbStr = SysAllocString(helpString);
					UpdateContactData_STRING((long)m_mainHandle, pinNum, tmpbStr);
				}
				break;
			}

		}
	}
	
	//���������� ��� ����������, ������������ � ������� ����
	for (UINT i = 0, sz = SecondPointHandles.size(); i < sz; ++i)
	{
		if (SecondPointHandles[i].myContact == pinNum)
		{
			if (SecondPointHandles[i].hwnd != NULL)
			{
				if (SecondPointHandles[i].pDisp != NULL)
				{
					if (IsWindow(SecondPointHandles[i].hwnd))
					{
						VARIANT mVars[5];	
						VARIANT vaResult_RefreshData;
						VariantInit(&vaResult_RefreshData);
						
						//VariantInit(&mVars[4]);			
						mVars[4].vt = VT_UI4;
						mVars[4].uintVal = OLE_HANDLE(m_thisHandle);

						//VariantInit(&mVars[3]);							
						mVars[3].vt = VT_UI4;
						mVars[3].uintVal = SecondPointHandles[i].secContact;

						//VariantInit(&mVars[2]);
						mVars[2].vt = VT_BSTR;
						BSTR tempStr;
						tempStr = SysAllocString(helpString);
						mVars[2].bstrVal = tempStr;
						//SysFreeString(tempStr);

						//VariantInit(&mVars[1]);
						mVars[1].vt = VT_I4;
						mVars[1].lVal = parametr;

						//VariantInit(&mVars[0]);
						mVars[0].vt = VT_I4;
						mVars[0].lVal = parametrType;

						params_RefreshData.rgvarg = mVars;
						params_RefreshData.rgdispidNamedArgs = 0;
						params_RefreshData.cArgs = 5;	// 5
						params_RefreshData.cNamedArgs = 0;

						//�������� ����� ����������
						SecondPointHandles[i].pDisp->Invoke(dispidRefreshData,       //ID ������� ���������� ������ 
															IID_NULL,				 //���������
															LOCALE_SYSTEM_DEFAULT,   //���� �� ���������
															DISPATCH_METHOD,		 //��� �����
															&params_RefreshData,	 //���������
															&vaResult_RefreshData,	 //���������
															NULL,					 
															NULL);

						//VariantClear(&vaResult_RefreshData);
						VariantClear(&mVars[4]);
						VariantClear(&mVars[3]);
						//VariantClear(&mVars[2]);
						SysFreeString(tempStr);
						VariantClear(&mVars[1]);
						VariantClear(&mVars[0]);
					}
				}
			}
		}
	}
}
void CZetBasicOle::doRefreshData2(int pinNum, const CString &helpString, long parametr, long arrSize, long parametrType)
{
	//��������� �� ������������ � ���������� ��������
	if (arrSize > MAXPOINTS)
		arrSize = MAXPOINTS;

	if (m_bDebug)
	{
		if (parametrType == TYPE_POINTER)
		{
			UpdateArray((long)m_mainHandle, pinNum, arrSize, parametr);
		}
	}

	//���������� ��� ����������, ������������ � ������� ����
	for (UINT i = 0, sz = SecondPointHandles.size(); i < sz; ++i)
	{
		if (SecondPointHandles[i].myContact == pinNum)
		{
			if (SecondPointHandles[i].hwnd != NULL)
			{
				if (SecondPointHandles[i].pDisp != NULL)
				{
					if (IsWindow(SecondPointHandles[i].hwnd))
					{
						VARIANT mVars2[6];
						VARIANT vaResult_RefreshData2;
						VariantInit(&vaResult_RefreshData2);	

						//VariantInit(&mVars2[5]);							
						mVars2[5].vt = VT_UI4;
						mVars2[5].uintVal = OLE_HANDLE(m_thisHandle);

						//VariantInit(&mVars2[4]);							
						mVars2[4].vt = VT_UI4;
						mVars2[4].uintVal = SecondPointHandles[i].secContact;

						//VariantInit(&mVars2[3]);
						mVars2[3].vt = VT_BSTR;
						BSTR tempStr;
						tempStr = SysAllocString(helpString);
						mVars2[3].bstrVal = tempStr;
						SysFreeString(tempStr);

						//VariantInit(&mVars2[2]);
						mVars2[2].vt = VT_I4;
						mVars2[2].lVal = parametr;

						//VariantInit(&mVars2[1]);
						mVars2[1].vt = VT_I4;
						mVars2[1].lVal = arrSize;

						//VariantInit(&mVars2[0]);
						mVars2[0].vt = VT_I4;
						mVars2[0].lVal = parametrType;

						params_RefreshData2.rgvarg = mVars2;
						params_RefreshData2.rgdispidNamedArgs = 0;
						params_RefreshData2.cArgs = 6;
						params_RefreshData2.cNamedArgs = 0;

						//�������� ����� ����������
						SecondPointHandles[i].pDisp->Invoke(dispidRefreshData2,			//ID ������� ���������� ������ 
															IID_NULL,					//���������
															LOCALE_SYSTEM_DEFAULT,		//���� �� ���������
															DISPATCH_METHOD,			//��� �����
															&params_RefreshData2,		//���������
															&vaResult_RefreshData2,		//���������
															NULL,					 
															NULL);

						//VariantClear(&vaResult_RefreshData2);

						VariantClear(&mVars2[5]);
						VariantClear(&mVars2[4]);
						//VariantClear(&mVars2[3]);
						VariantClear(&mVars2[2]);
						VariantClear(&mVars2[1]);
						VariantClear(&mVars2[0]);
						
					}
				}
			}
		}
	}		
}
void CZetBasicOle::doRefreshData3(int pinNum, const CString &helpString, long long parametr, long arrSize, long parametrType)
{
	//��������� �� ������������ � ���������� ��������
	if (arrSize > MAXPOINTS)
		arrSize = MAXPOINTS;

	//���������� ��� ����������, ������������ � ������� ����
	for (UINT i = 0, sz = SecondPointHandles.size(); i < sz; ++i)
	{
		if (SecondPointHandles[i].myContact == pinNum)
		{
			if (SecondPointHandles[i].hwnd != NULL)
			{
				if (SecondPointHandles[i].pDisp != NULL)
				{
					if (IsWindow(SecondPointHandles[i].hwnd))
					{
						VARIANT mVars3[6];
						VARIANT vaResult_RefreshData3;
						VariantInit(&vaResult_RefreshData3);	

						//VariantInit(&mVars3[5]);							
						mVars3[5].vt = VT_UI4;
						mVars3[5].uintVal = OLE_HANDLE(m_thisHandle);

						//VariantInit(&mVars3[4]);							
						mVars3[4].vt = VT_UI4;
						mVars3[4].uintVal = SecondPointHandles[i].secContact;

						//VariantInit(&mVars3[3]);
						mVars3[3].vt = VT_BSTR;
						BSTR tempStr;
						tempStr = SysAllocString(helpString);
						mVars3[3].bstrVal = tempStr;
						SysFreeString(tempStr);

						//VariantInit(&mVars3[2]);
						mVars3[2].vt = VT_I8;
						mVars3[2].lVal = (LONG)parametr;

						//VariantInit(&mVars3[1]);
						mVars3[1].vt = VT_I4;
						mVars3[1].lVal = arrSize;

						//VariantInit(&mVars3[0]);
						mVars3[0].vt = VT_I4;
						mVars3[0].lVal = parametrType;

						params_RefreshData3.rgvarg = mVars3;
						params_RefreshData3.rgdispidNamedArgs = 0;
						params_RefreshData3.cArgs = 6;
						params_RefreshData3.cNamedArgs = 0;

						//�������� ����� ����������
						SecondPointHandles[i].pDisp->Invoke(dispidRefreshData3,			//ID ������� ���������� ������ 
															IID_NULL,					//���������
															LOCALE_SYSTEM_DEFAULT,		//���� �� ���������
															DISPATCH_METHOD,			//��� �����
															&params_RefreshData3,		//���������
															&vaResult_RefreshData3,		//���������
															NULL,					 
															NULL);

						//VariantClear(&vaResult_RefreshData3);

						VariantClear(&mVars3[5]);
						VariantClear(&mVars3[4]);
						//VariantClear(&mVars3[3]);
						VariantClear(&mVars3[2]);
						VariantClear(&mVars3[1]);
						VariantClear(&mVars3[0]);
						
					}
				}
			}
		}
	}
}
void CZetBasicOle::doRefreshData4(int pinNum, const CString &helpString, IDispatch* parametr, long parametrType)
{
	//���������� ��� ����������, ������������ � ������� ����
	for (UINT i = 0, sz = SecondPointHandles.size(); i < sz; ++i)
	{
		if (SecondPointHandles[i].myContact == pinNum)
		{
			if (SecondPointHandles[i].hwnd != NULL)
			{
				if (SecondPointHandles[i].pDisp != NULL)
				{
					if (IsWindow(SecondPointHandles[i].hwnd))
					{
						VARIANT mVars[5];	
						VARIANT vaResult_RefreshData;
						VariantInit(&vaResult_RefreshData);

						//VariantInit(&mVars[4]);							
						mVars[4].vt = VT_UI4;
						mVars[4].uintVal = OLE_HANDLE(m_thisHandle);

						//VariantInit(&mVars[3]);							
						mVars[3].vt = VT_UI4;
						mVars[3].uintVal = SecondPointHandles[i].secContact;

						//VariantInit(&mVars[2]);
						mVars[2].vt = VT_BSTR;
						BSTR tempStr;
						tempStr = SysAllocString(helpString);
						mVars[2].bstrVal = tempStr;
						SysFreeString(tempStr);

						//VariantInit(&mVars[1]);
						mVars[1].vt = VT_DISPATCH;
						mVars[1].pdispVal = parametr;

						//VariantInit(&mVars[0]);
						mVars[0].vt = VT_I4;
						mVars[0].lVal = parametrType;

						params_RefreshData.rgvarg = mVars;
						params_RefreshData.rgdispidNamedArgs = 0;
						params_RefreshData.cArgs = 5;
						params_RefreshData.cNamedArgs = 0;

						//�������� ����� ����������
						SecondPointHandles[i].pDisp->Invoke(dispidRefreshData4,       //ID ������� ���������� ������ 
							IID_NULL,				 //���������
							LOCALE_SYSTEM_DEFAULT,   //���� �� ���������
							DISPATCH_METHOD,		 //��� �����
							&params_RefreshData,	 //���������
							&vaResult_RefreshData,	 //���������
							NULL,					 
							NULL);

						//VariantClear(&vaResult_RefreshData);

						mVars[1].pdispVal = NULL;
						VariantClear(&mVars[4]);
						VariantClear(&mVars[3]);
						//VariantClear(&mVars[2]);
						VariantClear(&mVars[1]);
						VariantClear(&mVars[0]);
					}
				}
			}
		}
	}		
}
void CZetBasicOle::doRefreshData5(int pinNum, const CString &helpString, double parametr, long parametrType)
{	//���������� ��� ����������, ������������ � ������� ����
	for (UINT i = 0, sz = SecondPointHandles.size(); i < sz; ++i)
	{
		if ( (SecondPointHandles[i].myContact == pinNum) &&
			 (SecondPointHandles[i].hwnd != NULL) &&
			 (SecondPointHandles[i].pDisp != NULL) &&
			 (IsWindow(SecondPointHandles[i].hwnd)) )
		{
			VARIANT mVars[5];	
			VARIANT vaResult_RefreshData;
			VariantInit(&vaResult_RefreshData);

			//VariantInit(&mVars[4]);			
			mVars[4].vt = VT_UI4;
			mVars[4].uintVal = OLE_HANDLE(m_thisHandle);

			//VariantInit(&mVars[3]);							
			mVars[3].vt = VT_UI4;
			mVars[3].uintVal = SecondPointHandles[i].secContact;
			
			//VariantInit(&mVars[2]);
			mVars[2].vt = VT_BSTR;
			BSTR tempStr;
			tempStr = SysAllocString(helpString);
			mVars[2].bstrVal = tempStr;
			SysFreeString(tempStr);

			//VariantInit(&mVars[1]);
			mVars[1].vt = VT_R8;
			mVars[1].dblVal = parametr;

			//VariantInit(&mVars[0]);
			mVars[0].vt = VT_I4;
			mVars[0].lVal = parametrType;

			params_RefreshData.rgvarg = mVars;
			params_RefreshData.rgdispidNamedArgs = 0;
			params_RefreshData.cArgs = 5;	// 5
			params_RefreshData.cNamedArgs = 0;

			//�������� ����� ����������
			SecondPointHandles[i].pDisp->Invoke(dispidRefreshData5,      //ID ������� ���������� ������ 
												IID_NULL,				 //���������
												LOCALE_SYSTEM_DEFAULT,   //���� �� ���������
												DISPATCH_METHOD,		 //��� �����
												&params_RefreshData,	 //���������
												&vaResult_RefreshData,	 //���������
												NULL,					 
												NULL);

			//VariantClear(&vaResult_RefreshData);

			VariantClear(&mVars[4]);
			VariantClear(&mVars[3]);
			//VariantClear(&mVars[2]);
			VariantClear(&mVars[1]);
			VariantClear(&mVars[0]);
		}
	}
}

void CZetBasicOle::doRefreshDataU(int pinNum, const CString &helpString, ZetViewTag *pData)
{// �������� ������������� ��������� ������

	BSTR tmpbStr;
	if (m_bDebug)
	{
		if (pData != nullptr)
		{
			switch (pData->type)
			{
			case TYPE_STRING:
			{
				if (pData->value.pCText != NULL)
				{
					tmpbStr = SysAllocString(*pData->value.pCText);
					UpdateContactData_STRING((long)m_mainHandle, pinNum, tmpbStr);
				}
				break;
			}
			}
		}

	}
	
	for (UINT i = 0, sz = SecondPointHandles.size(); i<sz; ++i)
	{
		if ( (SecondPointHandles[i].myContact == pinNum) &&
			(SecondPointHandles[i].hwnd != NULL) &&
			(SecondPointHandles[i].pDisp != NULL) &&
			(IsWindow(SecondPointHandles[i].hwnd)) )
		{
			VARIANT mVars[4];	
			VARIANT vaResult_RefreshData;
			VariantInit(&vaResult_RefreshData);
	
			mVars[3].vt = VT_UI4;
			mVars[3].uintVal = OLE_HANDLE(m_thisHandle);
						
			mVars[2].vt = VT_UI4;
			mVars[2].uintVal = SecondPointHandles[i].secContact;

			mVars[1].vt = VT_BSTR;
			BSTR tempStr = SysAllocString(helpString);
			mVars[1].bstrVal = tempStr;
			SysFreeString(tempStr);

			mVars[0].plVal = (LONG*)pData;
			mVars[0].vt = VT_BYREF|VT_I4;

			params_RefreshData.rgvarg = mVars;
			params_RefreshData.rgdispidNamedArgs = 0;
			params_RefreshData.cArgs = 4;
			params_RefreshData.cNamedArgs = 0;

			//�������� ����� ����������
			SecondPointHandles[i].pDisp->Invoke(dispidRefreshDataU,      //ID ������� ���������� ������ 
				IID_NULL,				 //���������
				LOCALE_SYSTEM_DEFAULT,   //���� �� ���������
				DISPATCH_METHOD,		 //��� �����
				&params_RefreshData,	 //���������
				&vaResult_RefreshData,	 //���������
				NULL,					 
				NULL);

		//	VariantClear(&vaResult_RefreshData);
			VariantClear(&mVars[3]);
			VariantClear(&mVars[2]);
		//	VariantClear(&mVars[1]);
			VariantClear(&mVars[0]);
		}
	}
}

BSTR CZetBasicOle::ContactCaption(LONG contactNum)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	CString strResult(L"-.-");
	for (auto it = contactPinInfoArray.cbegin(), ie = contactPinInfoArray.cend();
			it != ie; ++it)
	{
		if ( (it->number == contactNum) && (it->pinType < 30) )
		{
			strResult = it->name + L"\t" + it->helpInfo;
			break;
		}
	}
	//for (int i = 0, sz = contactPinInfoArray.size(); i < sz; ++i)
	//{
	//	if (contactPinInfoArray[i].number == contactNum)
	//	{
	//		if (contactPinInfoArray[i].pinType < 30)
	//		{
	//			strResult = contactPinInfoArray[i].name+L"\t"+contactPinInfoArray[i].helpInfo;
	//		}
	//	}
	//}
	return strResult.AllocSysString();
}

void CZetBasicOle::CheckVisibleAtRunTime(WPARAM wParam)
{
	if (GetIsVisibleAtRunTime() == 0)
		//���������� ���������
		if (wParam == 0)
		{
			ShowWindow(SW_SHOW);
		}
		else
		//������ ���������
		{
			ShowWindow(SW_HIDE);
			//������ ����� �������� ���������
			::PostMessage(m_mainHandle,WM_USER_CHANGE_ACTIVEX_NAME,WPARAM(this->m_hWnd),0);
		}
}

IDispatch* CZetBasicOle::GetmeDispatch(void)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	return meDisp;
}
void CZetBasicOle::SetmeDispatch(IDispatch* pVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	SetModifiedFlag();
}

LRESULT CZetBasicOle::OnRemoveChannels(WPARAM wParam, LPARAM lParam)
{
	return 0;
}

LRESULT CZetBasicOle::OnAddChannels(WPARAM wParam, LPARAM lParam)
{
	return 0;
}

void CZetBasicOle::OnTimer(UINT_PTR nIDEvent)
{
	if (nIDEvent == IDT_UPDATE_TIMER)
	{
		KillTimer(IDT_UPDATE_TIMER);

		if (!serverInfo.channel.empty() || !serverInfo.old_channel.empty())
		{
			//��� ������ ������� ������ ����������� � ��������� �������
			if (!serverInfo.lost_channel.empty())
				serverInfo.lost_channel.clear();
			if (!serverInfo.new_channel.empty())
				serverInfo.new_channel.clear();

			std::vector<chanInfoS>::iterator iter_old;
			std::vector<chanInfoS>::iterator iter_new;
			// ��������� ������� ������� �� ������ ������ � ������ ������
			for (iter_new = serverInfo.channel.begin(); iter_new != serverInfo.channel.end(); ++iter_new)
			{
				bool bChanAppear = true;	// ���� ��������� ������ ������
				for (iter_old = serverInfo.old_channel.begin(); iter_old != serverInfo.old_channel.end();)
				{
					if (iter_old->ID == iter_new->ID &&
						iter_old->typeADC == iter_new->typeADC)
					{
						bChanAppear = false;
						iter_old = serverInfo.old_channel.erase(iter_old);
					}
					else
						++iter_old;
				}
				if (bChanAppear)
				{
					serverInfo.new_channel.push_back(*iter_new);
					OnAddChannels(0, 0);
				}
			}
			// ���� � ������ ������ ������� �������� ������, ������ ��� �������
			if (!serverInfo.old_channel.empty())
			{
				for (iter_old = serverInfo.old_channel.begin(); iter_old != serverInfo.old_channel.end(); ++iter_old)
				{
					serverInfo.lost_channel.push_back(*iter_old);
					OnRemoveChannels(0, 0);
				}
				serverInfo.old_channel.clear();
			}
			// �������� ����� ������ ������� 
			serverInfo.old_channel.assign(serverInfo.channel.begin(), serverInfo.channel.end());
		}

		if (!serverInfo.device.empty() || !serverInfo.old_device.empty())
		{
			//��� ������ ������� ������ ����������� � ��������� ���������
			if (!serverInfo.lost_device.empty())
				serverInfo.lost_device.clear();
			if (!serverInfo.new_device.empty())
				serverInfo.new_device.clear();

			std::vector<deviceInfoS>::iterator iter_old;
			std::vector<deviceInfoS>::iterator iter_new;
			// ��������� ������� ��������� �� ������ ������ � ������ ������
			for (iter_new = serverInfo.device.begin(); iter_new != serverInfo.device.end(); ++iter_new)
			{
				bool bDeviceAppear = true;	// ���� ��������� ������ ����������
				for (iter_old = serverInfo.old_device.begin(); iter_old != serverInfo.old_device.end();)
				{
					if (iter_old->typeDevice == iter_new->typeDevice &&
						iter_old->serial     == iter_new->serial &&
						iter_old->numberDSP  == iter_new->numberDSP)
					{
						bDeviceAppear = false;
						iter_old = serverInfo.old_device.erase(iter_old);
					}
					else
						++iter_old;
				}
				if (bDeviceAppear)
				{
					if ((iter_new->typeDevice >= 0) && (iter_new->numberDSP >= 0))
					{
						serverInfo.new_device.push_back(*iter_new);
						::PostMessage(this->m_hWnd, WM_USER_ADD_DEVICES, 0, 0);
					}
				}
			}
			// ���� � ������ ������ ��������� �������� ������, ������ ��� �������
			if (!serverInfo.old_device.empty())
			{
				for (iter_old = serverInfo.old_device.begin(); iter_old != serverInfo.old_device.end(); ++iter_old)
				{
					if ((iter_old->typeDevice >= 0) && (iter_old->numberDSP >= 0))
					{
						serverInfo.lost_device.push_back(*iter_old);
						::PostMessage(this->m_hWnd, WM_USER_REMOVE_DEVICES, 0, 0);
					}
				}
				serverInfo.old_device.clear();
			}
			// �������� ����� ������ ��������� 
			serverInfo.old_device.assign(serverInfo.device.begin(), serverInfo.device.end());
		}
	}

	COleControl::OnTimer(nIDEvent);
}

LONG CZetBasicOle::fillChannels(LONG id, LPCTSTR name, LPCTSTR conv, FLOAT freq, FLOAT refer, LONG typeADC)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	chanInfoS temp;
	//���������� ID ������
	temp.ID = id;
	//���������� �������� ������
	temp.Name = name;									
	//����������� ������� �������������
	temp.Freq = freq;									
	temp.Convertion = conv;
	temp.Reference = refer;

	//���������� ��� ��� �� ����� - adc, dac ��� virt
	//�������� ���
// 	if (typeADC >= 0 && typeADC < 100)
// 		temp.typeADC = 0;
// 	//���
// 	if (typeADC >= 100 && typeADC < 200)
// 		temp.typeADC = 1;
// 	//��� ����������� �����
// 	if (typeADC == -5)	
// 		temp.typeADC = 2;
	temp.typeADC = typeADC;

	serverInfo.channel.push_back(temp);	//������� ����� ������ � �� �������

	return 0;
}

LONG CZetBasicOle::fillDevices(LONG typeADC, LONG numberDSP, LONG serial, LPCTSTR name)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	deviceInfoS temp;

	temp.typeDevice = typeADC;
	temp.numberDSP = numberDSP;
	temp.serial = serial;
	temp.Name = name;
	serverInfo.device.push_back(temp);

	return 0;
}

LONG CZetBasicOle::removeAllChannels(void)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	if (!serverInfo.channel.empty())
		serverInfo.channel.clear();
	if (!serverInfo.device.empty())
		serverInfo.device.clear();

	return 0;
}

void CZetBasicOle::applyChannels(void)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	KillTimer(IDT_UPDATE_TIMER);
	SetTimer(IDT_UPDATE_TIMER, 1000, NULL);
}

TDataQuality CZetBasicOle::TDataQualityFromString(const CString &s_quality)
{
	if (s_quality == L"UNCERTAIN")
		return dqUncertain;
	else
	if (s_quality == L"GOOD")
		return dqGood;
	else
		return dqBad;

}

LONG CZetBasicOle::SignalTypeFromString(const CString &s_quality)
{
	if (s_quality == L"UNCERTAIN")
		return TYPE_FLOAT_UNCERTAIN;
	else
	if (s_quality == L"GOOD")
		return TYPE_FLOAT_GOOD;
	else
		return TYPE_FLOAT_BAD;
}

LONG CZetBasicOle::SignalTypeFromTDataQuality(long n_quality)
{
	if (n_quality == dqUncertain)
		return TYPE_FLOAT_UNCERTAIN;
	else
	if (n_quality == dqGood)
		return TYPE_FLOAT_GOOD;
	else
		return TYPE_FLOAT_BAD;
}

void CZetBasicOle::AddManagerContact()
{
	fillContactList(contactPinInfoArray.size() + 1,	//������� ����� ���������
					L"",
					L"������� ����� � ���������� �������",
					1.0/2,								//���������� X ����� 1/2 ������
					1.0,								//���������� Y ����� ������ ����������
					BOTTOM_SIDE,						//������� ��������� - ������
					IN_MANAGE,							//���� �������� IN_MANAGE 
					TYPE_MANAGE,
					false,								//��� ��������� ����������������
					false);									
}

void CZetBasicOle::ClearArrayStr(void)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	for (int i = 0; i < MAXPOINTS; ++i)
		pArrayStr[i].Empty();
}

void CZetBasicOle::CreateArrayStr(void)
{
	//�������������� ������� ���������
	IDStr = 0;
	long pTemp = 0;
	//������� ������, ������������� � ZETView, � �� � ����� ����������

	CreateArrayStr(long(this->m_hWnd),VARIANT_TRUE,MAXPOINTS,&IDStr,&pTemp);
	pArrayStr = (CString*)pTemp;
}

void CZetBasicOle::fillDefArrayStr(void)
{
	ClearArrayStr();
}

void CZetBasicOle::OnWindowPosChanging(WINDOWPOS* lpwndpos)
{

	if (m_bInOperate == 0)
		lpwndpos->flags &= ~SWP_SHOWWINDOW;

	COleControl::OnWindowPosChanging(lpwndpos);
}

LONG CZetBasicOle::OnChangeGenParam(LONG param, FLOAT fValue, LPCTSTR sValue)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	return 0;
}

bool CZetBasicOle::FindChannelFromName(const CString &name, long &id)
{// ����� ������ �� ��� �����. ����������� ��������: ����� ������ ��� ���.
	// ���� ����� ������, �� ��� ������������� ������������ � id
	bool ret(false);
	if (!serverInfo.channel.empty())
	{
		for (auto it = serverInfo.channel.cbegin(), ie = serverInfo.channel.cend();
			it != ie; ++it)
		{
			if (name == it->Name)
			{
				id = it->ID;
				ret = true;
				break;
			}
		}
	}
	return ret;
}

uint64_t CZetBasicOle::getCurrComponentDeviceId()
{
	return m_lCurrComponentDeviceId;
}

uint32_t CZetBasicOle::getCurrComponentModuleId()
{
	return m_lCurrComponentModuleId;
}
