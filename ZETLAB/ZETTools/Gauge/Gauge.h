#pragma once

#include "BackCircle.h"
#include "Dial.h"
#include "Needle.h"



class Gauge
{
public:
	Gauge::Gauge();
	Gauge::Gauge(int X, int Y, int W, int H);
	virtual Gauge::~Gauge();
	void Init();

	void DrawObject(Graphics& gr);

private:

	boost::shared_ptr<BackCircle> m_pBackCircle;
	boost::shared_ptr<Dial> m_pDial;
	boost::shared_ptr<Needle> m_pNeedle;
	boost::shared_ptr<Bitmap> m_pGlassGlare; 
	

	Rect m_rcBounds;

	void DrawGlassGlare(Graphics& graph);

	enum rotatetype
	{
		RT_PLUS,
		RT_MINUS
	};

	void RotateGr(Graphics &gr, int &diametr, float angle, rotatetype rt);

	float m_currentValue;

	VolumeStyle m_gaugeStyle;

	boost::shared_ptr<Color> m_pBackColor; // для скады.
	Color m_3dFace;

public: //GetSetMethods------------------------------------------------------------------


//--------------Set Methods--------------------------------------------------------------

	void SetRect(const int &x, const int &y, const int &w, const int &h);
	void SetCurrentValue(float currVal);
	void SetBorderDraw(bool borderDraw);
	void SetBorderColor(Color borderColor);
	void SetBackColor(Color backColor);
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
	void SetCurrentAngle(float currentAngle);


//------------------------Get Methods----------------------------------------------------------------

	float GetAngleFromCurrentValue(float currVal);

	float GetCurrentValue();
	Rect GetRect();
	bool GetBorderDraw();
	Color GetBorderColor();
	Color* GetBackColor();
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