// new_chrec.h
#ifndef __new_chrec_h__
#define __new_chrec_h__

struct NEW_CHREC		//параметры читаются из флэшки при включении питания
{
	unsigned long sizeSelf;		// размер этой структуры
	char Signature[16];			// сигнатура
	unsigned short TotalDetectors;
	unsigned short TypeDetector[1];
	NEW_CHREC()
		: sizeSelf(0)
		, TotalDetectors(0)
	{
		ZeroMemory(Signature, sizeof(Signature));
		ZeroMemory(TypeDetector, sizeof(TypeDetector));

	}
} ;

#endif __new_chrec_h__