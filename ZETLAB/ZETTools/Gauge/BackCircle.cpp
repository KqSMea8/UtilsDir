#include "stdafx.h"
#include "BackCircle.h"


void BackCircle::DrawObject(Graphics &graph)
{
	m_bitmap.reset();

	m_bitmap = boost::make_shared<Bitmap>(m_bitmapSize, m_bitmapSize, &graph);
	Graphics gr(m_bitmap.get());

	gr.SetSmoothingMode(SmoothingModeAntiAlias);

	if(m_style == VS_SURROUND)
	{
		GraphicsPath gp;
		gp.AddEllipse(0, 0, m_diametr, m_diametr);
		PathGradientBrush pgb(&gp);
		int countCol = 1;

		pgb.SetCenterColor(m_shadowColor);
		pgb.SetSurroundColors(&m_backColor, &countCol);
		pgb.SetCenterPoint(Point(0, (int)(m_diametr * 0.1f)));
		pgb.SetFocusScales(0.1f, 0.1f);

		gr.FillPath(&pgb, &gp);
		gr.DrawEllipse(&Pen(m_backColor), 0, 0, m_diametr, m_diametr);

		gp.Reset();

		gp.AddEllipse(m_shadowStart + 5, m_shadowStart + 5, m_shadowDiam - 10, m_shadowDiam - 10);
		pgb.SetGraphicsPath(&gp);

		pgb.SetCenterColor(m_shadowColor);
		//Color col = Color(255, 160, 160, 160);
		Color col = Color(255, max(m_backColor.GetR() - 25, 0), max(m_backColor.GetG() - 25, 0), max(m_backColor.GetB() - 25, 0));
		pgb.SetSurroundColors(&col, &countCol);
		pgb.SetCenterPoint(Point((int)(m_diametr / 1.3f), m_diametr));
		pgb.SetFocusScales(0.1f, 0.1f);

		gr.FillPath(&pgb, &gp);
	}
	else
	{
		gr.FillEllipse(&SolidBrush(m_backColor), 0, 0, m_diametr, m_diametr);
		gr.FillEllipse(&SolidBrush(m_shadowColor), m_shadowStart, m_shadowStart, m_shadowDiam, m_shadowDiam);
	}
 	

 	if(m_drawBorder)
 		gr.DrawEllipse(&Pen(&SolidBrush(m_borderColor), 2), 0, 0, m_diametr, m_diametr);
 
 	

}