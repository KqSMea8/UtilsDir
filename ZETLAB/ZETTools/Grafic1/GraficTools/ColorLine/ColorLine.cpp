#include "stdafx.h"
#include "ColorLine.h"

CColorLine::CColorLine()

{
}

CColorLine::~CColorLine()
{
}

void CColorLine::DrowColorLine(_In_ CDC* pdc, _In_ struct ColorLineStruct *Struct)
{
	// отображение текста 
	int TopCoord = Struct->FullRect.top;
	if (!Struct->Caption.IsEmpty())
	{
		CSize stroka = pdc->GetTextExtent(Struct->Caption);

		TopCoord += stroka.cy - 3;

		glColor3ub(GetRValue(Struct->TextColor), GetGValue(Struct->TextColor), GetBValue(Struct->TextColor));
		glRasterPos2f((GLfloat)Struct->PixBegin, (GLfloat)(TopCoord - 1));
		
		glPushAttrib(GL_LIST_BIT);							// Pushes The Display List Bits
		glListBase(Struct->FontList);						// Sets The Base Character to 32

#ifndef _UNICODE
		glCallLists((GLsizei)_tcslen(Struct->Caption), GL_UNSIGNED_BYTE, Struct->Caption);	// Draws The Display List Text
#else
		glCallLists((GLsizei)_tcslen(Struct->Caption), GL_UNSIGNED_SHORT, Struct->Caption);	// Draws The Display List Text
#endif
		glPopAttrib();										// Pops The Display List Bits
	}
	
	glColor3ub(GetRValue(Struct->FontColor), GetGValue(Struct->FontColor), GetBValue(Struct->FontColor));
	glBegin(GL_QUADS);
	{
		glVertex3f((GLfloat)Struct->PixBegin, (GLfloat)TopCoord, 0.f);
		glVertex3f((GLfloat)Struct->PixEnd,   (GLfloat)TopCoord, 0.f);
		glVertex3f((GLfloat)Struct->PixEnd,   (GLfloat)Struct->FullRect.bottom, 0.f);
		glVertex3f((GLfloat)Struct->PixBegin, (GLfloat)Struct->FullRect.bottom, 0.f);
	}
	glEnd();

	for (auto &it : Struct->SpanDrowVector)
	{
		// левая граница интервала вышла за пределы области видимости
		if (it.PixStart < Struct->PixBegin)
		{
			if (it.PixEnd < Struct->PixBegin)	// правая граница тоже вышла значит не отображаем этот интервал
				continue;
			else
				it.PixStart = Struct->PixBegin;		// правая граница не вышла за пределы значит левую приравниваем началу видимой области

			if (it.PixEnd > Struct->PixEnd)
				it.PixEnd = Struct->PixEnd;
		}
		else if (it.PixStart > Struct->PixEnd)
		{
			// левая граница вышла за пределы видимой области ничего не рисуем
			continue;
		}
		else
		{
			// левая граница внутри видимой области проверяем правую границу
			if (it.PixEnd > Struct->PixEnd)
				it.PixEnd = Struct->PixEnd;
			else if (it.PixEnd < it.PixStart)
				continue;
		}

		glColor3ub(GetRValue(it.Color), GetGValue(it.Color), GetBValue(it.Color));
		glBegin(GL_QUADS);
		{
			if (it.PixEnd - it.PixStart < 1)
				it.PixEnd = it.PixStart + 1;

			glVertex3f((GLfloat)it.PixStart, (GLfloat)TopCoord, 1.f);
			glVertex3f((GLfloat)it.PixEnd,   (GLfloat)TopCoord, 1.f);
			glVertex3f((GLfloat)it.PixEnd,   (GLfloat)Struct->FullRect.bottom, 1.f);
			glVertex3f((GLfloat)it.PixStart, (GLfloat)Struct->FullRect.bottom, 1.f);
		}
		glEnd();
	}

	// рисуем рамку вокруг рабочей области контрола
	glLineWidth(1);
	glColor3ub(GetRValue(0/*GetSysColor(COLOR_3DSHADOW)*/), GetGValue(0/*GetSysColor(COLOR_3DSHADOW)*/), GetBValue(0/*GetSysColor(COLOR_3DSHADOW)*/));
	glBegin(GL_LINE_LOOP);
	{
		glVertex3f((GLfloat)Struct->PixBegin, (GLfloat)TopCoord, 1.f);
		glVertex3f((GLfloat)Struct->PixEnd,   (GLfloat)TopCoord, 1.f);
		glVertex3f((GLfloat)Struct->PixEnd,   (GLfloat)Struct->FullRect.bottom, 1.f);
		glVertex3f((GLfloat)Struct->PixBegin, (GLfloat)Struct->FullRect.bottom, 1.f);
	}
	glEnd();
}