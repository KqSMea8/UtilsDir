// CLabel.cpp : implementation file
//

#include "stdafx.h"
#include "Label.h"


// CLabel

IMPLEMENT_DYNAMIC(CLabel, CStatic)

CLabel::CLabel()
: text(_T("Метка"))
, m_bTransparent(true)
{
	m_crText = RGB(0,0,0);
	m_backColor = RGB(255,255,255);
}

CLabel::~CLabel()
{
}


BEGIN_MESSAGE_MAP(CLabel, CStatic)
	ON_WM_PAINT()
	ON_WM_MOVE()
END_MESSAGE_MAP()



// CLabel message handlers



void CLabel::OnPaint()
{
	CPaintDC dc(this); // device context for painting

	DWORD dwFlags = 0;

	//вычисляем границы компонента Label
	CRect rc;
	GetClientRect(rc);
	//----------------------------------

	//берум текст, который до этого был в окне
	CString strText;
	GetWindowText(strText);

	//создаем контекст, в котором будем работать
	CBitmap bmp;
	CDC* pDCMem;
	pDCMem = new CDC;
	if (pDCMem->CreateCompatibleDC(&dc))
	{
		if (bmp.CreateCompatibleBitmap(&dc,rc.Width(),rc.Height()))
		{
			CBitmap* oldBitmap = (CBitmap*)pDCMem->SelectObject(&bmp);
			//------------------------------------------

			UINT nMode = pDCMem->SetBkMode(TRANSPARENT);

			COLORREF crText = pDCMem->SetTextColor(m_crText);
			CFont* oldFont = (CFont*)pDCMem->SelectObject(m_font);

			//формируем цвет заднего фона
			CBrush br(m_backColor);
			pDCMem->FillRect(rc,&br);
			br.DeleteObject();
			//---------------------------

			//формируем стиль вывода текста
			DWORD style = GetStyle();
			
			switch (style & SS_TYPEMASK)
			{
				case SS_RIGHT: 
					dwFlags = DT_RIGHT | DT_WORDBREAK; 
					break; 
				
				case SS_CENTER: 
					dwFlags = SS_CENTER | DT_WORDBREAK;
					break;

				case SS_LEFTNOWORDWRAP: 
					dwFlags = DT_LEFT; 
					break;

				default: // treat other types as left
					case SS_LEFT: 
						dwFlags = DT_LEFT | DT_WORDBREAK; 
						break;
			}	
			if (style & SS_CENTERIMAGE)
			{
				dwFlags = DT_CENTER;
				if (strText.Find(_T("\r\n")) == -1)
				{
					dwFlags |= DT_VCENTER;
					dwFlags |= DT_SINGLELINE; 
				}
			}
			//-----------------------------------

			//выводим текст
			pDCMem->DrawText(strText,rc,dwFlags);

			pDCMem->SetBkMode(nMode);
			pDCMem->SetTextColor(crText);

			//прозрачный битмап, убираем цвет фона
			if (m_bTransparent)
				DrawTransparentBitmap(dc.m_hDC,
									  pDCMem->m_hDC,
									  rc,
									  m_backColor);	

			//копируем содержимое контекста в контекст Label
			dc.BitBlt(0,0,rc.Width(),rc.Height(),pDCMem,0,0,SRCCOPY);

			pDCMem->SelectObject(oldFont);
			pDCMem->SelectObject(oldBitmap);
			pDCMem->DeleteDC();
		}
		bmp.DeleteObject();
		
	}
	delete pDCMem;
	pDCMem = nullptr;
}



void CLabel::SetColorText(COLORREF color)
{
	m_crText = color;
	
	UpdateSurface();
}

void CLabel::UpdateSurface()
{
	CRect (rc);
	GetWindowRect(rc);
	RedrawWindow();
}
void CLabel::SetColorBack(COLORREF backColor)
{
	m_backColor = backColor;

	UpdateSurface();
}

void CLabel::SetText(CString str)
{
	SetWindowText(str);
	UpdateSurface();
}

void CLabel::SetTransparent(bool isTransp)
{
	m_bTransparent = isTransp;
	//ModifyStyleEx(0,WS_EX_TRANSPARENT); // Fix for transparency
	UpdateSurface();
}

void CLabel::DrawTransparentBitmap(HDC hdc, HDC hdc_, CRect rect, COLORREF cTransparentColor)
   {

   long xStart = rect.left;
   long yStart = rect.top;
   CPoint      ptSize(rect.Width(),rect.Height()); 

   COLORREF   cColor;
   HGDIOBJ	  bmAndBack,bmAndObject,bmBackOld,bmSaveOld, bmMemOld, bmObjectOld;
   HBITMAP    bmAndMem, bmSave;
   HDC        hdcMem, hdcBack, hdcObject, hdcTemp, hdcSave;


   hdcTemp =  hdc_;   // Выбираем битмап

   //  DPtoLP(hdcTemp, &ptSize, 1);      // Конвертируем из координат
										 // устройства в логические
										 // точки

   // Создаём несколько DC для хранения временных данных.
   hdcBack   = CreateCompatibleDC(hdc);
   hdcObject = CreateCompatibleDC(hdc);
   hdcMem    = CreateCompatibleDC(hdc);
   hdcSave   = CreateCompatibleDC(hdc);

   // Создаём битмап для каждого DC.

   // Монохромный DC
   bmAndBack   = CreateBitmap(ptSize.x, ptSize.y, 1, 1, NULL);

   // Монохромный DC
   bmAndObject = CreateBitmap(ptSize.x, ptSize.y, 1, 1, NULL);

   bmAndMem    = CreateCompatibleBitmap(hdc, ptSize.x, ptSize.y);
   bmSave      = CreateCompatibleBitmap(hdc, ptSize.x, ptSize.y);

   // В каждом DC должен быть выбран объект битмапа для хранения
   // пикселей.
   bmBackOld   = SelectObject(hdcBack, bmAndBack);
   bmObjectOld = SelectObject(hdcObject, bmAndObject);
   bmMemOld    = SelectObject(hdcMem, bmAndMem);
   bmSaveOld   = SelectObject(hdcSave, bmSave);

   // Устанавливаем режим маппинга.
   SetMapMode(hdcTemp, GetMapMode(hdc));

   // Сохраняем битмап, переданный в параметре функции, так как
   // он будет изменён.
   BitBlt(hdcSave, 0, 0, ptSize.x, ptSize.y, hdcTemp, 0, 0, SRCCOPY);

   // Устанавливаем фоновый цвет (в исходном DC) тех частей,
   // которые будут прозрачными.
   cColor = SetBkColor(hdcTemp, cTransparentColor);

   // Создаём маску для битмапа путём вызова BitBlt из исходного
   // битмапа на монохромный битмап.
   BitBlt(hdcObject, 0, 0, ptSize.x, ptSize.y, hdcTemp, 0, 0,
          SRCCOPY);

   // Устанавливаем фоновый цвет исходного DC обратно в
   // оригинальный цвет.
   SetBkColor(hdcTemp, cColor);

   // Создаём инверсию маски.
   BitBlt(hdcBack, 0, 0, ptSize.x, ptSize.y, hdcObject, 0, 0,
          NOTSRCCOPY);

   // Копируем фон главного DC в конечный.
   BitBlt(hdcMem, 0, 0, ptSize.x, ptSize.y, hdc, xStart, yStart,
          SRCCOPY);

   // Накладываем маску на те места, где будет помещён битмап.
   BitBlt(hdcMem, 0, 0, ptSize.x, ptSize.y, hdcObject, 0, 0, SRCAND);

   // Накладываем маску на прозрачные пиксели битмапа.
   BitBlt(hdcTemp, 0, 0, ptSize.x, ptSize.y, hdcBack, 0, 0, SRCAND);

   // XOR-им битмап с фоном на конечном DC.
   BitBlt(hdcMem, 0, 0, ptSize.x, ptSize.y, hdcTemp, 0, 0, SRCPAINT);

   // Копируем на экран.
   BitBlt(hdc, xStart, yStart, ptSize.x, ptSize.y, hdcMem, 0, 0,
          SRCCOPY);

   // Помещаем оригинальный битмап обратно в битмап, переданный в
   // параметре функции.
   BitBlt(hdcTemp, 0, 0, ptSize.x, ptSize.y, hdcSave, 0, 0, SRCCOPY);

   // Удаляем битмапы из памяти.
   DeleteObject(SelectObject(hdcBack, bmBackOld));
   DeleteObject(SelectObject(hdcObject, bmObjectOld));
   DeleteObject(SelectObject(hdcMem, bmMemOld));
   DeleteObject(SelectObject(hdcSave, bmSaveOld));

   // Удаляем DC из памяти.
   DeleteDC(hdcMem);
   DeleteDC(hdcBack);
   DeleteDC(hdcObject);
   DeleteDC(hdcSave);
   DeleteDC(hdcTemp);

DeleteObject(bmAndBack);
DeleteObject(bmAndObject);
DeleteObject(bmBackOld);
DeleteObject(bmSaveOld);
DeleteObject(bmMemOld);
DeleteObject(bmObjectOld);
DeleteObject(bmAndMem);
DeleteObject(bmSave);
//DeleteObject(hdcMem);
//DeleteObject(hdcBack);
//DeleteObject(hdcObject);
//DeleteObject(hdcTemp);
//DeleteObject(hdcSave);
   }

   void CLabel::OnMove(int x, int y)
   {
	   CStatic::OnMove(x, y);

	   UpdateSurface();
   }

   void CLabel::ReconstructFont(CFont* font)
   {
	m_font = font;
	UpdateSurface();
   }
