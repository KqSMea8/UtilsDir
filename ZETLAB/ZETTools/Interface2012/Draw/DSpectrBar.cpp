//------------------------------------------------------------------------------
// DOscillPlot.cpp : файл реализации
//------------------------------------------------------------------------------
#include "stdafx.h"
#include <Interface2012\Draw\DSpectrBar.h>

int g_nSpectrBar = 0;

int error;

CDSpectrBar::CDSpectrBar(CRect rect, bool *pFlagIsChange, double *pTimeAnimation) :
CCustomDraw(rect, pFlagIsChange, pTimeAnimation){
    m_pParamDraw = &m_param;	// обеспечение ОБЩИХ параметров
    m_param.nElement = ++g_nSpectrBar;
    SetNameWithNumber(L"SpectrBar");
	//TRACE("CREATE CDSpectrBar %s\n", (CStringA)m_pParamDraw->Name);
    m_param.m_rect=rect;
	m_rect = rect;
    m_param.bMouseChange = true;
}
//------------------------------------------------------------------------------
void CDSpectrBar::Clear (void)
{

}
//------------------------------------------------------------------------------
CDSpectrBar::~CDSpectrBar()
{
	//TRACE("DELETE CDSpectrBar %s\n", (CStringA)m_pParamDraw->Name);
};
//------------------------------------------------------------------------------
void CDSpectrBar::Resize(CRect rect)
{
	m_param.m_rect = rect;
	m_rect = rect;
}
//------------------------------------------------------------------------------
void CDSpectrBar::SetBarVertex(const std::vector<double> &vertex)
{
    m_param.pVertexs.clear();
    m_param.pVertexs = vertex;
    IsChange();
}
//------------------------------------------------------------------------------
void CDSpectrBar::SetOutlineVertex(const std::vector<double> &vertex)
{
    m_param.pOutlineVertexs.clear();
    m_param.pOutlineVertexs = vertex;
}
//------------------------------------------------------------------------------
void CDSpectrBar::DrawPlot()
{
    int matrix[4];

    glPushMatrix();
    glLoadIdentity();
    glEnable(GL_SCISSOR_TEST);
	glGetIntegerv(GL_VIEWPORT, matrix);
	glScissor(m_param.m_rect.left, matrix[3] - m_param.m_rect.bottom,
		m_param.m_rect.Width(), m_param.m_rect.Height());
    glPushAttrib(GL_ENABLE_BIT | GL_LIGHTING_BIT);
    glPushClientAttrib(GL_ENABLE_BIT | GL_LIGHTING_BIT);
    glEnableClientState(GL_VERTEX_ARRAY);
    if(!m_param.pOutlineVertexs.empty())
    {
        glColor3ub(80, 80, 80);
        glVertexPointer(3, GL_DOUBLE, 0, (void*)m_param.pOutlineVertexs.data());
        glLineWidth(1);
        glDrawArrays(GL_LINE_STRIP, 0, m_param.pOutlineVertexs.size()/3);
    }
    glColor3ub(1, 203, 247);
    glVertexPointer(3, GL_DOUBLE, 0, (void*)m_param.pVertexs.data());
    glLineWidth(1);
    glDrawArrays(GL_QUADS, 0, m_param.pVertexs.size()/3);
    glPopClientAttrib();
    glPopAttrib();
    glDisable(GL_SCISSOR_TEST);
    glPopMatrix();
}
//------------------------------------------------------------------------------
void CDSpectrBar::OnDraw()
{
    glEnable(GL_COLOR_MATERIAL);
    glDisable(GL_LIGHTING);
    DrawPlot();
    glEnable(GL_LIGHTING);
    glDisable(GL_COLOR_MATERIAL);
}