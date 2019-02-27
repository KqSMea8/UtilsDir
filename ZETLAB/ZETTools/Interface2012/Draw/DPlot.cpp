//------------------------------------------------------------------------------
// DPlot.cpp : файл реализации
//------------------------------------------------------------------------------
#include "stdafx.h"
#include "C:\ZETTools\Interface2012\Draw\DPlot.h"

#include "time.h"

int g_nPlot = 0;
int FlagParam1 = 0;

float z_round_p(float val, int Pow){
    float temp=val/pow(10.0,Pow);
    float middle=int(temp)+0.5;
    if(temp<middle) return (int)temp*pow(10.0,Pow);
    else return (int(temp)+1)*pow(10.0, Pow);
};


CDPlot::CDPlot(CRect rect, bool *pFlagIsChange, double *pTimeAnimation) :
CCustomDraw(rect, pFlagIsChange, pTimeAnimation){
    m_pParamDraw = &m_param;	// обеспечение ОБЩИХ параметров
    m_param.nElement = ++g_nPlot;
    SetNameWithNumber(L"Plot");
	//TRACE("CREATE CDPlot %s\n", (CStringA)m_pParamDraw->Name);
    m_param.m_rect=rect;
    m_param.m_qntPoints = MAX_QNT_OF_POINTS;
    SetSpace(m_param.m_startValue, m_param.m_endValue, m_param.m_step);
    //double X[5] = {0, 0, 0, 10, 20};
    //double Y[5] = {-5, 0, 5, 0, 5};
    //SieveArray(X, Y, 5);
}
//------------------------------------------------------------------------------
CDPlot::~CDPlot()
{
	//TRACE("DELETE CDPlot %s\n", (CStringA)m_pParamDraw->Name);
}
//------------------------------------------------------------------------------
void CDPlot::OnDraw(){
    glDisable(GL_LIGHTING);
    if(m_param.m_plotType == ptOscill)
        DrawPlot(m_param.m_workArrayX, m_param.m_workArrayYmax);
    if(m_param.m_plotType == ptSpectr)
        DrawSpectr(m_param.m_workArrayX, m_param.m_workArrayYmax);
    glEnable(GL_LIGHTING);
};
//------------------------------------------------------------------------------
void CDPlot::DrawPlot(double *DataArrayX, double *DataArrayY){
    int i, j;
    int matrix[4];
    glEnable(GL_SCISSOR_TEST);
    glPushMatrix();
    glLoadIdentity();  
    glColor3ub(m_param.m_plotColor[0], m_param.m_plotColor[1],
        m_param.m_plotColor[2]);
    glLineWidth(2.0f);
    glGetIntegerv(GL_VIEWPORT, matrix);
    glScissor(m_param.m_rect.left+1, matrix[3]-m_param.m_rect.bottom,
        m_param.m_rect.Width()-1, m_param.m_rect.Height());
    glEnable(GL_BLEND);
    //glBlendFunc(GL_DST_COLOR, GL_ONE);//очень бледный, запомнить
    //glBlendFunc(GL_ONE, GL_ONE);
    glBegin(GL_LINE_STRIP);
    for(i = m_param.m_begin, j = m_param.m_begin;  i < m_param.m_qntPoints, j < m_param.m_qntPoints; i++, j++)
	{
        if(j < 0) 
			continue;
        glVertex3f(m_param.m_rect.left+FromWorldToScreen(m_param.m_workArrayX[i] - m_param.m_minCurrentX, tpVertical), 
			m_param.m_rect.bottom - FromWorldToScreen(m_param.m_workArrayYmax[j] - m_param.m_minCurrentY, tpHorizontal), ZDEPTH);
    }
    glEnd();
    glDisable(GL_BLEND);
    glPopMatrix();
    glDisable(GL_SCISSOR_TEST);
}
//------------------------------------------------------------------------------
void CDPlot::DrawSpectr(double *DataArrayX, double *DataArrayY)
{
    int i, j;
    int matrix[4];
    glEnable(GL_SCISSOR_TEST);
    glPushMatrix();
    glLoadIdentity();  
    glLineWidth(2.0f);
    glGetIntegerv(GL_VIEWPORT, matrix);
    glScissor(m_param.m_rect.left+1, matrix[3]-m_param.m_rect.bottom,
        m_param.m_rect.Width()-1, m_param.m_rect.Height());
    glEnable(GL_BLEND);
    //glBlendFunc(GL_DST_COLOR, GL_ONE);//очень бледный, запомнить
    //glBlendFunc(GL_ONE, GL_ONE);
    SieveArray(m_param.m_dataArrayX, m_param.m_dataArrayY, m_param.m_qntPoints);
    if(FromWorldToScreen(m_param.m_workArrayX[1]
    -m_param.m_workArrayX[0], tpVertical)>10)
    {
        glColor3ub(m_param.m_plotColor[0], m_param.m_plotColor[1], 
            m_param.m_plotColor[2]);
        glBegin(GL_QUADS);
        for(i=m_param.m_begin, j=m_param.m_begin;
            i<m_param.m_qntPoints, j<m_param.m_qntPoints; i++, j++){
                if(j<0) continue;
                glVertex3f(m_param.m_rect.left+FromWorldToScreen(
                    m_param.m_workArrayX[i]-m_param.m_minCurrentX
                    -m_param.m_step/2.0, tpVertical)+2, m_param.m_rect.bottom,
                    ZDEPTH);
                glVertex3f(m_param.m_rect.left+FromWorldToScreen(
                    m_param.m_workArrayX[i]-m_param.m_minCurrentX
                    -m_param.m_step/2.0, tpVertical)+2, m_param.m_rect.bottom
                    -FromWorldToScreen(m_param.m_workArrayYmax[j]
                -m_param.m_minCurrentY, tpHorizontal), ZDEPTH);
                glVertex3f(m_param.m_rect.left+FromWorldToScreen(
                    m_param.m_workArrayX[i]-m_param.m_minCurrentX
                    +m_param.m_step/2.0, tpVertical)-2, m_param.m_rect.bottom-
                    FromWorldToScreen(m_param.m_workArrayYmax[j]
                -m_param.m_minCurrentY, tpHorizontal), ZDEPTH);
                glVertex3f(m_param.m_rect.left+FromWorldToScreen(
                    m_param.m_workArrayX[i]-m_param.m_minCurrentX
                    +m_param.m_step/2.0, tpVertical)-2, m_param.m_rect.bottom,
                    ZDEPTH);
        }
        glEnd();

        glColor3ub(m_param.m_plotBorderColor[0], m_param.m_plotBorderColor[1],
            m_param.m_plotBorderColor[2]);
        glBegin(GL_LINE_STRIP);
        for(i=m_param.m_begin, j=m_param.m_begin;
            i<m_param.m_qntPoints, j<m_param.m_qntPoints; i++, j++){
                if(j<0) continue;
            glVertex3f(m_param.m_rect.left+FromWorldToScreen(
                m_param.m_workArrayX[i]-m_param.m_minCurrentX
                -m_param.m_step/2.0, tpVertical)+2,
                m_param.m_rect.bottom, ZDEPTH);
            glVertex3f(m_param.m_rect.left+FromWorldToScreen(
                m_param.m_workArrayX[i]-m_param.m_minCurrentX
                -m_param.m_step/2.0, tpVertical)+2,
                m_param.m_rect.bottom-FromWorldToScreen(
                m_param.m_workArrayYmax[j]-m_param.m_minCurrentY,
                tpHorizontal), ZDEPTH);
            glVertex3f(m_param.m_rect.left+FromWorldToScreen(
                m_param.m_workArrayX[i]-m_param.m_minCurrentX
                +m_param.m_step/2.0, tpVertical)-2,
                m_param.m_rect.bottom-FromWorldToScreen(
                m_param.m_workArrayYmax[j]-m_param.m_minCurrentY,
                tpHorizontal), ZDEPTH);
            glVertex3f(m_param.m_rect.left+FromWorldToScreen(
                m_param.m_workArrayX[i]-m_param.m_minCurrentX
                +m_param.m_step/2.0, tpVertical)-2,
                m_param.m_rect.bottom, ZDEPTH);
        }
        glEnd();
    }

    if(FromWorldToScreen(m_param.m_workArrayX[1]
        -m_param.m_workArrayX[0], tpVertical)<10 && FromWorldToScreen(
        m_param.m_workArrayX[1]-m_param.m_workArrayX[0], tpVertical)>2)
    {
        glColor3ub(m_param.m_plotColor[0], m_param.m_plotColor[1], 
            m_param.m_plotColor[2]);
        glBegin(GL_QUADS);
        for(i=m_param.m_begin, j=m_param.m_begin;
            i<m_param.m_qntPoints, j<m_param.m_qntPoints; i++, j++){
                if(j<0) continue;
                glVertex3f(m_param.m_rect.left+FromWorldToScreen(
                    m_param.m_workArrayX[i]-m_param.m_minCurrentX
                    -m_param.m_step/2.0, tpVertical), m_param.m_rect.bottom,
                    ZDEPTH);
                glVertex3f(m_param.m_rect.left+FromWorldToScreen(
                    m_param.m_workArrayX[i]-m_param.m_minCurrentX
                    -m_param.m_step/2.0, tpVertical), m_param.m_rect.bottom
                    -FromWorldToScreen(m_param.m_workArrayYmax[j]
                -m_param.m_minCurrentY, tpHorizontal), ZDEPTH);
                glVertex3f(m_param.m_rect.left+FromWorldToScreen(
                    m_param.m_workArrayX[i]-m_param.m_minCurrentX
                    +m_param.m_step/2.0, tpVertical), m_param.m_rect.bottom-
                    FromWorldToScreen(m_param.m_workArrayYmax[j]
                -m_param.m_minCurrentY, tpHorizontal), ZDEPTH);
                glVertex3f(m_param.m_rect.left+FromWorldToScreen(
                    m_param.m_workArrayX[i]-m_param.m_minCurrentX
                    +m_param.m_step/2.0, tpVertical), m_param.m_rect.bottom,
                    ZDEPTH);
        }
        glEnd();

        glColor3ub(m_param.m_plotBorderColor[0], m_param.m_plotBorderColor[1],
            m_param.m_plotBorderColor[2]);
        glBegin(GL_LINE_STRIP);
        for(i=m_param.m_begin, j=m_param.m_begin;
            i<m_param.m_qntPoints, j<m_param.m_qntPoints; i++, j++){
                if(j<0) continue;
                glVertex3f(m_param.m_rect.left+FromWorldToScreen(
                    m_param.m_workArrayX[i]-m_param.m_minCurrentX
                    -m_param.m_step/2.0, tpVertical),
                    m_param.m_rect.bottom-FromWorldToScreen(
                    m_param.m_workArrayYmax[j]-m_param.m_minCurrentY,
                    tpHorizontal), ZDEPTH);
                glVertex3f(m_param.m_rect.left+FromWorldToScreen(
                    m_param.m_workArrayX[i]-m_param.m_minCurrentX
                    +m_param.m_step/2.0, tpVertical),
                    m_param.m_rect.bottom-FromWorldToScreen(
                    m_param.m_workArrayYmax[j]-m_param.m_minCurrentY,
                    tpHorizontal), ZDEPTH);
        }
        glEnd();
    }


    if(FromWorldToScreen(
        m_param.m_workArrayX[1]-m_param.m_workArrayX[0], tpVertical)<2)
    {
        glColor3ub(m_param.m_plotColor[0], m_param.m_plotColor[1], 
            m_param.m_plotColor[2]);
        glBegin(GL_QUADS);
        for(i=m_param.m_begin, j=m_param.m_begin;
            i<m_param.m_qntPoints, j<m_param.m_qntPoints; i++, j++){
            if(j<0) continue;
            glVertex3f(m_param.m_rect.left+FromWorldToScreen(
                m_param.m_workArrayX[i]-m_param.m_minCurrentX
                -m_param.m_step/2.0, tpVertical), m_param.m_rect.bottom,
                ZDEPTH);
            glVertex3f(m_param.m_rect.left+FromWorldToScreen(
                    m_param.m_workArrayX[i]-m_param.m_minCurrentX
                    -m_param.m_step/2.0, tpVertical), m_param.m_rect.bottom
                    -FromWorldToScreen(m_param.m_workArrayYmax[j]
                    -m_param.m_minCurrentY, tpHorizontal), ZDEPTH);
            glVertex3f(m_param.m_rect.left+FromWorldToScreen(
                m_param.m_workArrayX[i]-m_param.m_minCurrentX
                +m_param.m_step/2.0, tpVertical), m_param.m_rect.bottom-
                FromWorldToScreen(m_param.m_workArrayYmax[j]
                -m_param.m_minCurrentY, tpHorizontal), ZDEPTH);
            glVertex3f(m_param.m_rect.left+FromWorldToScreen(
                m_param.m_workArrayX[i]-m_param.m_minCurrentX
                +m_param.m_step/2.0, tpVertical), m_param.m_rect.bottom,
                ZDEPTH);
            }
            glEnd();
        }
    glDisable(GL_BLEND);
    glPopMatrix();
    glDisable(GL_SCISSOR_TEST);
}
//------------------------------------------------------------------------------
void CDPlot::SetData(double *DataArrayX, double *DataArrayY, int Quality){
    /*m_param.m_settings = sManual;
    memcpy(m_param.m_dataArrayX, DataArrayX, sizeof(double)*ArraySize);
    memcpy(m_param.m_workArrayX, DataArrayX, sizeof(double)*ArraySize);
    memcpy(m_param.m_dataArrayY, DataArrayY, sizeof(double)*ArraySize);
    memcpy(m_param.m_workArrayYmax, DataArrayY, sizeof(double)*ArraySize);
    m_param.m_qntPoints = ArraySize;*/
    IsChange();
}
//------------------------------------------------------------------------------
void CDPlot::SetData(double *DataArrayY, int Quality){
    /*m_param.m_settings = sAuto;
    memcpy(m_param.m_dataArrayY, DataArrayY, sizeof(double)*ArraySize);
    memcpy(m_param.m_workArrayYmax, DataArrayY, sizeof(double)*ArraySize);*/
    IsChange();
}
//------------------------------------------------------------------------------
void CDPlot::SetStep(double step){
    m_param.m_step = step;
    IsChange();
}
//------------------------------------------------------------------------------
double CDPlot::FromWorldToScreen(double val, TypePosition Type){
    if(Type==tpVertical){
        return val/(m_param.m_maxCurrentX-m_param.m_minCurrentX)
            *(double)(m_param.m_rect.Width());
        //else return val/(m_param.m_maxCurrentLogX-m_param.m_minCurrentLogX)
        //*(m_param.m_rect.Width()-m_param.m_border);
    }
    if(Type==tpHorizontal){
        return val/(m_param.m_maxCurrentY-m_param.m_minCurrentY)
            *(double)(m_param.m_rect.Height());
        //else return val/(m_param.m_maxCurrentLogY-m_param.m_minCurrentLogY)
        //*(m_param.m_rect.Height()-m_param.m_border);
    }
    return -1.0;
};
//------------------------------------------------------------------------------
int CDPlot::SetSpace(double minX, double maxX, double step){
    int i;
    if(step==0){
        return -1;
    }
    m_param.m_startValue = minX;
    m_param.m_endValue = maxX;
    m_param.m_qntPoints = (maxX-minX)/step;
    if(m_param.m_qntPoints>MAX_QNT_OF_POINTS)
         m_param.m_qntPoints = MAX_QNT_OF_POINTS;
    m_param.m_workArrayX[0] = minX;
    for(i=1; i<m_param.m_qntPoints; i++){
        m_param.m_workArrayX[i] = m_param.m_workArrayX[i-1]+step;
    }
    memcpy(m_param.m_dataArrayX, m_param.m_workArrayX, sizeof(double)*m_param.m_qntPoints);
    return m_param.m_qntPoints;
}
//------------------------------------------------------------------------------
void CDPlot::SetBorder(double border){
    m_param.m_border = border;
    IsChange();
}
//------------------------------------------------------------------------------
void CDPlot::SetAxis(double minX, double maxX, double minY, double maxY)
{
    m_param.m_maxCurrentX=maxX;
    m_param.m_minCurrentX=minX;
    m_param.m_maxCurrentY=maxY;
    m_param.m_minCurrentY=minY;
    IsChange();
}
//------------------------------------------------------------------------------
void CDPlot::SetBounds(double minX, double maxX, double minY, double maxY)
{
    m_param.m_leftBound = minX;
    m_param.m_rightBound = maxX;
    m_param.m_bottomBound = minY;
    m_param.m_topBound = maxY;
}
//------------------------------------------------------------------------------
void CDPlot::SetFit(FitType fitTypeX, FitType fitTypeY)
{
    m_param.m_fitTypeX = fitTypeX;
    m_param.m_fitTypeY = fitTypeY;
}
//------------------------------------------------------------------------------
void CDPlot::SetPlotType(PlotType plotType)
{
    m_param.m_plotType = plotType;
}
//------------------------------------------------------------------------------
void CDPlot::OnLButtonDown(UINT nFlags, CPoint point){
    zRect val;
    val=IsMouseInRect(point);
    Shift(val);
    Scale(val);
    IsChange();
}
//------------------------------------------------------------------------------
void CDPlot::SetRectAll(CRect leftShiftRect, CRect rightShiftRect,
    CRect bottomShiftRect, CRect topShiftRect, CRect leftScaleRect,
    CRect rightScaleRect, CRect topScaleRect, CRect bottomScaleRect,
    CRect centerAutoScaleRect){
    m_param.m_rectLeftShift = leftShiftRect;
    m_param.m_rectRightShift = rightShiftRect;
    m_param.m_rectBottomShift = bottomShiftRect;
    m_param.m_rectTopShift = topShiftRect;
    m_param.m_rectLeftScale = leftScaleRect;
    m_param.m_rectRightScale = rightScaleRect;
    m_param.m_rectTopScale = topScaleRect;
    m_param.m_rectBottomScale = bottomScaleRect;
    m_param.m_centerAutoScaleRect = centerAutoScaleRect;
    m_param.m_rectPlot=m_param.m_rect;
    IsChange();
}
//------------------------------------------------------------------------------
zRect CDPlot::IsMouseInRect(CPoint point){
    if(PointInRect(point, &m_param.m_rectLeftShift) == true)
        return gLeftShift;

    if(PointInRect(point, &m_param.m_rectRightShift) == true)
        return gRightShift;

    if(PointInRect(point, &m_param.m_rectTopShift) == true)
        return gTopShift;

    if(PointInRect(point, &m_param.m_rectBottomShift) == true)
        return gBottomShift;

    if(PointInRect(point, &m_param.m_rectRightScale) == true)
        return gRightScale;

    if(PointInRect(point, &m_param.m_rectLeftScale) == true)
        return gLeftScale;

    if(PointInRect(point, &m_param.m_rectTopScale) == true)
        return gTopScale;

    if(PointInRect(point, &m_param.m_rectBottomScale) == true)
        return gBottomScale;

    if(PointInRect(point, &m_param.m_rectPlot) == true)
        return gGridArea;

    if(PointInRect(point, &m_param.m_centerAutoScaleRect) == true)
        return gCenterAutoScale;

    return gNone;
    
};
//------------------------------------------------------------------------------
void CDPlot::Shift(zRect val)
{
    int temp;
    if(val==gLeftShift){
        if((m_param.m_minCurrentX-m_param.m_shiftX) < m_param.m_leftBound){
            temp = m_param.m_leftBound - m_param.m_minCurrentX;
            m_param.m_minCurrentX = m_param.m_leftBound;
            m_param.m_maxCurrentX = m_param.m_maxCurrentX - temp;
            m_param.m_begin = m_param.m_begin - temp
                /(m_param.m_workArrayX[1]-m_param.m_workArrayX[0]);
            return;
        }
        else{
            m_param.m_minCurrentX = m_param.m_minCurrentX-m_param.m_shiftX;
            m_param.m_maxCurrentX = m_param.m_maxCurrentX-m_param.m_shiftX;
            m_param.m_begin = m_param.m_begin - m_param.m_shiftX
            /(m_param.m_workArrayX[1]-m_param.m_workArrayX[0]);
        }
    }
    if(val==gRightShift){
        if((m_param.m_maxCurrentX+m_param.m_shiftX) > m_param.m_rightBound){
            temp = m_param.m_rightBound-m_param.m_maxCurrentX;
            m_param.m_maxCurrentX = m_param.m_rightBound;
            m_param.m_minCurrentX = m_param.m_minCurrentX+temp;
            m_param.m_begin = m_param.m_begin + temp
                /(m_param.m_workArrayX[1]-m_param.m_workArrayX[0]);
        }
        else{
            m_param.m_minCurrentX = m_param.m_minCurrentX+m_param.m_shiftX;
            m_param.m_maxCurrentX = m_param.m_maxCurrentX+m_param.m_shiftX;
            m_param.m_begin = m_param.m_begin + m_param.m_shiftX
            /(m_param.m_workArrayX[1]-m_param.m_workArrayX[0]);
        }
    }
    if(val==gTopShift){
        if((m_param.m_maxCurrentY+m_param.m_shiftY) > m_param.m_topBound){
                temp = m_param.m_topBound - m_param.m_maxCurrentY;
                m_param.m_maxCurrentY = m_param.m_topBound;
                m_param.m_minCurrentY = m_param.m_minCurrentY + temp;
        }
        else{
            m_param.m_minCurrentY = m_param.m_minCurrentY+m_param.m_shiftY;
            m_param.m_maxCurrentY = m_param.m_maxCurrentY+m_param.m_shiftY;
        }
    }
    if(val==gBottomShift){
        if((m_param.m_minCurrentY-m_param.m_shiftY) < m_param.m_bottomBound){
            temp = m_param.m_minCurrentY - m_param.m_bottomBound;
            m_param.m_minCurrentY = m_param.m_bottomBound;
            m_param.m_maxCurrentY = m_param.m_maxCurrentY - temp;
        }
        else{
            m_param.m_minCurrentY = m_param.m_minCurrentY-m_param.m_shiftY;
            m_param.m_maxCurrentY = m_param.m_maxCurrentY-m_param.m_shiftY;
        }
    }
    if(val==gNone) return;
};
//------------------------------------------------------------------------------
void CDPlot::Scale(zRect val)
{
    double minValue, maxValue;

    if(val==gLeftScale){
        minValue = maxValue = (m_param.m_maxCurrentX-m_param.m_minCurrentX)
            *(m_param.m_scaleX-1)/2.0;
        if(m_param.m_fitTypeX == ftLeft)
            m_param.m_maxCurrentX=m_param.m_maxCurrentX+maxValue*2;
        if(m_param.m_fitTypeX == ftCenter)
        {
            m_param.m_minCurrentX=m_param.m_minCurrentX-minValue;
            m_param.m_maxCurrentX=m_param.m_maxCurrentX+maxValue;
            m_param.m_begin = m_param.m_begin - minValue
                /(m_param.m_workArrayX[1]-m_param.m_workArrayX[0]);
        }
         if(m_param.m_fitTypeX == ftRight){
             m_param.m_minCurrentX=m_param.m_minCurrentX-minValue*2;
             m_param.m_begin = m_param.m_begin - 2*minValue
                 /(m_param.m_workArrayX[1]-m_param.m_workArrayX[0]);
         }
        m_param.m_shiftX = m_param.m_shiftX*m_param.m_scaleX;
    }
    if(val==gRightScale){
        minValue = maxValue = (m_param.m_maxCurrentX-m_param.m_minCurrentX)
            /2.0*(1-1/m_param.m_scaleX);
        if(m_param.m_fitTypeX == ftLeft)
            m_param.m_maxCurrentX=m_param.m_maxCurrentX-maxValue*2;
        if(m_param.m_fitTypeX == ftCenter)
        {
            m_param.m_minCurrentX=m_param.m_minCurrentX+minValue;
            m_param.m_maxCurrentX=m_param.m_maxCurrentX-maxValue;
            m_param.m_begin = m_param.m_begin + minValue
                /(m_param.m_workArrayX[1]-m_param.m_workArrayX[0]);
        }
        if(m_param.m_fitTypeX == ftRight){
            m_param.m_minCurrentX=m_param.m_minCurrentX+minValue*2;
            m_param.m_begin = m_param.m_begin + 2*minValue
                /(m_param.m_workArrayX[1]-m_param.m_workArrayX[0]);
        }
        m_param.m_shiftX = m_param.m_shiftX/m_param.m_scaleX;
    }

    if(val==gTopScale){
        minValue = maxValue = (m_param.m_maxCurrentY-m_param.m_minCurrentY)
            /2.0*(1-1/m_param.m_scaleY);
        if(m_param.m_fitTypeY == ftLeft)
            m_param.m_maxCurrentY=m_param.m_maxCurrentY-maxValue*2;
        if(m_param.m_fitTypeY == ftCenter)
        {
            m_param.m_minCurrentY=m_param.m_minCurrentY+minValue;
            m_param.m_maxCurrentY=m_param.m_maxCurrentY-maxValue;
        }
        if(m_param.m_fitTypeY == ftRight)
            m_param.m_minCurrentY=m_param.m_minCurrentY+minValue*2;
        m_param.m_shiftY = m_param.m_shiftY/m_param.m_scaleY;
    }
    if(val==gBottomScale){
        minValue = maxValue = (m_param.m_maxCurrentY-m_param.m_minCurrentY)
            *(m_param.m_scaleY-1)/2.0;
        if(m_param.m_fitTypeY == ftLeft)
            m_param.m_maxCurrentY=m_param.m_maxCurrentY+maxValue*2;
        if(m_param.m_fitTypeY == ftCenter)
        {
            m_param.m_minCurrentY=m_param.m_minCurrentY-minValue;
            m_param.m_maxCurrentY=m_param.m_maxCurrentY+maxValue;
        }
        if(m_param.m_fitTypeY == ftRight)
            m_param.m_minCurrentY=m_param.m_minCurrentY-minValue*2;
        m_param.m_shiftY = m_param.m_shiftY*m_param.m_scaleY;
    }

    if(val==gNone) return;
};
//------------------------------------------------------------------------------
void CDPlot::SetShift(double shiftX, double shiftY)
{
    m_param.m_shiftX = shiftX;
    m_param.m_shiftY = shiftY;
    IsChange();
}
//------------------------------------------------------------------------------
void CDPlot::SetScale(double scaleX, double scaleY)
{
    m_param.m_scaleX = scaleX;
    m_param.m_scaleY = scaleY;
    IsChange();
}
//------------------------------------------------------------------------------
void CDPlot::SetScaleType(ScaleType scaleTypeX, ScaleType scaleTypeY)
{
    m_param.m_scaleTypeX = scaleTypeX;
    m_param.m_scaleTypeY = scaleTypeY;
}
//------------------------------------------------------------------------------
void CDPlot::SieveArray(double *pArrayX, double *pArrayY, int arraySize)
{
    double current, next, max, min;
    int Counter, Counter1=0, temp;
    bool flag=false;

    for(Counter=0; Counter<arraySize-1; Counter++)
    {
        current = FromWorldToScreen(/*pArrayX[Counter+1]-*/pArrayX[Counter], tpVertical);
        next = FromWorldToScreen(pArrayX[Counter+1], tpVertical);
        if((next - current) > 1.0)
        {
            if(flag)
            {
                m_param.m_workArrayYmax[Counter1] = max;
                m_param.m_workArrayYmin[Counter1] = min;
                m_param.m_workArrayX[Counter1] = pArrayX[temp];
                flag = false;
                Counter1++;
                continue;
            }
            else
            {
                m_param.m_workArrayX[Counter1] = pArrayX[Counter];
                m_param.m_workArrayYmax[Counter1] = pArrayY[Counter];
                m_param.m_workArrayYmin[Counter1] = pArrayY[Counter];
                Counter1++;
            }
        }

        else
        {
            if(!flag)
            {
                if(pArrayY[Counter] > pArrayY[Counter+1]){
                    max = pArrayY[Counter];
                    min = pArrayY[Counter+1];
                }
                else{
                    min = pArrayY[Counter];
                    max = pArrayY[Counter+1];
                }
                temp = Counter;
                flag = true;
                continue;
            }
            else
            {
                if(pArrayY[Counter+1] > max)
                    max = pArrayY[Counter+1];
                else
                    min = pArrayY[Counter+1];
            }
        }
    }
}
//------------------------------------------------------------------------------
void CDPlot::SaveParameters(SimpleXML *pSimpleXML)
{
    CString cString;
    char String[16];

    sprintf_s(String, "%d", m_param.nElement);

    cString = '\0';
    for(unsigned int Counter2=0; Counter2<strlen(String); Counter2++)
        cString = cString + String[Counter2];

    hXml hRoot = pSimpleXML->GetRootNode();
    hXml hInterfaceRoot = pSimpleXML->GetChildNode(hRoot);

    hXml hPlotRoot = pSimpleXML->AddElement(L"Plot_"+cString, hInterfaceRoot);

    hXml hBorderColorRoot = pSimpleXML->AddElement(L"PlotBorderColor", hPlotRoot);
    pSimpleXML->SetAttributeValue(hBorderColorRoot, L"red", CComVariant(m_param.m_plotBorderColor[0]));
    pSimpleXML->SetAttributeValue(hBorderColorRoot, L"green", CComVariant(m_param.m_plotBorderColor[1]));
    pSimpleXML->SetAttributeValue(hBorderColorRoot, L"blue", CComVariant(m_param.m_plotBorderColor[2]));
}
//------------------------------------------------------------------------------
void CDPlot::LoadParameters(SimpleXML *pSimpleXML)
{
    VARIANT vAtrrib;
    CString str = L"";
    CString Pattern = L"Plot_";
    char String[16];

    sprintf_s(String, "%d", m_param.nElement);
    for(unsigned int Counter2=0; Counter2<strlen(String); Counter2++)
        Pattern = Pattern + String[Counter2];

    hXml hRoot = pSimpleXML->GetRootNode();
    hXml hInterfaceRoot = pSimpleXML->GetChildNode(hRoot);
    hXml hElement = pSimpleXML->GetChildNode(hInterfaceRoot);
    while(str!=Pattern)
    {
        hElement = pSimpleXML->GetNextNode(hElement);
        str = pSimpleXML->GetName(hElement);
        if(str == L"")
            return;
    }

    hXml hPlotBorderColor = pSimpleXML->GetChildNode(hElement);
    pSimpleXML->GetAttributeValue(hPlotBorderColor, L"red", vAtrrib);
    if (vAtrrib.vt == VT_BSTR)
        if (CString(vAtrrib.bstrVal).GetLength() != 0)
            m_param.m_plotBorderColor[0] = _wtof(vAtrrib.bstrVal);
    pSimpleXML->GetAttributeValue(hPlotBorderColor, L"green", vAtrrib);
    if (vAtrrib.vt == VT_BSTR)
        if (CString(vAtrrib.bstrVal).GetLength() != 0)
            m_param.m_plotBorderColor[1] = _wtof(vAtrrib.bstrVal);
    pSimpleXML->GetAttributeValue(hPlotBorderColor, L"blue", vAtrrib);
    if (vAtrrib.vt == VT_BSTR)
        if (CString(vAtrrib.bstrVal).GetLength() != 0)
            m_param.m_plotBorderColor[2] = _wtof(vAtrrib.bstrVal);
}