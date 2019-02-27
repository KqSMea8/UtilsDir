//-----------------------------------------------------------------------------
// Файл EventDetector.h
//-----------------------------------------------------------------------------
//		Шаблонный класс событийного детектора
//-----------------------------------------------------------------------------
#pragma once
#include "afx.h"
#include <list>
#include <set>
#include "float.h"
#include <mathZet.h>
#include <ZetTools\include\ZetTools.h>
#include <Seismo/CustomAlgorithm/CustomAlgorithm.h>
#include <Seismo/CustomAlgorithm/Struct_Res/Ssm_res_algr_Custom.zschema1.h>
#include <Seismo/StationSSEC/Detection/Struct_Cfg/Ssm_cfg_EventDetector_StationProxy.h>
//-----------------------------------------------------------------------------
// Структура события
template <class T>
struct StructEventAndTime
{
	DATE time;
	T structure;

	StructEventAndTime()
		: time(0.)
	{}
	
	StructEventAndTime(const T & val)
	{
		*this = val;
	}
	
	StructEventAndTime(const StructEventAndTime & val)
	{
		*this = val;
	}

	virtual ~StructEventAndTime()
	{}

	void operator = (const StructEventAndTime & val)
	{
		time = val.time;
		UINT size = *(UINT*)&val.structure;
		memcpy_s(&this->structure, size, &val.structure, size);
	}

	void operator = (const T & val)
	{
		UINT size = *(UINT*)&val;
		memcpy_s(&structure, size, &val, size);
		long ret = ztGetStructureTimeLocal((ZetHeader*)&structure, &time);
	}
};

template <class T>
bool operator < (const StructEventAndTime<T> & val1, const StructEventAndTime<T> & val2)
{
	return val1.time < val2.time;
}
//-----------------------------------------------------------------------------
enum StatusObject
{
	so_fault = -1,		// объект не исправен
	so_unknown,			// состояние объекта неизвестно
	so_ok				// объект исправен
};
//-----------------------------------------------------------------------------
template <class T>
struct ObjectFlags
{
	UINT objectID;				// ID объекта
	StatusObject status;		// статус объекта
	std::vector<StructEventAndTime<T> > vectorET;

	ObjectFlags(
		const UINT id = 0,
		const StatusObject stat = StatusObject::so_unknown)
	{
		objectID = id;
		status = stat;
		vectorET.clear();
	}
	ObjectFlags(const ObjectFlags & val)
	{
		*this = val;
	}
	void operator = (const ObjectFlags & val)
	{
		objectID = val.objectID;
		status = val.status;
		vectorET = val.vectorET;
	}
	~ObjectFlags()
	{
		vectorET.clear();
	}
};

template <class T>
bool operator < (const ObjectFlags<T> & val1, const ObjectFlags<T> & val2)
{
	return val1.objectID < val2.objectID;
}
//-----------------------------------------------------------------------------
using namespace zet::algorithm::dsp::seismo;
// Шаблонный класс событийного детектора
template <class T>
class CEventDetector : public CCustomAlgorithm
{
private:
	double m_timeRMS;
	// вектор индексов
	std::vector<int> m_vIndx, m_vIndxFind;
	// вспомогательный вектор
	std::vector<double> m_vectDouble;
	// Функция перебора
	void Combination(std::set<ObjectFlags<T> > &setObjectWithEvent, UINT indxCur, UINT size);
	// Удаление старых событий
	void DeleteOldEvents(const DATE Tcur);

	Ssm_cfg_EventDetector_Station m_config;

	DATE m_dT;			// величина временного окна детектирования
	DATE m_timeLive;	// время жизни событий в буфере детектора
	std::list<StructEventAndTime<T> > m_listEvent;

protected:

public:
	CEventDetector();
	CEventDetector(const CEventDetector & val);
	virtual ~CEventDetector() {}
	void operator = (const CEventDetector & val);

	std::set<ObjectFlags<T> > m_setObject;
	Ssm_cfg_EventDetector_Station_ns::Proxy m_cfgClass;
	Ssm_res_Algr_Custom m_result;

	long SetConfigFromProxy(const DATE timeSRV);
	virtual long SetConfig(ZetHeader* pConfig, const DATE timeSRV);

	void UpdateSetObject(const UINT id, const StatusObject status);

	UINT GetNumberObjectIsOk();

	// Структура с событием от какого-то объекта
	void AddStruct(const T & event);

	// Определение было-ли событие
	bool Calculation(const DATE Tcur, std::list<T> & list);

	void ReStart();
};
//-----------------------------------------------------------------------------
template <class T>
void CEventDetector<T>::ReStart()
{
	m_setObject.clear();
	m_listEvent.clear();
}
//-----------------------------------------------------------------------------
template <class T>
CEventDetector<T>::CEventDetector() : CCustomAlgorithm()
{	// обнуление контейнеров класса
	m_pConfig = &m_config.zet_header;
	m_pResult = &m_result.zet_header;

	long ret = ztOpenStructure(m_pConfig,
		ZETLAB_SSM_RES_ALGR_CUSTOM_TYPE, ZETLAB_SSM_RES_ALGR_CUSTOM_SIZE);

	ret = ztOpenStructure(m_pResult,
		ZETLAB_SSM_RES_ALGR_CUSTOM_TYPE, ZETLAB_SSM_RES_ALGR_CUSTOM_SIZE);

	m_listEvent.clear();
	m_setObject.clear();

	// копирование параметрров из Proxy в структуру класса
	//SetConfigFromProxy();

	// вспомогательные переменные
	m_dT = 0.;
	m_timeLive = 0.;
}
//-----------------------------------------------------------------------------
template <class T>
CEventDetector<T>::CEventDetector(const CEventDetector & val)
	: CCustomAlgorithm(val)
{
	*this = val;
}
//-----------------------------------------------------------------------------
template <class T>
void CEventDetector<T>::operator = (const CEventDetector & val)
{
	*((CCustomAlgorithm*)this) = *((CCustomAlgorithm*)&val);

	m_dT = val.dT;
	m_timeLive = val.m_timeLive;
	m_listEvent = val.m_listEvent;

	m_config = val.m_config;
	m_result = val.m_result;

	m_setObject = val.m_setObject;
	m_cfgClass = val.m_cfgClass;

	m_pConfig = &m_config.zet_header;
	m_pResult = &m_result.zet_header;
}
//-----------------------------------------------------------------------------
template <class T>
long CEventDetector<T>::SetConfigFromProxy(const DATE timeSRV)
{
	m_config = m_cfgClass;
	m_dT = m_config.durationTimeWindow * M_SecAsPartOfDay;
	m_timeLive = m_config.timeLiveEvent * M_SecAsPartOfDay;
	m_result.valueThreshold = m_config.thresholdPercent;
	return ztSetStructureTimeLocal(m_pConfig, timeSRV);
}
//-----------------------------------------------------------------------------
template <class T>
long CEventDetector<T>::SetConfig(ZetHeader* pParam, const DATE timeSRV)
{	// проверка пришедшей структуры
	long ret = CCustomAlgorithm::SetConfig(pParam);
	if (ret == 0)
	{// копирование этой структуры в Proxy с целью проверки полей структуры
		m_cfgClass = *((Ssm_cfg_EventDetector_Station*)pParam);
		// копирование параметрров из Proxy в структуру класса
		ret = SetConfigFromProxy(timeSRV);
	}
	return ret;
}
//-----------------------------------------------------------------------------
template <class T>
void CEventDetector<T>::UpdateSetObject(const UINT id, const StatusObject status)
{
	ObjectFlags<T> tmp(id);
	m_setObject.insert(tmp);
	*(WORD*)&m_setObject.find(tmp)->status = status;
}
//-----------------------------------------------------------------------------
template <class T>
UINT CEventDetector<T>::GetNumberObjectIsOk()
{
	UINT ret(0);
	if (!m_setObject.empty())
	{
		for (auto & it : m_setObject)
		{
			if (it.status == StatusObject::so_ok)
				++ret;
		}
	}
	return ret;
}
//-----------------------------------------------------------------------------
template <class T>
void CEventDetector<T>::DeleteOldEvents(const DATE Tcur)
{
	DATE T0 = Tcur - m_timeLive;
	while (
		!m_listEvent.empty() &&
		(m_listEvent.cbegin()->time < T0))
		m_listEvent.pop_front();
}
//-----------------------------------------------------------------------------
template <class T>
void CEventDetector<T>::AddStruct(const T & event)
{
	StructEventAndTime<T> tmp(event);

	// удаление старых событий
	DeleteOldEvents(tmp.time);

	// добавление события в контейнер событий
	m_listEvent.push_back(tmp);
	m_listEvent.sort();		// обязательно!!!
}
//-----------------------------------------------------------------------------
// Перебираем все возможные варианты (один датчик - одна структура)
// с целью определения варианта с минимальным разбросом времён.
// Разброс определяю по СКО времён и сравниваю с текущим минимальным СКО,
// которое в m_timeRMS.
// Результат - это вектор индексов m_vIndxFind
template <class T>
void CEventDetector<T>::Combination(
	// набор структур от разных датчиков (есть датчики, у которых несколько структур)
	std::set<ObjectFlags<T> > &setObjectWithEvent,
	UINT indxCur,	// текущий индекс датчика (от 0 до size-1)
	UINT size)		// кол-во датчиков (размер setObjectWithEvent)
{
	if (indxCur < size)
	{	// перебор структур у датчика с текущим индексом
		int n(0);
		auto it_set = setObjectWithEvent.cbegin();
		if (indxCur > 0)
		{
			for (UINT m = 0; m < indxCur; ++m)
				++it_set;
		}
		for (auto it = it_set->vectorET.cbegin(),
			ie = it_set->vectorET.cend(); it != ie; ++it)
		{
			m_vIndx[indxCur] = n++;
			Combination(setObjectWithEvent, indxCur + 1, size);
		}
	}
	else
	{// Есть очередной набор (один датчик - одна структура)
		// В данном наборе определяю вектор времён
		for (UINT i = 0; i < size; ++i)
		{
			auto it_set = setObjectWithEvent.cbegin();
			if (i > 0)
			{
				for (UINT m = 0; m < i; ++m)
					++it_set;
			}
			m_vectDouble[i] = (it_set->vectorET.cbegin() + m_vIndx[i])->time;
		}
		// по вектору времён определяю СКО, и если оно меньше текущего СКО,
		// то запоминаю этот набор
		double rms(0.);
		if (ippsStdDev_64f(m_vectDouble.data(), size, &rms) == ippStsNoErr)
		{
			if (m_timeRMS > rms)
			{
				m_timeRMS = rms;
				m_vIndxFind = m_vIndx;
			}
		}
	}
}
//-----------------------------------------------------------------------------
template <class T>
bool CEventDetector<T>::Calculation(
	const DATE Tcur,
	std::list<T> & list)
{
	m_result.result = 0;
	m_result.value = 0.f;
	list.clear();

	bool ret = (Tcur > 0.) &&
		!m_listEvent.empty() &&		// контейнер событий
		!m_setObject.empty();		// контейнер объектов
	if (ret)
	{	// определение временного окна детектирования ----------------------
		DATE dt = m_dT * 0.5;
		DATE Tstart = Tcur - dt;
		DATE Tend = Tcur + dt;

		// Определение событий, попавших в окно ----------------------------
		ret = (Tstart > 0.) && (Tend > 0.);
		if (ret)
		{	// обнуление событий у всех объектов
			for (auto & it : m_setObject)
			{
				std::vector<StructEventAndTime<T> > *pVector =
					(std::vector<StructEventAndTime<T> > *)&it.vectorET;
				pVector->clear();
			}
			// определение перечня событий от всех объектов, попавших во временное окно
			// события от одного объекта заносим в вектор этого объекта
			for (auto & it : m_listEvent)
			{
				if ((Tstart <= it.time) && (it.time <= Tend))
				{
					ObjectFlags<Ssm_str_EventParam> of(it.structure.objectID);
					auto it_find_inSet = m_setObject.find(of);
					std::vector<StructEventAndTime<T> > *pVector =
						(std::vector<StructEventAndTime<T> >*)&it_find_inSet->vectorET;
					pVector->push_back(it);
				}
			}

			// определение результата детектора
			UINT nObjectIsOk(0);		// кол-во работоспособных объектов
			UINT nObjecWithtEvent(0);	// кол-во сработавших объектов
			for (auto & it : m_setObject)
			{
				if (it.status == StatusObject::so_ok)
					++nObjectIsOk;
				if (!it.vectorET.empty())
					++nObjecWithtEvent;
			}
			ret = (nObjectIsOk > 0) && (nObjecWithtEvent > 0);
			if (ret)
			{
				m_result.value =
					(float)nObjecWithtEvent / (float)nObjectIsOk * 100.f;
				ret = m_result.value >= m_config.thresholdPercent;
				if (ret)
					m_result.result = 1;
			}

			// результат есть, определяю вариант с наибольшей "кучностью" событий 
			if (ret)
			{	// Новый набор объектов - только те,				
				// у которых есть события setObjectWithEvent == setOWE
				std::set<ObjectFlags<T> > setOWE = m_setObject;
				bool flag(true);
				while (flag)
				{
					flag = false;
					for (auto it = setOWE.begin(), ie = setOWE.end(); it != ie; ++it)
					{
						if (it->vectorET.empty())
						{
							flag = true;
							setOWE.erase(it);
							break;
						}
					}
				}
				UINT size = setOWE.size();	// кол-во датчиков с событием
				ret = false;
				// событие только у одного датчика
				if (size == 1)
				{
					ret = true;
					list.push_back(setOWE.cbegin()->vectorET.crbegin()->structure);
				}
				// событие у нескольких датчиков
				else if (size > 1)
				{
					ret = true;
					UINT i(1);
					for (auto & it : setOWE)
						i *= it.vectorET.size();
					// у всех датчиков по одной структуре
					if (i == 1)
					{
						for (auto & it : setOWE)
							list.push_back(it.vectorET.cbegin()->structure);
					}
					else
					{// не у всех датчиков по одной структуре
						m_vIndx.resize(size);
						m_vIndxFind.resize(size);
						m_vectDouble.resize(size);
						i = 0;
						for (auto & it : setOWE)
						{
							m_vIndx[i] = 0;
							m_vIndxFind[i] = 0;
							++i;
						}
						// перебираем все возможные варианты (один датчик - одна структура)
						// с целью определения варианта с минимальным разбросом времён
						// результат - это вектор индексов m_vIndxFind
						m_timeRMS = DBL_MAX;	// инициализация перед перебором
						Combination(setOWE, 0, size);
						// структуры найденного варианта - в list
						i = 0;
						for (auto & it_set : setOWE)
						{
							auto it_vect = it_set.vectorET.cbegin() + m_vIndxFind[i++];
							list.push_back(it_vect->structure);
						}
					}
				}
			}//	Новый набор объектов
		}// временное окно определено
	}// контейнеры событий и объектов не пусты
	if (!ret)
		list.clear();

	long r = ztSetStructureTimeLocal(&m_result.zet_header, Tcur);
	
	return ret;
}
//-----------------------------------------------------------------------------