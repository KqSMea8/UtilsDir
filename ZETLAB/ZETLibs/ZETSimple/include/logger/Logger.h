#ifndef logger_h_
#define logger_h_

#include <string>

class CLogger
{
public:
	CLogger(_In_ const std::wstring& sFileName);
	~CLogger();

	void write(_In_ const std::wstring& sText);

protected:
	std::wstring m_sFileName;

private:
	std::wstring getTimestamp();

	FILE* m_pFile;
};

#endif // logger_h_