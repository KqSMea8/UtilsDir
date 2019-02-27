//------------------------------------------------------------------------------
// Файл WriterDTU.cpp
// Версия от 07.09.2012
// ЗАО "ЭТМС", Зеленоград, Савостин Ю.М.
//------------------------------------------------------------------------------
#include "stdafx.h"
#include <WriterDTU\WriterDTU.h>
//------------------------------------------------------------------------------
CWriterDTU::CWriterDTU()
{
	m_bNewFolder = true;
	m_bAllConvertSignals = true;
	m_bNeedBandFiltr = true;
	bDataEnabled = false;
	m_bNextFileDTU = false;
	bZetPathInternal = false;
	numSignals = numKadrs = 0;
	m_FreqADC = Fmin = Fmax = 0.f;
	t0 = 0.;
	T0 = 0.;
	ppData = NULL;
	pParent = NULL;
	pzPath = NULL;
	m_saMain.resize(9);
	strDate = L"Дата";
	strTime = L"Время";
	strHz = L"Гц";
	strSec = L"сек";
	strFrom = L"от";
	strTo = L"до";
	m_NameFileDTU = L"sig";
	m_saMain[0] = L"Временная реализация сигналов";
	m_saMain[1] = L"Полосовая фильтрация исходных сигналов";
	m_saMain[4] = L"Частотный диапазон";
}
//------------------------------------------------------------------------------
CWriterDTU::~CWriterDTU()
{
	DeleteData();
	if (bZetPathInternal && pzPath)
	{
		delete pzPath;
		pzPath = nullptr;
	}
}
//------------------------------------------------------------------------------
bool CWriterDTU::Create(CWnd *parent, CDZetPath *pZetPath, int nSignals, int nKadrs)
{
	bool ret = parent != 0;
	if (ret)
	{
		pParent = parent;
		pzPath = pZetPath;
		bZetPathInternal = pzPath == 0;
		numSignals = nSignals;
		numKadrs = nKadrs;
		m_saName.resize(numSignals);
		m_saConv.resize(numSignals);
		if ( (nKadrs > 0) && (nSignals > 0) )
			CreateData();
	}
	else
		ret = false;
	return ret;
}
//------------------------------------------------------------------------------
void CWriterDTU::DeleteData()
{
	if (ppData)
	{
		for(int i=0; i<numSignals; i++)
		{ 
			delete [] ppData[i];
			ppData[i] = nullptr;
		}
		delete [] ppData;
		ppData = nullptr;
	}
	ppData = nullptr;
}
//------------------------------------------------------------------------------
void CWriterDTU::CreateData()
{
	ppData = new float* [numSignals];
	for(int i=0; i<numSignals; i++)
		ppData[i] = new float [numKadrs];
}
//------------------------------------------------------------------------------
void CWriterDTU::SetConvertSignalAll(CString str)
{
	for(int i=0; i<numSignals; i++)
		m_saConv[i] = str;
}
//------------------------------------------------------------------------------
void CWriterDTU::SetNameSignal(int index, CString str)
{
	if (index < numSignals)
		m_saName[index] = str;
}
//------------------------------------------------------------------------------
void CWriterDTU::SetConvertSignal(int index, CString str)
{
	if (index < numSignals)
		m_saConv[index] = str;
}
//------------------------------------------------------------------------------
void CWriterDTU::SetNameFileDTU(CString str)
{
	m_NameFileDTU = str;
	if ( m_NameFileDTU.Find(dtu) == -1 )
		m_NameFileDTU += dtu;
}
//------------------------------------------------------------------------------
bool CWriterDTU::CopyData(int index, float *pAdd)
{
	bool ret;
	if ( ppData && (numKadrs > 0) && (index < numSignals) )
	{
		int n = numKadrs * sizeof(float);
		memcpy_s(ppData[index], n, pAdd, n);
		bDataEnabled = true;
		ret = true;
	}
	else
		ret = false;
	return ret;
}
//------------------------------------------------------------------------------
bool CWriterDTU::CopyDataAll(float **ppAdd)
{
	bool ret;
	if ( ppData && (numKadrs > 0) && (numSignals > 0) )
	{
		int n = numKadrs * sizeof(float);
		for(int i=0; i<numSignals; i++)
			memcpy_s(ppData[i], n, ppAdd[i], n);
		bDataEnabled = true;
		ret = true;
	}
	else
		ret = false;
	return ret;
}
//------------------------------------------------------------------------------
bool CWriterDTU::SetNumberSignals(int val)
{
	bool ret;
	if (val > 0)
	{
		if (numSignals != val)
		{
			m_saName.resize(val);	// имена сигналов
			m_saConv.resize(val);	// размерности сигналов
			if (numKadrs > 0)
			{
				DeleteData();
				numSignals = val;	// именно так!!!
				CreateData();
			}
			else
				numSignals = val;	// именно так!!!
		}
		ret = true;
	}
	else
		ret = false;
	return ret;
}
//------------------------------------------------------------------------------
bool CWriterDTU::SetNumberKadrs(int val)
{
	bool ret;
	if (val > 0)
	{
		if (numSignals > 0)
		{
			DeleteData();
			numKadrs = val;		// именно так!!!
			CreateData();
		}
		else
			numKadrs = val;		// именно так!!!
		ret = true;
	}
	else
		ret = false;
	return ret;
}
//------------------------------------------------------------------------------
bool CWriterDTU::Write()
{
	int i;
	CString str, str1;
	if ( !bDataEnabled || (numSignals < 1) || ( numKadrs < 1 ) )
		return false;
	// готовим строки к записи -------------------------------------------
//	m_bNeedBandFiltr = !( (Fmin == 0.f) && (Fmax == 0.f) );
	m_bNeedBandFiltr = !( (IsEqual(Fmin, 0.f, 0.0001f)) && (IsEqual(Fmax, 0.f, 0.0001f)) );
	if (m_bNeedBandFiltr)
	{
		m_saMain[1] += L":";
		m_saMain[2].Format(L"\tFmin = %.4g " + strHz, Fmin);
		m_saMain[3].Format(L"\tFmax = %.4g " + strHz, Fmax);
	}
	// строка "Частотный диапазон - от 0 Гц до 1000.000000 Гц"
	str.Format(L" - " + strFrom + L" 0 " + strHz + L" " + strTo + L" %f " + strHz,
		m_FreqADC * 0.5f);
	m_saMain[4] += str;
	// строка "Дата : 17.07.2012"
	if (T0 == 0.)
		T0 = COleDateTime::GetCurrentTime().m_dt;
	str.Preallocate(32);
	CZetTime zt(T0);
	zt.ConvertToString(str.GetBuffer(), 32, 1./m_FreqADC, L"dd.MM.yyyy HH:mm:ss", false);
	str.ReleaseBuffer();
	str1 = str;
	i = str.Find(L" ");
	str.Delete(i, str.GetLength() - i);
	str1.Delete(0, i+1);
	m_saMain[5] = strDate + L" : " + str;
	// строка "Время : 13:17:05.01234"
	m_saMain[6] = strTime + L" : " + str1;
	// строка с именами сигналов
	m_saMain[7]	= strTime;
	for(i=0; i<numSignals; i++)
		m_saMain[7] += L"\t" + m_saName[i];
	// строка с размерностями сигналов
	m_saMain[8]	= strSec;
	for(i=0; i<numSignals; i++)
		m_saMain[8] += L"\t" + m_saConv[i];

	// определяю имя файл ----------------------------------------------
	while(true)
	{// удаляю из имени файла путь если он есть
		i = m_NameFileDTU.Find(L"\\");
		if (i != -1)
			m_NameFileDTU.Delete(0, i+1);
		else
			break;
	}
	bool flag;
	CString path;
	if (bZetPathInternal && (pzPath == 0))
	{// создаю ZetPath, если его нет
		pzPath = new CDZetPath();
		flag = pzPath->Create(L"ZetPath", WS_CHILD, CRect(0,0,0,0), pParent, 1707) != 0;
	}
	else
		flag = true;
	if (flag)
	{
		BSTR dir = _T("DirResult");
		if (m_bNewFolder)	// если папка нужна новая
			pzPath->GetNextSignals();
		path = pzPath->ZetPathW(&dir);
		if (m_bNextFileDTU)
		{// если нужна инкрементация номера файла
			BSTR Bpath, Bfile, Bdtu;
			Bpath = path.AllocSysString();
			Bdtu = dtu.AllocSysString();
			i = m_NameFileDTU.Find(dtu);
			if ( i != -1 )
			{
				CString nfnd = m_NameFileDTU;	// Name File No DTU
				nfnd.Delete(i, 4);				// удалил ".dtu"
				Bfile = nfnd.AllocSysString();
			}
			else
				Bfile = m_NameFileDTU.AllocSysString();
			m_NameFileDTU = pzPath->GetNextFileInDir(&Bpath, &Bfile, &Bdtu );
			SysFreeString(Bpath);
			SysFreeString(Bfile);
			SysFreeString(Bdtu);
		}
		m_NameFileDTU = path + m_NameFileDTU;
	}
	SetNameFileDTU(m_NameFileDTU);	// проверка, что в конце .dtu
	
	// открываю файл -------------------------------------------------------
	FILE *File(nullptr);
	bool ret = 0 == zfOpenFile(m_NameFileDTU, L"at, ccs=UTF-8", &File);
	if (ret)
	{
		// пишу заголовок файла ---------------------------------------------
		long err = zfWriteIntoFileW(File, m_saMain[0] + L"\n");

		if ( !m_saComm.empty())
		{
			for (auto & it : m_saComm)
				err = zfWriteIntoFileW(File, it + L"\n");
		}
		if (m_bNeedBandFiltr)
		{
			for (i = 1; i < 4; i++)
				err = zfWriteIntoFileW(File, m_saMain[i] + L"\n");
		}
		for (i = 4; i < 9; i++)
			err = zfWriteIntoFileW(File, m_saMain[i] + L"\n");
		// сами данные -----------------------------------------------------
		double t = t0, dt = 1. / m_FreqADC;
		for(i=0; i<numKadrs; i++)
		{
			str.Format(L"%f", t);
			for(int n=0; n<numSignals; n++)
			{
				str1.Format( L"\t%f", ppData[n][i] );
				str += str1;
			}
			str += L"\n";
			err = zfWriteIntoFileW(File, str);
			t += dt;
		}
		zfCloseFile(File);
		ret = true;
	}
	return ret;
}
//------------------------------------------------------------------------------