//------------------------------------------------------------------------------
//	Файл CustomAlgorithm.h
//------------------------------------------------------------------------------
/*		Базовый класс для классов, реализующих алгоритмы СКСВ.
Создавать экземпляры этого класса нецелесообразно.
*/
//------------------------------------------------------------------------------
#pragma once
#include <Seismo/Seismo.h>
#include <ZetTools\include\ZetTools.h>
//------------------------------------------------------------------------------
//using namespace zet::configuration::schema;
//struct ZetHeader;
//------------------------------------------------------------------------------
namespace zet {
namespace algorithm	{
namespace dsp {
namespace seismo {
//------------------------------------------------------------------------------
class CCustomAlgorithm
{
private:
protected:
	ZetHeader* m_pConfig;	// конфигурационные параметры алгоритма
	ZetHeader* m_pResult;	// результат работы алгоритма

public:
	CCustomAlgorithm();
	CCustomAlgorithm(const CCustomAlgorithm & val);
	virtual ~CCustomAlgorithm() {}
	void operator = (const CCustomAlgorithm & val);

	// параметры класса
	ZetHeader* GetConfig() { return m_pConfig; }
	ZetHeader* GetResult() { return m_pResult; }

	virtual long SetConfig(ZetHeader* pConfig, const DATE timeSRV = 0);
};
//------------------------------------------------------------------------------
}//namespace seismo
}//namespace dsp
}//namespace algorithm
}//namespace zet
//------------------------------------------------------------------------------
