#include "stdafx.h"
#include "FreeTypeFont.h"
#include <vector>

FreeTypeFont::FreeTypeFont()
	: textures	(nullptr)
	, Height	(0)
	, FontListsNum(0)
	, list_base	(0)
	, face		(nullptr)
	, library	(nullptr)
{

}

FreeTypeFont::~FreeTypeFont()
{
	//We don't need the face information now that the display
	//lists have been created, so we free the assosiated resources.
	if (face)
		FT_Done_Face(face);

	//Ditto for the library.
	if (library)
		FT_Done_FreeType(library);

	clean();
}

///This function gets the first power of 2 >= the
///int that we pass it.
inline int next_p2(int a)
{
	int rval = 1;
	while (rval < a) rval <<= 1;
	return rval;
}

///Create a display list coresponding to the give character.
inline CSize make_dlist(FT_Face face, int ch, GLuint list_base, GLuint * tex_base, int TextureIndex) 
{
	//The first thing we do is get FreeType to render our character
	//into a bitmap.  This actually requires a couple of FreeType commands:

	//Load the Glyph for our character.
	if (FT_Load_Glyph(face, FT_Get_Char_Index(face, ch), FT_LOAD_DEFAULT))
		throw std::runtime_error("FT_Load_Glyph failed");

	//Move the face's glyph into a Glyph object.
	FT_Glyph glyph;
	if (FT_Get_Glyph(face->glyph, &glyph))
		throw std::runtime_error("FT_Get_Glyph failed");

	//Convert the glyph to a bitmap.
	FT_Glyph_To_Bitmap(&glyph, ft_render_mode_normal, 0, 1);
	FT_BitmapGlyph bitmap_glyph = (FT_BitmapGlyph)glyph;

	//This reference will make accessing the bitmap easier
	FT_Bitmap& bitmap = bitmap_glyph->bitmap;

	CSize CharSize(face->glyph->advance.x >> 6, bitmap.rows);

	//Use our helper function to get the widths of
	//the bitmap data that we will need in order to create
	//our texture.
	int width = next_p2(bitmap.width);
	int height = next_p2(bitmap.rows);

	//Allocate memory for the texture data.
	GLubyte* expanded_data = new GLubyte[2 * width * height];

	//Here we fill in the data for the expanded bitmap.
	//Notice that we are using two channel bitmap (one for
	//luminocity and one for alpha), but we assign
	//both luminocity and alpha to the value that we
	//find in the FreeType bitmap. 
	//We use the ?: operator so that value which we use
	//will be 0 if we are in the padding zone, and whatever
	//is the the Freetype bitmap otherwise.
	for (int j = 0; j < height; j++) {
		for (int i = 0; i < width; i++){
			expanded_data[2 * (i + j*width)] = expanded_data[2 * (i + j*width) + 1] =
				(i >= bitmap.width || j >= bitmap.rows) ?
				0 : bitmap.buffer[i + bitmap.width*j];
		}
	}
	
	//Now we just setup some texture paramaters.
	glBindTexture(GL_TEXTURE_2D, tex_base[TextureIndex]);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	//Here we actually create the texture itself, notice
	//that we are using GL_LUMINANCE_ALPHA to indicate that
	//we are using 2 channel data.
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height,
					0, GL_LUMINANCE_ALPHA, GL_UNSIGNED_BYTE, expanded_data);

	//With the texture created, we don't need to expanded data anymore
	delete[] expanded_data;

	//So now we can create the display list
	glNewList(list_base + ch, GL_COMPILE);

	glBindTexture(GL_TEXTURE_2D, tex_base[TextureIndex]);

	glPushMatrix();

	//first we need to move over a little so that
	//the character has the right amount of space
	//between it and the one before it.
	glTranslatef(GLfloat(bitmap_glyph->left), 0, 0);

	//Now we move down a little in the case that the
	//bitmap extends past the bottom of the line 
	//(this is only true for characters like 'g' or 'y'.
	glTranslatef(0, GLfloat(bitmap_glyph->top - bitmap.rows), 0);

	//Now we need to account for the fact that many of
	//our textures are filled with empty padding space.
	//We figure what portion of the texture is used by 
	//the actual character and store that information in 
	//the x and y variables, then when we draw the
	//quad, we will only reference the parts of the texture
	//that we contain the character itself.
	float	x = (float)bitmap.width / (float)width,
		y = (float)bitmap.rows / (float)height;

	//Here we draw the texturemaped quads.
	//The bitmap that we got from FreeType was not 
	//oriented quite like we would like it to be,
	//so we need to link the texture to the quad
	//so that the result will be properly aligned.
	glBegin(GL_QUADS);
	glTexCoord2d(0, 0); glVertex2f(0, GLfloat(bitmap.rows));
	glTexCoord2d(0, y); glVertex2f(0, 0);
	glTexCoord2d(x, y); glVertex2f(GLfloat(bitmap.width), 0);
	glTexCoord2d(x, 0); glVertex2f(GLfloat(bitmap.width), GLfloat(bitmap.rows));
	glEnd();
	glPopMatrix();
	glTranslatef(GLfloat(face->glyph->advance.x >> 6), 0, 0);


	//increment the raster position as if we were a bitmap font.
	//(only needed if you want to calculate text length)
	//glBitmap(0,0,0,0,face->glyph->advance.x >> 6,0,NULL);

	//Finnish the display list
	glEndList();

	return CharSize;
}

void FreeTypeFont::InitFont(CString filename, unsigned int h, int NumberListFonts)
{
	ListOfFonts FontList;
	FontList.push_back(std::make_pair(0, NumberListFonts));
	InitFont(filename, h, FontList);
}

void FreeTypeFont::InitFont(CString filename, unsigned int h, ListOfFonts &FontLists)
{
	CStringA charstr(filename);

	clean();
	
	int NumberListFonts(0);
	for (auto &it : FontLists)
	{
		NumberListFonts += it.second - it.first;
	}
	m_FontList = FontLists;
	FontListsNum = NumberListFonts;

	//Allocate some memory to store the texture ids.
	textures = new GLuint[FontListsNum];

	if (FT_Init_FreeType(&library))
		throw std::runtime_error("FT_Init_FreeType failed");

	//This is where we load in the font information from the file.
	//Of all the places where the code might die, this is the most likely,
	//as FT_New_Face will die if the font file does not exist or is somehow broken.
	if (FT_New_Face(library, charstr, 0, &face))
		throw std::runtime_error("FT_New_Face failed (there is probably a problem with your font file)");

	//Here we ask opengl to allocate resources for
	//all the textures and displays lists which we
	//are about to create.  
	list_base = glGenLists(FontListsNum);
	glGenTextures(FontListsNum, textures);

	SetCharSize(h);
}

void FreeTypeFont::SetCharSize(unsigned int NewHight)
{
	// ����� �������������� ������� ��������� ��� glGetError ���������� 0
	// ����� ������� ����� �� ���������
	//if (Height != NewHight)	// ������ ������ ��� ������ ����� ������������� �����
	{
		Height = NewHight;

		//For some twisted reason, Freetype measures font size
		//in terms of 1/64ths of pixels.  Thus, to make a font
		//h pixels high, we need to request a size of h*64.
		//(h << 6 is just a prettier way of writting h*64)
		FT_Set_Char_Size(face, NewHight << 6, NewHight << 6, 96, 96);

		//This is where we actually create each of the fonts display lists.
		int TextureIndex = 0;
		m_CharsSize.clear();
		for (auto &it : m_FontList)
		{
			for (DWORD i = it.first; i < it.second; i++)
			{
				m_CharsSize[i] = make_dlist(face, i, list_base, textures, TextureIndex);

				//���� ����������� ��� ����������� ������ ����������� ������ �� ������ ������� �����
				if ((i >= 0x2070 && i <= 0x207F) || 
					(i >= 0x2080 && i <= 0x209F))
				{
					auto &it = m_CharsSize.find(L'A');
					if (it != m_CharsSize.end())
						m_CharsSize[i].cy = it->second.cy + m_CharsSize[i].cy / 2;
				}
				TextureIndex++;
			}
		}
	}
}

void FreeTypeFont::clean()
{
	if (FontListsNum > 0)
	{
		glDeleteLists(list_base, FontListsNum);
		glDeleteTextures(FontListsNum, textures);		
	}

	if (textures)
	{ 
		delete[] textures;
		textures = nullptr;
	}

	FontListsNum = 0;
}

/// A fairly straight forward function that pushes
/// a projection matrix that will make object world 
/// coordinates identical to window coordinates.
inline void pushScreenCoordinateMatrix() {
	glPushAttrib(GL_TRANSFORM_BIT);
	GLint	viewport[4];
	glGetIntegerv(GL_VIEWPORT, viewport);
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
	gluOrtho2D(viewport[0], viewport[2], viewport[1], viewport[3]);
	glPopAttrib();
}

/// Pops the projection matrix without changing the current
/// MatrixMode.
inline void pop_projection_matrix() {
	glPushAttrib(GL_TRANSFORM_BIT);
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	glPopAttrib();
}

// ��������� �� ���������
void GetLinesFromsString(_In_ CString String, _Out_ std::list<CString> &LineStrings)
{
	LineStrings.clear();

	if (String.Find(L'\n') == -1)
		LineStrings.push_back(String);
	else
	{
		int Index(0);
		CString strSub;
		while (AfxExtractSubString(strSub, String, Index++, '\n') == TRUE)
			LineStrings.push_back(strSub);
	}
}

void FreeTypeFont::PrintTextGL(GLfloat x, GLfloat y, GLfloat z,		/* ����������, (0,0) - ������ ����� ���� */ 
						 GLubyte R, GLubyte G, GLubyte B,		/* ����� ������ */ 
						 CString Text)				/* �������������� ������ � �������� �������� */
{
	if (Text.IsEmpty())									// If There's No Text
		return;

	// We want a coordinate system where things coresponding to window pixels.
	pushScreenCoordinateMatrix();

	float h = Height / .63f;							//We make the height about 1.5* that of

	std::list<CString> Lines;
	GetLinesFromsString(Text, Lines);
	
	glPushAttrib(GL_LIST_BIT | GL_CURRENT_BIT | GL_ENABLE_BIT | GL_TRANSFORM_BIT);
	glMatrixMode(GL_MODELVIEW);
	glDisable(GL_LIGHTING);
	glEnable(GL_TEXTURE_2D);
	glDisable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glListBase(list_base);

	float modelview_matrix[16];
	glGetFloatv(GL_MODELVIEW_MATRIX, modelview_matrix);

	glColor3ub(R, G, B);

	//This is where the text display actually happens.
	//For each line of text we reset the modelview matrix
	//so that the line's text will start in the correct position.
	//Notice that we need to reset the matrix, rather than just translating
	//down by h. This is because when each character is
	//draw it modifies the current matrix so that the next character
	//will be drawn immediatly after it.  
	UINT Index(0);
	GLfloat StartPos(y + h * (Lines.size() - 1));
	for (auto &Line : Lines)
	{
		glPushMatrix();
		glLoadIdentity();
		//glRasterPos3f(x, y, 0);
		glTranslatef(x, StartPos - h * Index, z);

		glMultMatrixf(modelview_matrix);

		//  The commented out raster position stuff can be useful if you need to
		//  know the length of the text that you are creating.
		//  If you decide to use it make sure to also uncomment the glBitmap command
		//  in make_dlist().
		//	glRasterPos2f(0,0);
		
		glCallLists(Line.GetLength(), GL_UNSIGNED_SHORT, Line);
			
		glPopMatrix();
		Index++;
	}

	glPopAttrib();

	pop_projection_matrix();
}

void FreeTypeFont::PrintTextGL(GLfloat x, GLfloat y,				/* ����������, (0,0) - ������ ����� ���� */ 
						 GLubyte R, GLubyte G, GLubyte B,	/* ����� ������ */ 
						 CString Text)			/* �������������� ������ � �������� �������� */
{
	PrintTextGL(x, y, 0, R, G, B, Text);
}

CSize FreeTypeFont::GetTextSize(CString Str)
{
	if (!face)
		return CSize(NULL, NULL);

	std::list<CString> Lines;
	GetLinesFromsString(Str, Lines);

	FT_Glyph glyph;
	FT_BitmapGlyph bitmap_glyph;
	CSize TextSize(NULL, NULL);
	CSize LineTextSize(NULL, NULL);
	CSize CharSize(NULL, NULL);
	for (auto LineStr = Lines.begin(), End = Lines.end(); LineStr != End; ++LineStr)
	{
		LineTextSize.SetSize(NULL, NULL);
		// ������������ ������ ������� ������� ��������
		for (int n = 0, StrLength = LineStr->GetLength(); n < StrLength; n++)
		{
			auto &it = m_CharsSize.find((*LineStr)[n]);
			if (it != m_CharsSize.end())
				// ���� ���� ������ ��� ��������, �� ���������� ��� ����������� ������
				CharSize = it->second;
			else
			{
				//Load the Glyph for our character.
				if (FT_Load_Glyph(face, FT_Get_Char_Index(face, (*LineStr)[n]), FT_LOAD_DEFAULT))
					throw std::runtime_error("FT_Load_Glyph failed");

				if (FT_Get_Glyph(face->glyph, &glyph))
					throw std::runtime_error("FT_Get_Glyph failed");

				//Convert the glyph to a bitmap.
				FT_Glyph_To_Bitmap(&glyph, ft_render_mode_normal, 0, 1);
				bitmap_glyph = (FT_BitmapGlyph)glyph;

				CharSize.SetSize(face->glyph->advance.x >> 6, bitmap_glyph->bitmap.rows);
			}
			LineTextSize.cx += CharSize.cx;
			if (LineTextSize.cy < CharSize.cy)
				LineTextSize.cy = CharSize.cy;
		}

		if (TextSize.cx < LineTextSize.cx)
			TextSize.cx = LineTextSize.cx;
		TextSize.cy += LineTextSize.cy;

		if (Lines.size() > 1)
		{
			// ���� ��������� ������ �������������� ������
			if (LineStr == --Lines.end())
				// ���� ��� ��������� ������� �� ��������� �������� �������
				TextSize.cy += LONG((GLfloat(Height) / .63f - LineTextSize.cy) / 2);
			else if (LineStr != Lines.begin() && (LineStr != --Lines.end()))
				// ���� ��� ������������� ������� �� ��������� �� ������
				TextSize.cy += LONG(GLfloat(Height)  / .63f - LineTextSize.cy);
		}
	}

	return TextSize;
}
