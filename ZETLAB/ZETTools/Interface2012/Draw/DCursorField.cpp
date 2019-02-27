//------------------------------------------------------------------------------
// DCursor.cpp : файл реализации
//------------------------------------------------------------------------------
#include "stdafx.h"
#include <Interface2012\Draw\DCursorField.h>

int g_nCursorField = 0;

CDCursorField::CDCursorField(CRect rect, int qnt, bool *pFlagIsChange, double *pTimeAnimation) :
CCustomDraw(rect, pFlagIsChange, pTimeAnimation)
{
    m_pParamDraw = &m_param;	// обеспечение ОБЩИХ параметров
    m_param.nElement = ++g_nCursorField;
    SetNameWithNumber(L"CursorField");
	m_rect = rect;
    m_param.m_rect=rect;
    m_qntOfCursors = qnt;
    m_currentCursor = 0;
    m_lineLength = 0;
    currentCursorPosition = CPoint((m_param.m_rect.left+m_param.m_rect.right)/2.0,
        (m_param.m_rect.top+m_param.m_rect.bottom)/2.0);

    for(int i = 0; i < qnt; i++)
    {
        if(rect.Width() >= rect.Height())
            #ifdef _ACTIVEX_ZIS
                m_pCursor.push_back(new CDetail(long(39),
                (m_param.m_rect.left+m_param.m_rect.right)/2.0,
                (m_param.m_rect.top+m_param.m_rect.bottom)/2.0, 0, 0, 
                m_rect.Height(), m_rect.Height(), 50));
            #else
                m_pCursor.push_back(new CDetail("$(ZetToolsDir)Interface2012\\Models\\Слайдер\\Ручка ползунка.stl",
                (m_param.m_rect.left+m_param.m_rect.right)/2.0,
                (m_param.m_rect.top+m_param.m_rect.bottom)/2.0, 0, 0, 
                m_rect.Height(), m_rect.Height(), 50));
            #endif

        
        if(rect.Width() <= rect.Height())
#ifdef _ACTIVEX_ZIS
            m_pCursor.push_back(new CDetail(long(39),
            (m_param.m_rect.left+m_param.m_rect.right)/2.0,
            (m_param.m_rect.top+m_param.m_rect.bottom)/2.0, 0, 0, 
            m_rect.Width(), m_rect.Width(), 50));
#else
            m_pCursor.push_back(new CDetail("$(ZetToolsDir)Interface2012\\Models\\Слайдер\\Ручка ползунка.stl",
            (m_param.m_rect.left+m_param.m_rect.right)/2.0,
            (m_param.m_rect.top+m_param.m_rect.bottom)/2.0, 0, 0, 
            m_rect.Width(), m_rect.Width(), 50));
#endif
    }
};
//------------------------------------------------------------------------------
CDCursorField::~CDCursorField()
{
	for (unsigned int i = 0; i < m_pCursor.size(); i++)
	{ 
		delete m_pCursor[i];
		m_pCursor[i] = nullptr;
	}
};
//------------------------------------------------------------------------------
void CDCursorField::Resize(CRect rect)
{
	m_param.m_rect = rect;
	m_rect = rect;
}
//------------------------------------------------------------------------------
void CDCursorField::SetActiveCursor(int id)
{
    m_currentCursor = id;
}
//------------------------------------------------------------------------------
void CDCursorField::SetCursorPosition(CPoint point)
{
    currentCursorPosition = point;
    m_pCursor[m_currentCursor]->SetOffset(point.x, point.y, 5.0);
	IsChange();
}
//------------------------------------------------------------------------------
void CDCursorField::SetLineLength(double val)
{
    m_lineLength = val;
}
//------------------------------------------------------------------------------
void CDCursorField::OnDraw()
{
    glEnable(GL_COLOR_MATERIAL);
    glDisable(GL_LIGHTING);
    DrawField();
    DrawLine();
    glEnable(GL_LIGHTING);
    glDisable(GL_COLOR_MATERIAL);
    DrawCursor();
    
};
//------------------------------------------------------------------------------
void CDCursorField::DrawField()
{
#ifdef _ACTIVEX2012
	if(m_param.m_rect.Width() >= m_param.m_rect.Height())
	{
		glBegin(GL_QUADS);
		glColor3ub(216, 209, 174);
		glVertex3f(m_param.m_rect.left, m_param.m_rect.top, 4);
		glVertex3f(m_param.m_rect.left, m_param.m_rect.bottom, 4);
		glVertex3f(m_param.m_rect.right, m_param.m_rect.bottom, 4);
		glVertex3f(m_param.m_rect.right, m_param.m_rect.top, 4);
		glEnd();
	}

	if(m_param.m_rect.Width() <= m_param.m_rect.Height())
	{
		glBegin(GL_QUADS);
		glColor3ub(216, 209, 174);
		glVertex3f(m_param.m_rect.left, m_param.m_rect.top, 4);
		glVertex3f(m_param.m_rect.left, m_param.m_rect.bottom, 4);
		glVertex3f(m_param.m_rect.right, m_param.m_rect.bottom, 4);
		glVertex3f(m_param.m_rect.right, m_param.m_rect.top, 4);
		glEnd();
	}
#else
    if(m_param.m_rect.Width() >= m_param.m_rect.Height())
    {
        glBegin(GL_QUADS);
        glColor3ub(80, 80, 80);
        glVertex3f(m_param.m_rect.left, m_param.m_rect.top, 4);
        glColor3ub(200, 200, 200);
        glVertex3f(m_param.m_rect.left, m_param.m_rect.bottom, 4);
        glColor3ub(200, 200, 200);
        glVertex3f(m_param.m_rect.right, m_param.m_rect.bottom, 4);
        glColor3ub(80, 80, 80);
        glVertex3f(m_param.m_rect.right, m_param.m_rect.top, 4);
        glEnd();
    }

    if(m_param.m_rect.Width() <= m_param.m_rect.Height())
    {
        glBegin(GL_QUADS);
        glColor3ub(200, 200, 200);
        glVertex3f(m_param.m_rect.left, m_param.m_rect.top, 4);
        glColor3ub(200, 200, 200);
        glVertex3f(m_param.m_rect.left, m_param.m_rect.bottom, 4);
        glColor3ub(80, 80, 80);
        glVertex3f(m_param.m_rect.right, m_param.m_rect.bottom, 4);
        glColor3ub(80, 80, 80);
        glVertex3f(m_param.m_rect.right, m_param.m_rect.top, 4);
        glEnd();
    }
#endif
}
//------------------------------------------------------------------------------
void CDCursorField::DrawCursor()
{
    glPushMatrix();
    glLoadIdentity();
    glMaterialfv(GL_FRONT, GL_AMBIENT, m_param.mat_ambient);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, m_param.mat_diffuse);
    glMaterialfv(GL_FRONT, GL_EMISSION, m_param.mat_emission);
    glMaterialfv(GL_FRONT, GL_SPECULAR, m_param.mat_specular);
    glMaterialf(GL_FRONT, GL_SHININESS, m_param.mat_shininess);
    glEnable(GL_COLOR_MATERIAL);
    for(int i = 0; i < m_qntOfCursors; i++){
        glColor3ub(GetRValue(m_param.palette[i]), GetGValue(m_param.palette[i]), GetBValue(m_param.palette[i]));
        m_pCursor[i]->Draw(m_pCursor[i]->ReadFacet(), m_pCursor[i]->GetQntPlanes());
    }
    glDisable(GL_COLOR_MATERIAL);
    glPopMatrix();
};
//------------------------------------------------------------------------------
void CDCursorField::DrawLine()
{
	float x(0.f), y(0.f), z(0.f);
    glLineWidth(1);
    glBegin(GL_LINES);
    for(int i = 0; i < m_qntOfCursors; i++){
        m_pCursor[i]->GetOffset(x, y, z);
        if(m_param.m_rect.Width() >= m_param.m_rect.Height())
        {
            glColor3ub(GetRValue(m_param.palette[i]), GetGValue(m_param.palette[i]), GetBValue(m_param.palette[i]));
            glVertex3f(x, m_param.m_rect.bottom, 10);
            glVertex3f(x, m_param.m_rect.bottom + m_lineLength, 10);
        }
                        
        if(m_param.m_rect.Width() <= m_param.m_rect.Height())
        {
            glColor3ub(GetRValue(m_param.palette[i]), GetGValue(m_param.palette[i]), GetBValue(m_param.palette[i]));
            glVertex3f(m_param.m_rect.left, y, 10);
            glVertex3f(m_param.m_rect.left - m_lineLength, y, 10);
        }
        
    }
    glEnd();
};
//------------------------------------------------------------------------------
void CDCursorField::Clear()
{
	for (unsigned int i = 0; i < m_pCursor.size(); i++)
		m_pCursor[i]->Clear();
}