//------------------------------------------------------------------------------
// AutoScaleXY.cpp : ���� ����������
// ����������, ��� "����", �������� �.�.
//------------------------------------------------------------------------------
#include "stdafx.h"
#include <algorithm>
#include "AutoScaleXY.h"
//------------------------------------------------------------------------------
CAutoScaleXY::CAutoScaleXY(CWnd * parent)
	: m_bInit(false)
	, m_cy_all (0)	// ������ ���������� ������ ����
	, m_cx_all  (0)	// ������ ���������� ������ ����
	, m_pParent(nullptr)
{
	m_rectReSize.left =
	m_rectReSize.top = LONG_MAX;
	m_rectReSize.right =
	m_rectReSize.bottom = 0;

	Create(parent);
}
//------------------------------------------------------------------------------
CAutoScaleXY::~CAutoScaleXY()
{
	m_list_ReSize.clear();
	m_list_LeftTop.clear();
	m_list_LeftBottom.clear();
	m_list_RightTop_Src.clear();
	m_list_RightTop.clear();
	m_list_RightBottom.clear();
	m_list_WithMaster.clear();
}
//------------------------------------------------------------------------------
bool CAutoScaleXY::Create(CWnd * parent)
{
	bool ret = parent != nullptr;
	if (ret)
	{
		RECT rect;
		m_pParent = parent;
		m_pParent->GetClientRect(&rect);		// ���� �����������!!!
		m_cy_all = rect.bottom - rect.top;
		m_cx_all = rect.right - rect.left;
	}
	return ret;
}
//------------------------------------------------------------------------------
bool CAutoScaleXY::IsEmpty()
{
	return  m_list_ReSize.empty() && m_list_LeftTop.empty() &&
			m_list_LeftBottom.empty() && m_list_RightTop.empty() &&
			m_list_RightBottom.empty() && m_list_WithMaster.empty();
}
//------------------------------------------------------------------------------
bool CAutoScaleXY::AutoScale(int cx, int cy)
{
	bool ret = (m_pParent != nullptr) && m_bInit && ((cx != 0) || (cy != 0));
	if (!ret)
		return ret;

	int dx = cx - m_cx_all;		// ���������� ������ ����
	int dy = cy - m_cy_all;		// ��������� ������ ����

	// ������ m_ListLeftTop ������� � ������ �������� �� ����
	if (!m_list_LeftTop.empty())
	{
		for (auto & it : m_list_LeftTop)
		{
			if ((it.flags & tfRight) && (it.flags & tfBottom))
				ret = ret && ShiftControl(it, dx, dy, false, false);
			else if (it.flags & tfRight)	// ������ ������ �� ������ ��������
				ret = ret && ShiftControl(it, dx, 0, false, false);
			else if (it.flags & tfBottom)
				ret = ret && ShiftControl(it, 0, dy, false, false);
		}
	}
	// ������ m_ListLeftBottom �������� ������ �� ���������
	if (!m_list_LeftBottom.empty())
	{
		for (auto & it : m_list_LeftBottom)
		{
			if (it.flags & tfRight)	// ������ ������ �� ������ ��������
				ret = ret && ShiftControl(it, dx, dy, false, true);
			else
				ret = ret && ShiftControl(it, 0, dy);
		}
	}
	// ������ m_ListRightTop �������� ������ �� �����������
	if (!m_list_RightTop.empty())
	{
		for (auto & it : m_list_RightTop)
		{
			if (it.flags & tfBottom)	// ������ ������ �� ������ ��������
				ret = ret && ShiftControl(it, dx, dy, true, false);
			else
				ret = ret && ShiftControl(it, dx, 0);
		}
	}
	// ������ m_ListRightBottom �������� � �� ��������� � �� �����������
	if (!m_list_RightBottom.empty())
	{
		for (auto & it : m_list_RightBottom)
			ret = ret && ShiftControl(it, dx, dy);
	}
	// ����� ������� ��� �������������� ���������
	m_rectReSize.right += dx;
	m_rectReSize.bottom += dy;

	// ����������� �������� �� ������ m_ListReSize
	if (!m_list_ReSize.empty())
	{
		for (auto & it : m_list_ReSize)
			ret = ret && ScaleControl(it);
	}
	// ����� �������������� ��������� ������� �������� � ��������
	if (!m_list_WithMaster.empty())
	{
		for (auto & it : m_list_WithMaster)
		{
			if (it.type == tipWithMaster)
				ret = ret && ShiftWithMaster(it, dx, dy);
			else
				ret = ret && ScaleWithMaster(it, dx, dy);
		}
	}
	m_cy_all = cy;		// ����� ������ ����
	m_cx_all  = cx;		// ����� ������ ����
	return ret;
}
//------------------------------------------------------------------------------
bool CAutoScaleXY::ShiftWithMaster(
	ControlWithMaster & par, int dx, int dy,
	CWnd *pCont, CWnd *pContMaster)
{
	CWnd *cont, *contMaster;
	if (pCont != NULL)
		cont = pCont;
	else
		cont = m_pParent->FromHandle(par.hwnd);
	if (pContMaster != NULL)
		contMaster = pContMaster;
	else
		contMaster = m_pParent->FromHandle(par.hwndMaster);
	bool ret = (cont != NULL) && (contMaster != NULL);
	if (ret)
	{
		int m(0);
		RECT rect, rectMaster;
		cont->GetWindowRect(&rect);
		m_pParent->ScreenToClient(&rect);

		contMaster->GetWindowRect(&rectMaster);
		m_pParent->ScreenToClient(&rectMaster);

		switch (par.flags)
		{
		case tfLeft:
			rect.top    += dy;
			rect.bottom += dy;
			if (par.flagsMaster == tfLeft)
			{// ����� ������� �������� ���������� � ����� ������� �������
				m = rectMaster.left + par.shift;
			}
			else if (par.flagsMaster == tfRight)
			{// ����� ������� �������� ���������� � ������ ������� �������
				m = rectMaster.right + par.shift;
			}
			rect.right += m - rect.left;
			rect.left = m;
			break;

		case tfRight:
			rect.top    += dy;
			rect.bottom += dy;
			if (par.flagsMaster == tfLeft)
			{// ������ ������� �������� ���������� � ����� ������� �������
				m = rectMaster.left + par.shift;
			}
			else if (par.flagsMaster == tfRight)
			{// ������ ������� �������� ���������� � ������ ������� �������
				m = rectMaster.right + par.shift;
			}
			rect.left += m - rect.right;
			rect.right = m;
			break;

		case tfTop:
			rect.left  += dx;
			rect.right += dx;
			if (par.flagsMaster == tfTop)
			{// ������� ������� �������� ���������� � ������� ������� �������
				m = rectMaster.top + par.shift;
			} 
			else if (par.flagsMaster == tfBottom)
			{// ������� ������� �������� ���������� � ������ ������� �������
				m = rectMaster.bottom + par.shift;
			}
			rect.bottom += m - rect.top;
			rect.top = m;
			break;

		case tfBottom:
			rect.left  += dx;
			rect.right += dx;
			if (par.flagsMaster == tfTop)
			{// ������ ������� �������� ���������� � ������� ������� �������
				m = rectMaster.top + par.shift;
			} 
			else if (par.flagsMaster == tfBottom)
			{// ������ ������� �������� ���������� � ������ ������� �������
				m = rectMaster.bottom + par.shift;
			}
			rect.top += m - rect.bottom;
			rect.bottom = m;
//			break;
		}
		cont->MoveWindow(&rect);
	}
	return ret;
}
//------------------------------------------------------------------------------
bool CAutoScaleXY::ScaleWithMaster(ControlWithMaster & par, int dx, int dy)
{
	CWnd *cont = m_pParent->FromHandle(par.hwnd);
	CWnd *contMaster = m_pParent->FromHandle(par.hwndMaster);
	bool ret = (cont != NULL) && (contMaster != NULL);
	if (ret)
	{
		RECT rect, rectMaster;
		cont->GetWindowRect(&rect);
		m_pParent->ScreenToClient(&rect);

		contMaster->GetWindowRect(&rectMaster);
		m_pParent->ScreenToClient(&rectMaster);

		switch (par.type)
		{
		case tipLeftTop:	// ����� ������� ���� �� ���������
			if (par.flags == tfRight)
			{
				rect.bottom += dy;
				if (par.flagsMaster == tfLeft)
				{// ������ ������� �������� ������������� � ����� ������� �������
					rect.right = rectMaster.left + par.shift;
				}
				else if (par.flagsMaster == tfRight)
				{// ������ ������� �������� ������������� � ������ ������� �������
					rect.right = rectMaster.right + par.shift;
				}
			}
			else if (par.flags == tfBottom)
			{
				rect.right += dx;
				if (par.flagsMaster == tfLeft)
				{// ������ ������� �������� ������������� � ����� ������� �������
					rect.bottom = rectMaster.left + par.shift;
				}
				else if (par.flagsMaster == tfRight)
				{// ������ ������� �������� ������������� � ������ ������� �������
					rect.bottom = rectMaster.right + par.shift;
				}
			}
			break;

		case tipLeftBottom:		// ����� ������� �� �����, ������ ���������
			//	rect.left += 0;
			rect.bottom += dy;
			if (par.flags == tfRight)
			{
				rect.top += dy;
				if (par.flagsMaster == tfLeft)
				{// ������ ������� �������� ���������� � ����� ������� �������
					rect.right = rectMaster.left + par.shift;
				}
				else if (par.flagsMaster == tfRight)
				{// ������ ������� �������� ���������� � ������ ������� �������
					rect.right = rectMaster.right + par.shift;
				}
			}
			else if (par.flags == tfTop)
			{
				rect.right += dx;
				if (par.flagsMaster == tfTop)
				{// ������� ������� �������� ���������� � ������� ������� �������
					rect.top = rectMaster.top + par.shift;
				}
				else if (par.flagsMaster == tfBottom)
				{// ������� ������� �������� ���������� � ������ ������� �������
					rect.top = rectMaster.bottom + par.shift;
				}
			}
			break;

		case tipRightTop:
			//	rect.top += 0;
			rect.right += dx;
			if (par.flags == tfLeft)
			{
				rect.bottom += dy;
				if (par.flagsMaster == tfLeft)
				{// ����� ������� �������� ���������� � ����� ������� �������
					rect.left = rectMaster.left + par.shift;
				}
				else if (par.flagsMaster == tfRight)
				{// ����� ������� �������� ���������� � ������ ������� �������
					rect.left = rectMaster.right + par.shift;
				}
			}
			else if (par.flags == tfBottom)
			{
				rect.left += dx;
				if (par.flagsMaster == tfTop)
				{// ������� ������� �������� ���������� � ������� ������� �������
					rect.bottom = rectMaster.top + par.shift;
				}
				else if (par.flagsMaster == tfBottom)
				{// ������� ������� �������� ���������� � ������ ������� �������
					rect.bottom = rectMaster.bottom + par.shift;
				}
			}
			break;

		case tipRightBottom:
		{
			rect.right += dx;
			rect.bottom += dy;
			if (par.flags == tfLeft)
			{
				rect.top += dy;
				if (par.flagsMaster == tfLeft)
				{// ����� ������� �������� ����������� � ������ ������� �������
					rect.left = rectMaster.left + par.shift;
				}
				else if (par.flagsMaster == tfRight)
				{// ����� ������� �������� ����������� � ������ ������� �������
					rect.left = rectMaster.right + par.shift;
				}
			}
			else if (par.flags == tfTop)
			{
				rect.left += dx;
				if (par.flagsMaster == tfTop)
				{// ������� ������� �������� ����������� � ������� ������� �������
					rect.top = rectMaster.top + par.shift;
				}
				else if (par.flagsMaster == tfBottom)
				{// ������� ������� �������� ����������� � ������ ������� �������
					rect.top = rectMaster.bottom + par.shift;
				}
			}
			break;
		}

		case tipReSize:
		{
			LONG xy = m_rectReSize.bottom - m_rectReSize.top;
			LONG xx = m_rectReSize.right - m_rectReSize.left;

			if (par.flagsMaster == tfBottom)
			{	// ������� ������� �������� ���������� � ������ ������� �������
				rect.top = rectMaster.bottom + par.shift;
			}
			else
				rect.top = m_rectReSize.top + LONG(par.resize._top * xy);
			

			if (par.flagsMaster == tfRight)
			{	// ����� ������� �������� ���������� � ������ ������� �������
				rect.left = rectMaster.right + par.shift;
				// ��������� ������� ������� �������
				rect.top = rectMaster.top;
			}
			else
				rect.left = m_rectReSize.left + LONG(par.resize._left * xx);
			
			if ((par.resize.flags & tfConstHeight))	
				// �� ������ ������
				rect.bottom = rect.top + par.resize._default_height;
			else
			{	// ���������
				if ((par.resize.flags & tfBottom))
					rect.bottom += dy;
				else
					rect.bottom = m_rectReSize.top + LONG(par.resize._top * xy) + LONG(par.resize._height * xy);
			}

			// ������ ������ ������
			if ((par.resize.flags & tfRight))
				rect.right += dx;
			else
				rect.right = m_rectReSize.left + LONG(par.resize._left * xx) + LONG(par.resize._width * xx);

		}
		}
		cont->MoveWindow(&rect);
	}
	return ret;
}
//------------------------------------------------------------------------------
// �������� ������������� ��������� �� ������ ListReSize.
//-----------------------------------------------------------------
bool CAutoScaleXY::CheckControlsReSize()
{
	bool ret = !m_list_ReSize.empty();
	if (ret)
	{
		for (auto & it : m_list_ReSize)
		{
			ret = m_pParent->FromHandle(it.hwnd) != nullptr;
			if (ret)
				ret = (it.flags & tfLeft) && (it.flags & tfRight);

			if (!ret)
				break;
		}
	}
	return ret;
}
//------------------------------------------------------------------------------
bool CAutoScaleXY::ScaleControl(UseControlReSize & puc)
{	// ����������� ������� �� ������ m_ListReSize
	CWnd* cont = m_pParent->FromHandle(puc.hwnd);
	bool ret = cont != nullptr;
	if (ret)
	{
		puc.rect.top = m_rectReSize.top + LONG(puc._top * m_rectReSize.Height());
		
		if ((puc.flags & tfConstHeight))
			puc.rect.bottom = puc.rect.top + puc._default_height;			
		else
			puc.rect.bottom = puc.rect.top + LONG(puc._height * m_rectReSize.Height());
		
		puc.rect.left = m_rectReSize.left + LONG(puc._left * m_rectReSize.Width());
		puc.rect.right = puc.rect.left + LONG(puc._width * m_rectReSize.Width());

		TestAndMove(puc);
	}
	return ret;
}
//------------------------------------------------------------------------------
// ����� ��������, �������� � ����������� (���� ��� ������)
bool CAutoScaleXY::ShiftControl(
	ControlNoSize & control,	// hWnd ����������� ��������
	int dx,		// �� ������� ������� �� �
	int dy,		// �� ������� ������� �� Y
	bool bLeft,	// ������� ����� �������
	bool bTop)	// ������� ������� �������
{	// ������� ������� � �������������� ���������
	bool ret;
	CRect rect;
	CWnd* cont = m_pParent->FromHandle(control.hwnd);
	if (cont)
	{
		
		cont->GetWindowRect(rect);
		m_pParent->ScreenToClient(rect);

		// ��������� ���� � ���������� ���������� ��������������� ������ ��������
		// ���� ������ ����, �� �������� ������� � ������ ���� ������
		if (control.WidthError < 0)
		{
			control.WidthError += dx;
			dx = control.WidthError;
		}
		// ��������� ���� � ���������� ���������� ��������������� ������ ��������
		// ���� ������ ����, �� �������� ������� � ������ ���� ������
		if (control.HeightError < 0)
		{
			control.HeightError += dy;
			dy = control.HeightError;
		}

		if (bLeft)
			rect.left += dx;
		rect.right += dx;
		if (bTop)
			rect.top += dy;
		rect.bottom += dy;

		// ��������� ��������� ��������������� 
		if (rect.right < rect.left)
			control.WidthError = rect.right - rect.left;
		if (rect.bottom < rect.top)
			control.HeightError = rect.bottom - rect.top;

		cont->MoveWindow(rect);

		ret = true;
	}
	else
		ret = false;
	return ret;
}
//------------------------------------------------------------------------------
// ���������� �������������� ��������� � ������ �� ������������ ������.
// ������ � ������ ����� ����� �������, ��������� - ����� ������.
// �������� �� ������������� ��������� �� ������!!!
void CAutoScaleXY::SortControls()
{// ��� ����� ������ ������� � ������� � ������ ������ ������
	std::list<UseControlReSize>::iterator iterTop;
	int numCont = m_list_ReSize.size() - 1;	// ��������� ������� ������� ������

	std::list<UseControlReSize> *list = new std::list<UseControlReSize>;
	for(int i=0; i<numCont; ++i)	// ������ numCont, size �����������!!!
	{
		int top(-1);
		for(auto it=m_list_ReSize.begin(); it!=m_list_ReSize.end(); ++it)
		{
			if ( it->rect.top > top )
			{
				iterTop = it;
				top = it->rect.top;
			}
		}
		list->push_front( *iterTop );
		m_list_ReSize.erase(iterTop);
	}
	// �������� ��������� �������
	list->push_front( *m_list_ReSize.begin() );
	m_list_ReSize = *list;
	delete list;
	list = nullptr;
}
//------------------------------------------------------------------------------
// hControl - ID ��������, ������ ���������������, �������� � �������, ID �������, 
// flagsMaster - ���� �������� � ������� ������� (tfBottom ����� �������� tfRight ������ �� ��������)
// shift - ������ �� �������
// ���������� ��� ������� �������� ������ � OnInitDialog ����� AddControlFinish
// � �� OnPostInitDialog()!!!
bool CAutoScaleXY::AddControlWithMaster(HWND hControl, TipContrl type, WORD flags,
	HWND hControlMaster, WORD flagsMaster, long shift)
{
	if (m_pParent == nullptr)
		return false;

	CWnd* cont = m_pParent->FromHandle(hControl);
	CWnd* contMaster = m_pParent->FromHandle(hControlMaster);
	bool ret = (cont != NULL) && (contMaster != NULL);
	if (ret)
	{// �������� ���� ��� ��� ����� � ������
		ret = false;
		if ( !m_list_WithMaster.empty() )
		{
			for (auto & it : m_list_WithMaster)
			{
				if (it.hwnd == hControl)
				{
					it.flags = flags;
					it.flagsMaster = flagsMaster;
					it.hwndMaster = hControlMaster;
					it.shift = shift;
					it.type = type;
					ret = ShiftWithMaster(it, 0, 0, cont, contMaster);
				}
			}
		}
		if (!ret)
		{// ��� ������
			ControlWithMaster cnswm;
			cnswm.flags = flags;
			cnswm.flagsMaster = flagsMaster;
			cnswm.hwnd = hControl;
			cnswm.hwndMaster = hControlMaster;
			cnswm.shift = shift;
			cnswm.type = type;
			cnswm.resize.flags = flags;
			cnswm.resize.hwnd = hControl;
			m_list_WithMaster.push_back(cnswm);
			ret = true;

			if (type == tipReSize)
			{
				RECT rect;
				cont->GetWindowRect(&rect);
				m_pParent->ScreenToClient(&rect);
				if (m_rectReSize.left > rect.left)
					m_rectReSize.left = rect.left;
				if (m_rectReSize.top > rect.top)
					m_rectReSize.top = rect.top;
				if (m_rectReSize.right < rect.right)
					m_rectReSize.right = rect.right;
				if (m_rectReSize.bottom < rect.bottom)
					m_rectReSize.bottom = rect.bottom;
			}
		}
	}
	return ret;
}
//------------------------------------------------------------------------------
// ���������� �������� � ��������������� ������ �� ���������������.
// ���������� ��� ������� �������� ������ � OnInitDialog ����� AddControlFinish
// � �� OnPostInitDialog()!!!
bool CAutoScaleXY::AddControl(HWND hControl, TipContrl type, WORD flags)
{
	if (m_pParent == nullptr)
		return false;

	CWnd* cont = m_pParent->FromHandle(hControl);
	bool ret = cont != nullptr;

	if (ret)
	{
		RECT rect;
		UseControlReSize ucRe;
		ControlNoSize ucNo;
		ControlNoSizeWithShow ucNoWS;

		switch (type)
		{
		case tipLeftTop:
			ucNo.hwnd = hControl;
			ucNo.flags = flags;
			m_list_LeftTop.push_back(ucNo);
			break;

		case tipLeftBottom:
			ucNo.hwnd = hControl;
			ucNo.flags = flags;
			m_list_LeftBottom.push_back(ucNo);
			break;

		case tipRightTop:
			ucNoWS.hwnd = hControl;
			ucNoWS.flags = flags;
			cont->GetWindowRect(&ucNoWS.rect);
			m_pParent->ScreenToClient(&ucNoWS.rect);
			m_list_RightTop_Src.push_back(ucNoWS);
			break;

		case tipRightBottom:
			ucNo.hwnd = hControl;
			ucNo.flags = flags;
			m_list_RightBottom.push_back(ucNo);
			break;

		case tipReSize:
			if (!m_bInit)
			{
				ucRe.hwnd = hControl;
				ucRe.flags = flags;			
				m_list_ReSize.push_back(ucRe);
			}
		//	break; - ����� �� �����

		case tipReSizeNo:
			if (!m_bInit)
			{
				cont->GetWindowRect(&rect);
				m_pParent->ScreenToClient(&rect);
				if ( m_rectReSize.left > rect.left )			
					m_rectReSize.left = rect.left;
				if ( m_rectReSize.top > rect.top )
					m_rectReSize.top = rect.top;
				if ( m_rectReSize.right < rect.right )
					m_rectReSize.right = rect.right;
				if ( m_rectReSize.bottom < rect.bottom )
					m_rectReSize.bottom = rect.bottom;
			}
			break;

		default :
			ret = false;
		}	
	}
	return ret;
}
//------------------------------------------------------------------------------
// ��������� ����������� ���������
void CAutoScaleXY::AddControlFinish()
{// �������� �������� �� ������������� �� ���������, ��� ���� � AddControl
	if (m_pParent == nullptr)
		return;

	float f_Height = 1.f / float(m_rectReSize.Height());
	float f_Width = 1.f / float(m_rectReSize.Width());

	// ��������� �������������� ���������
	for (auto & it : m_list_ReSize)
	{
		m_pParent->FromHandle(it.hwnd)->GetWindowRect(&it.rect);
		m_pParent->ScreenToClient(&it.rect);
		TestAndMove(it);

		auto &tr = it.rect;
		it._top = float(tr.top - m_rectReSize.top) * f_Height;
		it._height = float(tr.bottom - tr.top) *
			f_Height;
		it._left = float(tr.left - m_rectReSize.left) * f_Width;
		it._width = float(tr.right - tr.left) *
			f_Width;

		it._default_height = tr.bottom - tr.top;
	}

	// ��������� ��������� � ��������
	for (auto it = m_list_WithMaster.begin(); it != m_list_WithMaster.end(); ++it)
	{
		auto &tr = it->resize;
		m_pParent->FromHandle(it->hwnd)->GetWindowRect(&tr.rect);
		m_pParent->ScreenToClient(&tr.rect);

		CRect Rect = tr.rect;
		tr._top = float(Rect.top - m_rectReSize.top)  * f_Height;
		tr._height = float(Rect.bottom - Rect.top)		  * f_Height;
		tr._left = float(Rect.left - m_rectReSize.left) * f_Width;
		tr._width = float(Rect.right - Rect.left)		  *	f_Width;
		tr._default_height = Rect.bottom - Rect.top;
	}

	// ��������� ���������, ������������ � ������� �������� ����, �������
	// ����� ������
	AddControlFinish_RightTop();

	m_bInit = true;
}//------------------------------------------------------------------------------
void CAutoScaleXY::AddControlFinish_RightTop()
{
	if (!m_list_RightTop_Src.empty())
	{	// ����������� ��������������� ���������
		bool bFlag(true);
		for (auto & it : m_list_RightTop_Src)
		{
			CWnd * cont = m_pParent->FromHandle(it.hwnd);
			bFlag &= cont != nullptr;
			if (bFlag)
			{
				cont->GetWindowRect(&it.rect);
				m_pParent->ScreenToClient(&it.rect);
			}
			else
				break;
		}
		// ����������� � ����������� �������� ���������� ���������
		if (bFlag)
		{
			m_list_RightTop_Src.sort();
			m_top0_RightTop = m_list_RightTop_Src.begin()->rect.top;


			long index = (long)(m_list_RightTop_Src.size() - 1);
			long top_i(0);
			bool bFirst(true);
			for (auto it = m_list_RightTop_Src.rbegin(), ie = m_list_RightTop_Src.rend(); it != ie; ++it, --index)
			{// ���� �� ��������� � �����
				if (bFirst)
				{
					it->dy = 0;
					bFirst = false;
				}
				else
					it->dy = top_i - it->rect.bottom;

				top_i = it->rect.top;
				it->index = index;
			}

			m_list_RightTop = m_list_RightTop_Src;
		}
	}
}
//------------------------------------------------------------------------------
void CAutoScaleXY::TestAndMove(UseControlReSize & puc)
{// ��������� ������ � ������������ � ������� � ����������� ��������
	if (puc.flags & tfLeft)
		puc.rect.left = m_rectReSize.left;
	if (puc.flags & tfTop)
		puc.rect.top = m_rectReSize.top;
	if (puc.flags & tfRight)
		puc.rect.right = m_rectReSize.right;
	if (puc.flags & tfBottom)
		puc.rect.bottom = m_rectReSize.bottom;

	CWnd * pcw = m_pParent->FromHandle(puc.hwnd);
	if (pcw != nullptr)
		pcw->MoveWindow(&puc.rect);
}
//------------------------------------------------------------------------------
// ������ ������� �� ������ �������������� m_ListReSize
// ���� ���������� ��������:
//	1. ������ ���������� �������� (� ���� ������ ��������� � �����������
//			������ ���� ���������, � ����� ����� tfLeft � tfRight):
//			hControlNew != 0.
//	2. ��������������� �������� ���������� ( � ���� ������ ���������
//			������� ������ ���� ����� ������:
//			hControlNew = 0.
// ��� ��������������� �������� ������ ���� �� ����� �� ������ ������
bool CAutoScaleXY::DeleteControl(
	HWND hControlDel,	// ��������� �������
	HWND hControlNew)	// �������, ������� �������� ����������
{	
	// ��������, ���������� � ������������ ����� ������� ������� ������
	if ( m_list_ReSize.size() == 1 )
		return false;

	bool ret(false);
	float xy(0.f);
	CWnd * contDel(nullptr);
	std::list<UseControlReSize>::iterator iterDel, iterNew;

	// ��� ��������� ������� � ������ �������������� -------------------------
	for (auto it = m_list_ReSize.begin(), ie = m_list_ReSize.end(); it != ie; ++it)
	{
		if (it->hwnd == hControlDel)
		{
			iterDel = it;
			contDel = m_pParent->FromHandle(hControlDel);
			if (contDel)
			{
				ret = true;
				break;
			}
		}
	}
	if (!ret)	// ������� �� ������
		return false;

	// ����������� � ������� ������ -----------------------------------------
	// ���� �� ������, � ����� ������
	if (hControlNew)
	{// ���� ���������� �������� ����� ���������
		ret = false;
		// ��� ����� ������� � ������ �������������� --------------------
		for(auto it=m_list_ReSize.begin(); it!=m_list_ReSize.end(); ++it)
		{
			if (it->hwnd == hControlNew)
			{
				iterNew = it;
				if ( m_pParent->FromHandle(hControlNew) )
				{
					ret = true;
					break;
				}
			}
		}
		if (!ret)	// ������� �� ������
			return false;

		// ��� �������� �������, �������� �� ����� --------------------------
		// ��� �������� ������ ���� �� ������ ���� �� ������� ����
		if ( !( (iterDel->flags & tfLeft) &&
				(iterNew->flags & tfLeft) &&
				(iterDel->flags & tfRight) &&
				(iterNew->flags & tfRight) ) )
				return false;

		// �������� ���� ����� � ���� ���������� ��������
		iterNew->flags = iterNew->flags | (iterDel->flags & tfTop) |
			(iterDel->flags & tfBottom);
		auto &tr = iterDel->rect;
		auto &mr = iterNew->rect;
		mr.top = min(mr.top,
			tr.top);
		mr.bottom = max(mr.bottom,
			tr.bottom);

		// ����� ��������� ������ ��������
		xy = 1.f / float(m_rectReSize.bottom - m_rectReSize.top);
		iterNew->_top = float(mr.top - m_rectReSize.top) * xy;
		iterNew->_height = float(mr.bottom - mr.top) * xy;
		xy = 1.f / float(m_rectReSize.right - m_rectReSize.left);
		iterNew->_left = float(mr.left - m_rectReSize.left) * xy;
		iterNew->_width = float(mr.right - mr.left) * xy;

		// ����������, ret ��� ����� true
		TestAndMove(*iterNew);
		contDel->ShowWindow(0);
		m_list_ReSize.erase(iterDel);
	}// if (hControlNew) - ���� ���������� �������� ����� ���������

	else
	{// ���� ���������� �������� ����� ���� �������, ���������� �� Y
		// ��������: ��������� - ������?
		if (iterDel->rect.bottom != m_rectReSize.bottom)
			return false;

		// �������� ����� � ������� ���� �������������� ���������
		if ( !CheckControlsReSize() )
			return false;

		// ��������� � �������, ret = true
		contDel->ShowWindow(0);
		m_list_ReSize.erase(iterDel);

		// ��� ����� ������ ������� �� ����������
		int bottom(0);		
		for(auto it=m_list_ReSize.begin(); it!=m_list_ReSize.end(); ++it)
		{
			if ( bottom < it->rect.bottom )
			{
				bottom = it->rect.bottom;
				iterNew = it;
			}
		}
		iterNew->flags = iterNew->flags | tfBottom;

		// ���������� ��������� ����� ����� ��������� �� ���������
		xy = 1.f / float(bottom - m_rectReSize.top);
		for (auto it = m_list_ReSize.begin(); it != m_list_ReSize.end(); ++it)
		{
			auto &tr = it->rect;
			it->_top = float(tr.top - m_rectReSize.top) * xy;
			it->_height = float(tr.bottom - tr.top) * xy;
		}

		// ����������� �������� �� ������ m_ListReSize ������ �� ���������
		xy = float(m_rectReSize.bottom - m_rectReSize.top);
		for (auto it = m_list_ReSize.begin(); it != m_list_ReSize.end(); ++it)
		{
			auto &tr = it->rect;
			tr.top = m_rectReSize.top + LONG(it->_top    * xy);
			tr.bottom = tr.top + LONG(it->_height * xy);
			TestAndMove(*it);
		}
	}// else if (hControlNew) - ���� ���������� �������� ����� ���� �������

	if (!m_list_WithMaster.empty())
		ret = AutoScale(m_cx_all, m_cy_all);
	return ret;
}
//------------------------------------------------------------------------------
// ���������� 1 ��� 2 ���������, �������� ��������
// ��� ��� �������� ������ ���� �� ����� �������, �� ������
bool CAutoScaleXY::DivideControl(
	HWND hControlDiv,	// ������� �������, �� �������
	bool flagTop,		// ��� ����� ������� �������: ������(true), �����(false)
	HWND hControl1,		// 1-�� ����� ������� (�������� � �������)
	HWND hControl2)		// 2-�� ����� ������� (�������� � 1-��)
{
	bool ret(false);
	int heigth(0), dh(0), n(0);
	CWnd *contDiv(nullptr), *contNew1(nullptr), *contNew2(nullptr);
	UseControlReSize ucrs1, ucrs2;
	std::list<UseControlReSize>::iterator iterDiv;
	// ��� ������� � ����� �������� -------------------------------
	for(auto it=m_list_ReSize.begin(); it!=m_list_ReSize.end(); ++it)
	{
		if ( it->hwnd == hControlDiv )
		{
			iterDiv = it;
			contDiv = m_pParent->FromHandle(hControlDiv);
			if (contDiv)
			{// ������� ������� � ������ ����, ������
				ret = true;
				break;
			}
		}
	}
	if (!ret)
		return false;
	// hControl1, ret = true
	contNew1 = m_pParent->FromHandle(hControl1);
	if (contNew1)
	{
		for(auto it=m_list_ReSize.begin(); it!=m_list_ReSize.end(); ++it)
		{
			if (it->hwnd == hControl1)
			{// ������ ������� ��� ���� � ������, �����
				ret = false;
				break;
			}
		}
	}
	else
		ret = false;
	if (!ret)
		return false;

	if (hControl2)
	{// hControl2, ret = true
		contNew2 = m_pParent->FromHandle(hControl2);
		if (contNew2)
		{
			for(auto it=m_list_ReSize.begin(); it!=m_list_ReSize.end(); ++it)
			{
				if (it->hwnd == hControl2)
				{// ������ ������� ��� ���� � ������, �����
					ret = false;
					break;
				}
			}
		}
		else
			ret = false;
		if (!ret)
		return false;
	}

	ucrs1.hwnd = hControl1;
	ucrs1.flags = tfLeft | tfRight;
	ucrs1.rect.left = m_rectReSize.left;
	ucrs1.rect.right = m_rectReSize.right;
	ucrs1._left = 0.f;
	ucrs1._width = 1.f;
	float f_Height = 1.f / float(m_rectReSize.bottom - m_rectReSize.top);
	// ��������� �� ������� ����� ������ -----------------
	if (hControl2)
	{// ����� �� 3, ret = true
		ucrs2.hwnd = hControl2;
		ucrs2.flags = tfLeft | tfRight;
		ucrs2.rect.left = m_rectReSize.left;
		ucrs2.rect.right = m_rectReSize.right;
		ucrs2._left = 0.f;
		ucrs2._width = 1.f;
		auto &tr = iterDiv->rect;
		heigth = (tr.bottom - tr.top) / 3;
		// dh - �������, ����� = 0, 1 ��� 2
		dh = tr.bottom - tr.top - 3 * heigth;
		n = tr.top;

		if (flagTop)
		{// contDiv �������, ��� rect.top ������� �������
			n += heigth;
			tr.bottom = n;
			if (dh--)
				n++;
			ucrs1.rect.top = n;
			n += heigth;
			ucrs1.rect.bottom = n;
			if (dh)
				n++;
			ucrs2.rect.top = n;
			ucrs2.rect.bottom = n + heigth;

			// ������� ������� ��� ���� ������, ���� tfBottom ������� puc2
			// ���� �������, �� ������ ������ �� ����, ������� � �������
			ucrs2.flags = ucrs2.flags | (iterDiv->flags & tfBottom);
			iterDiv->flags = iterDiv->flags & (~tfBottom);
		}
		else
		{			
			// contDiv ������, ��� rect.bottom �� ���������
			ucrs2.rect.top = n;
			n += heigth;
			ucrs2.rect.bottom = n;
			if (dh--)
				n++;
			ucrs1.rect.top = n;
			n += heigth;
			ucrs1.rect.bottom = n;
			if (dh)
				n++;
			iterDiv->rect.top = n;

			// ������� ������� ��� ���� �������, ���� tfTop ������� puc2
			// ���� ������, �� �� � �������
			ucrs2.flags = ucrs2.flags | (iterDiv->flags & tfTop);
			iterDiv->flags = iterDiv->flags & (~tfTop);
		}
		ucrs2._top = ( ucrs2.rect.top - m_rectReSize.top ) * f_Height;
		ucrs2._height = ( ucrs2.rect.bottom - ucrs2.rect.top ) * f_Height;
		contNew2->ShowWindow(1);
		TestAndMove(ucrs2);
		m_list_ReSize.push_back(ucrs2);
	}// if (hControl2) - ����� �� 3

	else
	{// ����� �� 2, ret = true
		auto &tr = iterDiv->rect;
		heigth = (tr.bottom - tr.top) / 2;
		// dh - �������, ����� = 0 ��� 1
		dh = tr.bottom - tr.top - 2 * heigth;
		n = tr.top;
		if (flagTop)
		{// contDiv �������, ��� rect.top ������� �������			
			n += heigth;
			tr.bottom = n;
			if (dh)
				n++;
			ucrs1.rect.top = n;
			ucrs1.rect.bottom = n + heigth;

			// ������� ������� ��� ���� ������, ���� tfBottom ������� puc1
			// ���� �������, �� ������ ������ �� ����, ������� � �������
			ucrs1.flags = ucrs1.flags | (iterDiv->flags & tfBottom);
			iterDiv->flags = iterDiv->flags & (~tfBottom);
		}
		else
		{// contDiv ������, ��� rect.bottom �� ���������
			ucrs1.rect.top = n;
			n += heigth;
			ucrs1.rect.bottom = n;
			if (dh)
				n++;
			iterDiv->rect.top = n;

			// ������� ������� ��� ���� �������, ���� tfTop ������� puc1
			// ���� ������, �� �� � �������
			ucrs1.flags = ucrs1.flags | (iterDiv->flags & tfTop);
			iterDiv->flags = iterDiv->flags & (~tfTop);

		}
	}// else  if (hControl2) - ����� �� 2
	
	ucrs1._top = ( ucrs1.rect.top - m_rectReSize.top ) * f_Height;
	ucrs1._height = ( ucrs1.rect.bottom - ucrs1.rect.top ) * f_Height;
	contNew1->ShowWindow(1);
	TestAndMove(ucrs1);
	m_list_ReSize.push_back(ucrs1);
	auto &tr = iterDiv->rect;
	iterDiv->_top = (tr.top - m_rectReSize.top) * f_Height;
	iterDiv->_height = (tr.bottom - tr.top) * f_Height;
	TestAndMove(*iterDiv);

	if ( !m_list_WithMaster.empty() )
		ret = AutoScale(m_cx_all, m_cy_all);

	return ret;
}
//------------------------------------------------------------------------------
// ���� �������������� ��������� ������ ���������� ������
bool CAutoScaleXY::EqualHight()
{
	// �������� ����� � ������� ���� �������������� ���������
	if (!CheckControlsReSize())
		return false;

	int num = m_list_ReSize.size();
	if (num > 1)
	{
		SortControls();		// �������� �������� � ������ �� �� ������������
		int height(0);		//	������ ����
		for (auto it = m_list_ReSize.begin(); it != m_list_ReSize.end(); ++it)
			height += it->rect.bottom - it->rect.top;

		// dy - ������� ���������� �/� ����������
		int dy = (m_rectReSize.bottom - m_rectReSize.top - height) / (num - 1);
		height /= num;	// ������� ������
		// ddy - ������� (�� 0 �� num-1)
		int ddy = m_rectReSize.bottom - m_rectReSize.top - height * num - dy * (num - 1);
		int n = m_rectReSize.top;
		float f_Height = 1.f / float(m_rectReSize.bottom - m_rectReSize.top);
		for (auto it = m_list_ReSize.begin(); it != m_list_ReSize.end(); ++it)
		{
			auto &tr = it->rect;
			tr.top = n;
			it->_top = (tr.top - m_rectReSize.top) * f_Height;
			n += height;
			tr.bottom = n;
			it->_height = (n - tr.top) * f_Height;
			TestAndMove(*it);
			n += dy;
			if (ddy--)
				n++;
		}
	}
	return true;
}

//------------------------------------------------------------------------------
// � ��� �������� ����� ������� � ������������ hControl
// ��� ������ = HeightAsPercentOfMiddle ��������� �� ������� ������ ���������
bool CAutoScaleXY::NewControl(HWND hControl, int HeightAsPercentOfMiddle)
{
	bool ret;
	CWnd *contNew = m_pParent->FromHandle(hControl);
	if (contNew)
		ret = true;
	else
		return false;
	
	for(auto it=m_list_ReSize.begin(); it!=m_list_ReSize.end(); ++it)
	{
		ret = ret && (it->flags & tfLeft) && (it->flags & tfRight);
		if ( it->hwnd == hControl )
		{
			ret = false;
			break;
		}
	}
	if (!ret)
		return false;

	UseControlReSize ucrs;
	ucrs.flags = tfLeft | tfRight | tfBottom;
	ucrs.hwnd = hControl;
	ucrs.rect.left = m_rectReSize.left;
	ucrs.rect.right = m_rectReSize.right;
	ucrs.rect.bottom = m_rectReSize.bottom;
	ucrs._left = 0.f;
	ucrs._width = 1.f;

	// ��������� ������ �������� ������ ��������,
	// ��� � ����� ���� ���������� ������ ������� � �������� ���� ����
	int height(0);		// ������ ������ ��������
	for(auto it=m_list_ReSize.begin(); it!=m_list_ReSize.end(); ++it)
	{
		auto &tr = it->rect;
		height += tr.bottom - tr.top;
		if (tr.bottom == m_rectReSize.bottom)
			it->flags = it->flags & (~tfBottom);
	}
	int num = m_list_ReSize.size();
	int Height = m_rectReSize.bottom - m_rectReSize.top;
	// dy ���������� �/� ����� ��������� � ��������� ������
	int dy = (Height - height) / num;
	height = int( (float)HeightAsPercentOfMiddle * 0.01f *
					(float)height / (float)(num + 1) );
	ucrs.rect.top = ucrs.rect.bottom - height;
	float f_Height = 1.f / (float)Height;

	ucrs._top = ( ucrs.rect.top - m_rectReSize.top ) * f_Height;
	ucrs._height = ( ucrs.rect.bottom - ucrs.rect.top ) * f_Height;
	contNew->MoveWindow(&ucrs.rect);

	// ��� ������ �������� �� ������ ������
	float xy = float(Height - height - dy) / (float)Height;
	for(auto it=m_list_ReSize.begin(); it!=m_list_ReSize.end(); ++it)
	{
		it->_top *= xy;
		it->_height *= xy;
		auto &tr = it->rect;
		tr.top = m_rectReSize.top + LONG(it->_top * (float)Height);
		tr.bottom = tr.top + LONG( it->_height * (float)Height );
		TestAndMove(*it);
	}
	m_list_ReSize.push_back(ucrs);
	contNew->ShowWindow(1);

	if ( !m_list_WithMaster.empty() )
		ret = AutoScale(m_cx_all, m_cy_all);
	return ret;
}
//------------------------------------------------------------------------------
bool CAutoScaleXY::Show(HWND hControl, bool bShow)
{
	bool ret = !m_list_RightTop_Src.empty();
	if (ret)
	{
		RECT re = { 0 };
		CWnd *cont = m_pParent->FromHandle(m_list_RightTop_Src.begin()->hwnd);
		cont->GetWindowRect(&re);
		m_pParent->ScreenToClient(&re);
		long left = re.left;

		// ����� �������� � m_list_RightTop_Src
		for (auto & it : m_list_RightTop_Src)
		{
			if (it.hwnd == hControl)
			{
				CWnd *cont = m_pParent->FromHandle(hControl);
				ret = cont != nullptr;
				if (ret)
				{
					ret = it.bShow != bShow;
					if (ret)
					{
						it.bShow = bShow;
						cont->ShowWindow((int)bShow);
					}
				}
				break;
			}
		}
		// ���� ������� ������ � ���� �������� ��� �����������
		if (ret)
		{
			long top_i(m_top0_RightTop);

			m_list_RightTop.clear();	// ��� �������� ���� ���� ��������� ���� list � ������
			for (auto & it : m_list_RightTop_Src)
			{
				if (it.bShow)
				{
					ControlNoSizeWithShow cnsws;
					cnsws.flags = it.flags;
					cnsws.hwnd = it.hwnd;
					cnsws.HeightError = 0;
					cnsws.WidthError = 0;

					cnsws.bShow = true;
					cnsws.dy = it.dy;
					cnsws.rect.left = left;
					cnsws.rect.right = left + it.rect.right - it.rect.left;
					cnsws.rect.top = top_i;
					cnsws.rect.bottom = top_i + it.rect.bottom - it.rect.top;

					CWnd *cont = m_pParent->FromHandle(it.hwnd);
					if (cont != nullptr)
					{
						m_list_RightTop.push_back(cnsws);
						cont->MoveWindow(&cnsws.rect);
					}
					top_i = cnsws.rect.bottom + cnsws.dy;
				}
			}
		}
	}
	return ret;
}
//------------------------------------------------------------------------------