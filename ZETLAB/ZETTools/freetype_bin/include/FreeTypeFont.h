#pragma once

//FreeType Headers
#define CALCDIRFILE( FILE ) <freetype_bin/include/freetype2/freetype/##FILE>

#include CALCDIRFILE(config/ftheader.h)
#include CALCDIRFILE(freetype.h)
#include CALCDIRFILE(ftglyph.h)
#include CALCDIRFILE(ftoutln.h)
#include CALCDIRFILE(fttrigon.h)

//OpenGL Headers 
//#include <windows.h>		//(the GL headers need it)
#include <GL/gl.h>
#include <GL/glu.h>

// линкуем библиотеки
#pragma comment (lib, "OpenGL32.lib")
#pragma comment (lib, "GLU32.lib")
#pragma comment (lib, "freetype_bin\\lib\\freetype.lib")

//Using the STL exception library increases the
//chances that someone else using our code will corretly
//catch any exceptions that we throw.
#include <stdexcept>

#include <list>
#include <map>

// список загружаемых шрифтов
//		    ЛИСТ		    НАЧАЛО КОНЕЦ
typedef std::list<std::pair<DWORD, DWORD>> ListOfFonts;

class FreeTypeFont
{
public:// *****************************************************************************************
	FreeTypeFont();
	~FreeTypeFont();
public:// *****************************************************************************************
	// Инициализация шрифтов
	// filename - имя файла со шрифтом
	// h - высота шрифта
	// FontLists - лист пар индексов для шрифта, first - с какого номера грузить, second - до какого
	void InitFont(CString filename, unsigned int h, ListOfFonts &FontLists);
	void InitFont(CString filename, unsigned int h, int NumberListFonts);

	void SetCharSize(unsigned int NewHight);
	// ********************************************************************************************
	void PrintTextGL(GLfloat x, GLfloat y, GLfloat z,		// координаты, (0,0) - нижний левый угол
			   GLubyte R, GLubyte G, GLubyte B,		// цвета текста
			   CString Text);			// отображающаяся строка с заданным форматом

	void PrintTextGL(GLfloat x, GLfloat y,				// координаты, (0,0) - нижний левый угол
			   GLubyte R, GLubyte G, GLubyte B,		// цвета текста
			   CString Text);			// отображающаяся строка с заданным форматом

	CSize GetTextSize(CString Str);
private:
	//Free all the resources assosiated with the font.
	void clean();
public:// *****************************************************************************************
	unsigned int Height;				///< Holds the height of the font.
	GLuint * textures;					///< Holds the texture id's 
	GLuint list_base;					///< Holds the first display list id
	int FontListsNum;					///< содержит количество загружаемых текстур
	ListOfFonts m_FontList;				///< содержит список листов для отображения
	std::map<int, CSize> m_CharsSize;	///< содержит размеры для каждого символа
	//*********************************************************************************************
	//The object in which Freetype holds information on a given
	//font is called a "face".
	FT_Face face;
	//Create and initilize a freetype font library.
	FT_Library library;
};	//*********************************************************************************************