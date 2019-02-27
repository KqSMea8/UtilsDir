#pragma once

#ifndef thread_module_h_
#define thread_module_h_

#include <boost\noncopyable.hpp>

#include "base\Types.h"

class CThreadModule : private boost::noncopyable
{
public:
	CThreadModule();
	virtual ~CThreadModule();

protected:
	void startThread();
	void stopThread();

	bool isThread() const;

	virtual DWORD threadFunction() = 0;

	uint32_t m_stopTime;

private:
	static DWORD WINAPI thread(_In_ LPVOID pParam);

	HANDLE m_hThread;
	bool m_bThread;
};

#endif //thread_module_h_

