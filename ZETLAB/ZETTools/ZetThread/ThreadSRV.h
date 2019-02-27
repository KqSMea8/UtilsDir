//------------------------------------------------------------------------------
//	Файл ThreadSRV.h
//	Зеленоград, ЗАО "ЭТМС", Савостин Ю.М.
//------------------------------------------------------------------------------
//		Класс предназначен для считывания данных и их обработки одного
// многоканального сигнала, т.е. с нескольких каналов, объединённых общей целью.
// Т.е. каналов от одного устройств, с одинаковыми частотой дискретизации,
// кол-вом кадров, декадой и т.д.
//		Клаас является наследником класса CCustomThreadSRV.
//		Считается, что данные этих каналов требуют совместной обработки, поэтому
// для всех каналов имеется общая критическая секция для синхронизации.
//		При выполнении кода класса возможны 3 сообщения родительскому окну:
// 1. WM_NEW_DATA_SRV - данные считаны и обработаны без ошибок,
//		сообщение без параметров;
// 2. WM_ERROR_READ_SRV	- ошибка при чтении данных,
//		передаётся код ошибки и номер канала, при чтении которого была ошибка
// 3. WM_ERROR_CALC_SRV - ошибка при обработке данных, передаётся код ошибки.
//------------------------------------------------------------------------------
#pragma once
#include "afx.h"
#include <ZetThread\CollectionOfKits\ChannelForKit.h>
#include <ZetThread\CustomThreadSRV.h>
//------------------------------------------------------------------------------
//   Структура, содержащая частные параметры потока, указатель на её экземпляр
// является членом структуры общих параметров (см. класс-родитель).
struct ZetThreadParamSRV1
{
// 	int *pChannels;			// массив номеров каналов
	bool bNeedReadData;		// читать данные надо/не надо
	int NumberKadrs;		// кол-во считываемых кадров
// 	float **ppMasData;		// одномерный массив указателей (куда класть данные)
	long Decade;			// индекс декады сервера
	bool bNoWaitMissingChannel;	// не ждём пропавший канал
	double dTimeData;		// время чтения данных
	long QualityCode;		// код качества данных
	std::vector<CHANNEL_FOR_KIT> vectChan;	// вектор каналов
	PVOID pSaveData;		// адрес структуры текущих параметров программы
};
//------------------------------------------------------------------------------
class CThreadSRV : public CCustomThreadSRV
{
private:
	ZetThreadParamSRV1 par1;	// частные параметры потока
	void CreateMassivChan(int numChan);
// 	void DeleteMassivAll();
	double SetCurTime();
// 	bool bEnableMassChan;
// 	bool bEnableMassKadr;
	bool FAutoInterval;
protected:

public:
	CThreadSRV();
	virtual bool Create( CString nameThread, HWND hwnd, CDSRV *psrv,
					int numberChannels,  int numberKadr, bool bautoInterval,
					FunctionThread function, PVOID paramFunction = NULL );
	virtual ~CThreadSRV(void);

	// методы свойств
	bool GetAutoInterval() { return FAutoInterval; }
	void SetAutoInterval(bool val) { FAutoInterval = val; }

	bool SetNumberChannels(int val);

	int GetChannels(int iChannels);
	bool SetChannels(int iChannels, int val);

	long GetDecade()	{ return par1.Decade; }
	void SetDecade(long val)	{ par1.Decade = val; }

	HWND GetWindowHWND() { return zParam0.hWndParent; }

	bool GetNeedReadData() { return par1.bNeedReadData; }
	void SetNeedReadData(bool val) { par1.bNeedReadData = val; }
	
	int  GetNumberKadrs()	{ return par1.NumberKadrs; }
	bool SetNumberKadrs(int val);
	
	bool GetWaitMissingChannel() { return !par1.bNoWaitMissingChannel; }
	void SetWaitMissingChannel(bool val)
					{ par1.bNoWaitMissingChannel = !val; }

	double GetTimeData() { return par1.dTimeData; }
	long GetQualityData() { return par1.QualityCode; }

	// члены класса

	// методы класса
	virtual float* GetDataPointer(int iChannel);

	// Возвращает true, если у каналов были изменения
	bool Modify();
};
//------------------------------------------------------------------------------