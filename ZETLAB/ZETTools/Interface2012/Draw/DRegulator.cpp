//------------------------------------------------------------------------------
// DRegulator.cpp : файл реализации
// Прямоугольник элемента в процессе работы не меняется, поэтому положение
// (горизонтальное или вертикальное) определяется один раз в конструкторе.
//------------------------------------------------------------------------------
#include "stdafx.h"
#include "C:\ZETTools\Interface2012\Draw\DRegulator.h"
#include <math.h>

//------------------------------------------------------------------------------
int g_nRegulator = 0;		// счётчик статических картинок
//------------------------------------------------------------------------------
#ifdef _GDIPLUS
CDRegulator::CDRegulator(CRect rect, PVOID ppGraphics,
	bool *pFlagIsChange, double *pTimeAnimation) :
	CCustomDraw(rect, ppGraphics, pFlagIsChange, pTimeAnimation)
#else
CDRegulator::CDRegulator(CRect rect,	bool *pFlagIsChange, 
	double *pTimeAnimation, bool variant, bool draw_scale, float start_angle) :
	CCustomDraw(rect, pFlagIsChange, pTimeAnimation)
#endif
{
	m_pParamDraw = &m_param;	// обеспечение ОБЩИХ параметров
	m_param.nElement = ++g_nRegulator;
	SetBorderWidth(rect.Width()/4);
	SetNameWithNumber(L"Regulator");
	//TRACE("CREATE CDRegulator %s\n", (CStringA)m_pParamDraw->Name);
	m_param.bMouseChange = true;
	// переменные класса
	m_bFlag = false;
	m_bEvent = m_bFlag = false;
	m_param.blocking = false;
	m_nStart = 0;
	m_param.variant = variant;
	m_param.draw_scale = draw_scale;
	m_param.quan_scale = 0;
	m_param.turn_coeff = 1;
#ifdef _GDIPLUS
#else
	m_Angle_Cur = start_angle;
	m_Angle_Prev = start_angle;
	m_param.start_angle = start_angle;
	m_Angle_Rot = 0;
	lastpointx = 0;
	m_Angle_Rot_sum = 0;
	m_center_x = (rect.right + rect.left)/2; 
	m_center_y = (rect.bottom + rect.top)/2;
	sum = 0;
//#ifdef _RELEASE
	font1 = new CDFonts("Calibri", 12.0 , FontsStyleItalic, RGB(20,10,110));
//#endif	
    //Texture = LoadGLTextures(_T("C:\\ZETTools\\Interface2012\\Textures\\Seamless\\Metal\\metal_06.bmp"), PicSize);
	//передаются координаты центра области
	qobj = gluNewQuadric();
#ifdef _ACTIVEX_ZIS
	m_Regulator = new CDetail(long(35), (rect.right + rect.left)/2, (rect.bottom + rect.top)/2, 0, 0, rect_draw.Width(), rect_draw.Height(), 10);//*/
#else
	m_Regulator = new CDetail("C:\\ZETTools\\Interface2012\\Models\\Регулятор 15x5x15 hr.stl",
		(rect.right + rect.left)/2, (rect.bottom + rect.top)/2, 0, 0, 
		rect_draw.Width(), rect_draw.Height(), 10);//*/
#endif
	rect1.bottom = rect_draw.bottom;
	rect1.top = rect_draw.top;
	rect1.right = rect_draw.right;
	rect1.left = rect_draw.left;
	rect_draw.top -= (rect_draw.bottom - rect_draw.top)/4;
    SetImageFileName(m_param.FileName);
//	rect_draw.right += (rect_draw.right - rect_draw.left)/3;
//	rect_draw.left -= (rect_draw.right - rect_draw.left)/5;
#endif
}
//------------------------------------------------------------------------------
void CDRegulator::Clear(void)
{
	font1->Clear();
	m_Regulator->Clear();
}
//------------------------------------------------------------------------------
CDRegulator::~CDRegulator()
{
	//TRACE("DELETE CDRegulator %s\n", (CStringA)m_pParamDraw->Name);
	if (qobj)
		gluDeleteQuadric(qobj);
	if (m_Regulator)
		delete m_Regulator;
	if (font1)
		delete font1;
}
//------------------------------------------------------------------------------
bool CDRegulator::GetNeedEvent()
{
	bool ret = m_bEvent;
	m_bEvent = false;
	return ret;
}
//------------------------------------------------------------------------------
void CDRegulator::SetImageFileName(CString str)
{
    if (!str.IsEmpty())
    {
        m_param.FileName = str;
#ifdef _GDIPLUS
        LoadImageFromFile(&m_pImage, str);
#else
        Texture = LoadGLTextures(m_param.FileName, PicSize, m_param.Number_Texture);
		m_Regulator->CalcTexCoordHMME(PicSize[0], PicSize[1]);
       // m_Regulator->CalcTexCoors(PicSize[0], PicSize[1]);
#endif	
        IsChange();	
    }
}
//------------------------------------------------------------------------------
void CDRegulator::SetTimeAnimation(double val)
{
	if (val > 0.)
		m_param.dTimeAnimation = val;
}
//------------------------------------------------------------------------------
void CDRegulator::SetBorderWidth(float val)
{
	if (m_param.BorderWidth != val)
	{
		m_param.BorderWidth = val;
		rect_draw.left = m_rect.left + m_param.BorderWidth;
		rect_draw.top = m_rect.top + m_param.BorderWidth;
		rect_draw.right = m_rect.right - m_param.BorderWidth;
		rect_draw.bottom = m_rect.bottom - m_param.BorderWidth;
		IsChange();
	}
}
//------------------------------------------------------------------------------
void CDRegulator::DoProcess()
{
	if (m_param.bAnimation)
	{
		IsChange();
	}
}
//------------------------------------------------------------------------------
void CDRegulator::OnMouseEnter()
{
#ifdef _GDIPLUS
#endif
	m_param.MouseIsEnter = true;
	CCustomDraw::OnMouseEnter();
}
//------------------------------------------------------------------------------
void CDRegulator::OnMouseLeave()
{
#ifdef _GDIPLUS
#else
	m_Angle_Prev = m_Angle_Cur;
	IsChange();
	lastpointx += (m_lastx - m_nStart_x);
    m_param.MouseIsEnter = false;
#endif	
	if (m_bFlag)
		m_bFlag = false;
	CCustomDraw::OnMouseLeave();
}
//------------------------------------------------------------------------------
void CDRegulator::OnLButtonClk(UINT nFlags, CPoint point)
{
	bool flag = false;
	m_bEvent = true;
	if (m_bEvent)
	{
		m_param.bAnimation = true;
		dTimeStart = GetTime();
		IsChange();
	}
}
//------------------------------------------------------------------------------
void CDRegulator::OnMouseWheel(UINT nFlags, CPoint point, short delta)
{
#ifdef _GDIPLUS
#else
	if (m_bEvent)
	{
	if (m_param.variant)
	{
		if (!m_param.blocking)
		{
			sum = sum + delta;
			m_Angle_Rot = (atan2((m_nStart_x - m_center_x),(m_nStart_y - m_center_y)) - atan2((point.x - m_center_x),(point.y - m_center_y)))*57.269;
			m_Angle_Rot_sum = m_Angle_Rot_sum + m_Angle_Rot;
			m_Angle_Cur = m_Angle_Prev + sum/10;
		}
	}
	else
	{
/*		sum = sum + delta;
		m_Angle_Rot = point.x;
		m_Angle_Rot_sum = m_Angle_Rot_sum + m_Angle_Rot;
		m_Angle_Cur = m_Angle_Prev + sum/10;*/
	}
		IsChange();
	}	
#endif
}
//------------------------------------------------------------------------------
void CDRegulator::OnLButtonDown(UINT nFlags, CPoint point)
{
		m_nStart_x = point.x;
		m_nStart_y = point.y;
		m_bFlag = true;
		IsChange();
}
//------------------------------------------------------------------------------
void CDRegulator::OnLButtonUp(UINT nFlags, CPoint point)
{
	if (m_bFlag)
	{
		m_bFlag = false;
#ifdef _GDIPLUS
#else	
		m_Angle_Prev = m_Angle_Cur;
#endif
		IsChange();
		lastpointx += (point.x - m_nStart_x);
	}
}
//------------------------------------------------------------------------------
void CDRegulator::OnMouseMove(UINT nFlags, CPoint point)
{
	if (m_bFlag)
	{
	if (m_param.variant)
	{
#ifdef _GDIPLUS
#else
		if (!m_param.blocking)
		{
			m_Angle_Rot = ((atan2((m_nStart_x - m_center_x), (m_nStart_y - m_center_y)) - atan2((point.x - m_center_x), (point.y - m_center_y)))*57.269) / m_param.turn_coeff;
			m_Angle_Rot_sum = m_Angle_Rot_sum + m_Angle_Rot;
			m_Angle_Cur = m_Angle_Prev + m_Angle_Rot;
		}
#endif
	}
	else
	{
#ifdef _GDIPLUS
#else
		if (!m_param.blocking)
		{
			m_Angle_Rot = (point.x - m_nStart_x)/m_param.turn_coeff;
			m_Angle_Rot_sum = m_Angle_Rot_sum + m_Angle_Rot;
			m_Angle_Cur = m_Angle_Prev + m_Angle_Rot;
		}
#endif
	}
	if (m_param.draw_scale)
	{
		if (m_Angle_Cur < 90 + 45)
			m_Angle_Cur = 90 + 45;
		else if (m_Angle_Cur > 365 + 40)
			m_Angle_Cur = 365 + 40;
	}
	m_lastx = point.x;
		IsChange();
	}
}
//------------------------------------------------------------------------------
void CDRegulator::SetRegulatorType(bool val)
{
	if (m_param.m_bRegulatorType != val)
	{
		m_param.m_bRegulatorType = val;
	}
}
//------------------------------------------------------------------------------
#ifdef _GDIPLUS
#else
float CDRegulator::GetAngle()
{
	if (m_param.m_bRegulatorType)
		return m_Angle_Rot_sum;
	else
		return m_Angle_Cur;
}
#endif
#ifdef _GDIPLUS
void CDRegulator::OnDraw()
{// отображение
	if (m_param.bVisible)
	{
	}
}
#else
//------------------------------------------------------------------------------
void CDRegulator::OnDraw()
{// отображение
	if (m_param.bVisible)
	{		
		glPushAttrib(GL_ENABLE_BIT | GL_LIGHTING_BIT | GL_TEXTURE_BIT);

		glMaterialfv(GL_FRONT, GL_AMBIENT, m_param.mat_ambient);
		glMaterialfv(GL_FRONT, GL_DIFFUSE, m_param.mat_diffuse);
		glMaterialfv(GL_FRONT, GL_EMISSION, m_param.mat_emission);
		glMaterialfv(GL_FRONT, GL_SPECULAR, m_param.mat_specular);
		glMaterialf(GL_FRONT, GL_SHININESS, m_param.mat_shininess);

		if (!m_param.m_bRegulatorType)
		{
			if ((m_Angle_Cur >= 120) || (m_Angle_Cur <= 60))
				m_Regulator->SetRotate(m_Angle_Cur);
		}
		else
			m_Regulator->SetRotate(m_Angle_Cur);

		glBindTexture(GL_TEXTURE_2D, Texture);
		m_Regulator->Draw_HMME(m_Regulator->ReadFacet(), m_Regulator->GetQntPlanes(),PicSize);
		//m_Regulator->Draw_texture(m_Regulator->ReadFacet(), m_Regulator->GetQntPlanes(),PicSize);
		if (m_param.drawcaption)
		font1->DrawString(rect_draw, m_param.Name);

		glPushMatrix();
		glLoadIdentity();		
		glPointSize(3);
		glTranslatef(rect1.left + (rect1.right - rect1.left)/2, rect1.bottom - (rect1.bottom - rect1.top)/2, 5);
		glRotatef(m_Angle_Cur - 45, 0.0f, 0.0f, 1.0f);
		glEnable(GL_COLOR_MATERIAL);
		if (m_param.MouseIsEnter)
			glColor3f(1.f/255.f, 203.f/255.f, 247.f/255.f);	
		else
			glColor3f(1.f, 1.f, 1.f);	
		glTranslatef((rect1.right - rect1.left)/5 , (rect1.bottom - rect1.top)/5, 0);
		gluSphere(qobj, (m_rect.right - m_rect.left)/35, 16, 6);
//		auxSolidSphere(2);
		glDisable(GL_COLOR_MATERIAL);
		glPopMatrix();

		if (m_param.draw_scale)
		{
			float Angle = 90;
			for (int i = 0; i < m_param.quan_scale; i++)
			{
				if (i != 0)
					Angle += (float)270.f/(m_param.quan_scale - 1);
				glPushMatrix();
				glLoadIdentity();
				glPointSize(2);
				glTranslatef(rect1.left + (rect1.right - rect1.left)/2, rect1.bottom - (rect1.bottom - rect1.top)/2, 5);
				glRotatef(Angle, 0.0f, 0.0f, 1.0f);
				glEnable(GL_COLOR_MATERIAL);
				if (i == static_cast<int>((m_Angle_Cur - m_param.start_angle)/(270.f/(m_param.quan_scale - 1)) + 0.5))
					glColor3f(203.f/255.f, 1.f/255.f, 1.f/255.f);	
				else
					glColor3f(1.f, 1.f, 1.f);		
				glBegin(GL_POINTS);
				glVertex3f((rect1.right - rect1.left)/2.4, (rect1.bottom - rect1.top)/2.4, 0);
				glEnd();
				glDisable(GL_COLOR_MATERIAL);
				glPopMatrix();
			}
		}
		glPopAttrib();
	}
}
//------------------------------------------------------------------------------
void CDRegulator::SaveParameters(SimpleXML *pSimpleXML)
{
    CString cString;
    char String[16];

    sprintf_s(String, "%d", m_param.nElement);

    cString = '\0';
    for(int Counter2 = 0, Size = strlen(String); Counter2 < Size; Counter2++)
        cString = cString + String[Counter2];

    hXml hRoot = pSimpleXML->GetRootNode(); // возвращает самый верхний корень
    hXml hInterfaceRoot = pSimpleXML->GetChildNode(hRoot); // возвращает следующий корень

    hXml hSliderRoot = pSimpleXML->AddElement(L"Regulator_"+cString, hInterfaceRoot); // создание дочернего элемента от предидущего корня
	// на одном уровне
    hXml hTextureRoot = pSimpleXML->AddElement(L"Texture", hSliderRoot); // создание след уровня
	pSimpleXML->SetAttributeValue(hTextureRoot, L"ZISNumber_Texture", CComVariant(m_param.Number_Texture)); // прописываем имя и аттрибут
    pSimpleXML->SetAttributeValue(hTextureRoot, L"path", CComVariant(m_param.FileName)); // прописываем имя и аттрибут

    hXml hMaterialRoot = pSimpleXML->AddElement(L"Material", hSliderRoot); // создание след уровня

    hXml hAmbientPanelRoot = pSimpleXML->AddElement(L"Ambient", hMaterialRoot);
    pSimpleXML->SetAttributeValue(hAmbientPanelRoot, L"red", CComVariant(m_param.mat_ambient[0]));
    pSimpleXML->SetAttributeValue(hAmbientPanelRoot, L"green", CComVariant(m_param.mat_ambient[1]));
    pSimpleXML->SetAttributeValue(hAmbientPanelRoot, L"blue", CComVariant(m_param.mat_ambient[2]));
    pSimpleXML->SetAttributeValue(hAmbientPanelRoot, L"alpha", CComVariant(m_param.mat_ambient[3]));

    hXml hDiffusePanelRoot = pSimpleXML->AddElement(L"Diffuse", hMaterialRoot);
    pSimpleXML->SetAttributeValue(hDiffusePanelRoot, L"red", CComVariant(m_param.mat_diffuse[0]));
    pSimpleXML->SetAttributeValue(hDiffusePanelRoot, L"green", CComVariant(m_param.mat_diffuse[1]));
    pSimpleXML->SetAttributeValue(hDiffusePanelRoot, L"blue", CComVariant(m_param.mat_diffuse[2]));
    pSimpleXML->SetAttributeValue(hDiffusePanelRoot, L"alpha", CComVariant(m_param.mat_diffuse[3]));

    hXml hSpecularPanelRoot = pSimpleXML->AddElement(L"Specular", hMaterialRoot);
    pSimpleXML->SetAttributeValue(hSpecularPanelRoot, L"red", CComVariant(m_param.mat_specular[0]));
    pSimpleXML->SetAttributeValue(hSpecularPanelRoot, L"green", CComVariant(m_param.mat_specular[1]));
    pSimpleXML->SetAttributeValue(hSpecularPanelRoot, L"blue", CComVariant(m_param.mat_specular[2]));
    pSimpleXML->SetAttributeValue(hSpecularPanelRoot, L"alpha", CComVariant(m_param.mat_specular[3]));

    hXml hEmissionPanelRoot = pSimpleXML->AddElement(L"Emission", hMaterialRoot);
    pSimpleXML->SetAttributeValue(hEmissionPanelRoot, L"red", CComVariant(m_param.mat_emission[0]));
    pSimpleXML->SetAttributeValue(hEmissionPanelRoot, L"green", CComVariant(m_param.mat_emission[1]));
    pSimpleXML->SetAttributeValue(hEmissionPanelRoot, L"blue", CComVariant(m_param.mat_emission[2]));
    pSimpleXML->SetAttributeValue(hEmissionPanelRoot, L"alpha", CComVariant(m_param.mat_emission[3]));

    hXml hShininessPanelRoot = pSimpleXML->AddElement(L"Shininess", hMaterialRoot);
    pSimpleXML->SetAttributeValue(hShininessPanelRoot, L"blink_coefficient", CComVariant(m_param.mat_shininess));
}
//------------------------------------------------------------------------------
void CDRegulator::LoadParameters(SimpleXML *pSimpleXML)
{
    VARIANT vAtrrib;
	VariantInit(&vAtrrib);

    CString str = L"";
    CString Pattern = L"Regulator_";
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
    pSimpleXML->GetAttributeValue(hTextureRoot, L"ZISNumber_Textur", vAtrrib);
    if (vAtrrib.vt == VT_BSTR)
        if (CString(vAtrrib.bstrVal).GetLength() != 0)
			m_param.Number_Texture = _wtoi(vAtrrib.bstrVal);
    pSimpleXML->GetAttributeValue(hTextureRoot, L"path", vAtrrib);
    if (vAtrrib.vt == VT_BSTR)
        if (CString(vAtrrib.bstrVal).GetLength() != 0)
            SetImageFileName(CString(vAtrrib.bstrVal));

    //рассеянный цвет материала (цвет материала в тени)
    hXml hMaterialRoot = pSimpleXML->GetNextNode(hTextureRoot);
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
}
//------------------------------------------------------------------------------
void CDRegulator::SetScale (int quan)
{
	m_param.quan_scale = quan;
}
//------------------------------------------------------------------------------
int CDRegulator::GetScale(void)
{
	return m_param.quan_scale;
}
//------------------------------------------------------------------------------
void CDRegulator::SetColor(GLfloat *ma, GLfloat *md, GLfloat *me, GLfloat *msp, GLfloat msh)
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
//------------------------------------------------------------------------------
float CDRegulator::GetRegulatorValue(void)
{
	if (m_param.draw_scale)
		return static_cast<int>((m_Angle_Cur - m_param.start_angle)/(270.f/(m_param.quan_scale - 1)) + 0.5);
	else
		return m_Angle_Cur - m_param.start_angle;
}
//------------------------------------------------------------------------------
void CDRegulator::SetRegulatorValue(float angle)
{
	if (!m_param.blocking)
	{
		if (m_param.draw_scale)
		{
			m_Angle_Prev = (angle)*(270.f / (m_param.quan_scale - 1)) + 135.f;
			m_Angle_Cur = (angle)*(270.f / (m_param.quan_scale - 1)) + 135.f;
		}
		else
		{
			m_Angle_Prev = angle;
			m_Angle_Cur = angle;
		}
	}
}
//------------------------------------------------------------------------------
void CDRegulator::SetTurnCoeff ( float turn_coeff)
{
	if (turn_coeff == 0)
		m_param.turn_coeff = 1;
	else
		m_param.turn_coeff = turn_coeff;
}
//------------------------------------------------------------------------------
void CDRegulator::OnLButtonDblClk(UINT nFlags, CPoint point)
{
	if (m_param.draw_scale)
	{
		m_Angle_Prev = 0.f + 135.f;
		m_Angle_Cur = 0.f + 135.f;
	}
	else
	{
//  	m_nStart_x = angle;
		m_Angle_Prev = 270.f;
		m_Angle_Cur = 270.f;
	}
}
#endif