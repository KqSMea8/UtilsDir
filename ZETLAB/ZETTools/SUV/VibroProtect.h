#ifndef CVIBROPROTECT_H
#define CVIBROPROTECT_H
#pragma once

#include <vector>

class CVibroProtect
{
public:
	CVibroProtect(void);
	~CVibroProtect(void);
	UINT uSerialNumber;					//  Серийный номер
	float fPeakForce;					//  Выталкивающая сила
	float fMovingMass;					//  Масса подвижной части
	float fMaxWeight;					//  Максимальная нагрузка
	float MaxAccel;						//	Максимальное ускорение в единицах g
	float MaxVel;						//	Максимальная скорость в метрах на секунду
	float MaxDispUp;					//	Максимальное перемещение в милиметрах
	float MaxDispDown;					//	Максимальное перемещение в милиметрах
	float MinFreq;						//	Минимальная частота в герцах
	float MaxFreq;						//	Максимальная частота в герцах
	float MaxVolt;						//	Максимальное напряжение в вольтах
	CString orient;						//	Ориентация стенда в пространстве
	CString name;						//	Название стенда в бкувах
	CString imgName;					//	Название картинки
	bool AccelMesureInG;				//	Измерение ускорения производятся в единицах g

	std::vector<CString> m_unit;


	void CheckChanData();
	bool GetProtectData(CString sShakerParPath);
	bool GetProtectDataXML(CString sShakerCfgPath);
};

#endif