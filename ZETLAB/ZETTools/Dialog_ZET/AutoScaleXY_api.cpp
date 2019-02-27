//------------------------------------------------------------------------------
// AutoScaleXY.cpp : файл реализации
// Версия от 12.08.2014
// Зеленоград, ЗАО "ЭТМС", Савостин Ю.М.
//------------------------------------------------------------------------------
#include "stdafx.h"
#include "AutoScaleXY_api.h"
//------------------------------------------------------------------------------
CAutoScaleXY::CAutoScaleXY()
	: m_bNoFinish (true)
	, m_HeightAll (0)	// полная клиентская высота окна
	, m_WidthAll  (0)	// полная клиентская ширина окна

{
	m_rectReSize.left =
	m_rectReSize.top = LONG_MAX;
	m_rectReSize.right =
	m_rectReSize.bottom = 0;
	m_hParent = NULL;
}
//------------------------------------------------------------------------------
CAutoScaleXY::~CAutoScaleXY()
{
	m_ListReSize.clear();
	m_ListLeftTop.clear();
	m_ListLeftBottom.clear();
	m_ListRightTop.clear();
	m_ListRightBottom.clear();
	m_ListWithMaster.clear();
}
//------------------------------------------------------------------------------
bool CAutoScaleXY::Create(HWND hParent)
{
	RECT rect;
	m_hParent = hParent;
	GetClientRect(m_hParent, &rect);		// Надо обязательно!!!
	m_HeightAll = rect.bottom - rect.top;
	m_WidthAll = rect.right - rect.left;
	return true;
}
//------------------------------------------------------------------------------
bool CAutoScaleXY::IsEmpty()
{
	return  m_ListReSize.empty() && m_ListLeftTop.empty() &&
			m_ListLeftBottom.empty() && m_ListRightTop.empty() &&
			m_ListRightBottom.empty() && m_ListWithMaster.empty();
}
//------------------------------------------------------------------------------
bool CAutoScaleXY::AutoScale(int cx, int cy)
{
	bool ret = true;
	if ((cx == 0) || (cy == 0))
		return ret;

	int dx = cx - m_WidthAll;		// измененние ширина окна
	int dy = cy - m_HeightAll;		// изменение высоты окна

	// список m_ListLeftTop никогда и никуда сдвигать не надо
	if (!m_ListLeftTop.empty())
		for (auto it = m_ListLeftTop.begin(); it != m_ListLeftTop.end();
			++it)
	{
		if (it->flags & tfRight)	// ширина следит за правой границей
			ret = ret && ShiftControl(it->hwnd, dx, 0, false, false);
		else if (it->flags & tfBottom)
			ret = ret && ShiftControl(it->hwnd, 0, dy, false, false);
	}

	// список m_ListLeftBottom сдвигаем только по вертикали
	if ( !m_ListLeftBottom.empty() )
		for(auto it=m_ListLeftBottom.begin(); it!=m_ListLeftBottom.end();
																	++it)
		{
			if (it->flags & tfRight)	// ширина следит за правой границей
				ret = ret && ShiftControl(it->hwnd, dx, dy, false, true);
			else
				ret = ret && ShiftControl(it->hwnd, 0, dy);
		}
	// список m_ListRightTop сдвигаем только по горизонтали
	if ( !m_ListRightTop.empty() )
		for(auto it=m_ListRightTop.begin(); it!=m_ListRightTop.end();
																	++it)
		{
			if (it->flags & tfBottom)	// высота следит за нижней границей
				ret = ret && ShiftControl(it->hwnd, dx, dy, true, false);
			else
				ret = ret && ShiftControl(it->hwnd, dx, 0);
		}
	// список m_ListRightBottom сдвигаем и по вертикали и по горизонтали
	if ( !m_ListRightBottom.empty() )
		for(auto it=m_ListRightBottom.begin(); it!=m_ListRightBottom.end();
																	++it)
			ret = ret && ShiftControl(it->hwnd, dx, dy);
	
	// новая область для масштабируемых элементов
	m_rectReSize.right += dx;
	m_rectReSize.bottom += dy;

	// масштабирую элементы из списка m_ListReSize
	if ( !m_ListReSize.empty() )
		for(auto it=m_ListReSize.begin(); it!=m_ListReSize.end(); ++it)
			ret = ret && ScaleControl(&(*it));

	// После масштабируемых элементов сдвигаю элементы с мастером
	if ( !m_ListWithMaster.empty() )
		for(auto it=m_ListWithMaster.begin(); it!=m_ListWithMaster.end(); ++it)
		{
			if (it->type == tipWithMaster)
				ret = ret && ShiftWithMaster(&(*it), dx, dy);
			else
				ret = ret && ScaleWithMaster(&(*it), dx, dy);
		}
	
	m_HeightAll = cy;		// новая высота окна
	m_WidthAll  = cx;		// новая ширина окна
	return ret;
}
//------------------------------------------------------------------------------
bool CAutoScaleXY::ShiftWithMaster(
	ControlWithMaster *par, int dx, int dy,
	HWND hCont, HWND hContMaster)
{
	HWND cont, contMaster;
	if (hCont != NULL)
		cont = hCont;
	else
		cont = par->hwnd;
	
	if (hContMaster != NULL)
		contMaster = hContMaster;
	else
		contMaster = par->hwndMaster;
	bool ret = (cont != NULL) && (contMaster != NULL);
	if (ret)
	{
		int m(0);
		RECT rect, rectMaster;
		GetWindowRect(cont, &rect);
		POINT pt;
		pt.x = rect.left;
		pt.y = rect.top;
		ScreenToClient(m_hParent, &pt);
		rect.left = pt.x;
		rect.top = pt.y;
		pt.x = rect.right;
		pt.y = rect.bottom;
		ScreenToClient(m_hParent, &pt);
		rect.right = pt.x;
		rect.bottom = pt.y;

		GetWindowRect(contMaster, &rectMaster);
		POINT ptMaster;
		ptMaster.x = rectMaster.left;
		ptMaster.y = rectMaster.top;
		ScreenToClient(m_hParent, &ptMaster);
		rectMaster.left = ptMaster.x;
		rectMaster.top = ptMaster.y;
		ptMaster.x = rectMaster.right;
		ptMaster.y = rectMaster.bottom;
		ScreenToClient(m_hParent, &ptMaster);
		rectMaster.right = ptMaster.x;
		rectMaster.bottom = ptMaster.y;

		switch (par->flags)
		{
		case tfLeft:
			rect.top    += dy;
			rect.bottom += dy;
			if (par->flagsMaster == tfLeft)
			{// левую границу элемента привязываю к левой границы мастера
				m = rectMaster.left + par->shift;
			}
			else if (par->flagsMaster == tfRight)
			{// левую границу элемента привязываю к правой границы мастера
				m = rectMaster.right + par->shift;
			}
			rect.right += m - rect.left;
			rect.left = m;
			break;
		case tfRight:
			rect.top    += dy;
			rect.bottom += dy;
			if (par->flagsMaster == tfLeft)
			{// правую границу элемента привязываю к левой границы мастера
				m = rectMaster.left + par->shift;
			}
			else if (par->flagsMaster == tfRight)
			{// правую границу элемента привязываю к правой границы мастера
				m = rectMaster.right + par->shift;
			}
			rect.left += m - rect.right;
			rect.right = m;
			break;
		case tfTop:
			rect.left  += dx;
			rect.right += dx;
			if (par->flagsMaster == tfTop)
			{// верхнюю границу элемента привязываю к верхней границы мастера
				m = rectMaster.top + par->shift;
			} 
			else if (par->flagsMaster == tfBottom)
			{// верхнюю границу элемента привязываю к нижней границы мастера
				m = rectMaster.bottom + par->shift;
			}
			rect.bottom += m - rect.top;
			rect.top = m;
			break;
		case tfBottom:
			rect.left  += dx;
			rect.right += dx;
			if (par->flagsMaster == tfTop)
			{// нижнюю границу элемента привязываю к верхней границы мастера
				m = rectMaster.top + par->shift;
			} 
			else if (par->flagsMaster == tfBottom)
			{// нижнюю границу элемента привязываю к нижней границы мастера
				m = rectMaster.bottom + par->shift;
			}
			rect.top += m - rect.bottom;
			rect.bottom = m;
//			break;
		}
		MoveWindow(cont, rect.left, rect.top, rect.right - rect.left, rect.bottom - rect.top, true);
	}
	return ret;
}
//------------------------------------------------------------------------------
bool CAutoScaleXY::ScaleWithMaster(ControlWithMaster *par, int dx, int dy)
{
	HWND cont = par->hwnd;
	HWND contMaster = par->hwndMaster;
	bool ret = (cont != NULL) && (contMaster != NULL);
	if (ret)
	{
		RECT rect, rectMaster;
		GetWindowRect(cont, &rect);
		POINT pt;
		pt.x = rect.left;
		pt.y = rect.top;
		ScreenToClient(m_hParent, &pt);
		rect.left = pt.x;
		rect.top = pt.y;
		pt.x = rect.right;
		pt.y = rect.bottom;
		ScreenToClient(m_hParent, &pt);
		rect.right = pt.x;
		rect.bottom = pt.y;
		
		GetWindowRect(contMaster, &rectMaster);
		POINT ptMaster;
		ptMaster.x = rectMaster.left;
		ptMaster.y = rectMaster.top;
		ScreenToClient(m_hParent, &ptMaster);
		rectMaster.left = ptMaster.x;
		rectMaster.top = ptMaster.y;
		ptMaster.x = rectMaster.right;
		ptMaster.y = rectMaster.bottom;
		ScreenToClient(m_hParent, &ptMaster);
		rectMaster.right = ptMaster.x;
		rectMaster.bottom = ptMaster.y;

		switch (par->type)
		{
		case tipLeftTop:	// левый верхний угол не двигается
			//	rect.left += 0;
			//	rect.top  += 0;
			if (par->flags == tfRight)
			{
				rect.bottom += dy;
				if (par->flagsMaster == tfLeft)
				{// правая граница элемента привязывается к левой границы мастера
					rect.right = rectMaster.left + par->shift;
				}
				else if (par->flagsMaster == tfRight)
				{// правая граница элемента привязывается к правой границы мастера
					rect.right = rectMaster.right + par->shift;
				}
			}
			else if (par->flags == tfBottom)
			{
				rect.right += dx;
				if (par->flagsMaster == tfLeft)
				{// нижняя граница элемента привязывается к левой границы мастера
					rect.bottom = rectMaster.left + par->shift;
				}
				else if (par->flagsMaster == tfRight)
				{// нижняя граница элемента привязывается к правой границы мастера
					rect.bottom = rectMaster.right + par->shift;
				}
			}
			break;
		case tipLeftBottom:		// левая граница на месте, нижняя двигается
			//	rect.left += 0;
			rect.bottom += dy;
			if (par->flags == tfRight)
			{
				rect.top += dy;
				if (par->flagsMaster == tfLeft)
				{// правую границу элемента привязываю к левой границе мастера
					rect.right = rectMaster.left + par->shift;
				}
				else if (par->flagsMaster == tfRight)
				{// правую границу элемента привязываю к правой границе мастера
					rect.right = rectMaster.right + par->shift;
				}
			}
			else if (par->flags == tfTop)
			{
				rect.right += dx;
				if (par->flagsMaster == tfTop)
				{// верхнюю границу элемента привязываю к верхней границе мастера
					rect.top = rectMaster.top + par->shift;
				}
				else if (par->flagsMaster == tfBottom)
				{// верхнюю границу элемента привязываю к нижней границе мастера
					rect.top = rectMaster.bottom + par->shift;
				}
			}
			break;
		case tipRightTop:
			//	rect.top += 0;
			rect.right += dx;
			if (par->flags == tfLeft)
			{
				rect.bottom += dy;
				if (par->flagsMaster == tfLeft)
				{// левую границу элемента привязываю к левой границе мастера
					rect.left = rectMaster.left + par->shift;
				}
				else if (par->flagsMaster == tfRight)
				{// левую границу элемента привязываю к правой границе мастера
					rect.left = rectMaster.right + par->shift;
				}
			}
			else if (par->flags == tfBottom)
			{
				rect.left += dx;
				if (par->flagsMaster == tfTop)
				{// верхнюю границу элемента привязываю к верхней границе мастера
					rect.bottom = rectMaster.top + par->shift;
				}
				else if (par->flagsMaster == tfBottom)
				{// верхнюю границу элемента привязываю к нижней границе мастера
					rect.bottom = rectMaster.bottom + par->shift;
				}
			}
			break;
		case tipRightBottom:
		{
			rect.right += dx;
			rect.bottom += dy;
			if (par->flags == tfLeft)
			{
				rect.top += dy;
				if (par->flagsMaster == tfLeft)
				{// левую границу элемента привязываем к правой границе мастера
					rect.left = rectMaster.left + par->shift;
				}
				else if (par->flagsMaster == tfRight)
				{// левую границу элемента привязываем к правой границе мастера
					rect.left = rectMaster.right + par->shift;
				}
			}
			else if (par->flags == tfTop)
			{
				rect.left += dx;
				if (par->flagsMaster == tfTop)
				{// верхнюю границу элемента привязываем к верхней границе мастера
					rect.top = rectMaster.top + par->shift;
				}
				else if (par->flagsMaster == tfBottom)
				{// верхнюю границу элемента привязываем к нижней границе мастера
					rect.top = rectMaster.bottom + par->shift;
				}
			}
			break;
		}
		case tipReSize:
		{
			LONG xy = m_rectReSize.bottom - m_rectReSize.top;
			LONG xx = m_rectReSize.right - m_rectReSize.left;

			if (par->flagsMaster == tfBottom)
			{	// верхнюю границу элемента привязываю к нижней границе мастера
				rect.top = rectMaster.bottom + par->shift;
			}
			else
				rect.top = m_rectReSize.top + LONG(par->resize._top * xy);
			

			if (par->flagsMaster == tfRight)
			{	// левую границу элемента привязываю к правой границе мастера
				rect.left = rectMaster.right + par->shift;
				// сохраняем верхнюю границу мастера
				rect.top = rectMaster.top;
			}
			else
				rect.left = m_rectReSize.left + LONG(par->resize._left * xx);
			
			if ((par->resize.flags & tfConstHeight))	
				// не меняем высоту
				rect.bottom = rect.top + par->resize._default_height;
			else
			{	// расширяем
				if ((par->resize.flags & tfBottom))
					rect.bottom += dy;
				else
					rect.bottom = m_rectReSize.top + LONG(par->resize._top * xy) + LONG(par->resize._height * xy);
			}

			// меняем ширину экрана
			if ((par->resize.flags & tfRight))
				rect.right += dx;
			else
				rect.right = m_rectReSize.left + LONG(par->resize._left * xx) + LONG(par->resize._width * xx);			
		}
		}
		MoveWindow(cont, rect.left, rect.top, rect.right - rect.left, rect.bottom - rect.top, true);
	}
	return ret;
}
//------------------------------------------------------------------------------
// Проверка существования элементов из списка ListReSize.
//-----------------------------------------------------------------
bool CAutoScaleXY::CheckControls()
{
	bool ret = true;
	for(auto it=m_ListReSize.begin(); it!=m_ListReSize.end(); ++it)
	{
		if ( NULL != it->hwnd )
			ret = ret && ( it->flags & tfLeft ) && ( it->flags & tfRight );
		else
		{
			ret = false;
			break;
		}
	}
	return ret;
}
//------------------------------------------------------------------------------
bool CAutoScaleXY::ScaleControl(UseControlReSize *puc)
{	// масштабирую элемент из списка m_ListReSize
	bool ret;
	float xy(0.f);
	HWND cont = puc->hwnd;
	if (cont)
	{
		xy = float(m_rectReSize.bottom - m_rectReSize.top);
		puc->rect.top = m_rectReSize.top   + LONG( puc->_top    * xy );
		
		if (!(puc->flags & tfConstHeight))
			puc->rect.bottom = puc->rect.top + LONG(puc->_height * xy);
		else
			puc->rect.bottom = puc->rect.top + puc->_default_height;

		xy = float(m_rectReSize.right - m_rectReSize.left);
		puc->rect.left = m_rectReSize.left + LONG( puc->_left   * xy );
		puc->rect.right = puc->rect.left + LONG( puc->_width * xy );
		TestAndMove(puc);
		ret = true;
	}
	else
		ret = false;
	return ret;
}
//------------------------------------------------------------------------------
// Сдвиг элемента, возможно с растяжением (вниз или вправо)
bool CAutoScaleXY::ShiftControl(
	HWND hwnd,	// hWnd сдвигаемого элемента
	int dx,		// на сколько сдвигаю по Х
	int dy,		// на сколько сдвигаю по Y
	bool bLeft,	// смещать левую сторону
	bool bTop)	// смещать верхнюю сторону
{	// сдвигаю элемент с фиксированными размерами
	bool ret;
	RECT rect;
	HWND cont = hwnd;
	if (cont)
	{
		GetWindowRect(cont, &rect);
		POINT pt;
		pt.x = rect.left;
		pt.y = rect.top;
		ScreenToClient(m_hParent, &pt);
		rect.left = pt.x;
		rect.top = pt.y;
		pt.x = rect.right;
		pt.y = rect.bottom;
		ScreenToClient(m_hParent, &pt);
		rect.right = pt.x;
		rect.bottom = pt.y;
		if (bLeft)
			rect.left += dx;
		rect.right += dx;
		if (bTop)
			rect.top += dy;
		rect.bottom += dy;
		MoveWindow(cont, rect.left, rect.top, rect.right - rect.left, rect.bottom - rect.top, true);
		ret = true;
	}
	else
		ret = false;
	return ret;
}
//------------------------------------------------------------------------------
// Сортировка масштабируемых элементов в списке по расположению сверху.
// Первым в списке будет самый верхний, последним - самый нижний.
// Проверку на существование элементов не ставлю!!!
void CAutoScaleXY::SortControls()
{// ищу самый нижний элемент и помещаю в начало нового списка
	std::list<UseControlReSize>::iterator iterTop;
	int numCont = m_ListReSize.size() - 1;	// последний элемент добавлю руками

	std::list<UseControlReSize> *list = new std::list<UseControlReSize>;
	for(int i=0; i<numCont; ++i)	// именно numCont, size уменьшается!!!
	{
		int top = -1;
		for(auto it=m_ListReSize.begin(); it!=m_ListReSize.end(); ++it)
		{
			if ( it->rect.top > top )
			{
				iterTop = it;
				top = it->rect.top;
			}
		}
		list->push_front( *iterTop );
		m_ListReSize.erase(iterTop);
	}
	// добавляю последний элемент
	list->push_front( *m_ListReSize.begin() );
	m_ListReSize = *list;
	delete list;
}
//------------------------------------------------------------------------------
// hControl - ID контрола, способ масштабирования, привязка к стороне, ID мастера, 
// flagsMaster - флаг привязки к стороне МАСТЕРА (tfBottom снизу контрола tfRight справа от контрола)
// shift - отступ от мастера
// Вызывается для каждого элемента только в OnInitDialog перед AddControlFinish
// и до OnPostInitDialog()!!!
bool CAutoScaleXY::AddControlWithMaster(HWND hControl, TipContrl type, WORD flags,
	HWND hControlMaster, WORD flagsMaster, long shift)
{
	HWND cont = hControl;
	HWND contMaster = hControlMaster;
	bool ret = (cont != NULL) && (contMaster != NULL);
	if (ret)
	{// проверяю есть или нет такой в списке
		ret = false;
		if ( !m_ListWithMaster.empty() )
		{
			for (auto it=m_ListWithMaster.begin(); it!=m_ListWithMaster.end(); ++it)
			{
				if (it->hwnd == hControl)
				{
					it->flags = flags;
					it->flagsMaster = flagsMaster;
					it->hwndMaster = hControlMaster;
					it->shift = shift;
					it->type = type;
					ret = ShiftWithMaster(&(*it), 0, 0, cont, contMaster);
				}
			}
		}
		if (!ret)
		{// нет такого
			ControlWithMaster cnswm;
			cnswm.flags = flags;
			cnswm.flagsMaster = flagsMaster;
			cnswm.hwnd = hControl;
			cnswm.hwndMaster = hControlMaster;
			cnswm.shift = shift;
			cnswm.type = type;
			cnswm.resize.flags = flags;
			cnswm.resize.hwnd = hControl;
			m_ListWithMaster.push_back(cnswm);
			ret = true;

			if (type == tipReSize)
			{
				RECT rect;
				GetWindowRect(cont, &rect);
				POINT pt;
				pt.x = rect.left;
				pt.y = rect.top;
				ScreenToClient(m_hParent, &pt);
				rect.left = pt.x;
				rect.top = pt.y;
				pt.x = rect.right;
				pt.y = rect.bottom;
				ScreenToClient(m_hParent, &pt);
				rect.right = pt.x;
				rect.bottom = pt.y;
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
// Добавление элемента в соответствующий список на масштабирование.
// Вызывается для каждого элемента только в OnInitDialog перед AddControlFinish
// и до OnPostInitDialog()!!!
bool CAutoScaleXY::AddControl(HWND hControl, TipContrl type, WORD flags)
{	
	bool ret = true;
	RECT rect;
	UseControlReSize ucRe;
	ControlNoSize ucNo;
	HWND cont = hControl;
	if (cont)
	{
		switch (type)
		{
		case tipLeftTop:
			ucNo.hwnd = hControl;
			ucNo.flags = flags;
			m_ListLeftTop.push_back(ucNo);
			break;

		case tipLeftBottom:
			ucNo.hwnd = hControl;
			ucNo.flags = flags;
			m_ListLeftBottom.push_back(ucNo);
			break;

		case tipRightTop:
			ucNo.hwnd = hControl;
			ucNo.flags = flags;
			m_ListRightTop.push_back(ucNo);
			break;

		case tipRightBottom:
			ucNo.hwnd = hControl;
			ucNo.flags = flags;
			m_ListRightBottom.push_back(ucNo);
			break;

		case tipReSize:
			if (m_bNoFinish)
			{
				ucRe.hwnd = hControl;
				ucRe.flags = flags;			
				m_ListReSize.push_back(ucRe);
			}
		//	break; - здесь НЕ нужен

		case tipReSizeNo:
			if (m_bNoFinish)
			{
				GetWindowRect(cont, &rect);
				POINT pt;
				pt.x = rect.left;
				pt.y = rect.top;
				ScreenToClient(m_hParent, &pt);
				rect.left = pt.x;
				rect.top = pt.y;
				pt.x = rect.right;
				pt.y = rect.bottom;
				ScreenToClient(m_hParent, &pt);
				rect.right = pt.x;
				rect.bottom = pt.y;
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
	else
		ret = false;
	return ret;
}
//------------------------------------------------------------------------------
// Окончание регистрации элементов
void CAutoScaleXY::AddControlFinish()
{// проверка элемента на существование не требуется, она есть в AddControl
	float f_Height = 1.f / float(m_rectReSize.bottom - m_rectReSize.top);
	float f_Width = 1.f / float(m_rectReSize.right - m_rectReSize.left);

	// задаю параметры масштабируемым элементам
	for (auto it = m_ListReSize.begin(); it != m_ListReSize.end(); ++it)
	{
		GetWindowRect(it->hwnd, &it->rect);
		POINT pt;
		pt.x = it->rect.left;
		pt.y = it->rect.top;
		ScreenToClient(m_hParent, &pt);
		it->rect.left = pt.x;
		it->rect.top = pt.y;
		pt.x = it->rect.right;
		pt.y = it->rect.bottom;
		ScreenToClient(m_hParent, &pt);
		it->rect.right = pt.x;
		it->rect.bottom = pt.y;
		TestAndMove(&(*it));

		it->_top = float(it->rect.top - m_rectReSize.top) * f_Height;
		it->_height = float(it->rect.bottom - it->rect.top) *
			f_Height;
		it->_left = float(it->rect.left - m_rectReSize.left) * f_Width;
		it->_width = float(it->rect.right - it->rect.left) *
			f_Width;

		it->_default_height = it->rect.bottom - it->rect.top;
	}

	for (auto it = m_ListWithMaster.begin(); it != m_ListWithMaster.end(); ++it)
	{
		GetWindowRect(it->hwnd, &it->resize.rect);
		POINT pt;
		pt.x = it->resize.rect.left;
		pt.y = it->resize.rect.top;
		ScreenToClient(m_hParent, &pt);
		it->resize.rect.left = pt.x;
		it->resize.rect.top = pt.y;
		pt.x = it->resize.rect.right;
		pt.y = it->resize.rect.bottom;
		ScreenToClient(m_hParent, &pt);
		it->resize.rect.right = pt.x;
		it->resize.rect.bottom = pt.y;
		//TestAndMove(&(it->resize));

		it->resize._top = float(it->resize.rect.top - m_rectReSize.top) * f_Height;
		it->resize._height = float(it->resize.rect.bottom - it->resize.rect.top) *
			f_Height;
		it->resize._left = float(it->resize.rect.left - m_rectReSize.left) * f_Width;
		it->resize._width = float(it->resize.rect.right - it->resize.rect.left) *
			f_Width;
		it->resize._default_height = it->resize.rect.bottom - it->resize.rect.top;
	}

	m_bNoFinish = false;
}
//------------------------------------------------------------------------------
void CAutoScaleXY::TestAndMove(UseControlReSize *puc)
{// Установка границ в соответствии с флагами и перерисовка элемента
	if (puc->flags & tfLeft)
		puc->rect.left = m_rectReSize.left;
	if (puc->flags & tfTop)
		puc->rect.top = m_rectReSize.top;
	if (puc->flags & tfRight)
		puc->rect.right = m_rectReSize.right;
	if (puc->flags & tfBottom)
		puc->rect.bottom = m_rectReSize.bottom;

	MoveWindow(puc->hwnd, puc->rect.left, puc->rect.top, puc->rect.right - puc->rect.left, puc->rect.bottom - puc->rect.top, true);
}
//------------------------------------------------------------------------------
// Удаляю элемент из списка масштабируемых m_ListReSize
// Поле удаляемого элемента:
//	1. придаю указанному элементу (в этом случае удаляемый и поглощаемый
//			должны быть соседними, и иметь флаги tfLeft и tfRight):
//			hControlNew != 0.
//	2. пропорционально раздаётся оставшимся ( в этом случае удаляемый
//			элемент должен быть самым нижним:
//			hControlNew = 0.
// Все задействованные элементы должны быть от левой до правой границ
bool CAutoScaleXY::DeleteControl(
	HWND hControlDel,	// удаляемый элемент
	HWND hControlNew)	// элемент, который поглотит удаляемого
{	
	bool ret = false;
	float xy(0.f);
	HWND contDel(nullptr);
	std::list<UseControlReSize>::iterator iterDel, iterNew;

	// проверка, оставшийся в единственном числе элемент удалять нельзя
	if ( m_ListReSize.size() == 1 )
		return false;
	// ищу удаляемый элемент в списке масштабируемых -------------------------
	for(auto it=m_ListReSize.begin(); it!=m_ListReSize.end(); ++it)
	{
		if (it->hwnd == hControlDel)
		{
			iterDel = it;
			contDel = hControlDel;
			if (contDel)
			{
				ret = true;
				break;
			}
		}
	}
	if (!ret)	// элемент не найден
		return false;

	// определяюсь с режимом работы -----------------------------------------
	// поле не удалею, а отдаю другим
	if (hControlNew)
	{// поле удаляемого элемента отдаю соседнему
		ret = false;
		// ищу новый элемент в списке масштабируемых --------------------
		for(auto it=m_ListReSize.begin(); it!=m_ListReSize.end(); ++it)
		{
			if (it->hwnd == hControlNew)
			{
				iterNew = it;
				if ( NULL != hControlNew )
				{
					ret = true;
					break;
				}
			}
		}
		if (!ret)	// элемент не найден
			return false;

		// оба элемента найдены, проверяю их флаги --------------------------
		// оба элемента должны быть от левого края до правого края
		if ( !( (iterDel->flags & tfLeft) &&
				(iterNew->flags & tfLeft) &&
				(iterDel->flags & tfRight) &&
				(iterNew->flags & tfRight) ) )
				return false;

		// добавляю биты верха и низа удаляемого элемента
		iterNew->flags = iterNew->flags | (iterDel->flags & tfTop ) |
											(iterDel->flags & tfBottom );
		iterNew->rect.top = min( iterNew->rect.top,
									iterDel->rect.top );
		iterNew->rect.bottom = max( iterNew->rect.bottom,
									iterDel->rect.bottom );

		// новые параметры новому элементу
		xy = 1.f / float(m_rectReSize.bottom - m_rectReSize.top);
		iterNew->_top = float(iterNew->rect.top - m_rectReSize.top) * xy;
		iterNew->_height = float(iterNew->rect.bottom - iterNew->rect.top) * xy;
		xy = 1.f / float(m_rectReSize.right - m_rectReSize.left);
		iterNew->_left = float(iterNew->rect.left - m_rectReSize.left) * xy;
		iterNew->_width = float(iterNew->rect.right - iterNew->rect.left) * xy;
		
		// завершение, ret уже равен true
		TestAndMove(&(*iterNew));
		ShowWindow(contDel, 0);
		m_ListReSize.erase(iterDel);
	}// if (hControlNew) - поле удаляемого элемента отдаю соседнему

	else
	{// поле удаляемого элемента отдаю всем поровну, растягивая по Y
		// проверка: удаляемый - нижний?
		if (iterDel->rect.bottom != m_rectReSize.bottom)
			return false;

		// проверяю флаги и наличие всех масштабируемых элементов
		if ( !CheckControls() )
			return false;

		// приступаю к действу, ret = true
		ShowWindow(contDel, 0);
		m_ListReSize.erase(iterDel);

		// ищу самый нижний элемент из оставшихся
		int bottom = 0;		
		for(auto it=m_ListReSize.begin(); it!=m_ListReSize.end(); ++it)
		{
			if ( bottom < it->rect.bottom )
			{
				bottom = it->rect.bottom;
				iterNew = it;
			}
		}
		iterNew->flags = iterNew->flags | tfBottom;

		// оставшимся элементам задаю новые параметры по вертикали
		xy = 1.f / float(bottom - m_rectReSize.top);
		for(auto it=m_ListReSize.begin(); it!=m_ListReSize.end(); ++it)
		{
			it->_top = float(it->rect.top - m_rectReSize.top) * xy;
			it->_height = float(it->rect.bottom - it->rect.top) * xy;
		}

		// масштабирую элементы из списка m_ListReSize только по вертикали
		xy = float(m_rectReSize.bottom - m_rectReSize.top);
		for(auto it=m_ListReSize.begin(); it!=m_ListReSize.end(); ++it)
		{
			it->rect.top = m_rectReSize.top   + LONG( it->_top    * xy );
			it->rect.bottom = it->rect.top + LONG( it->_height * xy );
			TestAndMove(&(*it));
		}
	}// else if (hControlNew) - поле удаляемого элемента отдаю всем поровну

	if ( !m_ListWithMaster.empty() )
		ret = AutoScale(m_WidthAll, m_HeightAll);
	return ret;
}
//------------------------------------------------------------------------------
// Добавление 1 или 2 элементов, делением элемента
// Все три элемента должны быть от левой границы, до правой
bool CAutoScaleXY::DivideControl(
	HWND hControlDiv,	// делимый элемент, он остаётся
	bool flagTop,		// где будет делимый элемент: сверху(true), снизу(false)
	HWND hControl1,		// 1-ый новый элемент (соседний с делимым)
	HWND hControl2)		// 2-ой новый элемент (соседний с 1-ым)
{
	bool ret = false;
	int heigth, dh, n;
	HWND contDiv, contNew1, contNew2 = NULL;
	UseControlReSize *puc1 = NULL, *puc2 = NULL;
	std::list<UseControlReSize>::iterator iterDiv;
	// ищу делимый и новые элементы -------------------------------
	for(auto it=m_ListReSize.begin(); it!=m_ListReSize.end(); ++it)
	{
		if ( it->hwnd == hControlDiv )
		{
			iterDiv = it;
			contDiv =hControlDiv;
			if (contDiv)
			{// делимый элемент в списке есть, хорошо
				ret = true;
				break;
			}
		}
	}
	if (!ret)
		return false;
	// hControl1, ret = true
	contNew1 = hControl1;
	if (contNew1)
	{
		for(auto it=m_ListReSize.begin(); it!=m_ListReSize.end(); ++it)
		{
			if (it->hwnd == hControl1)
			{// первый элемент уже есть в списке, плохо
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
		contNew2 = hControl2;
		if (contNew2)
		{
			for(auto it=m_ListReSize.begin(); it!=m_ListReSize.end(); ++it)
			{
				if (it->hwnd == hControl2)
				{// второй элемент уже есть в списке, плохо
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

	puc1 = new UseControlReSize;
	puc1->hwnd = hControl1;
	puc1->flags = tfLeft | tfRight;
	puc1->rect.left = m_rectReSize.left;
	puc1->rect.right = m_rectReSize.right;
	puc1->_left = 0.f;
	puc1->_width = 1.f;
	float f_Height = 1.f / float(m_rectReSize.bottom - m_rectReSize.top);
	// определяю на сколько будем делить -----------------
	if (hControl2)
	{// делим на 3, ret = true
		puc2 = new UseControlReSize;
		puc2->hwnd = hControl2;
		puc2->flags = tfLeft | tfRight;
		puc2->rect.left = m_rectReSize.left;
		puc2->rect.right = m_rectReSize.right;
		puc2->_left = 0.f;
		puc2->_width = 1.f;
		heigth = ( iterDiv->rect.bottom - iterDiv->rect.top ) / 3;
		// dh - разница, может = 0, 1 или 2
		dh = iterDiv->rect.bottom - iterDiv->rect.top - 3 * heigth;
		n = iterDiv->rect.top;

		if (flagTop)
		{// contDiv верхний, его rect.top остался прежним
			n += heigth;
			iterDiv->rect.bottom = n;
			if (dh--)
				n++;
			puc1->rect.top = n;
			n += heigth;
			puc1->rect.bottom = n;
			if (dh)
				n++;
			puc2->rect.top = n;
			puc2->rect.bottom = n + heigth;

			// делимый элемент мог быть нижним, флаг tfBottom передаю puc2
			// если верхним, то делать ничего не надо, верхним и остался
			puc2->flags = puc2->flags | (iterDiv->flags & tfBottom);
			iterDiv->flags = iterDiv->flags & (~tfBottom);
		}
		else
		{			
			// contDiv нижний, его rect.bottom не изменился
			puc2->rect.top = n;
			n += heigth;
			puc2->rect.bottom = n;
			if (dh--)
				n++;
			puc1->rect.top = n;
			n += heigth;
			puc1->rect.bottom = n;
			if (dh)
				n++;
			iterDiv->rect.top = n;

			// делимый элемент мог быть верхним, флаг tfTop передаю puc2
			// если нижним, то им и остался
			puc2->flags = puc2->flags | (iterDiv->flags & tfTop);
			iterDiv->flags = iterDiv->flags & (~tfTop);
		}
		puc2->_top = ( puc2->rect.top - m_rectReSize.top ) * f_Height;
		puc2->_height = ( puc2->rect.bottom - puc2->rect.top ) * f_Height;
		ShowWindow(contNew2, 1);
		TestAndMove(puc2);
		m_ListReSize.push_back(*puc2);
		delete puc2;
	}// if (hControl2) - делим на 3

	else
	{// делим на 2, ret = true
		heigth = ( iterDiv->rect.bottom - iterDiv->rect.top ) / 2;
		// dh - разница, может = 0 или 1
		dh = iterDiv->rect.bottom - iterDiv->rect.top - 2 * heigth;
		n = iterDiv->rect.top;
		if (flagTop)
		{// contDiv верхний, его rect.top остался прежним			
			n += heigth;
			iterDiv->rect.bottom = n;
			if (dh)
				n++;
			puc1->rect.top = n;
			puc1->rect.bottom = n + heigth;

			// делимый элемент мог быть нижним, флаг tfBottom передаю puc1
			// если верхним, то делать ничего не надо, верхним и остался
			puc1->flags = puc1->flags | (iterDiv->flags & tfBottom);
			iterDiv->flags = iterDiv->flags & (~tfBottom);
		}
		else
		{// contDiv нижний, его rect.bottom не изменился
			puc1->rect.top = n;
			n += heigth;
			puc1->rect.bottom = n;
			if (dh)
				n++;
			iterDiv->rect.top = n;

			// делимый элемент мог быть верхним, флаг tfTop передаю puc1
			// если нижним, то им и остался
			puc1->flags = puc1->flags | (iterDiv->flags & tfTop);
			iterDiv->flags = iterDiv->flags & (~tfTop);

		}
	}// else  if (hControl2) - делим на 2
	
	puc1->_top = ( puc1->rect.top - m_rectReSize.top ) * f_Height;
	puc1->_height = ( puc1->rect.bottom - puc1->rect.top ) * f_Height;
	ShowWindow(contNew1, 1);
	TestAndMove(puc1);
	m_ListReSize.push_back(*puc1);

	iterDiv->_top = ( iterDiv->rect.top - m_rectReSize.top ) * f_Height;
	iterDiv->_height = ( iterDiv->rect.bottom - iterDiv->rect.top ) * f_Height;
	TestAndMove(&(*iterDiv));
	delete puc1;

	if ( !m_ListWithMaster.empty() )
		ret = AutoScale(m_WidthAll, m_HeightAll);

	return ret;
}
//------------------------------------------------------------------------------
// Всем масштабируемым элементам придаю одинаковую ширину
bool CAutoScaleXY::EqualHight()
{
	// проверяю флаги и наличие всех масштабируемых элементов
	if ( !CheckControls() )
		return false;

	int num = m_ListReSize.size();
	if (num > 1)
	{
		SortControls();		// сортирую элементы в списке по их расположению
		int height = 0;		//	сверху вниз
		for(auto it=m_ListReSize.begin(); it!=m_ListReSize.end(); ++it)
			height += it->rect.bottom - it->rect.top;

		// dy - среднее расстояние м/д элементами
		int dy = (m_rectReSize.bottom - m_rectReSize.top - height) / (num - 1);
		height /= num;	// средняя высота
		// ddy - остаток (от 0 до num-1)
		int ddy = m_rectReSize.bottom - m_rectReSize.top - height * num - dy * (num - 1);
		int n = m_rectReSize.top;
		float f_Height = 1.f / float(m_rectReSize.bottom - m_rectReSize.top);
		for(auto it=m_ListReSize.begin(); it!=m_ListReSize.end(); ++it)
		{
			it->rect.top = n;
			it->_top = ( it->rect.top - m_rectReSize.top ) * f_Height;
			n += height;
			it->rect.bottom = n;
			it->_height = ( n - it->rect.top ) * f_Height;
			TestAndMove(&(*it));
			n += dy;
			if (ddy--)
				n++;
		}
	}
	return true;
}
//------------------------------------------------------------------------------
// В низ добавляю новый элемент с дескриптором hControl
// Его высота = HeightAsPercentOfMiddle процентов от средней высоты элементов
bool CAutoScaleXY::NewControl(HWND hControl, int HeightAsPercentOfMiddle)
{
	bool ret;
	HWND contNew = hControl;
	if (contNew)
		ret = true;
	else
		return false;
	
	for(auto it=m_ListReSize.begin(); it!=m_ListReSize.end(); ++it)
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

	UseControlReSize *puc = new UseControlReSize;
	puc->flags = tfLeft | tfRight | tfBottom;
	puc->hwnd = hControl;
	puc->rect.left = m_rectReSize.left;
	puc->rect.right = m_rectReSize.right;
	puc->rect.bottom = m_rectReSize.bottom;
	puc->_left = 0.f;
	puc->_width = 1.f;

	// определяю высоту элемента нового элемента,
	// ещё в цикле надо определить нижний элемент и сбросить флаг низа
	int height = 0;		// высота ноовго элемента
	for(auto it=m_ListReSize.begin(); it!=m_ListReSize.end(); ++it)
	{
		height += it->rect.bottom - it->rect.top;
		if (it->rect.bottom == m_rectReSize.bottom)
			it->flags = it->flags & (~tfBottom);
	}
	int num = m_ListReSize.size();
	int Height = m_rectReSize.bottom - m_rectReSize.top;
	// dy расстояние м/у новым элементом и последним старым
	int dy = (Height - height) / num;
	height = int( (float)HeightAsPercentOfMiddle * 0.01f *
					(float)height / (float)(num + 1) );
	puc->rect.top = puc->rect.bottom - height;
	float f_Height = 1.f / (float)Height;

	puc->_top = ( puc->rect.top - m_rectReSize.top ) * f_Height;
	puc->_height = ( puc->rect.bottom - puc->rect.top ) * f_Height;
	MoveWindow(contNew, puc->rect.left, puc->rect.top, puc->rect.right - puc->rect.left, puc->rect.bottom - puc->rect.top, true);
	
	// все старые элементы из списка сжимаю
	float xy = float(Height - height - dy) / (float)Height;
	for(auto it=m_ListReSize.begin(); it!=m_ListReSize.end(); ++it)
	{
		it->_top *= xy;
		it->_height *= xy;

		it->rect.top = m_rectReSize.top + LONG( it->_top * (float)Height );
		it->rect.bottom = it->rect.top +
								LONG( it->_height * (float)Height );
		TestAndMove(&(*it));
	}
	m_ListReSize.push_back(*puc);
	ShowWindow(contNew, 1);
	delete puc;

	if ( !m_ListWithMaster.empty() )
		ret = AutoScale(m_WidthAll, m_HeightAll);
	return ret;
}
//------------------------------------------------------------------------------