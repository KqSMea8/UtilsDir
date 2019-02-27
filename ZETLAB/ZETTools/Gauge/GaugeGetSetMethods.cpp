#include "stdafx.h"
#include "Gauge.h"

//Структура файла:
//SetMethods
//GetMethods


//---------------------------------------------------------------------------------------------------------------------------------------------------------
//----------------------------------SetMethods-------------------------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------------------------------------------------------------------


void Gauge::SetRect(const int &x, const int &y, const int &w, const int &h)
{
	if(m_rcBounds.X != x || m_rcBounds.Y != y || m_rcBounds.Width != w || m_rcBounds.Height != h)
	{
		m_rcBounds = Rect(x, y, w, h);

		int bigDiam = min(w, h) - 2;

		int shadowDiam = min(bigDiam - bigDiam / 22, bigDiam - 5);
		int shadowStart = (bigDiam - shadowDiam) / 2;

		int dialDiam = shadowDiam - 20;
		int dialStart = (shadowStart + (shadowDiam - dialDiam) / 2);

		int arrowCircleDiam = dialDiam / 5;
		int arrowCircleStart = dialStart + (dialDiam - arrowCircleDiam) / 2;

		m_pBackCircle->SetDiam(bigDiam);
		m_pBackCircle->SetShadowDiam(shadowDiam);
		m_pBackCircle->SetShadowStart(shadowStart);

		m_pDial->SetX(dialStart);
		m_pDial->SetY(dialStart);
		m_pDial->SetDiam(dialDiam);


		//m_needle.SetNeedleLength(m_dial.GetDiam() / 2 - m_dial.GetBigLineLength() - 2);
		m_pNeedle->SetDiam(dialDiam);
		m_pNeedle->SetNeedleCircleDiam(arrowCircleDiam);
		m_pNeedle->SetX(dialStart);
		m_pNeedle->SetY(dialStart);

		m_pGlassGlare.reset();
	}
}



void Gauge::SetCurrentValue(float currVal)
{
	m_currentValue = currVal;
}


void Gauge::SetStyle(VolumeStyle gs)
{
	if(gs != m_gaugeStyle)
	{
		m_gaugeStyle = gs;

		if(m_pBackCircle->GetStyle() != m_gaugeStyle)
		{
			m_pBackCircle->SetStyle(m_gaugeStyle);
		}

		if(m_pDial->GetStyle() != m_gaugeStyle)
		{
			m_pDial->SetStyle(m_gaugeStyle);
		}

	}
}


void Gauge::SetBorderDraw(bool borderDraw)
{
	m_pBackCircle->SetBorderDraw(borderDraw);
}

void Gauge::SetBorderColor(Color borderColor)
{
	m_pBackCircle->SetBorderColor(borderColor);
}

void Gauge::SetBackColor(Color backColor)
{
// 	m_pBackCircle->SetBackColor(backColor);
// 	m_pDial->SetBackColor(Color(255, max(backColor.GetR() - 20, 0), max(backColor.GetG() - 20, 0), max(backColor.GetB() - 20, 0)));
	
	m_pBackColor.reset();
	m_pBackColor = boost::make_shared<Color>(backColor);
}

void Gauge::SetGaugeColor(Color gaugeColor)
{
	m_pBackCircle->SetBackColor(gaugeColor);
}

void Gauge::SetDialBackColor(Color dialBackColor)
{
	m_pDial->SetBackColor(dialBackColor);
}




void Gauge::SetBigLineLength(int biglineLength)
{
	m_pDial->SetBigLineLength(biglineLength);
}

void Gauge::SetLittleLineLength(int littleLineLength)
{
	m_pDial->SetLittleLineLength(littleLineLength);
}

void Gauge::SetBigLineWidth(int bigLineWidth)
{
	m_pDial->SetBigLineWidth(bigLineWidth);
}

void Gauge::SetlittleLineWidth(int littleLineWidth)
{
	m_pDial->SetlittleLineWidth(littleLineWidth);
}

void Gauge::SetUnderLineWidth(int underlineWidth)
{
	m_pDial->SetUnderLineWidth(underlineWidth);
}

void Gauge::SetAngleFrom(int angleFrom)
{
	m_pDial->SetAngleFrom(angleFrom);
}

void Gauge::SetAngleTo(int angleTo)
{
	m_pDial->SetAngleTo(angleTo);
}

void Gauge::SetValueFrom(float valueFrom)
{
	m_pDial->SetValueFrom(valueFrom);
}

void Gauge::SetValueTo(float valueTo)
{
	m_pDial->SetValueTo(valueTo);
}

void Gauge::SetMulti(int multi)
{
	m_pDial->SetMulti(multi);
}

void Gauge::SetUnderlineVisible(bool ulineVisible)
{
	m_pDial->SetUnderlineVisible(ulineVisible);
}

void Gauge::SetDigitsVisible(bool digitsVisible)
{
	m_pDial->SetDigitsVisible(digitsVisible);
}

void Gauge::SetGradientDialType(gradientDialType gDVisible)
{
	m_pDial->SetGradientDialType(gDVisible);
}

void Gauge::SetDigitsPos(digitsPos digitspos)
{
	m_pDial->SetDigitsPos(digitspos);
}

void Gauge::SetLLVS(littleLinesValSub llvs)
{
	m_pDial->SetLLVS(llvs);
}

void Gauge::SetBigLineColor(Color bigLineColor)
{
	m_pDial->SetBigLineColor(bigLineColor);
}

void Gauge::SetLittleLineColor(Color littleLineColor)
{
	m_pDial->SetLittleLineColor(littleLineColor);
}

void Gauge::SetDigitsColor(Color digitsColor)
{
	m_pDial->SetDigitsColor(digitsColor);
}

void Gauge::SetFontSize(int fontSize)
{
	m_pDial->SetFontSize(fontSize);
}

void Gauge::SetFontStyle(FontStyle fontStyle)
{
	m_pDial->SetFontStyle(fontStyle);
}

void Gauge::SetFormatDigits(CString formatStr)
{
	m_pDial->SetFormatDigits(formatStr);
}

void Gauge::SetunderlineColor(Color underLineColor)
{
	m_pDial->SetunderlineColor(underLineColor);
}

void Gauge::SetWidthGradDialLine(int widthGradDialLine)
{
	m_pDial->SetWidthGradDialLine(widthGradDialLine);
}

void Gauge::SetRedPercentGradDialLine(float redPercentGradDialLine)
{
	m_pDial->SetRedPercentGradDialLine(redPercentGradDialLine);
}

void Gauge::SetYellowPercentGradDialLine(float yellowPercentGradDialLine)
{
	m_pDial->SetYellowPercentGradDialLine(yellowPercentGradDialLine);
}

void Gauge::SetGreenPercentGradDialLine(float greenPercentGradDialLine)
{
	m_pDial->SetGreenPercentGradDialLine(greenPercentGradDialLine);
}

void Gauge::SetGradientDialTone(Color gradientDialTone)
{
	m_pDial->SetGradientDialTone(gradientDialTone);
}




void Gauge::SetNeedleLength(int needleLength)
{
	m_pNeedle->SetNeedleLength(needleLength);
}

void Gauge::SetNeedleWidth(int needleWidth)
{
	m_pNeedle->SetNeedleWidth(needleWidth);
}

void Gauge::SetNeedleArrowWidth(int needleArrowWidth)
{
	m_pNeedle->SetNeedleArrowWidth(needleArrowWidth);
}

void Gauge::SetNeedleStyle(needleStyle needleSt)
{
	m_pNeedle->SetNeedleStyle(needleSt);
}

// void Gauge::SetNeedleCircleDiam(int needleCircleDiam)
// {
// 	m_pNeedle->SetNeedleCircleDiam(needleCircleDiam);
// }

void Gauge::SetNeedleColor(Color needleColor)
{
	m_pNeedle->SetNeedleColor(needleColor);
}





//---------------------------------------------------------------------------------------------------------------------------------------------------------
//----------------------------------GetMethods-------------------------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------------------------------------------------------------------


Rect Gauge::GetRect()
{
	return m_rcBounds;
}

float Gauge::GetCurrentValue()
{
	return m_currentValue;
}


VolumeStyle Gauge::GetStyle()
{
	return m_gaugeStyle;
}


bool Gauge::GetBorderDraw()
{
	return m_pBackCircle->GetBorderDraw();
}

Color Gauge::GetBorderColor()
{
	return m_pBackCircle->GetBorderColor();
}

Color* Gauge::GetBackColor()
{
	if(m_pBackColor)
		return m_pBackColor.get();

	return NULL;
}

Color Gauge::GetGaugeColor()
{
	return m_pBackCircle->GetBackColor();
}



float Gauge::GetAngleFromCurrentValue(float currVal)
{
	return m_pDial->GetAngleFromCurrentValue(currVal);
}

Color Gauge::GetDialBackColor()
{
	return m_pDial->GetBackColor();
}

int Gauge::GetBigLineLength()
{
	return m_pDial->GetBigLineLength();
}

int Gauge::GetLittleLineLength()
{
	return m_pDial->GetLittleLineLength();
}

int Gauge::GetBigLineWidth()
{
	return m_pDial->GetBigLineWidth();
}

int Gauge::GetlittleLineWidth()
{
	return m_pDial->GetlittleLineWidth();
}

int Gauge::GetUnderLineWidth()
{
	return m_pDial->GetUnderLineWidth();
}

int Gauge::GetAngleFrom()
{
	return m_pDial->GetAngleFrom();
}

int Gauge::GetAngleTo()
{
	return m_pDial->GetAngleTo();
}

float Gauge::GetValueFrom()
{
	return m_pDial->GetValueFrom();
}

float Gauge::GetValueTo()
{
	return m_pDial->GetValueTo();
}

int Gauge::GetMulti()
{
	return m_pDial->GetMulti();
}

bool Gauge::GetUnderlineVisible()
{
	return m_pDial->GetUnderlineVisible();
}

bool Gauge::GetDigitsVisible()
{
	return m_pDial->GetDigitsVisible();
}

gradientDialType Gauge::GetGradientDialType()
{
	return m_pDial->GetGradientDialType();
}

digitsPos Gauge::GetDigitsPos()	
{
	return m_pDial->GetDigitsPos();
}

littleLinesValSub Gauge::GetLLVS()
{
	return m_pDial->GetLLVS();
}

Color Gauge::GetBigLineColor()
{
	return m_pDial->GetBigLineColor();
}

Color Gauge::GetLittleLineColor()
{
	return m_pDial->GetLittleLineColor();
}

Color Gauge::GetDigitsColor()
{
	return m_pDial->GetDigitsColor();
}

int Gauge::GetFontSize()
{
	return m_pDial->GetFontSize();
}

FontStyle Gauge::GetFontStyle()
{
	return m_pDial->GetFontStyle();
}

CString Gauge::GetFormatDigits()
{
	return m_pDial->GetFormatDigits();
}

Color Gauge::GetUnderLineColor()
{
	return m_pDial->GetUnderLineColor();
}

int Gauge::GetWidthGradDialLine()
{
	return m_pDial->GetWidthGradDialLine();
}

float Gauge::GetRedPercentGradDialLine()
{
	return m_pDial->GetRedPercentGradDialLine();
}

float Gauge::GetYellowPercentGradDialLine()
{
	return m_pDial->GetYellowPercentGradDialLine();
}

float Gauge::GetGreenPercentGradDialLine()
{
	return m_pDial->GetGreenPercentGradDialLine();
}

Color Gauge::GetGradientDialTone()
{
	return m_pDial->GetGradientDialTone();
}





int Gauge::GetNeedleLength()
{
	return m_pNeedle->GetNeedleLength();
}

int Gauge::GetNeedleWidth()
{
	return m_pNeedle->GetNeedleWidth();
}

int Gauge::GetNeedleArrowWidth()
{
	return m_pNeedle->GetNeedleArrowWidth();
}

needleStyle Gauge::GetNeedleStyle()
{
	return m_pNeedle->GetNeedleStyle();
}

// int Gauge::GetNeedleCircleDiam()
// {
// 	return m_pNeedle->GetNeedleCircleDiam();
// }

Color Gauge::GetNeedleColor()
{
	return m_pNeedle->GetNeedleColor();
}
