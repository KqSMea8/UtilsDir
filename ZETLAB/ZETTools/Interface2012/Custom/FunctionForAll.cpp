//------------------------------------------------------------------------------
//	FunctionForAll.h - файл реализации
//	Функции для использования во всех классах
//------------------------------------------------------------------------------
#include "StdAfx.h"
#include <Interface2012\Custom\FunctionForAll.h>

#include <map>
#ifdef _ACTIVEX_ZIS
extern CDZetIntSource *ZetIntSrc = nullptr;
#endif
typedef struct sTextureParams 
{
	sTextureParams(GLuint t, int w, int h)
	{
		Texture = t;
		TextureSize[0] = w;
		TextureSize[1] = h;
	};

	GLuint Texture;
	int TextureSize[2];
};

struct sTexture
{
	CString FileName;
    GLuint Texture;
	int Size[2];
	sTexture() 
		: FileName(L"")
		, Texture(0)
	{
		ZeroMemory(Size, sizeof(Size));
	}
};

typedef std::pair < CString, sTextureParams > TexturePair;
static  std::map  < CString, sTextureParams > TextureMap;
static  std::map  < CString, sTextureParams >::const_iterator MapIterator;

static std::vector <sTexture> Vector;

//------------------------------------------------------------------------------
// Точка принадлежит прямоугольнику
bool PointInRect(
	CPoint point,	// точка
	CRect *pRect)	// ук. на прямоугольник
{
	return  (pRect->left < point.x) && (point.x < pRect->right) &&
			(pRect->top  < point.y) && (point.y < pRect->bottom);
}
// Отрезок пересекает прямоугольник
//------------------------------------------------------------------------------
bool LineCrossRect(CPoint point1, CPoint point2, CRect *pRect)
{
	double a(0.0), b(0.0), y(0.0), x(0.0);

	if(PointInRect(point1, pRect))
		return true;

	if(PointInRect(point2, pRect))
		return true;

	if (point1.x == point2.x)
	{
		return false;
	}
	a = (point1.y - point2.y) / (double)(point1.x - point2.x);
	b = (point2.y*point1.x - point1.y*point2.x) / (double)(point1.x - point2.x);

	y = a*pRect->left + b;

	if (point1.y > point2.y)
	{
		if (y <= point1.y && y >= point2.y && y <= pRect->bottom && y>= pRect->top)
			return true;
	}

	else
	{
		if (y >= point1.y && y <= point2.y && y <= pRect->bottom && y >= pRect->top)
			return true;
	}
	
	y = a*pRect->right + b;

	if (point1.y > point2.y)
	{
		if (y <= point1.y && y >= point2.y && y <= pRect->bottom && y >= pRect->top)
			return true;
	}

	else
	{
		if (y >= point1.y && y <= point2.y && y <= pRect->bottom && y >= pRect->top)
			return true;
	}

	x = (pRect->bottom - b) / (double)a;


	if (point1.x > point2.x)
	{
		if (x <= point1.x && x >= point2.x && x >= pRect->left && x <= pRect->right)
			return true;
	}

	else
	{
		if (x >= point1.x && x <= point2.x && x >= pRect->left && x <= pRect->right)
			return true;
	}

	x = (pRect->top - b) / (double)a;

	if (point1.x > point2.x)
	{
		if (x <= point1.x && x >= point2.x && x >= pRect->left && x <= pRect->right)
			return true;
	}

	else
	{
		if (x >= point1.x && x <= point2.x && x >= pRect->left && x <= pRect->right)
			return true;
	}

	return false;
}
//------------------------------------------------------------------------------
// Задание параметров вложенного прямоугольника 
void DefineRect2(
	CRect *pRect,	// исходный прямоугольник
	CRect *pRect2,	// вложенный прямоугольник
	int height)		// ширина границы между ними
{
	pRect2->left   = pRect->left   + height;
	pRect2->top    = pRect->top    + height;
	pRect2->right  = pRect->right  - height;
	pRect2->bottom = pRect->bottom - height;
}
//------------------------------------------------------------------------------
// Задание параметров CheckBox
void DefineRect3(
	CRect *pRect,	// исходный прямоугольник
	CRect *pRect2)	// вложенный прямоугольник
{
	pRect2->left   = pRect->left;
	pRect2->top    = pRect->top;
	pRect2->right  = pRect->left + pRect->bottom - pRect->top;
	pRect2->bottom = pRect->bottom;
}
//------------------------------------------------------------------------------
//		Определение ключевых точек для прорисовки прямоугольника с
// закруглёнными углами.
//		pPointForLine - ук. на массив из 8 точек, являющихся точками
// касания строн прямоугольника и дуг в его углах. 0-ая точка
// массива - это точка касания верхней горизонтальной строны и дуги
// правого верхнего угла прямоугольника. Далее по часовой стрелке.
//		pPointForArc - ук. на массив из 4 точек, являющихся левыми верхними
// точками квадратов, в которые будут вписаны дуги. 0-ая точка массива - это
// точка дуги правого верхнего угла прямоугольника. Далее по часовой стрелке.
void DefinePointForRoundRect(
	CRect *pRect,			// исходный прямоугольник
	Point *pPointForLine,	// точки пересечения сторок с дугами
	Point *pPointForArc,	// ключевых точек для прорисовки дуг
	int radius)				// радиус закруглений
{
	pPointForLine[0].X = pRect->right - radius;
	pPointForLine[0].Y = pRect->top;

	pPointForLine[1].X = pRect->right;
	pPointForLine[1].Y = pRect->top + radius;

	pPointForLine[2].X = pRect->right;
	pPointForLine[2].Y = pRect->bottom - radius;

	pPointForLine[3].X = pPointForLine[0].X;
	pPointForLine[3].Y = pRect->bottom;

	pPointForLine[4].X = pRect->left + radius;
	pPointForLine[4].Y = pRect->bottom;

	pPointForLine[5].X = pRect->left;
	pPointForLine[5].Y = pPointForLine[2].Y;

	pPointForLine[6].X = pRect->left;
	pPointForLine[6].Y = pPointForLine[1].Y;

	pPointForLine[7].X = pPointForLine[4].X;
	pPointForLine[7].Y = pRect->top;

	int size = 2 * radius;	// сторона квадрата дуг
	pPointForArc[0].X = pRect->right - size;
	pPointForArc[0].Y = pRect->top;

	pPointForArc[1].X = pRect->right - size;
	pPointForArc[1].Y = pRect->bottom - size;

	pPointForArc[2].X = pRect->left;
	pPointForArc[2].Y = pPointForArc[1].Y;

	pPointForArc[3].X = pRect->left;
	pPointForArc[3].Y = pRect->top;
}
//------------------------------------------------------------------------------
#ifdef _GDIPLUS
// Чтение графического файла
bool LoadImageFromFile(
	Image **ppImage,	// куда загружать
	CString fileName)	// из какого файла брать
{
	bool ret;
	CString name(L"");
	if ( !fileName.IsEmpty() )
	{
		name = fileName;
		if (name.Find(L".") == -1)
			name += L".png";
		DeleteGdiPlusObject(*ppImage);
		*ppImage = Image::FromFile(name);
		if (*ppImage)
			ret = (*ppImage)->GetLastStatus() == Ok;
		else
			ret = false;
	}
	else
		ret = false;
	return ret;
}
//------------------------------------------------------------------------------
// Отображение строки символов
void DrawString(
	Graphics *pGraphics,		// контент отображения
	CString text,				// текст
	CZFont *pFont,				// шрифт
	RectF rect,					// прямоугольник для отображения
	StringAlignment alignmentHorizontal,	// выравнивание по горизонтали
	StringAlignment alignmentVertical)		// выравнивание по вертикали
{
	Color col;
	col.SetFromCOLORREF( pFont->GetColor() );
	SolidBrush colB(col);
	StringFormat format;
	format.SetAlignment(alignmentHorizontal);
	format.SetLineAlignment(alignmentVertical);
	pGraphics->DrawString(text, -1, pFont->GetFont(), rect, &format, &colB);					
}
//------------------------------------------------------------------------------
// Отображение линии
void DrawLine(
	Graphics *pGraphics,		// контент отображения
	CRect *pRect,
	COLORREF color,
	int width)				// координаты отрезка
{
	Color col;
	col.SetFromCOLORREF(color);
	Pen* LinePen = new Pen( col, width);
	//col.SetFromCOLORREF( );
	//const Pen* pen;
	//SolidBrush colB(col);
	//StringFormat format;
	pGraphics->DrawLine(LinePen, pRect->left, pRect->top, pRect->right, pRect->bottom);				
}
//------------------------------------------------------------------------------
// На основе исходного прямоугольника в GraphicsPath строится замкнутая фигура,
// являющаяся прямоугольником с закруглёнными углами.
void RoundRectInGraphicsPath(
	GraphicsPath *pGraphPath,		// замкнутая фигура
	CRect *pRect,					// исходный прямоугольник
	int radius)						// радиус закруглений
{
	int side = radius + radius;	// сторона квадратов, в которых надо рисовать дуги
	Point p[8], P[4];
	DefinePointForRoundRect(pRect, p, P, radius);
	pGraphPath->Reset();
	pGraphPath->AddArc(P[0].X, P[0].Y, side, side, 270, 90);// top right arc
	pGraphPath->AddLine(p[1], p[2]);	 // right line
	pGraphPath->AddArc(P[1].X, P[1].Y, side, side, 0, 90);	// bottom right arc
	pGraphPath->AddLine(p[3], p[4]);	// bottom line
	pGraphPath->AddArc(P[2].X, P[2].Y, side, side, 90, 90);	// bottom left arc
	pGraphPath->AddLine(p[5], p[6]);    // left line
	pGraphPath->AddArc(P[3].X, P[3].Y, side, side, 180, 90);// top left arc
//	pGraphPath->AddLine(p[7], p[0]);    // top line	
	pGraphPath->CloseFigure();
	// top line	не рисуется т.к. GraphPath - это замкнутая фигура, дорисует сама
}	// при выполнении CloseFigure

void DrawRadiusRect(Graphics& graphics, Pen* pen, float X, float Y, float width, float height, float radius)
{
	GraphicsPath* gp = new GraphicsPath();
	gp->AddLine(X + radius, Y, X + width - (radius*2), Y);
	gp->AddArc(X + width - (radius*2), Y, radius*2, radius*2, 270, 90);
	gp->AddLine(X + width, Y + radius, X + width, Y + height - (radius*2));
	gp->AddArc(X + width - (radius*2), Y + height - (radius*2), radius*2, radius*2,0,90);
	gp->AddLine(X + width - (radius*2), Y + height, X + radius, Y + height);
	gp->AddArc(X, Y + height - (radius*2), radius*2, radius*2, 90, 90);
	gp->AddLine(X, Y + height - (radius*2), X, Y + radius);
	gp->AddArc(X, Y, radius*2, radius*2, 180, 90);
	gp->CloseFigure();
	graphics.DrawPath(pen, gp);
	DeleteObject(&graphics);
	delete gp;
	gp = nullptr;
}
//------------------------------------------------------------------------------
// На основе исходного прямоугольника в GraphicsPath строится замкнутая фигура,
// являющаяся прямоугольником с двумя закруглёнными углами слева.
void RoundRectInGraphicsPathLeft(
	GraphicsPath *pGraphPath,		// замкнутая фигура
	CRect *pRect,					// исходный прямоугольник
	int radius)						// радиус закруглений
{
	int side = radius + radius;	// сторона квадратов, в которых надо рисовать дуги
	Point p[8], P[4];
	DefinePointForRoundRect(pRect, p, P, radius);
	p[4].X = pRect->left;
	p[4].Y = pRect->bottom;
	p[5].X = p[4].X;
	p[5].Y = pRect->top;
	pGraphPath->Reset();
	pGraphPath->AddArc(P[0].X, P[0].Y, side, side, 270, 90);// top right arc
	pGraphPath->AddLine(p[1], p[2]);	 // right line
	pGraphPath->AddArc(P[1].X, P[1].Y, side, side, 0, 90);	// bottom right arc 
	pGraphPath->AddLine(p[3], p[4]);	// bottom line
	pGraphPath->AddLine(p[4], p[5]);    // top line
	pGraphPath->CloseFigure();
	// top line	не рисуется т.к. GraphPath - это замкнутая фигура, дорисует сама
}	// при выполнении CloseFigure
//------------------------------------------------------------------------------
// На основе исходного прямоугольника в GraphicsPath строится замкнутая фигура,
// являющаяся прямоугольником с двумя закруглёнными углами снизу
void RoundRectInGraphicsPathDown(
	GraphicsPath *pGraphPath,		// замкнутая фигура
	CRect *pRect,					// исходный прямоугольник
	int radius)						// радиус закруглений
{
	int side = radius + radius;	// сторона квадратов, в которых надо рисовать дуги
	Point p[8], P[4];
	DefinePointForRoundRect(pRect, p, P, radius);
	p[1].X = pRect->right;
	p[1].Y = pRect->top;
	p[6].X = pRect->left;
	p[6].Y = p[1].Y;
	pGraphPath->Reset();
	pGraphPath->AddLine(p[1], p[2]);	 // right line
	pGraphPath->AddArc(P[1].X, P[1].Y, side, side, 0, 90);	// bottom right arc
	pGraphPath->AddLine(p[3], p[4]);	// bottom line
	pGraphPath->AddArc(P[2].X, P[2].Y, side, side, 90, 90);	// bottom left arc
	pGraphPath->AddLine(p[5], p[6]);    // left line
	pGraphPath->CloseFigure();
	// top line	не рисуется т.к. GraphPath - это замкнутая фигура, дорисует сама
}	// при выполнении CloseFigure
//------------------------------------------------------------------------------
// Заливка прямоугольника одним цветом с предварительным закруглением углов
void FillRoundRectSolid(
	Graphics *pGraphics,			// контент отображения
	CRect *pRect,					// исходный прямоугольник
	int radius,						// радиус закруглений
	COLORREF color,					// цвет заливки
	GraphicsPath *pGraphicsPath)	// графическая фигура
{
	// инициализация
	pGraphics->SetSmoothingMode(SmoothingModeAntiAlias);
	pGraphics->SetInterpolationMode(InterpolationModeHighQualityBicubic);
	Color col;
	col.SetFromCOLORREF(color);
	SolidBrush sol_brush(col);
	
	// замкнутая фигура для заполнения
	GraphicsPath *pTemp;
	if (pGraphicsPath)
		pTemp = pGraphicsPath;
	else
		pTemp = new GraphicsPath();
	RoundRectInGraphicsPath(pTemp, pRect, radius);

	// заливка
	pGraphics->FillPath(&sol_brush, pTemp);

	// завершение
	if (pGraphicsPath == NULL)
		DeleteGdiPlusObject(pTemp);
}
//------------------------------------------------------------------------------
// Заливка прямоугольника одним цветом с предварительным закруглением двух углов слева
void FillRoundRectSolidLeft(
	Graphics *pGraphics,			// контент отображения
	CRect *pRect,					// исходный прямоугольник
	int radius,						// радиус закруглений
	COLORREF color,					// цвет заливки
	GraphicsPath *pGraphicsPath)	// графическая фигура
{
	// инициализация
	pGraphics->SetSmoothingMode(SmoothingModeAntiAlias);
	pGraphics->SetInterpolationMode(InterpolationModeHighQualityBicubic);
	Color col;
	col.SetFromCOLORREF(color);
	SolidBrush sol_brush(col);

	// замкнутая фигура для заполнения
	GraphicsPath *pTemp;
	if (pGraphicsPath)
		pTemp = pGraphicsPath;
	else
		pTemp = new GraphicsPath();
	RoundRectInGraphicsPathLeft(pTemp, pRect, radius);

	// заливка
	pGraphics->FillPath(&sol_brush, pTemp);

	// завершение
	if (pGraphicsPath == NULL)
		DeleteGdiPlusObject(pTemp);
}
//------------------------------------------------------------------------------
// Заполнение прямоугольника цветовым градиентом с предварительным
// закруглением углов
void FillRoundRectLinearGradient(
	Graphics *pGraphics,			// контент отображения
	CRect *pRect,					// исходный прямоугольник
	int radius,						// радиус закруглений
	COLORREF colorStart,			// начальный цвет градиента
	COLORREF colorEnd,				// конечный цвет градиента
	LinearGradientMode gradientMode,// тип градиента
	GraphicsPath *pGraphicsPath)	// графическая фигура
{
	// инициализация
	pGraphics->SetSmoothingMode(SmoothingModeAntiAlias);
	pGraphics->SetInterpolationMode(InterpolationModeHighQualityBicubic);
	Color colSta, colEnd;
	colSta.SetFromCOLORREF(colorStart);
	colEnd.SetFromCOLORREF(colorEnd);
	LinearGradientBrush lg_brush(
		Rect( pRect->left, pRect->top, pRect->Width(), pRect->Height() ),
		colSta, colEnd, gradientMode);
	
	// замкнутая фигура для заполнения
	GraphicsPath *pTemp;
	if (pGraphicsPath)
		pTemp = pGraphicsPath;
	else
		pTemp = new GraphicsPath();
	RoundRectInGraphicsPath(pTemp, pRect, radius);

	// заливка
	pGraphics->FillPath(&lg_brush, pTemp);

	// завершение
	if (pGraphicsPath == NULL)
		DeleteGdiPlusObject(pTemp);
}
//------------------------------------------------------------------------------
// Заполнение прямоугольника цветовым градиентом с предварительным
// закруглением двух углов
void FillRoundRectLinGradLeft(
	Graphics *pGraphics,			// контент отображения
	CRect *pRect,					// исходный прямоугольник
	int radius,						// радиус закруглений
	COLORREF colorStart,			// начальный цвет градиента
	COLORREF colorEnd,				// конечный цвет градиента
	LinearGradientMode gradientMode,// тип градиента
	GraphicsPath *pGraphicsPath)	// графическая фигура
{
	// инициализация
	pGraphics->SetSmoothingMode(SmoothingModeAntiAlias);
	pGraphics->SetInterpolationMode(InterpolationModeHighQualityBicubic);
	Color colSta, colEnd;
	colSta.SetFromCOLORREF(colorStart);
	colEnd.SetFromCOLORREF(colorEnd);
	LinearGradientBrush lg_brush(
		Rect( pRect->left, pRect->top, pRect->Width(), pRect->Height() ),
		colSta, colEnd, gradientMode);

	// замкнутая фигура для заполнения
	GraphicsPath *pTemp;
	if (pGraphicsPath)
		pTemp = pGraphicsPath;
	else
		pTemp = new GraphicsPath();
	RoundRectInGraphicsPathLeft(pTemp, pRect, radius);

	// заливка
	pGraphics->FillPath(&lg_brush, pTemp);

	// завершение
	if (pGraphicsPath == NULL)
		DeleteGdiPlusObject(pTemp);
}
//------------------------------------------------------------------------------
// Заполнение прямоугольника цветовым градиентом с предварительным
// закруглением двух углов снизу
void FillRoundRectLinGradDown(
	Graphics *pGraphics,			// контент отображения
	CRect *pRect,					// исходный прямоугольник
	int radius,						// радиус закруглений
	COLORREF colorStart,			// начальный цвет градиента
	COLORREF colorEnd,				// конечный цвет градиента
	LinearGradientMode gradientMode,// тип градиента
	GraphicsPath *pGraphicsPath)	// графическая фигура
{
	// инициализация
	pGraphics->SetSmoothingMode(SmoothingModeAntiAlias);
	pGraphics->SetInterpolationMode(InterpolationModeHighQualityBicubic);
	Color colSta, colEnd;
	colSta.SetFromCOLORREF(colorStart);
	colEnd.SetFromCOLORREF(colorEnd);
	LinearGradientBrush lg_brush(
		Rect( pRect->left, pRect->top, pRect->Width(), pRect->Height() ),
		colSta, colEnd, gradientMode);

	// замкнутая фигура для заполнения
	GraphicsPath *pTemp;
	if (pGraphicsPath)
		pTemp = pGraphicsPath;
	else
		pTemp = new GraphicsPath();
	RoundRectInGraphicsPathDown(pTemp, pRect, radius);

	// заливка
	pGraphics->FillPath(&lg_brush, pTemp);

	// завершение
	if (pGraphicsPath == NULL)
		DeleteGdiPlusObject(pTemp);
}
//------------------------------------------------------------------------------
// Заливка прямоугольника одним цветом
void FillRectSolid(
	Graphics *pGraphics,			// контент отображения
	CRect *pRect,					// исходный прямоугольник
	COLORREF color)					// цвет заливки
{
	// инициализация
	Color col;
	col.SetFromCOLORREF(color);
	SolidBrush sol_brush(col);
	
	// заливка
	pGraphics->FillRectangle( &sol_brush, pRect->left, pRect->top,
								pRect->Width(), pRect->Height());
}
//------------------------------------------------------------------------------
void FillRectLinearGradient(
	Graphics *pGraphics,			// контент отображения
	CRect *pRect,					// исходный прямоугольник
	COLORREF colorStart,			// начальный цвет градиента
	COLORREF colorEnd,				// конечный цвет градиента
	LinearGradientMode gradientMode)// тип градиента
{
	// инициализация
	Color colSta, colEnd;
	colSta.SetFromCOLORREF(colorStart);
	colEnd.SetFromCOLORREF(colorEnd);
	LinearGradientBrush lg_brush(
		Rect( pRect->left, pRect->top, pRect->Width(), pRect->Height() ),
		colSta, colEnd, gradientMode);
	
	// заливка
	pGraphics->FillRectangle( &lg_brush, pRect->left, pRect->top,
								pRect->Width(), pRect->Height() );
}
#else
GLuint LoadGLNormalMap(CString FileName, int *size, long number)                            // Load Bitmaps And Convert To Textures
{
	GLuint texture[1] = {0};
	GLuint texture_Cur = {0};
    sTexture temp;

	/*MapIterator = TextureMap.find(FileName);
	if (MapIterator != TextureMap.end())
	{
		size[0] = MapIterator->second.TextureSize[0];
		size[1] = MapIterator->second.TextureSize[1];
		return MapIterator->second.Texture;
	}*/
    if(!Vector.empty())
    {
		for (UINT Counter = 0; Counter<Vector.size(); ++Counter)
        {
            if(FileName == Vector[Counter].FileName)
            {
                size[0] =  Vector[Counter].Size[0];
                size[1] =  Vector[Counter].Size[1];
                return Vector[Counter].Texture;
            }
        }
    }

	int Status=FALSE;                           // Status Indicator
	AUX_RGBImageRec *TextureImage[1];   
	memset(TextureImage, 0, sizeof(void *)*1);
	// Load The Bitmap, Check For Errors, If Bitmap's Not Found Quit
	int iNeeded = WideCharToMultiByte( CP_ACP, 0, FileName, -1, NULL, 0, NULL, NULL);
	char* FName = new char[iNeeded];
	WideCharToMultiByte( CP_ACP, 0, FileName, -1, FName, iNeeded, NULL, NULL);
#ifdef _ACTIVEX_ZIS
//	if (TextureImage[0] = (AUX_RGBImageRec*)ZetIntSrc->get_MainImagePointer(nullptr, number))//LoadBMP(FName)
#else
	if (TextureImage[0] = LoadBMP(FName))
#endif
    {
#ifdef _ACTIVEX_ZIS
        GLubyte bits[64][64][4];
		unsigned char *d;
		long sizeX = ZetIntSrc->get_SizeOfImage(number, 0);
		long sizeY = ZetIntSrc->get_SizeOfImage(number, 1);
		long sizeP = ZetIntSrc->get_SizeOfImage(number, 2);
		d = new unsigned char [ZetIntSrc->get_SizeOfImage(number, 2)];
		//d = new unsigned char [sizeP];
		ZetIntSrc->get_MainImagePointer((LONG*)d, number);
		for(int i = 0; i < ZetIntSrc->get_SizeOfImage(number, 0); i++)
		//for(int i = 0; i < sizeX; i++)
        {
            for(int j = 0; j < ZetIntSrc->get_SizeOfImage(number, 1); j++)
			//for(int j = 0; j < sizeY; j++)
            {
                bits[i][j][0]= d[((ZetIntSrc->get_SizeOfImage(number, 0)-1-i)*ZetIntSrc->get_SizeOfImage(number, 0)+j)*3];
                bits[i][j][1]= d[((ZetIntSrc->get_SizeOfImage(number, 0)-1-i)*ZetIntSrc->get_SizeOfImage(number, 0)+j)*3 + 1];
                bits[i][j][2]= d[((ZetIntSrc->get_SizeOfImage(number, 0)-1-i)*ZetIntSrc->get_SizeOfImage(number, 0)+j)*3 + 2];
				//bits[i][j][0]= d[((sizeX-1-i)*sizeY+j)*3];
				//bits[i][j][1]= d[((sizeX-1-i)*sizeY+j)*3 + 1];
				//bits[i][j][2]= d[((sizeX-1-i)*sizeY+j)*3 + 2];
                if ((bits[i][j][0] == 255) && (bits[i][j][1] == 255) && (bits[i][j][2] == 255)) 
				{
                    bits[i][j][3]= (GLbyte)0;
                }
                else
                    bits[i][j][3]= (GLbyte)64;
                //bits[i][j][3] = 255 - (bits[i][j][0]+bits[i][j][1]+bits[i][j][2]) / 3;
                //bits[i][j][0]= TextureImage[0]->data[(j*TextureImage[0]->sizeX+i)*3];
                //bits[i][j][1]= TextureImage[0]->data[(j*TextureImage[0]->sizeX+i)*3 + 1];
                //bits[i][j][2]= TextureImage[0]->data[(j*TextureImage[0]->sizeX+i)*3 + 2];
            }
        }
#else
        GLubyte bits[64][64][4];
        //GLubyte bits[512][512][3];
        for(int i = 0; i < TextureImage[0]->sizeX; i++)
        {
            for(int j = 0; j < TextureImage[0]->sizeY; j++)
            {
                bits[i][j][0]= TextureImage[0]->data[((TextureImage[0]->sizeX-1-i)*TextureImage[0]->sizeX+j)*3];
                bits[i][j][1]= TextureImage[0]->data[((TextureImage[0]->sizeX-1-i)*TextureImage[0]->sizeX+j)*3 + 1];
                bits[i][j][2]= TextureImage[0]->data[((TextureImage[0]->sizeX-1-i)*TextureImage[0]->sizeX+j)*3 + 2];
                if ((bits[i][j][0] == 255) && (bits[i][j][1]==255) &&
                    (bits[i][j][2]==255)) {
                        bits[i][j][3]= (GLbyte)0;
                }
                else
                    bits[i][j][3]= (GLbyte)255;
                //bits[i][j][3] = 255 - (bits[i][j][0]+bits[i][j][1]+bits[i][j][2]) / 3;
                //bits[i][j][0]= TextureImage[0]->data[(j*TextureImage[0]->sizeX+i)*3];
                //bits[i][j][1]= TextureImage[0]->data[(j*TextureImage[0]->sizeX+i)*3 + 1];
                //bits[i][j][2]= TextureImage[0]->data[(j*TextureImage[0]->sizeX+i)*3 + 2];
            }
        }
#endif
		Status = TRUE;    // Set The Pointer To NULL
		glGenTextures(1, &texture[0]);                  // Create The Texture
		// Typical Texture Generation Using Data From The Bitmap
		glBindTexture(GL_TEXTURE_2D, texture[0]);
		// Generate The Texture

		/* Задание параметров текстуры */
		/* Повтор изображения по параметрическим осям s и t */
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

#ifdef _ACTIVEX_ZIS
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, ZetIntSrc->get_SizeOfImage(number, 0), 
			ZetIntSrc->get_SizeOfImage(number, 1), 0, GL_RGBA, GL_UNSIGNED_BYTE, bits);
		size[0] = ZetIntSrc->get_SizeOfImage(number, 0);
		size[1] = ZetIntSrc->get_SizeOfImage(number, 1);
		delete [] d;
		d = nullptr;
#else
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, TextureImage[0]->sizeX, TextureImage[0]->sizeY, 0, GL_RGBA, GL_UNSIGNED_BYTE, bits);
		size[0] = TextureImage[0]->sizeX;
		size[1] = TextureImage[0]->sizeY;
#endif
	}
#ifdef _ACTIVEX_ZIS
#else
	if (TextureImage[0])                            // If Texture Exists
	{
		if (TextureImage[0]->data)                  // If Texture Image Exists
		{
			free(TextureImage[0]->data);                // Free The Texture Image Memory
			TextureImage[0]->data = nullptr;
		}

		free(TextureImage[0]);                      // Free The Image Structure
		TextureImage[0] = nullptr;
	}
#endif
    temp.FileName = FileName;
    temp.Texture = texture[0];
    temp.Size[0] = size[0];
    temp.Size[1] = size[1];
    Vector.push_back(temp);

	delete [] FName;
	FName = nullptr;

	return texture[0];
}
//------------------------------------------------------------------------------
GLuint LoadGLTextures(CString FileName, int *size, int number)                                // Load Bitmaps And Convert To Textures
{
	GLuint texture[1] = {0};
	GLuint texture_Cur = { 0 };
    sTexture temp;

	/*MapIterator = TextureMap.find(FileName);
	if (MapIterator != TextureMap.end())
	{
		size[0] = MapIterator->second.TextureSize[0];
		size[1] = MapIterator->second.TextureSize[1];
		return MapIterator->second.Texture;
	}*/

    if(!Vector.empty())
    {
        for(UINT Counter = 0; Counter<Vector.size(); ++Counter)
        {
            if(FileName == Vector[Counter].FileName)
            {
                size[0] =  Vector[Counter].Size[0];
                size[1] =  Vector[Counter].Size[1];
                return Vector[Counter].Texture;
            }
        }
    }//*/

	int Status=FALSE;                           // Status Indicator
	AUX_RGBImageRec *TextureImage[1];   
	memset(TextureImage, 0, sizeof(void *)*1);
	// Load The Bitmap, Check For Errors, If Bitmap's Not Found Quit
	int iNeeded = WideCharToMultiByte( CP_ACP, 0, FileName, -1, NULL, 0, NULL, NULL);
	char* FName = new char[iNeeded];
	WideCharToMultiByte( CP_ACP, 0, FileName, -1, FName, iNeeded, NULL, NULL);
#ifdef _ACTIVEX_ZIS
//	if (TextureImage[0] = (AUX_RGBImageRec*)ZetIntSrc->get_MainImagePointer(nullptr, number))//LoadBMP(FName)
#else
	if (TextureImage[0] = LoadBMP(FName))
#endif
	{
		Status = TRUE;    // Set The Pointer To NULL
		glGenTextures(1, &texture[0]);                  // Create The Texture
		// Typical Texture Generation Using Data From The Bitmap
		glBindTexture(GL_TEXTURE_2D, texture[0]);
		// Generate The Texture

		/* Задание параметров текстуры */
		/* Повтор изображения по параметрическим осям s и t */
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
#ifdef _ACTIVEX_ZIS
		unsigned char *d;
		d = new unsigned char [ZetIntSrc->get_SizeOfImage(number, 2)];
		ZetIntSrc->get_MainImagePointer((LONG*)d, number);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, ZetIntSrc->get_SizeOfImage(number, 0), 
			ZetIntSrc->get_SizeOfImage(number, 1), 0, GL_RGB, GL_UNSIGNED_BYTE, d);
		size[0] = ZetIntSrc->get_SizeOfImage(number, 0);
		size[1] = ZetIntSrc->get_SizeOfImage(number, 1);
		delete [] d;
		d = nullptr;
#else
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, TextureImage[0]->sizeX, TextureImage[0]->sizeY, 0, GL_RGB, GL_UNSIGNED_BYTE, TextureImage[0]->data);
		size[0] = TextureImage[0]->sizeX;
		size[1] = TextureImage[0]->sizeY;
#endif
	}
#ifdef _ACTIVEX_ZIS
#else
	if (TextureImage[0])                            // If Texture Exists
	{
		if (TextureImage[0]->data)                  // If Texture Image Exists
		{
			free(TextureImage[0]->data);                // Free The Texture Image Memory
			TextureImage[0]->data = nullptr;
		}
		free(TextureImage[0]);						// Free The Image Structure
		TextureImage[0] = nullptr;
	}
#endif	
	delete [] FName;
	FName = nullptr;

    temp.FileName = FileName;
    temp.Texture = texture[0];
    temp.Size[0] = size[0];
    temp.Size[1] = size[1];
    Vector.push_back(temp);
	return texture[0];                              // Return Texture
}
//------------------------------------------------------------------------------
AUX_RGBImageRec* LoadBMP(char *Filename)                    // Loads A Bitmap Image
{
	FILE *File(nullptr);    
	if (!Filename)                              // Make Sure A Filename Was Given
	{
		return NULL;                            // If Not Return NULL
	}
	File=fopen(Filename,"r");                       // Check To See If The File Exists
	if (File)                               // Does The File Exist?
	{
		fclose(File);                           // Close The Handle
		File = nullptr;
		return auxDIBImageLoadA(Filename);               // Load The Bitmap And Return A Pointer
	}
	return NULL;                                // If Load Failed Return NULL
}
//------------------------------------------------------------------------------
void FillRectSolid(CRect *pRect, float Z, COLORREF color)
{
	glPushAttrib(GL_LIGHTING_BIT);
	glDisable(GL_LIGHTING);
	glEnable(GL_COLOR_MATERIAL);
	glBegin(GL_POLYGON);
	glColor3ub(GetRValue(color), GetGValue(color), GetBValue(color));
	glVertex3f(pRect->left, pRect->bottom, Z);
	glVertex3f(pRect->left, pRect->top, Z);
	glVertex3f(pRect->right, pRect->top, Z);
	glVertex3f(pRect->right, pRect->bottom, Z);
	glEnd();
	glPopAttrib();
}
//------------------------------------------------------------------------------
void FillRoundRectSolid(CRect *pRect, float Z, int radius, COLORREF color)
{
	Point p[8], P[4];
	float xt(0.f), yt(0.f);
	DefinePointForRoundRect(pRect, p, P, radius);
    glPushAttrib(GL_ENABLE_BIT | GL_LIGHTING_BIT);
    glPushClientAttrib(GL_ENABLE_BIT | GL_LIGHTING_BIT);
	glDisable(GL_LIGHTING);
	glEnable(GL_COLOR_MATERIAL);
	glBegin(GL_POLYGON);
	glColor3ub(GetRValue(color), GetGValue(color), GetBValue(color));
	for(float t = 0.0; t <= 1.0; t += 0.1) {
		xt = (p[0].X*(1-t) + 2*t*pRect->right)*(1-t) + pow(t,2)*p[1].X;
		yt = (p[0].Y*(1-t) + 2*t*pRect->top)*(1-t) + pow(t,2)*p[1].Y;
		glVertex3f(xt, yt, Z);
	}
	for(float t = 0.0; t <= 1.0; t += 0.1) {
		xt = (p[2].X*(1-t) + 2*t*pRect->right)*(1-t) + pow(t,2)*p[3].X;
		yt = (p[2].Y*(1-t) + 2*t*pRect->bottom)*(1-t) + pow(t,2)*p[3].Y;
		glVertex3f(xt, yt, Z);
	}
	for(float t = 0.0; t <= 1.0; t += 0.1) {
		xt = (p[4].X*(1-t) + 2*t*pRect->left)*(1-t) + pow(t,2)*p[5].X;
		yt = (p[4].Y*(1-t) + 2*t*pRect->bottom)*(1-t) + pow(t,2)*p[5].Y;
		glVertex3f(xt, yt, Z);
	}
	for(float t = 0.0; t <= 1.0; t += 0.1) {
		xt = (p[6].X*(1-t) + 2*t*pRect->left)*(1-t) + pow(t,2)*p[7].X;
		yt = (p[6].Y*(1-t) + 2*t*pRect->top)*(1-t) + pow(t,2)*p[7].Y;
		glVertex3f(xt, yt, Z);
	}
	glVertex3f(p[0].X, p[0].Y, Z);
	glEnd();
    glPopClientAttrib();
    glPopAttrib();
}
//------------------------------------------------------------------------------
void FillRoundRectLinearGradient(CRect *pRect, float Z,
	int radius, COLORREF colorStart, COLORREF colorEnd)
{
	Point p[8], P[4];
	float xt(0.f), yt(0.f);
	DefinePointForRoundRect(pRect, p, P, radius);
	glPushAttrib(GL_LIGHTING_BIT);
	glDisable(GL_LIGHTING);
	glEnable(GL_COLOR_MATERIAL);
	glBegin(GL_POLYGON);
	glColor3ub(GetRValue(colorStart), GetGValue(colorStart), GetBValue(colorStart));
	for(float t = 0.0; t <= 1.0; t += 0.1) {
		xt = (p[0].X*(1-t) + 2*t*pRect->right)*(1-t) + pow(t,2)*p[1].X;
		yt = (p[0].Y*(1-t) + 2*t*pRect->top)*(1-t) + pow(t,2)*p[1].Y;
		glVertex3f(xt, yt, Z);
	}
	for(float t = 0.0; t <= 1.0; t += 0.1) {
		xt = (p[2].X*(1-t) + 2*t*pRect->right)*(1-t) + pow(t,2)*p[3].X;
		yt = (p[2].Y*(1-t) + 2*t*pRect->bottom)*(1-t) + pow(t,2)*p[3].Y;
		glVertex3f(xt, yt, Z);
	}
	for(float t = 0.0; t <= 1.0; t += 0.1) {
		xt = (p[4].X*(1-t) + 2*t*pRect->left)*(1-t) + pow(t,2)*p[5].X;
		yt = (p[4].Y*(1-t) + 2*t*pRect->bottom)*(1-t) + pow(t,2)*p[5].Y;
		glVertex3f(xt, yt, Z);
	}
	for(float t = 0.0; t <= 1.0; t += 0.1) {
		xt = (p[6].X*(1-t) + 2*t*pRect->left)*(1-t) + pow(t,2)*p[7].X;
		yt = (p[6].Y*(1-t) + 2*t*pRect->top)*(1-t) + pow(t,2)*p[7].Y;
		glVertex3f(xt, yt, Z);
	}

	glVertex3f(p[0].X, p[0].Y, Z);
	glEnd();
	glPopAttrib();
}
//------------------------------------------------------------------------------
void FillRectLinearGradient(CRect *pRect, float Z,
	COLORREF colorStart, COLORREF colorEnd)
{
	glPushAttrib(GL_LIGHTING_BIT);
	glDisable(GL_LIGHTING);
	glEnable(GL_COLOR_MATERIAL);
	glBegin(GL_POLYGON);
	glColor3ub(GetRValue(colorStart), GetGValue(colorStart), GetBValue(colorStart));
	glVertex3f(pRect->left, pRect->bottom, Z);
	glVertex3f(pRect->left, pRect->top, Z);
	glVertex3f(pRect->right, pRect->top, Z);
	glColor3ub(GetRValue(colorEnd), GetGValue(colorEnd), GetBValue(colorEnd));
	glVertex3f(pRect->right, pRect->bottom, Z);
	glEnd();
	glPopAttrib();
}
//------------------------------------------------------------------------------
void FillTriangleSolid(CRect *pRect, float Z, COLORREF color)
{
	glPushAttrib(GL_LIGHTING_BIT);
	glDisable(GL_LIGHTING);
	glEnable(GL_COLOR_MATERIAL);
	glBegin(GL_POLYGON);
	glColor3ub(GetRValue(color), GetGValue(color), GetBValue(color));
	glVertex3f(pRect->left, pRect->bottom, Z);
	glVertex3f((pRect->left + pRect->right)/2, pRect->top, Z);
	glVertex3f(pRect->right, pRect->bottom, Z);
	glEnd();
	glPopAttrib();
}
//------------------------------------------------------------------------------
void FillParallepiped(CRect *pRect, float Z_up, float Z_down, COLORREF color)
{
	glPushAttrib(GL_LIGHTING_BIT);
	glDisable(GL_LIGHTING);
	glEnable(GL_COLOR_MATERIAL);
	glColor3ub(GetRValue(color), GetGValue(color), GetBValue(color));
	glBegin(GL_QUAD_STRIP);
	glVertex3f( pRect->left,  pRect->top,    Z_down);
	glVertex3f( pRect->left,  pRect->top,    Z_up);
	glVertex3f( pRect->left,  pRect->bottom, Z_down);
	glVertex3f( pRect->left,  pRect->bottom, Z_up); 
	glVertex3f( pRect->right, pRect->bottom, Z_down);
	glVertex3f( pRect->right, pRect->bottom, Z_up); 
	glVertex3f( pRect->right, pRect->top,    Z_down);
	glVertex3f( pRect->right, pRect->top,    Z_up); 
	glVertex3f( pRect->left,  pRect->top,    Z_down);
	glVertex3f( pRect->left,  pRect->top,    Z_up);
	glEnd();

	glBegin(GL_QUADS);
	glVertex3f( pRect->left,  pRect->top,    Z_up);
	glVertex3f( pRect->left,  pRect->bottom, Z_up);
	glVertex3f( pRect->right, pRect->bottom, Z_up);
	glVertex3f( pRect->right, pRect->top,    Z_up);
	glVertex3f( pRect->left,  pRect->top,    Z_down);
	glVertex3f( pRect->left,  pRect->bottom, Z_down);
	glVertex3f( pRect->right, pRect->bottom, Z_down);
	glVertex3f( pRect->right, pRect->top,    Z_down);
	glEnd();
	glPopAttrib();
}
#endif
//------------------------------------------------------------------------------
bool ColorCompare(COLORREF color1, COLORREF color2, double threshold)
{
	double res(0.0);
	res = sqrt(pow(GetRValue(color2) - GetRValue(color1), 2.0) + pow(GetGValue(color2)
		- GetGValue(color1), 2.0) + pow(GetBValue(color2) - GetBValue(color1), 2.0));
	if (res > threshold)
		return true;
	else
		return false;
}
//------------------------------------------------------------------------------
int ChangeColorComponent(int comp, int factor)
{
	int ret = comp + factor;
	if (ret < 0)
		ret = 0;
	if (ret > 255)
		ret = 255;
	return ret;
}
//------------------------------------------------------------------------------
COLORREF BrightnessChange(COLORREF color, int iPercent)
{// 100% - это 255
	int factor = int(2.55f * iPercent);		// 2.55 == 255 / 100
	int iR(GetRValue(color)), iG(GetGValue(color)), iB(GetBValue(color));
	return COLORREF( RGB( ChangeColorComponent(iR, factor),
						  ChangeColorComponent(iG, factor),
						  ChangeColorComponent(iB, factor) ) );
}
//------------------------------------------------------------------------------
COLORREF LighterColor( COLORREF clrColor, int iPercent )
{
	return BrightnessChange(clrColor, iPercent );
}
//------------------------------------------------------------------------------
COLORREF DarkerColor(COLORREF clrColor, int iPercent)
{
	return BrightnessChange(clrColor, -iPercent );
}
//------------------------------------------------------------------------------
