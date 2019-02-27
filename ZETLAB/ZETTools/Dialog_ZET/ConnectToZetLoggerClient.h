// ---------------------------------------------------------------------
// ����������� � ������� ZetLoggerClient.dll
// ---------------------------------------------------------------------
// � ��������� ������� ��� ���� ������������ � �������
// "�����������->�����->�������������� �������� ���������"
// ������ ���� ������ "$(ZetLibsDir)",
// ��� ��������� � ����������� ����������� �������� ZetLab � ZetView.
// ---------------------------------------------------------------------
#pragma once
#include <ZetLoggerClient\include\ZetLoggerClient.h>

#define ZetLoggerClient_NAME		"ZetLoggerClient"
#define ZetLoggerClient_LIB_EXT	".lib"

#ifndef DEBUG
#define ZetLoggerClient_LIB_NAME	ZetLoggerClient_NAME ZetLoggerClient_LIB_EXT
#else
#define ZetLoggerClient_LIB_NAME	ZetLoggerClient_NAME "D" ZetLoggerClient_LIB_EXT
#endif

#ifndef _WIN64
#define ARCHITECTURE_WIN	"Win32"
#else
#define ARCHITECTURE_WIN	"Win64"
#endif

#pragma comment(lib, ZetLoggerClient_NAME "\\bin\\" ARCHITECTURE_WIN "\\" ZetLoggerClient_LIB_NAME)
// ---------------------------------------------------------------------