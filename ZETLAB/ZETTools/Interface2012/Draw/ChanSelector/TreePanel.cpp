#include "stdafx.h"
#include "C:\ZETTools\Interface2012\Draw\ChanSelector\TreePanel.h"
#include "math.h"
#include <vector>
#include "C:\ZETTools\Interface2012\Custom\Custom_ZETi12.h"

#define fontHeight 10

std::vector <ChannelS> Channel;
std::vector <DeviceS> Device;
std::vector <CompS> Comp;
std::vector <ServerS> Server;

CDSRV* m_server;
long NeedDelta;
long scrollRange;
GdiplusStartupInput gdiplusStartupInput; 
ULONG_PTR gdiplusToken; 
float device_caps;
//CString startTime;

IMPLEMENT_DYNCREATE(CTreePanel, CScrollView)
CTreePanel::CTreePanel()
: isPaused(false)
, backBuffer(NULL)
, format_name(NULL)
{
	flag_channel = false;
	CurrentChannel = 0;
}

CTreePanel::~CTreePanel()
{
}

BEGIN_MESSAGE_MAP(CTreePanel, CScrollView)
	ON_WM_LBUTTONUP()
	ON_WM_LBUTTONDBLCLK()
	ON_WM_LBUTTONDOWN()
	ON_WM_SIZE()
	ON_WM_RBUTTONDOWN()
END_MESSAGE_MAP()

void CTreePanel::OnInitialUpdate()
{
	CScrollView::OnInitialUpdate();

	GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL);
	
	//цвет фона
	
	Color col;
	col.SetFromCOLORREF(RGB(0,0,0));
	brush_back = new Gdiplus::SolidBrush(col);
	col.SetFromCOLORREF(RGB(66,66,66));
	brush_selectedServer = new Gdiplus::SolidBrush(col);
	col.SetFromCOLORREF(RGB(66,66,66));
	brush_selectedDevice = new Gdiplus::SolidBrush(col);
	col.SetFromCOLORREF(RGB(240,240,240));
	brush_selectedChannel = new Gdiplus::SolidBrush(col);
	col.SetFromCOLORREF(RGB(200,200,200));
	brush_Channel = new Gdiplus::SolidBrush(col);
	fontServer = new Gdiplus::Font(L"System", 10, FontStyleBold);
	fontDevice = new Gdiplus::Font(L"System", 10, FontStyleBold);
	fontChannel = new Gdiplus::Font(L"System", 9, FontStyleRegular);
	col.SetFromCOLORREF(RGB(225,225,225));
	brush_litera = new SolidBrush(col);
	col.SetFromCOLORREF(RGB(0,0,0));
	brush_litera_inactive = new SolidBrush(col);
	col.SetFromCOLORREF(RGB(255,255,255));
	pen_circle = new Pen(Gdiplus::Color(col), 1);
	col.SetFromCOLORREF(RGB(66,66,66));
	pen_frame = new Pen(Gdiplus::Color(col), 1);
	col.SetFromCOLORREF(RGB(187,187,187));
	pen_line = new Pen(Gdiplus::Color(col), 1);

	//считываем из ресурсов единицу измерения
	stringSeconds.LoadString(IDS_SECOND);
	stringMinutes.LoadString(IDS_MINUTE);
	stringHours.LoadString(IDS_HOUR);
	stringDays.LoadString(IDS_DAY);
	//считываем из ресурсов "Глобальное время ZETServer"
	stringGlobal.LoadString(IDS_GLOBALTIME);
	//считываем из ресурсов "Время старта ZETServer"
	stringStart.LoadString(IDS_START);
	//считываем из ресурсов "Количество каналов ZETServer"
	stringQuantity.LoadString(IDS_QUANTITY);
	scrollPosition = GetScrollPosition().y;
	OnSize(0, 0, 0);
}

void CTreePanel::OnSize(UINT nType, int cx, int cy)
{
	CScrollView::OnSize(nType, cx, cy);

	CSize sizeTotal = CSize(4096 * device_caps, 20 * scrollRange * device_caps + 10 * device_caps);
	//CSize sizePage = CSize(4096 * device_caps, 20 * scrollRange * device_caps + 10 * device_caps);
	CSize sizePage = CSize(100,200);
	CSize sizeLine = CSize(20 * device_caps, 20 * device_caps);
	
	if (NeedDelta > 0)
	{
		sizeTotal.cx = 100 * device_caps;
		sizeTotal.cy = 20 * scrollRange * device_caps + 10 * device_caps;
	}
	else
	{
		sizeTotal.cx  = 100 * device_caps;
		sizeTotal.cy = 50 * device_caps;
	}

	SetScrollSizes(MM_TEXT, sizeTotal, sizePage, sizeLine);
}

void CTreePanel::OnDraw(CDC* pDC)
{
#ifdef _GDIPLUS
	if (m_server != NULL)
	{
		CRect rcBounds;
		GetClientRect(rcBounds);
		rcBounds.bottom = GetScrollPosition().y + rcBounds.Height();
		Graphics graphics(pDC->m_hDC);

		if (backBuffer == NULL)
			backBuffer = new Bitmap(rcBounds.right, rcBounds.bottom, &graphics);
		else
		{
			if (scrollPosition != GetScrollPosition().y)
			{
				scrollPosition = GetScrollPosition().y;
				backBuffer->operator delete;
				delete backBuffer;
				backBuffer = NULL;
				backBuffer = new Bitmap(rcBounds.right, rcBounds.bottom, &graphics);
			}
		}
		Graphics temp(backBuffer);

		OnDrawMy(pDC, temp, rcBounds);
		graphics.DrawImage(backBuffer, 0, 0, 0, 0, rcBounds.right, rcBounds.bottom, UnitPixel);
	}
#endif
}

void CTreePanel::OnDrawMy(CDC* pdc, Graphics& gr, CRect rcBounds)
{
#ifdef _GDIPLUS	
	CString temp;
	long chanQuan = m_server->QuanChan();
	CString stringChan = L"";
	stringChan.Format(L"%d", chanQuan);
	long addings = 0;
	long chanum = 1;
	
	if (format_name == NULL)
	{
		format_name = new StringFormat;
		format_name->SetAlignment(StringAlignmentNear);
		format_name->SetLineAlignment(StringAlignmentCenter);
		//StringFormat format_name;
	}
	//заполняем фон
	
	//gr.FillRectangle(brush_back, float(-1), float(-1), float(rcBounds.Width() + 2), float(rcBounds.Height()) + 2);
	gr.FillRectangle(brush_selectedDevice, 0, 0, rcBounds.Width(), rcBounds.Height());
	gr.FillRectangle(brush_back, 1, 1, rcBounds.Width()-2, rcBounds.Height()-2);
	//область вывода количества каналов

	OnDrawServers(gr, rcBounds, &addings);

	CRect tempRect;
	GetClientRect(tempRect);
	long currSizeV = tempRect.Height();
	long needSizeV = long(10.f * device_caps + (fontHeight + 10.f) * float(addings) * device_caps);
	NeedDelta = needSizeV - currSizeV;
	scrollRange = addings;
#endif
	CString temp;
	long chanQuan = m_server->QuanChan();
	CString stringChan = L"";
	stringChan.Format(L"%d", chanQuan);
	long addings = 0;
	long chanum = 1;

	if (format_name == NULL)
	{
		format_name = new StringFormat;
		format_name->SetAlignment(StringAlignmentNear);
		format_name->SetLineAlignment(StringAlignmentCenter);
		//StringFormat format_name;
	}
	//заполняем фон

	//gr.FillRectangle(brush_back, float(-1), float(-1), float(rcBounds.Width() + 2), float(rcBounds.Height()) + 2);
	gr.FillRectangle(brush_selectedDevice, 0, 0, rcBounds.Width(), rcBounds.Height());
	gr.FillRectangle(brush_back, 1, 1, rcBounds.Width()-2, rcBounds.Height()-2);
	//область вывода количества каналов

	OnDrawServers(gr, rcBounds, &addings);

	CRect tempRect;
	GetClientRect(tempRect);
	long currSizeV = tempRect.Height();
	long needSizeV = long(10.f * device_caps + (fontHeight + 10.f) * float(addings) * device_caps);
	NeedDelta = needSizeV - currSizeV;
	scrollRange = addings;
}

void CTreePanel::OnDrawServers(Graphics &gr, CRect rcBounds, long* addings)
{
#ifdef _GDIPLUS	
	CString temp;
	bool thisServerOnly = false;
	CString my;
	my.LoadString(IDS_MYCOMPUTER);

	if (Server.size() == 1)
	{
		if (Server[0].sName == my)
		{
			thisServerOnly = true;
		}
	}
	long serverQuantity = Server.size();
	long startDrawing = 20;
	if (thisServerOnly)
		startDrawing = 0;
	for (int n = 0; n < serverQuantity; n++)
	{
		if (!thisServerOnly)
		{
			if (!isPaused)
			{
				long lComp = Server[n].comp[0];
				long lDevice = Comp[lComp].device[0];
				long lChannel = Device[lDevice].channel[0];
			}

			//область рисования плюсика/минусика дерева cерверов
			//gr.DrawRectangle(pen_circle, float(5 * device_caps), float(10 * device_caps + (fontHeight + 10) * (*addings) * device_caps), float(10 * device_caps), float(10 * device_caps));
			Server[n].rCheckBoxRect = CRect(CPoint(5 * device_caps, 10 * device_caps + (fontHeight + 10) * (*addings) * device_caps), CSize(10 * device_caps, 10 * device_caps));
			//область вывода имени сервера
			RectF bounds(float(20 * device_caps), float(5 * device_caps + ((fontHeight + 10) * (*addings) + 1) * device_caps), float(rcBounds.Width() - 10/*50 * device_caps*/), float(16 * device_caps));
			Server[n].rServerRect = CRect(CPoint(20 * device_caps, 5 * device_caps + ((fontHeight + 10) * (*addings) + 1) * device_caps), CSize(rcBounds.Width() - 50 * device_caps, 16 * device_caps));
			//выделяем строку с сервером
			gr.FillRectangle(brush_selectedServer, bounds);
			//рисуем плюсик/минусик
			gr.DrawLine(pen_circle, Point(7 * device_caps, 10 * device_caps + (fontHeight + 10) * (*addings) * device_caps + 5 * device_caps), Point(13 * device_caps, 10 * device_caps + (fontHeight + 10) * (*addings) * device_caps + 5 * device_caps));
			if (!Server[n].bExpanded)
				gr.DrawLine(pen_circle, Point(10 * device_caps, 12 * device_caps + (fontHeight + 10) * (*addings) * device_caps), Point(10 * device_caps, 10 * device_caps + (fontHeight + 10) * (*addings) * device_caps + 8 * device_caps));

			
		
			//загрузка иконки сервера
			if (Server[n].hBitmap == NULL)
			{
				Server[n].hBitmap = new Bitmap(Server[n].hIcon);
				//bm->operator delete;
				//delete bm;
			}
			if (Server[n].hBitmap != NULL)
				gr.DrawImage(Server[n].hBitmap, RectF(bounds.X, bounds.Y , 20 * device_caps, 20 * device_caps), 0, 0, 32, 32, UnitPixel, &attr);

			bounds.X += 24 * device_caps;
				//выводим имя сервера
				temp.Format(L"%s", Server[n].sName);
				gr.DrawString(temp, -1, fontServer, bounds, format_name, brush_back);
			(*addings)++;
		}

		if (Server[n].bExpanded)
		{
			
			gr.FillRectangle(brush_litera,RectF(float(7 * device_caps),float(5 * device_caps + ((fontHeight + 10) * (*addings) + 1) * device_caps),2,2 ));
			OnDrawComps(gr, rcBounds, addings, startDrawing, n);
		}
	}
#endif
}

void CTreePanel::OnDrawComps(Graphics &gr, CRect rcBounds, long* addings, long startDrawing, long n)
{
#ifdef _GDIPLUS
	CString temp;
	long compQuantity = Server[n].nCounter;
	for (int i = 0; i < compQuantity; i++)
	{
		long compIndex = Server[n].comp[i];

		//область рисования плюсика/минусика дерева cерверов
		//gr.DrawRectangle(pen_circle, float((startDrawing + 5) * device_caps), float(10 * device_caps + (fontHeight + 10) * (*addings) * device_caps), float(10 * device_caps), float(10 * device_caps));
		Comp[compIndex].rCheckBoxRect = CRect(CPoint((startDrawing + 5) * device_caps, 10 * device_caps + (fontHeight + 10) * (*addings) * device_caps), CSize(10 * device_caps, 10 * device_caps));
		
		//область вывода имени компьютера
		RectF bounds(float((startDrawing + 5) * device_caps), float(5 * device_caps + ((fontHeight + 10) * (*addings) + 1) * device_caps), float(rcBounds.Width() - 5 - ((startDrawing + 5) * device_caps)), float(16 * device_caps));
		Comp[compIndex].rCompRect = CRect(CPoint((startDrawing + 20) * device_caps, 5 * device_caps + ((fontHeight + 10) * (*addings) + 1) * device_caps), CSize(rcBounds.Width() - (startDrawing + 50) * device_caps, 16 * device_caps));
		//рисуем плюсик/минусик
		gr.FillRectangle(brush_selectedServer, bounds);
		gr.DrawLine(pen_circle, Point((startDrawing + 7) * device_caps, 10 * device_caps + (fontHeight + 10) * (*addings) * device_caps + 5 * device_caps), Point((startDrawing + 13) * device_caps, 10 * device_caps + (fontHeight + 10) * (*addings) * device_caps + 5 * device_caps));
		if (!Comp[compIndex].bExpanded)
			gr.DrawLine(pen_circle, Point((startDrawing + 10) * device_caps, 12 * device_caps + (fontHeight + 10) * (*addings) * device_caps), Point((startDrawing + 10) * device_caps, 10 * device_caps + (fontHeight + 10) * (*addings) * device_caps + 8 * device_caps));
		//загрузка иконки компьютера
		if (Comp[compIndex].hBitmap == NULL)
		{
			Comp[compIndex].hBitmap = new Bitmap(Comp[compIndex].hIcon);
			//bm->operator delete;
			//delete bm;
		}
		if (Comp[compIndex].hBitmap != NULL)
			gr.DrawImage(Comp[compIndex].hBitmap, RectF(bounds.X, bounds.Y , 20 * device_caps, 20 * device_caps), 0, 0, 32, 32, UnitPixel, &attr);

		bounds.X += 24 * device_caps;
			temp.Format(L"%s", Comp[compIndex].sName);
			gr.DrawString(temp, -1, fontServer, bounds, format_name, brush_litera);
		//инкрементируем отступ
		(*addings)++;

		//если список устройств компьютера развернут, то выводим его
		if (Comp[compIndex].bExpanded)
		{
			int down = 0;
			while (down < 20/2)
			{
				gr.FillRectangle(brush_litera,RectF(float((startDrawing + 10) * device_caps),float(5 * device_caps + ((fontHeight + 10) * (*addings) + 1) * device_caps) + down,2,2 ));
				gr.FillRectangle(brush_litera,RectF(float((startDrawing + 10) * device_caps) + down,float(10 * device_caps + (fontHeight + 10) * (*addings) * device_caps + 5 * device_caps) ,2,2 ));
				down = down + 3;
			}
			OnDrawDevices(gr, rcBounds, addings, startDrawing + 20, compIndex);
		}
	}
#endif
}

void CTreePanel::OnDrawDevices(Graphics &gr, CRect rcBounds, long* addings, long startDrawing, long compIndex)
{
#ifdef _GDIPLUS
	CString temp;
	long deviceQuantity = Comp[compIndex].nCounter;
	for (int j = 0; j < deviceQuantity; j++)
	{
		long deviceIndex = Comp[compIndex].device[j];
		
		//область рисования плюсика/минусика дерева устройств
		//gr.DrawRectangle(pen_circle, float((startDrawing + 5) * device_caps), float(10 * device_caps + (fontHeight + 10) * (*addings) * device_caps), float(10 * device_caps), float(10 * device_caps));
		Device[deviceIndex].rCheckBoxRect = CRect(CPoint((startDrawing + 5) * device_caps, 10 * device_caps + (fontHeight + 10) * (*addings) * device_caps), CSize(10 * device_caps, 10 * device_caps));
		//область вывода имени устройства
		RectF bounds(float((startDrawing + 5) * device_caps), float(5 * device_caps + ((fontHeight + 10) * (*addings) + 1) * device_caps), float(rcBounds.Width() - 5 - ((startDrawing + 5) * device_caps)), float(16 * device_caps));
		Device[deviceIndex].rDeviceRect = CRect(CPoint((startDrawing + 20) * device_caps, 5 * device_caps + ((fontHeight + 10) * (*addings) + 1) * device_caps), CSize(rcBounds.Width() - (startDrawing + 50) * device_caps, 16 * device_caps));
		//выделяем строку с устройством
		gr.FillRectangle(brush_selectedDevice, bounds);
		//рисуем плюсик/минусик
		gr.DrawLine(pen_circle, Point((startDrawing + 7) * device_caps, 10 * device_caps + (fontHeight + 10) * (*addings) * device_caps + 5 * device_caps), Point((startDrawing + 13) * device_caps, 10 * device_caps + (fontHeight + 10) * (*addings) * device_caps + 5 * device_caps));
		if (!Device[deviceIndex].bExpanded)
			gr.DrawLine(pen_circle, Point((startDrawing + 10) * device_caps, 12 * device_caps + (fontHeight + 10) * (*addings) * device_caps), Point((startDrawing + 10) * device_caps, 10 * device_caps + (fontHeight + 10) * (*addings) * device_caps + 8 * device_caps));

		
		
		//загрузка иконки устройства
		if (Device[deviceIndex].hBitmap == NULL)
		{
			Device[deviceIndex].hBitmap = new Bitmap(Device[deviceIndex].hIcon);
			//bm->operator delete;
			//delete bm;
		}
		if (Device[deviceIndex].hBitmap != NULL)
			gr.DrawImage(Device[deviceIndex].hBitmap, RectF(bounds.X, bounds.Y , 20 * device_caps, 20 * device_caps), 0, 0, 32, 32, UnitPixel, &attr);
						
		bounds.X += 24 * device_caps;
		
			//выводим имя устройства
			temp.Format(L"%s", Device[deviceIndex].sName);
			gr.DrawString(temp, -1, fontDevice, bounds, format_name, brush_litera);

		//инкрементируем отступ
		(*addings)++;
		//если список каналов устройства развернут, то выводим его
		if (Device[deviceIndex].bExpanded)
		{
			int down = 0;
			while (down < 20/2)
			{
				gr.FillRectangle(brush_litera,RectF(float((startDrawing + 10) * device_caps),float(5 * device_caps + ((fontHeight + 10) * (*addings) + 1) * device_caps) + down,2,2 ));
				gr.FillRectangle(brush_litera,RectF(float((startDrawing + 10) * device_caps) + down,float(10 * device_caps + (fontHeight + 10) * (*addings) * device_caps + 5 * device_caps) ,2,2 ));
				down = down + 3;
			}
			OnDrawChannels(gr, rcBounds, addings, startDrawing + 20, deviceIndex);
		}
	}
#endif
}

void CTreePanel::OnDrawChannels(Graphics &gr, CRect rcBounds, long* addings, long startDrawing, long deviceIndex)
{
#ifdef _GDIPLUS
	CString temp;
	long channelQuantity = Device[deviceIndex].nCounter;
	//вывод каналов устройства
	
	
	for (int k = 0; k < channelQuantity; k++)
	{
		long channelIndex = Device[deviceIndex].channel[k];
								
		if (!isPaused)
		{
			//узнаем статус канала
			Channel[channelIndex].lStatus = m_server->GetStatus(Channel[channelIndex].lNumber);
			Device[deviceIndex].lStatus = Channel[channelIndex].lStatus;
			m_server->IdChan(Channel[channelIndex].lNumber, &Channel[channelIndex].lID);
			//m_server->GetWatermarkFlag(Channel[channelIndex].lNumber, &Channel[channelIndex].lWatermarkFlag);
			m_server->NumFileUsed(Channel[channelIndex].lNumber);
		}
							
		//область вывода имени канала
		RectF bounds(float((startDrawing + 5) * device_caps), float(5 * device_caps + (fontHeight + 10) * (*addings) * device_caps), float(rcBounds.Width() -  ((startDrawing + 5) * device_caps) - 5), float(20 * device_caps));
		Channel[channelIndex].rChanRect = CRect(CPoint((startDrawing + 24) * device_caps, 5 * device_caps + (fontHeight + 10) * (*addings) * device_caps), CSize(rcBounds.Width() - (startDrawing + 30) * device_caps, 15 * device_caps));
					
			//выделяем цветом
		if (k == CurrentChannel)
			gr.FillRectangle(brush_Channel, bounds);
		else
			gr.FillRectangle(brush_selectedChannel, bounds);
			gr.DrawLine(pen_line, float((startDrawing ) * device_caps)+10, float(5 * device_caps + (fontHeight + 10) * (*addings) * device_caps) + float(19 * device_caps) , float(rcBounds.Width() - 10), float(5 * device_caps + (fontHeight + 10) * (*addings) * device_caps) + float(19 * device_caps));
			//выводим порядковый номер и имя канала

			temp.Format(L"%d -  %s", Channel[channelIndex].lNumber + 1, Channel[channelIndex].sName);
			gr.DrawString(temp, -1, fontChannel, bounds, format_name, brush_litera_inactive);
		(*addings)++;
	}
#endif
}

void CTreePanel::OnLButtonUp(UINT nFlags, CPoint point)
{
	long offset = GetScrollPosition().y;
	long channelQuantity = Channel.size();
	long deviceQuantity = Device.size();
	long compQuantity = Comp.size();
	long serverQuantity = Server.size();
	
	//проверка попадания на плюсик/минусик сервера
	for (int i = 0; i < serverQuantity; i++)
	{
		CRect tempRect = Server[i].rCheckBoxRect;
		tempRect.top -= offset;
		tempRect.bottom -= offset;
		if (PtInRect(tempRect, point))
		{	
			Server[i].bExpanded = !Server[i].bExpanded;
		}
	}

	//проверка попадания на плюсик/минусик компьютера
	for (int i = 0; i < compQuantity; i++)
	{
		CRect tempRect = Comp[i].rCheckBoxRect;
		tempRect.top -= offset;
		tempRect.bottom -= offset;
		if (PtInRect(tempRect, point))
		{	
			Comp[i].bExpanded = !Comp[i].bExpanded;
		}
	}

	//проверка попадания на плюсик/минусик модуля
	for (int i = 0; i < deviceQuantity; i++)
	{
		CRect tempRect = Device[i].rCheckBoxRect;
		tempRect.top -= offset;
		tempRect.bottom -= offset;
		if (PtInRect(tempRect, point))
		{	
			Device[i].bExpanded = !Device[i].bExpanded;
		}
	}

	long selDevice = -1;
	long selComp = -1;
	long selServer = -1;

	bool inChannel = false;
	//проверка попадания на канал/устройство/компьютер/сервер
	for (int i = 0; i < channelQuantity; i++)
	{
		CRect tempRect = Channel[i].rChanRect;
		tempRect.top -= offset;
		tempRect.bottom -= offset;
		long device = FindDevice(Channel[i].sDeviceName, Channel[i].sCompName, Channel[i].sServerName) - 1;
		long comp = FindComp(Channel[i].sCompName, Channel[i].sServerName) - 1;
		long server = FindServer(Channel[i].sServerName) - 1;
		if (PtInRect(tempRect, point) && Device[device].bExpanded && Comp[comp].bExpanded && Server[server].bExpanded)
		{	
			Channel[i].bSelected = true;
			Device[device].bSelected = true;
			selDevice = device;
			Comp[comp].bSelected = true;
			selComp = comp;
			Server[server].bSelected = true;
			selServer = server;
			inChannel = true;
			if (Channel[i].bSelected)
			{
				str = Channel[i].sName;
				if (CurrentChannel != i)
				{
					CurrentChannel = i;
					GetParent()->GetWindowRect(rect);
					for (int j = 1; j<=10; j++)
					{	
						GetParent()->SetWindowPos(&CWnd::wndTopMost,
							rect.left, rect.top, rect.Width(), rect.Height()/j,				
							SWP_NOACTIVATE | SWP_SHOWWINDOW);
						Sleep(50);
					}
					
					GetParent()->SetWindowPos(&CWnd::wndBottom,
						rect.left, rect.bottom, 0, 0,
						SWP_NOACTIVATE | SWP_SHOWWINDOW);
				}
			}
		}
		else
		{
			Channel[i].bSelected = false;
			if (selDevice != device)
			{
				Device[device].bSelected = false;
			}
			if (selComp != comp)
			{
				Comp[comp].bSelected = false;
			}
			if (selServer != server)
			{
				Server[server].bSelected = false;
			}
		}
	}

	bool inDevice = false;
	if (!inChannel)
	{
		for (int i = 0; i < deviceQuantity; i++)
		{
			CRect tempRect = Device[i].rDeviceRect;
			tempRect.top -= offset;
			tempRect.bottom -= offset;
			long comp = FindComp(Device[i].sCompName, Device[i].sServerName) - 1;
			long server = FindServer(Device[i].sServerName) - 1;
			if (PtInRect(tempRect, point) && Comp[comp].bExpanded && Server[server].bExpanded)
			{	
				Device[i].bSelected = true;
				Comp[comp].bSelected = true;
				selComp = comp;
				Server[server].bSelected = true;
				selServer = server;
				inDevice = true;
			}
			else
			{
				Device[i].bSelected = false;
				if (selComp != comp)
				{
					Comp[comp].bSelected = false;
				}
				if (selServer != server)
				{
					Server[server].bSelected = false;
				}
			}
		}
	}

	bool inComp = false;
	if (!inDevice && !inChannel)
	{
		for (int i = 0; i < compQuantity; i++)
		{
			CRect tempRect = Comp[i].rCompRect;
			tempRect.top -= offset;
			tempRect.bottom -= offset;
			long server = FindServer(Comp[i].sServerName) - 1;
			if (PtInRect(tempRect, point) && Server[server].bExpanded)
			{	
				Comp[i].bSelected = true;
				Server[server].bSelected = true;
				selServer = server;
				inComp = true;
			}
			else
			{
				Comp[i].bSelected = false;
				if (selServer != server)
				{
					Server[server].bSelected = false;
				}
			}
		}
	}

	if (!inComp && !inDevice && !inChannel)
	{
		for (int i = 0; i < serverQuantity; i++)
		{
			CRect tempRect = Server[i].rServerRect;
			tempRect.top -= offset;
			tempRect.bottom -= offset;
			if (PtInRect(tempRect, point))
			{	
				Server[i].bSelected = true;
			}
			else
			{
				Server[i].bSelected = false;
			}
		}
	}


	isPaused = false;
	Invalidate(false);
	//GetParent()->CloseWindow();
	this->GetParent()->KillTimer(1);
	this->GetParent()->SetTimer(1, 200, NULL);


	CScrollView::OnLButtonUp(nFlags, point);
}
void CTreePanel::OnLButtonDblClk(UINT nFlags, CPoint point)
{
	long offset = GetScrollPosition().y;
	long deviceQuantity = Device.size();
	long compQuantity = Comp.size();
	long serverQuantity = Server.size();

	bool inDevice = false;
	for (int i = 0; i < deviceQuantity; i++)
	{
		CRect tempRect = Device[i].rDeviceRect;
		tempRect.top -= offset;
		tempRect.bottom -= offset;
		long comp = FindComp(Device[i].sCompName, Device[i].sServerName) - 1;
		long server = FindServer(Device[i].sServerName) - 1;
		if (PtInRect(tempRect, point) && Comp[comp].bExpanded && Server[server].bExpanded)
		{	
			Device[i].bExpanded = !Device[i].bExpanded;
			inDevice = true;
		}
	}

	bool inComp = false;
	if (!inDevice)
	{
		for (int i = 0; i < compQuantity; i++)
		{
			CRect tempRect = Comp[i].rCompRect;
			tempRect.top -= offset;
			tempRect.bottom -= offset;
			long server = FindServer(Comp[i].sServerName) - 1;
			if (PtInRect(tempRect, point) && Server[server].bExpanded)
			{	
				Comp[i].bExpanded = !Comp[i].bExpanded;
				inComp = true;
			}
		}
	}

	if (!inDevice && !inComp)
	{
		for (int i = 0; i < serverQuantity; i++)
		{
			CRect tempRect = Server[i].rServerRect;
			tempRect.top -= offset;
			tempRect.bottom -= offset;
			if (PtInRect(tempRect, point))
			{	
				Server[i].bExpanded = !Server[i].bExpanded;
			}
		}
	}
	
	this->GetParent()->KillTimer(1);
	this->GetParent()->SetTimer(1, 200, NULL);

	CScrollView::OnLButtonDblClk(nFlags, point);
}

void CTreePanel::OnLButtonDown(UINT nFlags, CPoint point)
{
	isPaused = true;
	CScrollView::OnLButtonDown(nFlags, point);
}

long CTreePanel::FindServer(CString serverName)
{
	for (UINT i = 0; i < Server.size(); i++)
	{
		if (Server[i].sName == serverName)
			return i + 1;
	}
	return 0;
}

long CTreePanel::FindComp(CString compName, CString serverName)
{
	for (UINT i = 0; i < Comp.size(); i++)
	{
		if (Comp[i].sName == compName && Comp[i].sServerName == serverName)
			return i + 1;
	}
	return 0;
}

long CTreePanel::FindDevice(CString deviceName, CString compName, CString serverName)
{
	for (UINT i = 0; i < Device.size(); i++)
	{
		if (Device[i].sName == deviceName && Device[i].sCompName == compName && Device[i].sServerName == serverName)
			return i + 1;
	}
	return 0;
}