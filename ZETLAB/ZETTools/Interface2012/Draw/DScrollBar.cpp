//------------------------------------------------------------------------------
// DScrollBar.cpp : файл реализации
// Прямоугольник элемента в процессе работы не меняется, поэтому положение
// (горизонтальное или вертикальное) определяется один раз в конструкторе.
//------------------------------------------------------------------------------
#include "stdafx.h"
#include "C:\ZETTools\Interface2012\Draw\DScrollBar.h"
//------------------------------------------------------------------------------
int g_nScrollBar = 0;		// счётчик статических картинок
//------------------------------------------------------------------------------
CDScrollBar::CDScrollBar(CRect rect,	bool *pFlagIsChange, 
	double *pTimeAnimation) :
	CCustomDraw(rect, pFlagIsChange, pTimeAnimation)
{
	m_pParamDraw = &m_param;	// обеспечение ОБЩИХ параметров
	m_param.nElement = ++g_nScrollBar;
	SetNameWithNumber(L"ScrollBar");
	//TRACE("CREATE CDScrollBar %s\n", (CStringA)m_pParamDraw->Name);
	if ( m_rect.Width() > m_rect.Height() )
		m_param.PositionType = tpHorizontal;
	else
		m_param.PositionType = tpVertical;
	m_param.bMouseChange = true;
	// переменные класса
	m_bFlag = false;
	m_bEvent = m_bFlag = false;
	m_nStart = 0;
	DefineRectAll();
	float Depth = 55.f;
	float Rotate, CenterX[4], CenterY[4];
	for (int i = 0; i < 4; i++)
		ScrollBar[i] = nullptr;
	//передаются координаты центра области
	if (m_param.PositionType==tpHorizontal)
	{
		Rotate=0;
		CenterY[0]=CenterY[1]=CenterY[2]=CenterY[3]=(rect.bottom+rect.top)/2.0f;
		CenterX[0]=(2*rect.left+rect.Height()/2.0f)/2.0f;
		CenterX[1]=(rect.left+rect.right)/2.0f;
		CenterX[2]=(2*rect.right-rect.Height()/2.0f)/2.0f;
		CenterX[3]=(m_rectC.left+m_rectC.right)/2.0f;

       // SetFileNameCursor(L"C:\\ZETTools\\Interface2012\\Textures\\Seamless\\Other\\cursor_1.bmp", Rotate);
		//SetFileNameCursor(L"C:\\ZETTools\\Interface2012\\Textures\\Patterns\\metal_mart14.bmp", Rotate);

#ifdef _ACTIVEX_ZIS
        ScrollBar[0] = new CDetail(long(36), CenterX[0], CenterY[0], 0, Rotate, rect.Height() / 2.f, rect.Height(), Depth);
        ScrollBar[1] = new CDetail(long(37), CenterX[1], CenterY[1], 0, Rotate, rect.Width() - rect.Height(), rect.Height(), Depth);
        ScrollBar[2] = new CDetail(long(38), CenterX[2], CenterY[2], 0, Rotate, rect.Height() / 2.f, rect.Height(), Depth);
        ScrollBar[3] = new CDetail(long(39), CenterX[3], CenterY[3], 0, Rotate, rect.Height() - 2, rect.Height() - 2, Depth);//*/
#else
		ScrollBar[0] = new CDetail("C:\\ZETTools\\Interface2012\\Models\\Слайдер\\Граница ползунка L 6,205х12,235х5.stl",CenterX[0], CenterY[0], 0, Rotate, rect.Height() / 2.f, rect.Height(), rect.Height());
        ScrollBar[1] = new CDetail("C:\\ZETTools\\Interface2012\\Models\\Слайдер\\Основание ползунка 30х12,235х5.stl",CenterX[1], CenterY[1], 0, Rotate, rect.Width() - rect.Height(), rect.Height(), rect.Height());
        ScrollBar[2] = new CDetail("C:\\ZETTools\\Interface2012\\Models\\Слайдер\\Граница ползунка R 6,205х12,235х5.stl",CenterX[2], CenterY[2], 0, Rotate, rect.Height() / 2.f, rect.Height(), rect.Height());
        ScrollBar[3] = new CDetail("C:\\ZETTools\\Interface2012\\Models\\Слайдер\\Ручка ползунка.stl",CenterX[3], CenterY[3], 0, Rotate, rect.Height() - 2, rect.Height() - 2, rect.Height());//*/
#endif

	}
	if (m_param.PositionType==tpVertical)
	{
		Rotate = 90.f;
		CenterX[0] = CenterX[1] = CenterX[2] = CenterX[3] = (rect.left + rect.right)/2.0f;
		CenterY[0] = (2*rect.top + rect.Width()/2.0f)/2.0f;
		CenterY[1] = (rect.bottom + rect.top)/2.0f;
		CenterY[2] = (2*rect.bottom - rect.Width()/2.0f)/2.0f;
		CenterY[3] = (m_rectC.top + m_rectC.bottom)/2.0f;

#ifdef _ACTIVEX_ZIS
		ScrollBar[0] = new CDetail(long(36), CenterX[0], CenterY[0], 0, Rotate, rect.Width(), rect.Width() / 2.f, rect.Width());
		ScrollBar[1] = new CDetail(long(37), CenterX[1], CenterY[1], 0, Rotate, rect.Width(), rect.Width() - rect.Height(), rect.Width());
		ScrollBar[2] = new CDetail(long(38), CenterX[2], CenterY[2], 0, Rotate, rect.Width(), rect.Width() / 2.f, rect.Width());
		ScrollBar[3] = new CDetail(long(39), CenterX[3], CenterY[3], 0, Rotate, rect.Width() - 2, rect.Width() - 2, rect.Width());//*/
#else
		ScrollBar[0] = new CDetail("C:\\ZETTools\\Interface2012\\Models\\Слайдер\\Граница ползунка L 6,205х12,235х5.stl",CenterX[0], CenterY[0], 0, Rotate, rect.Width(), rect.Width() / 2.f, rect.Width());
        ScrollBar[1] = new CDetail("C:\\ZETTools\\Interface2012\\Models\\Слайдер\\Основание ползунка 30х12,235х5.stl",CenterX[1], CenterY[1], 0, Rotate, rect.Width(), rect.Width() - rect.Height(), rect.Width());
        ScrollBar[2] = new CDetail("C:\\ZETTools\\Interface2012\\Models\\Слайдер\\Граница ползунка R 6,205х12,235х5.stl",CenterX[2], CenterY[2], 0, Rotate, rect.Width(), rect.Width() / 2.f, rect.Width());
        ScrollBar[3] = new CDetail("C:\\ZETTools\\Interface2012\\Models\\Слайдер\\Ручка ползунка.stl",CenterX[3], CenterY[3], 0, Rotate, rect.Width() - 2, rect.Width() - 2, rect.Width());//*/
#endif
        //SetFileNameCursor(L"C:\\ZETTools\\Interface2012\\Textures\\Seamless\\Other\\cursor_2.bmp", Rotate);
	//	SetFileNameCursor(L"C:\\ZETTools\\Interface2012\\Textures\\Patterns\\metal_mart14.bmp", Rotate);
		
	}
    m_param.Rotate = Rotate;
	nowUsing = false;
    SetFileNameField(m_param.Texture_1_FileName, Rotate);
	SetFileNameCursor(m_param.Texture_2_FileName, Rotate);
}
//------------------------------------------------------------------------------
void CDScrollBar::Clear (void)
{
	for (int i = 0; i < 4; i++)
	{
		if (ScrollBar[i])
		ScrollBar[i]->Clear();
	}
}
//------------------------------------------------------------------------------
CDScrollBar::~CDScrollBar()
{
	for (int i = 0; i < 4; i++)
	{
		if (ScrollBar[i])
			delete ScrollBar[i];
	}
}
//------------------------------------------------------------------------------
void CDScrollBar::DefineRectAll()
{
	if (m_param.PositionType == tpHorizontal)
	{
		m_rect0.top = m_rect1.top = m_rectC.top =
			m_rect.top + m_param.deltaY;
		m_rect0.bottom = m_rect1.bottom = m_rectC.bottom =
			m_rect.bottom - m_param.deltaY;
		m_SizeCursor = m_rect.Height()/2.0f;

		m_rect0.left = m_rect.left + m_param.deltaX;
		m_rect0.right = m_rect0.left + m_SizeCursor;

		m_rect1.right = m_rect.right - m_param.deltaX;
		m_rect1.left = m_rect1.right - m_SizeCursor;

		m_LengthValue = m_rect1.left - m_rect0.left;

		m_param.CurrentPosition = m_rect0.left+m_SizeCursor/2;
	}
	else
	{
		m_rect0.left = m_rect1.left = m_rectC.left =
			m_rect.left + m_param.deltaY;
		m_rect0.right = m_rect1.right = m_rectC.right =
			m_rect.right - m_param.deltaY;
		m_SizeCursor = m_rect.Width()/2;

		m_rect0.top = m_rect.top + m_param.deltaX;
		m_rect0.bottom = m_rect0.top + m_SizeCursor;

		m_rect1.bottom = m_rect.bottom - m_param.deltaX;
		m_rect1.top = m_rect1.bottom - m_SizeCursor;

		m_LengthValue = m_rect1.top - m_rect0.top;

		m_param.CurrentPosition = m_rect0.top+m_SizeCursor/2;
	}
	DefineRectCur();
}
//------------------------------------------------------------------------------
void CDScrollBar::DefineRectCur()
{
	float num;

	num = (m_param.fValueCur * m_LengthValue);

	if (m_param.PositionType == tpHorizontal){
		if(num==0) num=m_SizeCursor/2;
		if(num>m_rect1.left-m_rect.left-m_SizeCursor/2.0) num=m_rect1.left-m_rect.left-m_SizeCursor/2.0;
		if(num<m_rect0.right-m_rect.left-m_SizeCursor/2.0) num=m_rect0.right-m_rect.left-m_SizeCursor/2.0;
		m_rectC.left =m_rect.left+num;
		m_rectC.right = m_rectC.left + m_SizeCursor;
	}
	else{  //m_param.PositionType == tpVertical
		if(num==0) num=m_SizeCursor/2.0;
		if(num>m_rect1.top-m_rect.top-m_SizeCursor/2.0) num=m_rect1.top-m_rect.top-m_SizeCursor/2.0;
		if(num<m_rect0.bottom-m_rect.top-m_SizeCursor/2.0) num=m_rect0.bottom-m_rect.top-m_SizeCursor/2.0;
		m_rectC.top =m_rect.top+num;
		m_rectC.bottom = m_rectC.top + m_SizeCursor;
	}
}
//------------------------------------------------------------------------------
void CDScrollBar::SetPositionValue(float val) //функция не используется, смысл непонятен!!!!
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
	}	// сгенерировано в CScrollBar::SetPositionValue
}
//------------------------------------------------------------------------------
void CDScrollBar::SetDeltaX(int val)
{
	if ( (val > 0) && (m_param.deltaX != val) )
	{
		m_param.deltaX = val;
		DefineRectAll();
		IsChange();
	}
}
//------------------------------------------------------------------------------
void CDScrollBar::SetDeltaY(int val)
{
	if ( (val > 0.f) && (m_param.deltaY != val) )
	{
		m_param.deltaY = val;
		DefineRectAll();
		IsChange();
	}
}
//------------------------------------------------------------------------------
void CDScrollBar::SetFileNameField(CString str, double angle)
{
	if ( !str.IsEmpty() && (m_param.FileNameField != str) )
	{
		m_param.FileNameField = str;
        Texture = LoadGLTextures(m_param.FileNameField, PicSize, m_param.Number_Texture[0]);
        for(int Counter=0; Counter<QNT_DETAILS-1; Counter++)
            ScrollBar[Counter]->CalcTexCoors(PicSize[0], PicSize[1], angle);
		IsChange();
	}
}
//------------------------------------------------------------------------------
void CDScrollBar::SetFileNameCursor(CString str, double angle)
{
	if ( !str.IsEmpty() && (m_param.FileNameCursor != str) )
	{
		m_param.FileNameCursor = str;
        Texture1 = LoadGLTextures(m_param.FileNameCursor, PicSize1, m_param.Number_Texture[1]);
        ScrollBar[3]->CalcTexCoordHMME(PicSize1[0], PicSize1[1], angle+180);
		IsChange();
	}
}
//------------------------------------------------------------------------------
bool CDScrollBar::GetNeedEvent()
{
	bool ret = m_bEvent;
	m_bEvent = false;
	return ret;
}
//------------------------------------------------------------------------------
void CDScrollBar::SetTimeAnimation(double val)
{
	if (val > 0.)
		m_param.dTimeAnimation = val;
}
//------------------------------------------------------------------------------
void CDScrollBar::DoProcess()
{
	if (m_param.bAnimation)
	{
		m_param.fValueCur =
			float( (GetTime() - dTimeStart) / m_param.dTimeAnimation );
		if (m_param.PositionType == tpHorizontal){
			if (m_param.PositionValue>m_nStart/(float)m_rect.Width())
			{// движение слева направо
				if(m_param.fValueCur<m_nStart/(float)m_rect.Width())
					return;
				if (m_param.fValueCur > m_param.PositionValue)
					m_param.fValueCur = m_param.PositionValue;
				if (m_param.fValueCur == m_param.PositionValue)
					m_param.bAnimation = false;
			}
			else	// m_param.PositionValue == 1
			{// движение справа налево
				m_param.fValueCur = 1.0 - m_param.fValueCur;
				if(m_param.fValueCur>m_nStart/(float)m_rect.Width())
					return;
				if (m_param.fValueCur < m_param.PositionValue)
					m_param.fValueCur = m_param.PositionValue;
				if (m_param.fValueCur == m_param.PositionValue)
					m_param.bAnimation = false;
			}
		}
		else{
			if (m_param.PositionValue>m_nStart/(float)m_rect.Height())
			{// движение слева направо
				if(m_param.fValueCur<m_nStart/(float)m_rect.Height())
					return;
				if (m_param.fValueCur > m_param.PositionValue)
					m_param.fValueCur = m_param.PositionValue;
				if (m_param.fValueCur == m_param.PositionValue)
					m_param.bAnimation = false;
			}
			else	// m_param.PositionValue == 1
			{// движение справа налево
				m_param.fValueCur = 1.0 - m_param.fValueCur;
				if(m_param.fValueCur>m_nStart/(float)m_rect.Height())
					return;
				if (m_param.fValueCur < m_param.PositionValue)
					m_param.fValueCur = m_param.PositionValue;
				if (m_param.fValueCur == m_param.PositionValue)
					m_param.bAnimation = false;
			}
		}
		DefineRectCur();
		IsChange();
	}
}
//------------------------------------------------------------------------------
void CDScrollBar::OnMouseEnter()
{
	m_param.mat_ambient_b[0] = 0.25f;
	m_param.mat_ambient_b[1] = 0.25f;
	m_param.mat_ambient_b[2] = 0.25f;
	m_param.mat_emission_b[0] = 90.f / 255.f;
	m_param.mat_emission_b[1] = 115.f / 255.f;
	m_param.mat_emission_b[2] = 115.f / 255.f;
	CCustomDraw::OnMouseEnter();
}
//------------------------------------------------------------------------------
void CDScrollBar::OnMouseLeave()
{
	if (m_bFlag)
	{
		m_bFlag = false;
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
void CDScrollBar::OnLButtonClk(UINT nFlags, CPoint point)
{
	bool flag = false;
	/////////
	float Start;

	if (m_param.PositionType == tpHorizontal){
		m_nStart = m_rectC.CenterPoint().x-m_rect.left;
		Start=m_nStart/(float)m_rect.Width();
		m_param.CurrentPosition=m_rectC.left;
	}
	else{
		m_nStart = m_rectC.CenterPoint().y-m_rect.top;
		Start=m_nStart/(float)m_rect.Height();
		m_param.CurrentPosition=m_rectC.top;
    }

	if ( PointInRect(point, &m_rect) )
		m_bEvent = true;

	if (m_bEvent)
	{
		if (m_param.PositionType == tpHorizontal){
			m_param.PositionValue = ((float)point.x-(float)m_rect.left-m_SizeCursor/2)/m_LengthValue;
			if(m_param.PositionValue>(m_rect1.left-m_rect.left)/m_LengthValue) m_param.PositionValue=(m_rect1.left-m_rect.left)/m_LengthValue;
			if(m_param.PositionValue<(m_rect0.right-m_rect.left)/m_LengthValue) m_param.PositionValue=(m_rect0.right-m_rect.left)/m_LengthValue;
		}
		else{
			m_param.PositionValue = ((float)point.y-(float)m_rect.top-m_SizeCursor/2)/m_LengthValue;
			if(m_param.PositionValue>m_rect1.bottom/m_LengthValue) m_param.PositionValue=m_rect1.bottom/m_LengthValue;
			if(m_param.PositionValue<m_rect0.bottom/m_LengthValue) m_param.PositionValue=m_rect0.bottom/m_LengthValue;
		}
		m_param.bAnimation = true;
		dTimeStart = GetTime();
		IsChange();
	}
}
//------------------------------------------------------------------------------
void CDScrollBar::OnLButtonDown(UINT nFlags, CPoint point)
{
	if ( PointInRect(point, &m_rectC) )
	{
		if (m_param.PositionType == tpHorizontal)
			m_nStart = m_rectC.CenterPoint().x-m_rect.left;
		else
			m_nStart = m_rectC.CenterPoint().y-m_rect.top;
		m_bFlag = true;
		IsChange();
	}
}
//------------------------------------------------------------------------------
void CDScrollBar::OnLButtonUp(UINT nFlags, CPoint point)
{
	if (m_bFlag)
	{
		m_bFlag = false;
		IsChange();
	}
}
//------------------------------------------------------------------------------
void CDScrollBar::OnMouseMove(UINT nFlags, CPoint point)
{
	if (m_bFlag)
	{
		if (m_param.PositionType == tpHorizontal)
		{
			m_nStart = m_rectC.CenterPoint().x-m_rect.left;
			float Start=m_nStart/(float)m_rect.Width();
			m_param.CurrentPosition=m_rectC.left;
			m_param.PositionValue = ((float)point.x)/m_LengthValue;
			m_param.fValueCur = float(point.x - m_rect.left-m_SizeCursor/2) / m_LengthValue;
		}
		else
		{
			m_nStart = m_rectC.CenterPoint().y-m_rect.top;
			float Start=m_nStart/(float)m_rect.Height();
			m_param.CurrentPosition=m_rectC.top;
			m_param.PositionValue = ((float)point.y)/m_LengthValue;
			m_param.fValueCur = float(point.y - m_rect.top-m_SizeCursor/2) / m_LengthValue;
		}
		TRACE("Current value %f (must be [0;1])", m_param.fValueCur);
		if (m_param.fValueCur < 0.f)
			m_param.fValueCur = 0.f;
		else if (m_param.fValueCur > 1.f)
			m_param.fValueCur = 1.f;
		DefineRectCur();
		IsChange();
	} 
	
	nowUsing = true;
	pScroll();//*/
}
//------------------------------------------------------------------------------
void CDScrollBar::OnDraw()
{// отображение
	if (m_param.bVisible)
	{
        glPushAttrib(GL_LIGHTING_BIT | GL_TEXTURE_BIT);

        glMaterialfv(GL_FRONT, GL_AMBIENT,   m_param.mat_ambient);
        glMaterialfv(GL_FRONT, GL_DIFFUSE,   m_param.mat_diffuse);
        glMaterialfv(GL_FRONT, GL_EMISSION,  m_param.mat_emission);
        glMaterialfv(GL_FRONT, GL_SPECULAR,  m_param.mat_specular);
        glMaterialf (GL_FRONT, GL_SHININESS, m_param.mat_shininess);

        ScrollBar[3]->SetOffset((m_rectC.left+m_rectC.right)/2.0, (m_rectC.top+m_rectC.bottom)/2.0, 5.0);
        glBindTexture(GL_TEXTURE_2D, Texture);

        for(int Counter=0; Counter<QNT_DETAILS-1; Counter++)
            ScrollBar[Counter]->Draw_texture(ScrollBar[Counter]->ReadFacet(), ScrollBar[Counter]->GetQntPlanes(), PicSize, true);

		glMaterialfv(GL_FRONT, GL_AMBIENT, m_param.mat_ambient_b);
		glMaterialfv(GL_FRONT, GL_DIFFUSE, m_param.mat_diffuse_b);
		glMaterialfv(GL_FRONT, GL_EMISSION, m_param.mat_emission_b);
		glMaterialfv(GL_FRONT, GL_SPECULAR, m_param.mat_specular_b);
		glMaterialf(GL_FRONT, GL_SHININESS, m_param.mat_shininess_b);

        glBindTexture(GL_TEXTURE_2D, Texture1);
        ScrollBar[3]->Draw_texture(ScrollBar[3]->ReadFacet(), ScrollBar[3]->GetQntPlanes(), PicSize1, false);
        glPopAttrib();
	}
}
//------------------------------------------------------------------------------
void CDScrollBar::SetColor(GLfloat *ma, GLfloat *md,	
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
//------------------------------------------------------------------------------
void CDScrollBar::LiftScrollBar(float val)
{
	ScrollBar[0]->SetOffsetZ(val);
    ScrollBar[1]->SetOffsetZ(val);
    ScrollBar[2]->SetOffsetZ(val);
    ScrollBar[3]->SetOffsetZ(val);
}
//------------------------------------------------------------------------------
void CDScrollBar::SaveParameters(SimpleXML *pSimpleXML)
{
    CString cString;
    char String[16];

    sprintf_s(String, "%d", m_param.nElement);

    cString = '\0';
    for(int Counter2 = 0, Size = strlen(String); Counter2 < Size; Counter2++)
        cString = cString + String[Counter2];
    
    hXml hRoot = pSimpleXML->GetRootNode();
    hXml hInterfaceRoot = pSimpleXML->GetChildNode(hRoot);

    hXml hElementRoot = pSimpleXML->AddElement(L"ScrollBar_"+cString, hInterfaceRoot);

    hXml hTextureRoot = pSimpleXML->AddElement(L"Texture", hElementRoot);
    pSimpleXML->SetAttributeValue(hTextureRoot, L"path", CComVariant(m_param.FileNameField));

    hXml hMaterialRoot = pSimpleXML->AddElement(L"Material", hElementRoot);

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
void CDScrollBar::LoadParameters(SimpleXML *pSimpleXML){
    VARIANT vAtrrib;
    CString str = L"";
    CString Pattern = L"ScrollBar_";
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
            SetFileNameField(CString(vAtrrib.bstrVal), m_param.Rotate);

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
