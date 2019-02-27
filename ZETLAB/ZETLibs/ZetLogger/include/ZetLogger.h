//------------------------------------------------------------------------------
/*	������ ������ �������� - �������������.
Ÿ ���� - �� API-�������� �������������� ����������������� ������
*/
//------------------------------------------------------------------------------
#pragma once
//------------------------------------------------------------------------------
// �������� ��������������� ������� ������
enum LoggerThreadID
{
	tw_Struct,			// Thread Write Struct
	tw_Register,		// Thread Write Register
	tr_Struct,			// Thread Read Struct
	tr_Register,		// Thread Read Register
};
//------------------------------------------------------------------------------
#include "CustomLogger.h"
//------------------------------------------------------------------------------
class ZETLOGGER_API CLogger : public CCustomLogger
{
private:
public:
	// ����������� ������. ���������:
	// loggerName - ��� ������������ ���������� ������, ������������ ��� ������ � OutputDebugString;
	// fileName - ������ ��� ����� (����� ��� ���������� ����� ".sqlite");
	// password - ������ ��� ������� � ����� �����.
	CLogger(
		const std::string loggerName = "",
		const std::string fileName = "",
		const std::string password = "");

	virtual ~CLogger();

	// ��������� ����� ����� ��
	long GetFileName(std::string & fileName);

	// ������� ����� ����� �� ��� ������ �� ���� �������� (� ������������ � ����)
	long SetFileNameForRead(std::string & fileName);

	// ������� ����� ����� �� ��� ������ � ���� �������� (� ������������ � ����)
	long SetFileNameForWrite(std::string & fileName);

	// ������ � �� �������� ������ ��������, ���������������
	// ��� ������ ������ ��������.
	// ������������ ��������:
	// ������������� �������� - ��� ������;
	// 0 - ����� �������� � ��������� � ������� ������� �����,
	//     � ������� ����� �� ����� ���������� ����� �������� � ��
	long Register(const std::vector<ZetHeader*> & descriptors);

	// ������ � �� ����� ���������.
	// ������������ ��������:
	// ������������� �������� - ��� ������;
	// 0 - ��������� ����������� � ���������, �� ���������� ��������
	//		��� ��������� �� ���� � ������� ������ ����� �������� � ��.
	long Write(const ZetHeader & rHeader);

	// ������ �� ����� �� ��������, ��������������� �������� ��������.
	// ������ - ��� ������ � �������, ����������� ";". � ����� ����";".
	// ����� ����� �������� � ����������������� ���� (0�0hhhh ��� 0X0HHHH).
	// ������ ����� "%Y-%m-%d %H:%M:%S"
	// ������������ ��������:
	// ������������� �������� - ��� ������;
	// 0 - ������� ������� ����� ������, ��������� ����� ����� �������� �
	//     ������� ������� GetReadResult().
	long Read(
		ReadDataType * pData,
		const std::string timeFrom = "",
		const std::string timeTo = "",
		const std::string filterTypeStructure = "",
		const std::string filterProgramID = "",
		const std::string filterProgramNumber = "",
		const std::string filterModuleID = "",
		const std::string filterModuleNumber = "");

	// ��������� ���������� ������ �������� ������ ������.
	// ������������ ��������:
	// ������������� �������� - ��� ������;
	//-23 - ����� �� �������;
	// 0 - ����� �������� ���� ������;
	// 5 - ����� ������� ������� � �����;
	// 7 - �������� ���������� ������;
	// 8 - ���������� ������ � �������.
	StatusThread GetReadResult(LoggerThreadID id = tr_Struct);

	// ������ �� �� �������� ������ ��������.
	// ������������ ��������:
	// ������������� �������� - ��� ������;
	// 0 - ������� ������� ����� ������, ��������� ����� ����� �������� �
	//     ������� ������� GetReadResult().
	long ReadRegister(ReadDataRegisterType & descriptors);
};
//------------------------------------------------------------------------------