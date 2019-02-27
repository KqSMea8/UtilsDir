// ---------------------------------------------------------------------
// ����������� � ������� ZetViewResult.dll
// ---------------------------------------------------------------------
// � ��������� ������� ��� ���� ������������ � �������
// "�����������->�����->�������������� �������� ���������"
// ������ ���� ������ "$(ZetLibsDir)",
// ��� ��������� � ����������� ����������� �������� ZetLab � ZetView.
// ---------------------------------------------------------------------
#pragma once
#include <ZetViewResult/include/ZetViewResult.h>

#define ZetViewResult_NAME		"ZetViewResult"
#define ZetViewResult_LIB_EXT	".lib"

#ifndef DEBUG
#define ZetViewResult_LIB_NAME	ZetViewResult_NAME ZetViewResult_LIB_EXT
#else
#define ZetViewResult_LIB_NAME	ZetViewResult_NAME "D" ZetViewResult_LIB_EXT
#endif

#ifndef _WIN64
#define ARCHITECTURE_WIN	"Win32"
#else
#define ARCHITECTURE_WIN	"Win64"
#endif

#pragma comment(lib, ZetViewResult_NAME "\\bin\\" ARCHITECTURE_WIN "\\" ZetViewResult_LIB_NAME)
// ---------------------------------------------------------------------