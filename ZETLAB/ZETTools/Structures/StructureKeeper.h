/*	����� ��� ������ �� �����������, �������� ������������ �����-����������.
	������������ ������ � ������ ��������.
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

// ��������� ��������� ��� �����-�������
struct ZetIndex
{
	uint32_t stype;		// ��� ���������
	uint32_t offset;	// ��������� ��������� � ����� ������
	double dtime;		// ����� �������� ���������
};

// TODO : ����� �������� ZetBuffer �� ZetIndex
struct SFile
{
	ZetBuffer index;
	ZetBuffer buffer;
};

// TODO : ��������� ���� ��������� �� ZetHeader
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

	long OpenStream(const wchar_t* name, mode m = write);		// ������� ����� ��� ������������ ������ ��� ������
	long CloseStream();

	void PushStructure(ZetHeader* newStruct);					// ��������� ��������� � ������ ��� ������
	void PopStructure();										// ����������� ������ ��������� �� ������� ������
	bool CopyBuffer();											// ���������� ������ �� ����� � ������ ��� ������
	bool WriteLastStructure(const wchar_t* name);				// ������������ � ���� ��������� ���������
	//long WriteStructuresInLog(CLogger* pLogger);				// �������� ��� ��������� � ������

	ZetBuffer* GetStructure();
	ZetBuffer* GetIndex();
	
	long ReadStructureByBuffer(ZetBuffer *buffer);
	long ReadStructureByIndex(SFile *data);

protected:

private:
	std::list<SFile> m_pStructures;
	std::list<ZetBuffer> m_pDescriptors;

	ZetBuffer m_WriteBuffer;	// ����� ������ �������������� ��� ������
	ZetBuffer m_IndexBuffer;	// ����� �������� �������������� ��� ������

	bool m_bThreadWork;

	unsigned long m_lOffsetCounter;

	CStream m_FileStream;
	CStream m_IndexStream;

	SmartArray<wchar_t> m_saIndexName;
//	wchar_t* m_sIndexName;
	wchar_t * GetIndexFileName(const wchar_t * filename);

};

