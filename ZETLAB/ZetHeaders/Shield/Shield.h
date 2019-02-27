#ifndef SHIELD_LIB_INCLUDED
#define SHIELD_LIB_INCLUDED
#pragma once

/*
  “ипы устройств.
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
  ¬се функции возвращают количество проверенных устройств (>= 0)
  или отрицательное значение, сигнализирующее о том, что остались
  непроверенные устройства, которые можно проверить повторным вызовом.
*/

/*
  ¬ычисл€ет маску программной (Maska) и аппаратной (EnaExt) лицензий
  со всех устройств в системе, которые удалось проверить.
*/
int __cdecl ShieldAll(unsigned long *Maska, unsigned long *EnaExt);

/*
  ¬ычисл€ет маску программной (Maska) и аппаратной (EnaExt) лицензий
  со всех устройств в системе, которые удалось проверить, а также
  выдает маску типов обнаруженных устройств в следующем формате:
    - биты с 0 по 23-й соответствуют типам из Zadc,
        (например, (1 << 19) - означает наличие типа 19)
    - бит 30 соответствует ZET Key.
*/
int __cdecl ShieldAllEx(unsigned long *Maska, unsigned long *EnaExt, unsigned long *DeviceTypes);

/*
  ¬ычисл€ет доступность указанного кода лицензии путем проверки
  лицензионных масок со всех устройств в системе.

  ≈сли возвращаемое значение меньше 0, то делать выводы о доступности
  кода лицензии пока рано - лучше вызвать функцию еще раз.
*/
int __cdecl Shield(char ShieldCode, char *Result);

/*
  ¬ычисл€ет маски лицензий только дл€ первого найденного устройства с указанным серийным номером.

  ≈сли устройство не найдено, возвращаетс€ 0 или отрицательное значение.

  ¬озвращаемое значение >1 сигнализирует об ошибочной ситуации, когда в системе есть
  несколько устройств с одинаковым серийным номером. ѕри этом Maska и EnaExt содержат
  маски только дл€ первого из найденных устройств.
*/
int __cdecl ShieldDevice(unsigned long long Serial, unsigned long *Maska, unsigned long *EnaExt);


#define SHIELD_NAME		"Shield"
#define SHIELD_LIB_EXT	".lib"

/* ѕуть к статической библиотеке */
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
