#include "stdafx.h"
#include "Selector.h"



Selector::Selector()
{
	m_rcBounds = Rect(0, 0, 140, 40);
	Init();
}

Selector::Selector(int X, int Y, int Width, int Height)
{
	m_rcBounds = Rect(X, Y, Width, Height);

	Init();
}

Selector::~Selector()
{
	
	if(m_digitGp != NULL)
	{
		DeleteObject(m_digitGp);
		delete m_digitGp;
	}
		

	if(m_rectUnderDigitsBr != NULL)
	{
		DeleteObject(m_rectUnderDigitsBr);
		delete m_rectUnderDigitsBr;
	}
		

	if(m_lineDigitPen != NULL)
	{
		DeleteObject(m_lineDigitPen);
		delete m_lineDigitPen;
	}
	

	DeleteObject(m_chLightBrush.get());
	m_chLightBrush.reset();
	DeleteObject(m_digitsBr.get());
	m_digitsBr.reset();
	DeleteObject(m_font.get());
	m_font.reset();
	DeleteObject(m_bitmap.get());
	m_bitmap.reset();
	DeleteObject(m_backBr1.get());
	m_backBr1.reset();
	DeleteObject(m_backBr2.get());
	m_backBr2.reset();
	DeleteObject(m_backPen1.get());
	m_backPen1.reset();
	DeleteObject(m_pBackColor.get());
	m_pBackColor.reset();
}


void Selector::Init()
{
	m_countSurrColors = 1;
	m_digitGp = NULL;
	m_rectUnderDigitsBr = NULL;
	m_lineDigitPen = NULL;

	m_currentDigit = -1;
	m_countVisibleDigits = 7;
	m_countVisibleDivDigits = 2;
	m_maxValue = 1000;
	m_minValue = 0;
	m_currentValue = 1;
	m_selectorColor.SetValue(0xff444444);
	m_textColor.SetFromCOLORREF(0xeaa955);
	m_selectedDigitColor = Color(255, 127, 199, 255);

	DWORD _3dFace = GetSysColor(COLOR_3DFACE);
	m_3dFace = Color(255, _3dFace & 255, (_3dFace >> 8) & 255, (_3dFace >> 16) & 255);

	memset(m_digits, 0, sizeof(USHORT) * MaxDivCount);

	m_digitGp = new GraphicsPath();
	
	m_rectUnderDigitsBr = new SolidBrush(Color(255, 45, 45, 45));
	m_lineDigitPen = new Pen(Color(255, 79, 79, 79), 2);

	m_pthSurrColor.SetValue(0xff7b7b7b);

	m_strFmt.SetAlignment(StringAlignmentCenter);
	m_strFmt.SetLineAlignment(StringAlignmentCenter);
}



void Selector::DrawObject(Graphics &gr)
{
	gr.SetSmoothingMode(SmoothingModeAntiAlias);
	gr.SetTextRenderingHint(TextRenderingHintAntiAlias);

	if(!m_bitmap)
	{
		InitDrawTools(gr);
		CreateFont(gr);
		CreateBackBmp(gr);
	}

	if(m_bitmap)
		gr.DrawImage(m_bitmap.get(), m_rcBounds.X, m_rcBounds.Y, m_rcBounds.Width, m_rcBounds.Height);


	if(m_currentDigit >= 0)	// рисуем выделенный прямоугольник с подсветкой.
	{
		m_digitRect.X = CalcRectX(m_currentDigit) + m_rcBounds.X;

		gr.FillRectangle(m_chLightBrush.get(), m_digitRect.X - 1, m_digitRect.Y, m_digitRect.Width + 2, m_digitRect.Height);

		m_digitGp->Reset();
		DrawDigitRect(gr, m_digitGp, m_digitRect);
	}


	DrawText(gr);
}


void Selector::InitDrawTools(Graphics &gr)
{
	if(m_minValue >= 0) 
	{
		m_charWidth = (int)((float)(m_rcBounds.Width - LEFT_GAP - RIGHT_GAP - m_countVisibleDigits * RECT_GAP) / (float)((float)m_countVisibleDigits + 0.45f) + 0.5f);
		m_signWidth = (int)((float)m_charWidth * 0.15f + 0.5f); 
	}
	else 
	{
		m_charWidth = (int)((float)(m_rcBounds.Width - LEFT_GAP - RIGHT_GAP - m_countVisibleDigits * RECT_GAP) / (float)((float)m_countVisibleDigits + 0.9f) + 0.5f);
		m_signWidth = (int)((float)m_charWidth * 0.6f  + 0.5f); 
	}

	Rect brushRect( 1,  1, m_rcBounds.Width - 2, m_rcBounds.Height - 2);

	m_backBr1.reset();
	m_backBr1 = boost::make_shared <LinearGradientBrush> (brushRect, Color(255, 100, 100, 100), Color(255, 110, 110, 110), LinearGradientModeForwardDiagonal);

	m_backBr2.reset();
	m_backBr2 = boost::make_shared <LinearGradientBrush> (brushRect, Color(255, 110, 110, 110), Color(255, 135, 135, 135), LinearGradientModeForwardDiagonal);

	m_backPen1.reset();
	m_backPen1 = boost::make_shared <Pen>(m_backBr1.get(), 1);

	m_chLightBrush.reset();
	m_chLightBrush = boost::make_shared <SolidBrush> (m_selectedDigitColor);


	//Rect digitRect(DIG_RECT_OFFS_X, DIG_RECT_OFFS_Y, m_CharWidth, rcBounds.Height() - 10);
	m_digitRect.X = m_rcBounds.X + DIG_RECT_OFFS_X;
	m_digitRect.Y = m_rcBounds.Y + DIG_RECT_OFFS_Y;
	m_digitRect.Width = m_charWidth;
	m_digitRect.Height = m_rcBounds.Height - DIG_RECT_OFFS_Y * 2;

	m_dotWidth = (int)(m_charWidth * 0.3f);//(pdc->GetOutputTextExtent(_T("."))).cx; // + m_CharWidth/6.;
}



void Selector::CreateBackBmp(Graphics &gr)
{
	m_bitmap.reset();
	m_bitmap = boost::make_shared<Bitmap>(m_rcBounds.Width, m_rcBounds.Height, &gr);

	Graphics graph(m_bitmap.get());

	if(m_pBackColor)
	{
		if(m_pBackColor->GetA() != 0)
		{
			graph.FillRectangle(&SolidBrush(*m_pBackColor.get()), 0, 0, m_rcBounds.Width, m_rcBounds.Height);
			graph.DrawRectangle(&Pen(&SolidBrush(*m_pBackColor.get()), 1), 0, 0, m_rcBounds.Width, m_rcBounds.Height);
		}
	}
	else
	{
		graph.FillRectangle(&SolidBrush(m_3dFace), 0, 0, m_rcBounds.Width, m_rcBounds.Height);
		graph.DrawRectangle(&Pen(&SolidBrush(m_3dFace), 1), 0, 0, m_rcBounds.Width, m_rcBounds.Height);
	}

	int radiusSideRects = 4;
	if(m_rcBounds.Height < 8) {radiusSideRects = m_rcBounds.Height / 2;} 

 	DrawRadiusRect(graph, m_backBr1.get(), m_backPen1.get(), 1, 1, m_rcBounds.Width - 2, m_rcBounds.Height - 2, radiusSideRects, 0);
 	DrawRadiusRect(graph, m_backBr2.get(), NULL, 3, 3, m_rcBounds.Width - 6, m_rcBounds.Height - 6, radiusSideRects, 0);

	graph.FillRectangle(m_rectUnderDigitsBr, 5, 5, m_rcBounds.Width - 10, m_rcBounds.Height - 10);

	m_digitRect.X = DIG_RECT_OFFS_X;
	m_digitRect.Y = DIG_RECT_OFFS_Y;

	for(int i = 0 ; i < m_countVisibleDigits; i++)
	{
		//if(i != m_currentDigit)
		{
			m_digitRect.X = CalcRectX(i);
			m_digitGp->Reset();
			DrawDigitRect(graph, m_digitGp, m_digitRect);
		}
	}

	m_digitRect.Y = m_rcBounds.Y + DIG_RECT_OFFS_Y;
}



long Selector::CalcRectX(const int &rectNumb)
{
	if( (rectNumb < (m_countVisibleDigits - m_countVisibleDivDigits)))		//до точки
		return (  LEFT_GAP + rectNumb * m_charWidth + m_signWidth + RECT_GAP * rectNumb );
	else													// после точки
		return (  LEFT_GAP + rectNumb * m_charWidth + m_signWidth + m_dotWidth + RECT_GAP * rectNumb );	

	return DIG_RECT_OFFS_X;
}



void Selector::DrawDigitRect(Graphics &graph, GraphicsPath *gp, const Rect &digitRect)
{
	gp->AddRectangle(digitRect);

	PathGradientBrush pthGrBrush(gp);

	pthGrBrush.SetCenterColor(m_selectorColor);
	pthGrBrush.SetSurroundColors(&m_pthSurrColor, &m_countSurrColors);
	pthGrBrush.SetFocusScales(0.8f, 0.5f);
	
	graph.FillRectangle(&pthGrBrush, digitRect);

	graph.DrawLine(m_lineDigitPen, digitRect.X + 4, digitRect.Y + digitRect.Height / 7, digitRect.GetRight() - 4, digitRect.Y + digitRect.Height / 7);
	graph.DrawLine(m_lineDigitPen, digitRect.X + 4, digitRect.GetBottom() - digitRect.Height / 7, digitRect.GetRight() - 4, digitRect.GetBottom() - digitRect.Height / 7);
}



void Selector::DrawText(Graphics &gr)	
{
	CString val;
	int i;

	if (m_Sign == -1)
		val.Format(_T("%s"), _T("-"));
	else
		val.Format(_T("%s"), _T(" "));

	if(!m_font)
		return;

	//знак числа
	gr.DrawString(val, 1, m_font.get(), RectF(m_rcBounds.X + LEFT_GAP + m_signWidth / 6, m_rcBounds.Y + m_textTop + 2, m_signWidth - m_signWidth / 6, m_textRect.Height), &m_strFmt, m_digitsBr.get());

	//Вывод целой части числа
	for (i = 0; i < m_countVisibleDigits - m_countVisibleDivDigits; i++)
	{
		val.Format(_T("%i"), m_digits[i]);
		gr.DrawString(val, 1, m_font.get(), RectF(m_rcBounds.X + LEFT_GAP + i*(m_charWidth) + m_signWidth + (m_charWidth - m_textRect.Width) / 2 + RECT_GAP * i, m_rcBounds.Y + m_textTop + 2, m_textRect.Width, m_textRect.Height), &m_strFmt, m_digitsBr.get());
	}

	//Вывод разделителя и оставшейся части числа
	if (m_countVisibleDivDigits != 0)
	{
		val.Format(_T("%s"), _T("."));
		gr.DrawString(val, 1, m_font.get(), RectF(m_rcBounds.X + LEFT_GAP + (m_countVisibleDigits - m_countVisibleDivDigits ) * m_charWidth + m_signWidth + RECT_GAP * (m_countVisibleDigits - m_countVisibleDivDigits), m_rcBounds.Y + m_textTop + 2, m_dotWidth, m_textRect.Height), &m_strFmt, m_digitsBr.get());

		for (i = m_countVisibleDigits - m_countVisibleDivDigits; i < m_countVisibleDigits; i++)
		{
			val.Format(_T("%i"), m_digits[i]);
			gr.DrawString(val, 1, m_font.get(), RectF(m_rcBounds.X + LEFT_GAP + i*(m_charWidth) + m_signWidth + m_dotWidth + (m_charWidth - m_textRect.Width) / 2 + RECT_GAP * i, m_rcBounds.Y + m_textTop + 2, m_textRect.Width, m_textRect.Height), &m_strFmt, m_digitsBr.get());
		}
	}
}



void Selector::DrawRadiusRect(Graphics& graphics, Brush *brush, Pen* pen, float X, float Y, float width, float height, float radius, int cut)
{
	m_digitGp->Reset();

	// 0 - no cut, 1 - top cut, 2 - right cut, 3 - bottom cut, 4 - left cut.
	if(cut < 0 || cut > 4) cut = 0;

	if(!cut)
	{
		m_digitGp->AddLine(X + radius, Y, X + width - (radius*2), Y);				// top line
		m_digitGp->AddArc(X + width - (radius*2), Y, radius*2, radius*2, 270, 90);		// top right arc
		m_digitGp->AddLine(X + width, Y + radius, X + width, Y + height - radius);		// right line
		m_digitGp->AddArc(X + width - (radius*2), Y + height - (radius*2), radius*2, radius*2,0,90);	// bottom right arc 
		m_digitGp->AddLine(X + width - radius, Y + height, X + radius, Y + height);	// bottom line
		m_digitGp->AddArc(X, Y + height - (radius*2), radius*2, radius*2, 90, 90);		// bottom left arc
		m_digitGp->AddLine(X, Y + height - radius, X, Y + radius);				// left line
		m_digitGp->AddArc(X, Y, radius*2, radius*2, 180, 90);					// top left arc
	}

	m_digitGp->CloseFigure();

	if(pen != NULL)
		graphics.DrawPath(pen, m_digitGp);
	if(brush != NULL)
		graphics.FillPath(brush, m_digitGp);
}


void Selector::CreateFont(Graphics &gr)
{
	FittingFont(gr);

	m_textTop = m_rcBounds.Height / 2 - m_textRect.Height / 2;
	
	m_digitsBr.reset();
	m_digitsBr = boost::make_shared <LinearGradientBrush> (m_rcBounds, m_textColor, m_textColor, LinearGradientModeVertical); 
}


void Selector::FittingFont(Graphics &gr)
{
	REAL fontSize = 19;
	int counter = 0;
	bool heightFitted = false;

	m_font.reset();	
	m_font = boost::make_shared <Gdiplus::Font> (L"Arial", fontSize, FontStyleRegular);
	
	RectF digitRect(7, 5, m_charWidth, m_rcBounds.Height - 10);
	gr.MeasureString(_T("0"), 1, m_font.get(), digitRect, &m_strFmt, &m_textRect);

	while(1)
	{
		if(!heightFitted)
		{
			if(m_textRect.Height > m_digitRect.Height - (m_digitRect.Height / 20) * 2 )
				fontSize -= 0.5;
			else
				if(m_textRect.Height < m_digitRect.Height - (m_digitRect.Height / 4) * 2 )
					fontSize += 0.5;
				else
					heightFitted = true;
		}
		else
			if(m_textRect.Width >= m_charWidth)
				fontSize -= 0.1;
			else
				break;


		m_font.reset();	
		m_font = boost::make_shared <Gdiplus::Font> (L"Arial", fontSize, FontStyleRegular);
		gr.MeasureString(_T("0"), 1, m_font.get(), digitRect, &m_strFmt, &m_textRect);

		if(counter > 70)
			break;

		counter++;
	}
}



void Selector::SetRect(const int &x, const int &y, const int &w, const int &h)
{
	if ( (m_rcBounds.X != x || m_rcBounds.Y != y || m_rcBounds.Width != w || m_rcBounds.Height != h) )
	{
		m_rcBounds = Rect(x, y, w, h);
		m_bitmap.reset();
	}
}

void Selector::SetSelectorColor(const Color &selectorColor)
{
	CheckAndSet_Reset(selectorColor, m_selectorColor);
}

void Selector::SetTextColor(const Color &textColor)
{
	CheckAndSet_Reset(textColor, m_textColor);
}

void Selector::SetBackColor(const Color &backColor)
{
	if(!m_pBackColor || m_pBackColor.get()->GetValue() != backColor.GetValue())
	{
		m_pBackColor = boost::make_shared <Color> (backColor); 
		m_bitmap.reset();
	}
}

void Selector::SetSelectedDigitColor(const Color &selectedDigitColor)
{
	CheckAndSet_Reset(selectedDigitColor, m_selectedDigitColor);
}

void Selector::SetMaxValue(const float &maxVal)
{
	CheckAndSet_Reset(maxVal, m_maxValue);
	SetCurrentValue(m_currentValue);
}

void Selector::SetMinValue(const float &minVal)
{
	CheckAndSet_Reset(minVal, m_minValue);
	SetCurrentValue(m_currentValue);
}

void Selector::SetCurrentValue(const float &currVal)
{
	CheckAndSet_Reset(currVal, m_currentValue);

	if (m_currentValue > m_maxValue)
		m_currentValue = m_maxValue;
	else
		if (m_currentValue < m_minValue)
			m_currentValue = m_minValue;

	UnFormIndicatorValue();	//Переводим его в цифры
	FormIndicatorValue();	//чтобы m_Val точно соответствовала отображаемому
}


void Selector::SetCountVisibleDigits(const int &numDig)
{
	CheckAndSet_Reset(numDig, m_countVisibleDigits);

	if (m_countVisibleDigits < 1)	//Количество позиций не может быть меньше 1
		m_countVisibleDigits = 1;
	else
		if (m_countVisibleDigits > MaxDivCount)	//Количество позиций не может быть больше 10
			m_countVisibleDigits = MaxDivCount;

	if(!m_bitmap)
		SetCurrentValue(m_currentValue);
}

void Selector::SetCountVisibleDivDigits(const int &numDig)
{
	CheckAndSet_Reset(numDig, m_countVisibleDivDigits);

	if (m_countVisibleDivDigits > m_countVisibleDigits - 1)
		m_countVisibleDivDigits = m_countVisibleDigits - 1;
	else
		if(m_countVisibleDivDigits < 0)
			m_countVisibleDivDigits = 0;

	if(!m_bitmap)
		SetCurrentValue(m_currentValue);
}


void Selector::SetCurrentDigit(const short &currDigit)
{
	m_currentDigit = currDigit;
}






Rect Selector::GetRect()
{
	return m_rcBounds;
}

Color Selector::GetSelectorColor()
{
	return m_selectorColor;
}

Color Selector::GetTextColor()
{
	return m_textColor;
}

Color* Selector::GetBackColor()
{
	if(m_pBackColor)
		return m_pBackColor.get();

	return NULL;
}

Color Selector::GetSelectedDigitColor()
{
	return m_selectedDigitColor;
}

float Selector::GetMaxValue()
{
	return m_maxValue;
}

float Selector::GetMinValue()
{
	return m_minValue;
}

float Selector::GetCurrentValue()
{
	return m_currentValue;
}

int Selector::GetCountVisibleDigits()
{
	return m_countVisibleDigits;
}

int Selector::GetCountVisibleDivDigits()
{
	return m_countVisibleDivDigits;
}


short Selector::GetCurrentDigit()
{
	return m_currentDigit;
}



short Selector::GetPosFromPoint( CPoint pt ) 
{
	short i, pos = -1;

	for (i = 0; i < m_countVisibleDigits - m_countVisibleDivDigits; i++)	//Цифры до запятой
	{
		if ((pt.x > (m_rcBounds.X + LEFT_GAP + i * m_charWidth + m_signWidth + RECT_GAP * i)) && (pt.x < (m_rcBounds.X + LEFT_GAP + (i + 1) * m_charWidth + m_signWidth + RECT_GAP * (i + 1))) &&
			(pt.y >= m_rcBounds.Y) && (pt.y <= m_rcBounds.GetBottom()))
			{
				pos = i;
			}
	}

	for (i = m_countVisibleDigits - m_countVisibleDivDigits; i < m_countVisibleDigits; i++)	//Цифры после запятой
	{
		if ((pt.x >= (m_rcBounds.X + LEFT_GAP + i * m_charWidth + m_dotWidth + m_signWidth + RECT_GAP * i)) && (pt.x < (m_rcBounds.X + LEFT_GAP + (i + 1) * m_charWidth + m_signWidth + m_dotWidth + RECT_GAP * (i + 1))) &&
			(pt.y >= m_rcBounds.Y) && (pt.y <=m_rcBounds.GetBottom()))
			{
				pos = i;
			}
	}

	return pos;
}



//////////////////////////////////////////////////////////////////////////
// формирует float из цифр разрядов
void Selector::FormIndicatorValue(void)
{
	double buf1;
	double res;
	int i;

	buf1=0.0;
	for (i=0; i<m_countVisibleDigits; i++)
		buf1 += (double)(m_digits[i] * pow((double)10., (double)m_countVisibleDigits-i-1));

	res = buf1 / (double)(pow((double)10.,(double)m_countVisibleDivDigits))*m_Sign;

	m_currentValue = (float)res;

	if (m_currentValue>=0)
		m_Sign = 1;
	else
		m_Sign = -1;

}

//////////////////////////////////////////////////////////////////////////
// формирует цифры разрядов из float
void Selector::UnFormIndicatorValue(void)
{
	double tmp = m_currentValue;
	int i;
	tmp = (double)(tmp*pow((double)10.,m_countVisibleDivDigits));
	//end.test

	//Запоминаем знак числа
	if (tmp < 0)
		m_Sign = -1;
	else
		m_Sign = 1;

	tmp = abs(tmp);						//Абсолютное значение

	//Инициализация цифр индикатора нулями
	memset(m_digits, 0, sizeof(USHORT) * MaxDivCount);

	for(i = m_countVisibleDigits - 1; i >= 0; i--)
	{
		m_digits[i] = (USHORT)((unsigned long long)(tmp+0.3)%10);
		tmp = tmp - m_digits[i];
		tmp = tmp/10.;
	}
}



template <typename T>
void Selector::CheckAndSet(const T &setVal, T &currVal)
{
	if(setVal != currVal)
		currVal = setVal;
}

template <typename T>
void Selector::CheckAndSet_Reset(const T &setVal, T &currVal)
{
	if(setVal != currVal)
	{
		currVal = setVal;
		m_bitmap.reset();
	}
}

template <>
void Selector::CheckAndSet_Reset <Color> (const Color &setVal, Color &currVal)
{
	if(setVal.GetValue() != currVal.GetValue())
	{
		currVal = setVal;
		m_bitmap.reset();
	}
}


void Selector::OnMouseWheel(short &zDelta, CPoint &clPoint)
{
	FormIndicatorValue();	//Формируем значение

	//Проверяем над какой цифрой сейчас курсор
	m_currentDigit = GetPosFromPoint(clPoint);
	if(m_currentDigit >= 0)
	{
		if (zDelta > 0)					//Колесо крутится вверх
			IncVariable(m_currentDigit);
		else
			DecVariable(m_currentDigit);
	}
}


void Selector::OnLButtonDown(CPoint point)
{
	m_currentDigit = GetPosFromPoint(point);
}


void Selector::OnKeyUp(UINT nChar)
{
	if(m_currentDigit >= 0)
	{
		if((nChar >= 0x30) && (nChar <= 0x39))		//Цифры от 0 до 9 
		{
			m_digits[m_currentDigit] = nChar - 0x30;
			FormIndicatorValue();
			m_currentDigit++;
			if(m_currentDigit >= m_countVisibleDigits) 
				m_currentDigit = m_countVisibleDigits - 1;
		}
		if((nChar >= 0x60) && (nChar <= 0x69))		//Цифры от 0 до 9 
		{
			m_digits[m_currentDigit] = nChar - 0x60;
			FormIndicatorValue();
			m_currentDigit++;	
			if(m_currentDigit >= m_countVisibleDigits) 
				m_currentDigit = m_countVisibleDigits - 1;
		}
		if(nChar == 0x8)		// BackSpace
		{
			m_digits[m_currentDigit] = 0;
			FormIndicatorValue();
			m_currentDigit--;
			if(m_currentDigit < 0) 
				m_currentDigit = 0;
		}
	}
}


void Selector::IncVariable(short var)
{
	//Увеличиваем переданное значение в пределах 0..9
	//Для изменения значений выбранной цифры по кругу

	double dval = pow((double)10., (double)(m_countVisibleDigits - var - 1) ) / pow((double)10.,(double)m_countVisibleDivDigits);

	SetCurrentValue((float)((double)m_currentValue + dval));
}

void Selector::DecVariable(short var)
{
	//Уменьшаем переданное значение в пределах 0..9
	//Для изменения значений выбранной цифры по кругу

	double dval = pow((double)10.,(double)m_countVisibleDigits - var - 1) / pow((double)10.,(double)m_countVisibleDivDigits);

	SetCurrentValue((float)((double)m_currentValue - (double)dval));
}