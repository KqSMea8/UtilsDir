//------------------------------------------------------------------------------
// DButtonHMME.cpp : файл реализации
//
//------------------------------------------------------------------------------
#include "stdafx.h"
#include <Interface2012\Draw\DButtonHMME.h>
#include "stdio.h"
//------------------------------------------------------------------------------
int g_nButtonHMME = 0;		// счётчик кнопок
//------------------------------------------------------------------------------
#ifdef _GDIPLUS
CDButtonHMME::CDButtonHMME(CRect rect, PVOID ppGraphics,
	bool *pFlagIsChange, double *pTimeAnimation) :
CCustomDraw(rect, ppGraphics, pFlagIsChange, pTimeAnimation)
#else
CDButtonHMME::CDButtonHMME(CRect rect, bool *pFlagIsChange, double *pTimeAnimation, TypeButtonHMME codeButton) :
CCustomDraw(rect, pFlagIsChange, pTimeAnimation)
#endif
{
	m_pParamDraw = &m_param;	// обеспечение ОБЩИХ параметров
	// изменение значений общих параметров, заданных по умолчанию
	m_param.nElement = ++g_nButtonHMME;
	SetNameWithNumber(L"ButtonHMME");
	//TRACE("CREATE CDButtonHMME %s\n", (CStringA)m_pParamDraw->Name);
	m_param.bMouseChange = true;
	m_bFlag = false;
	m_param.CodeButton = codeButton;
	// переменные класса
#ifdef _GDIPLUS
	m_pImage_Cur =
	m_pImage_s =
	m_pImage_m =
	m_pImage_p = NULL;
#else
#ifdef _ACTIVEX_ZIS
		m_ButtonHMME = new CDetail(long(13), (m_rect.right + m_rect.left)/2.0, (m_rect.bottom + m_rect.top)/2.0, 0, 180, m_rect.right - m_rect.left, m_rect.bottom - m_rect.top,0.1);//*/
#else
		m_ButtonHMME = new CDetail("$(ZetToolsDir)Interface2012\\Models\\Кнопка HMME.stl",
			(m_rect.right + m_rect.left) / 2.0, (m_rect.bottom + m_rect.top) / 2.0, 0, 180,
			m_rect.right - m_rect.left, m_rect.bottom - m_rect.top, 0.1);//*/

#endif
		SetImageFileName(_T("$(ZetToolsDir)Interface2012\\Textures\\texture15"));
#endif
}
//------------------------------------------------------------------------------
void CDButtonHMME::Clear (void)
{
	m_ButtonHMME->Clear();
}
//------------------------------------------------------------------------------
CDButtonHMME::~CDButtonHMME()
{
	//TRACE("DELETE CDButtonHMME %s\n", (CStringA)m_pParamDraw->Name);
#ifdef _GDIPLUS
	DeleteGdiPlusObject(m_pImage_s);
	DeleteGdiPlusObject(m_pImage_m);
	DeleteGdiPlusObject(m_pImage_p);
#else
	//m_ButtonHMME->Clear();
	delete m_ButtonHMME;
	m_ButtonHMME = nullptr;
#endif
}
//------------------------------------------------------------------------------
void CDButtonHMME::Resize(CRect rect)
{
	m_rect = rect;
	m_ButtonHMME->Resize((m_rect.right + m_rect.left) / 2.0, (m_rect.bottom + m_rect.top) / 2.0, 0, 180,
		m_rect.right - m_rect.left, m_rect.bottom - m_rect.top, 0.1);
}
//------------------------------------------------------------------------------
void CDButtonHMME::SetImageFileName(CString str)
{
	if ( !str.IsEmpty() && (m_param.fileName != str) )
	{
#ifdef _GDIPLUS
		LoadImageFromFile(&m_pImage_s, str + L"_s");
		LoadImageFromFile(&m_pImage_m, str + L"_m");
		LoadImageFromFile(&m_pImage_p, str + L"_p");
#else
		m_param.fileName = str + _T(".bmp");
		long number(0);
		if (m_param.CodeButton == tbHMME_Help) number = 0;
		if (m_param.CodeButton == tbHMME_Min) number = 1;
		if (m_param.CodeButton == tbHMME_End) number = 2;
		Texture = LoadGLTextures(m_param.fileName, PicSize , m_param.Number_Texture[number]);
        m_ButtonHMME->CalcTexCoordHMME(PicSize[0], PicSize[1]);
#endif
		IsChange();
	}
}
//------------------------------------------------------------------------------
void CDButtonHMME::SetTimeAnimation(double val)
{
	if (val > 0.)
		m_param.dTimeAnimation = val;
}
//------------------------------------------------------------------------------
void CDButtonHMME::OnLButtonClk(UINT nFlags, CPoint point)
{// Начало анимации
	m_param.bAnimation = true;
	m_param.dTimeCur = 0.;
	m_param.dTimeStart = GetTime();
}
//------------------------------------------------------------------------------
void CDButtonHMME::DoProcess()
{
	if (m_param.bAnimation)
	{
		m_param.dTimeCur = GetTime() - m_param.dTimeStart;
		IsChange();
	}
}
//------------------------------------------------------------------------------
void CDButtonHMME::OnMouseEnter()
{
	m_bFlag = true;
	if (m_param.CodeButton != tbHMME_End)
	{
		m_param.mat_ambient[0] += 1.f/255.f;
		m_param.mat_ambient[1] += 203.f/255.f;
		m_param.mat_ambient[2] += 247.f/255.f;
	}
	else
	{
		m_param.MouseIsEnter = TRUE;
		m_param.mat_ambient[0] += 203.f/255.f;
		m_param.mat_ambient[1] += 1.f/255.f;
		m_param.mat_ambient[2] += 1.f/255.f;
	}
	CCustomDraw::OnMouseEnter();
}
//------------------------------------------------------------------------------
void CDButtonHMME::OnMouseLeave()
{
	m_bFlag = false;
	if (m_param.CodeButton != tbHMME_End)
	{
		m_param.mat_ambient[0] -= 1.f/255.f;
		m_param.mat_ambient[1] -= 203.f/255.f;
		m_param.mat_ambient[2] -= 247.f/255.f;
	}
	else
	{
		m_param.MouseIsEnter = FALSE;
		m_param.mat_ambient[0] -= 203.f/255.f;
		m_param.mat_ambient[1] -= 1.f/255.f;
		m_param.mat_ambient[2] -= 1.f/255.f;
	}
	CCustomDraw::OnMouseLeave();
}
//------------------------------------------------------------------------------
#ifdef _GDIPLUS
void CDButtonHMME::OnDraw()
{// Служебные кнопки всегда отображаются, поэтому без if (m_param.bVisible)
	// определяю что будем отображать
	if (m_param.bAnimation)
	{
		if (m_param.dTimeCur < m_param.dTimeAnimation)
			m_pImage_Cur = m_pImage_p;
		else
		{
			if (m_param.bMouseCome)
				m_pImage_Cur = m_pImage_m;
			else
				m_pImage_Cur = m_pImage_s;
			m_param.bAnimation = false;
		}
	}
	else
	{
		if (m_param.bMouseCome)
			m_pImage_Cur = m_pImage_m;
		else
			m_pImage_Cur = m_pImage_s;
	}
	// отображение
	Graphics *pGraphics = *((Graphics**)m_ppGraphics);
	pGraphics->DrawImage(m_pImage_Cur,
		m_rect.left, m_rect.top, m_rect.Width(), m_rect.Height() );
}
#else
//------------------------------------------------------------------------------
void CDButtonHMME::OnDraw()
{// Служебные кнопки всегда отображаются, поэтому без if (m_param.bVisible)
	// определяю что будем отображать
	glColor3f(1.0f,1.0f,1.0f);

	glMaterialfv(GL_FRONT, GL_AMBIENT, m_param.mat_ambient);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, m_param.mat_diffuse);
	glMaterialfv(GL_FRONT, GL_EMISSION, m_param.mat_emission);
	glMaterialfv(GL_FRONT, GL_SPECULAR, m_param.mat_specular);
	glMaterialf(GL_FRONT, GL_SHININESS, m_param.mat_shininess);
	/*if (m_param.bMouseCome)
		glEnable(GL_COLOR_MATERIAL);
	m_ButtonHMME->Draw(m_ButtonHMME->ReadFacet(), m_ButtonHMME->GetQntPlanes());
	if (m_param.bMouseCome)
		glDisable(GL_COLOR_MATERIAL);*/
	//if (m_param.bMouseCome)
	//	glEnable(GL_COLOR_MATERIAL);
	//glEnable(GL_TEXTURE_2D);
	//glBlendFunc(GL_SRC_ALPHA,GL_ONE);
	//glBindTexture(GL_TEXTURE_2D, Texture);               // Select Our Texture
	//glBegin(GL_QUADS);

	//glTexCoord2f(0, 1);
	//glVertex3f(m_rect.left, m_rect.top, 0);
	//glTexCoord2f(0, 0);
	//glVertex3f(m_rect.left, m_rect.bottom, 0);
	//glTexCoord2f(1, 0);
	//glVertex3f(m_rect.right, m_rect.bottom, 0);
	//glTexCoord2f(1, 1);
	//glVertex3f(m_rect.right, m_rect.top, 0);
	//glEnd();

	glPushAttrib(GL_ENABLE_BIT | GL_TEXTURE_BIT);
	//glEnable(GL_TEXTURE_2D);
//	if (m_bFlag)
//		glEnable(GL_BLEND);
//	else
//		glDisable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA,GL_ONE);
	glBindTexture(GL_TEXTURE_2D, Texture); 
	//m_ButtonHMME->Draw_texture(m_ButtonHMME->ReadFacet(), m_ButtonHMME->GetQntPlanes(),PicSize);
	m_ButtonHMME->Draw_HMME(m_ButtonHMME->ReadFacet(), m_ButtonHMME->GetQntPlanes(),PicSize);
	//glDisable(GL_TEXTURE_2D);
	if (m_bFlag)
		glDisable(GL_BLEND);
	//else
		//glEnable(GL_BLEND);
	//glDisable(GL_TEXTURE_2D);
	//if (m_param.bMouseCome)
	//	glDisable(GL_COLOR_MATERIAL);
	//glDisable(GL_BLEND); 
	//glFinish();
	glPopAttrib();
}
//------------------------------------------------------------------------------
void CDButtonHMME::SaveParameters(SimpleXML *pSimpleXML)
{
	CString cString(L"");
	char String[16] =  "" ;

    sprintf_s(String, "%d", m_param.nElement);

    cString = CString(String);
    hXml hRoot = pSimpleXML->GetRootNode();
    hXml hInterfaceRoot = pSimpleXML->GetChildNode(hRoot);
    hXml hHMMERoot = pSimpleXML->AddElement(L"ButtonHMME_"+cString, hInterfaceRoot);//*/
	hXml hTextureRoot = pSimpleXML->AddElement(L"Texture", hHMMERoot);

	if (m_param.CodeButton == tbHMME_Help)
	{
		pSimpleXML->SetAttributeValue(hTextureRoot, L"ZISNumber_Texture", CComVariant(m_param.Number_Texture[0]));
		pSimpleXML->SetAttributeValue(hTextureRoot, L"path", CComVariant(m_param.Texture_1_FileName));
	}
	if (m_param.CodeButton == tbHMME_Min)
	{
		pSimpleXML->SetAttributeValue(hTextureRoot, L"ZISNumber_Texture", CComVariant(m_param.Number_Texture[1]));
		pSimpleXML->SetAttributeValue(hTextureRoot, L"path", CComVariant(m_param.Texture_2_FileName));
	}
	if (m_param.CodeButton == tbHMME_Max)
	{
		pSimpleXML->SetAttributeValue(hTextureRoot, L"ZISNumber_Texture", CComVariant(m_param.Number_Texture[3]));
		pSimpleXML->SetAttributeValue(hTextureRoot, L"path", CComVariant(m_param.Texture_3_FileName));
	}
	if (m_param.CodeButton == tbHMME_End)
	{
		// при закрытии программы через кнопке End свойства материалов подсвечены по этому вызывается OnMouseLeave()
		// чтобы привести значения свойств к первоначальному виду
		if (m_param.MouseIsEnter != FALSE) 
			OnMouseLeave();
		pSimpleXML->SetAttributeValue(hTextureRoot, L"ZISNumber_Texture", CComVariant(m_param.Number_Texture[2]));
		pSimpleXML->SetAttributeValue(hTextureRoot, L"path", CComVariant(m_param.Texture_4_FileName));
	}
    hXml hMaterialRoot = pSimpleXML->AddElement(L"Material", hTextureRoot);

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
void CDButtonHMME::LoadParameters(SimpleXML *pSimpleXML)
{
    VARIANT vAtrrib;
    CString str(L"");
    CString Pattern(L"ButtonHMME_");
	char String[16] =  "" ;
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
		if (str.IsEmpty())
            return;
    }
	hXml hTextureRoot = pSimpleXML->GetChildNode(hElement);
	pSimpleXML->GetAttributeValue(hTextureRoot, L"ZISNumber_Texture1", vAtrrib);
	if (vAtrrib.vt == VT_BSTR)
		if (CString(vAtrrib.bstrVal).GetLength() != 0)
			m_param.Number_Texture[0] = _wtoi(vAtrrib.bstrVal);
	pSimpleXML->GetAttributeValue(hTextureRoot, L"path1", vAtrrib);
	if (vAtrrib.vt == VT_BSTR)
		if (CString(vAtrrib.bstrVal).GetLength() != 0)
			SetImageFileName(CString(vAtrrib.bstrVal));
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