#include "stdafx.h"
#include "Stream.h"


CStream::CStream()
{

}

CStream::~CStream()
{

}

long CStream::OpenForWriting(const wchar_t* filename, bool append /* = true */)
{
	if (!m_pStream.is_open())
	{
		if (append)
			m_pStream.open(filename, std::fstream::binary | std::fstream::out | std::fstream::app);
		else
			m_pStream.open(filename, std::fstream::binary | std::fstream::out);
		return 0;
	}
	else
		return -1;
}

long CStream::OpenForReading(const wchar_t* filename)
{
	if (!m_pStream.is_open())
	{
		m_pStream.open(filename, std::fstream::binary | std::fstream::in);
		return 0;
	}
	else
		return -1;
}

long CStream::Close()
{
	if (m_pStream.is_open())
		m_pStream.close();
	return 0;
}

void CStream::WriteData(const ZetBuffer *buffer)
{
	if (m_pStream.is_open() && buffer != NULL)
	{
		m_pStream.write(buffer->data(), buffer->size());
		m_pStream.flush();
	}
}

long CStream::ReadData(ZetBuffer *buffer)
{
	if (!m_pStream.is_open())
		return -1;

	if (m_pStream.eof())
		return -2;

	buffer->clear();
	buffer->resize(4);
	m_pStream.read(buffer->data(), 4);

	if (m_pStream.eof())
		return 4;

	long size = *(unsigned long*)buffer->data();
	buffer->resize(size);
	m_pStream.read(&buffer->data()[4], size - 4);

	return size;
}

long CStream::ReadData(ZetBuffer *buffer, unsigned long bytes)
{
	if (!m_pStream.is_open())
		return -1;

	if (m_pStream.eof())
		return -2;

	buffer->clear();
	buffer->resize(bytes);

	m_pStream.read(buffer->data(), bytes);

	return bytes;
}

unsigned long CStream::GetOffset()
{
	if (m_pStream.flags() | std::ios_base::in)
		return (unsigned long)m_pStream.tellg();
	if (m_pStream.flags() | std::ios_base::out)
		return (unsigned long)m_pStream.tellp();

	return 0;
}

