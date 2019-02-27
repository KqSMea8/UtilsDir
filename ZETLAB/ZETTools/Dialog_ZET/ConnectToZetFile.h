// ---------------------------------------------------------------------
// ����������� � ������� ZetFile.dll
// ---------------------------------------------------------------------
// � ��������� ������� ��� ���� ������������ � �������
// "�����������->�����->�������������� �������� ���������"
// ������ ���� ������ "$(ZetLibsDir)",
// ��� ��������� � ����������� ����������� �������� ZetLab � ZetView.
// ---------------------------------------------------------------------
#pragma once
#include <ZetFile/include/ZetFile.h>

#define ZetFile_NAME		"ZetFile"
#define ZetFile_LIB_EXT		".lib"

#ifndef DEBUG
#define ZetFile_LIB_NAME	ZetFile_NAME ZetFile_LIB_EXT
#else
#define ZetFile_LIB_NAME	ZetFile_NAME "D" ZetFile_LIB_EXT
#endif

#ifndef _WIN64
#define ARCHITECTURE_WIN	"Win32"
#else
#define ARCHITECTURE_WIN	"Win64"
#endif

#pragma comment(lib, ZetFile_NAME "\\bin\\" ARCHITECTURE_WIN "\\" ZetFile_LIB_NAME)
// ---------------------------------------------------------------------