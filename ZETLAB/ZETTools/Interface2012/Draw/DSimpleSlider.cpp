//------------------------------------------------------------------------------
// DSimpleSlider.cpp : файл реализации
// Прямоугольник элемента в процессе работы не меняется, поэтому положение
// (горизонтальное или вертикальное) определяется один раз в конструкторе.
//------------------------------------------------------------------------------
#include "stdafx.h"
#include "C:\ZETTools\Interface2012\Draw\DSimpleSlider.h"

//------------------------------------------------------------------------------
int g_nSimpleSlider = 0;		// счётчик статических картинок
//------------------------------------------------------------------------------
#ifdef _GDIPLUS
CDSimpleSlider::CDSimpleSlider(CRect rect, PVOID ppGraphics,
	bool *pFlagIsChange, double *pTimeAnimation) :
	CCustomDraw(rect, ppGraphics, pFlagIsChange, pTimeAnimation)
#else
CDSimpleSlider::CDSimpleSlider(CRect rect,	bool *pFlagIsChange, 
	double *pTimeAnimation) :
	CCustomDraw(rect, pFlagIsChange, pTimeAnimation)
#endif
{
	m_pParamDraw = &m_param;	// обеспечение ОБЩИХ параметров
	m_param.nElement = ++g_nSimpleSlider;
	SetNameWithNumber(L"SimpleSlider");
	//TRACE("CREATE CDSimpleSlider %s\n", (CStringA)m_pParamDraw->Name);
	if ( m_rect.Width() > m_rect.Height() )
		m_param.PositionType = tpHorizontal;
	else
		m_param.PositionType = tpVertical;
	m_param.bMouseChange = true;
	// переменные класса
	//m_param.bVisible = false;
	m_bFlag = false;
	m_bEvent = m_bFlag = false;
	m_nStart = 0;
	m_rect2 = rect;
#ifdef _GDIPLUS
	m_pImage_Field = NULL;
	m_pImage_Cursor = NULL;
	m_pImage_Cursor_s = NULL;
	m_pImage_Cursor_m = NULL;
	m_pImage_Cursor_p = NULL;
#else
	//передаются координаты центра области
	float Rotate, /*CenterX[4], CenterY[4],*/ R=180;
	float CenterX[4], CenterY[4];
	DefineRectAll();
	if (m_param.PositionType == tpHorizontal){
		Rotate = 0;
		CenterY[0] = CenterY[1] = CenterY[2] = CenterY[3] = (rect.bottom + rect.top)/2.0;
		CenterX[0] = (rect.left + rect.Height()/4.0);
		CenterX[1] = (rect.left + rect.right)/2.0;
		CenterX[2] = (rect.right - rect.Height()/4.0);
		CenterX[3] = (m_rectC.left + m_rectC.right)/2.0;

#ifdef _ACTIVEX_ZIS
        Slider[0] = new CDetail(long(36), CenterX[0], CenterY[0], 0, Rotate, rect.Height() / 2.f, rect.Height(), rect.Height());
        Slider[1] = new CDetail(long(37), CenterX[1], CenterY[1], 0, Rotate, rect.Width() - rect.Height(), rect.Height(), rect.Height());
        Slider[2] = new CDetail(long(38), CenterX[2], CenterY[2], 0, Rotate, rect.Height() / 2.f, rect.Height(), rect.Height());
        Slider[3] = new CDetail(long(39), CenterX[3], CenterY[3], 0, Rotate, rect.Height() - 2, rect.Height() - 2, rect.Height());//*/
#else
		Slider[0] = new CDetail("C:\\ZETTools\\Interface2012\\Models\\Слайдер\\Граница ползунка L 6,205х12,235х5.stl",CenterX[0], CenterY[0], 0, Rotate, rect.Height() / 2.f, rect.Height(), rect.Height());
        Slider[1] = new CDetail("C:\\ZETTools\\Interface2012\\Models\\Слайдер\\Основание ползунка 30х12,235х5.stl",CenterX[1], CenterY[1], 0, Rotate, rect.Width() - rect.Height(), rect.Height(), rect.Height());
        Slider[2] = new CDetail("C:\\ZETTools\\Interface2012\\Models\\Слайдер\\Граница ползунка R 6,205х12,235х5.stl",CenterX[2], CenterY[2], 0, Rotate, rect.Height() / 2.f, rect.Height(), rect.Height());
        Slider[3] = new CDetail("C:\\ZETTools\\Interface2012\\Models\\Слайдер\\Ручка ползунка.stl",CenterX[3], CenterY[3], 0, Rotate, rect.Height() - 2, rect.Height() - 2, rect.Height());//*/
#endif

		SetFileNameCursor(m_param.FileNameCursor1, Rotate);
	}
	if (m_param.PositionType==tpVertical){
		Rotate=90;
		CenterX[0]=CenterX[1]=CenterX[2]=CenterX[3]=(rect.left+rect.right)/2.0f;
		CenterY[0]=(2*rect.top+rect.Width()/2.0f)/2.0f;
		CenterY[1]=(rect.bottom+rect.top)/2.0f;
		CenterY[2]=(2*rect.bottom-rect.Width()/2.0f)/2.0f;
		CenterY[3]=(m_rectC.top+m_rectC.bottom)/2.0f;

#ifdef _ACTIVEX_ZIS
        Slider[0] = new CDetail(long(36), CenterX[0], CenterY[0], 0, Rotate, rect.Width(), rect.Width() / 2.f, rect.Width());
        Slider[1] = new CDetail(long(37), CenterX[1], CenterY[1], 0, Rotate, rect.Width(), rect.Width() - rect.Height(), rect.Width());
        Slider[2] = new CDetail(long(38), CenterX[2], CenterY[2], 0, Rotate, rect.Width(), rect.Width() / 2.f, rect.Width());
        Slider[3] = new CDetail(long(39) ,CenterX[3], CenterY[3], 0, Rotate, rect.Width() - 2, rect.Width() - 2, rect.Width());//*/
#else
		Slider[0] = new CDetail("C:\\ZETTools\\Interface2012\\Models\\Слайдер\\Граница ползунка L 6,205х12,235х5.stl",CenterX[0], CenterY[0], 0, Rotate, rect.Width(), rect.Width() / 2.f, rect.Width());
        Slider[1] = new CDetail("C:\\ZETTools\\Interface2012\\Models\\Слайдер\\Основание ползунка 30х12,235х5.stl",CenterX[1], CenterY[1], 0, Rotate, rect.Width(), rect.Width() - rect.Height(), rect.Width());
        Slider[2] = new CDetail("C:\\ZETTools\\Interface2012\\Models\\Слайдер\\Граница ползунка R 6,205х12,235х5.stl",CenterX[2], CenterY[2], 0, Rotate, rect.Width(), rect.Width() / 2.f, rect.Width());
        Slider[3] = new CDetail("C:\\ZETTools\\Interface2012\\Models\\Слайдер\\Ручка ползунка.stl",CenterX[3], CenterY[3], 0, Rotate, rect.Width() - 2, rect.Width() - 2, rect.Width());//*/
#endif

		SetFileNameCursor(m_param.FileNameCursor2, Rotate);
	}
    m_param.Rotate = Rotate;
	SetFileNameField(m_param.FileNameField, Rotate);
#endif
}
//------------------------------------------------------------------------------
void CDSimpleSlider::Clear(void)
{
	for (int i = 0; i < 4; i++)
		Slider[i]->Clear();
}
//------------------------------------------------------------------------------
CDSimpleSlider::~CDSimpleSlider()
{
	//TRACE("DELETE CDSimpleSlider %s\n", (CStringA)m_pParamDraw->Name);
#ifdef _GDIPLUS
	DeleteGdiPlusObject(m_pImage_Field);
	DeleteGdiPlusObject(m_pImage_Cursor_s);
	DeleteGdiPlusObject(m_pImage_Cursor_m);
	DeleteGdiPlusObject(m_pImage_Cursor_p);
#else
	for (int i = 0; i < QNT_DETAILS; i++)
		delete Slider[i];
#endif
}
//------------------------------------------------------------------------------
void CDSimpleSlider::DefineRectAll()
{
	if (m_param.PositionType == tpHorizontal)
	{
		m_rect0.top = m_rect1.top = m_rectC.top =
			m_rect.top + m_param.deltaY;
		m_rect0.bottom = m_rect1.bottom = m_rectC.bottom =
			m_rect.bottom - m_param.deltaY;
		m_SizeCursor = m_rect0.bottom - m_rect0.top;

		m_rect0.left = m_rect.left + m_param.deltaX;
		m_rect0.right = m_rect0.left + m_SizeCursor;

		m_rect1.right = m_rect.right - m_param.deltaX;
		m_rect1.left = m_rect1.right - m_SizeCursor;

		m_LengthValue = m_rect1.left - m_rect0.left;
	}
	else
	{
		m_rect0.left = m_rect1.left = m_rectC.left =
			m_rect.left + m_param.deltaY;
		m_rect0.right = m_rect1.right = m_rectC.right =
			m_rect.right - m_param.deltaY;
		m_SizeCursor = m_rect0.right - m_rect0.left;

		m_rect0.top = m_rect.top + m_param.deltaX;
		m_rect0.bottom = m_rect0.top + m_SizeCursor;

		m_rect1.bottom = m_rect.bottom - m_param.deltaX;
		m_rect1.top = m_rect1.bottom - m_SizeCursor;

		m_LengthValue = m_rect1.top - m_rect0.top;
	}
	DefineRectCur();
}
//------------------------------------------------------------------------------
void CDSimpleSlider::DefineRectCur()
{
	int num;
	if (m_param.fValueCur < 1.f)
		num = int(m_param.fValueCur * m_LengthValue);
	else
		num = m_LengthValue;
	if (m_param.PositionType == tpHorizontal)
	{
		m_rectC.left = m_rect0.left + num;
		m_rectC.right = m_rectC.left + m_SizeCursor;
	}
	else
	{  //m_param.PositionType == tpVertical
		m_rectC.top = m_rect0.top + num;
		m_rectC.bottom = m_rectC.top + m_SizeCursor;
	}
}
//------------------------------------------------------------------------------
bool CDSimpleSlider::CenterAroundZero()
{
	bool ret;
	if (m_param.PositionType == tpHorizontal)
		ret = (m_rectC.left - m_rect0.left) > ((m_rect1.left - m_rect0.left) / 2);
	else  //m_param.PositionType == tpVertical
		ret = (m_rectC.bottom - m_rect0.bottom) >
				((m_rect1.bottom - m_rect0.bottom) / 2);
	return ret;
}
//------------------------------------------------------------------------------
void CDSimpleSlider::SetPositionValue(BOOL val)
{
	if (m_param.PositionValue != val)
	{
		CPoint point;
		if (val)
			point = m_rect1.CenterPoint();
		else
			point = m_rect0.CenterPoint();
		OnLButtonClk(0, point);
		m_bEvent = false;	// этот флаг надо сбросить, т.к. событие будет
	}	// сгенерировано в CSimpleSlider::SetPositionValue
}
//------------------------------------------------------------------------------
void CDSimpleSlider::SetDeltaX(int val)
{
	if ( (val > 0) && (m_param.deltaX != val) )
	{
		m_param.deltaX = val;
		DefineRectAll();
		IsChange();
	}
}
//------------------------------------------------------------------------------
void CDSimpleSlider::SetDeltaY(int val)
{
	if ( (val > 0.f) && (m_param.deltaY != val) )
	{
		m_param.deltaY = val;
		DefineRectAll();
		IsChange();
	}
}
//------------------------------------------------------------------------------
void CDSimpleSlider::SetFileNameField(CString str, double angle)
{
	if ( !str.IsEmpty() )
	{
#ifdef _GDIPLUS
		LoadImageFromFile(&m_pImage_Field, str);
#else
        Texture = LoadGLTextures(m_param.FileNameField, PicSize, m_param.Number_Texture[0]);
        for(int Counter=0; Counter<QNT_DETAILS-1; Counter++){
            Slider[Counter]->CalcTexCoors(PicSize[0], PicSize[1], angle);
        }
#endif
		IsChange();
	}
}
//------------------------------------------------------------------------------
void CDSimpleSlider::SetFileNameCursor(CString str, double angle)
{
	if (!str.IsEmpty() )
	{
#ifdef _GDIPLUS
		LoadImageFromFile(&m_pImage_Cursor_s, str + L"_s");
		LoadImageFromFile(&m_pImage_Cursor_m, str + L"_m");
		LoadImageFromFile(&m_pImage_Cursor_p, str + L"_p");
		m_pImage_Cursor = m_pImage_Cursor_s;
#else
//        Texture1 = LoadGLTextures(m_param.FileNameCursor, PicSize1, 4);
		if (m_param.PositionType == tpHorizontal)
			Texture1 = LoadGLTextures(m_param.FileNameCursor1, PicSize1, m_param.Number_Texture[1]);
		if (m_param.PositionType==tpVertical)
			Texture1 = LoadGLTextures(m_param.FileNameCursor2, PicSize1, m_param.Number_Texture[2]);
        Slider[3]->CalcTexCoordHMME(PicSize1[0], PicSize1[1], angle+180);
#endif
		IsChange();
	}
}
//------------------------------------------------------------------------------
bool CDSimpleSlider::GetNeedEvent()
{
	bool ret = m_bEvent;
	m_bEvent = false;
	return ret;
}
//------------------------------------------------------------------------------
void CDSimpleSlider::SetTimeAnimation(double val)
{
	if (val > 0.)
		m_param.dTimeAnimation = val;
}
//------------------------------------------------------------------------------
void CDSimpleSlider::DoProcess()
{
	if (m_param.bAnimation)
	{
		m_param.fValueCur =
			float( (GetTime() - dTimeStart) / m_param.dTimeAnimation );
		if (m_param.fValueCur > 1.f)
			m_param.fValueCur = 1.f;

		if (m_param.PositionValue == 1)
		{// движение из положения 0 в положение 1
			if (m_param.fValueCur == 1.f)
				m_param.bAnimation = false;
		}
		else	// m_param.PositionValue == 1
		{// движение из положения 1 в положение 0
			if (m_param.fValueCur == 1.f)
				m_param.bAnimation = false;
			m_param.fValueCur = 1.f - m_param.fValueCur;
		}
		DefineRectCur();
		IsChange();
	}
}
//------------------------------------------------------------------------------
void CDSimpleSlider::OnMouseEnter()
{
#ifdef _GDIPLUS
	m_pImage_Cursor = m_pImage_Cursor_m;
#endif
	m_param.mat_ambient_b[0] = 0.25f;
	m_param.mat_ambient_b[1] = 0.25f;
	m_param.mat_ambient_b[2] = 0.25f;
	m_param.mat_emission_b[0] = 90.f/255.f;
	m_param.mat_emission_b[1] = 115.f/255.f;
	m_param.mat_emission_b[2] = 115.f/255.f;
	CCustomDraw::OnMouseEnter();
}
//------------------------------------------------------------------------------
void CDSimpleSlider::OnMouseLeave()
{
#ifdef _GDIPLUS
	m_pImage_Cursor = m_pImage_Cursor_s;
#endif	
	if (m_bFlag)
	{
		NotPull();
		m_bFlag = false;
	}
	else
	{
		if (m_param.PositionValue == 0)
			m_rectC = m_rect0;
		else
			m_rectC = m_rect1;
	}
	m_param.mat_ambient_b[0] = 0.25f;
	m_param.mat_ambient_b[1] = 0.25f;
	m_param.mat_ambient_b[2] = 0.25f;
	m_param.mat_emission_b[0] = 0.2f;
	m_param.mat_emission_b[1] = 0.2f;
	m_param.mat_emission_b[2] = 0.2f;
	CCustomDraw::OnMouseLeave();
}
//------------------------------------------------------------------------------
void CDSimpleSlider::OnLButtonClk(UINT nFlags, CPoint point)
{
	bool flag = false;
	if (m_param.PositionValue)
	{//PositionValue == 1
		if ( PointInRect(point, &m_rect0) )
			m_bEvent = true;
	}
	else
	{//PositionValue == 0
		if ( PointInRect(point, &m_rect1) )
			m_bEvent = true;
	}
	if (m_bEvent)
	{
		m_param.PositionValue = -m_param.PositionValue + 1;
		m_param.bAnimation = true;
		dTimeStart = GetTime();
		IsChange();
	}
#ifdef _GDIPLUS
#else
	/*CString str;
	CFileDialog fileDialog(TRUE,NULL,_T("*.stl"));	// объект класса выбора файла
	TRACE("CDSimpleSlider::OnLButtonClk int result = fileDialog.DoModal();\n");
	int result = fileDialog.DoModal();			// запустить диалоговое окно
	if (result==IDOK)							// если файл выбран
	{
		str = fileDialog.GetPathName(); // показать полный путь
		int iNeeded = WideCharToMultiByte( CP_ACP, 0, str, -1, NULL, 0, NULL, NULL);
		char* psz = new char[iNeeded];
		WideCharToMultiByte( CP_ACP, 0, str, -1, psz, iNeeded, NULL, NULL);
		Slider[0]=new CDetail(psz, (m_rect.left+m_rect.right)/2, (m_rect.bottom+m_rect.top)/2, 0, 1, m_rect.right-m_rect.left, m_rect.bottom-m_rect.top, 5);
	}*/
#endif
}
//------------------------------------------------------------------------------
void CDSimpleSlider::OnLButtonDown(UINT nFlags, CPoint point)
{
	if ( PointInRect(point, &m_rectC) )
	{
#ifdef _GDIPLUS
		m_pImage_Cursor = m_pImage_Cursor_p;
#endif
		if (m_param.PositionType == tpHorizontal)
			m_nStart = point.x;
		else
			m_nStart = point.y;
		m_bFlag = true;
		IsChange();
	}
}
//------------------------------------------------------------------------------
void CDSimpleSlider::NotPull()
{// Тянул мышь, но не дотянул! Но если середину прошёл, то считаем, что дотянул!
	if (m_param.PositionValue)
	{	// 1
		if ( !CenterAroundZero() )
		{
			m_param.PositionValue = 0;
			m_rectC = m_rect0;
			m_bEvent = true;
		}
		else
			m_rectC = m_rect1;
	}
	else
	{	// 0
		if ( CenterAroundZero() )
		{
			m_param.PositionValue = 1;
			m_rectC = m_rect1;
			m_bEvent = true;
		}
		else
			m_rectC = m_rect0;
	}
}
//------------------------------------------------------------------------------
void CDSimpleSlider::OnLButtonUp(UINT nFlags, CPoint point)
{
#ifdef _GDIPLUS
	m_pImage_Cursor = m_pImage_Cursor_m;
#endif
	if (m_bFlag)
	{
		NotPull();
		m_bFlag = false;
		IsChange();
	}
}
//------------------------------------------------------------------------------
void CDSimpleSlider::OnMouseMove(UINT nFlags, CPoint point)
{
	if (m_bFlag)
	{
		if (m_param.PositionType == tpHorizontal)
		{
			if (m_param.PositionValue)
				m_param.fValueCur = 1.f - float(m_nStart - point.x) / m_LengthValue;
			else
				m_param.fValueCur = float(point.x - m_nStart) / m_LengthValue;
		}
		else
		{
			if (m_param.PositionValue)
				m_param.fValueCur = 1.f - float(m_nStart - point.y) / m_LengthValue;
			else
				m_param.fValueCur = float(point.y - m_nStart) / m_LengthValue;
		}
		if (m_param.fValueCur < 0.f)
			m_param.fValueCur = 0.f;
		else if (m_param.fValueCur > 1.f)
			m_param.fValueCur = 1.f;
		DefineRectCur();
		IsChange();
	}
}
#ifdef _GDIPLUS
void CDSimpleSlider::OnDraw()
{// отображение
	if (m_param.bVisible)
	{
		Graphics *pGraphics = *((Graphics**)m_ppGraphics);
		// поле слайдера
		pGraphics->DrawImage( m_pImage_Field, m_rect.left, m_rect.top,
			m_rect.Width(), m_rect.Height() );
		// курсор
		pGraphics->DrawImage( m_pImage_Cursor, m_rectC.left, m_rectC.top,
			m_SizeCursor, m_SizeCursor );
	}
}
#else
//------------------------------------------------------------------------------
void CDSimpleSlider::OnDraw()
{// отображение
	if (m_param.bVisible)
	{
		glPushAttrib(GL_LIGHTING_BIT | GL_TEXTURE_BIT);

		glMaterialfv(GL_FRONT, GL_AMBIENT,   m_param.mat_ambient);
		glMaterialfv(GL_FRONT, GL_DIFFUSE,   m_param.mat_diffuse);
		glMaterialfv(GL_FRONT, GL_EMISSION,  m_param.mat_emission);
		glMaterialfv(GL_FRONT, GL_SPECULAR,  m_param.mat_specular);
		glMaterialf (GL_FRONT, GL_SHININESS, m_param.mat_shininess);

        glBindTexture(GL_TEXTURE_2D, Texture);
		for(int Counter=0; Counter<QNT_DETAILS-1; Counter++){
			Slider[Counter]->Draw_texture(Slider[Counter]->ReadFacet(), Slider[Counter]->GetQntPlanes(), PicSize, true);
        }

		glMaterialfv(GL_FRONT, GL_AMBIENT,   m_param.mat_ambient_b);
		glMaterialfv(GL_FRONT, GL_DIFFUSE,   m_param.mat_diffuse_b);
		glMaterialfv(GL_FRONT, GL_EMISSION,  m_param.mat_emission_b);
		glMaterialfv(GL_FRONT, GL_SPECULAR,  m_param.mat_specular_b);
		glMaterialf (GL_FRONT, GL_SHININESS, m_param.mat_shininess_b);

        Slider[3]->SetOffset((m_rectC.left+m_rectC.right)/2.0, (m_rectC.top+m_rectC.bottom)/2.0, 5.0);
        glBindTexture(GL_TEXTURE_2D, Texture1);
        Slider[3]->Draw_HMME(Slider[3]->ReadFacet(), Slider[3]->GetQntPlanes(), PicSize1);

		glPopAttrib();
	}
}
//------------------------------------------------------------------------------
void CDSimpleSlider::SaveParameters(SimpleXML *pSimpleXML)
{
    CString cString;
    char String[16];

    sprintf_s(String, "%d", m_param.nElement);

    cString = '\0';
    for(int Counter2 = 0, Size = strlen(String); Counter2 < Size; Counter2++)
        cString = cString + String[Counter2];

    hXml hRoot = pSimpleXML->GetRootNode();
    hXml hInterfaceRoot = pSimpleXML->GetChildNode(hRoot);

    hXml hSimpleSliderRoot = pSimpleXML->AddElement(L"SimpleSlider_"+cString, hInterfaceRoot);

    hXml hTextureRoot_1 = pSimpleXML->AddElement(L"Texture_1", hSimpleSliderRoot);
	pSimpleXML->SetAttributeValue(hTextureRoot_1, L"ZISNumber_Texture1", CComVariant(m_param.Number_Texture[0]));
	pSimpleXML->SetAttributeValue(hTextureRoot_1, L"path1", CComVariant(m_param.FileNameField));

    hXml hTextureRoot_2 = pSimpleXML->AddElement(L"Texture_2", hSimpleSliderRoot);
	pSimpleXML->SetAttributeValue(hTextureRoot_2, L"ZISNumber_Texture2", CComVariant(m_param.Number_Texture[1]));
	pSimpleXML->SetAttributeValue(hTextureRoot_2, L"path2", CComVariant(m_param.FileNameCursor1));

    hXml hTextureRoot_3 = pSimpleXML->AddElement(L"Texture_3", hSimpleSliderRoot);
	pSimpleXML->SetAttributeValue(hTextureRoot_3, L"ZISNumber_Texture3", CComVariant(m_param.Number_Texture[2]));
	pSimpleXML->SetAttributeValue(hTextureRoot_3, L"path3", CComVariant(m_param.FileNameCursor2));

    hXml hMaterialRoot = pSimpleXML->AddElement(L"Material", hSimpleSliderRoot);

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
void CDSimpleSlider::LoadParameters(SimpleXML *pSimpleXML)
{
    VARIANT vAtrrib;
    CString str = L"";
    CString Pattern = L"SimpleSlider_";
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
	float Rotate;
	if (m_param.PositionType == tpHorizontal)
		Rotate = 0.f;
	else
		Rotate = 90.f;
    hXml hTextureRoot_1 = pSimpleXML->GetChildNode(hElement);
    pSimpleXML->GetAttributeValue(hTextureRoot_1, L"ZISNumber_Texture1", vAtrrib);
    if (vAtrrib.vt == VT_BSTR)
        if (CString(vAtrrib.bstrVal).GetLength() != 0)
			m_param.Number_Texture[0] = _wtoi(vAtrrib.bstrVal);
    pSimpleXML->GetAttributeValue(hTextureRoot_1, L"path1", vAtrrib);
    if (vAtrrib.vt == VT_BSTR)
        if (CString(vAtrrib.bstrVal).GetLength() != 0)
            SetFileNameField(CString(vAtrrib.bstrVal), Rotate);

    hXml hTextureRoot_2 = pSimpleXML->GetNextNode(hTextureRoot_1);
    pSimpleXML->GetAttributeValue(hTextureRoot_2, L"ZISNumber_Texture2", vAtrrib);
    if (vAtrrib.vt == VT_BSTR)
        if (CString(vAtrrib.bstrVal).GetLength() != 0)
			m_param.Number_Texture[1] = _wtoi(vAtrrib.bstrVal);
    pSimpleXML->GetAttributeValue(hTextureRoot_2, L"path2", vAtrrib);
    if (vAtrrib.vt == VT_BSTR)
        if (CString(vAtrrib.bstrVal).GetLength() != 0)
            SetFileNameCursor(CString(vAtrrib.bstrVal), Rotate);

    hXml hTextureRoot_3 = pSimpleXML->GetNextNode(hTextureRoot_1);
    pSimpleXML->GetAttributeValue(hTextureRoot_3, L"ZISNumber_Texture3", vAtrrib);
    if (vAtrrib.vt == VT_BSTR)
        if (CString(vAtrrib.bstrVal).GetLength() != 0)
			m_param.Number_Texture[1] = _wtoi(vAtrrib.bstrVal);
    pSimpleXML->GetAttributeValue(hTextureRoot_3, L"path3", vAtrrib);
    if (vAtrrib.vt == VT_BSTR)
        if (CString(vAtrrib.bstrVal).GetLength() != 0)
            SetFileNameCursor(CString(vAtrrib.bstrVal), Rotate);

    //рассеянный цвет материала (цвет материала в тени)
    hXml hMaterialRoot = pSimpleXML->GetNextNode(hTextureRoot_3);
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
void CDSimpleSlider::SetColor(GLfloat *ma, GLfloat *md,	GLfloat *me, GLfloat *msp, GLfloat msh)
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