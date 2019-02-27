// ---------------------------------------------------------------------
// ����������� � ������� ZetPath.dll
// ---------------------------------------------------------------------
// � ��������� ������� ��� ���� ������������ � �������
// "�����������->�����->�������������� �������� ���������"
// ������ ���� ������ "$(ZetLibsDir)",
// ��� ��������� � ����������� ����������� �������� ZetLab � ZetView.
// ---------------------------------------------------------------------
#pragma once
#include <ZetPath/include/ZetPath.h>

#define ZetPath_NAME		"ZetPath"
#define ZetPath_LIB_EXT	".lib"

#ifndef DEBUG
#define ZetPath_LIB_NAME	ZetPath_NAME ZetPath_LIB_EXT
#else
#define ZetPath_LIB_NAME	ZetPath_NAME "D" ZetPath_LIB_EXT
#endif

#ifndef _WIN64
#define ARCHITECTURE_WIN	"Win32"
#else
#define ARCHITECTURE_WIN	"Win64"
#endif

#pragma comment(lib, ZetPath_NAME "\\bin\\" ARCHITECTURE_WIN "\\" ZetPath_LIB_NAME)
// ---------------------------------------------------------------------