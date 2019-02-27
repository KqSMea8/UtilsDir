#include "stdafx.h"
#include "StructureKeeper.h"


CStructureKeeper::CStructureKeeper()
	: m_bThreadWork(false)
	, m_lOffsetCounter(0)
{
	
}

CStructureKeeper::~CStructureKeeper()
{
}

long CStructureKeeper::OpenStream(const wchar_t* name, mode m /* = write */)
{
	if (wcslen(name) == 0)
		return -1;

	switch (m)
	{
	case CStructureKeeper::write:
		m_FileStream.OpenForWriting(name);
		m_IndexStream.OpenForWriting(GetIndexFileName(name));
		break;
	case CStructureKeeper::read:
		m_FileStream.OpenForReading(name);
		m_IndexStream.OpenForReading(GetIndexFileName(name));
		break;
	}

	return 0;
}

long CStructureKeeper::CloseStream()
{
	m_lOffsetCounter = 0;
	return m_FileStream.Close() & m_IndexStream.Close();
}

void CStructureKeeper::PushStructure(ZetHeader* newStruct)
{
	SFile str;

	// копируем данные в список буферов
	str.buffer.resize(newStruct->size);
	memcpy_s(str.buffer.data(), newStruct->size, (void*)newStruct, newStruct->size);

	// заполняем индекс структур
	ZetIndex tmpIndex;
	tmpIndex.stype = newStruct->type;
	tmpIndex.offset = m_lOffsetCounter;
	tmpIndex.dtime = 0;				// TODO : время задаём по текущему времени сервера
	str.index.resize(sizeof(ZetIndex));
	memcpy_s(str.index.data(), sizeof(ZetIndex), &tmpIndex, sizeof(ZetIndex));

	try 
	{
		std::lock_guard<Syncro> guard(WriteQueueMutex);
		m_pStructures.push_back(str);
		// увеличиваем счётчик смещения
		m_lOffsetCounter += newStruct->size;
	}
	catch (std::exception &e)
	{
		e = e;
	}
}

void CStructureKeeper::PopStructure()
{
	if (!m_pStructures.empty())
	{
		m_pStructures.erase(m_pStructures.begin());
	}
}

ZetBuffer* CStructureKeeper::GetStructure()
{
	if (m_pStructures.empty())
		return NULL;
	else
		return &m_pStructures.front().buffer;
}

ZetBuffer* CStructureKeeper::GetIndex()
{
	if (m_pStructures.empty())
		return NULL;
	else
		return &m_pStructures.front().index;
}

bool CStructureKeeper::CopyBuffer()
{
	bool bRes = false;

	try 
	{
		long lTotalSize = 0;
		long lIndexSize = 0;
		{
			std::lock_guard<Syncro> guard(WriteQueueMutex);

			if (!m_pStructures.empty())
			{
				long size;
				for (auto iter : m_pStructures)
				{
					size = *(long*)(iter.buffer.data());
					lTotalSize += size;
					lIndexSize += sizeof(ZetIndex);
				}
				if (lTotalSize > 0)
				{
					m_WriteBuffer.resize(lTotalSize);
					m_IndexBuffer.resize(lIndexSize);
					lTotalSize = 0;
					lIndexSize = 0;
					for (auto iter : m_pStructures)
					{
						size = *(long*)(iter.buffer.data());
						memcpy_s(m_WriteBuffer.data() + lTotalSize, size, iter.buffer.data(), size);
						lTotalSize += size;
						memcpy_s(m_IndexBuffer.data() + lIndexSize, sizeof(ZetIndex), iter.index.data(), sizeof(ZetIndex));
						lIndexSize += sizeof(ZetIndex);
					}	
				}
				bRes = true;
				m_pStructures.clear();
			}

		}
		// после освобожения листа можно вызвать операцию записи
		if (lTotalSize > 0)
			m_FileStream.WriteData(&m_WriteBuffer);
		if (lIndexSize > 0)
			m_IndexStream.WriteData(&m_IndexBuffer);
	}
	catch (std::exception &e) 
	{
		e = e;
	}
	return bRes;
}

bool CStructureKeeper::WriteLastStructure(const wchar_t* name)
{
	if (m_pStructures.empty())
		return false;

	try
	{ 
		m_FileStream.OpenForWriting(name, false);
		m_FileStream.WriteData(&m_pStructures.back().buffer);
		m_FileStream.Close();
	}
	catch (std::exception &e)
	{
		e = e;
	}

	m_pStructures.clear();

	return true;
}

/*long CStructureKeeper::WriteStructuresInLog(CLogger* pLogger)
{
	long writes = 0;
	for (auto iter : m_pStructures)
	{
		if (pLogger->Write(*(ZetHeader*)iter.buffer.data()) == 0)
			writes++;
	}
	if (!m_pStructures.empty())
		m_pStructures.clear();
	return writes;
}*/

long CStructureKeeper::ReadStructureByBuffer(ZetBuffer *buffer)
{
	m_FileStream.ReadData(buffer);

	if (buffer->empty())
		return 0;

	long size = *(long*)(buffer->data());
	if (size < 16 || size > 65536 || size != buffer->size())
		return NULL;

	return size;
}

long CStructureKeeper::ReadStructureByIndex(SFile *data)
{
	long size = m_IndexStream.ReadData(&data->index, sizeof(ZetIndex));
	if (size != sizeof(ZetIndex))
		return NULL;

	size = m_FileStream.ReadData(&data->buffer);
	size = *(long*)(data->buffer.data());
	if (size < 16 || size > 65536 || size != data->buffer.size())
		return NULL;	

	return size;
}

wchar_t * CStructureKeeper::GetIndexFileName(const wchar_t * filename)
{
	if (m_saIndexName.ReSize((DWORD)(wcslen(filename) + 5)))
	{
		wcscpy_s(m_saIndexName.pData, m_saIndexName.size, filename);
		wcscat_s(m_saIndexName.pData, m_saIndexName.size, L".idx");
	}
	else
		m_saIndexName.Zero();

	return m_saIndexName.pData;
}