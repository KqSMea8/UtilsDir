//------------------------------------------------------------------------------
// DOscillGrid.cpp : файл реализации
//------------------------------------------------------------------------------
#include "stdafx.h"
#include <Interface2012\Draw\DTrendGrid.h>
bool TrendMutex = false;

int g_nTrendGrid = 0;

CDTrendGrid::CDTrendGrid(CRect rect, bool *pFlagIsChange, double *pTimeAnimation) :
CCustomDraw(rect, pFlagIsChange, pTimeAnimation)
{
    m_pParamDraw = &m_param;	// обеспечение ОБЩИХ параметров
    m_param.nElement = ++g_nTrendGrid;
    SetNameWithNumber(L"TrendGrid");
    m_param.m_rect=rect;
	m_rect = rect;
    m_param.bMouseChange = true;
    SetTexture(L"$(ZetToolsDir)Interface2012\\Textures\\Patterns\\metal_mart14.bmp");
    m_param.fontX.SetName("Arialbd");
    m_param.fontX.SetSize(10.0f);
}
//------------------------------------------------------------------------------
CDTrendGrid::~CDTrendGrid()
{
	m_param.Clear();
};
//------------------------------------------------------------------------------
void CDTrendGrid::Resize(CRect rect)
{
	m_param.m_rect = rect;
	m_rect = rect;
}
//------------------------------------------------------------------------------
void CDTrendGrid::SetGridVertex(const std::vector<double> &vertex)
{
    m_param.pVertexs = vertex;
    IsChange();
}
//------------------------------------------------------------------------------
void CDTrendGrid::SetBoldGridVertex(const std::vector<double> &vertex)
{
    m_param.pBoldVertexs = vertex;
    IsChange();
}
//------------------------------------------------------------------------------
void CDTrendGrid::SetUnderTextArray(const std::vector<CString> &textArray)
{
    if(TrendMutex)
        return;
    m_param.pUnderTextMarks = textArray;
    IsChange();
}
//------------------------------------------------------------------------------
void CDTrendGrid::SetUnderTextRect(const std::vector<CRect> &rectArray)
{
    if(TrendMutex)
        return;
    m_param.pUnderTextRect = rectArray;
    IsChange();
}
//------------------------------------------------------------------------------
void CDTrendGrid::SetOverTextArray(const std::vector<CString> &textArray)
{
    if(TrendMutex)
        return;
    m_param.pOverTextMarks = textArray;
    IsChange();
}
//------------------------------------------------------------------------------
void CDTrendGrid::SetOverTextRect(const std::vector<CRect> &rectArray)
{
    if(TrendMutex)
        return;
    m_param.pOverTextRect = rectArray;
    IsChange();
}
//------------------------------------------------------------------------------
void CDTrendGrid::SetTexture(CString path)
{
    if ( !path.IsEmpty() && (m_param.m_textureName != path) )
    {
        m_param.m_textureName = path;
#ifdef _GDIPLUS
        LoadImageFromFile(&m_pImage, str);
#else
        Texture = LoadGLTextures(m_param.m_textureName, PicSize, 0);
#endif	
        IsChange();	
    }
}
//------------------------------------------------------------------------------
void CDTrendGrid::SetAxisTitle(CString title)
{
    m_param.title = title;
}
//------------------------------------------------------------------------------
void CDTrendGrid::SetTitleRect(CRect rect)
{
    m_param.titleRect = rect;
}

//------------------------------------------------------------------------------
void CDTrendGrid::Clear (void)
{

}
//------------------------------------------------------------------------------
void CDTrendGrid::OnDraw()
{
    //DrawBorders();
    glEnable(GL_COLOR_MATERIAL);
    glDisable(GL_LIGHTING);
    //DrawBackGround();
    DrawGrid();
    DrawBoldGrid();
	DrawSignature();
    DrawTextMarks();
    glEnable(GL_LIGHTING);
    glDisable(GL_COLOR_MATERIAL);
}
//------------------------------------------------------------------------------
void CDTrendGrid::DrawGrid()
{
    int matrix[4];
    glEnable(GL_SCISSOR_TEST);
    glPushMatrix();
    glLoadIdentity();
    glColor3ub(m_param.m_gridColor[0], m_param.m_gridColor[1],
        m_param.m_gridColor[2]);
    glPushAttrib(GL_ENABLE_BIT | GL_LIGHTING_BIT);
    glPushClientAttrib(GL_ENABLE_BIT | GL_LIGHTING_BIT);
    glEnableClientState(GL_VERTEX_ARRAY);
    glVertexPointer(3, GL_DOUBLE, 0, (void*)m_param.pVertexs.data());
    glLineWidth(1);
    glGetIntegerv(GL_VIEWPORT, matrix);
    glScissor(m_param.m_rect.left+1, matrix[3]-m_param.m_rect.bottom,
        m_param.m_rect.Width()-1, m_param.m_rect.Height());
    glDrawArrays(GL_LINES, 0, m_param.pVertexs.size()/3);
    glPopClientAttrib();
    glPopAttrib();
    glPopMatrix();
    glDisable(GL_SCISSOR_TEST);
}
//------------------------------------------------------------------------------
void CDTrendGrid::DrawBoldGrid()
{
    int matrix[4];
    glEnable(GL_SCISSOR_TEST);
    glPushMatrix();
    glLoadIdentity();
    glColor3ub(m_param.m_gridColor[0], m_param.m_gridColor[1],m_param.m_gridColor[2]);
    glPushAttrib(GL_ENABLE_BIT | GL_LIGHTING_BIT);
    glPushClientAttrib(GL_ENABLE_BIT | GL_LIGHTING_BIT);
    glEnableClientState(GL_VERTEX_ARRAY);
    glVertexPointer(3, GL_DOUBLE, 0, (void*)m_param.pBoldVertexs.data());
    glLineWidth(2);
    glGetIntegerv(GL_VIEWPORT, matrix);
    glScissor(m_param.m_rect.left+1, matrix[3]-m_param.m_rect.bottom,m_param.m_rect.Width()-1, m_param.m_rect.Height());
    glDrawArrays(GL_LINES, 0, m_param.pVertexs.size()/3);
    glDrawArrays(GL_LINES, 0, m_param.pBoldVertexs.size()/3);
    glPopClientAttrib();
    glPopAttrib();
    glPopMatrix();
    glDisable(GL_SCISSOR_TEST);
}
//------------------------------------------------------------------------------
void CDTrendGrid::DrawBackGround()
{
    glBegin(GL_QUADS);
    glColor3ub(0, 0, 0);
    glVertex3f(m_param.m_rect.left, m_param.m_rect.top, 4);
    glColor3ub(80, 80, 80);
    glVertex3f(m_param.m_rect.left, m_param.m_rect.bottom, 4);
    glVertex3f(m_param.m_rect.right, m_param.m_rect.bottom, 4);
    glColor3ub(0, 0, 0);
    glVertex3f(m_param.m_rect.right, m_param.m_rect.top, 4);
    glEnd();
}
//------------------------------------------------------------------------------
void CDTrendGrid::DrawTextMarks()
{
    unsigned int Counter, QntMarks = m_param.pBoldVertexs.size()/3;
	CString textMarks(L"");
	CRect textRect, cur, prev;
    TrendMutex = true;
    std::vector<CString> copyTextMarks = m_param.pUnderTextMarks;
    std::vector<CRect> copyTextRect = m_param.pUnderTextRect;
    std::vector<CString> _copyTextMarks = m_param.pOverTextMarks;
    std::vector<CRect> _copyTextRect = m_param.pOverTextRect;
    TrendMutex = false;

    for(Counter=0; Counter < _copyTextMarks.size(); ++Counter)
    {
        textMarks = _copyTextMarks[Counter];
        textRect = _copyTextRect[Counter];
        if(textRect.CenterPoint().x > m_param.m_rect.right)
            continue;
		if (textRect.CenterPoint().y < m_param.m_rect.top)
			continue;
        glDisable(GL_DEPTH_TEST);
        m_param.fontX.DrawString(textRect, textMarks);
        if(!copyTextMarks.empty())
        {
            textMarks = copyTextMarks[Counter];
            textRect = copyTextRect[Counter];
            m_param.fontX.DrawString(textRect, textMarks);
        }
        glEnable(GL_DEPTH_TEST);
    }
}
//------------------------------------------------------------------------------
void CDTrendGrid::DrawSignature()
{
    glDisable(GL_DEPTH_TEST);
    m_param.fontX.DrawString(m_param.titleRect, m_param.title);
    glEnable(GL_DEPTH_TEST);
}
//------------------------------------------------------------------------------
void CDTrendGrid::SetTextColor(COLORREF color)
{
	m_param.m_textColor[0] = GetRValue(color);
	m_param.m_textColor[1] = GetGValue(color);
	m_param.m_textColor[2] = GetBValue(color);
	IsChange();
}
//------------------------------------------------------------------------------
void CDTrendGrid::SetGridColor(COLORREF color)
{
	m_param.m_gridColor[0] = GetRValue(color);
	m_param.m_gridColor[1] = GetGValue(color);
	m_param.m_gridColor[2] = GetBValue(color);
	IsChange();
}
//------------------------------------------------------------------------------
void CDTrendGrid::SaveParameters(SimpleXML *pSimpleXML)
{
	CString cString(L"");
	char String[16] =  "" ;

    sprintf_s(String, "%d", m_param.nElement);

    cString = '\0';
    for(int Counter2 = 0, Size = strlen(String); Counter2 < Size; Counter2++)
        cString = cString + String[Counter2];

    hXml hRoot = pSimpleXML->GetRootNode();
    hXml hInterfaceRoot = pSimpleXML->GetChildNode(hRoot);

    hXml hGridRoot = pSimpleXML->AddElement(L"TrendGrid_"+cString, hInterfaceRoot);

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
void CDTrendGrid::LoadParameters(SimpleXML *pSimpleXML)
{
    VARIANT vAtrrib;
	CString str(L"");
    CString Pattern(L"OscillGrid_");
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
    pSimpleXML->GetAttributeValue(hTextureRoot, L"path", vAtrrib);
    if (vAtrrib.vt == VT_BSTR)
        if (CString(vAtrrib.bstrVal).GetLength() != 0)
            SetTexture(CString(vAtrrib.bstrVal));

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

    hXml hGridColorRoot = pSimpleXML->GetNextNode(hBorderRoot);
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

    m_param.fontX.SetColor(RGB(m_param.m_textColor[0], m_param.m_textColor[1], m_param.m_textColor[2]));
    m_param.fontY.SetColor(RGB(m_param.m_textColor[0], m_param.m_textColor[1], m_param.m_textColor[2]));
}
//------------------------------------------------------------------------------