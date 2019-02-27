// ---------------------------------------------------------------------
// ����������� � ������� WriteData.dll
// ---------------------------------------------------------------------
// � ��������� ������� ��� ���� ������������ � �������
// "�����������->�����->�������������� �������� ���������"
// ������ ���� ������ "$(ZetLibsDir)",
// ��� ��������� � ����������� ����������� �������� ZetLab � ZetView.
// ---------------------------------------------------------------------
#pragma once
#include <WriteData/include/WriteData.h>

#define WRITEDATA_NAME		"WriteData"
#define WRITEDATA_LIB_EXT	".lib"

#ifndef DEBUG
#define WRITEDATA_LIB_NAME	WRITEDATA_NAME WRITEDATA_LIB_EXT
#else
#define WRITEDATA_LIB_NAME	WRITEDATA_NAME "D" WRITEDATA_LIB_EXT
#endif

#ifndef _WIN64
#define ARCHITECTURE_WIN	"Win32"
#else
#define ARCHITECTURE_WIN	"Win64"
#endif

#pragma comment(lib, WRITEDATA_NAME "\\bin\\" ARCHITECTURE_WIN "\\" WRITEDATA_LIB_NAME)
// ---------------------------------------------------------------------