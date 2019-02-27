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

// ������� ����������
#pragma comment (lib, "OpenGL32.lib")
#pragma comment (lib, "GLU32.lib")
#pragma comment (lib, "freetype_bin\\lib\\freetype.lib")

//Using the STL exception library increases the
//chances that someone else using our code will corretly
//catch any exceptions that we throw.
#include <stdexcept>

#include <list>
#include <map>

// ������ ����������� �������
//		    ����		    ������ �����
typedef std::list<std::pair<DWORD, DWORD>> ListOfFonts;

class FreeTypeFont
{
public:// *****************************************************************************************
	FreeTypeFont();
	~FreeTypeFont();
public:// *****************************************************************************************
	// ������������� �������
	// filename - ��� ����� �� �������
	// h - ������ ������
	// FontLists - ���� ��� �������� ��� ������, first - � ������ ������ �������, second - �� ������
	void InitFont(CString filename, unsigned int h, ListOfFonts &FontLists);
	void InitFont(CString filename, unsigned int h, int NumberListFonts);

	void SetCharSize(unsigned int NewHight);
	// ********************************************************************************************
	void PrintTextGL(GLfloat x, GLfloat y, GLfloat z,		// ����������, (0,0) - ������ ����� ����
			   GLubyte R, GLubyte G, GLubyte B,		// ����� ������
			   CString Text);			// �������������� ������ � �������� ��������

	void PrintTextGL(GLfloat x, GLfloat y,				// ����������, (0,0) - ������ ����� ����
			   GLubyte R, GLubyte G, GLubyte B,		// ����� ������
			   CString Text);			// �������������� ������ � �������� ��������

	CSize GetTextSize(CString Str);
private:
	//Free all the resources assosiated with the font.
	void clean();
public:// *****************************************************************************************
	unsigned int Height;				///< Holds the height of the font.
	GLuint * textures;					///< Holds the texture id's 
	GLuint list_base;					///< Holds the first display list id
	int FontListsNum;					///< �������� ���������� ����������� �������
	ListOfFonts m_FontList;				///< �������� ������ ������ ��� �����������
	std::map<int, CSize> m_CharsSize;	///< �������� ������� ��� ������� �������
	//*********************************************************************************************
	//The object in which Freetype holds information on a given
	//font is called a "face".
	FT_Face face;
	//Create and initilize a freetype font library.
	FT_Library library;
};	//*********************************************************************************************