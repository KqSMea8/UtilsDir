#pragma once
#include "CircleObject.h"
#include <vector>

enum needleStyle
{
	NS_1,
	NS_2,
	NS_3,
	NS_4,
	NS_5, 
	NS_6,
	NS_7

};

class Needle : public CircleObject
{
public:
	Needle::Needle(int X, int Y, int diam) : CircleObject(X, Y, diam)
	{
		NeedleInit();
	}

	Needle::Needle() : CircleObject()
	{
		m_diametr = 173;
		m_x = 14;
		m_y = 14;
		NeedleInit();
	}

	Needle::~Needle()
	{
		DeleteObject(m_needleBmp.get());
		DeleteObject(m_circleBmp.get());
		m_needleBmp.reset();
		m_circleBmp.reset();
	}


	virtual void DrawObject(Graphics &graph);


	void SetNeedleLength(int needleLength)
	{
		CheckAndSet_Reset(needleLength, m_needleLength);
		CheckBmpAndReset();
	}

	void SetNeedleWidth(int needleWidth)
	{
		CheckAndSet_Reset(needleWidth, m_needleWidth);
		CheckBmpAndReset();
	}

	void SetNeedleArrowWidth(int needleArrowWidth)
	{
		CheckAndSet_Reset(needleArrowWidth, m_needleArrowWidth);
		CheckBmpAndReset();
	}

	void SetNeedleStyle(needleStyle needleSt)
	{
		CheckAndSet_Reset(needleSt, m_needleStyle);
		CheckBmpAndReset();
	}

	void SetNeedleCircleDiam(int needleCircleDiam)
	{
		CheckAndSet_Reset(needleCircleDiam, m_needleCircleDiam);
		CheckBmpAndReset();
	}

	void SetNeedleColor(Color needleColor)
	{
		CheckAndSet_Reset(needleColor, m_needleColor);
		CheckBmpAndReset();
	}

	void SetCurrentAngle(float currentAngle)
	{
		CheckAndSet_Reset(currentAngle, m_currentAngle);
		CheckBmpAndReset();
	}




	int GetNeedleLength()
	{
		return m_needleLength;
	}

	int GetNeedleWidth()
	{
		return m_needleWidth;
	}

	int GetNeedleArrowWidth()
	{
		return m_needleArrowWidth;
	}

	needleStyle GetNeedleStyle()
	{
		return m_needleStyle;
	}

	int GetNeedleCircleDiam()
	{
		return m_needleCircleDiam;
	}

	float GetCurrentAngle()
	{
		return m_currentAngle;
	}

	Color GetNeedleColor()
	{
		return m_needleColor;
	}

	Bitmap* GetCircleBmp()
	{
		if(m_circleBmp)
			return m_circleBmp.get();

		return NULL;
	}

	Bitmap* GetNeedleBmp()
	{
		if(m_needleBmp)
			return m_needleBmp.get();

		return NULL;
	}

private:

	void NeedleInit();

	int m_needleLength;			// ����� �������
	int m_needleWidth;			//������ � ������ (�� ���� �������� ������� m_needleCircleDiam ��� ����������� ��������� ������� ��� ���������������)
	
	int m_needleArrowWidth;		// ������ ������� ������� (���� �������� ���������� �� 2);
	needleStyle m_needleStyle;	// �����

	int m_needleCircleDiam;		//������� ����������, �� ������� ������ ���������

	float m_currentAngle;		//����, �� ������� ��������� �������

	Color m_needleColor;		//���� �������

	std::vector<Point> m_needleCurvPoints;

	
	
	Bitmap_ptr m_needleBmp;
	Bitmap_ptr m_circleBmp;

	void CheckBmpAndReset();
	void CheckAndResize(int vectorSize);

	void DrawNeedle(Graphics &graph);
	void DrawCircle(Graphics &graph);
};