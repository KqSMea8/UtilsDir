
#pragma once
#include "C:\ZETTools\Interface2012\Custom\CustomElement.h"
#include "C:\ZETTools\Interface2012\Draw\DGauge.h"
//------------------------------------------------------------------------------


class CGauge : public CCustomElement
{
public:
	CGauge(CCustomContainer *owner, CRect rect);
	virtual ~CGauge();

private:
	DGauge *m_pDGauge;

public:

public: //GetSetMethods------------------------------------------------------------------


//--------------Set Methods--------------------------------------------------------------

	void SetCurrentValue(float currVal);
	void SetBorderDraw(bool borderDraw);
	void SetBorderColor(Color borderColor);
	void SetGaugeColor(Color gaugeColor);
	void SetStyle(VolumeStyle gs);

	//Dial
	void SetDialBackColor(Color dialBackColor);
	void SetBigLineLength(int biglineLength);
	void SetLittleLineLength(int littleLineLength);
	void SetBigLineWidth(int bigLineWidth);
	void SetlittleLineWidth(int littleLineWidth);
	void SetUnderLineWidth(int underlineWidth);
	void SetAngleFrom(int angleFrom);
	void SetAngleTo(int angleTo);
	void SetValueFrom(float valueFrom);
	void SetValueTo(float valueTo);
	void SetMulti(int multi);
	void SetUnderlineVisible(bool ulineVisible);
	void SetDigitsVisible(bool digitsVisible);
	void SetGradientDialType(gradientDialType gDVisible);
	void SetDigitsPos(digitsPos digitspos);
	void SetLLVS(littleLinesValSub llvs);
	void SetBigLineColor(Color bigLineColor);
	void SetLittleLineColor(Color littleLineColor);
	void SetDigitsColor(Color digitsColor);
	void SetFontSize(int fontSize);
	void SetFontStyle(FontStyle fontStyle);
	void SetFormatDigits(CString formatStr);
	void SetunderlineColor(Color underLineColor);
	void SetWidthGradDialLine(int widthGradDialLine);
	void SetRedPercentGradDialLine(float redPercentGradDialLine);
	void SetYellowPercentGradDialLine(float yellowPercentGradDialLine);
	void SetGreenPercentGradDialLine(float greenPercentGradDialLine);
	void SetGradientDialTone(Color gradientDialTone);

	//Needle
	void SetNeedleLength(int needleLength);
	void SetNeedleWidth(int needleWidth);
	void SetNeedleArrowWidth(int needleArrowWidth);
	void SetNeedleStyle(needleStyle needleSt);
	void SetNeedleColor(Color needleColor);


//------------------------Get Methods----------------------------------------------------------------

	float GetCurrentValue();
	bool GetBorderDraw();
	Color GetBorderColor();
	VolumeStyle GetStyle();
	Color GetGaugeColor();
	

	//Dial
	Color GetDialBackColor();
	int GetBigLineLength();
	int GetLittleLineLength();
	int GetBigLineWidth();
	int GetlittleLineWidth();
	int GetUnderLineWidth();
	int GetAngleFrom();
	int GetAngleTo();
	float GetValueFrom();
	float GetValueTo();
	int GetMulti();
	bool GetUnderlineVisible();
	bool GetDigitsVisible();
	gradientDialType GetGradientDialType();
	digitsPos GetDigitsPos();
	littleLinesValSub GetLLVS();
	Color GetBigLineColor();
	Color GetLittleLineColor();
	Color GetDigitsColor();
	int GetFontSize();
	FontStyle GetFontStyle();
	CString GetFormatDigits();
	Color GetUnderLineColor();
	int GetWidthGradDialLine();
	float GetRedPercentGradDialLine();
	float GetYellowPercentGradDialLine();
	float GetGreenPercentGradDialLine();
	Color GetGradientDialTone();

	//Needle
	int GetNeedleLength();
	int GetNeedleWidth();
	int GetNeedleArrowWidth();
	needleStyle GetNeedleStyle();
	Color GetNeedleColor();

};