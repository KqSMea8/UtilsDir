/*  ласс дл€ сохранени€ в данных в непрерывном потоке.
*/
#pragma once
#include <fstream>
#include <vector>

typedef char ZetByte;
typedef std::vector<ZetByte> ZetBuffer;

class CStream
{
public:
	CStream();
	~CStream();

	long OpenForWriting(const wchar_t* filename, bool append = true);
	long OpenForReading(const wchar_t* filename);
	long Close();

	void WriteData(const ZetBuffer *buffer);
	long ReadData(ZetBuffer *buffer);
	long ReadData(ZetBuffer *buffer, unsigned long bytes);

	unsigned long GetOffset();

private:
	std::fstream m_pStream;
};

