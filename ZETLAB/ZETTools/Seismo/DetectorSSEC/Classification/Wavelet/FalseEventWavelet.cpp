//------------------------------------------------------------------------------
#include "stdafx.h"
#include <Algorithms\SomeFeatures.h>
#include "FalseEventWavelet.h"

namespace zet {
namespace algorithm {
namespace dsp {
namespace seismo {
//------------------------------------------------------------------------------
CFalseEventWavelet::CFalseEventWavelet() : CCustomFalseEvent()
	, nTp(0)
{
	m_bOriginalSignal = false;

	m_type_Event_Algr = tea_ActualEvent;
	m_pConfig = &m_config.zet_header;

	long r = ztOpenStructure(m_pConfig, 
		ZETLAB_SSM_CFG_ALGR_WAVELET_TYPE, ZETLAB_SSM_CFG_ALGR_WAVELET_SIZE);

	// структура результата см. базовый класс
}
//------------------------------------------------------------------------------
CFalseEventWavelet::CFalseEventWavelet(
	const CFalseEventWavelet &val) : CCustomFalseEvent(val)
{
	m_cfgClass = val.m_cfgClass;
	m_config = val.m_config;

	nTp = val.nTp;
	m_saXYZ[0] = val.m_saXYZ[0];
	m_saXYZ[1] = val.m_saXYZ[1];
	m_saXYZ[2] = val.m_saXYZ[2];
}
//------------------------------------------------------------------------------
CFalseEventWavelet::~CFalseEventWavelet()
{}
//------------------------------------------------------------------------------
void CFalseEventWavelet::operator = (const CFalseEventWavelet &val)
{
	*((CCustomFalseEvent*)this) = *((CCustomFalseEvent*)&val);
	m_cfgClass = val.m_cfgClass;
	m_config = val.m_config;

	nTp = val.nTp;
	m_saXYZ[0] = val.m_saXYZ[0];
	m_saXYZ[1] = val.m_saXYZ[1];
	m_saXYZ[2] = val.m_saXYZ[2];
}
//------------------------------------------------------------------------------
long CFalseEventWavelet::ReStart()
{
	nTp = 0;
	m_saXYZ[0].Zero();
	m_saXYZ[1].Zero();
	m_saXYZ[2].Zero();
	return 0;
}
//------------------------------------------------------------------------------
long CFalseEventWavelet::SetConfigFromProxy(const DATE timeSRV)
{// копирование cfg-параметров из Proxy в структуру класса
	m_config = m_cfgClass;
	m_result.valueThreshold = m_config.thresholdPercent;
	
	return ztSetStructureTimeLocal(m_pConfig, timeSRV);
}
//------------------------------------------------------------------------------
long CFalseEventWavelet::SetConfig(ZetHeader* pParam, const DATE timeSRV)
{	// проверка пришедшей структуры
	long ret = CCustomAlgorithm::SetConfig(pParam, timeSRV);
	if (ret == NoError)
	{	// копирование этой структуры в Proxy с целью проверки полей структуры
		m_cfgClass = *((Ssm_cfg_Algr_Wavelet*)pParam);
		// копирование параметрров из Proxy в структуру класса
		ret = SetConfigFromProxy(timeSRV);
	}
	return ret;
}
//------------------------------------------------------------------------------
bool CFalseEventWavelet::FiltrFDWT(
	const float *pDataX, const float *pDataY, const float *pDataZ,
	int size, int sizePreHistory, int * pnT_start)
{
	bool ret =
		(pDataX != nullptr) &&
		(pDataY != nullptr) &&
		(pDataZ != nullptr) &&
		m_saXYZ[0].ReSize(size) &&
		m_saXYZ[1].ReSize(size) &&
		m_saXYZ[2].ReSize(size) &&
		(sizePreHistory < size);
	*pnT_start = 0;
	if (ret)
	{
		const float *pXYZ_inp[3] = { pDataX, pDataY, pDataZ };
		float *pXYZ_out[3] = { m_saXYZ[0].pData, m_saXYZ[1].pData, m_saXYZ[2].pData };
		ret = SearchPositionBeginUsingFDWT(
			pXYZ_inp,				//массив исходных данных
			pXYZ_out,				//массив данных после вейвлет-фильтрации
			3,						//размерность сигнала
			size,					//размер каждой из компонент сигнала
			sizePreHistory,			//размер предыстории, где точно нет события
			pnT_start,				//найденная точка при поиске слева
			nullptr,				//найденная точка при поиске справа
			(TypeWavelet)m_config.typeFDWT,	//материнский вейвлет
			m_config.levelFDWT);	//уровень вейвлет-разложения
	}
	return ret;
}
//------------------------------------------------------------------------------
long CFalseEventWavelet::FalseEvent(
	const float *pDataX, const float *pDataY, const float *pDataZ,
	const DWORD number)
{
	if ((pDataX == nullptr) || (pDataY == nullptr) || (pDataZ == nullptr))
		return ErrorCodes::Err_NULL_Pointer;
	if (number == 0)
		return ErrorCodes::InvalidParams;

	m_result.result = 0;
	m_result.value = 0.f;
	m_result.valueSNR = 0;

	long ret = (
		m_saXYZ[0].ReSize(number) &&
		m_saXYZ[1].ReSize(number) &&
		m_saXYZ[2].ReSize(number)) ?
		ErrorCodes::NoError : ErrorCodes::Err_Create_Array;

	if (ret == 0)
	{
		if (FiltrFDWT(pDataX, pDataY, pDataZ,
			number, m_sizePreHistory, &nTp))
		{
			int numNotZero(0);
			for (int i = nTp; i < (int)number; ++i)
			{
				if ((m_saXYZ[0].pData[i] != 0.f) ||
					(m_saXYZ[1].pData[i] != 0.f) ||
					(m_saXYZ[2].pData[i] != 0.f))
					++numNotZero;
			}
			m_result.value = (float)(numNotZero * 100) / (float)((int)number - m_sizePreHistory);
			m_result.result = m_result.value >= m_config.thresholdPercent;
			m_result.valueSNR = m_result.value / m_config.thresholdPercent;
		}
		else
		{
			ret = ErrorCodes::Err_Calc;
			nTp = 0;
		}
	}
	return ret;
}
//------------------------------------------------------------------------------
}//namespace seismo
}//namespace dsp
}//namespace algorithm
}//namespace zet
//------------------------------------------------------------------------------