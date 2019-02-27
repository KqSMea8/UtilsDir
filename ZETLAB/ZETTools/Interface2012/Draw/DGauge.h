//Класс отрисовки стрелочного индикатора

#pragma once
#include "C:\ZETTools\Interface2012\Draw\CustomDraw.h"
#include "C:\ZETTools\Gauge\Gauge.h"


struct ParamGauge : public ParamCustomDraw
{// Структура параметров отображения 
	// В родительской структуре - общие параметры, здесь - частные параметры
	float currVal;
	bool borderDraw;
	Color borderColor;
	Color backColor;
	Color gaugeColor;
	VolumeStyle gs;

	//Dial
	Color dialBackColor;
	int biglineLength;
	int littleLineLength;
	int bigLineWidth;
	int littleLineWidth;
	int underlineWidth;
	int angleFrom;
	int angleTo;
	float valueFrom;
	float valueTo;
	int multi;
	bool ulineVisible;
	bool digitsVisible;
	gradientDialType gDVisible;
	digitsPos digitspos;
	littleLinesValSub llvs;
	Color bigLineColor;
	Color littleLineColor;
	Color digitsColor;
	int fontSize;
	FontStyle fontStyle;
	CString formatStr;
	Color underLineColor;
	int widthGradDialLine;
	float redPercentGradDialLine;
	float yellowPercentGradDialLine;
	float greenPercentGradDialLine;
	Color gradientDialTone;

	//Needle
	int needleLength;
	int needleWidth;
	int needleArrowWidth;
	needleStyle needleSt;
	Color needleColor;
	float currentAngle;

	ParamGauge() : ParamCustomDraw()
	{
		Size = sizeof(ParamGauge);
	}
};


class DGauge : public CCustomDraw, public Gauge
{

public:

	DGauge(CRect rect, PVOID ppGraphics,
		bool *pFlagIsChange, double *pTimeAnimation);
	virtual ~DGauge();

	// виртуальные функции, переопределяемые в классе
	virtual void OnDraw();

private:

	ParamGauge m_param; // структура всех параметров отображения

	GraphicsPath *m_backLightGp;
	PathGradientBrush *pPgb;
	void initLight();

public: //GetSetMethods------------------------------------------------------------------

	bool SetStruct(PVOID add);	// копирование параметров из add в структуру
	bool GetStruct(PVOID add);	// копирование параметров из структуры в add

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
	//void SetNeedleCircleDiam(int needleCircleDiam);
	void SetNeedleColor(Color needleColor);

};