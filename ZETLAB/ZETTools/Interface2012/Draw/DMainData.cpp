//------------------------------------------------------------------------------
// DMainData.cpp : файл реализации
//
//------------------------------------------------------------------------------
#include "stdafx.h"
#include "C:\ZETTools\Interface2012\Draw\DMainData.h"
//------------------------------------------------------------------------------
int g_nMainData = 0;		// счётчик
//------------------------------------------------------------------------------
#ifdef _GDIPLUS
CDMainData::CDMainData(CRect rect, PVOID ppGraphics,
	bool *pFlagIsChange, double *pTimeAnimation) :
CCustomDraw(rect, ppGraphics, pFlagIsChange, pTimeAnimation)
#else
CDMainData::CDMainData(CRect rect,
				bool *pFlagIsChange, double *pTimeAnimation) :
				CCustomDraw(rect, pFlagIsChange, pTimeAnimation)
#endif
{
	m_pParamDraw = &m_param;	// обеспечение ОБЩИХ параметров
	m_param.nElement = ++g_nMainData;
	m_param.Name = L"MainData";
	//TRACE("CREATE CDMainData %s\n", (CStringA)m_pParamDraw->Name);
	SetWidthBorder(4);			// задание m_rect2
	m_param.PointIcon.x = m_rect2.left + 2*m_param.WidthBorder;
	m_param.PointIcon.y = m_rect2.top + 3;// + m_param.WidthBorder;
	// переменные класса
	size = 0;
	Bolder = 40;
    Depth = 30;
#ifdef _GDIPLUS
	m_pImage = NULL;						// FontStyleRegular FontStyleItalic
	m_param.pFont = new CZFont(L"Verdana", 9, FontStyleBold, RGB(0,0,0));	// Arial Vrinda
#else
	m_param.pFont = new CDFonts("Calibri", 20.0 , FontsStyleItalic, RGB(20,120,110));	
#ifdef _ACTIVEX_ZIS
	//центр
	m_pMain[0] = new CDetail(long(24), (m_rect.right + m_rect.left)/2.0, (m_rect.bottom + m_rect.top - Bolder)/2.0, -Depth, 0, m_rect.Width() - 2.0*Bolder, m_rect.Height() - Bolder, Depth);
	//правая часть панели
	m_pMain[1] = new CDetail(long(19), m_rect.right - Bolder/2.0,(m_rect.bottom + m_rect.top - Bolder)/2.0, -Depth, 0, Bolder, m_rect.Height() - Bolder, Depth);
	//правый нижний угол
	m_pMain[2] = new CDetail(long(20), m_rect.right - Bolder/2.0, m_rect.bottom - Bolder/2.0, -Depth, 0, Bolder, Bolder, Depth);
	//нижняя часть панели
	m_pMain[3] = new CDetail(long(21), (m_rect.right + m_rect.left) / 2.0, m_rect.bottom - Bolder / 2.0, -Depth, 0, m_rect.Width() - 2 * Bolder, Bolder, Depth);
	//левый нижний угол
	m_pMain[4] = new CDetail(long(22), Bolder / 2.0 + m_rect.left, m_rect.bottom - Bolder / 2.0, -Depth, 0, Bolder, Bolder, Depth);
	//левая часть панели
	m_pMain[5] = new CDetail(long(23), Bolder / 2.0 + m_rect.left, (m_rect.bottom + m_rect.top - Bolder) / 2.0, -Depth, 0, Bolder, m_rect.Height() - Bolder, Depth);//*/
#else
	//центр
	m_pMain[0] = new CDetail("C:\\ZETTools\\Interface2012\\Models\\Панель\\центр_панелька.stl",
		(m_rect.right + m_rect.left)/2.0, (m_rect.bottom + m_rect.top - Bolder)/2.0, -Depth, 0, 
		m_rect.Width() - 2.0*Bolder, m_rect.Height() - Bolder, Depth);
	//правая часть панели
	m_pMain[1] = new CDetail("C:\\ZETTools\\Interface2012\\Models\\Панель\\правая_панелька.stl",
		m_rect.right - Bolder/2.0,(m_rect.bottom + m_rect.top - Bolder)/2.0, -Depth, 0, 
		Bolder, m_rect.Height() - Bolder, Depth);
	//правый нижний угол
	m_pMain[2] = new CDetail("C:\\ZETTools\\Interface2012\\Models\\Панель\\правый верхний _панелька.stl",
		m_rect.right - Bolder/2.0, m_rect.bottom - Bolder/2.0, -Depth, 0, 
		Bolder, Bolder, Depth);
	//нижняя часть панели
	m_pMain[3] = new CDetail("C:\\ZETTools\\Interface2012\\Models\\Панель\\верх_панелька.stl",
		(m_rect.right + m_rect.left)/2, m_rect.bottom - Bolder/2, -Depth, 0, 
		m_rect.Width() - 2*Bolder, Bolder, Depth);
	//левый нижний угол
	m_pMain[4] = new CDetail("C:\\ZETTools\\Interface2012\\Models\\Панель\\левый верхний _панелька.stl",
		Bolder/2 + m_rect.left, m_rect.bottom - Bolder/2, -Depth, 0, 
		Bolder, Bolder, Depth);
	//левая часть панели
	m_pMain[5] = new CDetail("C:\\ZETTools\\Interface2012\\Models\\Панель\\левая_панелька.stl",
		Bolder/2 + m_rect.left, (m_rect.bottom + m_rect.top - Bolder)/2, -Depth, 0, 
		Bolder, m_rect.Height() - Bolder, Depth);//*/
#endif
	SetImageFileName(_T("C:\\ZETTools\\Interface2012\\Textures\\Patterns\\metal_mart14.bmp"));
#endif
}
//------------------------------------------------------------------------------
void CDMainData::Clear (void)
{
	m_param.pFont->Clear();
	for (int i = 0; i < 6; i++)
		m_pMain[i]->Clear();
}
//------------------------------------------------------------------------------
CDMainData::~CDMainData()
{
	//TRACE("DELETE CDMainData %s\n", (CStringA)m_pParamDraw->Name);
#ifdef _GDIPLUS
	DeleteGdiPlusObject(m_pImage);
	if (m_param.pFont)
		delete m_param.pFont;
#else
	delete m_param.pFont;
	for (int i = 0; i < 6; i++)
		delete m_pMain[i];
#endif
}
//------------------------------------------------------------------------------
void CDMainData::SetTitle(CString str)
{
	if (m_param.Title != str)
	{
		m_param.Title = str;
		IsChange();
	}
}
//------------------------------------------------------------------------------
void CDMainData::SetImageFileName(CString str)
{
	if ( !str.IsEmpty() && (m_param.fileName != str) )
	{
		m_param.fileName = str;
#ifdef _GDIPLUS
		LoadImageFromFile(&m_pImage, str);
#else
		Texture = LoadGLTextures(m_param.fileName, PicSize, m_param.Number_Texture);
        for(int i = 0; i < QNT_DETAILS; i++)
            m_pMain[i]->CalcTexCoors(PicSize[0], PicSize[1]);
#endif	
		IsChange();
	}
}
//------------------------------------------------------------------------------
void CDMainData::SetWidthBorder(int val)
{
	if ( (val >= 0) && (val != m_param.WidthBorder) )
	{
		m_param.WidthBorder = val;
		DefineRect2(&m_rect, &m_rect2, val);
		IsChange();
	}
}
//------------------------------------------------------------------------------
void CDMainData::SetColorBorderStart(COLORREF val)
{
	if (m_param.ColorBorderStart != val)
	{
		m_param.ColorBorderStart = val;
		IsChange();
	}
}
//------------------------------------------------------------------------------
void CDMainData::SetColorBorderEnd(COLORREF val)
{
	if (m_param.ColorBorderEnd != val)
	{
		m_param.ColorBorderEnd = val;
		IsChange();
	}
}
//------------------------------------------------------------------------------
void CDMainData::SetSizeIcon(int val)
{
	if ( (val >= 0) && (val <= 32) && (m_param.SizeIcon != val) )
	{
		m_param.SizeIcon = val;
		IsChange();
	}
}
//------------------------------------------------------------------------------
#ifdef _GDIPLUS
void CDMainData::SetGradientMode(LinearGradientMode val)
{
	if (m_param.GradientMode != val)
	{
		m_param.GradientMode = val;
		IsChange();
	}
}
void CDMainData::OnDraw()
{// Главный контейнер всегда отображается, поэтому без if (m_param.bVisible)
	Graphics *pGraphics = *((Graphics**)m_ppGraphics);
	m_rect2.top = m_rect.top;
	if (m_param.SizeIcon)
		FillRoundRectLinGradDown(pGraphics, &m_rect, 2*m_param.WidthBorder,
		m_param.ColorBorderStart, m_param.ColorBorderEnd,
		m_param.GradientMode, &m_GraphPath);
	else
		FillRectLinearGradient(pGraphics, &m_rect,
		m_param.ColorBorderStart, m_param.ColorBorderEnd,
		m_param.GradientMode);
	RoundRectInGraphicsPathDown(&m_GraphPath, &m_rect2, 2*m_param.WidthBorder);
	if (m_pImage)
	{
		TextureBrush tt_brush(m_pImage);
		pGraphics->FillPath(&tt_brush, &m_GraphPath);
	}
	float left = m_param.PointIcon.x + 1.1f * m_param.SizeIcon;
	DrawString(pGraphics, m_param.Title, m_param.pFont, 
		RectF( left, (float)m_param.PointIcon.y,
		m_rect2.Width() - left - m_param.WidthBorder,
		(float)m_param.SizeIcon ),
		StringAlignmentNear, StringAlignmentCenter);
}
#else
void CDMainData::OnDraw()
{// Главный контейнер всегда отображается, поэтому без if (m_param.bVisible)
	glPushAttrib(GL_LIGHTING_BIT | GL_TEXTURE_BIT);

	glMaterialfv(GL_FRONT, GL_AMBIENT,   m_param.mat_ambient);
	glMaterialfv(GL_FRONT, GL_DIFFUSE,   m_param.mat_diffuse);
	glMaterialfv(GL_FRONT, GL_EMISSION,  m_param.mat_emission);
	glMaterialfv(GL_FRONT, GL_SPECULAR,  m_param.mat_specular);
	glMaterialf (GL_FRONT, GL_SHININESS, m_param.mat_shininess);
	
	glBindTexture(GL_TEXTURE_2D, Texture); 
	for (int i = 0; i < QNT_DETAILS; i++){
        m_pMain[i]->Draw_texture(m_pMain[i]->ReadFacet(), m_pMain[i]->GetQntPlanes(), PicSize, true);
    }
	//m_pMain[QNT_DETAILS-1]->Draw_texture(m_pMain[QNT_DETAILS-1]->ReadFacet(), m_pMain[QNT_DETAILS-1]->GetQntPlanes(), PicSize, true);

	float left = m_param.PointIcon.x + 1.1f * m_param.SizeIcon;
	m_param.pFont->DrawString(CRect( left, (float)m_param.PointIcon.y,
		m_rect2.right - m_param.WidthBorder,
		(float)m_param.PointIcon.y + (float)m_param.SizeIcon ), m_param.Title);

	glPopAttrib();
}	// берётся из ресурсов, о которых данный класс не имеет права ничего знать
//------------------------------------------------------------------------------
void CDMainData::SaveParameters(SimpleXML *pSimpleXML)
{
    CString cString;
    char String[16];

    sprintf_s(String, "%d", m_param.nElement);

    cString = '\0';
    for(int Counter2 = 0, Size = strlen(String); Counter2 < Size; Counter2++)
        cString = cString + String[Counter2];

    hXml hRoot = pSimpleXML->GetRootNode();
    hXml hInterfaceRoot = pSimpleXML->GetChildNode(hRoot);

    hXml hMainPanelRoot = pSimpleXML->AddElement(L"MainData_"+cString, hInterfaceRoot);

    hXml hTextureRoot = pSimpleXML->AddElement(L"Texture", hMainPanelRoot);
	pSimpleXML->SetAttributeValue(hTextureRoot, L"ZISNumber_Texture", CComVariant(m_param.Number_Texture));
    pSimpleXML->SetAttributeValue(hTextureRoot, L"path", CComVariant(m_param.fileName));

    hXml hMaterialRoot = pSimpleXML->AddElement(L"Material", hMainPanelRoot);

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
void CDMainData::LoadParameters(SimpleXML *pSimpleXML)
{
    VARIANT vAtrrib;
    CString str = L"";
    CString Pattern = L"MainData_";
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
void CDMainData::OffDraw()
{
	//SwapBuffers(wglGetCurrentDC());
}
//------------------------------------------------------------------------------
void CDMainData::SetColor(GLfloat *ma, GLfloat *md,	
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