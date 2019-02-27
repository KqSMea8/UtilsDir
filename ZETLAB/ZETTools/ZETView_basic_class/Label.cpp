// CLabel.cpp : implementation file
//

#include "stdafx.h"
#include "Label.h"


// CLabel

IMPLEMENT_DYNAMIC(CLabel, CStatic)

CLabel::CLabel()
: text(_T("�����"))
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

	//��������� ������� ���������� Label
	CRect rc;
	GetClientRect(rc);
	//----------------------------------

	//����� �����, ������� �� ����� ��� � ����
	CString strText;
	GetWindowText(strText);

	//������� ��������, � ������� ����� ��������
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

			//��������� ���� ������� ����
			CBrush br(m_backColor);
			pDCMem->FillRect(rc,&br);
			br.DeleteObject();
			//---------------------------

			//��������� ����� ������ ������
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

			//������� �����
			pDCMem->DrawText(strText,rc,dwFlags);

			pDCMem->SetBkMode(nMode);
			pDCMem->SetTextColor(crText);

			//���������� ������, ������� ���� ����
			if (m_bTransparent)
				DrawTransparentBitmap(dc.m_hDC,
									  pDCMem->m_hDC,
									  rc,
									  m_backColor);	

			//�������� ���������� ��������� � �������� Label
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


   hdcTemp =  hdc_;   // �������� ������

   //  DPtoLP(hdcTemp, &ptSize, 1);      // ������������ �� ���������
										 // ���������� � ����������
										 // �����

   // ������ ��������� DC ��� �������� ��������� ������.
   hdcBack   = CreateCompatibleDC(hdc);
   hdcObject = CreateCompatibleDC(hdc);
   hdcMem    = CreateCompatibleDC(hdc);
   hdcSave   = CreateCompatibleDC(hdc);

   // ������ ������ ��� ������� DC.

   // ����������� DC
   bmAndBack   = CreateBitmap(ptSize.x, ptSize.y, 1, 1, NULL);

   // ����������� DC
   bmAndObject = CreateBitmap(ptSize.x, ptSize.y, 1, 1, NULL);

   bmAndMem    = CreateCompatibleBitmap(hdc, ptSize.x, ptSize.y);
   bmSave      = CreateCompatibleBitmap(hdc, ptSize.x, ptSize.y);

   // � ������ DC ������ ���� ������ ������ ������� ��� ��������
   // ��������.
   bmBackOld   = SelectObject(hdcBack, bmAndBack);
   bmObjectOld = SelectObject(hdcObject, bmAndObject);
   bmMemOld    = SelectObject(hdcMem, bmAndMem);
   bmSaveOld   = SelectObject(hdcSave, bmSave);

   // ������������� ����� ��������.
   SetMapMode(hdcTemp, GetMapMode(hdc));

   // ��������� ������, ���������� � ��������� �������, ��� ���
   // �� ����� ������.
   BitBlt(hdcSave, 0, 0, ptSize.x, ptSize.y, hdcTemp, 0, 0, SRCCOPY);

   // ������������� ������� ���� (� �������� DC) ��� ������,
   // ������� ����� �����������.
   cColor = SetBkColor(hdcTemp, cTransparentColor);

   // ������ ����� ��� ������� ���� ������ BitBlt �� ���������
   // ������� �� ����������� ������.
   BitBlt(hdcObject, 0, 0, ptSize.x, ptSize.y, hdcTemp, 0, 0,
          SRCCOPY);

   // ������������� ������� ���� ��������� DC ������� �
   // ������������ ����.
   SetBkColor(hdcTemp, cColor);

   // ������ �������� �����.
   BitBlt(hdcBack, 0, 0, ptSize.x, ptSize.y, hdcObject, 0, 0,
          NOTSRCCOPY);

   // �������� ��� �������� DC � ��������.
   BitBlt(hdcMem, 0, 0, ptSize.x, ptSize.y, hdc, xStart, yStart,
          SRCCOPY);

   // ����������� ����� �� �� �����, ��� ����� ������� ������.
   BitBlt(hdcMem, 0, 0, ptSize.x, ptSize.y, hdcObject, 0, 0, SRCAND);

   // ����������� ����� �� ���������� ������� �������.
   BitBlt(hdcTemp, 0, 0, ptSize.x, ptSize.y, hdcBack, 0, 0, SRCAND);

   // XOR-�� ������ � ����� �� �������� DC.
   BitBlt(hdcMem, 0, 0, ptSize.x, ptSize.y, hdcTemp, 0, 0, SRCPAINT);

   // �������� �� �����.
   BitBlt(hdc, xStart, yStart, ptSize.x, ptSize.y, hdcMem, 0, 0,
          SRCCOPY);

   // �������� ������������ ������ ������� � ������, ���������� �
   // ��������� �������.
   BitBlt(hdcTemp, 0, 0, ptSize.x, ptSize.y, hdcSave, 0, 0, SRCCOPY);

   // ������� ������� �� ������.
   DeleteObject(SelectObject(hdcBack, bmBackOld));
   DeleteObject(SelectObject(hdcObject, bmObjectOld));
   DeleteObject(SelectObject(hdcMem, bmMemOld));
   DeleteObject(SelectObject(hdcSave, bmSaveOld));

   // ������� DC �� ������.
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
