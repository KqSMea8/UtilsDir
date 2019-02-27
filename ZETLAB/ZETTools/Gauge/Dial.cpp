#include "stdafx.h"
#include "Dial.h"
#include <vector>
#include <mathZet.h>

void Dial::DialInit()
{
	m_bigLineLength = 10;
	m_littleLineLength = 5;

	m_bigLineWidth = 2;
	m_littleLineWidth = 1;

	m_angleFrom = -40;
	m_angleTo = 220;

	m_valueFrom = 0;
	m_valueTo = 260;

	m_multi = 1000;

	m_underlineVisible = false;	
	m_underLineWidth = 1;
	m_digitsVisible = true;

	m_gradDialType = GDT_APART_GRADIENT;	
	m_widthGradDialLine = 10;
	m_redAngleGradDialLine = (float)(m_angleTo - m_angleFrom) / 100.0f * 30;
	m_yellowAngleGradDialLine = (float)(m_angleTo - m_angleFrom) / 100.0f * 25;
	m_greenAngleGradDialLine = (float)(m_angleTo - m_angleFrom) / 100.0f * 45;

	m_digitsPos = DP_UP;
	m_llvs = LLVS_10;

	m_bigLineColor = Color(255, 0, 51, 102); //Color(255, 255, 139, 25);	
	m_littleLineColor = Color(255, 0, 51, 102);	
	m_backColor = Color(255, 165, 165, 165);
	m_borderColor = Color(255, 120, 120, 120);
	m_digitsColor = Color(255, 0, 0, 0);//Color(255, 255, 139, 25);
	m_endLineColor = Color(255, 0, 51, 102);//Color(255, 255, 45, 5);
	m_underLineColor = Color(255, 250, 250, 10);
	m_gradientDialTone = m_backColor;

	m_fontSize = 10;
	m_fontStyle = FontStyleBold;

	m_formatDigits = L"%.0f";

	m_style = VS_SURROUND;

	m_countLines = 1;
	m_valStep = 1;
	
	m_pFontFamily = new FontFamily(L"Agency FB");

	if(!m_pFontFamily->IsAvailable())
	{
		delete m_pFontFamily;
		m_pFontFamily = nullptr;
		m_pFontFamily = new FontFamily(L"Arial");
	}
}


template <typename T>
void Dial::CheckValues(T &minVal, T &maxVal)
{
	if(minVal > maxVal)
	{
		T tmp = maxVal;
		maxVal = minVal;
		minVal = tmp;
	}
}


void Dial::DrawObject(Graphics &graph)
{
	m_bitmap.reset();

	/////////////////////
	m_bitmap = boost::make_shared<Bitmap>(m_bitmapSize, m_bitmapSize, &graph);
	Graphics gr(m_bitmap.get());

	CString universalStr;
	Gdiplus::Font font(m_pFontFamily, (REAL)m_fontSize, m_fontStyle);

	StringFormat format;
	format.SetAlignment(StringAlignmentCenter);
	format.SetLineAlignment(StringAlignmentCenter);

	RectF rectf1(0,0,0,0), rectf2(0,0,0,0);	// прямоугольники для определения длины текста и вывода его на экран.
	/////////////////////

	
	gr.SetTextRenderingHint(TextRenderingHintAntiAlias);
	gr.SetSmoothingMode(SmoothingModeAntiAlias);
	
	if(m_style == VS_SURROUND)
	{
		GraphicsPath gp;
		gp.AddEllipse(0, 0, m_diametr, m_diametr);
		PathGradientBrush pgb(&gp);
		int countCol = 1;

		pgb.SetCenterColor(Color(255, 250, 250, 250));
		pgb.SetSurroundColors(&m_backColor, &countCol);
		pgb.SetCenterPoint(Point((int)(m_diametr / 1.3f), m_diametr));
		pgb.SetFocusScales(0.1f, 0.1f);

		gr.FillPath(&pgb, &gp);
	}
	else
		gr.FillEllipse(&LinearGradientBrush(Rect(0, 0, m_diametr, m_diametr), m_backColor, Color(255, 250, 250, 250), LinearGradientModeVertical), 0, 0, m_diametr, m_diametr);

	gr.DrawEllipse(&Pen(m_borderColor), 0, 0, m_diametr, m_diametr);

	CheckValues(m_angleFrom, m_angleTo);
	CheckValues(m_valueFrom, m_valueTo);

	// способ распределения рисок. если разница между значениями более чем в 2 раза
	if(m_valueTo)
		m_valStep = (float)pow(10.0f, (int)(log10f(m_valueTo) + 0.99f)) / 10.0f; // шаг значений, т.е. каждая большая риска будет кратна этому значению
	else
		m_valStep = m_valueTo;

	int countstep  = (int)((m_valueTo - m_valueFrom) / m_valStep);

	if(countstep < 5)	// подгоняем шаг значений в зависимости от диапазона значений.
	{
		if(countstep == 4 || countstep == 3)
			m_valStep /= 2;
		else
			if(countstep == 2)
				m_valStep /= 4;
			else
				if(countstep == 1)
					m_valStep /= 5;
				else
					if(countstep == 0)
						m_valStep /= 10;
	}


	m_countLines = (int)(( m_valueTo - (int)(m_valueFrom / m_valStep) * m_valStep ) / m_valStep) + 1;	// количество рисок

// исправлена из-за исправления ошибки найденной 2014.05.29 pvs studio 
//	if((int)(m_valueTo / m_valStep) * m_valStep != m_valueTo)	// если число не кратное, то добавляем еще одну риску.
	if((int)(m_valueTo / m_valStep) * (IsNotEqual(m_valStep, m_valueTo, 0.0001f)))	// если число не кратное, то добавляем еще одну риску.
		m_countLines++;

	

	
	int digitsOffs = 0;
	if(m_digitsVisible) // отступ чисел
	{
		universalStr.Format(m_formatDigits, pow(10.0, (int)max(log10(m_valueFrom), log10(m_valueTo)) - 1) - 1);	//вычисляем число, которое занимает максимальную площадь например 99, 999, 9999 и т.д. 
		gr.MeasureString(universalStr, universalStr.GetLength(), &font, rectf1, &rectf2); // берем длину максимально длинного прямоугольника

		if(rectf2.Width + max(m_bigLineLength, m_littleLineLength) + DIAL_OFFSET * 2 > (float)m_diametr / 4)
		{
			rectf2.Width = 0;
			digitsOffs = 0;
		}
		else
			digitsOffs = DIAL_OFFSET * 4;		
	}


	
	if(m_gradDialType)
		DrawGradDial(gr);

	DrawRiscs(gr, rectf2, digitsOffs, &font, format);
	
	//подчеркивающая линия
	if(m_underlineVisible)
		DrawUnderline(gr, rectf2, digitsOffs);




	//мультиплексор
	universalStr.Format(L"x %d",m_multi);
	gr.MeasureString(universalStr,universalStr.GetLength(), &font,rectf1,&rectf2);

	if(rectf2.Width < m_diametr / 2.5)
	{
		RectF bounds1((REAL)(m_diametr / 2 - rectf2.Width / 2), (REAL)(m_diametr / 2 + m_diametr / 6), (REAL)rectf2.Width, (REAL)rectf2.Height);
		gr.DrawString(universalStr, -1, &font, bounds1, &format, &SolidBrush(m_digitsColor));	
	}
}




void Dial::DrawUnderline(Graphics &gr, RectF &rect, int digitsOffs)
{
	Pen pen(m_underLineColor, (REAL)m_underLineWidth);
	pen.SetStartCap(LineCapRound);
	pen.SetEndCap(LineCapRound);
	if(m_digitsPos == DP_DOWN)
	{
		pen.SetAlignment(PenAlignmentInset);
		gr.DrawArc(&pen, (REAL)(DIAL_OFFSET - 1), (REAL)(DIAL_OFFSET - 1), (REAL)(m_diametr - DIAL_OFFSET * 2 + 2), (REAL)(m_diametr - DIAL_OFFSET * 2 + 2), (REAL)(180 + m_angleFrom), (REAL)(m_angleTo - m_angleFrom)); 
	}
	else
	{
		int xpos = DIAL_OFFSET + m_bigLineLength + (int)rect.Width + (int)rect.Height / 3 + digitsOffs;
		pen.SetAlignment(PenAlignmentCenter);
		gr.DrawArc(&pen, (REAL)(xpos + 1), (REAL)(xpos + 1), (REAL)(m_diametr - (xpos) * 2 - 3), (REAL)(m_diametr - (xpos) * 2 - 3), (REAL)(180 + m_angleFrom),  (REAL)(m_angleTo - m_angleFrom));
	}
}



void Dial::DrawRiscs(Graphics &gr, RectF &rect, int digitsOffs, Gdiplus::Font* font, StringFormat &format)
{
	float angleStep =  ((float)m_angleTo - (float)m_angleFrom) / (m_countLines - 1);	// расстояние между рисками в градусах
	CString universalStr;


	for(float i = (float)m_angleFrom; i <= (float)m_angleTo; i += angleStep / m_llvs)	// маленькие риски
	{
		Color currColor = m_littleLineColor;
		CheckZone(i, currColor);

		DrawDialLines(
			gr,
			currColor,
			m_littleLineWidth,
			m_littleLineLength, 
			DIAL_OFFSET + m_bigLineLength - m_littleLineLength + (int)rect.Width + (int)rect.Height / 3 + digitsOffs,
			DIAL_OFFSET + m_bigLineLength + (int)rect.Width + (int)rect.Height / 3 + digitsOffs,
			i);
	}


	float currentAngle = (float)m_angleFrom;
	for(int i = 0; i < m_countLines; i++)			// большие риски
	{
		//universalStr.Format(m_formatDigits, m_valStep * i + m_valueFrom);
		universalStr.Format(m_formatDigits, (int)(m_valueFrom / m_valStep) * m_valStep + m_valStep * i);

		Color currColor = m_bigLineColor;
		CheckZone(currentAngle, currColor);

		DrawDialLines(
			gr,
			currColor,
			m_bigLineWidth,
			m_bigLineLength, 
			DIAL_OFFSET + (int)rect.Width + (int)rect.Height / 3 + digitsOffs,
			DIAL_OFFSET + m_bigLineLength + (int)rect.Width + (int)rect.Height / 3 + digitsOffs,
			currentAngle);

//исправлена из-за исправления ошибки найденной 2014.05.29 pvs studio 
//		if(m_digitsVisible && rect.Width != 0 &&	// числа
		if(m_digitsVisible && (IsNotEqual(rect.Width, 0.f, 0.0001f)) &&	// числа
			(m_diametr / 2 / rect.Width > 5 || 
			(currentAngle < 50 || currentAngle > 110 || i % 2 == 0)))
				gr.DrawString(universalStr, universalStr.GetLength(), font, GetDigitPoint(currentAngle, digitsOffs, rect), &format, &SolidBrush(m_digitsColor) );
		

		currentAngle += angleStep;
	}

	//риски минимального и максимального значения.
	DrawMinMaxLines(gr, (int)rect.Width + (int)rect.Height / 3 + digitsOffs,  DIAL_OFFSET * 3 + m_bigLineLength + (int)rect.Width + (int)rect.Height / 3 + digitsOffs);

}


PointF Dial::GetDigitPoint(float &currAngle, int &digitsOffs, RectF &rectf)
{
	int radius = m_diametr / 2;

	if(m_digitsPos == DP_DOWN)
		return PointF(
						radius - cos( currAngle * 0.017444f) * ( radius - DIAL_OFFSET - m_bigLineLength - digitsOffs - (int)rectf.Width / 2), //x
						radius + sin( -currAngle * 0.017444f) * ( radius - DIAL_OFFSET - m_bigLineLength - digitsOffs - (int)rectf.Width / 2) //y
					);
	
	return PointF(
					radius - cos( currAngle * 0.017444f) * ( radius - DIAL_OFFSET  - (int)rectf.Width / 2 - (int)rectf.Height / 3), 
					radius + sin( -currAngle * 0.017444f) * ( radius - DIAL_OFFSET - (int)rectf.Width / 2 - (int)rectf.Height / 3) 
				);

}


void Dial::DrawMinMaxLines(Graphics &gr, int xpos, int width)
{
	for(float i = m_valueFrom; i <= m_valueTo; i += m_valueTo - m_valueFrom)

//		if(i != m_valueFrom || (int)m_valueFrom % (int)m_valStep != 0) исправлена из-за исправления ошибки найденной 2014.05.29 pvs studio 
		if (IsNotEqual(i, m_valueFrom, 0.0001f) || ((int)m_valueFrom % (int)m_valStep != 0))
		{
			RotateGr(gr, m_diametr, GetAngleFromCurrentValue(i), RT_PLUS);

			if(m_digitsPos == DP_DOWN)
				gr.DrawLine(&Pen(m_endLineColor, 1), 0, m_diametr / 2, DIAL_OFFSET * 3 + m_bigLineLength, m_diametr / 2);
			else
				gr.DrawLine(&Pen(m_endLineColor, 1), xpos, m_diametr / 2, width, m_diametr / 2);

			RotateGr(gr, m_diametr, GetAngleFromCurrentValue(i), RT_MINUS);
		}
}

void Dial::DrawDialLines(Graphics &gr, Color &lineColor, int &lineWidth, int &linelength, int x1, int x2, float &currentAngle)
{
	RotateGr(gr, m_diametr, currentAngle, RT_PLUS);

	if(m_digitsPos == DP_DOWN)
		gr.DrawLine(&Pen(lineColor, (REAL)lineWidth), (REAL)DIAL_OFFSET, (REAL)(m_diametr / 2), (REAL)(DIAL_OFFSET + linelength), (REAL)(m_diametr / 2));
	else
		gr.DrawLine(&Pen(lineColor, (REAL)lineWidth), (REAL)x1, (REAL)(m_diametr / 2), (REAL)x2, (REAL)(m_diametr / 2));

	RotateGr(gr, m_diametr, currentAngle, RT_MINUS);
}


void Dial::DrawGradDial(Graphics &gr)
{
	if(m_gradDialType == GDT_APART_GRADIENT || m_gradDialType == GDT_APART_HARD || m_gradDialType == GDT_APART_HARD_ONLYRED)
	{

		Pen penGradient(m_backColor, (REAL)m_widthGradDialLine);
		Pen penShadow(Color(255, 120, 120, 120), (REAL)(m_widthGradDialLine - 2));	//тень
		Pen penGlare(Color(255, 250, 250, 250), (REAL)(m_widthGradDialLine - 2));	//отблеск

		penGradient.SetStartCap(LineCapRound);
		penGradient.SetEndCap(LineCapRound);
		penGradient.SetAlignment(PenAlignmentCenter);
		penShadow.SetStartCap(LineCapRound);
		penShadow.SetEndCap(LineCapRound);
		penGlare.SetStartCap(LineCapRound);
		penGlare.SetEndCap(LineCapRound);

		RectF gradDialRect(DIAL_OFFSET * 2 + m_diametr*0.2f, DIAL_OFFSET * 2 + m_diametr*0.2f, m_diametr * 0.6f - DIAL_OFFSET * 4, m_diametr * 0.6f - DIAL_OFFSET * 4);

		gr.DrawArc(&penShadow, (REAL)(gradDialRect.X - 3),	(REAL)(gradDialRect.Y - 3), (REAL)(gradDialRect.Width + 4), (REAL)(gradDialRect.Height + 4), (REAL)(180 + m_angleFrom), (REAL)(m_angleTo - m_angleFrom)); 
		gr.DrawArc(&penGlare,  (REAL)gradDialRect.X,		(REAL)gradDialRect.Y,		(REAL)(gradDialRect.Width + 2), (REAL)(gradDialRect.Height + 2), (REAL)(180 + m_angleFrom), (REAL)(m_angleTo - m_angleFrom)); 

		gr.DrawArc(&penShadow, (REAL)gradDialRect.X,		(REAL)gradDialRect.Y,		(REAL)(gradDialRect.Width - 2), (REAL)(gradDialRect.Height - 2),(REAL)(180 + m_angleFrom), (REAL)(m_angleTo - m_angleFrom)); 
		gr.DrawArc(&penGlare,  (REAL)(gradDialRect.X + 1),	(REAL)(gradDialRect.Y + 1), (REAL)gradDialRect.Width,		(REAL)gradDialRect.Height,		(REAL)(180 + m_angleFrom), (REAL)(m_angleTo - m_angleFrom)); 


		if(m_gradDialType == GDT_APART_GRADIENT)
		{
			if(m_angleTo - m_angleFrom > 179)
			{
				int radius = m_diametr/ 2;
				std::vector <PointF> pointsF;
				for(float i = (float)m_angleFrom - (float)(m_angleTo - m_angleFrom) / 12; i <= (float)m_angleTo + (float)(m_angleTo - m_angleFrom) / 12; i += (float)(m_angleTo - m_angleFrom) / 6)
					pointsF.push_back(PointF((REAL)(radius - cos(i *  0.01744f) * radius * 1.5f), (REAL)(radius - sin(i *  0.01744f) * radius * 1.5f)));



				PathGradientBrush pBrush(pointsF.data(), pointsF.size());
				Color colors[] = {Color(255, 0, 0, 255),  // blue
					Color(255, 0, 255, 255),  // cian
					Color(255, 0, 255, 0),  // green
					Color(255, 255, 255, 0),  // yellow
					Color(255, 255, 165, 0),  // orange
					Color(255, 255, 0, 0)}; // red

				int count = 6;
				pBrush.SetSurroundColors(colors, &count);
				pBrush.SetCenterColor(m_gradientDialTone);

				//gr.FillEllipse(&pBrush,0,0, m_diametr, m_diametr );

				penGradient.SetBrush(&pBrush);
			}
			else
				penGradient.SetColor(m_backColor);

			gr.DrawArc(&penGradient, gradDialRect, (REAL)(180 + m_angleFrom), (REAL)(m_angleTo - m_angleFrom)); 
		}
		else
			if(m_gradDialType == GDT_APART_HARD)
			{
				penGradient.SetColor(Color(230, 255, 0, 0));//red
				gr.DrawArc(&penGradient, gradDialRect, (REAL)(180 + m_angleFrom), (REAL)(m_angleTo - m_angleFrom)); 
			
				penGradient.SetEndCap(LineCapFlat);
				penGradient.SetColor(Color(230, 255, 255, 0));//yellow

				gr.DrawArc(&penGradient, gradDialRect, (REAL)(180 + m_angleFrom), (REAL)(m_angleTo - m_angleFrom - m_redAngleGradDialLine)); 

				penGradient.SetColor(Color(230, 0, 255, 0));//green

				gr.DrawArc(&penGradient, gradDialRect, (REAL)(180 + m_angleFrom), (REAL)(m_angleTo - m_angleFrom - m_redAngleGradDialLine - m_yellowAngleGradDialLine)); 
			}
			else
				if(m_gradDialType == GDT_APART_HARD_ONLYRED)
				{
					gr.DrawArc(&penGradient, gradDialRect, (REAL)(180 + m_angleFrom), (REAL)(m_angleTo - m_angleFrom)); 
					
					penGradient.SetColor(Color(230, 255, 0, 0));//red
					penGradient.SetStartCap(LineCapFlat);
					
					gr.DrawArc(&penGradient, gradDialRect, (REAL)(180 + m_angleFrom + (m_angleTo - m_angleFrom - m_redAngleGradDialLine)), (REAL)(m_redAngleGradDialLine)); 
				}
	}
}


void Dial::CheckZone(float &currAngle, Color &currColor)
{
	if(m_gradDialType == GDT_LINES || m_gradDialType == GDT_LINES_ONLYRED)
	{
		if(m_gradDialType == GDT_LINES_ONLYRED)
		{
			if(m_angleTo - m_redAngleGradDialLine <= currAngle)
				currColor = Color(255, 255, 0, 0);
		}
		else
			if(m_angleTo - m_redAngleGradDialLine <= currAngle)
				currColor = Color(255, 255, 0, 0);
			else
				if(m_angleTo - m_redAngleGradDialLine - m_yellowAngleGradDialLine <= currAngle)
					currColor = Color(255, 255, 255, 0);
				else
					if(m_angleTo - m_redAngleGradDialLine - m_yellowAngleGradDialLine - m_greenAngleGradDialLine <= currAngle)
						currColor = Color(255, 0, 255, 0);
	}
}