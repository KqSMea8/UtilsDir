//------------------------------------------------------------------------------
// CNumIndicator.cpp : файл реализации
// ѕр€моугольник элемента в процессе работы не мен€етс€, поэтому положение
// (горизонтальное или вертикальное) определ€етс€ один раз в конструкторе.
//------------------------------------------------------------------------------
#include "stdafx.h"
#include "DLevelIndicator.h"
//------------------------------------------------------------------------------
int g_nIndicatorLvl = 0;		// счЄтчик статических картинок
//------------------------------------------------------------------------------
#ifdef _GDIPLUS
CDLevelIndicator::CDLevelIndicator(CRect rect, PVOID ppGraphics,
	bool *pFlagIsChange, double *pTimeAnimation) :
	CCustomDraw(rect, ppGraphics, pFlagIsChange, pTimeAnimation)
#else
CDLevelIndicator::CDLevelIndicator(CRect rect,
	bool *pFlagIsChange, double *pTimeAnimation, bool rotate, bool setdiod) :
	CCustomDraw(rect, pFlagIsChange, pTimeAnimation)
#endif
{
	m_pParamDraw = &m_param;	// обеспечение ќЅў»’ параметров
	m_param.nElement = ++g_nIndicatorLvl;
	SetNameWithNumber(L"IndicatorLevel");
	//TRACE("CREATE CDLevelIndicator %s\n", (CStringA)m_pParamDraw->Name);
	SetBorderWidth(rect.Width()/4);
	// переменные класса
	DefineRectAll();
	m_bDanger = false;
	flag = false;
#ifdef _GDIPLUS
	m_param.pFont = new CZFont();
#else
	m_param.pFont = new CDFonts("Calibri", int(rect.Height() - 6) , FontsStyleRegular, RGB(40,90,40));
	//передаютс€ координаты центра области
	qobj = gluNewQuadric();
	/*if (setdiod)
	if (rotate)
	{
		Texture = LoadGLTextures(_T("C:\\ZETTools\\Interface2012\\Textures\\Patterns\\metal_mart14.bmp"), PicSize);
		m_SvetoDiodOsn = new CDetail("C:\\ZETTools\\Interface2012\\Models\\диод6-1.stl", // вертикальный
		rect.right - (rect.bottom - rect.top)/2, rect.top + (rect.bottom - rect.top)/2, 0, 0, 
		(rect.bottom - rect.top) - (rect.bottom - rect.top)/5, (rect.bottom - rect.top) - (rect.bottom - rect.top)/5, 5);
		m_SvetoDiodLed = new CDetail("C:\\ZETTools\\Interface2012\\Models\\диод6-2.stl", // вертикальный
		rect.right - (rect.bottom - rect.top)/2, rect.top + (rect.bottom - rect.top)/2, 0, 0, 
		(rect.bottom - rect.top) - (rect.bottom - rect.top)/3, (rect.bottom - rect.top) - (rect.bottom - rect.top)/3, 5);
	}
	else
	{
		Texture = LoadGLTextures(_T("C:\\ZETTools\\Interface2012\\Textures\\Patterns\\metal_mart14.bmp"), PicSize);
		m_SvetoDiodOsn = new CDetail("C:\\ZETTools\\Interface2012\\Models\\диод6-1.stl", // горизонтальный
		rect.right - (rect.right - rect.left)/2, rect.top + (rect.right - rect.left)/2, 0, 0, 
		(rect.right - rect.left) - (rect.right - rect.left)/5, (rect.right - rect.left) - (rect.right - rect.left)/5, 5);
		m_SvetoDiodLed = new CDetail("C:\\ZETTools\\Interface2012\\Models\\диод6-2.stl", // горизонтальный
		rect.right - (rect.right - rect.left)/2, rect.top + (rect.right - rect.left)/2, 0, 0, 
		(rect.right - rect.left) - (rect.right - rect.left)/3, (rect.right - rect.left) - (rect.right - rect.left)/3, 5);
	}*/
	m_param.rotateindicator = rotate; // поворот на 90 градусов
	m_param.setdiod = setdiod; // установка диода
	m_bringmaxlevel = 0.12;
#endif
}
//------------------------------------------------------------------------------
void CDLevelIndicator::Clear (void)
{
	m_param.pFont->Clear();
	/*if (m_param.setdiod)
	{
	m_SvetoDiodOsn->Clear();
	m_SvetoDiodLed->Clear();
	}*/
}
//------------------------------------------------------------------------------
CDLevelIndicator::~CDLevelIndicator()
{
	//TRACE("DELETE CDLevelIndicator %s\n", (CStringA)m_pParamDraw->Name);
	if (m_param.pFont)
		delete m_param.pFont;
//	if (m_param.setdiod)
//	{
//	if (m_SvetoDiodOsn)
//		delete m_SvetoDiodOsn;
//	if (m_SvetoDiodLed)
//		delete m_SvetoDiodLed;
//	}
	if (qobj)
		gluDeleteQuadric(qobj);
}
//------------------------------------------------------------------------------
void CDLevelIndicator::SetText(CString val)
{
	if (m_param.Text != val)
	{
		m_param.Text = val;
		IsChange();
	}
}
//------------------------------------------------------------------------------
void CDLevelIndicator::DefineRectAll()
{
	m_rect1 = m_rectT = m_rect;
	if (m_param.PositionType == tpHorizontal)
	{
		m_rectT.left = m_rect.right - (m_rect.bottom - m_rect.top);
		m_rect1.right = m_rectT.left - m_param.Distance;
		m_SizeRectT = m_rectT.right - m_rectT.left;
	}
	else
	{
		m_rectT.bottom = m_rect.top + (m_rect.right - m_rect.left);
		m_rect1.top = m_rectT.bottom + m_param.Distance;
		m_SizeRectT = m_rectT.bottom - m_rectT.top;
	}
	DefineRect2(&m_rect1, &m_rect2, m_param.BorderWidth);
	if (m_param.PositionType == tpHorizontal)
		m_LengthRect2 = m_rect2.Width();
	else
		m_LengthRect2 = m_rect2.Height();
}
//------------------------------------------------------------------------------
void CDLevelIndicator::SetAmplitude(float val)
{
	long dlina;
	if (m_param.Amplitude != val)
	{
		m_param.Amplitude = val;
		if (!m_param.rotateindicator)
			dlina = (m_rect.bottom - m_rect.top);
		else
			dlina = (m_rect.right - m_rect.left);
		if (m_param.Amplitude > 0.95)
		{
			m_bringmaxlevel = 1.f;
			m_param.Amplitude = 1.;
		}
		if (m_param.Amplitude < 0.0001) 
			m_param.Amplitude = 0.00015;	
		if (m_param.LinLog == 0) 
			m_param.ur = m_param.Amplitude;
		else 
		{
			if(m_param.Amplitude < 0.0001) 
				m_param.Amplitude = 0.00015;
			m_param.ur = (log10(m_param.Amplitude) + 4.) / 4.;
		}
		if(m_param.ur < 0)
			m_param.ur = 0;
		m_param.formax = (long)(dlina *0.07);
		IsChange();
		}
}
//------------------------------------------------------------------------------
void CDLevelIndicator::SetMaxLevel (float valevel)
{
	if (m_param.maxlevel != valevel)
		m_param.maxlevel = valevel;
	IsChange();
}
//------------------------------------------------------------------------------
void CDLevelIndicator::SetThresholdDanger(float val)
{
	if ( (val > 0.9f) && (val <= 1.f) && (m_param.ThresholdDanger != val) )
	{
		m_param.ThresholdDanger = val;
		IsChange();
	}
}
//------------------------------------------------------------------------------
void CDLevelIndicator::SetLinLog(long val)
{
	long newVal;
	if (val != 0)
		newVal = 1;
	else
		newVal = 0;
	if (m_param.LinLog != newVal)
	{
		m_param.LinLog = newVal;
		IsChange();
	}
}
//------------------------------------------------------------------------------
void CDLevelIndicator::OnMouseEnter()
{
	CCustomDraw::OnMouseEnter();
}
//------------------------------------------------------------------------------
void CDLevelIndicator::OnMouseLeave()
{
	CCustomDraw::OnMouseLeave();
}
//------------------------------------------------------------------------------
void CDLevelIndicator::SetDistance(int val)
{
	int newVal;
	if (val < 2)
		newVal = 2;
	else
		newVal = val;
	if (m_param.Distance != newVal)
	{
		m_param.Distance = newVal;
		DefineRectAll();
		IsChange();
	}
}
//------------------------------------------------------------------------------
void CDLevelIndicator::SetBorderWidth(int val)
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
void CDLevelIndicator::SetBorderColor(COLORREF val)
{
	if (m_param.BorderColor != val)
	{
		m_param.BorderColor = val;
		IsChange();
	}
}
//------------------------------------------------------------------------------
void CDLevelIndicator::SetBackColor(COLORREF val)
{
	if (m_param.BackColor != val)
	{
		m_param.BackColor = val;
		IsChange();
	}
}
//------------------------------------------------------------------------------
void CDLevelIndicator::SetTimeAnimation(double val)
{
	if (val > 0.)
		m_param.dTimeAnimation = val;
}
//------------------------------------------------------------------------------
void CDLevelIndicator::DoProcess()
{
	if (m_param.bAnimation)
	{
		if ( (GetTime() - m_dTimeStart) > m_param.dTimeAnimation )
		{
			m_param.bAnimation = false;
			if (m_param.Amplitude > m_param.ThresholdDanger)
				m_bDanger = true;
		}
		IsChange();
	}
}
//------------------------------------------------------------------------------
void CDLevelIndicator::OnLButtonClk(UINT nFlags, CPoint point)
{
	if (point.x < m_rect.right && point.x > m_rect.right - (m_rect.bottom - m_rect.top))
		if (point.y < m_rect.bottom && point.y > m_rect.top)
			if (m_param.Amplitude < 0.999)
				m_bringmaxlevel = 0.12;
	if ( m_bDanger && PointInRect(point, &m_rectT) )
	{
		m_bDanger = false;
		m_param.bAnimation = true;
		m_dTimeStart = GetTime();
		IsChange();
	}
}
//------------------------------------------------------------------------------
void CDLevelIndicator::SaveParameters(SimpleXML *pSimpleXML)
{
    CString cString;
    char String[16];

    sprintf_s(String, "%d", m_param.nElement);

    cString = '\0';
    for(unsigned int Counter2=0; Counter2<strlen(String); Counter2++)
        cString = cString + String[Counter2];

    hXml hRoot = pSimpleXML->GetRootNode();
    hXml hInterfaceRoot = pSimpleXML->GetChildNode(hRoot);

    hXml hLevelIndicatorRoot = pSimpleXML->AddElement(L"LevelIndicator_"+cString, hInterfaceRoot);

    hXml hMaterialRoot = pSimpleXML->AddElement(L"Material", hLevelIndicatorRoot);

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
void CDLevelIndicator::LoadParameters(SimpleXML *pSimpleXML)
{
    VARIANT vAtrrib;
    CString str = L"";
    CString Pattern = L"LevelIndicator_";
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
    //рассе€нный цвет материала (цвет материала в тени)
    hXml hMaterialRoot = pSimpleXML->GetChildNode(hElement);
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

    //цвет диффузного отражени€ материала
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

    //цвет зеркального отражени€ материала
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

    //определ€ет степень зеркального отражени€ материала
    hXml hShininessRoot = pSimpleXML->GetNextNode(hEmissionRoot);
    pSimpleXML->GetAttributeValue(hShininessRoot, L"blink_coefficient", vAtrrib);
    if (vAtrrib.vt == VT_BSTR)
        if (CString(vAtrrib.bstrVal).GetLength() != 0)
            m_param.mat_shininess = _wtof(vAtrrib.bstrVal);
}
//------------------------------------------------------------------------------
#ifdef _GDIPLUS
void CDLevelIndicator::OnDraw()
{// отображение

}
#else
//------------------------------------------------------------------------------
long CDLevelIndicator::PointColor(double bit)
{
	long color;
	double pal;

	pal = bit*5;
	double k = 255.9;
	color = 0;
	int a = static_cast<int>(pal);
	/**************************************************************************************
	¬ вольтметре переменного тока наблюдаетс€ ошибка преобразовани€ типов. ѕреобразование 
	переменной из типа double в тип int происходит не по стандарту €зыка т.е. по стандарту 
	при преобразовании из типа double в тип int должна попросту отбрасыватьс€ дробна€ часть 
	числа double, таким образом ошибка преобразовани€ заключаетс€ в том что преобразование
	выполн€етс€ как округление до большего целого. Ќапример, при преобразовании числа 
	4.7583095746 в тип int (по средствам как int(pal) так и static_cast<int>(pal)) получаем 5.
	¬ св€зи с чем был введен дополнительный код что бы исправить эту ошибку.
	**************************************************************************************/
	if ((pal < 5 && a == 5)||(pal < 4 && a == 4)||(pal < 3 && a == 3)||(pal < 2 && a == 2))
		a -= 1;
	switch (a)
	{
	case 0:
		color=int(sqrt(pal)*k)*0x10000;
		break;
	case 1:
		color=int(sqrt(pal-1)*k)*0x100+0xff0000;
		break;
	case 2:
		color=int(sqrt(3-pal)*k)*0x10000+0xff00;
		break;
	case 3:
		color=int(sqrt(pal-3)*k)+0xff00;
		break;
	case 4:
		color=int(sqrt(5-pal)*k)*0x100+0xff;
		break;
	case 5:
		color=0xff;
	}

	return color;
}
//------------------------------------------------------------------------------
void CDLevelIndicator::OnDraw()
{// отображение
//	TRACE (L" level %f", m_param.ur);
//	TRACE (L" maxlevel %f\n", m_param.maxlevel);
	if (m_param.bVisible)
	{
		glPushAttrib(GL_LIGHTING_BIT);

		glMaterialfv(GL_FRONT, GL_AMBIENT,   m_param.mat_ambient);
		glMaterialfv(GL_FRONT, GL_DIFFUSE,   m_param.mat_diffuse);
		glMaterialfv(GL_FRONT, GL_EMISSION,  m_param.mat_emission);
		glMaterialfv(GL_FRONT, GL_SPECULAR,  m_param.mat_specular);
		glMaterialf (GL_FRONT, GL_SHININESS, m_param.mat_shininess);
		glEnable(GL_COLOR_MATERIAL);
		if (flag)
			FillRectSolid(&m_rect, 1, m_param.BackColor);
		CRect rect2 = m_rect;
		if (m_param.setdiod == true)
			if (m_param.rotateindicator)
				rect2.right -= (m_rect.bottom - m_rect.top) + 5;
			else
				rect2.top += (m_rect.right - m_rect.left) + 5;

		if (m_param.FieldRadius > 0)
			FillRoundRectSolid(&rect2, 0.125, m_param.FieldRadius, m_param.FieldSolidColor);
		else
			FillRectSolid(&rect2, 0.125, m_param.FieldSolidColor);

		if (m_param.setdiod == true)
		{
			long newcolor;
			newcolor = PointColor(m_param.ur);
			long r, g, b;
			r = (newcolor)&255;
			g = (newcolor>>8)&255;
			b = (newcolor>>16)&255;

			if (m_param.rotateindicator) // горизонтальное исполнение
			{
				glPushMatrix();
				glPushAttrib(GL_LIGHTING_BIT);
				glDisable(GL_LIGHTING);
				glColor3f(0.12f, 0.12f, 0.12f);
//				glColor3ub(GetRValue(m_param.FieldSolidColor), GetGValue(m_param.FieldSolidColor), GetBValue(m_param.FieldSolidColor));
				glTranslatef(m_rect.right - (m_rect.bottom - m_rect.top)/2, 0.5 + m_rect.top + (m_rect.bottom - m_rect.top)/2, 0.2f);
				gluDisk(qobj, 0, (m_rect.bottom - m_rect.top)/2.1, 20, 1);
				glPopAttrib();
				glPopMatrix();

				if (m_param.Amplitude > m_param.maxlevel)
				{					
					glPushMatrix();
					glTranslatef(m_rect.left + 5, m_rect.top + (m_rect.bottom - m_rect.top)/2, 0.0f);
					glRotatef(90, 0.0f, 0.0f, 10.0f);
					glRotatef(90.0, 1.0f, 0.0f, 0.0f);
					glColor3f(1.f, 0.f, 0.f);
					gluCylinder(qobj, (m_rect.bottom - m_rect.top)/2.6, (m_rect.bottom - m_rect.top)/2.6, (m_rect.right - m_rect.left  - (m_rect.bottom - m_rect.top))  - 15, 20, 20);
					glPopMatrix();
				}
				else
				{
					glPushMatrix();
					glColor3ub(r, g, b);
					glTranslatef(m_rect.left + 5, m_rect.top + (m_rect.bottom - m_rect.top)/2, 0.0f);
					glRotatef(90, 0.0f, 0.0f, 10.0f);
					glRotatef(90.0, 1.0f, 0.0f, 0.0f);
					gluCylinder(qobj, (m_rect.bottom - m_rect.top)/2.6, (m_rect.bottom - m_rect.top)/2.6, (m_param.ur*((m_rect.right - m_rect.left  - (m_rect.bottom - m_rect.top)) - m_param.formax))/(m_param.maxlevel) , 10, 10);
					glPopMatrix();

					//glColor3f(0.25f, 0.25f, 0.25f);
				}

				glPushMatrix();
				glColor3f(m_bringmaxlevel, 0.12, 0.12);
				glTranslatef(m_rect.right - (m_rect.bottom - m_rect.top)/2, 0.5 + m_rect.top + (m_rect.bottom - m_rect.top)/2, 0.0f);
				gluSphere(qobj, (m_rect.bottom - m_rect.top)/2.6, 16, 6);
				glPopMatrix();
			}
			else // вертикальное исполнение
			{
				glPushMatrix();
				glPushAttrib(GL_LIGHTING_BIT);
				glDisable(GL_LIGHTING);
				glColor3f(0.12f, 0.12f, 0.12f);
//				glColor3ub(GetRValue(m_param.FieldSolidColor), GetGValue(m_param.FieldSolidColor), GetBValue(m_param.FieldSolidColor));
				glTranslatef(m_rect.right - (m_rect.right - m_rect.left)/2, 0.5 + m_rect.top + (m_rect.right - m_rect.left)/2, 0.2f);
				gluDisk(qobj, 0, (m_rect.right - m_rect.left)/2.1, 20, 1);
				glPopAttrib();
				glPopMatrix();

				if (m_param.Amplitude >= m_param.maxlevel)
				{
					glPushMatrix();
					glTranslatef(0.5 + m_rect.left + (m_rect.right - m_rect.left)/2 , m_rect.bottom - 5, 0.0f);
					glRotatef(0, 0.0f, 0.0f, 10.0f);
					glRotatef(90, 0.0f, 1.0f, 0.0f); 
					glColor3ub(255.f, 0.f, 0.f);
					gluCylinder(qobj, (m_rect.right - m_rect.left)/2.6, (m_rect.right - m_rect.left)/2.6, (m_rect.bottom - m_rect.top - (m_rect.right - m_rect.left)) - 10, 20, 20);
					glPopMatrix();
				}
				else
				{ 
					glPushMatrix();
					glTranslatef(0.5 + m_rect.left + (m_rect.right - m_rect.left)/2, m_rect.bottom - 5, 0.0f);
					glRotatef(0, 0.0f, 0.0f, 1.0f);
					glRotatef (90, 0.0f, 1.0f, 0.0f);
					glColor3ub(r, g, b);
					gluCylinder(qobj, (m_rect.right - m_rect.left)/2.6, (m_rect.right - m_rect.left)/2.6, (m_param.ur*((m_rect.bottom - m_rect.top - (m_rect.right - m_rect.left)) - m_param.formax))/(m_param.maxlevel), 20, 20);
					glPopMatrix();
				}

				glPushMatrix();
				glColor3f(m_bringmaxlevel, 0.f, 0.f);
				glTranslatef(m_rect.right - (m_rect.right - m_rect.left)/2, 0.5 + m_rect.top + (m_rect.right - m_rect.left)/2, 0.0f);
				gluSphere(qobj, (m_rect.right - m_rect.left)/2.6, 16, 6);
				glPopMatrix();
			}
		}
		else
		{
			long newcolor;
			newcolor = PointColor(m_param.ur);
			long r, g, b;
			r = (newcolor)&255;
			g = (newcolor>>8)&255;
			b = (newcolor>>16)&255;
			glColor3f(abs((m_param.Amplitude)/(m_param.maxlevel)), 0.0f, abs(1.0f - (m_param.Amplitude)/(m_param.maxlevel)));
			if (m_param.rotateindicator) // горизонтальное исполнение
			{
				if (m_param.Amplitude >= m_param.maxlevel)
				{
					glPushMatrix();
					glTranslatef(m_rect.left + 5, 0.5 + m_rect.top + (m_rect.bottom - m_rect.top)/2, -5.0f);
					glRotatef(90, 0.0f, 0.0f, 1.0f);
					glRotatef(90.0, 1.0f, 0.0f, 0.0f);
					glColor3ub(255, 0, 0);
					gluCylinder(qobj, (m_rect.bottom - m_rect.top)/2.7, (m_rect.bottom - m_rect.top)/2.7, (m_rect.right - m_rect.left) - 10, 20, 20);
					glPopMatrix();
				}
				else
				{
					glPushMatrix();
					glTranslatef(m_rect.left + 5, 0.5 + m_rect.top + (m_rect.bottom - m_rect.top)/2, -5.0f);
					glRotatef(90, 0.0f, 0.0f, 1.0f);
					glRotatef(90.0, 1.0f, 0.0f, 0.0f);
					glColor3ub(r, g, b);
					gluCylinder(qobj, (m_rect.bottom - m_rect.top)/2.7, (m_rect.bottom - m_rect.top)/2.7, (m_param.ur*((m_rect.right - m_rect.left) - m_param.formax))/(m_param.maxlevel), 20, 20);
					glPopMatrix();
				}
			}
			else // вертикальное исполнение
			{
				if (m_param.Amplitude >= m_param.maxlevel)
				{
					glPushMatrix();
					glTranslatef(0.5 + m_rect.left + (m_rect.right - m_rect.left)/2, m_rect.bottom - 5, -5.0f);
					glRotatef(0, 0.0f, 0.0f, 1.0f);
					glRotatef (90, 0.0f, 1.0f, 0.0f);
					glColor3ub(255.f, 0.f, 0.f);
					gluCylinder(qobj, (m_rect.right - m_rect.left)/2.7, (m_rect.right - m_rect.left)/2.7, (m_rect.bottom - m_rect.top) - 10, 20, 20);
					glPopMatrix();
				}
				else
				{
					glPushMatrix();
					glTranslatef(0.5 + m_rect.left + (m_rect.right - m_rect.left)/2, m_rect.bottom - 5, -5.0f);
					glRotatef(0, 0.0f, 0.0f, 1.0f);
					glRotatef (90, 0.0f, 1.0f, 0.0f);
					glColor3ub(r, g, b);
					gluCylinder(qobj, (m_rect.right - m_rect.left)/2.7, (m_rect.right - m_rect.left)/2.7, (m_param.ur*((m_rect.bottom - m_rect.top) - m_param.formax))/(m_param.maxlevel), 20, 20);
					glPopMatrix();
				}
			}
		}

		glPopAttrib();
	}
}
//------------------------------------------------------------------------------
#endif