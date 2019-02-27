#pragma once
#include "base\thread_module\thread_module.h"
#include "work\MasterWork.h"

#include <boost\noncopyable.hpp>

class CScanner : protected CThreadModule
{
public:
	CScanner();
	virtual ~CScanner();

	virtual void start() = 0;
	virtual void stop() = 0;

	virtual void addMasterWork(_In_ const std::pair<ZET7XXX_DEVICE_TYPE, CMasterWork*>& masterWork) = 0;
	virtual void removeMasterWork(ZET7XXX_DEVICE_TYPE type) = 0;

protected:
	bool isAvailableType(_In_ const ZET7XXX_DEVICE_TYPE type);

	template<class _Fn1>
	inline _Fn1 for_each_master_work(_Fn1 _Func)
	{
		return std::for_each(m_masterWorks.begin(), m_masterWorks.end(), _Func);
	}

	std::map<ZET7XXX_DEVICE_TYPE, CMasterWork*> m_masterWorks;
	std::vector<ZET7XXX_DEVICE_TYPE> m_availableTypes;

private:
};

