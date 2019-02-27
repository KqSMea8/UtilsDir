#ifndef circ_buff_basic_zet_logger
#define circ_buff_basic_zet_logger

#include <zet_struct1.h>
#include <ZetErrorCodes.h>

#define ZETLOGGER_MAXIMUM_STRUCTURE_LENGTH 16384	// ������������ ����� ���������
#define ZETLOGGER_CIRC_BUFF_SIZE 65536	// ������ �������� �� ������� ������ ������ � ���� �� ZETLOGGER_CIRC_BUFF_SIZE/2 ����

// ��������� ����������� ����������� ����� ��� �������� ������
struct CircBuffBasicZetLoggerStruct
{
	char _buffer[ZETLOGGER_CIRC_BUFF_SIZE];		// ���������� ����� ������������ ������ ��� �������� ��������

	long _posWriteCircBuff;		// ��������� �� ������� ������ ������������ ������
	long _posReadCircBuff;		// ��������� �� ������� ������ ������������ ������
};

// ������ ����� ��������� ����������� ����� �������� ������ � ����������
class CCircBuffBasicZetLogger
{
private:
	CircBuffBasicZetLoggerStruct* _circBuffStruct;	// ��������� �� ��������� ����������� ����������� ����� (������� ����� ��������� ��� ������������� Shared Memory)

public:
	CCircBuffBasicZetLogger();
	CCircBuffBasicZetLogger(CircBuffBasicZetLoggerStruct* circBuffStruct_);
	~CCircBuffBasicZetLogger();
	
	// �������� ����������������������� ������ (������ ���� ��������� �� ��������� � ������� �� ������ ��������)
	// ������������ ��������: true - ����� ������������������; false - ����� �� ������������������
	bool isValid();
	// ����� ���������� ������������ ������ � ��������� ���������
	void reset();

	// ������ ������ � ����������� �����
	// ������������ ��������: >=0 - ���������� ���������� ������; <0 - ����� �� ������������������
	long writeStructures(char* structures_, long size_);
	// ������ ������ �� ������������ ������
	// ������������ ��������: >0 - ���������� ��������� ������, <0 - ����� �� ������������������
	long readStructures(char* structures_, long size_);
	// ������ ������ �� ������������ ������ �� ��������� ����� (��� ������ ��������� ������)
	// ������������ ��������:  >0 - ���������� ��������� ������, <0 - ����� �� ������������������
	long readTempStructures(char* structures_, long size_);
	
	// ����������� ���������� ����� ������������ ������ ��� ������ ������
	// ������������ ��������: ������ ���������� ����� ������������ ������ ��� ������ ������
	long getSizeWrite();
	// ����������� ������� ���������� ������ � ����������� ������ ��� ������
	// ������������ ��������: ������ ���������� ������
	long getSizeRead();
};

#endif