#pragma once
#include "Seismo\FalseEvent\0_Custom\CustomFalseEvent.h"
/*	�������� �������� ���������� ������������

*/
namespace zet
{
namespace algorithm
{
namespace dsp
{
struct MonitoreVarComponentPrm
{
	long HoldTime;	/**< ����� ��������� ������� ������� */
};

class CMonitoreVarComponent : public CCustomFalseEvent
{
public:
	CMonitoreVarComponent();
	CMonitoreVarComponent(const CMonitoreVarComponent* pMVC);
	~CMonitoreVarComponent();
	/** ������� ���������� ��������� �� ���������
	*	@return �������� �� ���������
	*/
	virtual PVOID GetParam();
	/** ������� ��������� ����������
	*	@return ��� ������
	*	@param ��������� �� ��������� ����������
	*/
	virtual long SetParam(const PVOID pParam);
	/** ������� �������
	*	@return ��� ������
	*	@param ��������� �� ������
	*	@param ������ �������
	*	@param ���� ����������� �� ���������
	*/
	virtual long FalseEvent(const PVOID pData, const DWORD number, bool &bFalseEvent);
	/** ������� ��������� ������������� � ������������ �������
	*	@param ����������� �����
	*/
	void SetMinThreshold(const float min);
	/** ������� ��������� ������������� � ������������ �������
	*	@param ������������ �����
	*/
	void SetMaxThreshold(const float max);
	/** ������� ������ ������ ���������
	*/
	virtual ErrorCode ReStart();
protected:
	MonitoreVarComponentPrm m_Prm;		/**< ��������� ���������  */
	bool m_bEventFlag;					/**< ���� ����������� �� ������ �������  */
	DWORD m_size;						/**< ������ �������� ��� �������� ������ */
	float m_MiddleVal;					/**< ���������� ������������ ������� �� ���*/
	float* m_pP;						/**< ��������� �� ������ �� ���������� ������� �� ��� */
	float m_min;						/**< ����������� ����� �� ��� */
	float m_max;						/**< ������������ ����� �� ��� */
	/** ������� ��������� ������ ��� ������� ���� ���������
	*	@param ��������� �� ������
	*	@param ������ �������
	*/
	void SetMemory(const PVOID pD, const DWORD size);
};
}
}
}

