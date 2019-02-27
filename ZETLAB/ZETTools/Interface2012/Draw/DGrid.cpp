//------------------------------------------------------------------------------
// DGrid.cpp : файл реализации
//------------------------------------------------------------------------------
#include "stdafx.h"
#include "C:\ZETTools\Interface2012\Draw\DGrid.h"

int g_nGrid = 0;
int FlagParam = 0;
bool FlagX = false, FlagY = false;

using namespace std;

float z_round(float val, int Pow){
		float temp=val/pow(10.0,Pow);
		float middle=int(temp)+0.5;
		if(temp<middle) return (int)temp*pow(10.0,Pow);
		else return (int(temp)+1)*pow(10.0, Pow);
};

float z_floor(float val, int Pow){
	float temp=val/pow(10.0,Pow);
	temp=floor(temp);
	return temp*pow(10.0, Pow);
}

float z_getStep(int Pow, float Difference){
    if(Difference<CONST1){
        return pow(10.0, (Pow-1));
    }
    if(Difference<CONST2){
        return 5*pow(10.0, (Pow-1));
    }
    else{
        return pow(10.0, Pow);
    }
}

//начальные значения параметров
CDGrid::CDGrid(CRect rect, bool *pFlagIsChange, double *pTimeAnimation) :
CCustomDraw(rect, pFlagIsChange, pTimeAnimation){
    m_pParamDraw = &m_param;	// обеспечение ОБЩИХ параметров
    m_param.nElement = ++g_nGrid;
    SetNameWithNumber(L"Grid");
    m_param.m_rect=rect;
    m_param.bMouseChange = true;
    float Color1[] = {0, 0, 255};
    float Color2[] = {1, 203, 247};
    SetTextColor(RGB(255, 255, 255));
    SetGridColor(RGB(255, 255, 255));
    SetLegendColor(RGB(255, 255, 255));
    m_param.m_borderStyle = SetLineParam(ltFirm, 2.0f, m_param.m_gridColor);
    m_param.m_boldVertGridStyle = SetLineParam(ltFirm, 2.0f, m_param.m_gridColor);
    m_param.m_boldHorGridStyle = SetLineParam(ltFirm, 2.0f, m_param.m_gridColor);
    m_param.m_fineVertGridStyle = SetLineParam(ltFirm, 1.0f, m_param.m_gridColor);
    m_param.m_fineHorGridStyle = SetLineParam(ltFirm, 1.0f, m_param.m_gridColor);
    m_param.m_marksStyle = SetLineParam(ltFirm, 1.0f, m_param.m_gridColor);
    m_param.m_axisStyle = SetLineParam(ltFirm, 2.0f, m_param.m_gridColor);
    m_param.m_crosshairStyle = SetLineParam(ltFirm, 1.0f, Color1);
    m_param.m_legendStyle = SetLineParam(ltFirm, 1.0f, Color2);
    m_param.m_fontH = CDFonts();
    m_param.m_fontV = CDFonts();
    m_param.m_font = CDFonts();
    m_param.m_legend = "Legend";
    m_param.m_fontH.SetAligmentHorizontal(StrAlignmentCenter);
    m_param.m_fontH.SetAligmentVertical(StrAlignmentNear);
    m_param.m_fontH.SetColor(RGB(m_param.m_textColor[0], m_param.m_textColor[1], m_param.m_textColor[2]));
    m_param.m_fontV.SetAligmentHorizontal(StrAlignmentFar);
    m_param.m_fontV.SetAligmentVertical(StrAlignmentCenter);
    m_param.m_fontV.SetColor(RGB(m_param.m_textColor[0], m_param.m_textColor[1], m_param.m_textColor[2]));
    m_param.m_font.SetAligmentHorizontal(StrAlignmentCenter);
    m_param.m_font.SetAligmentVertical(StrAlignmentCenter);
    m_param.m_font.SetColor(RGB(m_param.m_textColor[0], m_param.m_textColor[1], m_param.m_textColor[2]));
    SetImageFileName(L"C:\\ZETTools\\Interface2012\\Textures\\Patterns\\metal_mart14.bmp");
    SetParam();
};
//------------------------------------------------------------------------------
CDGrid::~CDGrid()
{

};
//------------------------------------------------------------------------------
void CDGrid::OnDraw(){
    DrawButtons();
    DrawBorder();
    glEnable(GL_COLOR_MATERIAL);
    glDisable(GL_LIGHTING);
	//DrawCrosshair(m_param.m_cursorPosition);
	//DrawArrows();
	DrawGrid();
    DrawTextMarks();
    DrawLegend();
    DrawSignature();
    DrawBackground();
    glEnable(GL_LIGHTING);
    glDisable(GL_COLOR_MATERIAL);
	glFlush();
};
//------------------------------------------------------------------------------
void CDGrid::DrawGrid(){
	glPushMatrix();
	glLoadIdentity();

    int Counter;
    double PointX[2], PointY[2];
    AxisParam Axis[2];
    Axis[0] = m_param.m_axisX;
    Axis[1] = m_param.m_axisY;

    for(Counter=0; Counter<2; Counter++){
        if(Axis[Counter].apScaleType == stNone) break;
        else{
            EnableStyle(m_param.m_fineVertGridStyle);
            
            for(int Counter1=0; Counter1<Axis[Counter].apQntMarks; Counter1++){
                if(Axis[Counter].apGridType==gtHorizontal){
                    PointX[0] = PointX[1] = m_param.m_rect.left
                        +FromWorldToScreen(Axis[Counter].apMarkingArray[Counter1],
                        Axis[Counter]);
                    PointY[0] = m_param.m_rect.bottom;
                    PointY[1] = m_param.m_rect.top;
                    if((PointX[0]>(m_param.m_rect.right)) 
                        || (PointX[0]<m_param.m_rect.left)) continue;
                }
                else{
                    PointX[0] = m_param.m_rect.left;
                    PointX[1] = m_param.m_rect.right;
                    PointY[0] = PointY[1] = m_param.m_rect.bottom
                        -FromWorldToScreen(Axis[Counter].apMarkingArray[Counter1],
                        Axis[Counter]);
                    if(PointY[0]>(m_param.m_rect.bottom) 
                        || PointY[0]<m_param.m_rect.top) continue;
                }
                glBegin(GL_LINES);
                    glVertex3f(PointX[0], PointY[0], ZDEPTH);
                    glVertex3f(PointX[1], PointY[1], ZDEPTH);
                glEnd();
            }
            DisableStyle(m_param.m_fineVertGridStyle);
        }
    }
	/*switch(m_param.m_scaleTypeVert){
		case stLinear:
            //тонкие линии
			EnableStyle(m_param.m_fineVertGridStyle);
			glBegin(GL_LINES);
			for(int Counter=0; Counter<m_param.m_qntMarksVert; Counter++){
				if((m_param.m_markingArrayVer[Counter]>(m_param.m_rect.right)) || (m_param.m_markingArrayVer[Counter]<m_param.m_rect.left)) continue;
				glVertex3f(m_param.m_markingArrayVer[Counter], m_param.m_rect.bottom, ZDEPTH);
				glVertex3f(m_param.m_markingArrayVer[Counter], m_param.m_rect.top, ZDEPTH);
			}
			glEnd();
            DisableStyle(m_param.m_fineVertGridStyle);

        case stLog:
            //тонкие линии
			EnableStyle(m_param.m_fineVertGridStyle);
			glBegin(GL_LINES);
			for(int Counter=0; Counter<m_param.m_qntMarksVertLog; Counter++){
				if((m_param.m_markingLogArrayVer[Counter]>(m_param.m_rect.right)) || (m_param.m_markingLogArrayVer[Counter]<m_param.m_rect.left)) continue;
				glVertex3f(m_param.m_markingLogArrayVer[Counter], m_param.m_rect.bottom, ZDEPTH);
				glVertex3f(m_param.m_markingLogArrayVer[Counter], m_param.m_rect.top, ZDEPTH);
			}
			glEnd();
            DisableStyle(m_param.m_fineVertGridStyle);

            //толстые линии
			EnableStyle(m_param.m_boldVertGridStyle);
			glBegin(GL_LINES);
			for(int Counter=0; Counter<m_param.m_qntMarksVertLogBold; Counter++){
				if(m_param.m_markingLogArrayVerBold[Counter]>(m_param.m_rect.right) || m_param.m_markingLogArrayVerBold[Counter]<m_param.m_rect.left) continue;
				glVertex3f(m_param.m_markingLogArrayVerBold[Counter], m_param.m_rect.bottom, ZDEPTH);
				glVertex3f(m_param.m_markingLogArrayVerBold[Counter], m_param.m_rect.top, ZDEPTH);
			}
			glEnd();
            DisableStyle(m_param.m_boldVertGridStyle);

        case gNone: break;
        default: break;
	}
	switch(m_param.m_scaleTypeHor){
        case stLinear:
			glLineWidth(1.f);
			glBegin(GL_LINES);
			for(int Counter=0; Counter<m_param.m_qntMarksHor; Counter++){
				if(m_param.m_markingArrayHor[Counter]>(m_param.m_rect.bottom) || m_param.m_markingArrayHor[Counter]<m_param.m_rect.top) continue;
				glVertex3f(m_param.m_rect.left, m_param.m_markingArrayHor[Counter], ZDEPTH);
				glVertex3f(m_param.m_rect.right, m_param.m_markingArrayHor[Counter], ZDEPTH);
			}
			glEnd();

        case stLog:
			glLineWidth(1.f);
			glBegin(GL_LINES);
			for(int Counter=0; Counter<m_param.m_qntMarksHorLog; Counter++){
				if(m_param.m_markingLogArrayHor[Counter]>(m_param.m_rect.bottom) || m_param.m_markingLogArrayHor[Counter]<m_param.m_rect.top) continue;
				glVertex3f(m_param.m_rect.left, m_param.m_markingLogArrayHor[Counter], ZDEPTH);
				glVertex3f(m_param.m_rect.right, m_param.m_markingLogArrayHor[Counter], ZDEPTH);
			}
			glEnd();
			glLineWidth(2.f);
			glBegin(GL_LINES);
			for(int Counter=0; Counter<m_param.m_qntMarksHorLogBold; Counter++){
				if(m_param.m_markingLogArrayHorBold[Counter]>(m_param.m_rect.bottom) || m_param.m_markingLogArrayHorBold[Counter]<m_param.m_rect.top) continue;
				glVertex3f(m_param.m_rect.left, m_param.m_markingLogArrayHorBold[Counter], ZDEPTH);
				glVertex3f(m_param.m_rect.right, m_param.m_markingLogArrayHorBold[Counter], ZDEPTH);
			}
			glEnd();
 
        case gNone: break;
        default: break;
    }*/
	glPopMatrix();
}
//------------------------------------------------------------------------------
void CDGrid::DrawBackground(){
	glBegin(GL_QUADS);
        glColor3ub(0, 0, 0);
		glVertex3f(m_param.m_rect.left, m_param.m_rect.top, ZDEPTH);
        glColor3ub(100, 100, 100);
		glVertex3f(m_param.m_rect.left, m_param.m_rect.bottom, ZDEPTH);
		glVertex3f(m_param.m_rect.right, m_param.m_rect.bottom, ZDEPTH);
        glColor3ub(0, 0, 0);
		glVertex3f(m_param.m_rect.right, m_param.m_rect.top, ZDEPTH);
	glEnd();
}
//------------------------------------------------------------------------------
void CDGrid::DrawBorder(){
	glPushMatrix();
	glLoadIdentity();
    glMaterialfv(GL_FRONT, GL_AMBIENT, m_param.mat_ambient);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, m_param.mat_diffuse);
    glMaterialfv(GL_FRONT, GL_EMISSION, m_param.mat_emission);
    glMaterialfv(GL_FRONT, GL_SPECULAR, m_param.mat_specular);
    glMaterialf(GL_FRONT, GL_SHININESS, m_param.mat_shininess);
    glBindTexture(GL_TEXTURE_2D, Texture);
    for(int Counter=0; Counter<8; Counter++){
        m_param.m_pBorders[Counter].Draw_texture(m_param.m_pBorders[Counter].ReadFacet(), m_param.m_pBorders[Counter].GetQntPlanes(), PicSize, true);
    }
	glPopMatrix();
};
//------------------------------------------------------------------------------
void CDGrid::DrawArrows(){
    glPushMatrix();
	glLoadIdentity();
    EnableStyle(m_param.m_fineVertGridStyle);
	//продолжение осей на концах
    glDisable(GL_DEPTH_TEST);
	glBegin(GL_LINES);
		glVertex3f(m_param.m_rect.left, m_param.m_rect.top, ZDEPTH);
		glVertex3f(m_param.m_rect.left, m_param.m_rect.top
            -m_param.m_topBorder/2, ZDEPTH);
		glVertex3f(m_param.m_rect.right, m_param.m_rect.bottom, ZDEPTH);
		glVertex3f(m_param.m_rect.right+m_param.m_topBorder/2,
            m_param.m_rect.bottom, ZDEPTH);
	glEnd();

	//верхняя стрелка
	glBegin(GL_TRIANGLES);
		glVertex3f(m_param.m_rect.left, m_param.m_rect.top-m_param.m_topBorder/2,
            ZDEPTH);
		glVertex3f(m_param.m_rect.left+m_param.m_topBorder/8, 
            m_param.m_rect.top-m_param.m_topBorder/2+m_param.m_topBorder/4, ZDEPTH);
		glVertex3f(m_param.m_rect.left-m_param.m_topBorder/8, 
            m_param.m_rect.top-m_param.m_topBorder/2+m_param.m_topBorder/4, ZDEPTH);
	glEnd();

	//боковая стрелка
	glBegin(GL_TRIANGLES);
		glVertex3f(m_param.m_rect.right+m_param.m_topBorder/2,
            m_param.m_rect.bottom, ZDEPTH);
		glVertex3f(m_param.m_rect.right+m_param.m_topBorder/2-m_param.m_topBorder/4,
            m_param.m_rect.bottom+m_param.m_topBorder/8, ZDEPTH);
		glVertex3f(m_param.m_rect.right+m_param.m_topBorder/2-m_param.m_topBorder/4,
            m_param.m_rect.bottom-m_param.m_topBorder/8, ZDEPTH);
	glEnd();

	//продолжение осей в начале отсчета
	/*glBegin(GL_LINES);
		glVertex3f(m_param.m_rect.left, m_param.m_rect.bottom, ZDEPTH);
		glVertex3f(m_param.m_rect.left-m_param.m_border/2,
            m_param.m_rect.bottom, ZDEPTH);
		glVertex3f(m_param.m_rect.left, m_param.m_rect.bottom, ZDEPTH);
		glVertex3f(m_param.m_rect.left, m_param.m_rect.bottom
            +m_param.m_border/2, ZDEPTH);
	glEnd();*/
    glEnable(GL_DEPTH_TEST);

    DisableStyle(m_param.m_axisStyle);
	glPopMatrix();
};
//------------------------------------------------------------------------------
void CDGrid::DrawCrosshair(CPoint point){
	glPushMatrix();
	glLoadIdentity();
	EnableStyle(m_param.m_crosshairStyle);
    if(IsMouseInRect(point)==gGridArea){
            glBegin(GL_LINES);
            glVertex3f(m_param.m_rect.left, point.y, ZDEPTH);
            glVertex3f(m_param.m_rect.right, point.y, ZDEPTH);
            glVertex3f(point.x, m_param.m_rect.bottom, ZDEPTH);
            glVertex3f(point.x, m_param.m_rect.top, ZDEPTH);
            glEnd();
    }
    DisableStyle(m_param.m_crosshairStyle);
    glPopMatrix();
    IsChange();
}
//------------------------------------------------------------------------------
void CDGrid::DrawTextMarks(){
    char String[20];
    CString str;
    AxisParam Axis[2];
    int Counter, Counter1, Counter2, Step, Size;
    LONG left, right, top, bottom;
    double CenterRect, Power, Value;

    Axis[0] = m_param.m_axisX;
    Axis[1] = m_param.m_axisY;

    for(Counter=0; Counter<2; Counter++){
        Step = Axis[Counter].apQntMarks/Axis[Counter].apQntTextMarks;
        if(Axis[Counter].apQntMarks%Axis[Counter].apQntTextMarks
            > Axis[Counter].apQntTextMarks/2)
            Step++;
         if(Axis[Counter].apGridType == gtHorizontal){
            CenterRect = m_param.m_rect.left
                +FromWorldToScreen(Axis[Counter].apMarkingArray[0],
                Axis[Counter]);
                left = CenterRect - Axis[Counter].apMaxTextLength/2;
                if(left < m_param.m_rect.left) Counter1=1;
                else Counter1=0;
         }
         if(Axis[Counter].apGridType == gtVertical){
             Counter1=0;
         }

        for(; Counter1<Axis[Counter].apQntMarks; Counter1=Counter1+Step){
            String[0] = '\0';
            Value = Axis[Counter].apMinCurrent
                +Axis[Counter].apMarkingArray[Counter1];
            if(abs(Value)<EPS)
                Value = 0;
            sprintf_s(String, "%0.3g", Value);

            str = '\0';
            for (Counter2 = 0, Size = strlen(String); Counter2 < Size; Counter2++){
                str = str + String[Counter2];
            }
            if(Axis[Counter].apGridType == gtHorizontal){
            CenterRect = m_param.m_rect.left
                +FromWorldToScreen(Axis[Counter].apMarkingArray[Counter1],
                Axis[Counter]);
                left = CenterRect - Axis[Counter].apMaxTextLength/2;
                right = CenterRect + Axis[Counter].apMaxTextLength/2;
                top = m_param.m_rect.bottom + 8;
                bottom = m_param.m_rect.bottom + m_param.m_bottomBorder;
                if(right >= m_param.m_rect.right) continue;
            }
            if(Axis[Counter].apGridType == gtVertical){
                CenterRect = m_param.m_rect.bottom
                    -FromWorldToScreen(Axis[Counter].apMarkingArray[Counter1],
                    Axis[Counter]);
                left = m_param.m_rect.left - m_param.m_leftBorder;
                right = m_param.m_rect.left - 8;
                top = CenterRect - Axis[Counter].apMaxTexthHeight/2;
                bottom = top + (LONG)Axis[Counter].apMaxTexthHeight+1;
                if(top <= m_param.m_rect.top+Axis[Counter].apMaxTexthHeight) continue;
            }
            if(Axis[Counter].apGridType == gtHorizontal){
                glDisable(GL_DEPTH_TEST);
                m_param.m_fontH.DrawString(CRect(left, top, right, bottom), str);
                glEnable(GL_DEPTH_TEST);
            }
            if(Axis[Counter].apGridType == gtVertical){
                glDisable(GL_DEPTH_TEST);
                m_param.m_fontV.DrawString(CRect(left, top, right, bottom), str);
                glEnable(GL_DEPTH_TEST);
            }
        }
    }
}
//------------------------------------------------------------------------------
void CDGrid::DrawLegend(){
    Dimention Dim;
    Dim = TextDimention(m_param.m_legend);
    m_param.m_rectLegend = CRect(m_param.m_rect.right-Dim.dWidth,
        m_param.m_rect.top-m_param.m_topBorder, m_param.m_rect.right,
        m_param.m_rect.top);
    glDisable(GL_DEPTH_TEST);
    m_param.m_font.DrawString(m_param.m_rectLegend, m_param.m_legend);
    glEnable(GL_DEPTH_TEST);
    glPushMatrix();
    glLoadIdentity();
    EnableStyle(m_param.m_legendStyle);
        glBegin(GL_LINES);
        glVertex3f(m_param.m_rectLegend.left-50, m_param.m_rectLegend.bottom
            - m_param.m_topBorder/2, ZDEPTH);
        glVertex3f(m_param.m_rectLegend.left-10, m_param.m_rectLegend.bottom
            - m_param.m_topBorder/2, ZDEPTH);
        glEnd();
    DisableStyle(m_param.m_legendStyle);
    glPopMatrix();
}
//------------------------------------------------------------------------------
void CDGrid::DrawButtons()
{
    glPushMatrix();
    glLoadIdentity();
    glMaterialfv(GL_FRONT, GL_AMBIENT, m_param.mat_ambient1);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, m_param.mat_diffuse1);
    glMaterialfv(GL_FRONT, GL_EMISSION, m_param.mat_emission1);
    glMaterialfv(GL_FRONT, GL_SPECULAR, m_param.mat_specular1);
    glMaterialf(GL_FRONT, GL_SHININESS, m_param.mat_shininess1);
    glBindTexture(GL_TEXTURE_2D, Texture);
    for(int Counter=0; Counter<9; Counter++){
        m_pButton[Counter].Draw_texture(m_pButton[Counter].ReadFacet(), m_pButton[Counter].GetQntPlanes(), PicSize, true);
    }
    glPopMatrix();
}
//------------------------------------------------------------------------------
void CDGrid::DrawSignature(){
    glDisable(GL_DEPTH_TEST);
    m_param.m_font.DrawString(CRect(m_param.m_rect.right, m_param.m_rect.bottom
        +m_param.m_bottomBorder/2, m_param.m_rect.right+20, m_param.m_rect.bottom
        +m_param.m_bottomBorder), m_param.m_axisX.apTitle);
    m_param.m_font.DrawString(CRect(m_param.m_rect.left-m_param.m_leftBorder,
        m_param.m_rect.top-10, m_param.m_rect.left, m_param.m_rect.top),  m_param.m_axisY.apTitle);
    glEnable(GL_DEPTH_TEST);
}
//------------------------------------------------------------------------------
void CDGrid::SaveParameters(SimpleXML *pSimpleXML)
{
    CString cString;
    char String[16];

    sprintf_s(String, "%d", m_param.nElement);

    cString = '\0';
    for(int Counter2 = 0, Size = strlen(String); Counter2 < Size; Counter2++)
        cString = cString + String[Counter2];

    hXml hRoot = pSimpleXML->GetRootNode();
    hXml hInterfaceRoot = pSimpleXML->GetChildNode(hRoot);

    hXml hGridRoot = pSimpleXML->AddElement(L"Grid_"+cString, hInterfaceRoot);

    hXml hTextureRoot = pSimpleXML->AddElement(L"Texture", hGridRoot);
    pSimpleXML->SetAttributeValue(hTextureRoot, L"path", CComVariant(m_param.m_textureName));

    hXml hBorderRoot = pSimpleXML->AddElement(L"Border", hGridRoot);

    hXml hMaterialRoot = pSimpleXML->AddElement(L"Material", hBorderRoot);

    hXml hAmbientRoot = pSimpleXML->AddElement(L"Ambient", hMaterialRoot);
    pSimpleXML->SetAttributeValue(hAmbientRoot, L"red", CComVariant(m_param.mat_ambient[0]));
    pSimpleXML->SetAttributeValue(hAmbientRoot, L"green", CComVariant(m_param.mat_ambient[1]));
    pSimpleXML->SetAttributeValue(hAmbientRoot, L"blue", CComVariant(m_param.mat_ambient[2]));
    pSimpleXML->SetAttributeValue(hAmbientRoot, L"alpha", CComVariant(m_param.mat_ambient[3]));

    hXml hDiffuseRoot = pSimpleXML->AddElement(L"Diffuse", hMaterialRoot);
    pSimpleXML->SetAttributeValue(hDiffuseRoot, L"red", CComVariant(m_param.mat_diffuse[0]));
    pSimpleXML->SetAttributeValue(hDiffuseRoot, L"green", CComVariant(m_param.mat_diffuse[1]));
    pSimpleXML->SetAttributeValue(hDiffuseRoot, L"blue", CComVariant(m_param.mat_diffuse[2]));
    pSimpleXML->SetAttributeValue(hDiffuseRoot, L"alpha", CComVariant(m_param.mat_diffuse[3]));

    hXml hSpecularRoot = pSimpleXML->AddElement(L"Specular", hMaterialRoot);
    pSimpleXML->SetAttributeValue(hSpecularRoot, L"red", CComVariant(m_param.mat_specular[0]));
    pSimpleXML->SetAttributeValue(hSpecularRoot, L"green", CComVariant(m_param.mat_specular[1]));
    pSimpleXML->SetAttributeValue(hSpecularRoot, L"blue", CComVariant(m_param.mat_specular[2]));
    pSimpleXML->SetAttributeValue(hSpecularRoot, L"alpha", CComVariant(m_param.mat_specular[3]));

    hXml hEmissionRoot = pSimpleXML->AddElement(L"Emission", hMaterialRoot);
    pSimpleXML->SetAttributeValue(hEmissionRoot, L"red", CComVariant(m_param.mat_emission[0]));
    pSimpleXML->SetAttributeValue(hEmissionRoot, L"green", CComVariant(m_param.mat_emission[1]));
    pSimpleXML->SetAttributeValue(hEmissionRoot, L"blue", CComVariant(m_param.mat_emission[2]));
    pSimpleXML->SetAttributeValue(hEmissionRoot, L"alpha", CComVariant(m_param.mat_emission[3]));

    hXml hShininessRoot = pSimpleXML->AddElement(L"Shininess", hMaterialRoot);
    pSimpleXML->SetAttributeValue(hShininessRoot, L"blink_coefficient", CComVariant(m_param.mat_shininess));

    hXml hButtonRoot = pSimpleXML->AddElement(L"Buttons", hGridRoot);

    hMaterialRoot = pSimpleXML->AddElement(L"Material", hButtonRoot);

    hAmbientRoot = pSimpleXML->AddElement(L"Ambient", hMaterialRoot);
    pSimpleXML->SetAttributeValue(hAmbientRoot, L"red", CComVariant(m_param.mat_ambient1[0]));
    pSimpleXML->SetAttributeValue(hAmbientRoot, L"green", CComVariant(m_param.mat_ambient1[1]));
    pSimpleXML->SetAttributeValue(hAmbientRoot, L"blue", CComVariant(m_param.mat_ambient1[2]));
    pSimpleXML->SetAttributeValue(hAmbientRoot, L"alpha", CComVariant(m_param.mat_ambient1[3]));

    hDiffuseRoot = pSimpleXML->AddElement(L"Diffuse", hMaterialRoot);
    pSimpleXML->SetAttributeValue(hDiffuseRoot, L"red", CComVariant(m_param.mat_diffuse1[0]));
    pSimpleXML->SetAttributeValue(hDiffuseRoot, L"green", CComVariant(m_param.mat_diffuse1[1]));
    pSimpleXML->SetAttributeValue(hDiffuseRoot, L"blue", CComVariant(m_param.mat_diffuse1[2]));
    pSimpleXML->SetAttributeValue(hDiffuseRoot, L"alpha", CComVariant(m_param.mat_diffuse1[3]));

    hSpecularRoot = pSimpleXML->AddElement(L"Specular", hMaterialRoot);
    pSimpleXML->SetAttributeValue(hSpecularRoot, L"red", CComVariant(m_param.mat_specular1[0]));
    pSimpleXML->SetAttributeValue(hSpecularRoot, L"green", CComVariant(m_param.mat_specular1[1]));
    pSimpleXML->SetAttributeValue(hSpecularRoot, L"blue", CComVariant(m_param.mat_specular1[2]));
    pSimpleXML->SetAttributeValue(hSpecularRoot, L"alpha", CComVariant(m_param.mat_specular1[3]));

    hEmissionRoot = pSimpleXML->AddElement(L"Emission", hMaterialRoot);
    pSimpleXML->SetAttributeValue(hEmissionRoot, L"red", CComVariant(m_param.mat_emission1[0]));
    pSimpleXML->SetAttributeValue(hEmissionRoot, L"green", CComVariant(m_param.mat_emission1[1]));
    pSimpleXML->SetAttributeValue(hEmissionRoot, L"blue", CComVariant(m_param.mat_emission1[2]));
    pSimpleXML->SetAttributeValue(hEmissionRoot, L"alpha", CComVariant(m_param.mat_emission1[3]));

    hShininessRoot = pSimpleXML->AddElement(L"Shininess", hMaterialRoot);
    pSimpleXML->SetAttributeValue(hShininessRoot, L"blink_coefficient", CComVariant(m_param.mat_shininess1));

    hXml hGridColorRoot = pSimpleXML->AddElement(L"GridColor", hGridRoot);
    pSimpleXML->SetAttributeValue(hGridColorRoot, L"red", CComVariant(m_param.m_gridColor[0]));
    pSimpleXML->SetAttributeValue(hGridColorRoot, L"green", CComVariant(m_param.m_gridColor[1]));
    pSimpleXML->SetAttributeValue(hGridColorRoot, L"blue", CComVariant(m_param.m_gridColor[2]));

    hXml hTextColorRoot = pSimpleXML->AddElement(L"TextColor", hGridRoot);
    pSimpleXML->SetAttributeValue(hTextColorRoot, L"red", CComVariant(m_param.m_textColor[0]));
    pSimpleXML->SetAttributeValue(hTextColorRoot, L"green", CComVariant(m_param.m_textColor[1]));
    pSimpleXML->SetAttributeValue(hTextColorRoot, L"blue", CComVariant(m_param.m_textColor[2]));
}
//------------------------------------------------------------------------------
void CDGrid::LoadParameters(SimpleXML *pSimpleXML)
{
    VARIANT vAtrrib;
    CString str = L"";
    CString Pattern = L"Grid_";
    char String[16];

    sprintf_s(String, "%d", m_param.nElement);
    for(int Counter2 = 0, Size = strlen(String); Counter2 < Size; Counter2++)
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
    hXml hTextureRoot = pSimpleXML->GetChildNode(hElement);
    pSimpleXML->GetAttributeValue(hTextureRoot, L"path", vAtrrib);
    if (vAtrrib.vt == VT_BSTR)
        if (CString(vAtrrib.bstrVal).GetLength() != 0)
            SetImageFileName(CString(vAtrrib.bstrVal));

    hXml hBorderRoot = pSimpleXML->GetNextNode(hTextureRoot);

    //рассеянный цвет материала (цвет материала в тени)
    hXml hMaterialRoot = pSimpleXML->GetChildNode(hBorderRoot);
    hXml hAmbientRoot = pSimpleXML->GetChildNode(hMaterialRoot);
    pSimpleXML->GetAttributeValue(hAmbientRoot, L"red", vAtrrib);
    if (vAtrrib.vt == VT_BSTR)
        if (CString(vAtrrib.bstrVal).GetLength() != 0)
            m_param.mat_ambient[0] = _wtof(vAtrrib.bstrVal);
    pSimpleXML->GetAttributeValue(hAmbientRoot, L"green", vAtrrib);
    if (vAtrrib.vt == VT_BSTR)
        if (CString(vAtrrib.bstrVal).GetLength() != 0)
            m_param.mat_ambient[1] = _wtof(vAtrrib.bstrVal);
    pSimpleXML->GetAttributeValue(hAmbientRoot, L"blue", vAtrrib);
    if (vAtrrib.vt == VT_BSTR)
        if (CString(vAtrrib.bstrVal).GetLength() != 0)
            m_param.mat_ambient[2] = _wtof(vAtrrib.bstrVal);
    pSimpleXML->GetAttributeValue(hAmbientRoot, L"alpha", vAtrrib);
    if (vAtrrib.vt == VT_BSTR)
        if (CString(vAtrrib.bstrVal).GetLength() != 0)
            m_param.mat_ambient[3] = _wtof(vAtrrib.bstrVal);

    //цвет диффузного отражения материала
    hXml hDiffuseRoot = pSimpleXML->GetNextNode(hAmbientRoot);
    pSimpleXML->GetAttributeValue(hDiffuseRoot, L"red", vAtrrib);
    if (vAtrrib.vt == VT_BSTR)
        if (CString(vAtrrib.bstrVal).GetLength() != 0)
            m_param.mat_diffuse[0] = _wtof(vAtrrib.bstrVal);
    pSimpleXML->GetAttributeValue(hDiffuseRoot, L"green", vAtrrib);
    if (vAtrrib.vt == VT_BSTR)
        if (CString(vAtrrib.bstrVal).GetLength() != 0)
            m_param.mat_diffuse[1] = _wtof(vAtrrib.bstrVal);
    pSimpleXML->GetAttributeValue(hDiffuseRoot, L"blue", vAtrrib);
    if (vAtrrib.vt == VT_BSTR)
        if (CString(vAtrrib.bstrVal).GetLength() != 0)
            m_param.mat_diffuse[2] = _wtof(vAtrrib.bstrVal);
    pSimpleXML->GetAttributeValue(hDiffuseRoot, L"alpha", vAtrrib);
    if (vAtrrib.vt == VT_BSTR)
        if (CString(vAtrrib.bstrVal).GetLength() != 0)
            m_param.mat_diffuse[3] = _wtof(vAtrrib.bstrVal);

    //цвет зеркального отражения материала
    hXml hSpecularRoot = pSimpleXML->GetNextNode(hDiffuseRoot);
    pSimpleXML->GetAttributeValue(hSpecularRoot, L"red", vAtrrib);
    if (vAtrrib.vt == VT_BSTR)
        if (CString(vAtrrib.bstrVal).GetLength() != 0)
            m_param.mat_specular[0] = _wtof(vAtrrib.bstrVal);
    pSimpleXML->GetAttributeValue(hSpecularRoot, L"green", vAtrrib);
    if (vAtrrib.vt == VT_BSTR)
        if (CString(vAtrrib.bstrVal).GetLength() != 0)
            m_param.mat_specular[1] = _wtof(vAtrrib.bstrVal);
    pSimpleXML->GetAttributeValue(hSpecularRoot, L"blue", vAtrrib);
    if (vAtrrib.vt == VT_BSTR)
        if (CString(vAtrrib.bstrVal).GetLength() != 0)
            m_param.mat_specular[2] = _wtof(vAtrrib.bstrVal);
    pSimpleXML->GetAttributeValue(hSpecularRoot, L"alpha", vAtrrib);
    if (vAtrrib.vt == VT_BSTR)
        if (CString(vAtrrib.bstrVal).GetLength() != 0)
            m_param.mat_specular[3] = _wtof(vAtrrib.bstrVal);

    //интенсивность излучаемого света материала
    hXml hEmissionRoot = pSimpleXML->GetNextNode(hSpecularRoot);
    pSimpleXML->GetAttributeValue(hEmissionRoot, L"red", vAtrrib);
    if (vAtrrib.vt == VT_BSTR)
        if (CString(vAtrrib.bstrVal).GetLength() != 0)
            m_param.mat_emission[0] = _wtof(vAtrrib.bstrVal);
    pSimpleXML->GetAttributeValue(hEmissionRoot, L"green", vAtrrib);
    if (vAtrrib.vt == VT_BSTR)
        if (CString(vAtrrib.bstrVal).GetLength() != 0)
            m_param.mat_emission[1] = _wtof(vAtrrib.bstrVal);
    pSimpleXML->GetAttributeValue(hEmissionRoot, L"blue", vAtrrib);
    if (vAtrrib.vt == VT_BSTR)
        if (CString(vAtrrib.bstrVal).GetLength() != 0)
            m_param.mat_emission[2] = _wtof(vAtrrib.bstrVal);
    pSimpleXML->GetAttributeValue(hEmissionRoot, L"alpha", vAtrrib);
    if (vAtrrib.vt == VT_BSTR)
        if (CString(vAtrrib.bstrVal).GetLength() != 0)
            m_param.mat_emission[3] = _wtof(vAtrrib.bstrVal);

    //определяет степень зеркального отражения материала
    hXml hShininessRoot = pSimpleXML->GetNextNode(hEmissionRoot);
    pSimpleXML->GetAttributeValue(hShininessRoot, L"blink_coefficient", vAtrrib);
    if (vAtrrib.vt == VT_BSTR)
        if (CString(vAtrrib.bstrVal).GetLength() != 0)
            m_param.mat_shininess = _wtof(vAtrrib.bstrVal);

    hXml hButtonRoot = pSimpleXML->GetNextNode(hBorderRoot);

    //рассеянный цвет материала (цвет материала в тени)
    hMaterialRoot = pSimpleXML->GetChildNode(hButtonRoot);
    hAmbientRoot = pSimpleXML->GetChildNode(hMaterialRoot);
    pSimpleXML->GetAttributeValue(hAmbientRoot, L"red", vAtrrib);
    if (vAtrrib.vt == VT_BSTR)
        if (CString(vAtrrib.bstrVal).GetLength() != 0)
            m_param.mat_ambient1[0] = _wtof(vAtrrib.bstrVal);
    pSimpleXML->GetAttributeValue(hAmbientRoot, L"green", vAtrrib);
    if (vAtrrib.vt == VT_BSTR)
        if (CString(vAtrrib.bstrVal).GetLength() != 0)
            m_param.mat_ambient1[1] = _wtof(vAtrrib.bstrVal);
    pSimpleXML->GetAttributeValue(hAmbientRoot, L"blue", vAtrrib);
    if (vAtrrib.vt == VT_BSTR)
        if (CString(vAtrrib.bstrVal).GetLength() != 0)
            m_param.mat_ambient1[2] = _wtof(vAtrrib.bstrVal);
    pSimpleXML->GetAttributeValue(hAmbientRoot, L"alpha", vAtrrib);
    if (vAtrrib.vt == VT_BSTR)
        if (CString(vAtrrib.bstrVal).GetLength() != 0)
            m_param.mat_ambient1[3] = _wtof(vAtrrib.bstrVal);

    //цвет диффузного отражения материала
    hDiffuseRoot = pSimpleXML->GetNextNode(hAmbientRoot);
    pSimpleXML->GetAttributeValue(hDiffuseRoot, L"red", vAtrrib);
    if (vAtrrib.vt == VT_BSTR)
        if (CString(vAtrrib.bstrVal).GetLength() != 0)
            m_param.mat_diffuse1[0] = _wtof(vAtrrib.bstrVal);
    pSimpleXML->GetAttributeValue(hDiffuseRoot, L"green", vAtrrib);
    if (vAtrrib.vt == VT_BSTR)
        if (CString(vAtrrib.bstrVal).GetLength() != 0)
            m_param.mat_diffuse1[1] = _wtof(vAtrrib.bstrVal);
    pSimpleXML->GetAttributeValue(hDiffuseRoot, L"blue", vAtrrib);
    if (vAtrrib.vt == VT_BSTR)
        if (CString(vAtrrib.bstrVal).GetLength() != 0)
            m_param.mat_diffuse1[2] = _wtof(vAtrrib.bstrVal);
    pSimpleXML->GetAttributeValue(hDiffuseRoot, L"alpha", vAtrrib);
    if (vAtrrib.vt == VT_BSTR)
        if (CString(vAtrrib.bstrVal).GetLength() != 0)
            m_param.mat_diffuse1[3] = _wtof(vAtrrib.bstrVal);

    //цвет зеркального отражения материала
    hSpecularRoot = pSimpleXML->GetNextNode(hDiffuseRoot);
    pSimpleXML->GetAttributeValue(hSpecularRoot, L"red", vAtrrib);
    if (vAtrrib.vt == VT_BSTR)
        if (CString(vAtrrib.bstrVal).GetLength() != 0)
            m_param.mat_specular1[0] = _wtof(vAtrrib.bstrVal);
    pSimpleXML->GetAttributeValue(hSpecularRoot, L"green", vAtrrib);
    if (vAtrrib.vt == VT_BSTR)
        if (CString(vAtrrib.bstrVal).GetLength() != 0)
            m_param.mat_specular1[1] = _wtof(vAtrrib.bstrVal);
    pSimpleXML->GetAttributeValue(hSpecularRoot, L"blue", vAtrrib);
    if (vAtrrib.vt == VT_BSTR)
        if (CString(vAtrrib.bstrVal).GetLength() != 0)
            m_param.mat_specular1[2] = _wtof(vAtrrib.bstrVal);
    pSimpleXML->GetAttributeValue(hSpecularRoot, L"alpha", vAtrrib);
    if (vAtrrib.vt == VT_BSTR)
        if (CString(vAtrrib.bstrVal).GetLength() != 0)
            m_param.mat_specular1[3] = _wtof(vAtrrib.bstrVal);

    //интенсивность излучаемого света материала
    hEmissionRoot = pSimpleXML->GetNextNode(hSpecularRoot);
    pSimpleXML->GetAttributeValue(hEmissionRoot, L"red", vAtrrib);
    if (vAtrrib.vt == VT_BSTR)
        if (CString(vAtrrib.bstrVal).GetLength() != 0)
            m_param.mat_emission1[0] = _wtof(vAtrrib.bstrVal);
    pSimpleXML->GetAttributeValue(hEmissionRoot, L"green", vAtrrib);
    if (vAtrrib.vt == VT_BSTR)
        if (CString(vAtrrib.bstrVal).GetLength() != 0)
            m_param.mat_emission1[1] = _wtof(vAtrrib.bstrVal);
    pSimpleXML->GetAttributeValue(hEmissionRoot, L"blue", vAtrrib);
    if (vAtrrib.vt == VT_BSTR)
        if (CString(vAtrrib.bstrVal).GetLength() != 0)
            m_param.mat_emission1[2] = _wtof(vAtrrib.bstrVal);
    pSimpleXML->GetAttributeValue(hEmissionRoot, L"alpha", vAtrrib);
    if (vAtrrib.vt == VT_BSTR)
        if (CString(vAtrrib.bstrVal).GetLength() != 0)
            m_param.mat_emission1[3] = _wtof(vAtrrib.bstrVal);

    //определяет степень зеркального отражения материала
    hShininessRoot = pSimpleXML->GetNextNode(hEmissionRoot);
    pSimpleXML->GetAttributeValue(hShininessRoot, L"blink_coefficient", vAtrrib);
    if (vAtrrib.vt == VT_BSTR)
        if (CString(vAtrrib.bstrVal).GetLength() != 0)
            m_param.mat_shininess1 = _wtof(vAtrrib.bstrVal);

    hXml hGridColorRoot = pSimpleXML->GetNextNode(hButtonRoot);
    pSimpleXML->GetAttributeValue(hGridColorRoot, L"red", vAtrrib);
    if (vAtrrib.vt == VT_BSTR)
        if (CString(vAtrrib.bstrVal).GetLength() != 0)
            m_param.m_gridColor[0] = _wtof(vAtrrib.bstrVal);
    pSimpleXML->GetAttributeValue(hGridColorRoot, L"green", vAtrrib);
    if (vAtrrib.vt == VT_BSTR)
        if (CString(vAtrrib.bstrVal).GetLength() != 0)
            m_param.m_gridColor[1] = _wtof(vAtrrib.bstrVal);
    pSimpleXML->GetAttributeValue(hGridColorRoot, L"blue", vAtrrib);
    if (vAtrrib.vt == VT_BSTR)
        if (CString(vAtrrib.bstrVal).GetLength() != 0)
            m_param.m_gridColor[2] = _wtof(vAtrrib.bstrVal);

    hXml hTextColorRoot = pSimpleXML->GetNextNode(hGridColorRoot);
    pSimpleXML->GetAttributeValue(hTextColorRoot, L"red", vAtrrib);
    if (vAtrrib.vt == VT_BSTR)
        if (CString(vAtrrib.bstrVal).GetLength() != 0)
            m_param.m_textColor[0] = _wtof(vAtrrib.bstrVal);
    pSimpleXML->GetAttributeValue(hTextColorRoot, L"green", vAtrrib);
    if (vAtrrib.vt == VT_BSTR)
        if (CString(vAtrrib.bstrVal).GetLength() != 0)
            m_param.m_textColor[1] = _wtof(vAtrrib.bstrVal);
    pSimpleXML->GetAttributeValue(hTextColorRoot, L"blue", vAtrrib);
    if (vAtrrib.vt == VT_BSTR)
        if (CString(vAtrrib.bstrVal).GetLength() != 0)
            m_param.m_textColor[2] = _wtof(vAtrrib.bstrVal);

    m_param.m_fineVertGridStyle = SetLineParam(ltFirm, 1.0f, m_param.m_gridColor);
    m_param.m_fontH.SetColor(RGB(m_param.m_textColor[0], m_param.m_textColor[1], m_param.m_textColor[2]));
    m_param.m_fontV.SetColor(RGB(m_param.m_textColor[0], m_param.m_textColor[1], m_param.m_textColor[2]));
    m_param.m_font.SetColor(RGB(m_param.m_textColor[0], m_param.m_textColor[1], m_param.m_textColor[2]));

}
//------------------------------------------------------------------------------
void CDGrid::SetImageFileName(CString str)
{
    if ( !str.IsEmpty() && (m_param.m_textureName != str) )
    {
        m_param.m_textureName = str;
#ifdef _GDIPLUS
        LoadImageFromFile(&m_pImage, str);
#else
        Texture = LoadGLTextures(m_param.m_textureName, PicSize, m_param.Number_Texture);
#endif	
        IsChange();	
    }
}
//------------------------------------------------------------------------------
void CDGrid::SetGridType(GridType val){
	//m_param.m_gridType=val;
};
//------------------------------------------------------------------------------
LineStyle CDGrid::SetLineParam(LineType lineType, float Width, GLfloat *Color){
	LineStyle Object;
    Object.lpLineType=lineType;
	Object.lpWidth=Width;
	Object.lpColor[0]=Color[0];
    Object.lpColor[1]=Color[1];
    Object.lpColor[2]=Color[2];
    return Object;
};
//------------------------------------------------------------------------------
void CDGrid::SetTextColor(COLORREF color)
{
    m_param.m_textColor[0] = GetRValue(color);
    m_param.m_textColor[1] = GetGValue(color);
    m_param.m_textColor[2] = GetBValue(color);
    IsChange();
}
//------------------------------------------------------------------------------
void CDGrid::SetGridColor(COLORREF color)
{
    m_param.m_gridColor[0] = GetRValue(color);
    m_param.m_gridColor[1] = GetGValue(color);
    m_param.m_gridColor[2] = GetBValue(color);
    IsChange();
}
//------------------------------------------------------------------------------
void CDGrid::SetLegendColor(COLORREF color)
{
    m_param.m_legendColor[0] = GetRValue(color);
    m_param.m_legendColor[1] = GetGValue(color);
    m_param.m_legendColor[2] = GetBValue(color);
    IsChange();
}
//------------------------------------------------------------------------------
void CDGrid::SetCursorPosition(CPoint point){
    m_param.m_cursorPosition.x=point.x;
	m_param.m_cursorPosition.y=point.y;
};
//------------------------------------------------------------------------------
void CDGrid::SetRectAll(CRect leftShiftRect, CRect rightShiftRect,
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
    m_param.m_rectGrid = m_param.m_rect;
    m_param.m_centerAutoScale = centerAutoScaleRect;
    IsChange();
}
//------------------------------------------------------------------------------
void CDGrid::SetParam(){
    MakeGrid();
    //верхняя часть
    m_param.m_pBorders[0] = CDetail("C:\\ZETTools\\Interface2012\\Models\\Поддон\\Поддон нижний.stl",
        (m_param.m_rectGrid.left+m_param.m_rectGrid.right)/2.0,
        (m_param.m_rectGrid.top*2-5)/2.0-1, -10, 0, 
        m_param.m_rectGrid.Width()+1, 5, 10);
    //нижняя часть
    m_param.m_pBorders[1] = CDetail("C:\\ZETTools\\Interface2012\\Models\\Поддон\\Поддон верхний.stl",
        (m_param.m_rectGrid.left+m_param.m_rectGrid.right)/2.0,
        (m_param.m_rectGrid.bottom*2+5)/2.0+1, -10, 0, 
        m_param.m_rectGrid.Width(), 5, 10);
    //левая часть
    m_param.m_pBorders[2] = CDetail("C:\\ZETTools\\Interface2012\\Models\\Поддон\\Поддон левый.stl",
        (m_param.m_rectGrid.left*2-5)/2.0-1,
        (m_param.m_rectGrid.bottom+m_param.m_rectGrid.top)/2.0-1, -10, 0, 
        5, m_param.m_rectGrid.Height()+2, 10);
    //правая часть
    m_param.m_pBorders[3] = CDetail("C:\\ZETTools\\Interface2012\\Models\\Поддон\\Поддон правый.stl",
        (m_param.m_rectGrid.right*2+5)/2.0+1,
        (m_param.m_rectGrid.bottom+m_param.m_rectGrid.top)/2.0, -10, 0, 
        5, m_param.m_rectGrid.Height()-1, 10);
    //левый верхний угол
    m_param.m_pBorders[4] = CDetail("C:\\ZETTools\\Interface2012\\Models\\Поддон\\Поддон левый нижний.stl",
        (m_param.m_rectGrid.left*2-5)/2.0,
        (m_param.m_rectGrid.top*2-5)/2.0, -10, 0,
        7, 7, 10);
    //левый нижний угол
    m_param.m_pBorders[5] = CDetail("C:\\ZETTools\\Interface2012\\Models\\Поддон\\Поддон левый верхний.stl",
        (m_param.m_rectGrid.left*2-5)/2.0,
        (m_param.m_rectGrid.bottom*2+5)/2.0, -10, 0,
        7, 6, 10);
    //правый верхний угол
    m_param.m_pBorders[6] = CDetail("C:\\ZETTools\\Interface2012\\Models\\Поддон\\Поддон правый нижний.stl",
        (m_param.m_rectGrid.right*2+5)/2.0,
        (m_param.m_rectGrid.top*2-5)/2.0, -10, 0,
        7, 7, 10);
    //правый нижний угол
    m_param.m_pBorders[7] = CDetail("C:\\ZETTools\\Interface2012\\Models\\Поддон\\Поддон правый верхний.stl",
        (m_param.m_rectGrid.right*2+5)/2.0,
        (m_param.m_rectGrid.bottom*2+5)/2.0, -10, 0,
        7, 6, 10);

    m_pButton[0] = CDetail("C:\\ZETTools\\Interface2012\\Models\\Кнопка квадратная 22,5х22,5х7.stl",
        m_param.m_rectLeftShift.CenterPoint().x, m_param.m_rectLeftShift.CenterPoint().y, 0, 0, m_param.m_rectLeftShift.Width(), m_param.m_rectLeftShift.Height(), 0.3);
    m_pButton[1] = CDetail("C:\\ZETTools\\Interface2012\\Models\\Кнопка квадратная 22,5х22,5х7.stl",
        m_param.m_rectLeftScale.CenterPoint().x, m_param.m_rectLeftScale.CenterPoint().y, 0, 0, m_param.m_rectLeftScale.Width(), m_param.m_rectLeftScale.Height(), 0.3);
    m_pButton[2] = CDetail("C:\\ZETTools\\Interface2012\\Models\\Кнопка квадратная 22,5х22,5х7.stl",
        m_param.m_rectRightScale.CenterPoint().x, m_param.m_rectRightScale.CenterPoint().y, 0, 0, m_param.m_rectRightScale.Width(), m_param.m_rectRightScale.Height(), 0.3);
    m_pButton[3] = CDetail("C:\\ZETTools\\Interface2012\\Models\\Кнопка квадратная 22,5х22,5х7.stl",
        m_param.m_rectRightShift.CenterPoint().x, m_param.m_rectRightShift.CenterPoint().y, 0, 0, m_param.m_rectRightShift.Width(), m_param.m_rectRightShift.Height(), 0.3);
    m_pButton[4] = CDetail("C:\\ZETTools\\Interface2012\\Models\\Кнопка квадратная 22,5х22,5х7.stl",
        m_param.m_rectTopShift.CenterPoint().x, m_param.m_rectTopShift.CenterPoint().y, 0, 0, m_param.m_rectTopShift.Width(), m_param.m_rectTopShift.Height(), 0.3);
    m_pButton[5] = CDetail("C:\\ZETTools\\Interface2012\\Models\\Кнопка квадратная 22,5х22,5х7.stl",
        m_param.m_rectTopScale.CenterPoint().x, m_param.m_rectTopScale.CenterPoint().y, 0, 0, m_param.m_rectTopScale.Width(), m_param.m_rectTopScale.Height(), 0.3);
    m_pButton[6] = CDetail("C:\\ZETTools\\Interface2012\\Models\\Кнопка квадратная 22,5х22,5х7.stl",
        m_param.m_rectBottomScale.CenterPoint().x, m_param.m_rectBottomScale.CenterPoint().y, 0, 0, m_param.m_rectBottomScale.Width(), m_param.m_rectBottomScale.Height(), 0.3);
    m_pButton[7] = CDetail("C:\\ZETTools\\Interface2012\\Models\\Кнопка квадратная 22,5х22,5х7.stl",
        m_param.m_rectBottomShift.CenterPoint().x, m_param.m_rectBottomShift.CenterPoint().y, 0, 0, m_param.m_rectBottomShift.Width(), m_param.m_rectBottomShift.Height(), 0.3);
    m_pButton[8] = CDetail("C:\\ZETTools\\Interface2012\\Models\\Кнопка квадратная 22,5х22,5х7.stl",
        m_param.m_centerAutoScale.CenterPoint().x, m_param.m_centerAutoScale.CenterPoint().y, 0, 0, m_param.m_centerAutoScale.Width(), m_param.m_centerAutoScale.Height(), 0.3);
    IsChange();

}
//------------------------------------------------------------------------------
void CDGrid::SetMinMaxCurrent(float MinX, float MaxX, float MinY, float MaxY){
    m_param.m_axisX.apMinCurrent = MinX;
    m_param.m_axisX.apMaxCurrent = MaxX;
    m_param.m_axisY.apMinCurrent = MinY;
    m_param.m_axisY.apMaxCurrent = MaxY;
    SetParam();
    IsChange();
};
//------------------------------------------------------------------------------
void CDGrid::SetStep(double stepX, double stepY){
    m_param.m_axisX.apStep = stepX;
    m_param.m_axisY.apStep = stepY;
    m_param.m_setting = sManual;
    SetParam();
    IsChange();
}
//------------------------------------------------------------------------------
double CDGrid::GetStep(GridType gridType)
{
    if(gridType == gtVertical)
        return m_param.m_axisY.apStep;
    if(gridType == gtHorizontal)
        return m_param.m_axisX.apStep;
}
//------------------------------------------------------------------------------
void CDGrid::SetAxis(double minX, double maxX, double minY, double maxY)
{
    m_param.m_axisX.apMinCurrent = minX;
    m_param.m_axisX.apMaxCurrent = maxX;
    m_param.m_axisY.apMinCurrent = minY;
    m_param.m_axisY.apMaxCurrent = maxY;
    SetParam();
    IsChange();
};
//------------------------------------------------------------------------------
TwoDouble CDGrid::GetAxis(GridType gridType)
{
    TwoDouble Axis;
    if(gridType == gtHorizontal)
    {
        Axis.Min = m_param.m_axisX.apMinCurrent;
        Axis.Max = m_param.m_axisX.apMaxCurrent;
    }
    if(gridType == gtVertical)
    {
        Axis.Min = m_param.m_axisY.apMinCurrent;
        Axis.Max = m_param.m_axisY.apMaxCurrent;
    }
    return Axis;
}
//------------------------------------------------------------------------------
void CDGrid::SetBounds(double minX, double maxX, double minY, double maxY)
{
    m_param.m_axisX.apBegin = minX;
    m_param.m_axisX.apEnd = maxX;
    m_param.m_axisY.apBegin = minY;
    m_param.m_axisY.apEnd = maxY;
    IsChange();
}
//------------------------------------------------------------------------------
void CDGrid::SetScaleType(ScaleType scaleTypeX, ScaleType scaleTypeY){
    m_param.m_axisX.apScaleType = scaleTypeX;
    m_param.m_axisY.apScaleType = scaleTypeY;
}
//------------------------------------------------------------------------------
void CDGrid::SetScale(double scaleX, double scaleY){
    m_param.m_axisX.apScale = scaleX;
    m_param.m_axisY.apScale = scaleY;
    IsChange();
}
//------------------------------------------------------------------------------
double CDGrid::GetScale(GridType gridType)
{
    if(gridType == gtVertical)
        return m_param.m_axisY.apScale;
    if(gridType == gtHorizontal)
        return m_param.m_axisX.apScale;
}
//------------------------------------------------------------------------------
void CDGrid::SetShift(double shiftX, double shiftY){
    m_param.m_axisX.apShift = shiftX;
    m_param.m_axisY.apShift = shiftY;
    IsChange();
}
//------------------------------------------------------------------------------
double CDGrid::GetShift(GridType gridType)
{
    if(gridType == gtHorizontal)
        return m_param.m_axisX.apShift;
    if(gridType == gtVertical)
        return m_param.m_axisY.apShift;
};
//------------------------------------------------------------------------------
void CDGrid::SetBorder(double leftBorder, double rightBorder, double topBorder, double bottomBorder){
    m_param.m_topBorder = topBorder;
    m_param.m_bottomBorder = bottomBorder;
    m_param.m_leftBorder = leftBorder;
    m_param.m_rightBorder = rightBorder;
    MakeGrid();
    IsChange();
}
//------------------------------------------------------------------------------
void CDGrid::SetFit(FitType fitTypeX, FitType fitTypeY)
{
    m_param.m_fitTypeX = fitTypeX;
    m_param.m_fitTypeY = fitTypeY;
}
//------------------------------------------------------------------------------
void CDGrid::SetAxisTitle(CString titleX, CString titleY)
{
    m_param.m_axisX.apTitle = titleX;
    m_param.m_axisY.apTitle = titleY;
}
//------------------------------------------------------------------------------
void CDGrid::EnableStyle(LineStyle Object){
	if(Object.lpLineType==ltDot){
		glEnable(GL_LINE_STIPPLE);
		glLineStipple(1,0x1010101010101010);
	}
	glColor3ub(Object.lpColor[0], Object.lpColor[1], Object.lpColor[2]);
	glLineWidth(Object.lpWidth);
};
//------------------------------------------------------------------------------
void CDGrid::DisableStyle(LineStyle Object){
	if(Object.lpLineType==ltDot)    glDisable(GL_LINE_STIPPLE);
};
//------------------------------------------------------------------------------
void CDGrid::MakeGrid(){
    AxisParam Axis[2];
    int Counter, Counter1, Counter2, QntOFIntervals, QntOfMarks;
    double minStep, Power;

    Axis[0] = m_param.m_axisX;
    Axis[1] = m_param.m_axisY;

    for(Counter=0; Counter<2; Counter++){
        if(m_param.m_setting == sManual){
            Axis[Counter] = FillStruct(Axis[Counter].apMaxCurrent,
                Axis[Counter].apMinCurrent, Axis[Counter].apStep,
                Axis[Counter].apGridType, Axis[Counter].apScaleType);
            if(Axis[Counter].apScaleType == stLinear){
                for(Counter1=0; Counter1<Axis[Counter].apQntMarks; Counter1++){
                    Axis[Counter].apMarkingArray[Counter1] = Axis[Counter].apStep*Counter1;
                };
            }
        }
        if(m_param.m_setting == sAuto){
            Axis[Counter] = FillStruct(Axis[Counter].apMaxCurrent,
                Axis[Counter].apMinCurrent, Axis[Counter].apGridType,
                Axis[Counter].apScaleType);

            //заполнение массива для линейной шкалы
            if(Axis[Counter].apScaleType == stLinear){
                Power=log10(Axis[Counter].apStep);
                if(Power<0) Power=floor(Power);
                else Power=(int)Power;
                Axis[Counter].apMarkingArray[0] = z_round(Axis[Counter].apMinCurrent
                    +Axis[Counter].apStep, Power)-Axis[Counter].apMinCurrent;
                if(FromWorldToScreen(Axis[Counter].apMarkingArray[0], Axis[Counter])>Axis[Counter].apPxStep)
                    Axis[Counter].apMarkingArray[0] = Axis[Counter].apMarkingArray[0] - Axis[Counter].apStep;

                for(Counter1=1; Counter1<Axis[Counter].apQntMarks; Counter1++){
                    Axis[Counter].apMarkingArray[Counter1] =
                        Axis[Counter].apMarkingArray[Counter1-1]+Axis[Counter].apStep;
                };
            }
            //заполнение масиива для логарифмической шкалы
            if(Axis[Counter].apScaleType == stLog){
                minStep=FromScreenToWorld(MIN_STEP_LOG, Axis[Counter]);
                for(Counter1=sizeof(StepConstants)/sizeof(StepConstants[0])-1;
                    Counter1>0; Counter1--){
                        if(minStep<=StepConstants[Counter1]-StepConstants[Counter1-1]){
                            minStep=StepConstants[Counter1];
                            break;
                        }
                        minStep=StepConstants[Counter1];
                }
                QntOFIntervals = log10(Axis[Counter].apMaxCurrent)
                    -log10(Axis[Counter].apMinCurrent);
                QntOfMarks = Axis[Counter].apQntMarks = Counter1+1;

                for(Counter2=0; Counter2<QntOFIntervals; Counter2++){
                    for(Counter1=Counter2*QntOfMarks;
                        Counter1<(1+Counter2)*QntOfMarks; Counter1++){
                            Axis[Counter].apMarkingArray[Counter1]=Counter2
                                +StepConstants[Counter1%QntOfMarks];
                    }
                }
            }
            for(Counter1=1;;Counter1++){
                if((Axis[Counter].apQntMarks/Counter1)<=Axis[Counter].apQntTextMarks){
                    Axis[Counter].apQntTextMarks = Axis[Counter].apQntMarks/Counter1;
                    break;
                }
            }
        }
    }
    m_param.m_axisX = Axis[0];
    m_param.m_axisY = Axis[1];
}
//------------------------------------------------------------------------------
AxisParam CDGrid::FillStruct(double maxCurrent, double minCurrent,
    GridType gridType, ScaleType scaleType){

    double Power, Step, Number, tmp, pxStep, Num;
    int Counter, QntMarks, QntTextMarks;
    AxisParam axis;
    char String[20];
    wchar_t str[20];
    Dimention Dim;

    if(gridType == gtHorizontal){
        axis = m_param.m_axisX;
    }
    else{
        axis = m_param.m_axisY;
    }

    axis.apMaxCurrent = maxCurrent;
    axis.apMinCurrent = minCurrent;
    axis.apGridType = gridType;
    axis.apScaleType = scaleType;

    Number=axis.apMaxCurrent-axis.apMinCurrent;
    Power=log10(Number);
    Step=z_getStep(Power, (Power-(int)Power));
    pxStep=FromWorldToScreen(Step, axis);
    if(pxStep>MAX_STEP){
        Step=FromScreenToWorld(MAX_STEP, axis);
        Power=log10(Step);
        if(Power<0) Power=floor(Power);
        else Power=(int)Power;
        Step=z_round(Step, Power);
        pxStep=FromWorldToScreen(Step, axis);
    }

    tmp=m_param.m_rect.Width()/pxStep;
    axis.apQntMarks=floor(tmp+0.5);
    axis.apStep = Step;
    axis.apPxStep = pxStep;
    axis.apShift = Step/2;

    Num = 1111111;
    Dim = TextDimention(Num);
    axis.apMaxTextLength = Dim.dWidth;
    axis.apMaxTexthHeight = Dim.dHeight;

    if(gridType == gtHorizontal){
        axis.apQntTextMarks = m_param.m_rect.Width() / axis.apMaxTextLength; //количество блоков. т.е. сколько меток с интервалом 1 между ними возможно расставить
    }
    else{
        axis.apQntTextMarks = m_param.m_rect.Height() / axis.apMaxTexthHeight;
    }
    if(axis.apQntMarks - axis.apQntTextMarks < 0)
        axis.apQntTextMarks = axis.apQntMarks;

    return axis;
}
//------------------------------------------------------------------------------
AxisParam CDGrid::FillStruct(double maxCurrent, double minCurrent, double step, GridType gridType, ScaleType scaleType){
    AxisParam axis;
    Dimention Dim;
    double Num, pxStep;

    if(gridType == gtHorizontal){
        axis = m_param.m_axisX;
        axis.apQntMarks = QNT_STEPS_X;
    }
    else{
        axis = m_param.m_axisY;
        axis.apQntMarks = QNT_STEPS_Y;
    }
    axis.apMaxCurrent = maxCurrent;
    axis.apMinCurrent = minCurrent;
    axis.apScaleType = scaleType;
    axis.apGridType = gridType;
    axis.apStep = step;

    pxStep=FromWorldToScreen(step, axis);
    axis.apPxStep = pxStep;

    Num = 1111111;
    Dim = TextDimention(Num);
    axis.apMaxTextLength = Dim.dWidth;
    axis.apMaxTexthHeight = Dim.dHeight;

    if(axis.apGridType == gtHorizontal){
        axis.apQntTextMarks = m_param.m_rect.Width() / axis.apMaxTextLength; //количество блоков. т.е. сколько меток с интервалом 1 между ними возможно расставить
    }
    else{
        axis.apQntTextMarks = m_param.m_rect.Height() / axis.apMaxTexthHeight;
    }
    if(axis.apQntMarks - axis.apQntTextMarks < 0)
        axis.apQntTextMarks = axis.apQntMarks;

    return axis;
}
//------------------------------------------------------------------------------
Dimention CDGrid::TextDimention(double val){
    char String[20];
    wchar_t str[20];
    int Counter, Size;
    double Power;
    Dimention Dim;

    sprintf_s(String, "%0.3g", val);

    for(Counter=0, Size = strlen(String); Counter < Size+1; Counter++){
        str[Counter] = '0';
    }
    str[Counter] = '\0';

    FTBBox bbox;
    FTPoint p1, p2;
    bbox = m_param.m_font.GetFont()->BBox(&(str[0]), wcslen(str), p1, p2);
    Dim.dWidth = (int)(bbox.Upper().Xf() - bbox.Lower().Xf())+5;

    bbox = m_param.m_font.GetFont()->BBox(&(str[0]), wcslen(str), p1, p2);
    Dim.dHeight = (bbox.Upper().Yf() - bbox.Lower().Yf());
    
    return Dim;
}
//------------------------------------------------------------------------------
Dimention CDGrid::TextDimention(CString val){
    int Counter;
    wchar_t String[200];
    Dimention Dim;

    for(Counter=0; Counter<val.GetLength(); Counter++){
        String[Counter] = val[Counter];
    }
    String[Counter] = '\0';
    FTBBox bbox;
    FTPoint p1, p2;
    bbox = m_param.m_font.GetFont()->BBox(&(String[0]), wcslen(String), p1, p2);
    Dim.dWidth = (int)(bbox.Upper().Xf() - bbox.Lower().Xf())+5;

    bbox = m_param.m_font.GetFont()->BBox(&(String[0]), wcslen(String), p1, p2);
    Dim.dHeight = (bbox.Upper().Yf() - bbox.Lower().Yf());
    return Dim;
}
//------------------------------------------------------------------------------
double CDGrid::FromWorldToScreen(double val, AxisParam axis){
    double Length;
    if(axis.apGridType == gtHorizontal) Length = m_param.m_rect.Width();
    if(axis.apGridType == gtVertical) Length = m_param.m_rect.Height();

    if(axis.apScaleType==stLinear)
        return val/(axis.apMaxCurrent-axis.apMinCurrent)*(double)(Length);
    else return val/(log10(axis.apMaxCurrent)
        -log10(axis.apMinCurrent))*(Length);
};
//------------------------------------------------------------------------------
double CDGrid::FromScreenToWorld(double pixel, AxisParam axis){
    double Length;
    if(axis.apGridType == gtHorizontal) Length = m_param.m_rect.Width();
    if(axis.apGridType == gtVertical) Length = m_param.m_rect.Height();

	if(axis.apScaleType==stLinear)
        return pixel*(axis.apMaxCurrent-axis.apMinCurrent)/(Length);
	else return pixel*(log10(axis.apMaxCurrent)
        -log10(axis.apMinCurrent))/(Length);
};
//------------------------------------------------------------------------------
zRect CDGrid::IsMouseInRect(CPoint point){

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

    if(PointInRect(point, &m_param.m_rectGrid) == true)
        return gGridArea;

    if(PointInRect(point, &m_param.m_centerAutoScale) == true)
        return gCenterAutoScale;
    
    return gNone;
};
//------------------------------------------------------------------------------
void CDGrid::Shift(zRect val){
    double temp;
	if(val==gLeftShift){
        if((m_param.m_axisX.apMinCurrent-m_param.m_axisX.apShift)
            < m_param.m_axisX.apBegin){
                temp = m_param.m_axisX.apMinCurrent-m_param.m_axisX.apBegin;
                m_param.m_axisX.apMinCurrent = m_param.m_axisX.apBegin;
                m_param.m_axisX.apMaxCurrent = m_param.m_axisX.apMaxCurrent
                    -temp;
        }
        else{
            m_param.m_axisX.apMinCurrent = m_param.m_axisX.apMinCurrent
                -m_param.m_axisX.apShift;
            m_param.m_axisX.apMaxCurrent = m_param.m_axisX.apMaxCurrent
                -m_param.m_axisX.apShift;
        }
	}
	if(val==gRightShift){
        if((m_param.m_axisX.apMaxCurrent+m_param.m_axisX.apShift)
            > m_param.m_axisX.apEnd){
                temp = m_param.m_axisX.apEnd - m_param.m_axisX.apMaxCurrent;
                m_param.m_axisX.apMaxCurrent = m_param.m_axisX.apEnd;
                m_param.m_axisX.apMinCurrent = m_param.m_axisX.apMinCurrent
                    + temp;
        }
        else{
            m_param.m_axisX.apMinCurrent=m_param.m_axisX.apMinCurrent
                +m_param.m_axisX.apShift;
            m_param.m_axisX.apMaxCurrent=m_param.m_axisX.apMaxCurrent
                +m_param.m_axisX.apShift;
        }
	}
	if(val==gTopShift){
        if((m_param.m_axisY.apMaxCurrent+m_param.m_axisY.apShift)
            > m_param.m_axisY.apEnd){
                temp = m_param.m_axisY.apEnd - m_param.m_axisY.apMaxCurrent;
                m_param.m_axisY.apMaxCurrent = m_param.m_axisY.apEnd;
                m_param.m_axisY.apMinCurrent = m_param.m_axisY.apMinCurrent
                    +temp;
        }
        else{
            m_param.m_axisY.apMinCurrent=m_param.m_axisY.apMinCurrent
                +m_param.m_axisY.apShift;
            m_param.m_axisY.apMaxCurrent=m_param.m_axisY.apMaxCurrent
                +m_param.m_axisY.apShift;
        }
	}
	if(val==gBottomShift){
        if((m_param.m_axisY.apMinCurrent-m_param.m_axisY.apShift)
            < m_param.m_axisY.apBegin){
                temp = m_param.m_axisY.apMinCurrent-m_param.m_axisY.apBegin;
                m_param.m_axisY.apMinCurrent = m_param.m_axisY.apBegin;
                m_param.m_axisY.apMaxCurrent=m_param.m_axisY.apMaxCurrent
                    -temp;
        }
        else{
            m_param.m_axisY.apMinCurrent=m_param.m_axisY.apMinCurrent
                -m_param.m_axisY.apShift;
            m_param.m_axisY.apMaxCurrent=m_param.m_axisY.apMaxCurrent
                -m_param.m_axisY.apShift;
        }
	}
	if(val==gNone) return;

	ShiftGrid(val);
};
//------------------------------------------------------------------------------
void CDGrid::ShiftGrid(zRect val)
{
	if(val==gLeftShift || val==gRightShift){
		if(m_param.m_axisX.apScaleType==stLinear){
            if(!FlagX){
                m_param.m_axisX.apMarkingArray[0] = m_param.m_axisX.apStep - m_param.m_axisX.apShift;
//                    m_param.m_axisX.apShift;
                FlagX = true;
            }
            else{
                m_param.m_axisX.apMarkingArray[0] = m_param.m_axisX.apStep;
                FlagX = false;
            }
			for(int Counter=1; Counter<m_param.m_axisX.apQntMarks; Counter++){
				m_param.m_axisX.apMarkingArray[Counter]
                   = m_param.m_axisX.apMarkingArray[Counter-1]
                       +m_param.m_axisX.apStep;
			}
		}
		else;
	}

	if(val==gTopShift || val==gBottomShift){
		if(m_param.m_axisY.apScaleType==stLinear){
            if(!FlagY){
                m_param.m_axisY.apMarkingArray[m_param.m_axisY.apQntMarks-1]
                    = m_param.m_axisY.apStep - m_param.m_axisY.apShift;

                FlagY = true;
            }
            else{
                m_param.m_axisY.apMarkingArray[m_param.m_axisY.apQntMarks-1]
                    = m_param.m_axisY.apStep;
                FlagY = false;
            }
            for(int Counter=m_param.m_axisY.apQntMarks-1; Counter>0; Counter--){
                m_param.m_axisY.apMarkingArray[Counter-1]
                = m_param.m_axisY.apMarkingArray[Counter]+m_param.m_axisY.apStep;
            }
        }
	};
};
//------------------------------------------------------------------------------
void CDGrid::Scale(zRect val)
{
    double Power, minValue, maxValue;

    if(val==gLeftScale){
        minValue = maxValue = (m_param.m_axisX.apMaxCurrent-m_param.m_axisX.apMinCurrent)*(m_param.m_axisX.apScale-1)/2.0;
        if(m_param.m_fitTypeX == ftLeft)
            m_param.m_axisX.apMaxCurrent = m_param.m_axisX.apMaxCurrent+maxValue*2;
        if(m_param.m_fitTypeX == ftCenter)
        {
            m_param.m_axisX.apMinCurrent = m_param.m_axisX.apMinCurrent-minValue;
            m_param.m_axisX.apMaxCurrent = m_param.m_axisX.apMaxCurrent+maxValue;
        }
        if(m_param.m_fitTypeX == ftRight)
            m_param.m_axisX.apMinCurrent = m_param.m_axisX.apMinCurrent-minValue*2;
        m_param.m_axisX.apStep = m_param.m_axisX.apStep*m_param.m_axisX.apScale;
        m_param.m_axisX.apShift = m_param.m_axisX.apShift*m_param.m_axisX.apScale;
    }
    if(val==gRightScale){
        minValue = maxValue = (m_param.m_axisX.apMaxCurrent-m_param.m_axisX.apMinCurrent)/2.0*(1-1/m_param.m_axisX.apScale);
        if(m_param.m_fitTypeX == ftLeft)
            m_param.m_axisX.apMaxCurrent = m_param.m_axisX.apMaxCurrent-maxValue*2;
        if(m_param.m_fitTypeX == ftCenter)
        {
            m_param.m_axisX.apMinCurrent = m_param.m_axisX.apMinCurrent+minValue;
            m_param.m_axisX.apMaxCurrent = m_param.m_axisX.apMaxCurrent-maxValue;
        }
        if(m_param.m_fitTypeX == ftRight)
            m_param.m_axisX.apMinCurrent = m_param.m_axisX.apMinCurrent+minValue*2;
        m_param.m_axisX.apStep = m_param.m_axisX.apStep/m_param.m_axisX.apScale;
        m_param.m_axisX.apShift = m_param.m_axisX.apShift/m_param.m_axisX.apScale;
    }

    if(val==gTopScale){
        minValue = maxValue = (m_param.m_axisY.apMaxCurrent-m_param.m_axisY.apMinCurrent)/2.0*(1-1/m_param.m_axisY.apScale);
        if(m_param.m_fitTypeY == ftLeft)
            m_param.m_axisY.apMaxCurrent = m_param.m_axisY.apMaxCurrent-maxValue*2;
        if(m_param.m_fitTypeY == ftCenter)
        {
            m_param.m_axisY.apMinCurrent = m_param.m_axisY.apMinCurrent+minValue;
            m_param.m_axisY.apMaxCurrent = m_param.m_axisY.apMaxCurrent-maxValue;
        }
        if(m_param.m_fitTypeY == ftRight)
            m_param.m_axisY.apMinCurrent = m_param.m_axisY.apMinCurrent+minValue*2;
        m_param.m_axisY.apStep = m_param.m_axisY.apStep/m_param.m_axisY.apScale;
        m_param.m_axisY.apShift = m_param.m_axisY.apShift/m_param.m_axisY.apScale;
    }

    if(val==gBottomScale){
        minValue = maxValue = (m_param.m_axisY.apMaxCurrent-m_param.m_axisY.apMinCurrent)*(m_param.m_axisY.apScale-1)/2.0;
        if(m_param.m_fitTypeY == ftLeft)
            m_param.m_axisY.apMaxCurrent = m_param.m_axisY.apMaxCurrent+maxValue*2;
        if(m_param.m_fitTypeY == ftCenter)
        {
            m_param.m_axisY.apMinCurrent = m_param.m_axisY.apMinCurrent-minValue;
            m_param.m_axisY.apMaxCurrent = m_param.m_axisY.apMaxCurrent+maxValue;
        }
        if(m_param.m_fitTypeY == ftRight)
            m_param.m_axisY.apMinCurrent = m_param.m_axisY.apMinCurrent-minValue*2;
        m_param.m_axisY.apStep = m_param.m_axisY.apStep*m_param.m_axisY.apScale;
        m_param.m_axisY.apShift = m_param.m_axisY.apShift*m_param.m_axisY.apScale;
    }
    
    if(val==gNone) return;
    MakeGrid();
};
//------------------------------------------------------------------------------
void CDGrid::OnLButtonDown(UINT nFlags, CPoint point)
{
    zRect val;
    val=IsMouseInRect(point);
    Scale(val);
    Shift(val);
    IsChange();
}
//------------------------------------------------------------------------------
void CDGrid::OnMouseMove(UINT nFlags, CPoint point)
{
    zRect val;
    unsigned int andMask=1, xorMask=0;
    SetCursorPosition(point);
    val=IsMouseInRect(point);
    IsChange();
}
//------------------------------------------------------------------------------
void CDGrid::OnMouseEnter()
{
    m_bFlag = true;
    CCustomDraw::OnMouseEnter();
}
//------------------------------------------------------------------------------
void CDGrid::OnMouseLeave()
{
    if (m_bFlag)
        m_bFlag = false;
    CCustomDraw::OnMouseLeave();
    IsChange();
}
//------------------------------------------------------------------------------