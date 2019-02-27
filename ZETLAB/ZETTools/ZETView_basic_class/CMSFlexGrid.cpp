// CMSFlexGrid.cpp  : Definition of ActiveX Control wrapper class(es) created by Microsoft Visual C++


#include "stdafx.h"
#include "CMSFlexGrid.h"

/////////////////////////////////////////////////////////////////////////////
// CMSFlexGrid

IMPLEMENT_DYNCREATE(CMSFlexGrid, CWnd)

void CMSFlexGrid::setFont()
{
	//� ������������ ������� ����� - �� ���������
	font.DeleteObject();
	font.CreateFont(14,                        // nHeight
					0,                         // nWidth
					0,                         // nEscapement
					0,                         // nOrientation
					FW_NORMAL,                 // nWeight
					FALSE,                     // bItalic
					FALSE,                     // bUnderline
					0,                         // cStrikeOut
					ANSI_CHARSET,              // nCharSet
					OUT_DEFAULT_PRECIS,        // nOutPrecision
					CLIP_DEFAULT_PRECIS,       // nClipPrecision
					DEFAULT_QUALITY,           // nQuality
					DEFAULT_PITCH | FF_SWISS,  // nPitchAndFamily
					L"Curier New");           // lpszFacename
}

void CMSFlexGrid::Selection()
{
		//����������� ����� ���������� ������
		if ((get_FixedRows() > 0 && get_MouseRow() != NULL) ||
			  (get_FixedRows() == 0))
	    if ((get_FixedCols() > 0 && get_MouseCol() != NULL) ||
		      (get_FixedCols() == 0))
		{
			long row = get_MouseRow ();
			long col = get_MouseCol ();
			//if ( oldCol != col || oldRow != row)
			{
				//����������� ���������
				CPaintDC pdc(this);
				//pdc = GetDC();
				long Resolution = 1440/pdc.GetDeviceCaps(LOGPIXELSX);

				CRect grRect;
				GetWindowRect(grRect);
				//����������� ���������� ���������� ������
				CRect cellRect;
				cellRect.left = get_CellLeft()/Resolution - 3;
				cellRect.top = get_CellTop()/Resolution - 3;
				cellRect.right = cellRect.left + get_CellWidth()/Resolution;
				cellRect.bottom = cellRect.top + get_CellHeight()/Resolution;
				//���� ���� ��� ����� ��� ����������, ����� ������� ���
				//������������ �������� ��� ���������� ������, ��� ������� ��� �����
				if (m_text != NULL)
				{
					CString str_(L"");
					m_text->GetWindowTextW(str_);				//����� ����� ���� ����� �
					this->put_TextMatrix(oldRow,oldCol,str_);	//����������� ���� ����� ������
					m_text->DestroyWindow();	//���������� ����
					m_text = NULL;	
				}
				//������� ���� ����� ������
				m_text = new CRichEditCtrl;
				m_text->Create(WS_VISIBLE|WS_HSCROLL,CRect(0,0,0,0),this,2);

				m_text->SetFont(&font);	//���������� �����		
				//-------------------------
				//��������� ������� ������ � ������
				CString str(L"");
				str = get_TextMatrix(row,col);

				//���� ������ ���������� ���� �����
				if (str.GetLength() == 0)
					str.Empty(); //�� ����������� �� �������� �� ���������
				
				//������� ����� � ������ ����� �� ���� �����
				m_text->SetWindowTextW(str);
				m_text->SetFocus();

				//������ ����� �������
				m_text->SetSel(0,str.GetLength());
				//����������� ���� ����� ��������������� ��� ������������� �������

				CSize textExt = pdc.GetTextExtent(str);
				//if (textExt.cx < cellRect.Width())
					m_text->SetWindowPos(&wndTop,
										 cellRect.left,
										 cellRect.top,
										 cellRect.Width(),
										 cellRect.Height(),
										 SWP_SHOWWINDOW);
				/*else
					m_text->SetWindowPos(&wndTop,
										 cellRect.left,
										 cellRect.top,
										 textExt.cx,
										 cellRect.Height(),
										 SWP_SHOWWINDOW);*/

				//���������� ������ � ������� �� ����������
				oldRow = get_Row();
				oldCol = get_Col();
			}
		} else
			if (m_text != NULL)
			{
				CString str_(L"");
				m_text->GetWindowTextW(str_);				//����� ����� ���� ����� �
				this->put_TextMatrix(oldRow,oldCol,str_);	//����������� ���� ����� ������
				m_text->DestroyWindow();	//���������� ����
				m_text = NULL;	
			}

}

void CMSFlexGrid::Unselect()
{
	if (m_text != NULL)
	{
		CString str_(L"");
		m_text->GetWindowTextW(str_);				//����� ����� ���� ����� �
		this->put_TextMatrix(oldRow,oldCol,str_);	//����������� ���� ����� ������
		m_text->DestroyWindow();					//���������� ����
		m_text = NULL;	
	}
}

BEGIN_MESSAGE_MAP(CMSFlexGrid, CWnd)
	ON_WM_KILLFOCUS()
END_MESSAGE_MAP()

void CMSFlexGrid::OnKillFocus(CWnd* pNewWnd)
{
	CWnd::OnKillFocus(pNewWnd);

	if (m_text != NULL)
	{
		CString str_(L"");
		m_text->GetWindowTextW(str_);				//����� ����� ���� ����� �
		this->put_TextMatrix(oldRow,oldCol,str_);	//����������� ���� ����� ������
		m_text->DestroyWindow();	//���������� ����
		m_text = NULL;	
	}
	
}
