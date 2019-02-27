#pragma once

#include <GdiPlus.h>
using namespace Gdiplus;
#pragma comment(lib,"GdiPlus.lib")

#include <boost/shared_ptr.hpp>
#include <boost/make_shared.hpp>

enum rotatetype
{
	RT_PLUS,
	RT_MINUS
};


enum VolumeStyle
{
	VS_FLAT,	//плоский
	VS_SURROUND	//объемный
};


class CircleObject
{
public:

	CircleObject::CircleObject()
		: m_x(0)
		, m_y(0)
		, m_diametr(201)
		, m_drawBorder(true)
	{
		m_bitmapSize = m_diametr + 2;
		m_borderColor = Color(255, 0, 51, 102);	
		m_backColor = Color(255, 185, 185, 185);	
	}

	CircleObject::CircleObject(int X, int Y, int diam)
		: m_x(X)
		, m_y(Y)
		, m_diametr(diam)
		, m_drawBorder(true)
	{
		m_bitmapSize = m_diametr + 2;
		m_borderColor = Color(255, 0, 51, 102);	
		m_backColor = Color(255, 185, 185, 185);	
	}

	virtual CircleObject::~CircleObject()
	{
		DeleteObject(m_bitmap.get());
		m_bitmap.reset();
	}

	void SetX(int X)
	{
		CheckAndSet(X, m_x);
	}

	void SetY(int Y)
	{
		CheckAndSet(Y, m_y);
	}

	void SetDiam(int diam)
	{
		CheckAndSet_Reset(diam, m_diametr);
		
		if(m_bitmapSize != m_diametr + 2)
			m_bitmapSize = m_diametr + 2;
	}

	void SetCircle(int X, int Y, int diam)
	{
		CheckAndSet(X, m_x);
		CheckAndSet(Y, m_y);
		CheckAndSet_Reset(diam, m_diametr);
	}

	void SetBorderColor(Color borderColor)
	{
		CheckAndSet_Reset(borderColor, m_borderColor);
	}

	void SetBackColor(Color backColor)
	{
		CheckAndSet_Reset(backColor, m_backColor);
	}

	void SetBorderDraw(bool borderDraw)
	{
		CheckAndSet_Reset(borderDraw, m_drawBorder);
	}





	int GetX()
	{
		return m_x;
	}

	int GetY()
	{
		return m_y;
	}

	int GetDiam()
	{
		return m_diametr;
	}

	Color GetBorderColor()
	{
		return m_borderColor;
	}

	bool GetBorderDraw()
	{
		return m_drawBorder;
	}

	Color GetBackColor()
	{
		return m_backColor;
	}

	Bitmap* GetBitmap()
	{
		if(m_bitmap)
			return m_bitmap.get();

		return NULL;
	}

	int GetBitmapSize()
	{
		return m_bitmapSize;
	}

	virtual void DrawObject(Graphics &graph)
	{}

protected:
	int m_x;				//положение по оси Х
	int m_y;				//положение по оси У
	int m_diametr;			//диаметр
	Color m_borderColor;	//цвет рамки
	Color m_backColor;		//цвет задней окружности
	

	bool m_drawBorder;		// если true, то рисуем рамку

	typedef boost::shared_ptr<Bitmap> Bitmap_ptr;
	Bitmap_ptr m_bitmap;							// картинка, в которой хранится изображение
	int m_bitmapSize;

	template <typename T>
	void CheckAndSet(T &setVal, T &currVal)
	{
		if(setVal != currVal)
			currVal = setVal;
	}

	template <typename T>
	void CheckAndSet_Reset(T &setVal, T &currVal)
	{
		if(setVal != currVal)
		{
			currVal = setVal;
			m_bitmap.reset();
		}
	}

	template <>
	void CheckAndSet_Reset <Color> (Color &setVal, Color &currVal)
	{
		if(setVal.GetValue() != currVal.GetValue())
		{
			currVal = setVal;
			m_bitmap.reset();
		}
	}

	void RotateGr(Graphics &gr, int &diametr, float angle, rotatetype rt)
	{
		if(rt == RT_MINUS) angle *= -1.0;
		gr.TranslateTransform((REAL)(diametr / 2), (REAL)(diametr / 2));
		gr.RotateTransform((REAL)angle);
		gr.TranslateTransform((REAL)(-diametr / 2), (REAL)(-diametr / 2));
	}
};