//--------------------------------------------------------------------------
// Файл WriterDTU.h
// Версия от 07.09.2012
// ЗАО "ЭТМС", Зеленоград, Савостин Ю.М.
//--------------------------------------------------------------------------
//	Класс предназначен для записи файлов DTU
//------------------------------------------------------------------------------
#pragma once
#include "afx.h"
#include <Constants.h>
#include <ZetTools\include\ZetTools.h>
#include <ZetFile\include\ZetFile.h>
#include <Dialog_ZET\CDZetPath.h>
const CString dtu = L".dtu";
//--------------------------------------------------------------------------
class CWriterDTU
{
private:
	CWnd *pParent;
	CDZetPath *pzPath;
	int numSignals;
	int numKadrs;
	float Fmin, Fmax;		// частоты среда полосового фильтра
	CString strDate, strTime, strHz, strSec, strFrom, strTo;
	double t0;
	DATE T0;
	bool bDataEnabled;
	bool bZetPathInternal;
	float **ppData;
	void DeleteData();
	void CreateData();

protected:

public:
	CWriterDTU();
	virtual ~CWriterDTU();
	bool Create(CWnd *parent, CDZetPath *pZetPath,
		int nSignals = 0, int nKadrs = 0);

	bool m_bNewFolder;
	bool m_bAllConvertSignals;
	bool m_bNeedBandFiltr;
	bool m_bNextFileDTU;
	float m_FreqADC;		// частота дискретизации
	CString m_NameFileDTU;
	std::vector<CString> m_saMain;	// общие 9 строк файла
	std::vector<CString> m_saComm;	// комментарии
	std::vector<CString> m_saName;	// имена сигналов
	std::vector<CString> m_saConv;	// размерности сигналов

	// функции свойств
	// функции SetStr..(..) нужно вызывать только при необходимости локализации
	void SetStrDate(CString str) { strDate = str; }
	void SetStrTime(CString str) { strTime = str; }
	void SetStrHz  (CString str) { strHz   = str; }
	void SetStrSec (CString str) { strSec  = str; }
	void SetStrFrom(CString str) { strFrom = str; }
	void SetStrTo  (CString str) { strTo   = str; }
	void SetStrTitle(CString str) { m_saMain[0] = str; }
	void SetStrFiltration(CString str) { m_saMain[1] = str; }
	void SetStrFreqBand(CString str) { m_saMain[2] = str; }

	void SetFreqADC(float val) { m_FreqADC = val; }
	void SetFreqBand(float fmin, float fmax) { Fmin = fmin; Fmax = fmax; }
	void SetFirstTimeSRV(double val) { t0 = val; }
	void SetFirstTimeDATE(DATE val) { T0 = val; }
	void SetNameSignal(int index, CString str);
	void SetConvertSignal(int index, CString str);
	void SetConvertSignalAll(CString str);
	void SetNameFileDTU(CString str);
	int GetNumberSignals() { return numSignals; }
	bool SetNumberSignals(int val);
	int GetNumberKadrs() { return numKadrs; }
	bool SetNumberKadrs(int val);

	// функции класса
	void AddComment(CString str) { m_saComm.push_back(str); }
	bool CopyData(int index, float *pAdd);
	bool CopyDataAll(float **ppAdd);
	void RemoveAllComment() { m_saComm.clear(); }
	bool Write();
};
//--------------------------------------------------------------------------