/*	Класс для работы со структурами, которыми обмениваются скада-компоненты.
	Обеспечивает чтение и запись структур.
*/

#pragma once

#include <list>
#include "Stream.h"
#include <stdint.h>
#include <mutex>
#include <zet_struct1.h>
#include <ZetLogger/include/ZetLogger.h>
#include <Buffer\SmartArray.h>

typedef std::mutex Syncro;

static Syncro WriteQueueMutex;

// заголовок структуры для файла-индекса
struct ZetIndex
{
	uint32_t stype;		// тип структуры
	uint32_t offset;	// положение структуры в файле записи
	double dtime;		// время создания структуры
};

// TODO : стоит заменить ZetBuffer на ZetIndex
struct SFile
{
	ZetBuffer index;
	ZetBuffer buffer;
};

// TODO : перевести весь интерфейс на ZetHeader
class CStructureKeeper
{
public:

	enum mode
	{
		write,
		read
	};

	CStructureKeeper();
	~CStructureKeeper();

	long OpenStream(const wchar_t* name, mode m = write);		// открыть поток для непрерывного чтения или записи
	long CloseStream();

	void PushStructure(ZetHeader* newStruct);					// добавляем структуру в буффер для записи
	void PopStructure();										// вытаскиваем пурвую структуру из буффера записи
	bool CopyBuffer();											// копировать данные из листа в буффер для записи
	bool WriteLastStructure(const wchar_t* name);				// перезаписать в файл последнюю структуру
	//long WriteStructuresInLog(CLogger* pLogger);				// записать все структуры в логгер

	ZetBuffer* GetStructure();
	ZetBuffer* GetIndex();
	
	long ReadStructureByBuffer(ZetBuffer *buffer);
	long ReadStructureByIndex(SFile *data);

protected:

private:
	std::list<SFile> m_pStructures;
	std::list<ZetBuffer> m_pDescriptors;

	ZetBuffer m_WriteBuffer;	// буфер данных подготовленный для записи
	ZetBuffer m_IndexBuffer;	// буфер индексов подготовленный для записи

	bool m_bThreadWork;

	unsigned long m_lOffsetCounter;

	CStream m_FileStream;
	CStream m_IndexStream;

	SmartArray<wchar_t> m_saIndexName;
//	wchar_t* m_sIndexName;
	wchar_t * GetIndexFileName(const wchar_t * filename);

};

