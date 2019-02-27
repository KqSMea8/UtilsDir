// calibr.h
#ifndef __calibr_h__
#define __calibr_h__

typedef struct _CALIBR		//параметры читаются из флэшки при включении питания
{
	unsigned long sizeSelf;		// размер этой структуры
	char Signature[16];			// сигнатура
	short ICP;					// 0-ICP выключен, 1-ICP включен,
	short TestCode[3];			// тестовый код для проверки целостности + выравнивание
	char name[128];				// название канала
	char conv[16];				// единицы измерения
	float sense;				// вес младшего разряда
	float DC;					// смещение нуля
	float refer;				// опорное значение для вычисления значения в дБ
	float ExtAmp;				// КУ внешнего усилителя
	_CALIBR() 
		: sizeSelf(2500)
		, ICP(0)
		, sense(0.f)
		, DC(0.f)
		, refer(0.f)
		, ExtAmp(0.f)
	{
		ZeroMemory(Signature, sizeof(Signature));
		ZeroMemory(TestCode, sizeof(TestCode));
		ZeroMemory(name, sizeof(name));
		ZeroMemory(conv, sizeof(conv));

	}
} CALIBR, *PCALIBR;

#endif __calibr_h__