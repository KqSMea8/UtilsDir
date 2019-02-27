//------------------------------------------------------------------------------
// DGraph.cpp : файл реализации
//------------------------------------------------------------------------------
#include "stdafx.h"
#include <Interface2012\Draw\DGraph.h>

bool GraphMutex = false;

int g_nGraph = 0;

CDGraph::CDGraph(CRect rect, CRect gridRect, bool *pFlagIsChange, double *pTimeAnimation) :
CCustomDraw(rect, pFlagIsChange, pTimeAnimation)
{
    m_pParamDraw = &m_param;	// обеспечение ќЅў»’ параметров
    m_param.nElement = ++g_nGraph;
    SetNameWithNumber(L"Graph");
	m_rect = rect;
    m_param.m_rect=rect;
    m_param.m_gridRect = gridRect;
    m_param.font.SetColor(RGB(0,0,0));
    m_param.font.SetAligmentHorizontal(StrAlignmentCenter);
    m_param.font.SetAligmentVertical(StrAlignmentCenter);
};
//------------------------------------------------------------------------------
CDGraph::~CDGraph()
{
	for (int i = 0; i < 13; i++)
	{
		delete m_pButton[i];
		m_pButton[i] = nullptr;
	}
	for (int j = 0; j < 8; j++)
	{
		delete m_pBorders[j];
		m_pBorders[j] = nullptr;
	}
};
//------------------------------------------------------------------------------
 void CDGraph::OnDraw()
 {
	 glPushAttrib(GL_ENABLE_BIT | GL_LIGHTING_BIT);
	 glPushClientAttrib(GL_ENABLE_BIT | GL_LIGHTING_BIT);
	 DrawBorders();
	 DrawButtons();
     glEnable(GL_COLOR_MATERIAL);
	 glDisable(GL_LIGHTING);
	 //DrawArrows();
     DrawLegend();
	 DrawBackGround();
	 glEnable(GL_LIGHTING);
     glDisable(GL_COLOR_MATERIAL);
	 glPopClientAttrib();
	 glPopAttrib();
 }
//------------------------------------------------------------------------------
 void CDGraph::DrawButtons()
 {
     glPushMatrix();
     glLoadIdentity();

     //glActiveTextureARB(GL_TEXTURE0_ARB);
     //glBindTexture(GL_TEXTURE_2D, NormalMap); //дл€ мультитекстурировани€
     //glActiveTextureARB(GL_TEXTURE1_ARB);
     glBindTexture(GL_TEXTURE_2D, ArrowTexture);

     for(int Counter = 0; Counter < 13; Counter++)
	 {
         if(Counter == m_param.ActiveArrow)
         {
            glMaterialfv(GL_FRONT, GL_AMBIENT, m_param.mat_ambient_b);
            glMaterialfv(GL_FRONT, GL_DIFFUSE, m_param.mat_diffuse_b);
            glMaterialfv(GL_FRONT, GL_EMISSION, m_param.mat_emission_b);
            glMaterialfv(GL_FRONT, GL_SPECULAR, m_param.mat_specular_b);
            glMaterialf(GL_FRONT, GL_SHININESS, m_param.mat_shininess_b);

            if(m_param.ActiveArrow == 1 || m_param.ActiveArrow == 3 || m_param.ActiveArrow == 7 || m_param.ActiveArrow == 9)
            {
                m_pButton[Counter]->Draw_texture(m_pButton[Counter]->ReadFacet(), m_pButton[Counter]->GetQntPlanes(), PicSize1, true);
                Counter++;
            }
         }
         else
         {
#ifdef _ACTIVEX2012
			 glMaterialfv(GL_FRONT, GL_AMBIENT, m_param.mat_ambient_a);
			 glMaterialfv(GL_FRONT, GL_DIFFUSE, m_param.mat_diffuse_a);
			 glMaterialfv(GL_FRONT, GL_EMISSION, m_param.mat_emission_a);
			 glMaterialfv(GL_FRONT, GL_SPECULAR, m_param.mat_specular_a);
			 glMaterialf(GL_FRONT, GL_SHININESS, m_param.mat_shininess_a);
#else
			 glMaterialfv(GL_FRONT, GL_AMBIENT, m_param.mat_ambient);
			 glMaterialfv(GL_FRONT, GL_DIFFUSE, m_param.mat_diffuse);
			 glMaterialfv(GL_FRONT, GL_EMISSION, m_param.mat_emission);
			 glMaterialfv(GL_FRONT, GL_SPECULAR, m_param.mat_specular);
			 glMaterialf(GL_FRONT, GL_SHININESS, m_param.mat_shininess);
#endif
         }

         m_pButton[Counter]->Draw_texture(m_pButton[Counter]->ReadFacet(), m_pButton[Counter]->GetQntPlanes(), PicSize1, true);
		 
         //m_pButton[Counter].Draw_texture_VBO(m_param.VBO, m_pButton[Counter].ReadFacet(), m_pButton[Counter].GetQntPlanes(), PicSize, true);
         //m_pButton[Counter].Draw_multitexture(m_pButton[Counter].ReadFacet(), m_pButton[Counter].GetQntPlanes());

     }
     glPopMatrix();
 }
//------------------------------------------------------------------------------
void CDGraph::DrawBorders()
{
     glPushMatrix();
     glLoadIdentity();
#ifdef _ACTIVEX2012
	 glMaterialfv(GL_FRONT, GL_AMBIENT, m_param.mat_ambient_a);
	 glMaterialfv(GL_FRONT, GL_DIFFUSE, m_param.mat_diffuse_a);
	 glMaterialfv(GL_FRONT, GL_EMISSION, m_param.mat_emission_a);
	 glMaterialfv(GL_FRONT, GL_SPECULAR, m_param.mat_specular_a);
	 glMaterialf(GL_FRONT, GL_SHININESS, m_param.mat_shininess_a);
#else
	 glMaterialfv(GL_FRONT, GL_AMBIENT, m_param.mat_ambient);
	 glMaterialfv(GL_FRONT, GL_DIFFUSE, m_param.mat_diffuse);
	 glMaterialfv(GL_FRONT, GL_EMISSION, m_param.mat_emission);
	 glMaterialfv(GL_FRONT, GL_SPECULAR, m_param.mat_specular);
	 glMaterialf(GL_FRONT, GL_SHININESS, m_param.mat_shininess);
#endif
     glBindTexture(GL_TEXTURE_2D, Texture);
     for(int Counter = 0; Counter < 8; Counter++)
	 {
         m_pBorders[Counter]->Draw_texture(m_pBorders[Counter]->ReadFacet(), m_pBorders[Counter]->GetQntPlanes(), PicSize, true);
     }
	 m_pDecorative->Draw(m_pDecorative->ReadFacet(), m_pDecorative->GetQntPlanes());
     glPopMatrix();
};
//------------------------------------------------------------------------------
 void CDGraph::DrawArrows()
 {
//     glPushMatrix();
//     glLoadIdentity();
//     glPushAttrib(GL_ENABLE_BIT | GL_LIGHTING_BIT);
//     glPushClientAttrib(GL_ENABLE_BIT | GL_LIGHTING_BIT);
//     glMaterialfv(GL_FRONT, GL_AMBIENT, m_param.mat_ambient);
//     glMaterialfv(GL_FRONT, GL_DIFFUSE, m_param.mat_diffuse);
//     glMaterialfv(GL_FRONT, GL_EMISSION, m_param.mat_emission);
//     glMaterialfv(GL_FRONT, GL_SPECULAR, m_param.mat_specular);
//     glMaterialf(GL_FRONT, GL_SHININESS, m_param.mat_shininess);
//
//     float x = (m_param.m_bottomShiftRect.Width() - imageSize[0].PicSize[0])/2;
//     float y = (m_param.m_bottomShiftRect.Height() - imageSize[0].PicSize[1])/2;
//
//     if(x < 0)
//         x=0;
//     if(y < 0)
//         y=0;
//     glDisable(GL_LIGHTING);
//     glEnable(GL_TEXTURE_2D);
//     glDisable(GL_DEPTH_TEST);
//     glEnable(GL_BLEND);
//     //glBlendFunc(GL_DST_COLOR,GL_SRC_COLOR);
//     //glBlendFunc(GL_ZERO, GL_DST_COLOR);
//     //glBlendEquation(GL_FUNC_SUBTRACT);
//
////	 (PFNGLBLENDEQUATIONSEPARATEPROC)wglGetProcAddress("glBlendEquation");
////	 glBlendEquation(GL_FUNC_REVERSE_SUBTRACT);
//	 glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
//
//     //glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
//
//     glBindTexture(GL_TEXTURE_2D, ArrowTextures);
//     glBegin(GL_QUADS);
//     glNormal3f( 0.0f, 0.0f, 1.0f);
//     glTexCoord2f(0.0f, 0.0f); glVertex3f(m_param.m_topShiftRect.left+x, m_param.m_topShiftRect.top+y, 100.0f);
//     glTexCoord2f(1.0f, 0.0f); glVertex3f(m_param.m_topShiftRect.right-x, m_param.m_topShiftRect.top+y, 100.0f);
//     glTexCoord2f(1.0f, 1.0f); glVertex3f(m_param.m_topShiftRect.right-x, m_param.m_topShiftRect.bottom-y, 100.0f);
//     glTexCoord2f(0.0f, 1.0f); glVertex3f(m_param.m_topShiftRect.left+x,  m_param.m_topShiftRect.bottom-y, 100.0f);
//     glEnd();
//
//     glBindTexture(GL_TEXTURE_2D, ArrowTextures[1]);
//     glBegin(GL_QUADS);
//     glNormal3f( 0.0f, 0.0f, 1.0f);
//     glTexCoord2f(0.0f, 0.0f); glVertex3f(m_param.m_topScaleRect.left+x, m_param.m_topScaleRect.top+y, 100.0f);
//     glTexCoord2f(1.0f, 0.0f); glVertex3f(m_param.m_topScaleRect.right-x, m_param.m_topScaleRect.top+y, 100.0f);
//     glTexCoord2f(1.0f, 1.0f); glVertex3f(m_param.m_topScaleRect.right-x, m_param.m_topScaleRect.bottom-y, 100.0f);
//     glTexCoord2f(0.0f, 1.0f); glVertex3f(m_param.m_topScaleRect.left+x,  m_param.m_topScaleRect.bottom-y, 100.0f);
//     glEnd();
//
//     glBindTexture(GL_TEXTURE_2D, ArrowTextures[2]);
//     glBegin(GL_QUADS);
//     glNormal3f( 0.0f, 0.0f, 1.0f);
//     glTexCoord2f(0.0f, 0.0f); glVertex3f(m_param.m_bottomScaleRect.left+x, m_param.m_bottomScaleRect.top+y, 100.0f);
//     glTexCoord2f(1.0f, 0.0f); glVertex3f(m_param.m_bottomScaleRect.right-x, m_param.m_bottomScaleRect.top+y, 100.0f);
//     glTexCoord2f(1.0f, 1.0f); glVertex3f(m_param.m_bottomScaleRect.right-x, m_param.m_bottomScaleRect.bottom-y, 100.0f);
//     glTexCoord2f(0.0f, 1.0f); glVertex3f(m_param.m_bottomScaleRect.left+x,  m_param.m_bottomScaleRect.bottom-y, 100.0f);
//     glEnd();
//
//     glBindTexture(GL_TEXTURE_2D, ArrowTextures[3]);
//     glBegin(GL_QUADS);
//     glNormal3f( 0.0f, 0.0f, 1.0f);
//     glTexCoord2f(0.0f, 0.0f); glVertex3f(m_param.m_bottomShiftRect.left+x, m_param.m_bottomShiftRect.top+y, 100.0f);
//     glTexCoord2f(1.0f, 0.0f); glVertex3f(m_param.m_bottomShiftRect.right-x, m_param.m_bottomShiftRect.top+y, 100.0f);
//     glTexCoord2f(1.0f, 1.0f); glVertex3f(m_param.m_bottomShiftRect.right-x, m_param.m_bottomShiftRect.bottom-y, 100.0f);
//     glTexCoord2f(0.0f, 1.0f); glVertex3f(m_param.m_bottomShiftRect.left+x,  m_param.m_bottomShiftRect.bottom-y, 100.0f);
//     glEnd();
//
//     x = (m_param.m_leftShiftRect.Width() - imageSize[0].PicSize[0])/2;
//     y = (m_param.m_leftShiftRect.Height() - imageSize[0].PicSize[1])/2;
//
//     if(x < 0)
//         x=0;
//     if(y < 0)
//         y=0;
//
//     glBindTexture(GL_TEXTURE_2D, ArrowTextures[4]);
//     glBegin(GL_QUADS);
//     glNormal3f( 0.0f, 0.0f, 1.0f);
//     glTexCoord2f(0.0f, 0.0f); glVertex3f(m_param.m_leftShiftRect.left+x, m_param.m_leftShiftRect.top+y, 100.0f);
//     glTexCoord2f(1.0f, 0.0f); glVertex3f(m_param.m_leftShiftRect.right-x, m_param.m_leftShiftRect.top+y, 100.0f);
//     glTexCoord2f(1.0f, 1.0f); glVertex3f(m_param.m_leftShiftRect.right-x, m_param.m_leftShiftRect.bottom-y, 100.0f);
//     glTexCoord2f(0.0f, 1.0f); glVertex3f(m_param.m_leftShiftRect.left+x,  m_param.m_leftShiftRect.bottom-y, 100.0f);
//     glEnd();
//
//     glBindTexture(GL_TEXTURE_2D, ArrowTextures[5]);
//     glBegin(GL_QUADS);
//        glNormal3f( 0.0f, 0.0f, 1.0f);
//        glTexCoord2f(0.0f, 0.0f); glVertex3f(m_param.m_leftScaleRect.left+x, m_param.m_leftScaleRect.top+y, 100.0f);
//        glTexCoord2f(1.0f, 0.0f); glVertex3f(m_param.m_leftScaleRect.right-x, m_param.m_leftScaleRect.top+y, 100.0f);
//        glTexCoord2f(1.0f, 1.0f); glVertex3f(m_param.m_leftScaleRect.right-x, m_param.m_leftScaleRect.bottom-y, 100.0f);
//        glTexCoord2f(0.0f, 1.0f); glVertex3f(m_param.m_leftScaleRect.left+x,  m_param.m_leftScaleRect.bottom-y, 100.0f);
//     glEnd();
//
//     glBindTexture(GL_TEXTURE_2D, ArrowTextures[6]);
//     glBegin(GL_QUADS);
//     glNormal3f( 0.0f, 0.0f, 1.0f);
//     glTexCoord2f(0.0f, 0.0f); glVertex3f(m_param.m_rightScaleRect.left+x, m_param.m_rightScaleRect.top+y, 100.0f);
//     glTexCoord2f(1.0f, 0.0f); glVertex3f(m_param.m_rightScaleRect.right-x, m_param.m_rightScaleRect.top+y, 100.0f);
//     glTexCoord2f(1.0f, 1.0f); glVertex3f(m_param.m_rightScaleRect.right-x, m_param.m_rightScaleRect.bottom-y, 100.0f);
//     glTexCoord2f(0.0f, 1.0f); glVertex3f(m_param.m_rightScaleRect.left+x,  m_param.m_rightScaleRect.bottom-y, 100.0f);
//     glEnd();
//
//     glBindTexture(GL_TEXTURE_2D, ArrowTextures[7]);
//     glBegin(GL_QUADS);
//     glNormal3f( 0.0f, 0.0f, 1.0f);
//     glTexCoord2f(0.0f, 0.0f); glVertex3f(m_param.m_rightShiftRect.left+x, m_param.m_rightShiftRect.top+y, 100.0f);
//     glTexCoord2f(1.0f, 0.0f); glVertex3f(m_param.m_rightShiftRect.right-x, m_param.m_rightShiftRect.top+y, 100.0f);
//     glTexCoord2f(1.0f, 1.0f); glVertex3f(m_param.m_rightShiftRect.right-x, m_param.m_rightShiftRect.bottom-y, 100.0f);
//     glTexCoord2f(0.0f, 1.0f); glVertex3f(m_param.m_rightShiftRect.left+x,  m_param.m_rightShiftRect.bottom-y, 100.0f);
//     glEnd();
//
//	 //glBlendEquation(GL_FUNC_ADD);
//
//     glDisable(GL_BLEND);
//     glEnable(GL_DEPTH_TEST);
//     glDisable(GL_TEXTURE_2D);
//     glEnable(GL_LIGHTING);
//     glPopClientAttrib();
//     glPopAttrib();
//     glPopMatrix();
 }
 //------------------------------------------------------------------------------
 void CDGraph::DrawLegend()
 {
	 CString legendX(L""), legendY(L"");
	 CRect rectX(0, 0, 0, 0), rectY(0, 0, 0, 0);
     glPushMatrix();
     glLoadIdentity();
     GraphMutex = true;
     for(unsigned int Counter = 0; Counter < m_param.m_bubblesBorder.size(); ++Counter)
     {
         if(m_param.m_bubblesBorder[Counter].Width() == 0
             || m_param.m_bubblesBorder[Counter].Height() == 0
             || m_param.m_textLegendX.empty())
             continue;
         FillRoundRectSolid(&m_param.m_bubblesField[Counter], 15.0, 5, RGB(255, 255, 255));
         FillRoundRectSolid(&m_param.m_bubblesColor[Counter], 15.0, 5, RGB(1, 203, 247));
         FillRoundRectSolid(&m_param.m_bubblesBorder[Counter], 15.0, 5, RGB(0, 0, 0));
         legendX = m_param.m_textLegendX[Counter];
         legendY = m_param.m_textLegendY[Counter];
         rectX = m_param.m_textFieldX[Counter];
         rectY = m_param.m_textFieldY[Counter];
         glDisable(GL_DEPTH_TEST);
         m_param.font.DrawString(rectX, legendX);
         m_param.font.DrawString(rectY, legendY);
         glEnable(GL_DEPTH_TEST);
     }
     GraphMutex = false;
     glPopMatrix();

 }
 //------------------------------------------------------------------------------
 void CDGraph::DrawBackGround()
 {
	 glBegin(GL_QUADS);
	 glColor3ub(GetRValue(m_param.m_backgroundColor),
		 GetGValue(m_param.m_backgroundColor),
		 GetBValue(m_param.m_backgroundColor));
	 glVertex3f(m_param.m_gridRect.left, m_param.m_gridRect.top, 0);
#ifdef _ACTIVEX2012
	 
#else
	 glColor3ub(GetRValue(m_param.m_backgroundColor) + 50,
		 GetGValue(m_param.m_backgroundColor) + 50,
		 GetBValue(m_param.m_backgroundColor) + 50);
#endif
	 glVertex3f(m_param.m_gridRect.left, m_param.m_gridRect.bottom, 0);
	 glVertex3f(m_param.m_gridRect.right, m_param.m_gridRect.bottom, 0);
	 glColor3ub(GetRValue(m_param.m_backgroundColor),
		 GetGValue(m_param.m_backgroundColor),
		 GetBValue(m_param.m_backgroundColor));
	 glVertex3f(m_param.m_gridRect.right, m_param.m_gridRect.top, 0);
	 glEnd();
 }
 //------------------------------------------------------------------------------
 void CDGraph::SetTexture(CString path)
 {
     if ( !path.IsEmpty() && (m_param.m_textureName != path) )
     {
         m_param.m_textureName = path;
#ifdef _GDIPLUS
         LoadImageFromFile(&m_pImage, str);
#else
         Texture = LoadGLTextures(m_param.m_textureName, PicSize, 0);

         for(int i = 0; i < 8; i++)
             m_pBorders[i]->CalcTexCoors(PicSize[0], PicSize[1]);
#endif	
         IsChange();	
     }
 }
 //------------------------------------------------------------------------------
 void CDGraph::SetArrowsTextures(CString path)
 {

     if ( !path.IsEmpty() && (m_param.m_arrowTextureName != path) )
     {
         m_param.m_arrowTextureName = path;
#ifdef _GDIPLUS
         LoadImageFromFile(&m_pImage, str);
#else
         ArrowTexture = LoadGLTextures(m_param.m_arrowTextureName, PicSize1, 0);

         for(int i=0; i<13; i++)
             m_pButton[i]->CalcTexCoors(PicSize1[0], PicSize1[1], m_pButton[i]->GetRotate());
#endif	
         IsChange();
     }
 }
 //------------------------------------------------------------------------------
void CDGraph::SetNormalMap(CString path)
{
    if ( !path.IsEmpty() && (m_param.m_normalMapName != path) )
    {
        m_param.m_normalMapName = path;
#ifdef _GDIPLUS
        LoadImageFromFile(&m_pImage, str);
#else
        //NormalMap = LoadGLNormalMap(m_param.m_normalMapName, PicSize);
        //NormalMap = LoadGLTextures(m_param.m_normalMapName, NormalSize, 0);
        for(int i=0; i<13; i++)
            //m_pButton[i]->CalcMultiTexCoordHMME(NormalSize[0], NormalSize[1]);
#endif	
        IsChange();
    }
 }
//------------------------------------------------------------------------------
void CDGraph::SetBackgroundColor(COLORREF backgroundColor)
{
	m_param.m_backgroundColor = backgroundColor;
	IsChange();
}
//------------------------------------------------------------------------------
void CDGraph::BrighteningForArrows(bool lButtonState, int ActiveArrow)
{
    if(lButtonState){
        m_param.mat_ambient_b[0] = 0.25f;
        m_param.mat_ambient_b[1] = 0.25f;
        m_param.mat_ambient_b[2] = 0.25f;
        m_param.mat_emission_b[0] = 58.f/255.f;
        m_param.mat_emission_b[1] = 117.f/255.f;
        m_param.mat_emission_b[2] = 196.f/255.f;
		IsChange();
    }

    if(!lButtonState)
    {
        m_param.mat_ambient_b[0] = 0.25f;
        m_param.mat_ambient_b[1] = 0.25f;
        m_param.mat_ambient_b[2] = 0.25f;
        m_param.mat_emission_b[0] = 26.f/255.f;
        m_param.mat_emission_b[1] = 71.f/255.f;
        m_param.mat_emission_b[2] = 128.f/255.f;
		IsChange();
    }

    m_param.ActiveArrow = ActiveArrow;
	
}
//------------------------------------------------------------------------------
void CDGraph::CreateButtons()
{
	CString arrowTextureName[9];
    float Coeff = 0.6;
#ifdef _ACTIVEX_ZIS
    m_pButton[0] = new CDetail(long(43),
        m_param.m_leftShiftRect.CenterPoint().x,
        m_param.m_leftShiftRect.CenterPoint().y, 0, 90,
        m_param.m_leftShiftRect.Height(), Coeff*m_param.m_leftShiftRect.Height(),
        100);

    m_pButton[1] = new CDetail(long(43),
        m_param.m_leftScaleRect.CenterPoint().x - m_param.m_leftScaleRect.Height()/2.0 - 1,
        m_param.m_leftScaleRect.CenterPoint().y, 0, 270,
        m_param.m_leftScaleRect.Height(), Coeff*m_param.m_leftScaleRect.Height(),
        0.3);

    m_pButton[2] = new CDetail(long(43),
        m_param.m_leftScaleRect.CenterPoint().x + m_param.m_leftScaleRect.Height()/2.0 + 1,
        m_param.m_leftScaleRect.CenterPoint().y, 0, 90,
        m_param.m_leftScaleRect.Height(), Coeff*m_param.m_leftScaleRect.Height(),
        0.3);

    m_pButton[3] = new CDetail(long(43),
        m_param.m_rightScaleRect.CenterPoint().x - m_param.m_rightScaleRect.Height()/2.0 - 1,
        m_param.m_rightScaleRect.CenterPoint().y, 0, 90,
        m_param.m_rightScaleRect.Height(), Coeff*m_param.m_rightScaleRect.Height(),
        0.3);

    m_pButton[4] = new CDetail(long(43),
        m_param.m_rightScaleRect.CenterPoint().x + m_param.m_rightScaleRect.Height()/2.0 + 1,
        m_param.m_rightScaleRect.CenterPoint().y, 0, 270,
        m_param.m_rightScaleRect.Height(), Coeff*m_param.m_rightScaleRect.Height(),
        0.3);

    m_pButton[5] = new CDetail(long(43),
        m_param.m_rightShiftRect.CenterPoint().x,
        m_param.m_rightShiftRect.CenterPoint().y, 0, 270,
        m_param.m_rightShiftRect.Height(), Coeff*m_param.m_rightShiftRect.Height(),
        0.3);

    m_pButton[6] = new CDetail(long(43),
        m_param.m_topShiftRect.CenterPoint().x,
        m_param.m_topShiftRect.CenterPoint().y, 0, 180,
        Coeff*m_param.m_topShiftRect.Width(), m_param.m_topShiftRect.Width(),
        0.3);

    m_pButton[7] = new CDetail(long(43),
        m_param.m_topScaleRect.CenterPoint().x,
        m_param.m_topScaleRect.CenterPoint().y - m_param.m_topScaleRect.Width()/2.0 - 1, 0, 180,
        Coeff*m_param.m_topScaleRect.Width(), m_param.m_topScaleRect.Width(),
        0.3);

    m_pButton[8] = new CDetail(long(43),
        m_param.m_topScaleRect.CenterPoint().x,
        m_param.m_topScaleRect.CenterPoint().y + m_param.m_topScaleRect.Width()/2.0 + 1, 0, 0,
        Coeff*m_param.m_topScaleRect.Width(), m_param.m_topScaleRect.Width(),
        0.3);
	auto trr = m_param.m_bottomScaleRect.CenterPoint();
    m_pButton[9] = new CDetail(long(43),
		trr.x,
		trr.y - m_param.m_bottomScaleRect.Width()/2.0 - 1, 0, 0,
        Coeff*m_param.m_bottomScaleRect.Width(), m_param.m_bottomScaleRect.Width(),
        0.3);

    m_pButton[10] = new CDetail(long(43),
		trr.x,
		trr.y + m_param.m_bottomScaleRect.Width()/2.0 + 1, 0, 180,
        Coeff*m_param.m_bottomScaleRect.Width(), m_param.m_bottomScaleRect.Width(),
        0.3);

    m_pButton[11] = new CDetail(long(43),
        m_param.m_bottomShiftRect.CenterPoint().x,
        m_param.m_bottomShiftRect.CenterPoint().y, 0, 0,
        Coeff*m_param.m_bottomShiftRect.Width(), m_param.m_bottomShiftRect.Width(),
        0.3);

    m_pButton[12] = new CDetail(long(44),
        m_param.m_centerAutoScaleRect.CenterPoint().x,
        m_param.m_centerAutoScaleRect.CenterPoint().y, 0, 90,
        Coeff*m_param.m_centerAutoScaleRect.Width(),
        Coeff*m_param.m_centerAutoScaleRect.Height(), 0.3);

	m_pDecorative = new CDetail(long(12),
		m_param.m_decorativeRect.CenterPoint().x, (m_param.m_decorativeRect.bottom + m_param.m_decorativeRect.top) / 2.0, 0,
		0, m_param.m_decorativeRect.Width(), m_param.m_decorativeRect.Height(), 0.1);
#else
    m_pButton[0] = new CDetail("$(ZetToolsDir)Interface2012\\Models\\стрелка.stl",
        m_param.m_leftShiftRect.CenterPoint().x,
        m_param.m_leftShiftRect.CenterPoint().y, 0, 90,
        m_param.m_leftShiftRect.Height(), Coeff*m_param.m_leftShiftRect.Height(),
        100);
	auto tl = m_param.m_leftScaleRect;
    m_pButton[1] = new CDetail("$(ZetToolsDir)Interface2012\\Models\\стрелка.stl",
		tl.CenterPoint().x - tl.Height() / 2.0 - 1,
		tl.CenterPoint().y, 0, 270,
		tl.Height(), Coeff*tl.Height(),
        0.3);

    m_pButton[2] = new CDetail("$(ZetToolsDir)Interface2012\\Models\\стрелка.stl",
		tl.CenterPoint().x + tl.Height() / 2.0 + 1,
		tl.CenterPoint().y, 0, 90,
		tl.Height(), Coeff*tl.Height(),
        0.3);
	auto rr = m_param.m_rightScaleRect;
    m_pButton[3] = new CDetail("$(ZetToolsDir)Interface2012\\Models\\стрелка.stl",
		rr.CenterPoint().x - rr.Height() / 2.0 - 1,
		rr.CenterPoint().y, 0, 90,
		rr.Height(), Coeff*rr.Height(),
        0.3);

    m_pButton[4] = new CDetail("$(ZetToolsDir)Interface2012\\Models\\стрелка.stl",
		rr.CenterPoint().x + rr.Height() / 2.0 + 1,
		rr.CenterPoint().y, 0, 270,
		rr.Height(), Coeff*rr.Height(),
        0.3);

    m_pButton[5] = new CDetail("$(ZetToolsDir)Interface2012\\Models\\стрелка.stl",
        m_param.m_rightShiftRect.CenterPoint().x,
        m_param.m_rightShiftRect.CenterPoint().y, 0, 270,
        m_param.m_rightShiftRect.Height(), Coeff*m_param.m_rightShiftRect.Height(),
        0.3);

    m_pButton[6] = new CDetail("$(ZetToolsDir)Interface2012\\Models\\стрелка.stl",
        m_param.m_topShiftRect.CenterPoint().x,
        m_param.m_topShiftRect.CenterPoint().y, 0, 180,
        Coeff*m_param.m_topShiftRect.Width(), m_param.m_topShiftRect.Width(),
        0.3);
	auto tr = m_param.m_topScaleRect.CenterPoint();
    m_pButton[7] = new CDetail("$(ZetToolsDir)Interface2012\\Models\\стрелка.stl",
		tr.x,
		tr.y - m_param.m_topScaleRect.Width() / 2.0 - 1, 0, 180,
        Coeff*m_param.m_topScaleRect.Width(), m_param.m_topScaleRect.Width(),
        0.3);

    m_pButton[8] = new CDetail("$(ZetToolsDir)Interface2012\\Models\\стрелка.stl",
		tr.x,
		tr.y + m_param.m_topScaleRect.Width() / 2.0 + 1, 0, 0,
        Coeff*m_param.m_topScaleRect.Width(), m_param.m_topScaleRect.Width(),
        0.3);
        auto trr = m_param.m_bottomScaleRect.CenterPoint();
    m_pButton[9] = new CDetail("$(ZetToolsDir)Interface2012\\Models\\стрелка.stl",
		trr.x,
		trr.y - m_param.m_bottomScaleRect.Width() / 2.0 - 1, 0, 0,
        Coeff*m_param.m_bottomScaleRect.Width(), m_param.m_bottomScaleRect.Width(),
        0.3);

    m_pButton[10] = new CDetail("$(ZetToolsDir)Interface2012\\Models\\стрелка.stl",
		trr.x,
		trr.y + m_param.m_bottomScaleRect.Width() / 2.0 + 1, 0, 180,
        Coeff*m_param.m_bottomScaleRect.Width(), m_param.m_bottomScaleRect.Width(),
        0.3);

    m_pButton[11] = new CDetail("$(ZetToolsDir)Interface2012\\Models\\стрелка.stl",
        m_param.m_bottomShiftRect.CenterPoint().x,
        m_param.m_bottomShiftRect.CenterPoint().y, 0, 0,
        Coeff*m_param.m_bottomShiftRect.Width(), m_param.m_bottomShiftRect.Width(),
        0.3);

    m_pButton[12] = new CDetail("$(ZetToolsDir)Interface2012\\Models\\углова€ стрелка.stl",
            m_param.m_centerAutoScaleRect.CenterPoint().x,
            m_param.m_centerAutoScaleRect.CenterPoint().y, 0, 90,
			Coeff*m_param.m_centerAutoScaleRect.Width()/1.5,
			Coeff*m_param.m_centerAutoScaleRect.Height()/1.5, 0.3);

	m_pDecorative = new CDetail("$(ZetToolsDir)Interface2012\\Models\\ нопка квадратна€ 22,5х22,5х7.stl",
		m_param.m_decorativeRect.CenterPoint().x, (m_param.m_decorativeRect.bottom
		+ m_param.m_decorativeRect.top)/2.0, 0, 0, m_param.m_decorativeRect.Width(),
		m_param.m_decorativeRect.Height(), 0.1);
#endif
#ifdef _ACTIVEX2012
    SetArrowsTextures(L"$(ZetToolsDir)Interface2012\\Textures\\Patterns\\SCADA.bmp");
#else
	SetArrowsTextures(L"$(ZetToolsDir)Interface2012\\Textures\\Patterns\\metal_mart14.bmp");
#endif
}
//------------------------------------------------------------------------------
void CDGraph::CreateBorders()
{
#ifdef _ACTIVEX_ZIS
    //лева€ часть
    m_pBorders[0] = new CDetail(long(0), (m_param.m_gridRect.left*2-6)/2.0,
        (m_param.m_gridRect.bottom+m_param.m_gridRect.top)/2.0, 0, 0, 6, m_param.m_gridRect.Height(), 90);
    //левый верхний угол
    m_pBorders[1] = new CDetail(long(1), (m_param.m_gridRect.left*2-6)/2.0,
        (m_param.m_gridRect.top*2-6)/2.0, 0, 0, 6, 6, 90);
    //верхн€€ часть
    m_pBorders[2] = new CDetail(long(2), (m_param.m_gridRect.left+m_param.m_gridRect.right)/2.0,
        (m_param.m_gridRect.top*2-6)/2.0, 0, 0, m_param.m_gridRect.Width(), 6, 90);
    //правый верхний угол
    m_pBorders[3] = new CDetail(long(3), (m_param.m_gridRect.right*2+6)/2.0,
        (m_param.m_gridRect.top*2-6)/2.0, 0, 0, 6, 6, 90);
    //права€ часть
    m_pBorders[4] = new CDetail(long(4), (m_param.m_gridRect.right*2+6)/2.0,
        (m_param.m_gridRect.bottom+m_param.m_gridRect.top)/2.0, 0, 0, 6, m_param.m_gridRect.Height(), 90);
    //правый нижний угол
    m_pBorders[5] = new CDetail(long(5), (m_param.m_gridRect.right*2+6)/2.0,
        (m_param.m_gridRect.bottom*2+6)/2.0, 0, 0, 6, 6, 90);
    //нижн€€ часть
    m_pBorders[6] = new CDetail(long(6), (m_param.m_gridRect.left+m_param.m_gridRect.right)/2.0,
        (m_param.m_gridRect.bottom*2+6)/2.0, 0, 0, m_param.m_gridRect.Width(), 6, 90);
    //левый нижний угол
    m_pBorders[7] = new CDetail(long(7), (m_param.m_gridRect.left*2-6)/2.0,
        (m_param.m_gridRect.bottom*2+6)/2.0, 0, 0, 6, 6, 90);//*/


#else
    //верхн€€ часть
    m_pBorders[0] = new CDetail("$(ZetToolsDir)Interface2012\\Models\\ComboBox\\низ_комб.stl",
        (m_param.m_gridRect.left+m_param.m_gridRect.right)/2.0,
        (m_param.m_gridRect.top*2-6)/2.0, 0, 0, 
        m_param.m_gridRect.Width(), 6, 10);
    //нижн€€ часть
    m_pBorders[1] = new CDetail("$(ZetToolsDir)Interface2012\\Models\\ComboBox\\верх_комб.stl",
        (m_param.m_gridRect.left+m_param.m_gridRect.right)/2.0,
        (m_param.m_gridRect.bottom*2+6)/2.0, 0, 0, 
        m_param.m_gridRect.Width(), 6, 10);
    //лева€ часть
    m_pBorders[2] = new CDetail("$(ZetToolsDir)Interface2012\\Models\\ComboBox\\лев_комб.stl",
        (m_param.m_gridRect.left*2-6)/2.0,
        (m_param.m_gridRect.bottom+m_param.m_gridRect.top)/2.0, 0, 0, 
        6, m_param.m_gridRect.Height(), 10);
    //права€ часть
    m_pBorders[3] = new CDetail("$(ZetToolsDir)Interface2012\\Models\\ComboBox\\правый_комб.stl",
        (m_param.m_gridRect.right*2+6)/2.0,
        (m_param.m_gridRect.bottom+m_param.m_gridRect.top)/2.0, 0, 0, 
        6, m_param.m_gridRect.Height(), 10);
    //левый верхний угол
    m_pBorders[4] = new CDetail("$(ZetToolsDir)Interface2012\\Models\\ComboBox\\лев ниж_комб.stl",
        (m_param.m_gridRect.left*2-6)/2.0,
        (m_param.m_gridRect.top*2-6)/2.0, 0, 0, 
        6, 6, 10);
    //левый нижний угол
    m_pBorders[5] = new CDetail("$(ZetToolsDir)Interface2012\\Models\\ComboBox\\лев верх_комб.stl",
        (m_param.m_gridRect.left*2-6)/2.0,
        (m_param.m_gridRect.bottom*2+6)/2.0, 0, 0, 
        6, 6, 10);
    //правый верхний угол
    m_pBorders[6] = new CDetail("$(ZetToolsDir)Interface2012\\Models\\ComboBox\\прав ниж_комб.stl",
        (m_param.m_gridRect.right*2+6)/2.0,
        (m_param.m_gridRect.top*2-6)/2.0, 0, 0, 
        6, 6, 10);
    //правый нижний угол
    m_pBorders[7] = new CDetail("$(ZetToolsDir)Interface2012\\Models\\ComboBox\\прав верх_комб.stl",
        (m_param.m_gridRect.right*2+6)/2.0,
        (m_param.m_gridRect.bottom*2+6)/2.0, 0, 0, 
        6, 6, 10);//*/
#endif

#ifdef _ACTIVEX2012
	SetTexture(L"$(ZetToolsDir)Interface2012\\Textures\\Patterns\\SCADA.bmp");
#else
	SetTexture(L"$(ZetToolsDir)Interface2012\\Textures\\Patterns\\metal_mart14.bmp");
#endif
}
//------------------------------------------------------------------------------
void CDGraph::SetRectAll(CRect leftShiftRect, CRect rightShiftRect,
    CRect bottomShiftRect, CRect topShiftRect, CRect leftScaleRect,
    CRect rightScaleRect, CRect topScaleRect, CRect bottomScaleRect,
	CRect centerAutoScaleRect, CRect decorativeRect)
{
    m_param.m_leftShiftRect = leftShiftRect;
    m_param.m_rightShiftRect = rightShiftRect;
    m_param.m_topShiftRect = topShiftRect;
    m_param.m_bottomShiftRect = bottomShiftRect;
    m_param.m_leftScaleRect = leftScaleRect;
    m_param.m_rightScaleRect = rightScaleRect;
    m_param.m_topScaleRect = topScaleRect;
    m_param.m_bottomScaleRect = bottomScaleRect;
    m_param.m_centerAutoScaleRect = centerAutoScaleRect;
	m_param.m_decorativeRect = decorativeRect;
    CreateButtons();
    CreateBorders();
    IsChange();
}
//------------------------------------------------------------------------------
void CDGraph::Resize(CRect rect, CRect gridRect, CRect leftShiftRect, CRect rightShiftRect,
	CRect bottomShiftRect, CRect topShiftRect, CRect leftScaleRect,
	CRect rightScaleRect, CRect topScaleRect, CRect bottomScaleRect,
	CRect centerAutoScaleRect, CRect decorativeRect)
{
	float Coeff = 0.6;

	m_param.m_leftShiftRect = leftShiftRect;
	m_param.m_rightShiftRect = rightShiftRect;
	m_param.m_topShiftRect = topShiftRect;
	m_param.m_bottomShiftRect = bottomShiftRect;
	m_param.m_leftScaleRect = leftScaleRect;
	m_param.m_rightScaleRect = rightScaleRect;
	m_param.m_topScaleRect = topScaleRect;
	m_param.m_bottomScaleRect = bottomScaleRect;
	m_param.m_centerAutoScaleRect = centerAutoScaleRect;
	m_param.m_decorativeRect = decorativeRect;

	m_rect = rect;
	m_param.m_rect = rect;
	m_param.m_gridRect = gridRect;

	m_pButton[0]->Resize(m_param.m_leftShiftRect.CenterPoint().x,
		m_param.m_leftShiftRect.CenterPoint().y, 0, 90,
		m_param.m_leftShiftRect.Height(), Coeff*m_param.m_leftShiftRect.Height(),
		100);
	auto ul = m_param.m_leftScaleRect;
	m_pButton[1]->Resize(ul.CenterPoint().x - ul.Height() / 2.0 - 1,
		ul.CenterPoint().y, 0, 270,
		ul.Height(), Coeff*ul.Height(),
		0.3);

	m_pButton[2]->Resize(ul.CenterPoint().x + ul.Height() / 2.0 + 1,
		ul.CenterPoint().y, 0, 90,
		ul.Height(), Coeff*ul.Height(),
		0.3);
	auto uu = m_param.m_rightScaleRect;
	m_pButton[3]->Resize(uu.CenterPoint().x - uu.Height() / 2.0 - 1,
		uu.CenterPoint().y, 0, 90,
		uu.Height(), Coeff*uu.Height(),
		0.3);

	m_pButton[4]->Resize(uu.CenterPoint().x + uu.Height() / 2.0 + 1,
		uu.CenterPoint().y, 0, 270,
		uu.Height(), Coeff*uu.Height(),
		0.3);

	m_pButton[5]->Resize(m_param.m_rightShiftRect.CenterPoint().x,
		m_param.m_rightShiftRect.CenterPoint().y, 0, 270,
		m_param.m_rightShiftRect.Height(), Coeff*m_param.m_rightShiftRect.Height(),
		0.3);

	m_pButton[6]->Resize(m_param.m_topShiftRect.CenterPoint().x,
		m_param.m_topShiftRect.CenterPoint().y, 0, 180,
		Coeff*m_param.m_topShiftRect.Width(), m_param.m_topShiftRect.Width(),
		0.3);
	auto tt = m_param.m_topScaleRect;
	m_pButton[7]->Resize(tt.CenterPoint().x,
		tt.CenterPoint().y - tt.Width() / 2.0 - 1, 0, 180,
		Coeff*tt.Width(), tt.Width(),
		0.3);

	m_pButton[8]->Resize(tt.CenterPoint().x,
		tt.CenterPoint().y + tt.Width() / 2.0 + 1, 0, 0,
		Coeff*tt.Width(), tt.Width(),
		0.3);
	auto tb = m_param.m_bottomScaleRect;
	m_pButton[9]->Resize(tb.CenterPoint().x,
		tb.CenterPoint().y - tb.Width() / 2.0 - 1, 0, 0,
		Coeff*tb.Width(), tb.Width(),
		0.3);

	m_pButton[10]->Resize(tb.CenterPoint().x,
		tb.CenterPoint().y + tb.Width() / 2.0 + 1, 0, 180,
		Coeff*tb.Width(), tb.Width(),
		0.3);

	m_pButton[11]->Resize(m_param.m_bottomShiftRect.CenterPoint().x,
		m_param.m_bottomShiftRect.CenterPoint().y, 0, 0,
		Coeff*m_param.m_bottomShiftRect.Width(), m_param.m_bottomShiftRect.Width(),
		0.3);

	m_pButton[12]->Resize(m_param.m_centerAutoScaleRect.CenterPoint().x,
		m_param.m_centerAutoScaleRect.CenterPoint().y, 0, 90,
		Coeff*m_param.m_centerAutoScaleRect.Width() / 1.5,
		Coeff*m_param.m_centerAutoScaleRect.Height() / 1.5, 0.3);

	m_pDecorative->Resize(m_param.m_decorativeRect.CenterPoint().x, (m_param.m_decorativeRect.bottom
		+ m_param.m_decorativeRect.top) / 2.0, 0, 0, m_param.m_decorativeRect.Width(),
		m_param.m_decorativeRect.Height(), 0.1);

	m_pBorders[0]->Resize((m_param.m_gridRect.left + m_param.m_gridRect.right) / 2.0,
		(m_param.m_gridRect.top * 2 - 6) / 2.0, 0, 0,
		m_param.m_gridRect.Width(), 6, 10);
	//нижн€€ часть
	m_pBorders[1]->Resize((m_param.m_gridRect.left + m_param.m_gridRect.right) / 2.0,
		(m_param.m_gridRect.bottom * 2 + 6) / 2.0, 0, 0,
		m_param.m_gridRect.Width(), 6, 10);
	//лева€ часть
	m_pBorders[2]->Resize((m_param.m_gridRect.left * 2 - 6) / 2.0,
		(m_param.m_gridRect.bottom + m_param.m_gridRect.top) / 2.0, 0, 0,
		6, m_param.m_gridRect.Height(), 10);
	//права€ часть
	m_pBorders[3]->Resize((m_param.m_gridRect.right * 2 + 6) / 2.0,
		(m_param.m_gridRect.bottom + m_param.m_gridRect.top) / 2.0, 0, 0,
		6, m_param.m_gridRect.Height(), 10);
	//левый верхний угол
	m_pBorders[4]->Resize((m_param.m_gridRect.left * 2 - 6) / 2.0,
		(m_param.m_gridRect.top * 2 - 6) / 2.0, 0, 0,
		6, 6, 10);
	//левый нижний угол
	m_pBorders[5]->Resize((m_param.m_gridRect.left * 2 - 6) / 2.0,
		(m_param.m_gridRect.bottom * 2 + 6) / 2.0, 0, 0,
		6, 6, 10);
	//правый верхний угол
	m_pBorders[6]->Resize((m_param.m_gridRect.right * 2 + 6) / 2.0,
		(m_param.m_gridRect.top * 2 - 6) / 2.0, 0, 0,
		6, 6, 10);
	//правый нижний угол
	m_pBorders[7]->Resize((m_param.m_gridRect.right * 2 + 6) / 2.0,
		(m_param.m_gridRect.bottom * 2 + 6) / 2.0, 0, 0,
		6, 6, 10);
}
//------------------------------------------------------------------------------
void CDGraph::SetRectBubble(CRect bubbleRect, int cursorId)
{
    if(m_param.m_bubblesBorder.empty() || (m_param.m_bubblesBorder.size() < cursorId+1))
    {
        m_param.m_bubblesBorder.resize(cursorId+1);
        m_param.m_bubblesColor.resize(cursorId+1);
        m_param.m_bubblesField.resize(cursorId+1);
        m_param.m_textFieldX.resize(cursorId+1);
        m_param.m_textFieldY.resize(cursorId+1);
    }
    m_param.m_bubblesBorder[cursorId] = bubbleRect;
    m_param.m_bubblesColor[cursorId] = CRect(bubbleRect.left + 1,
        bubbleRect.top + 1, bubbleRect.right - 1, bubbleRect.bottom - 1);
    m_param.m_bubblesField[cursorId] = CRect(bubbleRect.left + 4,
        bubbleRect.top + 4, bubbleRect.right - 4, bubbleRect.bottom - 4);
	auto trs = m_param.m_bubblesField[cursorId];
    m_param.m_textFieldX[cursorId] = CRect(m_param.m_bubblesField[cursorId].left,
		trs.top,
		trs.right,
		trs.top
		+ trs.Height() / 2.0);
    m_param.m_textFieldY[cursorId] = CRect(m_param.m_bubblesField[cursorId].left,
		trs.top
		+ trs.Height() / 2.0,
		trs.right,
		trs.bottom);
    IsChange();
}
//------------------------------------------------------------------------------
CRect CDGraph::GetRectBubble(int cursorId)
{
    return m_param.m_bubblesBorder[cursorId]; //кос€к
}
//------------------------------------------------------------------------------
void CDGraph::SetTextBubble(CString textX, CString textY, int cursorId)
{
    if(GraphMutex)
        return;
    if(m_param.m_textLegendX.empty() || m_param.m_textLegendX.size() < cursorId+1)
	{
        m_param.m_textLegendX.resize(cursorId+1);
        m_param.m_textLegendY.resize(cursorId+1);
    }
    m_param.m_textLegendX[cursorId] = textX;
    m_param.m_textLegendY[cursorId] = textY;
    IsChange();
}
//------------------------------------------------------------------------------
void CDGraph::SaveParameters(SimpleXML *pSimpleXML)
{
    TRACE("WOOP-WOOP-WOOP!\n");
}
//------------------------------------------------------------------------------
void CDGraph::LoadParameters(SimpleXML *pSimpleXML)
{

}
//------------------------------------------------------------------------------
void CDGraph::Clear (void)
{
	for (int i = 0; i < 13; i++)
		m_pButton[i]->Clear();
	for (int j = 0; j < 8; j++)
		m_pBorders[j]->Clear();
}
//------------------------------------------------------------------------------