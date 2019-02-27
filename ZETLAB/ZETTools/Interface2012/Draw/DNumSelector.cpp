#include "stdafx.h"
#include "C:\ZETTools\Interface2012\Draw\DNumSelector.h"
#include <math.h>

//------------------------------------------------------------------------------
int g_nNSelector = 0;		// счётчик статических картинок
//------------------------------------------------------------------------------
#ifdef _GDIPLUS
CDNumSelector::CDNumSelector(CRect rect, PVOID ppGraphics,
	bool *pFlagIsChange, double *pTimeAnimation) :
	CCustomDraw(rect, ppGraphics, pFlagIsChange, pTimeAnimation)
#else
CDNumSelector::CDNumSelector(CRect rect, bool *pFlagIsChange, 
	double *pTimeAnimation) :
	CCustomDraw(rect, pFlagIsChange, pTimeAnimation)
#endif
{
	m_pParamDraw = &m_param;	// обеспечение ОБЩИХ параметров
	m_param.nElement = ++g_nNSelector;
	SetNameWithNumber(L"NumSelector");
	m_param.bMouseChange = true;
	// переменные класса
	m_bUp = m_bDown = false;
	m_bEvent = m_bFlag = false;
	for (int i = 0; i<10; i++)
	{
		m_Angle_Prev[i] = m_Angle_Cur[i] = m_delta_Angle[i] = 0;
		m_delta_value[i] = 0;
	}
#ifdef _GDIPLUS
#else
	BackColorCur = m_param.BackColorLeave;
	m_param.pFont = new CDFonts("Calibri", 40.0 , FontsStyleItalic, RGB(20,10,110));
	//передаются координаты центра области
	m_SNumSelector = new CDetail("C:\\ZETTools\\Interface2012\\Models\\Барабан с цифрами new.stl",
		(rect.right + rect.left)/2, (rect.bottom + rect.top)/2, 0, 0, 
		rect.Width(), rect.Height(), 5);
	Texture = LoadGLTextures(_T("C:\\ZETTools\\Interface2012\\Textures\\texture6.bmp"), PicSize, 0);
	Init();
#endif
}
//------------------------------------------------------------------------------
void CDNumSelector::Clear(void)
{
	m_param.pFont->Clear();
	m_SNumSelector->Clear();
}
//------------------------------------------------------------------------------
CDNumSelector::~CDNumSelector()
{
	delete m_param.pFont;
	delete m_SNumSelector;
}
//------------------------------------------------------------------------------
void CDNumSelector::SetBolderWidth(int Width)
{
	if (BolderWidth != Width)
		BolderWidth = Width;
}
//------------------------------------------------------------------------------
void CDNumSelector::Init()
{
	SetBolderWidth(4);
	//rect_i[0] = m_rect;
	int width_dot = 0;
	width = int((m_rect.Width())/(Num[0] + Num[1] + 1.5));	
	rect_minus.top = rect_dot.top= m_rect.top + BolderWidth;
	rect_minus.bottom = rect_dot.bottom = m_rect.bottom - BolderWidth;
	rect_minus.left = m_rect.left + BolderWidth;
	rect_minus.right = m_rect.left + width;

	for (int i = 0; i < (Num[0] + Num[1]); i++)
	{
		rect_i[i].top =  m_rect.top + BolderWidth;
		rect_i[i].bottom  = m_rect.bottom - BolderWidth;
		if (i == Num[0])
		{
			width_dot = 0.5 * width; 
			rect_dot.left = rect_i[i-1].right + BolderWidth;
			rect_dot.right = rect_i[i-1].right + width_dot;
		}
		rect_i[i].left = m_rect.left + width_dot + BolderWidth + width*(i+1);
		rect_i[i].right = m_rect.left + width_dot + width*(i+2);
	}
}
//------------------------------------------------------------------------------
void CDNumSelector::SetAngles(double val)
{
	val = abs(val);
	int Value = 0;
	for (int i = 0; i < Num[0] + Num[1]; i++)
	{
		val = val - Value*pow(double(10), Num[0]+Num[1] - i);
		Value = int(val/pow(double(10), Num[0]+Num[1] - i - 1));
		m_Angle_Cur[i] = - Value*36;;
	}
	/*for (int j = 0; j < Num[1]; j++)
	{
		val = val - Value*pow(double(0.1), j);
			Value = int(val*pow(double(10), j + 1));
		m_Angle_Cur[j + Num[0]] = - Value*36;
	}*/
	IsChange();
}
//------------------------------------------------------------------------------
void CDNumSelector::SetCurValue(double val)
{
	if (m_param.CurValue != val)
	{
		m_param.CurValue = val;
		if (m_param.CurValue > m_param.max_limit)
			m_param.CurValue = m_param.max_limit;
		if (m_param.CurValue < m_param.min_limit)
			m_param.CurValue = m_param.min_limit;
	}
	IsChange();
}
//------------------------------------------------------------------------------
void CDNumSelector::SetMaxLimit(double val)
{
	val = val*pow(double(10), Num[0] + Num[1]);
	if ((m_param.max_limit != val)&&(val < (pow(double(10), Num[0] + Num[1]) - 1)))
		m_param.max_limit = val;
	IsChange();
}
//------------------------------------------------------------------------------
void CDNumSelector::SetMinLimit(double val)
{
	val = val*pow(double(10), Num[0] + Num[1]);
	if ((m_param.min_limit != val)&&(val > (-pow(double(10), Num[0] + Num[1]) + 1)))
		m_param.min_limit = val;
	IsChange();
}
//------------------------------------------------------------------------------
void CDNumSelector::SetTimeAnimation(double val)
{
	if (val > 0.)
		m_param.dTimeAnimation = val;
}
//------------------------------------------------------------------------------
void CDNumSelector::DoProcess()
{
	PrevValue = m_param.CurValue;
	if (m_param.bAnimation)
	{
		for (int i = 0; i < Num[0]+Num[1]; i++)
		{

			m_param.CurValue = m_param.CurValue + pow(double(10), Num[0] + Num[1] - i - 1)*m_delta_value[i];
			m_delta_value[i] = 0;
		}
		if (m_param.CurValue > m_param.max_limit)
			m_param.CurValue = m_param.max_limit;
		if (m_param.CurValue < m_param.min_limit)
			m_param.CurValue = m_param.min_limit;
		SetAngles(m_param.CurValue);
		for (int i = 0; i < Num[0] + Num[1]; i++)
			m_Angle_Prev[i] = m_Angle_Cur[i];
		m_param.dTimeCur = GetTime() - dTimeStart;
		if (m_param.dTimeCur <= m_param.dTimeAnimation)
		{
			if ((m_param.CurValue != m_param.max_limit)&&(m_param.CurValue != m_param.min_limit))	
				//if ((m_param.CurValue > 0) || (m_param.CurValue == 0)&&(PrevValue >= 0))
					for (int i = 0; i < (Num[0] + Num[1]); i++)
						m_Angle_Cur[i] = m_Angle_Prev[i] - m_param.dTimeCur*m_delta_Angle[i]/m_param.dTimeAnimation + m_delta_Angle[i];
				//if ((m_param.CurValue < 0) || (m_param.CurValue == 0)&&(PrevValue < 0))
					/*for (int i = 0; i < (Num[0] + Num[1]); i++)
						m_Angle_Cur[i] = m_Angle_Prev[i] + m_param.dTimeCur*m_delta_Angle[i]/m_param.dTimeAnimation - m_delta_Angle[i];*/
		}
		else
		{
			SetAngles(m_param.CurValue);
			m_param.bAnimation = false;
			m_bEvent = false;
			for (int i = 0; i < Num[0] + Num[1]; i++)
				m_delta_Angle[i] = 0;
		}
		IsChange();
	}
}
//------------------------------------------------------------------------------
void CDNumSelector::SetSign(bool val)
{
	if (flag != val)	
		flag = val;
	IsChange();
}
//------------------------------------------------------------------------------
void CDNumSelector::OnMouseEnter()
{
	m_bFlag = true;
	CCustomDraw::OnMouseEnter();
}
//------------------------------------------------------------------------------
void CDNumSelector::SetMask(int integer, int fraction)
{
	if (Num[0] != integer)
		Num[0] = integer;
	if (Num[0] < 1)
		Num[0] = 1;
	if (Num[1] != fraction)
		Num[1] = fraction;
	m_param.max_limit = pow(double(10), Num[0] + Num[1]) - 1;
	m_param.min_limit = - pow(double(10), Num[0] + Num[1]) + 1;
	Init();
}
//------------------------------------------------------------------------------
void CDNumSelector::OnMouseLeave()
{
	if (m_bFlag)
		m_bFlag = false;
	rect_tr = 0;
	//if (m_bEvent)
	//m_bEvent = false;
	//if (m_bUp)
	//m_bUp = false;
	//if (m_bDown)
	//m_bDown = false;
	CCustomDraw::OnMouseLeave();
}
//------------------------------------------------------------------------------
void CDNumSelector::OnLButtonClk(UINT nFlags, CPoint point)
{
	m_bEvent = true;
	if (m_bEvent)
	{
		for (int i = 0; i < (Num[0]+Num[1]); i++)
		{
			if (PointInRect(point, &CRect(rect_i[i].left, rect_i[i].top, rect_i[i].right, rect_i[i].top + rect_i[i].Height()/4)))
			{
				m_param.bAnimation = true;
				m_param.dTimeCur = 0.;
				dTimeStart = GetTime();
				m_bUp = true;
				m_bDown = false;
				if (m_param.CurValue >= 0)
					m_delta_Angle[i] = 36;
				else
					m_delta_Angle[i] = -36;
				m_delta_value[i] = 1;	
			}
			if (PointInRect(point, &CRect(rect_i[i].left, rect_i[i].bottom -  rect_i[i].Height()/4, rect_i[i].right, rect_i[i].bottom)))
			{
				m_param.bAnimation = true;
				m_param.dTimeCur = 0.;
				dTimeStart = GetTime();
				m_bDown = true;
				m_bUp = false;
				if (m_param.CurValue > 0)
					m_delta_Angle[i] = -36;
				else
					m_delta_Angle[i] = 36;
				m_delta_value[i] = -1;
			}
		}
		IsChange();
	}
}
//------------------------------------------------------------------------------
void CDNumSelector::OnMouseWheel(UINT nFlags, CPoint point, short delta)
{
#ifdef _GDIPLUS
#else
	if (m_bEvent)
	{
		for (int i = 0; i < Num[0]+Num[1]; i++)
			if (PointInRect(point, &rect_i[i]))
			{
				if (i < Num[0])
					m_param.CurValue = m_param.CurValue + pow(double(10), Num[0] - i - 1)*(delta/120);
				else
					m_param.CurValue = m_param.CurValue + pow(double(0.1), (i-Num[0]) + 1)*(delta/120);
				if (m_param.CurValue > m_param.max_limit)
					m_param.CurValue = m_param.max_limit;
				if (m_param.CurValue < m_param.min_limit)
					m_param.CurValue = m_param.min_limit;
				SetAngles(m_param.CurValue);
			}
		IsChange();
	}	
#endif
}
void CDNumSelector::OnMouseMove(UINT nFlags, CPoint point)
{
#ifdef _GDIPLUS
#else
	for (int i = 0; i < Num[0] + Num[1]; i++)
		if (PointInRect(point, &rect_i[i]))
		{
			rect_tr = rect_i[i];
		}
#endif
}
//------------------------------------------------------------------------------
#ifdef _GDIPLUS
void CDNSelector::OnDraw()
{// отображение
	if (m_param.bVisible)
	{
	}
}
#else
//------------------------------------------------------------------------------
void CDNumSelector::OnDraw()
{// отображение
	if (m_param.bVisible)
	{
		glPushAttrib(GL_ENABLE_BIT | GL_LIGHTING_BIT | GL_TEXTURE_BIT);

		glMaterialfv(GL_FRONT, GL_AMBIENT, m_param.mat_ambient);
		glMaterialfv(GL_FRONT, GL_DIFFUSE, m_param.mat_diffuse);
		glMaterialfv(GL_FRONT, GL_EMISSION, m_param.mat_emission);
		glMaterialfv(GL_FRONT, GL_SPECULAR, m_param.mat_specular);
		glMaterialf(GL_FRONT, GL_SHININESS, m_param.mat_shininess);

		//glEnable(GL_BLEND);
		//glEnable(GL_TEXTURE_2D);
		//glBlendFunc(GL_SRC_ALPHA,GL_ONE);
		glBindTexture(GL_TEXTURE_2D, Texture);
		for (int i = 0; i < 10; i++)
		{
			m_SNumSelector->SetOffset((rect_i[i].right + rect_i[i].left)/2, (rect_i[i].bottom + rect_i[i].top)/2, -0.48);
			m_SNumSelector->SetScale(rect_i[i].Width(), rect_i[i].Height()*2, 1);
			m_SNumSelector->SetRotateX(m_Angle_Cur[i]);
			m_SNumSelector->Draw_texture(m_SNumSelector->ReadFacet(), m_SNumSelector->GetQntPlanes(), PicSize, true);
		}
			//glEnable(GL_COLOR_MATERIAL);
		if (m_param.CurValue < 0)
		{
			rect_minus.top = (rect_minus.top + rect_minus.bottom)/2 - rect_dot.Width()/8;
			rect_minus.bottom = rect_minus.top + rect_dot.Width()/4;
			glPushMatrix();
			glTranslatef((rect_minus.left + rect_minus.right)/2, (rect_minus.top + rect_minus.bottom)/2, 0.05); 
			glScalef(rect_minus.Width()/rect_minus.Height(), 1,1);
			glutSolidCube(rect_minus.Height());
			//FillRectSolid(&rect_minus, 0, BackColorCur);
			glPopMatrix();	
		}
		glPushMatrix();
		glTranslatef((rect_dot.left + rect_dot.right)/2, (rect_dot.top + rect_dot.bottom)/2 + rect_dot.Height()/8, 0); 
		glutSolidSphere(rect_dot.Width()/4, 10, 10);
		glPopMatrix();
		//glDisable(GL_TEXTURE_2D);
		//glDisable(GL_BLEND);
		//стрелочки
		if (m_bFlag)
		{
			glEnable(GL_COLOR_MATERIAL);
			glDisable(GL_DEPTH_TEST);
			if ((m_bUp)&&(m_param.bAnimation))
			{
				glBegin(GL_POLYGON);
				glColor3ub(GetRValue(m_param.BackColor), GetGValue(m_param.BackColor), GetBValue(m_param.BackColor));
				glVertex3f(rect_tr.left + 2, rect_tr.top + rect_tr.Height()/4 - 2, 0);
				glVertex3f((rect_tr.left + rect_tr.right)/2, rect_tr.top + 2, 0);
				glVertex3f(rect_tr.right - 2, rect_tr.top + rect_tr.Height()/4 - 2, 0);
				glEnd();
			}
			else
			{
				glBegin(GL_POLYGON);
				glColor3ub(GetRValue(m_param.BackColor), GetGValue(m_param.BackColor), GetBValue(m_param.BackColor));
				glVertex3f(rect_tr.left, rect_tr.top + rect_tr.Height()/4, 0);
				glVertex3f((rect_tr.left + rect_tr.right)/2, rect_tr.top, 0);
				glVertex3f(rect_tr.right, rect_tr.top + rect_tr.Height()/4, 0);
				glEnd();
			}		
			if ((m_bDown)&&(m_param.bAnimation))
			{	
				glBegin(GL_POLYGON);
				glColor3ub(GetRValue(m_param.BackColor), GetGValue(m_param.BackColor), GetBValue(m_param.BackColor));
				glVertex3f(rect_tr.left + 2, rect_tr.bottom - rect_tr.Height()/4 + 2, 0);
				glVertex3f((rect_tr.left + rect_tr.right)/2, rect_tr.bottom - 2, 0);
				glVertex3f(rect_tr.right - 2, rect_tr.bottom - rect_tr.Height()/4 + 2, 0);
				glEnd();
			}
			else
			{
				glBegin(GL_POLYGON);
				glColor3ub(GetRValue(m_param.BackColor), GetGValue(m_param.BackColor), GetBValue(m_param.BackColor));
				glVertex3f(rect_tr.left, rect_tr.bottom - rect_tr.Height()/4, 0);
				glVertex3f((rect_tr.left + rect_tr.right)/2, rect_tr.bottom, 0);
				glVertex3f(rect_tr.right, rect_tr.bottom - rect_tr.Height()/4, 0);
				glEnd();
			}
			glDisable(GL_COLOR_MATERIAL);
			glEnable(GL_DEPTH_TEST);	
		}

		glPopAttrib();
	}
}
//------------------------------------------------------------------------------
void CDNumSelector::SetColor(GLfloat *ma, GLfloat *md,	
	GLfloat *me, 
	GLfloat *msp, GLfloat msh)
{
	for (int i = 0; i < 4; i++)
	{

		m_param.mat_ambient[i] = ma[i];
		m_param.mat_diffuse[i] = md[i];
		m_param.mat_emission[i] = me[i];
		m_param.mat_specular[i] = msp[i];

	}
	m_param.mat_shininess = msh;
}
#endif