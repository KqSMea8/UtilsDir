#include "GPSLogWriter.h"

#include <sstream>
#include <iomanip>

#include <ZETPath\include\ZETPath.h>
#include <ZETFile\include\ZETFile.h>

CGpsLogWriter::CGpsLogWriter(const uint32_t type, const uint64_t serial)
: m_type(type)
, m_serial(serial)
{
	m_sSignalDirectory = zpGetDirSignal();
	int a = 0;
}

CGpsLogWriter::~CGpsLogWriter()
{

}

uint32_t CGpsLogWriter::write(CZetTime& t, char* pBuffer, const uint32_t size)
{
	std::wostringstream woss;
	woss << m_sSignalDirectory;
	
	uint16_t year(0);
	if (0 != t.GetYear(year, false))
		return 1;
	woss << std::setfill(L'0') << std::setw(4) << year << L"\\";

	uint16_t month(0);
	if (0 != t.GetMonth(month, false))
		return 1;
	woss << std::setfill(L'0') << std::setw(2) << month << L"\\";

	uint16_t day(0);
	if (0 != t.GetDay(day, false))
		return 1;
	woss << std::setfill(L'0') << std::setw(2) << day << L"\\";

	uint16_t hour(0);
	if (0 != t.GetHour(hour, false))
		return 1;
	woss << std::setfill(L'0') << std::setw(2) << hour << L"\\";
	woss << L"gps_" << m_type << L"_" << m_serial << L".log";
	
	auto sFile = woss.str();

	if (0 != zfCreatePath(sFile.c_str()))
		return 1;

	FILE* pFile(nullptr);
	if (0 != zfOpenFile(sFile.c_str(), L"at", &pFile) || nullptr == pFile)
		return 1;

	char* p = new char[size + 1];
	memcpy_s(p, size + 1, pBuffer, size);
	p[size] = 0;
	if (0 != zfWriteIntoFileA(pFile, p))
		zfCloseFile(pFile);
	delete[]p;
	p = nullptr;
	zfCloseFile(pFile);

	return 0;	
}
