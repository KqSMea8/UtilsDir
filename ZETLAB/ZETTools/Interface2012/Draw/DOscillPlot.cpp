//------------------------------------------------------------------------------
// DOscillPlot.cpp : файл реализации
//------------------------------------------------------------------------------
#include "stdafx.h"
#include <Interface2012\Draw\DOscillPlot.h>

int g_nOscillPlot = 0;

CDOscillPlot::CDOscillPlot(CRect rect, bool *pFlagIsChange, double *pTimeAnimation) :
CCustomDraw(rect, pFlagIsChange, pTimeAnimation){
    m_pParamDraw = &m_param;	// обеспечение ОБЩИХ параметров
    m_param.nElement = ++g_nOscillPlot;
    SetNameWithNumber(L"OscillPlot");
	//TRACE("CREATE CDOscillPlot %s\n", (CStringA)m_pParamDraw->Name);
    m_param.m_rect = rect;
	m_rect = rect;
    m_param.bMouseChange = true;
}
//------------------------------------------------------------------------------
CDOscillPlot::~CDOscillPlot()
{
	//TRACE("DELETE CDOscillPlot %s\n", (CStringA)m_pParamDraw->Name);
	m_param.Clear();
};
//------------------------------------------------------------------------------
void CDOscillPlot::Resize(CRect rect)
{
	m_param.m_rect = rect;
	m_rect = rect;
}
//------------------------------------------------------------------------------
void CDOscillPlot::SetPlotVertex(const std::vector<double> &vertex)
{
    if(!m_param.pVertexs.empty())
        m_param.pVertexs.clear();
    m_param.pVertexs = vertex;
    IsChange();
}
//------------------------------------------------------------------------------
void CDOscillPlot::DrawPlot()
{
    int matrix[4];

    glPushMatrix();
    glLoadIdentity();
	//glEnable(GL_ALPHA_TEST);
	//glAlphaFunc(GL_GREATER, 0.0);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE);
	glColor4ub(GetRValue(m_param.plotColor), GetGValue(m_param.plotColor), GetBValue(m_param.plotColor), m_param.plotTransparency);
	//glColor3ub(GetRValue(m_param.plotColor), GetGValue(m_param.plotColor), GetBValue(m_param.plotColor));
    glEnable(GL_SCISSOR_TEST);
    glGetIntegerv(GL_VIEWPORT, matrix);
    glScissor(m_param.m_rect.left, matrix[3]-m_param.m_rect.bottom,
        m_param.m_rect.Width(), m_param.m_rect.Height());
    glPushAttrib(GL_ENABLE_BIT | GL_LIGHTING_BIT);
    glPushClientAttrib(GL_ENABLE_BIT | GL_LIGHTING_BIT);
    glEnableClientState(GL_VERTEX_ARRAY);
	glLineWidth(m_param.plotWidth);
    glVertexPointer(3, GL_DOUBLE, 0, (void*)m_param.pVertexs.data());
    glDrawArrays(GL_LINE_STRIP, 0, m_param.pVertexs.size()/3);
    //glDrawArrays(GL_LINES, 0, m_param.pVertexs.size()/3);
    glPopClientAttrib();
    glPopAttrib();
    glDisable(GL_SCISSOR_TEST);
	glDisable(GL_BLEND);
	//glDisable(GL_ALPHA_TEST);
    glPopMatrix();
}
//------------------------------------------------------------------------------
void CDOscillPlot::DrawBacklight()
{
	int matrix[4];

	glPushMatrix();
	glLoadIdentity();
	glEnable(GL_SCISSOR_TEST);
	glGetIntegerv(GL_VIEWPORT, matrix);
	glScissor(m_param.m_rect.left, matrix[3] - m_param.m_rect.bottom,
		m_param.m_rect.Width() - 1, m_param.m_rect.Height() + 1);
	glPushAttrib(GL_ENABLE_BIT | GL_LIGHTING_BIT);
	glPushClientAttrib(GL_ENABLE_BIT | GL_LIGHTING_BIT);
	glEnableClientState(GL_VERTEX_ARRAY);
	glColor3ub(1, 203, 247);
	glVertexPointer(3, GL_DOUBLE, 0, (void*)m_param.pVertexs.data());
	glLineWidth(1);
	glDrawArrays(GL_QUADS, 0, m_param.pVertexs.size() / 3);
	glPopClientAttrib();
	glPopAttrib();
	glDisable(GL_SCISSOR_TEST);
	glPopMatrix();
}
//------------------------------------------------------------------------------
void CDOscillPlot::OnDraw()
{
    glEnable(GL_COLOR_MATERIAL);
    glDisable(GL_LIGHTING);
    DrawPlot();
    glEnable(GL_LIGHTING);
    glDisable(GL_COLOR_MATERIAL);
}
//------------------------------------------------------------------------------
void CDOscillPlot::SetPlotColor(COLORREF color)
{
	m_param.plotColor = color;
	//IsChange();
}
//------------------------------------------------------------------------------
void CDOscillPlot::SetPlotWidth(float width)
{
	m_param.plotWidth = width;
}
//------------------------------------------------------------------------------
void CDOscillPlot::SetPlotTransparency(float transparency)
{
	m_param.plotTransparency = transparency;
}