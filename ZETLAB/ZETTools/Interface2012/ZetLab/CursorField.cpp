//------------------------------------------------------------------------------
// CursorField.cpp : файл реализации
//------------------------------------------------------------------------------
#include "stdafx.h"
#include <Interface2012\ZetLab\CursorField.h>

CCursorField::CCursorField(CCustomContainer *owner, CRect rect, int lineLength, int qntOfCursors, bool invertation) : CCustomElement(owner)
{
    if (m_pOwner)
    {
        if(qntOfCursors > QNTOFCOLORS)
            m_qntOfCursors = QNTOFCOLORS;
        else
            m_qntOfCursors = qntOfCursors;

        m_bFlag = false;
        m_rect = rect;
        DataType = true;
		m_invertation = invertation;

        m_pDCursorField = new CDCursorField(rect, qntOfCursors, m_pOwner->GetPointerToIsChange(), m_pOwner->GetPointerToTime());
        m_pDClass = m_pDCursorField;

        m_pDCursorField->SetLineLength(lineLength);

        if(rect.Width() >= rect.Height())
        {
            for(int i = 0; i < qntOfCursors; i++){
                cursorRect.push_back(CRect(rect.left, rect.top, rect.left+rect.Height(), rect.bottom));
                m_pDCursorField->SetActiveCursor(i);
                m_activeCursor = i;
                m_pDCursorField->SetCursorPosition(CPoint(rect.left+rect.Height()/2.0, rect.top+rect.Height()/2.0));
            }

        }

        if(rect.Width() <= rect.Height())
        {
            for(int i = 0; i < qntOfCursors; i++){
                cursorRect.push_back(CRect(rect.left, rect.top, rect.right, rect.top + rect.Width()));
                m_pDCursorField->SetActiveCursor(i);
                m_activeCursor = i;
                m_pDCursorField->SetCursorPosition(CPoint(rect.left+rect.Width()/2.0, rect.top+rect.Width()/2.0));
            }
        }
    }
};
//------------------------------------------------------------------------------
CCursorField::~CCursorField()
{
	if (m_pDCursorField)
	{
		delete m_pDCursorField;
		m_pDCursorField = nullptr;
	}
};
//------------------------------------------------------------------------------
void CCursorField::Resize(CRect rect)
{
	if (rect.Width() >= rect.Height())
	{
		for (int i = 0; i < m_qntOfCursors; i++){
			cursorRect[i] = CRect((cursorRect[i].left - m_rect.left)
				/ m_rect.Width() * rect.Width(), rect.top, rect.left - (cursorRect[i].left - m_rect.left)
				/ m_rect.Width() * rect.Width() + rect.Height(), rect.bottom);
			m_pDCursorField->SetActiveCursor(i);
			m_activeCursor = i;
			m_pDCursorField->SetCursorPosition(CPoint(rect.left - (cursorRect[i].left - m_rect.left)
				/ m_rect.Width() * rect.Width() + rect.Height() / 2.0, rect.top + rect.Height() / 2.0));
		}

	}

	if (rect.Width() <= rect.Height())
	{
		for (int i = 0; i < m_qntOfCursors; i++){
			cursorRect[i] = CRect(rect.left, rect.top - (m_rect.top - cursorRect[i].top)
				/ m_rect.Height() * rect.Height(), rect.right, rect.top - (m_rect.top - cursorRect[i].top)
				/ m_rect.Height() * rect.Height() + rect.Width());
			m_pDCursorField->SetActiveCursor(i);
			m_activeCursor = i;
			m_pDCursorField->SetCursorPosition(CPoint(rect.left + rect.Width() / 2.0, rect.top - (m_rect.top - cursorRect[i].top)
				/ m_rect.Height() * rect.Height() + rect.Width() / 2.0));
		}
	}
	m_rect = rect;
	m_pDCursorField->Resize(rect);
}
//------------------------------------------------------------------------------
void CCursorField::ResizeLine(int lineLength)
{
	m_pDCursorField->SetLineLength(lineLength);
}
//------------------------------------------------------------------------------
void CCursorField::SetStep(double step)
{
    m_Data.cdStep = step;
};
//------------------------------------------------------------------------------
void CCursorField::SetStep(TimeInterval step)
{
    m_Data.cdTimeStep = step;
    DataType = false;
};
//------------------------------------------------------------------------------
void CCursorField::SetMinMax(double min, double max)
{
    m_Data.cdCurrentMin = min;
    m_Data.cdCurrentMax = max;
};
//------------------------------------------------------------------------------
void CCursorField::SetPointOfStartVal(double val)
{
    m_Data.cdPointOfStartVal = val;
}
//------------------------------------------------------------------------------
int CCursorField::GetCursorPosition(int cursorId)
{
    if(m_rect.Width() >= m_rect.Height())
        return cursorRect[cursorId].CenterPoint().x;

    if(m_rect.Width() <= m_rect.Height())
        return cursorRect[cursorId].CenterPoint().y;

    return -1;
};
//------------------------------------------------------------------------------
int CCursorField::GetActiveCursor()
{
    return m_activeCursor;
};
//------------------------------------------------------------------------------
void CCursorField::OnDraw()
{
    m_pDCursorField->OnDraw();
    CCustomElement::OnDraw();
}
//------------------------------------------------------------------------------
void CCursorField::OnLButtonDown(UINT nFlags, CPoint point)
{
        m_bFlag = true;
        for(int i = m_qntOfCursors-1; i >= 0; i--)
        {
            if(PointInRect(point, &cursorRect[i]))
            {
                m_pDCursorField->SetActiveCursor(i);
                m_activeCursor = i;
            }
        }
        CCustomElement::OnLButtonDown(nFlags, point);
}
//------------------------------------------------------------------------------
void CCursorField::OnLButtonUp(UINT nFlags, CPoint point)
{
    m_bFlag = false;
    CCustomElement::OnLButtonUp(nFlags, point);
}
//------------------------------------------------------------------------------
void CCursorField::OnMouseLeave()
{
    int a = 0;
    CCustomElement::OnMouseLeave();
}
//------------------------------------------------------------------------------
void CCursorField::OnMouseMove(UINT nFlags, CPoint point)
{
    
    if(nFlags == MK_LBUTTON)
		CalcCursorPosition(point);
    CCustomElement::OnMouseMove(nFlags, point);
}
//------------------------------------------------------------------------------
void CCursorField::CalcCursorPosition(CPoint point)
{
	double StartPoint(0.0), Step(0.0), newPoint(0.0), tmp(0.0);

	if (m_rect.Width() >= m_rect.Height())
	{
		if (m_Data.cdPointOfStartVal == m_Data.cdPointOfStartVal && !m_invertation)	//проверка на NAN
		{
			StartPoint = (m_Data.cdPointOfStartVal - m_Data.cdCurrentMin)
				/ (m_Data.cdCurrentMax - m_Data.cdCurrentMin)*m_rect.Width(); //координата по оси Х первой точки видимой части графика
			Step = m_Data.cdStep
				/ (m_Data.cdCurrentMax - m_Data.cdCurrentMin)*m_rect.Width();
			newPoint = (int)((point.x - StartPoint - m_rect.left) / Step + 0.5)*Step + StartPoint + m_rect.left; // округлять в зависимости от ближайшей точки
		}

		else
		{
			newPoint = point.x;
		}

		cursorRect[m_activeCursor] =
			CRect(newPoint - m_rect.Height() / 2.0, cursorRect[m_activeCursor].top,
			newPoint + m_rect.Height() / 2.0, cursorRect[m_activeCursor].bottom);
		m_pDCursorField->SetCursorPosition(CPoint(newPoint, m_rect.top + m_rect.Height() / 2.0));
	}

	if (m_rect.Width() <= m_rect.Height())
	{
		if (m_Data.cdPointOfStartVal == m_Data.cdPointOfStartVal && m_invertation)	//проверка на NAN
		{
			StartPoint = (m_Data.cdPointOfStartVal - m_Data.cdCurrentMin)
				/ (m_Data.cdCurrentMax - m_Data.cdCurrentMin)*m_rect.Height(); //координата по оси Х первой точки видимой части графика
			Step = m_Data.cdStep
				/ (m_Data.cdCurrentMax - m_Data.cdCurrentMin)*m_rect.Height();
			newPoint = -(int)((m_rect.bottom + StartPoint - point.y) / Step + 0.5)*Step + StartPoint + m_rect.bottom; // округлять в зависимости от ближайшей точки
		}

		else
		{
			newPoint = point.y;
		}

		cursorRect[m_activeCursor] =
			CRect(cursorRect[m_activeCursor].left, newPoint - m_rect.Width() / 2.0,
			cursorRect[m_activeCursor].right, newPoint + m_rect.Width() / 2.0);
		m_pDCursorField->SetCursorPosition(CPoint(m_rect.left + m_rect.Width() / 2.0, newPoint));
	}
	m_pOwner->DoAction();
}
//------------------------------------------------------------------------------
void CCursorField::Clear ()
{
	m_pDCursorField->Clear();
}