#pragma once

#include "C:\ZETTools\ZETView_basic_class\CDSRV.h"
#include "C:\ZETTools\Interface2012\Draw\CustomDraw.h"

#define MAX_COMPUTERS					100
#define MAX_CHANNELS					271
#define MAX_DEVICES						100
#define IDS_SECOND                      137
#define IDS_MINUTE                      141
#define IDS_MYCOMPUTER                  140
#define IDS_HOUR                        142
#define IDS_DAY                         143
#define IDS_GLOBALTIME                  139
#define IDS_START                       144
#define IDS_QUANTITY                    145

struct ChannelS
{
	ChannelS::ChannelS()
	{
		sName = L"";
		sDeviceName = L"";
		sCompName = L"";
		sServerName = L"";
		lNumber = -1;
		bSelected = false;
		rChanRect = CRect(0, 0, 0, 0);
		bLBtnDown = false;
		lStatus = -1;
		lID = -1;
	}

	
	CString sName;			//�������� ������
	CString sDeviceName;	//������������� ����������
	CString sCompName;		//������������� ����������
	CString sServerName;	//������������� �������

	long lNumber;			//����� ������ �� �������
	long lStatus;			//������ ������
	long lID;				//������������� ������

	CRect rChanRect;		//������� ������ ����� � ������� ������
	bool bSelected;			//������ �� �����
	bool bLBtnDown;			//������ �� �� ������ ����� ������ ����
};

struct DeviceS
{
	DeviceS::DeviceS()
	{
		hIcon = NULL;
		lTypeDSP = 0;
		lNumberDSP = 0;
		sName = L"";
		sCompName = L"";
		sServerName = L"";
		bExpanded = true;
		bSelected = false;
		bLBtnDown = false;
		rCheckBoxRect = CRect(0, 0, 0, 0);
		rDeviceRect = CRect(0, 0, 0, 0);
		nCounter = 0;
		for (int i = 0; i < MAX_CHANNELS; i++)
			channel[i] = -1;
		hBitmap = NULL;
		lStatus = -1;
	}

	Bitmap* hBitmap;					//�������� ������
	HICON hIcon;						//������ ������

	long lTypeDSP;						//��� ������
	long lNumberDSP;					//����� ����������� ���������� ������

	CString sName;						//��� ������
	CString sCompName;					//������������� ����������
	CString sServerName;				//������������� �������

	long channel[MAX_CHANNELS];			//������ ������

	bool bExpanded;						//��������� ������ ��� ���
	bool bSelected;						//������� �� ������
	bool bLBtnDown;						//������ �� ����� ������ ����

	CRect rCheckBoxRect;				//������� ��������� �����/������
	CRect rDeviceRect;					//������� ������ ����� ������

	long nCounter;						//c������ ������� �� ������
	long lStatus;
};

struct CompS
{
	CompS::CompS()
	{
		hIcon = NULL;
		sName = L"";
		sServerName = L"";
		bExpanded = true;
		bSelected = false;
		bLBtnDown = false;
		rCheckBoxRect = CRect(0, 0, 0, 0);
		rCompRect = CRect(0, 0, 0, 0);
		nCounter = 0;
		for (int i = 0; i < MAX_DEVICES; i++)
			device[i] = -1;
		hBitmap = NULL;
	}

	Bitmap* hBitmap;				//�������� ����������
	HICON hIcon;					//������ ����������

	CString sName;					//��� ����������
	CString sServerName;			//������������� �������

	long device[MAX_DEVICES];		//���������� ����������

	bool bExpanded;					//��������� ������ ��� ���
	bool bSelected;					//������� �� ���������
	bool bLBtnDown;					//������ �� ����� ������ ����

	CRect rCheckBoxRect;			//������� ��������� �����/������
	CRect rCompRect;				//������� ������ ����� ����������

	long nCounter;					//������� ������� �� ����������
};

struct ServerS
{
	ServerS::ServerS()
	{
		hIcon = NULL;
		sName = L"";
		bExpanded = true;
		bSelected = false;
		bLBtnDown = false;
		rCheckBoxRect = CRect(0, 0, 0, 0);
		rServerRect = CRect(0, 0, 0, 0);
		nCounter = 0;
		for (int i = 0; i < MAX_COMPUTERS; i++)
			comp[i] = -1;
		nDeviceCounter = 0;
		hBitmap = NULL;
	}
	Bitmap* hBitmap;
	HICON hIcon;					//������

	CString sName;					//��� �������

	long comp[MAX_COMPUTERS];		//���������� �������
	long device[MAX_DEVICES];		//���������� �������

	bool bExpanded;					//��������� ������ ��� ���
	bool bSelected;					//������� �� ������
	bool bLBtnDown;					//������ �� ����� ������ ����

	CRect rCheckBoxRect;			//������� ������ �����/������
	CRect rServerRect;				//������� ������ ����� �������

	long nCounter;					//������� ����������� �� �������
	long nDeviceCounter;			//������� ��������� �� �������
};

class CTreePanel : public CScrollView
{
	DECLARE_DYNCREATE(CTreePanel)

public:
	CTreePanel();           
	virtual ~CTreePanel();

	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnSize(UINT nType, int cx, int cy);

	long FindServer(CString serverName);
	long FindComp(CString compName, CString serverName);
	long FindDevice(CString deviceName, CString compName, CString serverName);
	//CString GetTime(double time);

	SolidBrush* brush_back;
	SolidBrush* brush_selectedServer;
	SolidBrush* brush_selectedDevice;
	SolidBrush* brush_selectedChannel;
	SolidBrush* brush_Channel;
	SolidBrush* brush_Channel_cur;
	Gdiplus::Font* fontServer;
	Gdiplus::Font* fontDevice;
	Gdiplus::Font* fontChannel;
	SolidBrush* brush_litera;
	SolidBrush* brush_litera_inactive;
	Pen* pen_circle;
	Pen* pen_frame;
	Pen* pen_line;
	Bitmap* backBuffer;
	StringFormat* format_name;
	StringFormat* format_time;
	int CurrentChannel;
	CString stringSeconds;
	CString stringMinutes;
	CString stringHours;
	CString stringDays;
	CString stringGlobal;
	CString stringStart;
	CString stringQuantity;
	CString str;
	BOOL flag_channel;
	CRect rect;

	
	float workingTime;
	bool isPaused;
	ImageAttributes attr;
	long scrollPosition;

protected:
	virtual void OnDraw(CDC* pDC);      
	virtual void OnInitialUpdate(); 
	void OnDrawMy(CDC* pdc, Graphics& gr, CRect rcBounds);
	void OnDrawServers(Graphics &gr, CRect rcBounds, long* addings);
	void OnDrawComps(Graphics &gr, CRect rcBounds, long* addings, long startDrawing, long n);
	void OnDrawDevices(Graphics &gr, CRect rcBounds, long* addings, long startDrawing, long compIndex);
	void OnDrawChannels(Graphics &gr, CRect rcBounds, long* addings, long startDrawing, long deviceIndex);

	DECLARE_MESSAGE_MAP()
public:
	//afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	CString GetTextItem() {return str;}
	CString GetChannel(int i);
	BOOL CTreePanel::LButtonUp() {return flag_channel;}
};