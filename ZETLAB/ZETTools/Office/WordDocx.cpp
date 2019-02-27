#include "stdafx.h"
#include "WordDocx.h"

CWordDocx::CWordDocx()
{

}

CWordDocx::~CWordDocx()
{

}

int CWordDocx::FindDocument()
{
	char doc[18] = "word/document.xml";

	std::list<LocalFile>::iterator itFile;
	std::list<CentralDir>::iterator itDir;

	for (itFile = m_lFiles.begin(), itDir = m_lDirs.begin(); itFile != m_lFiles.end(), itDir != m_lDirs.end(); ++itFile, ++itDir)
	{
		int ret = strcmp(itFile->pFileName, doc);
		if ( ret == 0 )
		{
			m_iterFile = itFile;
			m_iterDir = itDir;
			return 0;
		}
	}

	return -1;
}

int CWordDocx::ReplacePictureFromFile(LPCSTR fileName)
{
	if (strlen((char*)fileName) == 0)
		return -1;

	// ищем имя файла изображения в файле "word/_rels/document.xml.rels"
	char drawing[29] = "word/_rels/document.xml.rels";
	FindFile(drawing);

	char* cpos = (char*)m_iterFile->pUncmData;
	char* cend = cpos;
	char* ctemp = nullptr;
	char* image1 = nullptr;
	int sizei1 = 0;
	while (cpos != NULL)
	{
		cpos = strstr(cpos, "Type=");
		cpos += 6;
		cend = strstr(cpos, "\"");
		ctemp = strstr(cpos, "relationships/image");
		if (ctemp != NULL && ctemp > cpos && ctemp < cend)
			break;
	}
	if (cpos == NULL)
		return -2;
	//cpos = strstr((char*)m_iterFile->pUncmData, "Target=");
	cpos = strstr(cpos, "Target=");
	cend = strstr(cpos, "\"/>");
	sizei1 = cend - cpos - 8 + 5;
	if (sizei1 < 1)
		return -3;
	image1 = new char[sizei1 + 1];
	strncpy_s(image1, sizei1 + 1, cpos + 8 - 5, sizei1);
	image1[0] = 'w';
	image1[1] = 'o';
	image1[2] = 'r';
	image1[3] = 'd';
	image1[4] = '/';

	// ищем имя файла изображения и его типа в файле "\[Content_Types].xml" 
	char content[20] = "[Content_Types].xml";
	FindFile(content);

	char *cimg = NULL;
	char *ccnt = NULL;
	char *cend2 = NULL;
	char *image2 = NULL;
	int sizei2 = 0;
	cimg = strstr((char*)m_iterFile->pUncmData, image1);
	if (cimg != NULL)
	{
		ccnt = strstr(cimg, "ContentType=") - 1;
		cend2 = strstr(ccnt, "\"/");
		sizei2 = cend2 - ccnt - 14;
		image2 = new char[sizei2 + 1];
		strncpy_s(image2, sizei2 + 1, ccnt + 14, sizei2);
	}

	// задаём новые строки для файлов
	char *cext = NULL;
	char *cext1 = NULL;
	char *cext2 = NULL;
	char *reimage1 = NULL;
	char *reimage2 = NULL;
	int extsize = 0;
	int resize1 = 0;
	int resize2 = 0;
	cext = strstr((char*)fileName, ".") + 1;
	extsize = strlen(cext);
	cext1 = strstr(image1, ".") + 1;
	resize1 = cext1 - image1 + strlen(cext) + 1;
	reimage1 = new char[resize1];
	strncpy_s(reimage1, resize1, image1, cext1 - image1);
	strncpy_s(&reimage1[cext1 - image1], strlen(cext) + 1, cext, strlen(cext));
	if (image2 != NULL)
	{
		cext2 = strstr(image2, "/") + 1;
		resize2 = cext2 - image2 + strlen(cext) + 1;
		reimage2 = new char[resize2];
		strncpy_s(reimage2, resize2, image2, cext2 - image2);
		strncpy_s(&reimage2[cext2 - image2], strlen(cext) + 1, cext, strlen(cext));
	}

	// замена файла "\xl\media\image2.emf", конкретно имя можно узнать из других документов
	FindFile(image1);

	// читаем файл изображения в битовый буффер
	FILE* fid(nullptr);
	errno_t err = fopen_s(&fid, fileName, "rb");
	if (err != 0)
		return -4;
	fseek(fid, 0, SEEK_END);
	unsigned int filesize = ftell(fid);
	fseek(fid, 0, SEEK_SET);
	Byte *pBuffer = new Byte[filesize];
	unsigned int readsize = fread(pBuffer, 1, filesize, fid);
	fclose(fid);
	fid = NULL;

	// заменяем данные в файле изображения, изображения не сжимаются
	delete[] m_iterFile->pUncmData;
	m_iterFile->pUncmData = nullptr;
	m_iterFile->pUncmData = pBuffer;
	if (m_iterFile->zdesc.uncompressed_size > 0)
		m_iterFile->zdesc.uncompressed_size = filesize;
	if (m_iterFile->zhead.uncompressed_size > 0)
		m_iterFile->zhead.uncompressed_size = filesize;
	m_iterDir->cdHead.uncompressed_size = filesize;

	delete[] m_iterFile->pFileName;
	m_iterFile->pFileName = nullptr;
	m_iterFile->pFileName = new char[resize1];
	strncpy_s(m_iterFile->pFileName, resize1, reimage1, resize1 - 1);
	m_iterFile->zhead.file_name_length = resize1 - 1;

	delete[] m_iterDir->pFileName;
	m_iterDir->pFileName = nullptr;
	m_iterDir->pFileName = new char[resize1];
	strncpy_s(m_iterDir->pFileName, resize1, reimage1, resize1 - 1);
	m_iterDir->cdHead.file_name_length = resize1 - 1;

	// замена ссылки на файл изображения в файле "word/_rels/document.xml.rels"
	FindFile(drawing);
	ReplaceString(image1 + 2, reimage1 + 2);

	// замена ссылки и типа файла изображения в файле "\[Content_Types].xml" 
	FindFile(content);
	int repos = 0;
	repos = ReplaceString(image1, reimage1);
	if (image2 != NULL)
		repos = ReplaceString(image2, reimage2);

	return 0;
}
