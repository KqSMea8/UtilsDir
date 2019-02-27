#include "stdafx.h"
#include "Needle.h"



void Needle::NeedleInit()
{
	m_needleLength = 74;
	m_needleWidth = 6;

	m_needleArrowWidth = 1;
	m_needleStyle = NS_2;

	m_needleCircleDiam = 34;

	m_needleColor = Color(255, 250, 0, 10);	//цвет стрелки
	m_backColor = Color(255, 180, 180, 180);

	m_drawBorder = false;
}


void Needle::DrawObject(Graphics &graph)
{
	if(!m_needleBmp)
		DrawNeedle(graph);

	if(!m_circleBmp)
		DrawCircle(graph);
}




void Needle::DrawNeedle(Graphics &graph)
{
	m_needleBmp = boost::make_shared<Bitmap>(m_bitmapSize, m_bitmapSize, &graph);
	Graphics needleGR(m_needleBmp.get());

	needleGR.SetSmoothingMode(SmoothingModeAntiAlias);


	if(m_needleStyle == NS_1 || m_needleStyle == NS_4 || m_needleStyle == NS_5 || m_needleStyle == NS_7 )
	{
		CheckAndResize(4);

		m_needleCurvPoints[0] = Point(m_diametr / 2, m_diametr / 2 + m_needleCircleDiam / m_needleWidth);
		m_needleCurvPoints[1] = Point(m_diametr / 2 - m_needleLength, m_diametr / 2 + m_needleArrowWidth);
		m_needleCurvPoints[2] = Point(m_diametr / 2 - m_needleLength, m_diametr / 2 - m_needleArrowWidth);
		m_needleCurvPoints[3] = Point(m_diametr / 2, m_diametr / 2 - m_needleCircleDiam / m_needleWidth);
	}
	else
		if(m_needleStyle == NS_2)
		{
			CheckAndResize(4);

			m_needleCurvPoints[0] = Point((int)(m_diametr / 2 + m_needleCircleDiam / 1.5f), (int)(m_diametr / 2 + m_needleCircleDiam / m_needleWidth));
			m_needleCurvPoints[1] = Point((int)(m_diametr / 2 - m_needleLength), (int)(m_diametr / 2 + m_needleArrowWidth));
			m_needleCurvPoints[2] = Point((int)(m_diametr / 2 - m_needleLength), (int)(m_diametr / 2 - m_needleArrowWidth));
			m_needleCurvPoints[3] = Point((int)(m_diametr / 2 + m_needleCircleDiam / 1.5f), (int)(m_diametr / 2 - m_needleCircleDiam / m_needleWidth));
		}
		else
			if(m_needleStyle == NS_3 || m_needleStyle == NS_6)
			{
				CheckAndResize(6);
				m_needleCurvPoints[0] = Point((int)(m_diametr / 2), (int)(m_diametr / 2 + m_needleCircleDiam / m_needleWidth));
				m_needleCurvPoints[1] = Point((int)(m_diametr / 2 - m_needleLength / 1.2f), (int)(m_diametr / 2 + m_needleCircleDiam / m_needleWidth));
				m_needleCurvPoints[2] = Point((int)(m_diametr / 2 - m_needleLength), (int)(m_diametr / 2 + m_needleArrowWidth));
				m_needleCurvPoints[3] = Point((int)(m_diametr / 2 - m_needleLength), (m_diametr / 2 - m_needleArrowWidth));
				m_needleCurvPoints[4] = Point((int)(m_diametr / 2 - m_needleLength / 1.2f), (int)(m_diametr / 2 - m_needleCircleDiam / m_needleWidth));
				m_needleCurvPoints[5] = Point((int)(m_diametr / 2), (int)(m_diametr / 2 - m_needleCircleDiam / m_needleWidth));

			}

			Rect gradientRect;
			if(m_needleStyle == NS_4)
			{
				gradientRect = Rect(m_diametr / 2 - m_needleLength, m_diametr / 2 + m_needleCircleDiam / m_needleWidth, m_needleLength, m_needleCircleDiam / m_needleWidth);
				needleGR.FillClosedCurve(&LinearGradientBrush(gradientRect, m_needleColor, Color(255, 250, 250, 250), LinearGradientModeVertical), m_needleCurvPoints.data(), m_needleCurvPoints.size(), FillModeWinding, (REAL)0.05);
			}

			else
				if(m_needleStyle == NS_2)
				{
					gradientRect = Rect(m_diametr / 2 - m_needleLength - 5, m_diametr / 2 + m_needleCircleDiam / m_needleWidth, m_needleLength + m_needleCircleDiam + 5, (m_needleCircleDiam / m_needleWidth) * 2);

					LinearGradientBrush lgb(gradientRect, m_needleColor, Color(255, 250, 250, 250), LinearGradientModeHorizontal);
					lgb.SetBlendTriangularShape((REAL)0.74, (REAL)0.8);

					needleGR.FillClosedCurve(&lgb, m_needleCurvPoints.data(), m_needleCurvPoints.size(), FillModeWinding, (REAL)0.15);
				}
				else
					if(m_needleStyle == NS_6 || m_needleStyle == NS_5)
					{
						gradientRect = Rect(m_diametr / 2 - m_needleLength - 5, m_diametr / 2 + m_needleCircleDiam / m_needleWidth, m_needleLength + 5, m_needleCircleDiam / m_needleWidth);
						needleGR.FillClosedCurve(&LinearGradientBrush(gradientRect, m_needleColor, Color(255, 250, 250, 250), LinearGradientModeHorizontal), m_needleCurvPoints.data(), m_needleCurvPoints.size(), FillModeWinding, (REAL)0.05);
					}
					else
						if(m_needleStyle == NS_7)
							needleGR.FillClosedCurve(&SolidBrush(Color(30, 125, 249, 255)), m_needleCurvPoints.data(), m_needleCurvPoints.size(), FillModeWinding, (REAL)0.05);
						else
							needleGR.FillClosedCurve(&SolidBrush(m_needleColor), m_needleCurvPoints.data(), m_needleCurvPoints.size(), FillModeWinding, (REAL)0.05);

			if(m_needleStyle == NS_2)
			{
				needleGR.DrawClosedCurve(&Pen(m_backColor, 2), m_needleCurvPoints.data(), m_needleCurvPoints.size(), (REAL)0.15);	
			}

			else
				if(m_needleStyle == NS_7)
					needleGR.DrawClosedCurve(&Pen(&SolidBrush(Color(200, m_needleColor.GetR(), m_needleColor.GetG(), m_needleColor.GetB())), 1.5), m_needleCurvPoints.data(), m_needleCurvPoints.size(), (REAL)0.05);
				else
					needleGR.DrawClosedCurve(&Pen(m_backColor), m_needleCurvPoints.data(), m_needleCurvPoints.size(), (REAL)0.05);
}




void Needle::DrawCircle(Graphics &graph)
{
	m_circleBmp = boost::make_shared<Bitmap>(m_bitmapSize, m_bitmapSize, &graph);
	Graphics circleGR(m_circleBmp.get());


	circleGR.SetSmoothingMode(SmoothingModeAntiAlias);

	int circleStart = m_diametr / 2 - m_needleCircleDiam / 2;


	circleGR.FillEllipse(&SolidBrush(m_backColor), circleStart, circleStart, m_needleCircleDiam, m_needleCircleDiam);

	if(m_drawBorder)
		circleGR.DrawEllipse(&Pen(&SolidBrush(m_borderColor), 1), circleStart, circleStart, m_needleCircleDiam, m_needleCircleDiam);


	GraphicsPath gp;
	gp.AddEllipse(circleStart + 3, circleStart + 3, m_needleCircleDiam - 6, m_needleCircleDiam - 6);

	PathGradientBrush pgb(&gp);
	int countCol = 1;

	Color col = Color(255, 240, 240, 240);
	pgb.SetCenterColor(col);
	pgb.SetSurroundColors(&m_needleColor, &countCol);
	pgb.SetCenterPoint
		(
			Point(	(int)(m_diametr / 2 + cos(2.35f + m_currentAngle * 0.017444f) * ((m_needleCircleDiam) / 2)), 
					(int)(m_diametr / 2 - sin(2.35f + m_currentAngle * 0.017444f) * ((m_needleCircleDiam) / 2)) 
				 )
		);

	pgb.SetFocusScales(0.1f, 0.1f);

	circleGR.DrawEllipse(&Pen(m_needleColor), circleStart + 3, circleStart + 3, m_needleCircleDiam - 6, m_needleCircleDiam - 6);
	circleGR.FillPath(&pgb, &gp);


	gp.Reset();
	gp.AddEllipse(circleStart + 7, circleStart + 7, m_needleCircleDiam - 14, m_needleCircleDiam - 14);
	pgb.SetGraphicsPath(&gp);
	pgb.SetCenterPoint
		(
		Point(	(int)(m_diametr / 2 + cos(2.35f + m_currentAngle * 0.017444f) * ((m_needleCircleDiam - 6) / 2)),
				(int)(m_diametr / 2 - sin(2.35f + m_currentAngle * 0.017444f) * ((m_needleCircleDiam - 6) / 2))
			 )
		);
	pgb.SetSurroundColors(&m_backColor, &countCol);

	circleGR.FillPath(&pgb, &gp);
	circleGR.DrawEllipse(&Pen(m_backColor, 1), circleStart + 7, circleStart + 7, m_needleCircleDiam - 14, m_needleCircleDiam - 14);
}






void Needle::CheckAndResize(int vectorSize)
{
	if(m_needleCurvPoints.size() != vectorSize)
	{
		m_needleCurvPoints.clear();
		m_needleCurvPoints.resize(vectorSize);
	}
}


void Needle::CheckBmpAndReset()
{
	if(!m_bitmap)
	{
		m_needleBmp.reset();
		m_circleBmp.reset();
	}
}