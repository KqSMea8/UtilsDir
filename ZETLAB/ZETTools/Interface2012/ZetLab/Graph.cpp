//------------------------------------------------------------------------------
// Graph.cpp : файл реализации
//------------------------------------------------------------------------------
#include "stdafx.h"
#include <Interface2012\ZetLab\Graph.h>

bool m_flag = false;

//------------------------------------------------------------------------------
CGraph::CGraph(CCustomContainer *owner, CRect rect, PlotType plotType, double leftBorder,
double rightBorder, double topBorder, double bottomBorder, int qntHorCursors,
int qntVertCursors)
: CCustomContainer(owner), m_pOscillGridX(nullptr), m_pOscillGridY(nullptr),
m_pSimpleGridX(nullptr), m_pSimpleGridY(nullptr), m_pOscillLogGridX(nullptr),
m_pOscillLogGridY(nullptr), m_pTrendGridX(nullptr), m_pTrendGridY(nullptr),
m_pCustomGridX(nullptr), m_pCustomGridY(nullptr), m_pOscillPlot(nullptr),
m_pSpectrBar(nullptr), m_pCustomPlot(nullptr), m_pCustomPlotVector(NULL), m_pHorCursor(nullptr),
m_pVertCursor(nullptr), m_activePlot(0)
{
    if (m_pOwner)
    {
		m_plotType = plotType;
        m_maxX = 100;
        m_minX = 0;
        m_maxY = 100;
        m_minY = 0;
        if(leftBorder>0)
            m_leftBorder = leftBorder;
        else
            m_leftBorder = 40;
        if(rightBorder>0)
            m_rightBorder = rightBorder;
        else
            m_rightBorder = 20;
        if(topBorder>0)
            m_topBorder = topBorder;
        else
            m_topBorder = 30;
        if(bottomBorder>0)
            m_bottomBorder = bottomBorder;
        else
            m_bottomBorder = 40;
        m_scaleTypeX = stLinear;
        m_scaleTypeY = stLinear;
		m_createButtons = true;

        m_borderRect = CRect(rect.left+10+m_leftBorder, rect.top+10+m_topBorder, rect.right-10-m_rightBorder, rect.bottom-10-m_bottomBorder);
        
        m_qntOfHorCursors = qntHorCursors;
        m_qntOfVertCursors = qntVertCursors;

        m_HorCursorRect = CRect(0,0,0,0);
        m_VertCursorRect = CRect(0,0,0,0);

        if(qntHorCursors > 0)
            m_HorCursorRect = CRect(m_borderRect.left, m_borderRect.top, m_borderRect.right, m_borderRect.top+10);

        if(qntVertCursors > 0)
            m_VertCursorRect = CRect(m_borderRect.right-10, m_borderRect.top + m_HorCursorRect.Height(), m_borderRect.right, m_borderRect.bottom);

		if (qntHorCursors > 0 && qntVertCursors > 0){
			m_VertCursorRect = CRect(m_borderRect.right - 10, m_borderRect.top + m_HorCursorRect.Height(), m_borderRect.right, m_borderRect.bottom);
			m_HorCursorRect = CRect(m_borderRect.left, m_borderRect.top, m_borderRect.right - m_VertCursorRect.Width(), m_borderRect.top + 10);
			m_decorativeRect = CRect(m_borderRect.right - 10, m_borderRect.top, m_borderRect.right, m_borderRect.top + 10);
		}

        m_gridRect = CRect(m_borderRect.left, m_borderRect.top+m_HorCursorRect.Height(), m_borderRect.right - m_VertCursorRect.Width(), m_borderRect.bottom);

       //порядок создания сетки важен, сначала необходимо создать для оси X, затем Y
        if(plotType == ptOscill)
        {
			m_pOscillGridX = new COscillGrid(this, m_gridRect, gtHorizontal);
            m_pCustomGridX = m_pOscillGridX;

            m_pOscillGridY = new COscillGrid(this, m_gridRect, gtVertical);
            m_pCustomGridY = m_pOscillGridY;

			m_pOscillPlot = new COscillPlot(this, stLinear, stLinear, m_gridRect);
			m_pCustomPlot = m_pOscillPlot;

			m_pCustomPlotVector.push_back(new COscillPlot(this, stLinear, stLinear, m_gridRect));
        }
		if(plotType == ptOscillLog)
		{
			m_pOscillPlot = new COscillPlot(this, stLog, stLinear, m_gridRect);
			m_pCustomPlot = m_pOscillPlot;

			m_pCustomPlotVector.push_back(new COscillPlot(this, stLog, stLinear, m_gridRect));

			m_pOscillLogGridX = new COscillLogGrid(this, m_gridRect, gtHorizontal);
			m_pCustomGridX = m_pOscillLogGridX;

			m_pOscillGridY = new COscillGrid(this, m_gridRect, gtVertical);
			m_pCustomGridY = m_pOscillGridY;
		}
        if(plotType == ptSpectr){

			m_pOscillGridX = new COscillGrid(this, m_gridRect, gtHorizontal);
			m_pCustomGridX = m_pOscillGridX;

			m_pOscillGridY = new COscillGrid(this, m_gridRect, gtVertical);
			m_pCustomGridY = m_pOscillGridY;

			m_pSpectrBar = new CSpectrBar(this, stLinear, stLinear, m_gridRect);
			m_pCustomPlot = m_pSpectrBar;

			m_pCustomPlotVector.push_back(new CSpectrBar(this, stLinear, stLinear, m_gridRect));
        }
		if(plotType == ptSpectrLog){
			m_pSpectrBar = new CSpectrBar(this, stLog, stLinear, m_gridRect);
			m_pCustomPlot = m_pSpectrBar;

			m_pCustomPlotVector.push_back(new CSpectrBar(this, stLog, stLinear, m_gridRect));

			m_pOscillLogGridX = new COscillLogGrid(this, m_gridRect, gtHorizontal);
			m_pCustomGridX = m_pOscillLogGridX;

			m_pOscillGridY = new COscillGrid(this, m_gridRect, gtVertical);
			m_pCustomGridY = m_pOscillGridY;
		}
        if(plotType == ptTrend)
        {
            m_pOscillPlot = new COscillPlot(this, stLinear, stLinear, m_gridRect);
            m_pCustomPlot = m_pOscillPlot;

			m_pCustomPlotVector.push_back(new COscillPlot(this, stLinear, stLinear, m_gridRect));

            m_pTrendGridX = new CTrendGrid(this, m_gridRect, gtHorizontal);
            m_pCustomGridX = m_pTrendGridX;
            
            m_pOscillGridY = new COscillGrid(this, m_gridRect, gtVertical);
            m_pCustomGridY = m_pOscillGridY;
        }

		if (plotType == ptParam)
		{
			m_pOscillPlot = new COscillPlot(this, stLinear, stLinear, m_gridRect);
			m_pCustomPlot = m_pOscillPlot;

			m_pCustomPlotVector.push_back(new COscillPlot(this, stLinear, stLinear, m_gridRect));

			m_pSimpleGridX = new CSimpleGrid(this, m_gridRect, gtHorizontal);
			m_pCustomGridX = m_pSimpleGridX;

			m_pSimpleGridY = new CSimpleGrid(this, m_gridRect, gtVertical);
			m_pCustomGridY = m_pSimpleGridY;

			m_invertation = true;
		}

        m_pDGraph = new CDGraph(rect, m_borderRect, m_pOwner->GetPointerToIsChange(), m_pOwner->GetPointerToTime());
        m_pDClass = m_pDGraph;

		if (m_qntOfHorCursors > 0){
			m_pHorCursor = new CCursorField(this, m_HorCursorRect, m_gridRect.Height(), m_qntOfHorCursors, m_invertation);
			m_cursorInfo.resize(m_qntOfHorCursors);
            for(int cursorId = 0; cursorId < m_qntOfHorCursors; cursorId++)
                m_pDGraph->SetRectBubble(CRect(0, 0, 0, 0), cursorId);
        }

		if (m_qntOfVertCursors > 0)
        {
			m_pVertCursor = new CCursorField(this, m_VertCursorRect, m_gridRect.Width(), m_qntOfVertCursors, m_invertation);
			m_cursorInfo.resize(m_qntOfVertCursors + m_qntOfHorCursors);
			for (int cursorId = m_qntOfHorCursors; cursorId < m_qntOfVertCursors + m_qntOfHorCursors; cursorId++)
                m_pDGraph->SetRectBubble(CRect(0, 0, 0, 0), cursorId);
        }

        m_rect = CRect(rect.left+10+m_leftBorder, rect.top+10+m_topBorder, rect.right-10-m_rightBorder, rect.bottom-10-m_bottomBorder);

		SetScaleShiftRect();
    }
}
//------------------------------------------------------------------------------
CGraph::CGraph(CCustomContainer *owner, GraphParam param) : CCustomContainer(owner)
{
	if (m_pOwner)
	{
		m_plotType = param.plotType;
		m_maxX = 100;
		m_minX = 0;
		m_maxY = 100;
		m_minY = 0;
		if (param.leftBorder>0)
			m_leftBorder = param.leftBorder;
		else
			m_leftBorder = 40;
		if (param.rightBorder>0)
			m_rightBorder = param.rightBorder;
		else
			m_rightBorder = 20;
		if (param.topBorder>0)
			m_topBorder = param.topBorder;
		else
			m_topBorder = 30;
		if (param.bottomBorder>0)
			m_bottomBorder = param.bottomBorder;
		else
			m_bottomBorder = 40;
		m_scaleTypeX = stLinear;
		m_scaleTypeY = stLinear;

		m_borderRect = CRect(param.rect.left + 10 + m_leftBorder, param.rect.top + 10 + m_topBorder, param.rect.right - 10 - m_rightBorder, param.rect.bottom - 10 - m_bottomBorder);

		m_qntOfHorCursors = param.qntHorCursors;
		m_qntOfVertCursors = param.qntVertCursors;

		m_HorCursorRect = CRect(0, 0, 0, 0);
		m_VertCursorRect = CRect(0, 0, 0, 0);

		if (param.qntHorCursors > 0)
			m_HorCursorRect = CRect(m_borderRect.left, m_borderRect.top, m_borderRect.right, m_borderRect.top + 10);

		if (param.qntVertCursors > 0)
			m_VertCursorRect = CRect(m_borderRect.right - 10, m_borderRect.top + m_HorCursorRect.Height(), m_borderRect.right, m_borderRect.bottom);

		if (param.qntHorCursors > 0)
			m_HorCursorRect = CRect(m_borderRect.left, m_borderRect.top, m_borderRect.right - m_VertCursorRect.Width(), m_borderRect.top + 10);

		m_gridRect = CRect(m_borderRect.left, m_borderRect.top + m_HorCursorRect.Height(), m_borderRect.right - m_VertCursorRect.Width(), m_borderRect.bottom);

		//порядок создания сетки важен, сначала необходимо создать для оси X, затем Y
		if (param.plotType == ptOscill)
		{
			m_pOscillPlot = new COscillPlot(this, stLinear, stLinear, m_gridRect);
			m_pCustomPlot = m_pOscillPlot;

			m_pOscillGridX = new COscillGrid(this, m_gridRect, gtHorizontal);
			m_pCustomGridX = m_pOscillGridX;

			m_pOscillGridY = new COscillGrid(this, m_gridRect, gtVertical);
			m_pCustomGridY = m_pOscillGridY;
		}
		if (param.plotType == ptOscillLog)
		{
			m_pOscillPlot = new COscillPlot(this, stLog, stLinear, m_gridRect);
			m_pCustomPlot = m_pOscillPlot;

			m_pOscillLogGridX = new COscillLogGrid(this, m_gridRect, gtHorizontal);
			m_pCustomGridX = m_pOscillLogGridX;

			m_pOscillGridY = new COscillGrid(this, m_gridRect, gtVertical);
			m_pCustomGridY = m_pOscillGridY;
		}
		if (param.plotType == ptSpectr){
			m_pSpectrBar = new CSpectrBar(this, stLinear, stLinear, m_gridRect);
			m_pCustomPlot = m_pSpectrBar;

			m_pOscillGridX = new COscillGrid(this, m_gridRect, gtHorizontal);
			m_pCustomGridX = m_pOscillGridX;

			m_pOscillGridY = new COscillGrid(this, m_gridRect, gtVertical);
			m_pCustomGridY = m_pOscillGridY;
		}
		if (param.plotType == ptSpectrLog){
			m_pSpectrBar = new CSpectrBar(this, stLog, stLinear, m_gridRect);
			m_pCustomPlot = m_pSpectrBar;

			m_pOscillLogGridX = new COscillLogGrid(this, m_gridRect, gtHorizontal);
			m_pCustomGridX = m_pOscillLogGridX;

			m_pOscillGridY = new COscillGrid(this, m_gridRect, gtVertical);
			m_pCustomGridY = m_pOscillGridY;
		}
		if (param.plotType == ptTrend)
		{
			m_pOscillPlot = new COscillPlot(this, stLinear, stLinear, m_gridRect);
			m_pCustomPlot = m_pOscillPlot;

			m_pTrendGridX = new CTrendGrid(this, m_gridRect, gtHorizontal);
			m_pCustomGridX = m_pTrendGridX;

			m_pOscillGridY = new COscillGrid(this, m_gridRect, gtVertical);
			m_pCustomGridY = m_pOscillGridY;
		}
		if (param.plotType == ptParam)
		{
			m_pOscillPlot = new COscillPlot(this, stLinear, stLinear, m_gridRect);
			m_pCustomPlot = m_pOscillPlot;

			m_pOscillGridX = new COscillGrid(this, m_gridRect, gtVertical);
			m_pCustomGridX = m_pOscillGridX;

			m_pTrendGridY = new CTrendGrid(this, m_gridRect, gtHorizontal);
			m_pCustomGridY = m_pTrendGridY;
		}

		m_pDGraph = new CDGraph(param.rect, m_borderRect, m_pOwner->GetPointerToIsChange(), m_pOwner->GetPointerToTime());
		m_pDClass = m_pDGraph;

		if (param.qntHorCursors > 0){
			m_pHorCursor = new CCursorField(this, m_HorCursorRect, m_gridRect.Height(), param.qntHorCursors, m_invertation);
			m_cursorInfo.reserve(param.qntHorCursors);
			for (int cursorId = 0; cursorId < m_qntOfHorCursors; cursorId++)
				m_pDGraph->SetRectBubble(CRect(0, 0, 0, 0), cursorId);
		}

		if (param.qntVertCursors > 0)
		{
			m_pVertCursor = new CCursorField(this, m_VertCursorRect, m_gridRect.Width(), param.qntVertCursors, m_invertation);
			m_cursorInfo.reserve(param.qntVertCursors);
			for (int cursorId = 0; cursorId < m_qntOfVertCursors; cursorId++)
				m_pDGraph->SetRectBubble(CRect(0, 0, 0, 0), cursorId);
		}

		m_rect = CRect(param.rect.left + 10 + m_leftBorder, param.rect.top + 10 + m_topBorder, param.rect.right - 10 - m_rightBorder, param.rect.bottom - 10 - m_bottomBorder);
		m_createButtons = param.createButtons;
		SetScaleShiftRect();

	}
}
//------------------------------------------------------------------------------
CGraph::~CGraph()
{
	if (m_pDGraph)	
	{ 
		delete m_pDGraph;
		m_pDGraph = nullptr;
	}
	if (m_pHorCursor)
	{ 
		delete m_pHorCursor;
		m_pHorCursor = nullptr;
	}
	if (m_pSpectrBar)
	{ 
		delete m_pSpectrBar;
		m_pSpectrBar = nullptr;
	}
	if (m_pOscillPlot)
	{ 
		delete m_pOscillPlot;
		m_pOscillPlot = nullptr;
	}
	if (m_pTrendGridX)	
	{
		delete m_pTrendGridX;
		m_pTrendGridX = nullptr;
	}
	if (m_pTrendGridY)
	{ 
		delete m_pTrendGridY;
		m_pTrendGridY = nullptr;
	}
	if (m_pOscillGridX)
	{ 
		delete m_pOscillGridX;
		m_pOscillGridX = nullptr;
	}
	if (m_pOscillGridY)	
	{ 
		delete m_pOscillGridY;
		m_pOscillGridY = nullptr;
	}
	if (m_pOscillLogGridX)
	{ 
		delete m_pOscillLogGridX;
		m_pOscillLogGridX = nullptr;
	}
	if (m_pOscillLogGridY)
	{ 
		delete m_pOscillLogGridY;
		m_pOscillLogGridY = nullptr;
	}
};
//------------------------------------------------------------------------------
void CGraph::Resize(CRect rect)
{
	int Counter(0);
	m_borderRect = CRect(rect.left + 10 + m_leftBorder, rect.top + 10 + m_topBorder, rect.right - 10 - m_rightBorder, rect.bottom - 10 - m_bottomBorder);

	m_HorCursorRect = CRect(0, 0, 0, 0);
	m_VertCursorRect = CRect(0, 0, 0, 0);

	if (m_qntOfHorCursors > 0)
		m_HorCursorRect = CRect(m_borderRect.left, m_borderRect.top, m_borderRect.right, m_borderRect.top + 10);

	if (m_qntOfVertCursors > 0)
		m_VertCursorRect = CRect(m_borderRect.right - 10, m_borderRect.top + m_HorCursorRect.Height(), m_borderRect.right, m_borderRect.bottom);

	if (m_qntOfHorCursors > 0 && m_qntOfVertCursors > 0){
		m_VertCursorRect = CRect(m_borderRect.right - 10, m_borderRect.top + m_HorCursorRect.Height(), m_borderRect.right, m_borderRect.bottom);
		m_HorCursorRect = CRect(m_borderRect.left, m_borderRect.top, m_borderRect.right - m_VertCursorRect.Width(), m_borderRect.top + 10);
		m_decorativeRect = CRect(m_borderRect.right - 10, m_borderRect.top, m_borderRect.right, m_borderRect.top + 10);
	}

	m_gridRect = CRect(m_borderRect.left, m_borderRect.top + m_HorCursorRect.Height(), m_borderRect.right - m_VertCursorRect.Width(), m_borderRect.bottom);
	
	m_pCustomGridX->Resize(m_gridRect);

	m_pCustomGridY->Resize(m_gridRect);

	for (auto it = m_pCustomPlotVector.begin(); it != m_pCustomPlotVector.end(); ++ it)
		(*it)->Resize(m_gridRect);

	if (m_qntOfHorCursors > 0){
		m_pHorCursor->Resize(m_HorCursorRect);
		m_pHorCursor->ResizeLine(m_gridRect.Height());
	}

	if (m_qntOfVertCursors > 0)
	{
		m_pVertCursor->Resize(m_VertCursorRect);
		m_pVertCursor->ResizeLine(m_gridRect.Width());
	}

	m_rect = CRect(rect.left + 10 + m_leftBorder, rect.top + 10 + m_topBorder,
		rect.right - 10 - m_rightBorder, rect.bottom - 10 - m_bottomBorder);

	if (m_createButtons)
	{
		m_leftShiftRect = CRect(m_rect.left, m_rect.bottom + m_bottomBorder / 2.0,
			m_rect.left + m_rect.Width() / 4.0, m_rect.bottom
			+ m_bottomBorder);

		m_rightShiftRect = CRect(m_rect.right - m_rect.Width() / 4.0,
			m_rect.bottom + m_bottomBorder / 2.0, m_rect.right, m_rect.bottom
			+ m_bottomBorder);

		m_bottomShiftRect = CRect(m_rect.left - m_leftBorder,
			m_rect.bottom - m_rect.Height() / 4.0, m_rect.left - m_leftBorder / 2.0,
			m_rect.bottom);

		m_topShiftRect = CRect(m_rect.left - m_leftBorder,
			m_rect.top, m_rect.left - m_leftBorder / 2.0, m_rect.top
			+ m_rect.Height() / 4.0);

		m_leftScaleRect = CRect(m_rect.left + m_rect.Width() / 4.0,
			m_rect.bottom + m_bottomBorder / 2.0, m_rect.left + m_rect.Width() / 2.0,
			m_rect.bottom + m_bottomBorder);

		m_rightScaleRect = CRect(m_rect.right - m_rect.Width() / 2.0,
			m_rect.bottom + m_bottomBorder / 2.0, m_rect.right - m_rect.Width() / 4.0,
			m_rect.bottom + m_bottomBorder);

		m_topScaleRect = CRect(m_rect.left - m_leftBorder,
			m_rect.top + m_rect.Height() / 4.0, m_rect.left - m_leftBorder / 2.0,
			m_rect.top + m_rect.Height() / 2.0);

		m_bottomScaleRect = CRect(m_rect.left - m_leftBorder,
			m_rect.bottom - m_rect.Height() / 2.0, m_rect.left - m_leftBorder / 2.0,
			m_rect.bottom - m_rect.Height() / 4.0);

		m_centerAutoScaleRect = CRect(m_rect.left - m_leftBorder,
			m_rect.bottom, m_rect.left, m_rect.bottom + m_bottomBorder);
	}

	else
	{
		m_leftShiftRect = m_rightShiftRect = m_bottomShiftRect =
			m_topShiftRect = m_leftScaleRect = m_rightScaleRect =
			m_topScaleRect = m_bottomScaleRect = m_centerAutoScaleRect = CRect(0,
			0, 0, 0);
	}

	m_pDGraph->Resize(rect, m_rect, m_leftShiftRect, m_rightShiftRect, m_bottomShiftRect,
		m_topShiftRect, m_leftScaleRect, m_rightScaleRect, m_topScaleRect,
		m_bottomScaleRect, m_centerAutoScaleRect, m_decorativeRect);
}
//------------------------------------------------------------------------------
void CGraph::OnDraw()
{
    m_pDGraph->OnDraw();
    CCustomContainer::OnDraw();
};
//------------------------------------------------------------------------------
void CGraph::OnLButtonDown(UINT nFlags, CPoint point){

	//при клике на область, выделенную под кнопку,
	//включаем подсветку

    zRect rect;
    rect = gNone;

    if(PointInRect(point, &m_leftShiftRect) == true){
        rect = gLeftShift;
        m_pCustomGridX->Shift(rect);
        m_pDGraph->BrighteningForArrows(false, 0);
    }

    if(PointInRect(point, &m_rightShiftRect) == true){
        rect = gRightShift;
        m_pCustomGridX->Shift(rect);
        m_pDGraph->BrighteningForArrows(false, 5);
    }

    if(PointInRect(point, &m_topShiftRect) == true){
        rect = gTopShift;
        m_pCustomGridY->Shift(rect);
        m_pDGraph->BrighteningForArrows(false, 6);
    }

    if(PointInRect(point, &m_bottomShiftRect) == true){
        rect = gBottomShift;
        m_pCustomGridY->Shift(rect);
        m_pDGraph->BrighteningForArrows(false, 11);
    }

    if(PointInRect(point, &m_rightScaleRect) == true){
        rect = gRightScale;
        m_pCustomGridX->Scale(rect);
        m_pDGraph->BrighteningForArrows(false, 3);
    }

    if(PointInRect(point, &m_leftScaleRect) == true){
        rect = gLeftScale;
        m_pCustomGridX->Scale(rect);
        m_pDGraph->BrighteningForArrows(false, 1);
    }

    if(PointInRect(point, &m_topScaleRect) == true){
        rect = gTopScale;
        m_pCustomGridY->Scale(rect);
        m_pDGraph->BrighteningForArrows(false, 7);
    }

    if(PointInRect(point, &m_bottomScaleRect) == true){
        rect = gBottomScale;
        m_pCustomGridY->Scale(rect);
        m_pDGraph->BrighteningForArrows(false, 9);
    }

	for (int Counter = 0; Counter < int(m_pCustomPlotVector.size()); Counter++)
	{
		m_pCustomPlotVector[Counter]->SetAxis(m_pCustomGridX->GetAxisMin(gtHorizontal),
			m_pCustomGridX->GetAxisMax(gtHorizontal),
			m_pCustomGridY->GetAxisMin(gtVertical),
			m_pCustomGridY->GetAxisMax(gtVertical));
	}

    if(PointInRect(point, &m_centerAutoScaleRect) == true)
        rect = gCenterAutoScale;

	//Если существует горизонтальный курсор и осью разбиения является Х, то при клике на линию курсора выводим "баббл-подсказку".
    if(m_pHorCursor && !m_invertation){
        m_flag = false;
        for(int cursorId = 0; cursorId < m_qntOfHorCursors; cursorId++){
            if(PointInRect(point, &m_cursorInfo[cursorId].ciRect) == true){
                rect = gGridArea;
                m_pDGraph->SetRectBubble(CRect(m_gridRect.left + m_cursorInfo[cursorId].ciX + 2,
                    m_gridRect.bottom - 50 - 2,
                    m_gridRect.left + m_cursorInfo[cursorId].ciX + 100 +2,
                    m_gridRect.bottom - 2), cursorId);
                m_pDGraph->SetTextBubble(m_cursorInfo[cursorId].ciTextX, m_cursorInfo[cursorId].ciTextY, cursorId);
                m_flag = true;
            }
        }

        if(!m_flag && PointInRect(point, &m_gridRect) == true)
        {
            for(int cursorId = 0; cursorId < m_qntOfHorCursors; cursorId++){
                m_pDGraph->SetRectBubble(CRect(0, 0, 0, 0), cursorId);
                m_pDGraph->SetTextBubble(m_cursorInfo[cursorId].ciTextX, m_cursorInfo[cursorId].ciTextY, cursorId);
            }
        }
    }

	//Если существует вертикальный курсор и осью разбиения является Y, то при клике на линию курсора выводим "баббл-подсказку".
	if (m_pVertCursor && m_invertation){
		m_flag = false;
		for (int cursorId = m_qntOfHorCursors; cursorId < m_qntOfHorCursors + m_qntOfVertCursors; cursorId++){
			if (PointInRect(point, &m_cursorInfo[cursorId].ciRect) == true){
				rect = gGridArea;
				m_pDGraph->SetRectBubble(CRect(m_gridRect.left + 2,
					m_cursorInfo[cursorId].ciY + 2,
					m_gridRect.left + 100 + 2,
					m_cursorInfo[cursorId].ciY + 50 + 2), cursorId);
				m_pDGraph->SetTextBubble(m_cursorInfo[cursorId].ciTextX, m_cursorInfo[cursorId].ciTextY, cursorId);
				m_flag = true;
			}
		}

		if (!m_flag && PointInRect(point, &m_gridRect) == true)
		{
			for (int cursorId = m_qntOfHorCursors; cursorId < m_qntOfHorCursors + m_qntOfVertCursors; cursorId++){
				m_pDGraph->SetRectBubble(CRect(0, 0, 0, 0), cursorId);
				m_pDGraph->SetTextBubble(m_cursorInfo[cursorId].ciTextX, m_cursorInfo[cursorId].ciTextY, cursorId);
			}
		}
	}

    if(PointInRect(point, &m_centerAutoScaleRect) == true)
    {
        AutoScaleY();
		AutoScaleX();
        m_pDGraph->BrighteningForArrows(false, 12);
    }

	if (m_pHorCursor && !m_invertation){
		m_pHorCursor->SetPointOfStartVal(m_pCustomPlotVector[m_activePlot]->GetPointOfStartVal());
        m_pHorCursor->SetMinMax(m_pCustomGridX->GetAxisMin(gtHorizontal), m_pCustomGridX->GetAxisMax(gtHorizontal));
    }

	if (m_pVertCursor && m_invertation){
		m_pVertCursor->SetPointOfStartVal(m_pCustomPlotVector[m_activePlot]->GetPointOfStartVal(m_invertation));
		m_pVertCursor->SetMinMax(m_pCustomGridY->GetAxisMin(gtHorizontal), m_pCustomGridY->GetAxisMax(gtHorizontal));
	}
};
//------------------------------------------------------------------------------
void CGraph::OnLButtonUp(UINT nFlags, CPoint point)
{
	//при клике на область, выделенную под кнопку,
	//убираем подсветку
    if(PointInRect(point, &m_leftShiftRect) == true){
        m_pDGraph->BrighteningForArrows(true, 0);
    }
    
    if(PointInRect(point, &m_rightShiftRect) == true){
        m_pDGraph->BrighteningForArrows(true, 5);
    }

    if(PointInRect(point, &m_topShiftRect) == true){
        m_pDGraph->BrighteningForArrows(true, 6);
    }

    if(PointInRect(point, &m_bottomShiftRect) == true){
        m_pDGraph->BrighteningForArrows(true, 11);
    }

    if(PointInRect(point, &m_leftScaleRect) == true){
        m_pDGraph->BrighteningForArrows(true, 1);
    }

    if(PointInRect(point, &m_rightScaleRect) == true){
        m_pDGraph->BrighteningForArrows(true, 3);
    }

    if(PointInRect(point, &m_bottomScaleRect) == true){
        m_pDGraph->BrighteningForArrows(true, 9);
    }

    if(PointInRect(point, &m_topScaleRect) == true){
        m_pDGraph->BrighteningForArrows(true, 7);
    }

    if(PointInRect(point, &m_centerAutoScaleRect) == true){
        m_pDGraph->BrighteningForArrows(true, 12);
    }
}
//------------------------------------------------------------------------------
void CGraph::OnRButtonDown(UINT nFlags, CPoint point)
{
	//выбираем график по клику правой кнопкой
	int Counter, Size = m_pCustomPlotVector.size();
	if (PointInRect(point, &m_gridRect) == true){
		for (Counter = (m_activePlot + 1) % Size; Counter != m_activePlot; Counter = (Counter + 1) % Size)
		{
			if (m_pCustomPlotVector[Counter]->PlotInRect(CRect(point.x - 20, point.y - 20, point.x + 20, point.y + 20)))
			{
				m_pCustomPlotVector[m_activePlot]->SetPlotWidth(2);
				m_pCustomPlotVector[m_activePlot]->SetPlotTransparency(127);
				m_pCustomPlotVector[Counter]->SetPlotWidth(4);
				m_pCustomPlotVector[Counter]->SetPlotTransparency(255);
				m_activePlot = Counter % Size;
				return;
			}
		}
	}
}
//------------------------------------------------------------------------------
void CGraph::OnRButtonUp(UINT nFlags, CPoint point)
{

}
//------------------------------------------------------------------------------
void CGraph::SetData(std::vector<double> &pDataX, std::vector<double> &pDataY, int plotNumber, int quality)
{

	if (plotNumber > int(m_pCustomPlotVector.size() - 1)){
		return;
	}

	m_pCustomPlotVector[plotNumber]->SetData(pDataX, pDataY, quality);

	if (m_pHorCursor){
		m_pHorCursor->SetPointOfStartVal(m_pCustomPlotVector[m_activePlot]->GetPointOfStartVal());
		for (int cursorId = 0; cursorId < m_qntOfHorCursors; cursorId++){
			GetCursorInfo(cursorId);
			m_pDGraph->SetTextBubble(m_cursorInfo[cursorId].ciTextX, m_cursorInfo[cursorId].ciTextY, cursorId);
		}
	}
}
//------------------------------------------------------------------------------
void CGraph::SetData(std::vector<double> &pDataY, int plotNumber, int quality)
{

	if(plotNumber > int(m_pCustomPlotVector.size() - 1))
		return;
	
	m_pCustomPlotVector[plotNumber]->SetData(pDataY, quality, m_invertation);

    if(m_pHorCursor){
		m_pHorCursor->SetPointOfStartVal(m_pCustomPlotVector[m_activePlot]->GetPointOfStartVal());
        for(int cursorId = 0; cursorId < m_qntOfHorCursors; cursorId++){
            GetCursorInfo(cursorId);
            m_pDGraph->SetTextBubble(m_cursorInfo[cursorId].ciTextX, m_cursorInfo[cursorId].ciTextY, cursorId);
        }
    }

	if (m_pVertCursor){
		m_pVertCursor->SetPointOfStartVal(m_pCustomPlotVector[m_activePlot]->GetPointOfStartVal(m_invertation));
		for (int cursorId = m_qntOfHorCursors; cursorId < m_qntOfVertCursors + m_qntOfHorCursors; cursorId++){
			GetCursorInfo(cursorId);
			m_pDGraph->SetTextBubble(m_cursorInfo[cursorId].ciTextX, m_cursorInfo[cursorId].ciTextY, cursorId);
		}
	}
}
//------------------------------------------------------------------------------
void CGraph::SetDataFloat(std::vector<float> &pDataY, int plotNumber, int quality)
{
	if (plotNumber > int(m_pCustomPlotVector.size() - 1)){
		return;
	}
	m_pCustomPlotVector[plotNumber]->SetDataFloat(pDataY, quality, m_invertation);

    if(m_pHorCursor){
		m_pHorCursor->SetPointOfStartVal(m_pCustomPlotVector[m_activePlot]->GetPointOfStartVal());
        for(int cursorId = 0; cursorId < m_qntOfHorCursors; cursorId++){
            GetCursorInfo(cursorId);
            m_pDGraph->SetTextBubble(m_cursorInfo[cursorId].ciTextX, m_cursorInfo[cursorId].ciTextY, cursorId);
        }
    }

	if (m_pVertCursor){
		m_pVertCursor->SetPointOfStartVal(m_pCustomPlotVector[m_activePlot]->GetPointOfStartVal(m_invertation));
		for (int cursorId = m_qntOfHorCursors; cursorId < m_qntOfVertCursors + m_qntOfHorCursors; cursorId++){
			GetCursorInfo(cursorId);
			m_pDGraph->SetTextBubble(m_cursorInfo[cursorId].ciTextX, m_cursorInfo[cursorId].ciTextY, cursorId);
		}
	}
}
//------------------------------------------------------------------------------
long CGraph::SetSpace(double pointOfStart, double pointOfEnd, double dx, double dy)
{
	long result(0);

	if(m_pCustomPlotVector.empty())
		return -1;

	for (auto it = m_pCustomPlotVector.begin(); it != m_pCustomPlotVector.end(); ++ it)
		result =  (*it)->SetSpace(pointOfStart, pointOfEnd, dx, m_invertation);
	
	m_pCustomGridX->SetMinStep(m_pCustomPlotVector[m_activePlot]->GetDataStep());
	m_pCustomGridY->SetMinStep(dy);

    if(m_pHorCursor)
        m_pHorCursor->SetStep(dx);

	if (m_pVertCursor)
		m_pVertCursor->SetStep(dx);

    return result;
};
//------------------------------------------------------------------------------
double CGraph::SetSpace(double pointOfStart, double pointOfEnd, long Qnt, double dy)
{
	double result(0.0);

	if (m_pCustomPlotVector.empty())
		return -1;

	for (auto it = m_pCustomPlotVector.begin(); it != m_pCustomPlotVector.end(); ++ it)
		result = (*it)->SetSpace(pointOfStart, pointOfEnd, Qnt, m_invertation);

	m_pCustomGridX->SetMinStep(m_pCustomPlotVector[m_activePlot]->GetDataStep());
	m_pCustomGridY->SetMinStep(dy);

    if(m_pHorCursor)
        m_pHorCursor->SetStep(result);

	if (m_pVertCursor)
		m_pVertCursor->SetStep(result);

	return result;
};
//------------------------------------------------------------------------------
void CGraph::SetBoundsX(double xBegin, double xEnd)
{
    m_pCustomGridX->SetBounds(xBegin, xEnd);
};
//------------------------------------------------------------------------------
void CGraph::SetBoundsY(double yBegin, double yEnd)
{
    m_pCustomGridY->SetBounds(yBegin, yEnd);
};
//------------------------------------------------------------------------------
void CGraph::SetMinStepX(double xMinStep)
{
    m_pCustomGridX->SetMinStep(xMinStep);
}
//------------------------------------------------------------------------------
void CGraph::SetMinStepY(double yMinStep)
{
    m_pCustomGridY->SetMinStep(yMinStep);
};
//------------------------------------------------------------------------------
void CGraph::SetAxisX(double minX, double maxX)
{
	if (minX == maxX)
		return;

    m_pCustomGridX->SetAxis(minX, maxX);

	if (m_pCustomPlotVector.empty()){
		return;
	}

	for (int Counter = 0; Counter < int(m_pCustomPlotVector.size()); Counter++)
	{
		m_pCustomPlotVector[Counter]->SetAxis(m_pCustomGridX->GetAxisMin(gtHorizontal),
			m_pCustomGridX->GetAxisMax(gtHorizontal),
			m_pCustomGridY->GetAxisMin(gtVertical),
			m_pCustomGridY->GetAxisMax(gtVertical));
	}

    if(m_pHorCursor){
		m_pHorCursor->SetPointOfStartVal(m_pCustomPlotVector[m_activePlot]->GetPointOfStartVal());
        m_pHorCursor->SetMinMax(m_pCustomGridX->GetAxisMin(gtHorizontal),
            m_pCustomGridX->GetAxisMax(gtHorizontal));
    }

	if (m_pVertCursor){
		m_pVertCursor->SetPointOfStartVal(m_pCustomPlotVector[m_activePlot]->GetPointOfStartVal(m_invertation));
		m_pVertCursor->SetMinMax(m_pCustomGridY->GetAxisMin(gtHorizontal), m_pCustomGridY->GetAxisMax(gtHorizontal));
	}
};
//------------------------------------------------------------------------------
void CGraph::SetAxisY(double minY, double maxY)
{
	if (minY == maxY)
		return;

    m_pCustomGridY->SetAxis(minY, maxY);

	if (m_pCustomPlotVector.empty())
		return;

	for (int Counter = 0; Counter < int(m_pCustomPlotVector.size()); Counter++)
	{
		m_pCustomPlotVector[Counter]->SetAxis(m_pCustomGridX->GetAxisMin(gtHorizontal),
			m_pCustomGridX->GetAxisMax(gtHorizontal),
			m_pCustomGridY->GetAxisMin(gtVertical),
			m_pCustomGridY->GetAxisMax(gtVertical));
	}

	if (m_pHorCursor){
		m_pHorCursor->SetPointOfStartVal(m_pCustomPlotVector[m_activePlot]->GetPointOfStartVal());
		m_pHorCursor->SetMinMax(m_pCustomGridX->GetAxisMin(gtHorizontal),
			m_pCustomGridX->GetAxisMax(gtHorizontal));
	}

	if (m_pVertCursor){
		m_pVertCursor->SetPointOfStartVal(m_pCustomPlotVector[m_activePlot]->GetPointOfStartVal(m_invertation));
		m_pVertCursor->SetMinMax(m_pCustomGridY->GetAxisMin(gtHorizontal), m_pCustomGridY->GetAxisMax(gtHorizontal));
	}
};
//------------------------------------------------------------------------------
void CGraph::SetCurrentTime(double NewTime)
{

	if (m_pCustomPlotVector.empty())
		return;

	for (int Counter = 0; Counter < int(m_pCustomPlotVector.size()); Counter++)
	{
		m_pCustomPlotVector[Counter]->SetSpace(NewTime 
			- (m_pCustomPlotVector[Counter]->GetEndValue()
			- m_pCustomPlotVector[Counter]->GetStartValue()),
			NewTime, m_pCustomPlotVector[Counter]->GetDataStep());
	}

    m_pCustomGridX->SetCurrentTime(NewTime);

	for (int Counter = 0; Counter < int(m_pCustomPlotVector.size()); Counter++)
	{
		m_pCustomPlotVector[Counter]->SetAxis(m_pCustomGridX->GetAxisMin(gtHorizontal),
			m_pCustomGridX->GetAxisMax(gtHorizontal),
			m_pCustomGridY->GetAxisMin(gtVertical),
			m_pCustomGridY->GetAxisMax(gtVertical));
	}

    if(m_pHorCursor){
		m_pHorCursor->SetPointOfStartVal(m_pCustomPlotVector[m_activePlot]->GetPointOfStartVal());
        m_pHorCursor->SetMinMax(m_pCustomGridX->GetAxisMin(gtHorizontal), m_pCustomGridX->GetAxisMax(gtHorizontal));
    }
}
//------------------------------------------------------------------------------
TwoDouble CGraph::GetAxisAbs(GridType gridType)
{
	TwoDouble ret;
	if(gridType == gtHorizontal){
		ret.Min = m_pCustomGridX->GetAxisAbsMin(gridType);
		ret.Max = m_pCustomGridX->GetAxisAbsMax(gridType);
	}
	if(gridType == gtVertical){
		ret.Min = m_pCustomGridY->GetAxisAbsMin(gridType);
		ret.Max = m_pCustomGridY->GetAxisAbsMax(gridType);
	}

	return ret;
}
//------------------------------------------------------------------------------
TwoDouble CGraph::GetAxis(GridType gridType)
{
    TwoDouble ret;
    if(gridType == gtHorizontal){
        ret.Min = m_pCustomGridX->GetAxisMin(gridType);
        ret.Max = m_pCustomGridX->GetAxisMax(gridType);
    }
    if(gridType == gtVertical){
        ret.Min = m_pCustomGridY->GetAxisMin(gridType);
        ret.Max = m_pCustomGridY->GetAxisMax(gridType);
    }

    return ret;
}
//------------------------------------------------------------------------------
double CGraph::GetStep(GridType gridType)
{
    if(gridType == gtHorizontal)
        return m_pCustomGridX->GetStep();

    if(gridType == gtVertical)
        return m_pCustomGridY->GetStep();

    return -1;
};
//------------------------------------------------------------------------------
CursorInfo CGraph::GetCursorInfo(int cursorId)
{
    CursorInfo temp;
	if (m_pHorCursor && cursorId < m_qntOfHorCursors && !m_invertation){
        if(m_cursorInfo.empty() || (m_cursorInfo.size() < cursorId+1))
        {
            m_cursorInfo.resize(cursorId+1);
            m_cursorInfo.resize(cursorId+1);
            m_cursorInfo.resize(cursorId+1);
        }
		m_cursorInfo[cursorId].ciTextX = m_pCustomPlotVector[m_activePlot]->GetCursorValueX(m_pHorCursor->GetCursorPosition(cursorId), m_plotType).X;
		m_cursorInfo[cursorId].ciTextY = m_pCustomPlotVector[m_activePlot]->GetCursorValueX(m_pHorCursor->GetCursorPosition(cursorId), m_plotType).Y;
            m_cursorInfo[cursorId].ciColor = RGB(0, 0, 0);
			m_cursorInfo[cursorId].ciX = m_pCustomPlotVector[m_activePlot]->GetCursorValueX(m_pHorCursor->GetCursorPosition(cursorId), m_plotType).pxX;
			m_cursorInfo[cursorId].ciY = m_pCustomPlotVector[m_activePlot]->GetCursorValueX(m_pHorCursor->GetCursorPosition(cursorId), m_plotType).pxY;
            m_cursorInfo[cursorId].ciRect = CRect(m_gridRect.left + m_cursorInfo[cursorId].ciX - 20,
            m_gridRect.top,
            m_gridRect.left + m_cursorInfo[cursorId].ciX + 20,
            m_gridRect.bottom);
    }

	if (m_pVertCursor && cursorId >= m_qntOfHorCursors && m_invertation){
		if (m_cursorInfo.empty() || m_cursorInfo.size() < cursorId + 1)
		{
			m_cursorInfo.resize(cursorId + 1);
			m_cursorInfo.resize(cursorId + 1);
			m_cursorInfo.resize(cursorId + 1);
		}
		m_cursorInfo[cursorId].ciTextX = m_pCustomPlotVector[m_activePlot]->GetCursorValueYInv(m_pVertCursor->GetCursorPosition(cursorId - m_qntOfHorCursors), m_plotType).X;
		m_cursorInfo[cursorId].ciTextY = m_pCustomPlotVector[m_activePlot]->GetCursorValueYInv(m_pVertCursor->GetCursorPosition(cursorId - m_qntOfHorCursors), m_plotType).Y;
		m_cursorInfo[cursorId].ciColor = RGB(0, 0, 0);
		m_cursorInfo[cursorId].ciX = m_pCustomPlotVector[m_activePlot]->GetCursorValueYInv(m_pVertCursor->GetCursorPosition(cursorId - m_qntOfHorCursors), m_plotType).pxX;
		m_cursorInfo[cursorId].ciY = m_pCustomPlotVector[m_activePlot]->GetCursorValueYInv(m_pVertCursor->GetCursorPosition(cursorId - m_qntOfHorCursors), m_plotType).pxY;
		m_cursorInfo[cursorId].ciRect = CRect(m_gridRect.left,
			m_cursorInfo[cursorId].ciY - 20,
			m_gridRect.right,
			m_cursorInfo[cursorId].ciY + 20);
	}

    return m_cursorInfo[cursorId];
}
//------------------------------------------------------------------------------
void CGraph::SetQntOfPlots(int QntOfPlots)
{
	m_pCustomPlotVector.resize(QntOfPlots);
	for (UINT Counter = 0; Counter < QntOfPlots; Counter++)
	{
		m_pCustomPlotVector[Counter] = new COscillPlot(this, stLinear, stLinear, m_gridRect);
		m_pCustomPlotVector[Counter]->SetPlotColor(RGB(rand() % 255, rand() % 255, rand() % 255));
	}
}
//------------------------------------------------------------------------------
void CGraph::SetFitX(FitType fitTypeX){
    m_pCustomGridX->SetFit(fitTypeX);
};
//------------------------------------------------------------------------------
void CGraph::SetFitY(FitType fitTypeY)
{
    m_pCustomGridY->SetFit(fitTypeY);
};
//------------------------------------------------------------------------------
void CGraph::SetAxisTitleX(CString titleX)
{
    m_pCustomGridX->SetAxisTitle(titleX, gtHorizontal);
}
//------------------------------------------------------------------------------
void CGraph::SetAxisTitleY(CString titleY)
{
    m_pCustomGridY->SetAxisTitle(titleY, gtVertical);
}
//------------------------------------------------------------------------------
void CGraph::SetScaleShiftRect()
{
	if(m_createButtons)
	{
		m_leftShiftRect = CRect(m_rect.left, m_rect.bottom + m_bottomBorder / 2.0,
			m_rect.left + m_rect.Width() / 4.0, m_rect.bottom
			+ m_bottomBorder);

		m_rightShiftRect = CRect(m_rect.right - m_rect.Width() / 4.0,
			m_rect.bottom + m_bottomBorder / 2.0, m_rect.right, m_rect.bottom
			+ m_bottomBorder);

		m_bottomShiftRect = CRect(m_rect.left - m_leftBorder,
			m_rect.bottom - m_rect.Height() / 4.0, m_rect.left - m_leftBorder / 2.0,
			m_rect.bottom);

		m_topShiftRect = CRect(m_rect.left - m_leftBorder,
			m_rect.top, m_rect.left - m_leftBorder / 2.0, m_rect.top
			+ m_rect.Height() / 4.0);

		m_leftScaleRect = CRect(m_rect.left + m_rect.Width() / 4.0,
			m_rect.bottom + m_bottomBorder / 2.0, m_rect.left + m_rect.Width() / 2.0,
			m_rect.bottom + m_bottomBorder);

		m_rightScaleRect = CRect(m_rect.right - m_rect.Width() / 2.0,
			m_rect.bottom + m_bottomBorder / 2.0, m_rect.right - m_rect.Width() / 4.0,
			m_rect.bottom + m_bottomBorder);

		m_topScaleRect = CRect(m_rect.left - m_leftBorder,
			m_rect.top + m_rect.Height() / 4.0, m_rect.left - m_leftBorder / 2.0,
			m_rect.top + m_rect.Height() / 2.0);

		m_bottomScaleRect = CRect(m_rect.left - m_leftBorder,
			m_rect.bottom - m_rect.Height() / 2.0, m_rect.left - m_leftBorder / 2.0,
			m_rect.bottom - m_rect.Height() / 4.0);

		m_centerAutoScaleRect = CRect(m_rect.left - m_leftBorder,
			m_rect.bottom, m_rect.left, m_rect.bottom + m_bottomBorder);
	}

	else
	{
		m_leftShiftRect = m_rightShiftRect = m_bottomShiftRect =
			m_topShiftRect = m_leftScaleRect = m_rightScaleRect =
			m_topScaleRect = m_bottomScaleRect = m_centerAutoScaleRect = CRect(0,
			0, 0, 0);
	}

    m_pDGraph->SetRectAll(m_leftShiftRect, m_rightShiftRect, m_bottomShiftRect,
		m_topShiftRect, m_leftScaleRect, m_rightScaleRect, m_topScaleRect,
		m_bottomScaleRect, m_centerAutoScaleRect, m_decorativeRect);
}
//------------------------------------------------------------------------------
void CGraph::Shift(zRect rectType){
    if(rectType == gLeftShift || rectType == gRightShift)
        m_pCustomGridX->Shift(rectType);
    if(rectType == gTopShift || rectType == gBottomShift)
        m_pCustomGridY->Shift(rectType);

	for (int Counter = 0; Counter < int(m_pCustomPlotVector.size()); Counter++)
	{
		m_pCustomPlotVector[Counter]->SetAxis(m_pCustomGridX->GetAxisMin(gtHorizontal),
			m_pCustomGridX->GetAxisMax(gtHorizontal),
			m_pCustomGridY->GetAxisMin(gtVertical),
			m_pCustomGridY->GetAxisMax(gtVertical));
	}

    if(m_pHorCursor){
		m_pHorCursor->SetPointOfStartVal(m_pCustomPlotVector[m_activePlot]->GetPointOfStartVal());
        m_pHorCursor->SetMinMax(m_pCustomGridX->GetAxisMin(gtHorizontal), m_pCustomGridX->GetAxisMax(gtHorizontal));
    }
}
//------------------------------------------------------------------------------
void CGraph::Scale(zRect rectType){
    if(rectType == gLeftScale || rectType == gRightScale)
        m_pCustomGridX->Scale(rectType);
    if(rectType == gTopScale || rectType == gBottomScale)
        m_pCustomGridY->Scale(rectType);

	for (int Counter = 0; Counter < int(m_pCustomPlotVector.size()); Counter++)
	{
		m_pCustomPlotVector[Counter]->SetAxis(m_pCustomGridX->GetAxisMin(gtHorizontal),
			m_pCustomGridX->GetAxisMax(gtHorizontal),
			m_pCustomGridY->GetAxisMin(gtVertical),
			m_pCustomGridY->GetAxisMax(gtVertical));
	}

    if(m_pHorCursor){
		m_pHorCursor->SetPointOfStartVal(m_pCustomPlotVector[m_activePlot]->GetPointOfStartVal());
        m_pHorCursor->SetMinMax(m_pCustomGridX->GetAxisMin(gtHorizontal), m_pCustomGridX->GetAxisMax(gtHorizontal));
    }
}
//------------------------------------------------------------------------------
void CGraph::AutoScaleX()
{
	if (!m_invertation)
		SetAxisX(m_pCustomPlotVector[m_activePlot]->GetStartValue(), m_pCustomPlotVector[m_activePlot]->GetEndValue());

	else
	{
		if ((m_pCustomPlotVector[m_activePlot]->GetMinMaxAxisX().Max - m_pCustomPlotVector[m_activePlot]->GetMinMaxAxisX().Min < Epsilon)
			&& (m_pCustomPlotVector[m_activePlot]->GetMinMaxAxisX().Max - 0 < Epsilon))
			return;

		m_pCustomGridX->AutoScale(m_pCustomPlotVector[m_activePlot]->GetMinMaxAxisX().Min, m_pCustomPlotVector[m_activePlot]->GetMinMaxAxisX().Max);

		for (int Counter = 0; Counter < int(m_pCustomPlotVector.size()); Counter++)
		{
			m_pCustomPlotVector[Counter]->SetAxis(m_pCustomGridX->GetAxisMin(gtHorizontal),
				m_pCustomGridX->GetAxisMax(gtHorizontal),
				m_pCustomGridY->GetAxisMin(gtVertical),
				m_pCustomGridY->GetAxisMax(gtVertical));
		}
		//if (m_pHorCursor)
		//{
		//	m_pHorCursor->SetPointOfStartVal(m_pCustomPlotVector[m_activePlot]->GetPointOfStartVal());
		//	m_pHorCursor->SetMinMax(m_pCustomGridX->GetAxisMin(gtHorizontal), m_pCustomGridX->GetAxisMax(gtHorizontal));
		//}
	}
}
//------------------------------------------------------------------------------
void CGraph::AutoScaleY()
{
	if (!m_invertation)
	{
		if ((m_pCustomPlotVector[m_activePlot]->GetMinMaxAxisY().Max - m_pCustomPlotVector[m_activePlot]->GetMinMaxAxisY().Min < Epsilon)
			&& (m_pCustomPlotVector[m_activePlot]->GetMinMaxAxisY().Max - 0 < Epsilon))
			return;
		
		m_pCustomGridY->AutoScale(m_pCustomPlotVector[m_activePlot]->GetMinMaxAxisY().Min, m_pCustomPlotVector[m_activePlot]->GetMinMaxAxisY().Max);
		
		for (int Counter = 0; Counter < int(m_pCustomPlotVector.size()); Counter++)
		{
			m_pCustomPlotVector[Counter]->SetAxis(m_pCustomGridX->GetAxisMin(gtHorizontal),
				m_pCustomGridX->GetAxisMax(gtHorizontal),
				m_pCustomGridY->GetAxisMin(gtVertical),
				m_pCustomGridY->GetAxisMax(gtVertical));
		}

		if (m_pHorCursor)
		{
			m_pHorCursor->SetPointOfStartVal(m_pCustomPlotVector[m_activePlot]->GetPointOfStartVal());
			m_pHorCursor->SetMinMax(m_pCustomGridX->GetAxisMin(gtHorizontal), m_pCustomGridX->GetAxisMax(gtHorizontal));
		}
	}

	else
		SetAxisY(m_pCustomPlotVector[m_activePlot]->GetStartValue(), m_pCustomPlotVector[m_activePlot]->GetEndValue());
}
//------------------------------------------------------------------------------
void CGraph::SetCursorPosition(int pointNumber)
{
	if (m_invertation && m_pVertCursor)
		m_pVertCursor->CalcCursorPosition(CPoint(0,
		m_pCustomPlotVector[m_activePlot]->GetCursorPositionInPoint(pointNumber, m_invertation)));
}
//------------------------------------------------------------------------------
void CGraph::OnMouseMove(UINT nFlags, CPoint point)
{
	//при перемещении курсора мыши в области,
	//выделенные под кнопки, подсвечиваем их

    HINSTANCE hInstance;
    hInstance = AfxGetInstanceHandle();

    if(PointInRect(point, &m_leftShiftRect)==true){
        m_pDGraph->BrighteningForArrows(true, 0);
        CCustomContainer::OnMouseMove(nFlags, point);
        return;

    }
    if(PointInRect(point, &m_rightShiftRect)==true){
        m_pDGraph->BrighteningForArrows(true, 5);
        CCustomContainer::OnMouseMove(nFlags, point);
        return;

    }
    if(PointInRect(point, &m_topShiftRect)==true){
        m_pDGraph->BrighteningForArrows(true, 6);
        CCustomContainer::OnMouseMove(nFlags, point);
        return;

    }
    if(PointInRect(point, &m_bottomShiftRect)==true){
        m_pDGraph->BrighteningForArrows(true, 11);
        CCustomContainer::OnMouseMove(nFlags, point);
        return;
    }
    if(PointInRect(point, &m_leftScaleRect)==true){
        m_pDGraph->BrighteningForArrows(true, 1);
        CCustomContainer::OnMouseMove(nFlags, point);
        return;
    }
    if(PointInRect(point, &m_rightScaleRect)==true){
        m_pDGraph->BrighteningForArrows(true, 3);
        CCustomContainer::OnMouseMove(nFlags, point);
        return;
    }
    if(PointInRect(point, &m_bottomScaleRect)==true){
        m_pDGraph->BrighteningForArrows(true, 9);
        CCustomContainer::OnMouseMove(nFlags, point);
        return;
    }
    if(PointInRect(point, &m_topScaleRect)==true){
        m_pDGraph->BrighteningForArrows(true, 7);
        CCustomContainer::OnMouseMove(nFlags, point);
        return;
    }
    if(PointInRect(point, &m_centerAutoScaleRect)==true){
        m_pDGraph->BrighteningForArrows(true, 12);
        CCustomContainer::OnMouseMove(nFlags, point);
        return;
    }
    m_hCursor = LoadCursorW(NULL, IDC_ARROW);
    SetClassLongW(GetParent()->m_hWnd, GCL_HCURSOR, (LONG)m_hCursor);
    m_pDGraph->BrighteningForArrows(false, -1);
    CCustomContainer::OnMouseMove(nFlags, point);
}
//------------------------------------------------------------------------------
void CGraph::DoAction()
{
    CPoint point;
    GetCursorPos(&point);
	CRect rect(0, 0, 0, 0);
    GetWindowRect(GetParent()->m_hWnd, &rect);
    point.x = point.x - rect.left - m_gridRect.left;
    point.y = point.y - rect.top;
    
	for (int cursorId = 0; cursorId < m_qntOfHorCursors; ++cursorId)
	{
        GetCursorInfo(cursorId);
		if (m_pDGraph->GetRectBubble(cursorId).Width() == 0 || m_pDGraph->GetRectBubble(cursorId).Height() == 0)
            continue;
        //здесь должно быть условие, по которому бабл должен исчезать, если курсор уходит за границу графика
        /*???*/
 
        m_pDGraph->SetRectBubble(CRect(m_gridRect.left + m_cursorInfo[cursorId].ciX + 2,
            m_gridRect.bottom - 50 - 2,
            m_gridRect.left + m_cursorInfo[cursorId].ciX + 100 +2,
            m_gridRect.bottom - 2), cursorId);
		m_pDGraph->SetTextBubble(m_cursorInfo[cursorId].ciTextX, m_cursorInfo[cursorId].ciTextY, cursorId);
    }

	for (int cursorId = m_qntOfHorCursors; cursorId < m_qntOfVertCursors + m_qntOfHorCursors; cursorId++){
		GetCursorInfo(cursorId);
		if (m_pDGraph->GetRectBubble(cursorId).Width() == 0 || m_pDGraph->GetRectBubble(cursorId).Height() == 0)
			continue;
		//здесь должно быть условие, по которому бабл должен исчезать, если курсор уходит за границу графика
		/*???*/
		m_pDGraph->SetRectBubble(CRect(m_gridRect.left + 2,
			m_cursorInfo[cursorId].ciY + 2,
			m_gridRect.left + 100 + 2,
			m_cursorInfo[cursorId].ciY + 50 + 2), cursorId);
		m_pDGraph->SetTextBubble(m_cursorInfo[cursorId].ciTextX, m_cursorInfo[cursorId].ciTextY, cursorId);
	}

}
//------------------------------------------------------------------------------
void CGraph::SaveParameters(SimpleXML *pSimpleXML)
{
    m_pDGraph->SaveParameters(pSimpleXML);
    CCustomContainer::SaveParameters(pSimpleXML);
}
//------------------------------------------------------------------------------
void CGraph::LoadParameters(SimpleXML *pSimpleXML)
{
    m_pDGraph->LoadParameters(pSimpleXML);
    CCustomContainer::LoadParameters(pSimpleXML);
}
//------------------------------------------------------------------------------
void CGraph::Clear(void)
{
	if(m_plotType == ptOscill)
    {
		m_pOscillPlot->Clear();
		m_pOscillGridX->Clear();
		m_pOscillGridY->Clear();
    }
	
	if(m_plotType == ptOscillLog)
	{
		m_pOscillPlot->Clear();
		m_pOscillLogGridX->Clear();
		m_pOscillGridY->Clear();
	}
    
	if(m_plotType == ptSpectr)
	{
		m_pSpectrBar->Clear();
		m_pOscillGridX->Clear();
		m_pOscillGridY->Clear();
    }
	
	if(m_plotType == ptSpectrLog)
	{
		m_pSpectrBar->Clear();
		m_pOscillLogGridX->Clear();
		m_pOscillGridY->Clear();
	}
    
	if(m_plotType == ptTrend)
    {
		if (m_pTrendGridX)
			m_pTrendGridX->Clear();
		if (m_pOscillGridY)
			m_pOscillGridY->Clear();
    }
	
	if (m_pHorCursor)
		m_pHorCursor->Clear();
	
	m_pDGraph->Clear();
}
//------------------------------------------------------------------------------
//	Настройка внешнего вида графика
//------------------------------------------------------------------------------
void CGraph::SetPlotColor(COLORREF PlotColor, int plotNumber)
{
	if(plotNumber > int(m_pCustomPlotVector.size() - 1))
		return;
	
	m_pCustomPlotVector[plotNumber]->SetPlotColor(PlotColor);
}
//------------------------------------------------------------------------------
void CGraph::SetGridColor(COLORREF GridColor)
{
	m_pCustomGridX->SetGridColor(GridColor);
	m_pCustomGridY->SetGridColor(GridColor);
}
//------------------------------------------------------------------------------
void CGraph::SetBackgroundColor(COLORREF BackgroundColor)
{
	m_pDGraph->SetBackgroundColor(BackgroundColor);
}