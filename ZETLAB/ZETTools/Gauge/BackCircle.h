#pragma once
#include "CircleObject.h"



class BackCircle : public CircleObject
{
public:
	BackCircle::BackCircle(int X, int Y, int diam) : CircleObject(X, Y, diam)
	{
		m_shadowDiam = diam - 8;
		m_shadowStart = (diam - m_shadowDiam) / 2;
		m_shadowColor = Color(255, 230, 230, 230);

	}

	BackCircle::BackCircle() : CircleObject()
	{
		m_shadowDiam = 193;
		m_shadowStart = 4;
		m_shadowColor = Color(255, 230, 230, 230);
		m_style = VS_SURROUND;//BCS_FLAT;
		m_drawBorder = false;
		//m_backColor = Color(255, 100, 100, 100);
	}

	virtual void DrawObject(Graphics &graph);


	void SetShadowColor(Color shadowColor)
	{
		CheckAndSet_Reset(shadowColor, m_shadowColor);
	}

	void SetShadowStart(int shadowStart)
	{
		CheckAndSet_Reset(shadowStart, m_shadowStart);
	}

	void SetShadowDiam(int shadowDiam)
	{
		CheckAndSet_Reset(shadowDiam, m_shadowDiam);
	}

	void SetStyle(VolumeStyle bcs)
	{
		CheckAndSet_Reset(bcs, m_style);
	}




	Color GetShadowColor()
	{
		return m_shadowColor;
	}

	int GetShadowStart()
	{
		return m_shadowStart;
	}

	int GetShadowDiam()
	{
		return m_shadowDiam;
	}

	VolumeStyle GetStyle()
	{
		return m_style;
	}

private:

	Color m_shadowColor;	//цвет тени
	int m_shadowStart;
	int m_shadowDiam;

	VolumeStyle m_style;

};
