//-----------------------------------------------------------------------------
#pragma once
//-----------------------------------------------------------------------------
#define SIZE_NAME_PRE_UNIT		4
#define NUMBER_PRE_UNIT			13
#define NUMBER_PRE_UNIT_HALF	6
//-----------------------------------------------------------------------------
// Флаги, все
#define FLAG_ERROR					0x00000001
#define FLAG_TRANSFORM_ENABLE		0x00000002	// у типа есть разрешенные кратные/дольные единицы, кроме основной единицы
#define FLAG_TRANSFORM_UP_ENABLE	0x00000004	// у единицы есть разрешенные кратные/дольные единицы с n больше, чеv n у единицы
#define FLAG_TRANSFORM_DOWN_ENABLE	0x00000008	// у единицы есть разрешенные кратные/дольные единицы с n меньше, чеv n у единицы
#define FLAG_TRANSFORM_NEED			0x00000010	// требуется автоматическое преобразование величины
#define FLAG_TRANSFORM_EVENT		0x00000020	// было преобразование величины

#define FLAG_PRE_UNIT_m24			0x00000040		// иокто
#define FLAG_PRE_UNIT_m21			0x00000080		// зепто
#define FLAG_PRE_UNIT_m18			0x00000100		// атто
#define FLAG_PRE_UNIT_m15			0x00000200		// фемто
#define FLAG_PRE_UNIT_m12			0x00000400		// пико
#define FLAG_PRE_UNIT_m9			0x00000800		// нано
#define FLAG_PRE_UNIT_m6			0x00001000		// микро
#define FLAG_PRE_UNIT_m3			0x00002000		// милли
#define FLAG_PRE_UNIT_m2			0x00004000		// санти
#define FLAG_PRE_UNIT_m1			0x00008000		// деци
#define FLAG_PRE_UNIT_0				0x00010000		// 65536	
#define FLAG_PRE_UNIT_p1			0x00020000		// дека
#define FLAG_PRE_UNIT_p2			0x00040000		// гекто
#define FLAG_PRE_UNIT_p3			0x00080000		// кило
#define FLAG_PRE_UNIT_p6			0x00100000		// мега
#define FLAG_PRE_UNIT_p9			0x00200000		// гига
#define FLAG_PRE_UNIT_p12			0x00400000		// тера
#define FLAG_PRE_UNIT_p15			0x00800000		// пета
#define FLAG_PRE_UNIT_p18			0x01000000		// экса
#define FLAG_PRE_UNIT_p21			0x02000000		// зетта
#define FLAG_PRE_UNIT_p24			0x04000000		// нотта

// #define FLAG_						0x08000000
// #define FLAG_						0x10000000
// #define FLAG_						0x20000000
// #define FLAG_						0x40000000
// #define FLAG_						0x80000000
//-----------------------------------------------------------------------------
// Определение флага кратной/дольной единицы по степени
DWORD DefFlagByN(short n);

// Определение степени кратной/дольной единицы по флагу
short DefNByFlag(DWORD flag);

// Определение индекса кратной/дольной единицы (в массиве этих единиц) по степени
WORD DefIndxByN(const short n);

// Определение по текущей степени кратной/дольной единицы степени допустимой
// соседней кратной/дольной единицы при увеличении степени
bool DefN_up(const short nSrc, short & nDst, const DWORD flagsTypeUnit);

// Определение по текущей степени кратной/дольной единицы степени допустимой
// соседней кратной/дольной единицы при уменьшении степени
bool DefN_doun(const short nSrc, short & nDst, const DWORD flagsTypeUnit);

// Структура параметров дольных/кратных подъединиц измерения
struct STRUCT_PRE_UNIT
{
	short n;			// показатель степени
	short reserved;
	DWORD flag;			// флаг
//	wchar_t nameInt[SIZE_NAME_PRE_UNIT];	// международное обозначение
	wchar_t nameLoc[SIZE_NAME_PRE_UNIT];	// локальное обозначение
	double k;			// коэффициент пересчета в основную единицу

	STRUCT_PRE_UNIT(const short _n = 0);

	void SetOrder(const short _n);

	void SetNameInt(LPCWCH name);

	void SetNameLoc(LPCWCH name);
};
//-----------------------------------------------------------------------------