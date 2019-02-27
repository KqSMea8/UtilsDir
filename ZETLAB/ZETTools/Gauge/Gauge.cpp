#include "stdafx.h"
#include "Gauge.h"
#include <mathZet.h>




Gauge::Gauge()
{
	m_pBackCircle = boost::make_shared<BackCircle>();
	m_pDial = boost::make_shared<Dial>();
	m_pNeedle = boost::make_shared<Needle>();
	m_pBackColor.reset();
	
	Init();
}


Gauge::Gauge(int X, int Y, int W, int H)
{
	m_rcBounds = Rect(X, Y, W, H);

	int bigDiam = min(W, H) - 2;

	m_pBackCircle = boost::make_shared<BackCircle>(0,0, bigDiam);

	int dialDiam = m_pBackCircle->GetShadowDiam() - 20;
	int dialStart = (m_pBackCircle->GetShadowStart() + (m_pBackCircle->GetShadowDiam() - dialDiam) / 2);

	m_pDial = boost::make_shared<Dial>(dialStart, dialStart, dialDiam);


	int arrowCircleDiam = m_pDial->GetDiam() / 5;

	m_pNeedle = boost::make_shared<Needle>(m_pDial->GetX(), m_pDial->GetY(), m_pDial->GetDiam());
	m_pNeedle->SetNeedleCircleDiam(arrowCircleDiam);

	m_gaugeStyle = VS_SURROUND;
	m_currentValue = 0;

	Init();
}


Gauge::~Gauge()
{
	m_pBackCircle.reset();
	m_pDial.reset();
	m_pNeedle.reset();
	m_pGlassGlare.reset();
	m_pBackColor.reset();
}


void Gauge::DrawObject(Graphics& gr)
{

	//gr.SetSmoothingMode(SmoothingModeAntiAlias);
	
	if(m_pBackColor)
	{
		if(m_pBackColor->GetA() != 0)
		{
			gr.FillRectangle(&SolidBrush(*m_pBackColor.get()), 0, 0, m_rcBounds.Width, m_rcBounds.Height);
			gr.DrawRectangle(&Pen(&SolidBrush(*m_pBackColor.get()), 1), 0, 0, m_rcBounds.Width, m_rcBounds.Height);
		}
	}
	else
	{
		gr.FillRectangle(&SolidBrush(m_3dFace), 0, 0, m_rcBounds.Width, m_rcBounds.Height);
		gr.DrawRectangle(&Pen(&SolidBrush(m_3dFace), 1), 0, 0, m_rcBounds.Width, m_rcBounds.Height);
	}
	

	int bigDiam = min(m_rcBounds.Width, m_rcBounds.Height) - 2;


	if(m_pBackCircle->GetBitmap() == NULL)
		m_pBackCircle->DrawObject(gr);

	gr.DrawImage(m_pBackCircle->GetBitmap(), m_pBackCircle->GetX(), m_pBackCircle->GetY(), m_pBackCircle->GetBitmapSize(), m_pBackCircle->GetBitmapSize());


	if(m_pDial->GetBitmap() == NULL)
		m_pDial->DrawObject(gr);

	gr.DrawImage(m_pDial->GetBitmap(), m_pDial->GetX(), m_pDial->GetY(), m_pDial->GetBitmapSize(), m_pDial->GetBitmapSize());




	if(m_pNeedle->GetCurrentAngle() != m_pDial->GetAngleFromCurrentValue(m_currentValue) || m_pNeedle->GetNeedleBmp() == NULL || m_pNeedle->GetCircleBmp() == NULL)
		
	{
		m_pNeedle->SetCurrentAngle(m_pDial->GetAngleFromCurrentValue(m_currentValue));

		m_pNeedle->DrawObject(gr);
	}



	RotateGr(gr, bigDiam, m_pNeedle->GetCurrentAngle(), RT_PLUS);
	if(m_pNeedle->GetNeedleStyle() != NS_2 /*&& m_needle.GetNeedleStyle() != NS_5*/)
	{
		gr.DrawImage(m_pNeedle->GetNeedleBmp(), m_pNeedle->GetX(), m_pNeedle->GetY(), m_pNeedle->GetBitmapSize(), m_pNeedle->GetBitmapSize());
		gr.DrawImage(m_pNeedle->GetCircleBmp(), m_pNeedle->GetX(), m_pNeedle->GetY(), m_pNeedle->GetBitmapSize(), m_pNeedle->GetBitmapSize());
	}
	else
	{
		gr.DrawImage(m_pNeedle->GetCircleBmp(), m_pNeedle->GetX(), m_pNeedle->GetY(), m_pNeedle->GetBitmapSize(), m_pNeedle->GetBitmapSize());
		gr.DrawImage(m_pNeedle->GetNeedleBmp(), m_pNeedle->GetX(), m_pNeedle->GetY(), m_pNeedle->GetBitmapSize(), m_pNeedle->GetBitmapSize());
	}
	RotateGr(gr, bigDiam, m_pNeedle->GetCurrentAngle(), RT_MINUS);





	if(!m_pGlassGlare)
		DrawGlassGlare(gr);

	if(m_pDial->GetStyle() == VS_SURROUND)
	{
		RotateGr(gr, bigDiam, -20, RT_PLUS);
		gr.DrawImage(m_pGlassGlare.get(), 0, 0, bigDiam, bigDiam);
		RotateGr(gr, bigDiam, -20, RT_MINUS);
	}
	else
		gr.DrawImage(m_pGlassGlare.get(), 0, 0, bigDiam, bigDiam);
}


void Gauge::DrawGlassGlare(Graphics& graph)
{
	int bigDiam = m_pBackCircle->GetDiam(); //min(rcBounds.Width(), rcBounds.Height()) - 2;

	int dialDiam = m_pDial->GetDiam();
	int dialStart = m_pDial->GetX();

	int arrowCircleDiam = m_pNeedle->GetNeedleCircleDiam();

	m_pGlassGlare.reset();

	m_pGlassGlare= boost::make_shared<Bitmap>(bigDiam, bigDiam, &graph);
	Graphics gr(m_pGlassGlare.get());

	gr.SetSmoothingMode(SmoothingModeAntiAlias);


	Point glassGlareUp[] = 
	{
		Point(dialStart, bigDiam / 2 - arrowCircleDiam / 3),
		Point((int)(bigDiam / 2 + (dialDiam / 2)*cos(3.925f)), (int)(bigDiam / 2 + (dialDiam / 2)*sin(3.925f))),
		Point((int)(bigDiam / 2), (int)(bigDiam / 2 - dialDiam / 2)), 
		Point((int)(bigDiam / 2 + (dialDiam / 2)*cos(5.495f)), (int)(bigDiam / 2 + (dialDiam / 2)*sin(5.495f))),
		Point((int)(dialStart + dialDiam), (int)(bigDiam / 2 - arrowCircleDiam / 4)),		
	};

	Point glassGlareDown[] = 
	{
		Point(dialStart, (int)(bigDiam / 2 - arrowCircleDiam / 3)), 
		Point((int)(bigDiam / 2), (int)(bigDiam / 2 - arrowCircleDiam / 1.2f)),
		Point(dialStart + dialDiam, (int)(bigDiam / 2 - arrowCircleDiam / 4)) 
	};


	//gr.FillClosedCurve(&LinearGradientBrush(Rect(0, 0, bigDiam, bigDiam ), Color(100, 255, 255, 255), Color(00, 255, 255, 255), LinearGradientModeVertical), glassGlare3, 4, FillModeAlternate, 0.0);

	GraphicsPath* gp = new GraphicsPath();
	gp->AddPie(Rect(dialStart, dialStart, dialDiam, dialDiam ), 180, 180 );
	Region rg(gp);

	gp->Reset();
	gp->AddCurve(glassGlareUp, 5, (REAL)1.2);
	gp->AddCurve(glassGlareDown, 3, (REAL)0.65);
	gp->CloseFigure();

	gr.SetClip(&rg);
	gr.FillPath(&LinearGradientBrush(Rect(dialStart, dialStart, dialDiam, dialDiam ), Color(0, 255, 255, 255), Color(100, 255, 255, 255), LinearGradientModeVertical), gp);

	DeleteObject(gp);
	delete gp;
	gp = nullptr;
}


void Gauge::Init()
{
	m_gaugeStyle = VS_SURROUND;
	m_currentValue = 0;

	DWORD _3dFace = GetSysColor(COLOR_3DFACE);
	m_3dFace = Color(255, _3dFace & 255, (_3dFace >> 8) & 255, (_3dFace >> 16) & 255);
}


void Gauge::RotateGr(Graphics &gr, int &diametr, float angle, rotatetype rt)
{
	if(rt == RT_MINUS) angle *= -1.0;
	gr.TranslateTransform((REAL)(diametr / 2), (REAL)(diametr / 2));
	gr.RotateTransform((REAL)angle);
	gr.TranslateTransform((REAL)(-diametr / 2), (REAL)(-diametr / 2));
}