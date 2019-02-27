#include "stdafx.h"
#include "CGauge.h"

CGauge::CGauge(CCustomContainer *owner, CRect rect) : CCustomElement(owner)
{
	m_pDGauge = NULL;

	if (m_pOwner)
	{
		m_pDGauge = new DGauge(rect, m_pOwner->GetPointerToGraphics(),
			m_pOwner->GetPointerToIsChange(), m_pOwner->GetPointerToTime() );

		if(m_pDGauge != NULL)
			m_pDClass = m_pDGauge;
	}

	ASSERT(m_pDGauge);
}


CGauge::~CGauge()
{
	if(m_pDGauge != NULL)
		delete m_pDGauge;
}



void CGauge::SetCurrentValue(float currVal)
{
	m_pDGauge->SetCurrentValue(currVal);
}


void CGauge::SetStyle(VolumeStyle gs)
{
	m_pDGauge->SetStyle(gs);
}


void CGauge::SetBorderDraw(bool borderDraw)
{
	m_pDGauge->SetBorderDraw(borderDraw);
}

void CGauge::SetBorderColor(Color borderColor)
{
	m_pDGauge->SetBorderColor(borderColor);
}

void CGauge::SetGaugeColor(Color gaugeColor)
{
	m_pDGauge->SetGaugeColor(gaugeColor);
}

void CGauge::SetDialBackColor(Color dialBackColor)
{
	m_pDGauge->SetDialBackColor(dialBackColor);
}




void CGauge::SetBigLineLength(int biglineLength)
{
	m_pDGauge->SetBigLineLength(biglineLength);
}

void CGauge::SetLittleLineLength(int littleLineLength)
{
	m_pDGauge->SetLittleLineLength(littleLineLength);
}

void CGauge::SetBigLineWidth(int bigLineWidth)
{
	m_pDGauge->SetBigLineWidth(bigLineWidth);
}

void CGauge::SetlittleLineWidth(int littleLineWidth)
{
	m_pDGauge->SetlittleLineWidth(littleLineWidth);
}

void CGauge::SetUnderLineWidth(int underlineWidth)
{
	m_pDGauge->SetUnderLineWidth(underlineWidth);
}

void CGauge::SetAngleFrom(int angleFrom)
{
	m_pDGauge->SetAngleFrom(angleFrom);
}

void CGauge::SetAngleTo(int angleTo)
{
	m_pDGauge->SetAngleTo(angleTo);
}

void CGauge::SetValueFrom(float valueFrom)
{
	m_pDGauge->SetValueFrom(valueFrom);
}

void CGauge::SetValueTo(float valueTo)
{
	m_pDGauge->SetValueTo(valueTo);
}

void CGauge::SetMulti(int multi)
{
	m_pDGauge->SetMulti(multi);
}

void CGauge::SetUnderlineVisible(bool ulineVisible)
{
	m_pDGauge->SetUnderlineVisible(ulineVisible);
}

void CGauge::SetDigitsVisible(bool digitsVisible)
{
	m_pDGauge->SetDigitsVisible(digitsVisible);
}

void CGauge::SetGradientDialType(gradientDialType gDVisible)
{
	m_pDGauge->SetGradientDialType(gDVisible);
}

void CGauge::SetDigitsPos(digitsPos digitspos)
{
	m_pDGauge->SetDigitsPos(digitspos);
}

void CGauge::SetLLVS(littleLinesValSub llvs)
{
	m_pDGauge->SetLLVS(llvs);
}

void CGauge::SetBigLineColor(Color bigLineColor)
{
	m_pDGauge->SetBigLineColor(bigLineColor);
}

void CGauge::SetLittleLineColor(Color littleLineColor)
{
	m_pDGauge->SetLittleLineColor(littleLineColor);
}

void CGauge::SetDigitsColor(Color digitsColor)
{
	m_pDGauge->SetDigitsColor(digitsColor);
}

void CGauge::SetFontSize(int fontSize)
{
	m_pDGauge->SetFontSize(fontSize);
}

void CGauge::SetFontStyle(FontStyle fontStyle)
{
	m_pDGauge->SetFontStyle(fontStyle);
}

void CGauge::SetFormatDigits(CString formatStr)
{
	m_pDGauge->SetFormatDigits(formatStr);
}

void CGauge::SetunderlineColor(Color underLineColor)
{
	m_pDGauge->SetunderlineColor(underLineColor);
}

void CGauge::SetWidthGradDialLine(int widthGradDialLine)
{
	m_pDGauge->SetWidthGradDialLine(widthGradDialLine);
}

void CGauge::SetRedPercentGradDialLine(float redPercentGradDialLine)
{
	m_pDGauge->SetRedPercentGradDialLine(redPercentGradDialLine);
}

void CGauge::SetYellowPercentGradDialLine(float yellowPercentGradDialLine)
{
	m_pDGauge->SetYellowPercentGradDialLine(yellowPercentGradDialLine);
}

void CGauge::SetGreenPercentGradDialLine(float greenPercentGradDialLine)
{
	m_pDGauge->SetGreenPercentGradDialLine(greenPercentGradDialLine);
}

void CGauge::SetGradientDialTone(Color gradientDialTone)
{
	m_pDGauge->SetGradientDialTone(gradientDialTone);
}




void CGauge::SetNeedleLength(int needleLength)
{
	m_pDGauge->SetNeedleLength(needleLength);
}

void CGauge::SetNeedleWidth(int needleWidth)
{
	m_pDGauge->SetNeedleWidth(needleWidth);
}

void CGauge::SetNeedleArrowWidth(int needleArrowWidth)
{
	m_pDGauge->SetNeedleArrowWidth(needleArrowWidth);
}

void CGauge::SetNeedleStyle(needleStyle needleSt)
{
	m_pDGauge->SetNeedleStyle(needleSt);
}

void CGauge::SetNeedleColor(Color needleColor)
{
	m_pDGauge->SetNeedleColor(needleColor);
}




//---------------------------------------------------------------------------------------------------------------------------------------------------------
//----------------------------------GetMethods-------------------------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------------------------------------------------------------------



float CGauge::GetCurrentValue()
{
	return m_pDGauge->GetCurrentValue();
}


VolumeStyle CGauge::GetStyle()
{
	return m_pDGauge->GetStyle();
}


bool CGauge::GetBorderDraw()
{
	return m_pDGauge->GetBorderDraw();
}

Color CGauge::GetBorderColor()
{
	return m_pDGauge->GetBorderColor();
}


Color CGauge::GetGaugeColor()
{
	return m_pDGauge->GetGaugeColor();
}


Color CGauge::GetDialBackColor()
{
	return m_pDGauge->GetDialBackColor();
}

int CGauge::GetBigLineLength()
{
	return m_pDGauge->GetBigLineLength();
}

int CGauge::GetLittleLineLength()
{
	return m_pDGauge->GetLittleLineLength();
}

int CGauge::GetBigLineWidth()
{
	return m_pDGauge->GetBigLineWidth();
}

int CGauge::GetlittleLineWidth()
{
	return m_pDGauge->GetlittleLineWidth();
}

int CGauge::GetUnderLineWidth()
{
	return m_pDGauge->GetUnderLineWidth();
}

int CGauge::GetAngleFrom()
{
	return m_pDGauge->GetAngleFrom();
}

int CGauge::GetAngleTo()
{
	return m_pDGauge->GetAngleTo();
}

float CGauge::GetValueFrom()
{
	return m_pDGauge->GetValueFrom();
}

float CGauge::GetValueTo()
{
	return m_pDGauge->GetValueTo();
}

int CGauge::GetMulti()
{
	return m_pDGauge->GetMulti();
}

bool CGauge::GetUnderlineVisible()
{
	return m_pDGauge->GetUnderlineVisible();
}

bool CGauge::GetDigitsVisible()
{
	return m_pDGauge->GetDigitsVisible();
}

gradientDialType CGauge::GetGradientDialType()
{
	return m_pDGauge->GetGradientDialType();
}

digitsPos CGauge::GetDigitsPos()	
{
	return m_pDGauge->GetDigitsPos();
}

littleLinesValSub CGauge::GetLLVS()
{
	return m_pDGauge->GetLLVS();
}

Color CGauge::GetBigLineColor()
{
	return m_pDGauge->GetBigLineColor();
}

Color CGauge::GetLittleLineColor()
{
	return m_pDGauge->GetLittleLineColor();
}

Color CGauge::GetDigitsColor()
{
	return m_pDGauge->GetDigitsColor();
}

int CGauge::GetFontSize()
{
	return m_pDGauge->GetFontSize();
}

FontStyle CGauge::GetFontStyle()
{
	return m_pDGauge->GetFontStyle();
}

CString CGauge::GetFormatDigits()
{
	return m_pDGauge->GetFormatDigits();
}

Color CGauge::GetUnderLineColor()
{
	return m_pDGauge->GetUnderLineColor();
}

int CGauge::GetWidthGradDialLine()
{
	return m_pDGauge->GetWidthGradDialLine();
}

float CGauge::GetRedPercentGradDialLine()
{
	return m_pDGauge->GetRedPercentGradDialLine();
}

float CGauge::GetYellowPercentGradDialLine()
{
	return m_pDGauge->GetYellowPercentGradDialLine();
}

float CGauge::GetGreenPercentGradDialLine()
{
	return m_pDGauge->GetGreenPercentGradDialLine();
}

Color CGauge::GetGradientDialTone()
{
	return m_pDGauge->GetGradientDialTone();
}