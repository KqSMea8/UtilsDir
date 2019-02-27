#pragma once

#ifndef zet7xxx_singleton_h_
#define zet7xxx_singleton_h_

#include <ZETSimple\include\singleton\singleton.h>

#include <boost\noncopyable.hpp>

class CZet7xxxSingletonImpl : private boost::noncopyable
{
public:
	CZet7xxxSingletonImpl();
	~CZet7xxxSingletonImpl();

	bool m_bStopDataStream;
	bool m_bEventLog;
	bool m_bNoStorage;
	bool m_bNoPinger;
};

typedef CSingleton<CZet7xxxSingletonImpl> CZet7xxxSingleton;

#endif //zet7xxx_singleton_h_

