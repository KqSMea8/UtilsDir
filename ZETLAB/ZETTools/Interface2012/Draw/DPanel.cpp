//------------------------------------------------------------------------------
// DPanel.cpp : файл реализации
//------------------------------------------------------------------------------
#include "stdafx.h"
#include "C:\ZETTools\Interface2012\Draw\DPanel.h"
//------------------------------------------------------------------------------
int g_nPanel = 0;		// счётчик
//------------------------------------------------------------------------------
#ifdef _GDIPLUS
CDPanel::CDPanel(CRect rect, PVOID ppGraphics,
				bool *pFlagIsChange, double *pTimeAnimation) :
				CCustomDraw(rect, ppGraphics, pFlagIsChange, pTimeAnimation)
#else
CDPanel::CDPanel(CRect rect, bool *pFlagIsChange, double *pTimeAnimation) :
				CCustomDraw(rect, pFlagIsChange, pTimeAnimation)
#endif
{
	m_pParamDraw = &m_param;	// обеспечение ОБЩИХ параметров
	m_param.nElement = ++g_nPanel;
	m_param.FieldRect = rect;
	SetNameWithNumber(L"Panel");
	//TRACE("CREATE CDPanel %s\n", (CStringA)m_pParamDraw->Name);
	size = 8;
    Depth = 10;
#ifdef _ACTIVEX_ZIS
	//левая часть панели
	m_Panel[0] = new CDetail(long(26), size/2.0f + m_rect.left, (m_rect.bottom + m_rect.top)/2.0f, -Depth, 0, size, m_rect.Height() - 2*size, Depth);		
	//левый верхний угол
	m_Panel[1] = new CDetail(long(27), size/2.0f + m_rect.left, size/2.0f + m_rect.top, -Depth, 0, size, size, Depth);
	//верхняя часть панели
	m_Panel[2] = new CDetail(long(28), (m_rect.right + m_rect.left)/2.0f, size/2.0f + m_rect.top, -Depth, 0, m_rect.Width() - 2.0f*size, size, Depth);
	//правый верхний угол
	m_Panel[3] = new CDetail(long(29), m_rect.right - size/2.0f, size/2.0f + m_rect.top, -Depth, 0, size, size, Depth);
	//правая часть панели
	m_Panel[4] = new CDetail(long(30), m_rect.right - size/2.0f,(m_rect.bottom + m_rect.top)/2.0f, -Depth, 0, size, m_rect.Height() - 2.0f*size, Depth);
	//правый нижний угол
	m_Panel[5] = new CDetail(long(31), m_rect.right - size/2.0f, m_rect.bottom - size/2.0f, -Depth, 0, size, size, Depth);
	//нижняя часть панели
	m_Panel[6] = new CDetail(long(32), (m_rect.right + m_rect.left)/2.0f, m_rect.bottom - size/2.0f, -Depth, 0, m_rect.Width() - 2.0f*size, size, Depth);
	//левый нижний угол
	m_Panel[7] = new CDetail(long(33), size/2.0f + m_rect.left, m_rect.bottom - size/2.0f, -Depth, 0, size, size, Depth);			
	//центр
	m_Panel[8] = new CDetail(long(34), (m_rect.right + m_rect.left)/2.0f, (m_rect.bottom + m_rect.top)/2.0f, -Depth, 0, m_rect.Width() - 2.0f*size, m_rect.Height() - 2.0f*size, Depth);//*/
#else
	//левая часть панели
	m_Panel[0] = new CDetail("C:\\ZETTools\\Interface2012\\Models\\Поддон\\Поддон левый.stl",
		size/2.0f + m_rect.left, (m_rect.bottom + m_rect.top)/2.0f, -Depth, 0, 
		size, m_rect.Height() - 2*size, Depth);		
	//левый верхний угол
	m_Panel[1] = new CDetail("C:\\ZETTools\\Interface2012\\Models\\Поддон\\Поддон левый нижний.stl",
		size/2.0f + m_rect.left, size/2.0f + m_rect.top, -Depth, 0, 
		size, size, Depth);
	//верхняя часть панели
	m_Panel[2] = new CDetail("C:\\ZETTools\\Interface2012\\Models\\Поддон\\Поддон нижний.stl",
		(m_rect.right + m_rect.left)/2.0f, size/2.0f + m_rect.top, -Depth, 0, 
		m_rect.Width() - 2.0f*size, size, Depth);
	//правый верхний угол
	m_Panel[3] = new CDetail("C:\\ZETTools\\Interface2012\\Models\\Поддон\\Поддон правый нижний.stl",
		m_rect.right - size/2.0f, size/2.0f + m_rect.top, -Depth, 0, 
		size, size, Depth);
	//правая часть панели
	m_Panel[4] = new CDetail("C:\\ZETTools\\Interface2012\\Models\\Поддон\\Поддон правый.stl",
		m_rect.right - size/2.0f,(m_rect.bottom + m_rect.top)/2.0f, -Depth, 0,
		size, m_rect.Height() - 2.0f*size, Depth);
	//правый нижний угол
	m_Panel[5] = new CDetail("C:\\ZETTools\\Interface2012\\Models\\Поддон\\Поддон правый верхний.stl",
		m_rect.right - size/2.0f, m_rect.bottom - size/2.0f, -Depth, 0,
		size, size, Depth);
	//нижняя часть панели
	m_Panel[6] = new CDetail("C:\\ZETTools\\Interface2012\\Models\\Поддон\\Поддон верхний.stl",
		(m_rect.right + m_rect.left)/2.0f, m_rect.bottom - size/2.0f, -Depth, 0,
		m_rect.Width() - 2.0f*size, size, Depth);
	//левый нижний угол
	m_Panel[7] = new CDetail("C:\\ZETTools\\Interface2012\\Models\\Поддон\\Поддон левый верхний.stl",
		size/2.0f + m_rect.left, m_rect.bottom - size/2.0f, -Depth, 0,
		size, size, Depth);			
	//центр
	m_Panel[8] = new CDetail("C:\\ZETTools\\Interface2012\\Models\\Поддон\\Поддон центр.stl",
		(m_rect.right + m_rect.left)/2.0f, (m_rect.bottom + m_rect.top)/2.0f, -Depth, 0, 
		m_rect.Width() - 2.0f*size, m_rect.Height() - 2.0f*size, Depth);//*/
#endif
	SetFieldFileNameTexture(m_param.FileName);
}
//------------------------------------------------------------------------------
CDPanel::~CDPanel()
{
	//TRACE("DELETE CDPanel %s\n", (CStringA)m_pParamDraw->Name);
#ifdef _GDIPLUS
	DeleteGdiPlusObject(m_pImage);
#else
	for (int i = 0; i < 9; i++)
		delete m_Panel[i];
#endif
}
//------------------------------------------------------------------------------
void CDPanel::Clear()
{
	for (int i = 0; i < 9; i++)
		m_Panel[i]->Clear();
}
//------------------------------------------------------------------------------
void CDPanel::SetBorderHeight(int val)
{
	if ( (val >= 0) && (m_param.BorderHeight != val) )
	{
		m_param.BorderHeight = val;
		IsChange();
	}
}
//------------------------------------------------------------------------------
void CDPanel::SetBorderRadius(int val)
{
	int n;
	if (val < 0)
		n = 0;
	else if (val >13)
		n = 13;
	else
		n = val;
	if (m_param.BorderRadius != n)
	{
		m_param.BorderRadius = n;
		IsChange();
	}
}
//------------------------------------------------------------------------------
void CDPanel::SetBorderFillType(PanelFillType val)
{
	PanelFillType n;
	if ( (val == pftSolid) || (val == pftGradient) )
		n = val;
	else
		n = m_param.BorderFillType;
	if (m_param.BorderFillType != n)
	{
		m_param.BorderFillType = n;
		IsChange();
	}
}
//------------------------------------------------------------------------------
void CDPanel::SetBorderSolidColor(COLORREF val)
{
	if (m_param.BorderSolidColor != val)
	{
		m_param.BorderSolidColor = val;
		IsChange();
	}
}
//------------------------------------------------------------------------------
void CDPanel::SetBorderGradientColorStart(COLORREF val)
{
	if (m_param.BorderGradientColorStart != val)
	{
		m_param.BorderGradientColorStart = val;
		IsChange();
	}
}
//------------------------------------------------------------------------------
void CDPanel::SetBorderGradientColorEnd(COLORREF val)
{
	if (m_param.BorderGradientColorEnd != val)
	{
		m_param.BorderGradientColorEnd = val;
		IsChange();
	}
}
//------------------------------------------------------------------------------
void CDPanel::SetFieldFillType(PanelFillType val)
{
	PanelFillType n;
	if ( (val == pftSolid) || (val == pftTexture) )
		n = val;
	else
		n = m_param.FieldFillType;
	if (m_param.FieldFillType != val)
	{
		m_param.FieldFillType = val;
		IsChange();
	}
}
//------------------------------------------------------------------------------
void CDPanel::SetFieldRadius(int val)
{
	int n;
	if (val < 0)
		n = 0;
	else if (val >13)
		n = 13;
	else
		n = val;
	if (m_param.FieldRadius != n)
	{
		m_param.FieldRadius = n;
		IsChange();
	}
}
//------------------------------------------------------------------------------
void CDPanel::SetFieldSolidColor(COLORREF val)
{
	if (m_param.FieldSolidColor != val)
	{
		m_param.FieldSolidColor = val;
		IsChange();
	}
}
//------------------------------------------------------------------------------
void CDPanel::SetFieldFileNameTexture(CString val)
{
	if (!val.IsEmpty())
	{
		m_param.FileName = val;
#ifdef _GDIPLUS
		LoadImageFromFile(&m_pImage, val);
#else
		Texture = LoadGLTextures(val, PicSize, m_param.Number_Texture);
        for (int i = 0; i < 9; i++)
            m_Panel[i]->CalcTexCoors(PicSize[0], PicSize[1]);
#endif
		IsChange();
	}
}
//------------------------------------------------------------------------------
#ifdef _GDIPLUS
void CDPanel::SetBorderGradientMode(LinearGradientMode val)
{
	if (m_param.BorderGradientMode != val)
	{
		m_param.BorderGradientMode = val;
		IsChange();
	}
}
//------------------------------------------------------------------------------
void CDPanel::OnDraw()
{// отображение
	CRect rect2;
	if (m_param.bVisible)
	{
		Graphics *pGraphics = *((Graphics**)m_ppGraphics);
		// граница -------------------------------------------------
		if (m_param.BorderHeight > 0)
		{
			DefineRect2(&m_rect, &rect2, m_param.BorderHeight);
			if (m_param.BorderFillType == pftSolid)
			{
				if (m_param.BorderRadius > 0)
					FillRoundRectSolid(pGraphics, &m_rect, m_param.BorderRadius,
						m_param.BorderSolidColor, &m_GraphPath);
				else
					FillRectSolid(pGraphics, &m_rect,
						m_param.BorderSolidColor);
			}
			else// if (m_param.BorderFillType == pftGradient)
			{
				if (m_param.BorderRadius > 0)
					FillRoundRectLinearGradient(pGraphics, &m_rect, m_param.BorderRadius,
						m_param.BorderGradientColorStart, m_param.BorderGradientColorEnd,
						m_param.BorderGradientMode, &m_GraphPath);
				else
					FillRectLinearGradient(pGraphics, &m_rect,
						m_param.BorderGradientColorStart, m_param.BorderGradientColorEnd,
						m_param.BorderGradientMode);
			}
		}
		else
			rect2 = m_rect;	// границы нет

		// само поле -----------------------------------------------
		if (m_param.FieldFillType == pftSolid)
		{
			if (m_param.FieldRadius > 0)
				FillRoundRectSolid(pGraphics, &rect2, m_param.FieldRadius,
					m_param.FieldSolidColor, &m_GraphPath);
			else
				FillRectSolid(pGraphics, &rect2, m_param.FieldSolidColor);
		}
		else// if (m_param.FieldFillType == pftTexture)
		{
			if (m_pImage)
				pGraphics->DrawImage(m_pImage, rect2.left, rect2.top,
										rect2.Width(), rect2.Height() );
		}
	}//	if (m_param.bVisible)
}
//------------------------------------------------------------------------------
#else
void CDPanel::OnDraw()
{// отображение
	CRect rect2 = m_param.FieldRect;
	if (m_param.bVisible)
	{
		glPushAttrib(GL_LIGHTING_BIT | GL_TEXTURE_BIT);

        glMaterialfv(GL_FRONT, GL_AMBIENT,   m_param.mat_ambient);
        glMaterialfv(GL_FRONT, GL_DIFFUSE,   m_param.mat_diffuse);
        glMaterialfv(GL_FRONT, GL_EMISSION,  m_param.mat_emission);
        glMaterialfv(GL_FRONT, GL_SPECULAR,  m_param.mat_specular);
        glMaterialf (GL_FRONT, GL_SHININESS, m_param.mat_shininess);
		// граница -------------------------------------------------
		//if (m_param.BorderHeight > 0)
		//{
		//	glEnable(GL_COLOR_MATERIAL);
		//	DefineRect2(&m_rect, &rect2, m_param.BorderHeight);
		//	if (m_param.BorderFillType == pftSolid)
		//	{
		//		if (m_param.BorderRadius > 0)
		//			FillRoundRectSolid(&m_rect, m_param.CoordZ, m_param.BorderRadius,
		//			m_param.BorderSolidColor);
		//		else
		//			FillRectSolid(&m_rect, m_param.CoordZ, m_param.BorderSolidColor);
		//	}
		//	else// if (m_param.BorderFillType == pftGradient)
		//	{
		//		/*if (m_param.BorderRadius > 0)
		//			FillRoundRectLinearGradient(pGraphics, &m_rect, m_param.BorderRadius,
		//			m_param.BorderGradientColorStart, m_param.BorderGradientColorEnd,
		//			m_param.BorderGradientMode, &m_GraphPath);
		//		else
		//			FillRectLinearGradient(pGraphics, &m_rect,
		//			m_param.BorderGradientColorStart, m_param.BorderGradientColorEnd,
		//			m_param.BorderGradientMode);*/
		//		if (m_param.BorderRadius > 0)
		//			FillRoundRectLinearGradient(&m_rect, m_param.CoordZ, m_param.BorderRadius,
		//			m_param.BorderGradientColorStart, m_param.BorderGradientColorEnd);
		//		else
		//			FillRectLinearGradient(&m_rect, m_param.CoordZ,
		//			m_param.BorderGradientColorStart, m_param.BorderGradientColorEnd);
		//	}
		//	glDisable(GL_COLOR_MATERIAL);
		//}
		//else
		//	rect2 = m_rect;	// границы нет
		// само поле -----------------------------------------------
		if (m_param.FieldFillType == pftSolid)
		{
			rect2.left += 4;
			rect2.right -= 4;
			rect2.top += 4;
			rect2.bottom -= 4;
			if (m_param.FieldRadius > 0)
				FillRoundRectSolid(&rect2, 6.15, m_param.FieldRadius, m_param.FieldSolidColor);
			else
				FillRectSolid(&rect2, 6.15, m_param.FieldSolidColor);
		}
		else// if (m_param.FieldFillType == pftTexture)
		{
			//if (m_pImage)
			//{
			//	glPushMatrix();
			//	glLoadIdentity();
			//	glColor3f(1.0f,1.0f,1.0f);
			//	glEnable(GL_COLOR_MATERIAL);
			//	glEnable(GL_TEXTURE_2D);
			//	glBindTexture(GL_TEXTURE_2D, m_pImage);
			//	glBegin(GL_QUADS);
			//	glTexCoord2f(0, 1);
			//	glVertex3f(rect2.left, rect2.top, m_param.CoordZ);
			//	glTexCoord2f(0, 0);
			//	glVertex3f(rect2.left, rect2.bottom, m_param.CoordZ);
			//	glTexCoord2f(1, 0);
			//	glVertex3f(rect2.right, rect2.bottom, m_param.CoordZ);
			//	glTexCoord2f(1, 1);
			//	glVertex3f(rect2.right, rect2.top, m_param.CoordZ);
			//	glEnd();
			//	glDisable(GL_TEXTURE_2D);
			//	glDisable(GL_COLOR_MATERIAL);
			//	glPopMatrix();
			//}
		}
		glBindTexture(GL_TEXTURE_2D, Texture);
		for (int i = 0; i < 9; i++)
			m_Panel[i]->Draw_texture(m_Panel[i]->ReadFacet(), m_Panel[i]->GetQntPlanes(), PicSize, true);

		glPopAttrib();
	}//	if (m_param.bVisible)
}
//------------------------------------------------------------------------------
void CDPanel::SaveParameters(SimpleXML *pSimpleXML)
{
    CString cString;
    char String[16];

    sprintf_s(String, "%d", m_param.nElement);

    cString = '\0';
    for(int Counter2 = 0, Size = strlen(String); Counter2 < Size; Counter2++)
        cString = cString + String[Counter2];

    hXml hRoot = pSimpleXML->GetRootNode();
    hXml hInterfaceRoot = pSimpleXML->GetChildNode(hRoot);

    hXml hPanelRoot = pSimpleXML->AddElement(L"Panel_"+cString, hInterfaceRoot);

    hXml hTextureRoot = pSimpleXML->AddElement(L"Texture", hPanelRoot);
	pSimpleXML->SetAttributeValue(hTextureRoot, L"ZISNumber_Texture", CComVariant(m_param.Number_Texture));
	pSimpleXML->SetAttributeValue(hTextureRoot, L"path", CComVariant(m_param.FileName));

    hXml hMaterialRoot = pSimpleXML->AddElement(L"Material", hPanelRoot);

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
}
//------------------------------------------------------------------------------
void CDPanel::LoadParameters(SimpleXML *pSimpleXML)
{
    VARIANT vAtrrib;
    CString str = L"";
    CString Pattern = L"Panel_";
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
    pSimpleXML->GetAttributeValue(hTextureRoot, L"ZISNumber_Texture", vAtrrib);
    if (vAtrrib.vt == VT_BSTR)
        if (CString(vAtrrib.bstrVal).GetLength() != 0)
			m_param.Number_Texture = _wtoi(vAtrrib.bstrVal);
    pSimpleXML->GetAttributeValue(hTextureRoot, L"path", vAtrrib);
    if (vAtrrib.vt == VT_BSTR)
        if (CString(vAtrrib.bstrVal).GetLength() != 0)
            SetFieldFileNameTexture(CString(vAtrrib.bstrVal));

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
#endif