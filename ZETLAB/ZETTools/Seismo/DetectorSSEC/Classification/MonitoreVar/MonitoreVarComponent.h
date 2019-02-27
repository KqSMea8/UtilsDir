#pragma once
#include "Seismo\FalseEvent\0_Custom\CustomFalseEvent.h"
/*	јлогритм контрол€ посто€нной составл€ющей

*/
namespace zet
{
namespace algorithm
{
namespace dsp
{
struct MonitoreVarComponentPrm
{
	long HoldTime;	/**< врем€ удержани€ ложного событи€ */
};

class CMonitoreVarComponent : public CCustomFalseEvent
{
public:
	CMonitoreVarComponent();
	CMonitoreVarComponent(const CMonitoreVarComponent* pMVC);
	~CMonitoreVarComponent();
	/** функци€ возвращает указатель на структуру
	*	@return указатль на структуру
	*/
	virtual PVOID GetParam();
	/** функци€ установки параметров
	*	@return код ошибки
	*	@param указатель на структуру параметров
	*/
	virtual long SetParam(const PVOID pParam);
	/** функци€ расчета
	*	@return код ошибки
	*	@param указатель на массив
	*	@param размер массива
	*	@param флаг указывающий на результат
	*/
	virtual long FalseEvent(const PVOID pData, const DWORD number, bool &bFalseEvent);
	/** функци€ установки максимального и минимального порогов
	*	@param минимальный порог
	*/
	void SetMinThreshold(const float min);
	/** функци€ установки максимального и минимального порогов
	*	@param максимальный порог
	*/
	void SetMaxThreshold(const float max);
	/** функци€ сброса класса алгоритма
	*/
	virtual ErrorCode ReStart();
protected:
	MonitoreVarComponentPrm m_Prm;		/**< структура парметров  */
	bool m_bEventFlag;					/**< флаг указывающий на ложное событие  */
	DWORD m_size;						/**< размер массивов дл€ выделни€ пам€ти */
	float m_MiddleVal;					/**< посто€нна€ составл€юща€ сигнала по оси*/
	float* m_pP;						/**< указатель на массив со значени€ми сигнала по оси */
	float m_min;						/**< минимальный порог по оси */
	float m_max;						/**< максимальный порог по оси */
	/** функци€ выдел€юща пам€ть под массивы если требуетс€
	*	@param указатель на массив
	*	@param размер массива
	*/
	void SetMemory(const PVOID pD, const DWORD size);
};
}
}
}

