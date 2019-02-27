//------------------------------------------------------------------------------
//	���� CustomAlgorithm.cpp
//------------------------------------------------------------------------------
#ifndef _WIN32_WINNT
#ifndef _WIN32_WINNT_MAXVER
#include <winsdkver.h>
#endif
#define _WIN32_WINNT _WIN32_WINNT_MAXVER
#endif
//------------------------------------------------------------------------------
#include "CustomAlgorithm.h"
//------------------------------------------------------------------------------
namespace zet {
namespace algorithm {
namespace dsp {
namespace seismo {
//------------------------------------------------------------------------------
CCustomAlgorithm::CCustomAlgorithm()
	: m_pConfig(nullptr)
	, m_pResult(nullptr)
{}
//------------------------------------------------------------------------------
CCustomAlgorithm::CCustomAlgorithm(const CCustomAlgorithm & val)
{
	m_pConfig = val.m_pConfig;
	m_pResult = val.m_pResult;
}
//------------------------------------------------------------------------------
void CCustomAlgorithm::operator = (const CCustomAlgorithm & val)
{
	m_pConfig = val.m_pConfig;
	m_pResult = val.m_pResult;
}
//------------------------------------------------------------------------------
/* ����������� �������. � ��������������� ������� ����������:
	- �������� ��� ������� � ����� �������� ������������ ���������;
	- ��������� ���������� � Proxy-����� ���������� � �����
		�������� �������� ���� �����;
	- ��������� (��� � ����������� ������) �� Proxy-������ ���������� �
		cfg-��������� ����������;
	- ���������� CloseStructure cfg-��������� ����������;
	- �������������� ��������� ��������� (���� ��� ����).
*/
long CCustomAlgorithm::SetConfig(ZetHeader* pConfig, const DATE timeSRV)
{
	return ztIsStructureValid(pConfig);
}
//------------------------------------------------------------------------------
}//namespace seismo
}//namespace dsp
}//namespace algorithm
}//namespace zet
//------------------------------------------------------------------------------