#pragma once
#include "CircleObject.h"


enum digitsPos
{
	DP_DOWN,	//����� ��� �������
	DP_UP		//����� ��� �������
};

enum littleLinesValSub	
{
	LLVS_1 = 1,
	LLVS_2 = 2,
	LLVS_4 = 4,
	LLVS_5 = 5,
	LLVS_10 = 10
};


enum gradientDialType
{
	GDT_INVIS,	// ����
	GDT_APART_GRADIENT,	// ��������, �����������(������� ������� ������)
	GDT_APART_HARD,		// ��������, ������ ������� ������ ( �������, ������, �������), ������ ���� �������� � ���������� �����������.
	GDT_APART_HARD_ONLYRED, //��������, ����������� ������ ������� ����, ������ ���� �������� � ���������� �����������
	GDT_LINES,	// ����� �������, ������ ���� �������� � ���������� �����������.
	GDT_LINES_ONLYRED //����� �������, ����������� ������ ������� ����, ������ ���� �������� � ���������� �����������
};



class Dial : public CircleObject
{
public:
	Dial::Dial(int X, int Y, int diam) : CircleObject(X, Y, diam)
	{
		DialInit();
	}

	Dial::Dial() : CircleObject()
	{
		m_diametr = 173;
		m_x = 14;
		m_y = 14;
		DialInit();
	}

	virtual Dial::~Dial()
	{
		DeleteObject(m_pFontFamily);
		delete m_pFontFamily;
		m_pFontFamily = nullptr;
	}

	virtual void DrawObject(Graphics &graph);


	void SetBigLineLength(int biglineLength)
	{
		CheckAndSet_Reset(biglineLength, m_bigLineLength);
	}

	void SetLittleLineLength(int littleLineLength)
	{
		CheckAndSet_Reset(littleLineLength, m_littleLineLength);
	}

	void SetBigLineWidth(int bigLineWidth)
	{
		CheckAndSet_Reset(bigLineWidth, m_bigLineWidth);
	}

	void SetlittleLineWidth(int littleLineWidth)
	{
		CheckAndSet_Reset(littleLineWidth, m_littleLineWidth);
	}

// 	void SetOffsetLines(int offsetLines)
// 	{
// 		CheckAndSet_Reset(offsetLines, DIAL_OFFSET);
// 	}

	void SetUnderLineWidth(int underlineWidth)
	{
		CheckAndSet_Reset(underlineWidth, m_underLineWidth);
	}

	void SetAngleFrom(int angleFrom)
	{
		CheckAndSet_Reset(angleFrom, m_angleFrom);
	}

	void SetAngleTo(int angleTo)
	{
		CheckAndSet_Reset(angleTo, m_angleTo);
	}

	void SetValueFrom(float valueFrom)
	{
		CheckAndSet_Reset(valueFrom, m_valueFrom);
	}

	void SetValueTo(float valueTo)
	{
		CheckAndSet_Reset(valueTo, m_valueTo);
	}
	
	void SetMulti(int multi)
	{
		CheckAndSet_Reset(multi, m_multi);
	}
	
	void SetUnderlineVisible(bool ulineVisible)
	{
		CheckAndSet_Reset(ulineVisible, m_underlineVisible);
	}
	
	void SetDigitsVisible(bool digitsVisible)
	{
		CheckAndSet_Reset(digitsVisible, m_digitsVisible);
	}
	
	void SetGradientDialType(gradientDialType gDVisible)
	{
		CheckAndSet_Reset(gDVisible, m_gradDialType);
	}
	
	void SetDigitsPos(digitsPos digitspos)
	{
		CheckAndSet_Reset(digitspos, m_digitsPos);
	}
	
	void SetLLVS(littleLinesValSub llvs)
	{
		CheckAndSet_Reset(llvs, m_llvs);
	}

	void SetBigLineColor(Color bigLineColor)
	{
		CheckAndSet_Reset(bigLineColor, m_bigLineColor);
	}

	void SetLittleLineColor(Color littleLineColor)
	{
		CheckAndSet_Reset(littleLineColor, m_littleLineColor);
	}

	void SetDigitsColor(Color digitsColor)
	{
		CheckAndSet_Reset(digitsColor, m_digitsColor);
	}

	void SetFontSize(int fontSize)
	{
		CheckAndSet_Reset(fontSize, m_fontSize);
	}

	void SetFontStyle(FontStyle fontStyle)
	{
		CheckAndSet_Reset(fontStyle, m_fontStyle);
	}

	void SetFormatDigits(CString formatStr)
	{
		CheckAndSet_Reset(formatStr, m_formatDigits);
	}

	void SetStyle(VolumeStyle ds)
	{
		CheckAndSet_Reset(ds, m_style);
	}

	void SetunderlineColor(Color underLineColor)
	{
		CheckAndSet_Reset(underLineColor, m_underLineColor);
	}

	void SetWidthGradDialLine(int widthGradDialLine)
	{
		CheckAndSet_Reset(widthGradDialLine, m_widthGradDialLine);
	}

	void SetRedPercentGradDialLine(float redPercentGradDialLine)
	{
		float redAngle = (float)(m_angleTo - m_angleFrom) / 100.0f * redPercentGradDialLine;
		CheckAndSet_Reset(redAngle, m_redAngleGradDialLine);
	}

	void SetYellowPercentGradDialLine(float yellowPercentGradDialLine)
	{
		float yellowAngle = (float)(m_angleTo - m_angleFrom) / 100.0f * yellowPercentGradDialLine;
		CheckAndSet_Reset( yellowAngle, m_yellowAngleGradDialLine);
	}

	void SetGreenPercentGradDialLine(float greenPercentGradDialLine)
	{
		float greenAngle = (float)(m_angleTo - m_angleFrom) / 100.0f * greenPercentGradDialLine;
		CheckAndSet_Reset(greenAngle, m_greenAngleGradDialLine);
	}

	void SetGradientDialTone(Color gradientDialTone)
	{
		CheckAndSet_Reset(gradientDialTone, m_gradientDialTone);
	}








	int GetBigLineLength()
	{
		return m_bigLineLength;
	}
	int GetLittleLineLength()
	{
		return m_littleLineLength;
	}
	int GetBigLineWidth()
	{
		return m_bigLineWidth;
	}
	int GetlittleLineWidth()
	{
		return m_littleLineWidth;
	}
// 	int GetOffsetlines()
// 	{
// 		return DIAL_OFFSET;
// 	}
	int GetUnderLineWidth()
	{
		return m_underLineWidth;
	}
	int GetAngleFrom()
	{
		return m_angleFrom;
	}
	int GetAngleTo()
	{
		return m_angleTo;
	}
	float GetValueFrom()
	{
		return m_valueFrom;
	}
	float GetValueTo()
	{
		return m_valueTo;
	}
	int GetMulti()
	{
		return m_multi;
	}
	bool GetUnderlineVisible()
	{
		return m_underlineVisible;
	}
	bool GetDigitsVisible()
	{
		return m_digitsVisible;
	}
	gradientDialType GetGradientDialType()
	{
		return m_gradDialType;
	}
	digitsPos GetDigitsPos()	
	{
		return m_digitsPos;
	}
	littleLinesValSub GetLLVS()
	{
		return m_llvs;
	}

	Color GetBigLineColor()
	{
		return m_bigLineColor;
	}

	Color GetLittleLineColor()
	{
		return m_littleLineColor;
	}

	Color GetDigitsColor()
	{
		return m_digitsColor;
	}

	int GetFontSize()
	{
		return m_fontSize;
	}

	FontStyle GetFontStyle()
	{
		return m_fontStyle;
	}

	CString GetFormatDigits()
	{
		return m_formatDigits;
	}

	VolumeStyle GetStyle()
	{
		return m_style;
	}

	Color GetUnderLineColor()
	{
		return m_underLineColor;
	}


	int GetWidthGradDialLine()
	{
		return m_widthGradDialLine;
	}

	float GetRedPercentGradDialLine()
	{
		return m_redAngleGradDialLine / (float)(m_angleTo - m_angleFrom) * 100;
	}

	float GetYellowPercentGradDialLine()
	{
		return m_yellowAngleGradDialLine / (float)(m_angleTo - m_angleFrom) * 100;
	}

	float GetGreenPercentGradDialLine()
	{
		return m_greenAngleGradDialLine / (float)(m_angleTo - m_angleFrom) * 100;
	}

	Color GetGradientDialTone()
	{
		return m_gradientDialTone;
	}


	float GetAngleFromCurrentValue(float currVal) // ���������� �������� ���� �������.
	{
		if(currVal > m_valueTo)
			currVal = m_valueTo;
		if(currVal < m_valueFrom)
			currVal = m_valueFrom;

		//return (float)((float)m_angleFrom + ((float)((float)m_angleTo - (float)m_angleFrom)*(currVal - m_valueFrom)/((float)((m_countLines - 1) * m_valStep) /*- m_valueFrom*/)));
		return (float)((float)m_angleFrom + ((float)((float)m_angleTo - (float)m_angleFrom)*(currVal - (int)(m_valueFrom / m_valStep) * m_valStep)/((float)((m_countLines - 1) * m_valStep) /*- m_valueFrom*/)));
	}

	float GetValueFromAngle(float currAngle)
	{
// 		if(currAngle > (float)m_angleTo)
// 			currAngle = (float)m_angleTo;
// 		if(currAngle < (float)m_angleFrom)
// 			currAngle = (float)m_angleFrom;
// 		
// 		return  m_valueFrom + (currAngle - m_angleFrom) / (m_angleTo - m_angleFrom) * (m_valueTo - m_valueFrom); 
		return 0;
	}


private:

	#define DIAL_OFFSET 2			// ������ ����� �� ���� ����������

	void DialInit();

	int m_bigLineLength;		// ����� ������� �����
	int m_littleLineLength;		// ����� ��������� �����

	int m_bigLineWidth;			//������ ������� �����
	int m_littleLineWidth;		//������ ��������� �����

	int m_underLineWidth;		// ������ �������������� �����

	int m_angleFrom;			//��������� �������� ����
	int m_angleTo;				//�������� �������� ����
	float m_valueFrom;			//��������� �������� ��������	
	float m_valueTo;			//�������� �������� ��������

	int m_multi;				//���������, �������� �10

	bool m_underlineVisible;	// ���� true, �� ������������ �����
	bool m_digitsVisible;		// ���� true, �� ����� ������������

	gradientDialType m_gradDialType;// ��� �����, ������������ ����.

	int m_widthGradDialLine;		// ������ �����, ������������ ����

	float m_redAngleGradDialLine;	// ���� ������� ����
	float m_yellowAngleGradDialLine;// ���� ������ ����
	float m_greenAngleGradDialLine;	// ���� ������� ����.


	digitsPos m_digitsPos;		// ���� 0, �� ����� ������ �����, ���� 1, �� ����� ��� ������.
	littleLinesValSub m_llvs;	// ��������, ������������, �� ������� ��� ��� ����� �������� ������� ������ ���� ����� ���������� (������ �� ������� ��������� ��������� �����).

	Color m_bigLineColor;		//���� ������� �����
	Color m_littleLineColor;	//���� ��������� �����.
	Color m_digitsColor;		//���� ����
	Color m_endLineColor;		//���� ��������� �����.
	Color m_underLineColor;		//���� �������������� �����
	Color m_gradientDialTone;	//������� ����������� �����


	int m_fontSize;				// ������ ������
	FontStyle m_fontStyle;		// ����� ������
	FontFamily *m_pFontFamily;

	CString m_formatDigits;			// ������ ���������� ��������������.

	VolumeStyle m_style;

	int m_countLines;
	float m_valStep;

	template < typename T>
	void CheckValues(T &minVal, T &maxVal);
	PointF GetDigitPoint(float &currAngle, int &digitsOffs, RectF &rectf);
	void DrawMinMaxLines(Graphics &gr, int xpos, int width);
	void DrawDialLines(Graphics &gr, Color &lineColor, int &lineWidth, int &linelength, int x1, int x2, float &currentAngle);
	void DrawGradDial(Graphics &gr);
	void DrawRiscs(Graphics &gr, RectF &rect, int digitsOffs, Gdiplus::Font* font, StringFormat &format);
	void DrawUnderline(Graphics &gr, RectF &rect, int digitsOffs);
	void CheckZone(float &currAngle, Color &currColor);
};