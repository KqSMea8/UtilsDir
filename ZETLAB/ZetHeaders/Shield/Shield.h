#ifndef SHIELD_LIB_INCLUDED
#define SHIELD_LIB_INCLUDED
#pragma once

/*
  ���� ���������.
*/
#define SHIELD_DEVICETYPE_KD1610	(0x1UL << 0)
#define SHIELD_DEVICETYPE_KD216		(0x1UL << 1)
#define SHIELD_DEVICETYPE_KD500		(0x1UL << 2)
#define SHIELD_DEVICETYPE_KD500P	(0x1UL << 3)
#define SHIELD_DEVICETYPE_KD816		(0x1UL << 4)
#define SHIELD_DEVICETYPE_KD1002	(0x1UL << 5)
#define SHIELD_DEVICETYPE_KDU216	(0x1UL << 6)
#define SHIELD_DEVICETYPE_KD24		(0x1UL << 7)
#define SHIELD_DEVICETYPE_KD1432	(0x1UL << 8)
#define SHIELD_DEVICETYPE_KDUACPB	(0x1UL << 9)
#define SHIELD_DEVICETYPE_KDU1616	(0x1UL << 10)
#define SHIELD_DEVICETYPE_KDUPD14	(0x1UL << 11)
#define SHIELD_DEVICETYPE_KDUVN		(0x1UL << 12)
#define SHIELD_DEVICETYPE_KDUOSC	(0x1UL << 13)
#define SHIELD_DEVICETYPE_KDU8500	(0x1UL << 14)
#define SHIELD_DEVICETYPE_KDU2500	(0x1UL << 15)
#define SHIELD_DEVICETYPE_KDU1624	(0x1UL << 16)
#define SHIELD_DEVICETYPE_KDU0424	(0x1UL << 17)
#define SHIELD_DEVICETYPE_KDU0414	(0x1UL << 18)
#define SHIELD_DEVICETYPE_KDU0824	(0x1UL << 19)

#define SHIELD_DEVICETYPE_FTDI		(0x1UL << 30)

/*
  ��� ������� ���������� ���������� ����������� ��������� (>= 0)
  ��� ������������� ��������, ��������������� � ���, ��� ��������
  ������������� ����������, ������� ����� ��������� ��������� �������.
*/

/*
  ��������� ����� ����������� (Maska) � ���������� (EnaExt) ��������
  �� ���� ��������� � �������, ������� ������� ���������.
*/
int __cdecl ShieldAll(unsigned long *Maska, unsigned long *EnaExt);

/*
  ��������� ����� ����������� (Maska) � ���������� (EnaExt) ��������
  �� ���� ��������� � �������, ������� ������� ���������, � �����
  ������ ����� ����� ������������ ��������� � ��������� �������:
    - ���� � 0 �� 23-� ������������� ����� �� Zadc,
        (��������, (1 << 19) - �������� ������� ���� 19)
    - ��� 30 ������������� ZET Key.
*/
int __cdecl ShieldAllEx(unsigned long *Maska, unsigned long *EnaExt, unsigned long *DeviceTypes);

/*
  ��������� ����������� ���������� ���� �������� ����� ��������
  ������������ ����� �� ���� ��������� � �������.

  ���� ������������ �������� ������ 0, �� ������ ������ � �����������
  ���� �������� ���� ���� - ����� ������� ������� ��� ���.
*/
int __cdecl Shield(char ShieldCode, char *Result);

/*
  ��������� ����� �������� ������ ��� ������� ���������� ���������� � ��������� �������� �������.

  ���� ���������� �� �������, ������������ 0 ��� ������������� ��������.

  ������������ �������� >1 ������������� �� ��������� ��������, ����� � ������� ����
  ��������� ��������� � ���������� �������� �������. ��� ���� Maska � EnaExt ��������
  ����� ������ ��� ������� �� ��������� ���������.
*/
int __cdecl ShieldDevice(unsigned long long Serial, unsigned long *Maska, unsigned long *EnaExt);


#define SHIELD_NAME		"Shield"
#define SHIELD_LIB_EXT	".lib"

/* ���� � ����������� ���������� */
#ifndef DEBUG
#define SHIELD_LIB_NAME	SHIELD_NAME SHIELD_LIB_EXT
#else
#define SHIELD_LIB_NAME	SHIELD_NAME "d" SHIELD_LIB_EXT
#endif

#ifndef _WIN64
#define ARCHITECTURE	"x32"
#else
#define ARCHITECTURE	"x64"
#endif

#pragma comment(lib, "Shield\\" ARCHITECTURE "\\" SHIELD_LIB_NAME)

#endif /* SHIELD_LIB_INCLUDED */
