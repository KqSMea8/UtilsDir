
#include "stdafx.h"
#include "DGauge.h"
//------------------------------------------------------------------------------
int g_nGauge = 0;		// счётчик стрелочных индикаторов
//------------------------------------------------------------------------------
DGauge::DGauge(CRect rect, PVOID ppGraphics, bool *pFlagIsChange, double *pTimeAnimation) : CCustomDraw(rect, ppGraphics, pFlagIsChange, pTimeAnimation)
{

	Gauge::SetGaugeColor(Color(255, 100, 100, 100));
	Gauge::SetDialBackColor(Color(255, 120, 120, 120));
	Gauge::SetNeedleStyle(NS_1);
	Gauge::SetNeedleColor(Color(255,0,0,0));
	Gauge::SetGradientDialType(GDT_APART_HARD_ONLYRED);

	Gauge::SetRect(0,0,m_rect.Width(), m_rect.Height());

	m_backLightGp = NULL;
	pPgb = NULL;

	initLight();

	m_pParamDraw = &m_param;	// обеспечение ОБЩИХ параметров
	// изменение значений общих параметров, заданных по умолчанию
	m_param.nElement = ++g_nGauge;
	SetNameWithNumber(L"Gauge");
}

DGauge::~DGauge()
{
		if(m_backLightGp != NULL)
	{
		DeleteObject(m_backLightGp);
		delete m_backLightGp;
	}

	if(pPgb != NULL)
	{
		DeleteObject(pPgb);
		delete pPgb;
	}
}


void DGauge::OnDraw()
{
	if(Gauge::GetBackColor() == NULL)
	{
		Gauge::SetBackColor(Color(0, 255, 255, 255));
	}

	if(Gauge::GetRect().Width != m_rect.Width() || Gauge::GetRect().Height != m_rect.Height())
	{
		Gauge::SetRect(m_rect.left, m_rect.top, m_rect.Width(), m_rect.Height());

		initLight();
	}
	
 	Graphics *pGr = *(Graphics**)m_ppGraphics;
	pGr->FillEllipse(pPgb, m_rect.left - 4, m_rect.top - 4, m_rect.Width() + 12, m_rect.Height()  + 10);

	Bitmap backBuffer(m_rect.Width(), m_rect.Height(), pGr);
	Graphics temp(&backBuffer);

	Gauge::DrawObject(temp);
	pGr->DrawImage(&backBuffer, m_rect.left, m_rect.top, 0, 0, m_rect.Width(), m_rect.Height(), UnitPixel);
}


void DGauge::initLight()
{
	if(m_backLightGp == NULL)
		m_backLightGp = new GraphicsPath();
	else
		m_backLightGp->Reset();

	if(pPgb != NULL)
		delete pPgb;
	
	Rect rc(Gauge::GetRect());

	m_backLightGp->AddEllipse(m_rect.left - 4, m_rect.top - 4, m_rect.Width() + 12, m_rect.Height()  + 10);

	pPgb = new PathGradientBrush(m_backLightGp);

	pPgb->SetCenterColor(Color(255, 0, 0, 0));

	Color colors[] = {Color(0, 180, 180, 180)};
	int count = 1;
	pPgb->SetSurroundColors(colors, &count);
	pPgb->SetFocusScales(0.80f, 0.80f);
}


bool DGauge::SetStruct(PVOID add)
{
	bool ret = CCustomDraw::SetStruct(add);

	if(!ret)
		return false;

	SetCurrentValue(m_param.currVal);
	SetBorderDraw(m_param.borderDraw);
	SetBorderColor(m_param.borderColor);
	SetGaugeColor(m_param.gaugeColor);
	SetStyle(m_param.gs);
	SetDialBackColor(m_param.dialBackColor);
	SetBigLineLength(m_param.biglineLength);
	SetLittleLineLength(m_param.littleLineLength);
	SetBigLineWidth(m_param.bigLineWidth);
	SetlittleLineWidth(m_param.littleLineWidth);
	SetUnderLineWidth(m_param.underlineWidth);
	SetAngleFrom(m_param.angleFrom);
	SetAngleTo(m_param.angleTo);
	SetValueFrom(m_param.valueFrom);
	SetValueTo(m_param.valueTo);
	SetMulti(m_param.multi);
	SetUnderlineVisible(m_param.ulineVisible);
	SetDigitsVisible(m_param.digitsVisible);
	SetGradientDialType(m_param.gDVisible);
	SetDigitsPos(m_param.digitspos);
	SetLLVS(m_param.llvs);
	SetBigLineColor(m_param.bigLineColor);
	SetLittleLineColor(m_param.littleLineColor);
	SetDigitsColor(m_param.digitsColor);
	SetFontSize(m_param.fontSize);
	SetFontStyle(m_param.fontStyle);
	SetFormatDigits(m_param.formatStr);
	SetunderlineColor(m_param.underLineColor);
	SetWidthGradDialLine(m_param.widthGradDialLine);
	SetRedPercentGradDialLine(m_param.redPercentGradDialLine);
	SetYellowPercentGradDialLine(m_param.yellowPercentGradDialLine);
	SetGreenPercentGradDialLine(m_param.greenPercentGradDialLine);
	SetGradientDialTone(m_param.gradientDialTone);
	SetNeedleLength(m_param.needleLength);
	SetNeedleWidth(m_param.needleWidth);
	SetNeedleArrowWidth(m_param.needleArrowWidth);
	SetNeedleStyle(m_param.needleSt);
	SetNeedleColor(m_param.needleColor);

	return ret;
}

bool DGauge::GetStruct(PVOID add)
{
	m_param.currVal = Gauge::GetCurrentValue();
	m_param.borderDraw = Gauge::GetBorderDraw();
	m_param.borderColor = Gauge::GetBorderColor();
	m_param.gaugeColor = Gauge::GetGaugeColor();
	m_param.gs = Gauge::GetStyle();
	m_param.dialBackColor = Gauge::GetDialBackColor();
	m_param.biglineLength = Gauge::GetBigLineLength();
	m_param.littleLineLength = Gauge::GetLittleLineLength();
	m_param.bigLineWidth = Gauge::GetBigLineWidth();
	m_param.littleLineWidth = Gauge::GetlittleLineWidth();
	m_param.underlineWidth = Gauge::GetUnderLineWidth();
	m_param.angleFrom = Gauge::GetAngleFrom();
	m_param.angleTo = Gauge::GetAngleTo();
	m_param.valueFrom = Gauge::GetValueFrom();
	m_param.valueTo = Gauge::GetValueTo();
	m_param.multi = Gauge::GetMulti();
	m_param.ulineVisible = Gauge::GetUnderlineVisible();
	m_param.digitsVisible = Gauge::GetDigitsVisible();
	m_param.gDVisible = Gauge::GetGradientDialType();
	m_param.digitspos = Gauge::GetDigitsPos();
	m_param.llvs = Gauge::GetLLVS();
	m_param.bigLineColor = Gauge::GetBigLineColor();
	m_param.littleLineColor = Gauge::GetLittleLineColor();
	m_param.digitsColor = Gauge::GetDigitsColor();
	m_param.fontSize = Gauge::GetFontSize();
	m_param.fontStyle = Gauge::GetFontStyle();
	m_param.formatStr = Gauge::GetFormatDigits();
	m_param.underLineColor = Gauge::GetUnderLineColor();
	m_param.widthGradDialLine = Gauge::GetWidthGradDialLine();
	m_param.redPercentGradDialLine = Gauge::GetRedPercentGradDialLine();
	m_param.yellowPercentGradDialLine = Gauge::GetYellowPercentGradDialLine();
	m_param.greenPercentGradDialLine = Gauge::GetGreenPercentGradDialLine();
	m_param.gradientDialTone = Gauge::GetGradientDialTone();
	m_param.needleLength = Gauge::GetNeedleLength();
	m_param.needleWidth = Gauge::GetNeedleWidth();
	m_param.needleArrowWidth = Gauge::GetNeedleArrowWidth();
	m_param.needleSt = Gauge::GetNeedleStyle();
	m_param.needleColor = Gauge::GetNeedleColor();

	return CCustomDraw::SetStruct(add);
}



void DGauge::SetCurrentValue(float currVal)
{
	if(currVal != Gauge::GetCurrentValue())
	{
		Gauge::SetCurrentValue(currVal);
		CCustomDraw::IsChange();
	}
}


void DGauge::SetStyle(VolumeStyle gs)
{
	if(gs != Gauge::GetStyle())
	{
		Gauge::SetStyle(gs);
		CCustomDraw::IsChange();
	}
}


void DGauge::SetBorderDraw(bool borderDraw)
{
	if(borderDraw != Gauge::GetBorderDraw())
	{
		Gauge::SetBorderDraw(borderDraw);
		CCustomDraw::IsChange();
	}
}

void DGauge::SetBorderColor(Color borderColor)
{
	if(borderColor.GetValue() != Gauge::GetBorderColor().GetValue())
	{
		Gauge::SetBorderColor(borderColor);
		CCustomDraw::IsChange();
	}
}

void DGauge::SetGaugeColor(Color gaugeColor)
{
	if(gaugeColor.GetValue() != Gauge::GetGaugeColor().GetValue())
	{
		Gauge::SetGaugeColor(gaugeColor);
		CCustomDraw::IsChange();
	}
}

void DGauge::SetDialBackColor(Color dialBackColor)
{
	if(dialBackColor.GetValue() != Gauge::GetDialBackColor().GetValue())
	{
		Gauge::SetDialBackColor(dialBackColor);
		CCustomDraw::IsChange();
	}
}




void DGauge::SetBigLineLength(int biglineLength)
{
	if(biglineLength != Gauge::GetBigLineLength())
	{
		Gauge::SetBigLineLength(biglineLength);
		CCustomDraw::IsChange();
	}
}

void DGauge::SetLittleLineLength(int littleLineLength)
{
	if(littleLineLength != Gauge::GetLittleLineLength())
	{
		Gauge::SetLittleLineLength(littleLineLength);
		CCustomDraw::IsChange();
	}
}

void DGauge::SetBigLineWidth(int bigLineWidth)
{
	if(bigLineWidth != Gauge::GetBigLineWidth())
	{
		Gauge::SetBigLineWidth(bigLineWidth);
		CCustomDraw::IsChange();
	}
}

void DGauge::SetlittleLineWidth(int littleLineWidth)
{
	if(littleLineWidth != Gauge::GetlittleLineWidth())
	{
		Gauge::SetlittleLineWidth(littleLineWidth);
		CCustomDraw::IsChange();
	}
}

void DGauge::SetUnderLineWidth(int underlineWidth)
{
	if(underlineWidth != Gauge::GetUnderLineWidth())
	{
		Gauge::SetUnderLineWidth(underlineWidth);
		CCustomDraw::IsChange();
	}
}

void DGauge::SetAngleFrom(int angleFrom)
{
	if(angleFrom != Gauge::GetAngleFrom())
	{
		Gauge::SetAngleFrom(angleFrom);
		CCustomDraw::IsChange();
	}
}

void DGauge::SetAngleTo(int angleTo)
{
	if(angleTo != Gauge::GetAngleTo())
	{
		Gauge::SetAngleTo(angleTo);
		CCustomDraw::IsChange();
	}
}

void DGauge::SetValueFrom(float valueFrom)
{
	if(valueFrom != Gauge::GetValueFrom())
	{
		Gauge::SetValueFrom(valueFrom);
		CCustomDraw::IsChange();
	}
}

void DGauge::SetValueTo(float valueTo)
{
	if(valueTo != Gauge::GetValueTo())
	{
		Gauge::SetValueTo(valueTo);
		CCustomDraw::IsChange();
	}
}

void DGauge::SetMulti(int multi)
{
	if(multi != Gauge::GetMulti())
	{
		Gauge::SetMulti(multi);
		CCustomDraw::IsChange();
	}
}

void DGauge::SetUnderlineVisible(bool ulineVisible)
{
	if(ulineVisible != Gauge::GetUnderlineVisible())
	{
		Gauge::SetUnderlineVisible(ulineVisible);
		CCustomDraw::IsChange();
	}
}

void DGauge::SetDigitsVisible(bool digitsVisible)
{
	if(digitsVisible != Gauge::GetDigitsVisible())
	{
		Gauge::SetDigitsVisible(digitsVisible);
		CCustomDraw::IsChange();
	}
}

void DGauge::SetGradientDialType(gradientDialType gDVisible)
{
	if(gDVisible != Gauge::GetGradientDialType())
	{
		Gauge::SetGradientDialType(gDVisible);
		CCustomDraw::IsChange();
	}
}

void DGauge::SetDigitsPos(digitsPos digitspos)
{
	if(digitspos != Gauge::GetDigitsPos())
	{
		Gauge::SetDigitsPos(digitspos);
		CCustomDraw::IsChange();
	}
}

void DGauge::SetLLVS(littleLinesValSub llvs)
{
	if(llvs != Gauge::GetLLVS())
	{
		Gauge::SetLLVS(llvs);
		CCustomDraw::IsChange();
	}
}

void DGauge::SetBigLineColor(Color bigLineColor)
{
	if(bigLineColor.GetValue() != Gauge::GetBigLineColor().GetValue())
	{
		Gauge::SetBigLineColor(bigLineColor);
		CCustomDraw::IsChange();
	}
}

void DGauge::SetLittleLineColor(Color littleLineColor)
{
	if(littleLineColor.GetValue() != Gauge::GetUnderLineColor().GetValue())
	{
		Gauge::SetLittleLineColor(littleLineColor);
		CCustomDraw::IsChange();
	}
}

void DGauge::SetDigitsColor(Color digitsColor)
{
	if(digitsColor.GetValue() != Gauge::GetDigitsColor().GetValue())
	{
		Gauge::SetDigitsColor(digitsColor);
		CCustomDraw::IsChange();
	}
}

void DGauge::SetFontSize(int fontSize)
{
	if(fontSize != Gauge::GetFontSize())
	{
		Gauge::SetFontSize(fontSize);
		CCustomDraw::IsChange();
	}
}

void DGauge::SetFontStyle(FontStyle fontStyle)
{
	if(fontStyle != Gauge::GetFontStyle())
	{
		Gauge::SetFontStyle(fontStyle);
		CCustomDraw::IsChange();
	}
}

void DGauge::SetFormatDigits(CString formatStr)
{
	if(formatStr != Gauge::GetFormatDigits())
	{
		Gauge::SetFormatDigits(formatStr);
		CCustomDraw::IsChange();
	}
}

void DGauge::SetunderlineColor(Color underLineColor)
{
	if(underLineColor.GetValue() != Gauge::GetUnderLineColor().GetValue())
	{
		Gauge::SetunderlineColor(underLineColor);
		CCustomDraw::IsChange();
	}
}

void DGauge::SetWidthGradDialLine(int widthGradDialLine)
{
	if(widthGradDialLine != Gauge::GetWidthGradDialLine())
	{
		Gauge::SetWidthGradDialLine(widthGradDialLine);
		CCustomDraw::IsChange();
	}
}

void DGauge::SetRedPercentGradDialLine(float redPercentGradDialLine)
{
	if(redPercentGradDialLine != Gauge::GetRedPercentGradDialLine())
	{
		Gauge::SetRedPercentGradDialLine(redPercentGradDialLine);
		CCustomDraw::IsChange();
	}
}

void DGauge::SetYellowPercentGradDialLine(float yellowPercentGradDialLine)
{
	if(yellowPercentGradDialLine != Gauge::GetYellowPercentGradDialLine())
	{
		Gauge::SetYellowPercentGradDialLine(yellowPercentGradDialLine);
		CCustomDraw::IsChange();
	}
}

void DGauge::SetGreenPercentGradDialLine(float greenPercentGradDialLine)
{
	if(greenPercentGradDialLine != Gauge::GetGreenPercentGradDialLine())
	{
		Gauge::SetGreenPercentGradDialLine(greenPercentGradDialLine);
		CCustomDraw::IsChange();
	}
}

void DGauge::SetGradientDialTone(Color gradientDialTone)
{
	if(gradientDialTone.GetValue() != Gauge::GetGradientDialTone().GetValue())
	{
		Gauge::SetGradientDialTone(gradientDialTone);
		CCustomDraw::IsChange();
	}
}




void DGauge::SetNeedleLength(int needleLength)
{
	if(needleLength != Gauge::GetNeedleLength())
	{
		Gauge::SetNeedleLength(needleLength);
		CCustomDraw::IsChange();
	}
}

void DGauge::SetNeedleWidth(int needleWidth)
{
	if(needleWidth != Gauge::GetUnderLineWidth())
	{
		Gauge::SetNeedleWidth(needleWidth);
		CCustomDraw::IsChange();
	}
}

void DGauge::SetNeedleArrowWidth(int needleArrowWidth)
{
	if(needleArrowWidth != Gauge::GetNeedleArrowWidth())
	{
		Gauge::SetNeedleArrowWidth(needleArrowWidth);
		CCustomDraw::IsChange();
	}
}

void DGauge::SetNeedleStyle(needleStyle needleSt)
{
	if(needleSt != Gauge::GetNeedleStyle())
	{
		Gauge::SetNeedleStyle(needleSt);
		CCustomDraw::IsChange();
	}
}

void DGauge::SetNeedleColor(Color needleColor)
{
	if(needleColor.GetValue() != Gauge::GetNeedleColor().GetValue())
	{
		Gauge::SetNeedleColor(needleColor);
		CCustomDraw::IsChange();
	}	
}
