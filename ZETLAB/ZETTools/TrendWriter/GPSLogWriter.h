#include <stdint.h>
#include <string>

#include <ZetTools\include\ZetTools.h>

class CGpsLogWriter
{
public:
	CGpsLogWriter(const uint32_t type, const uint64_t serial);
	~CGpsLogWriter();

	const uint32_t getType() const { return m_type; }
	const uint64_t getSerial() const { return m_serial; }

	uint32_t write(CZetTime& t, char* pBuffer, const uint32_t size);

protected:
	std::wstring m_sSignalDirectory;

private:
	uint32_t m_type;
	uint64_t m_serial;
};

