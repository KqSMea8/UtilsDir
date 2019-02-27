#pragma once
//#include "ZetViewMessage.h"
/////////////////////////////////////////////////////////////////////////////

// The following are message definitions.
//
//  Values are 32 bit values layed out as follows:
//
//   3 3 2 2 2 2 2 2 2 2 2 2 1 1 1 1 1 1 1 1 1 1
//   1 0 9 8 7 6 5 4 3 2 1 0 9 8 7 6 5 4 3 2 1 0 9 8 7 6 5 4 3 2 1 0
//  +---+-+-+-----------------------+-------------------------------+
//  |Sev|C|R|     Facility          |               Code            |
//  +---+-+-+-----------------------+-------------------------------+
//
//  where
//
//      Sev - is the severity code
//
//          00 - Success
//          01 - Informational
//          10 - Warning
//          11 - Error
//
//      C - is the Customer code flag
//      R - is a reserved bit
//
//      Facility - is the facility code
//      Code - is the facility's status code
//
// Define the facility codes
//
#define FACILITY_SYSTEM                  0x0
#define FACILITY_STUBS                   0x3
#define FACILITY_RUNTIME                 0x2
#define FACILITY_IO_ERROR_CODE           0x4
//
// Define the severity codes
//
#define STATUS_SEVERITY_WARNING          0x2
#define STATUS_SEVERITY_SUCCESS          0x0
#define STATUS_SEVERITY_INFORMATIONAL    0x1
#define STATUS_SEVERITY_ERROR            0x3

//////////////////////////////////////////////////////////////////
// MessageId: MSG_CONFIG_ERROR
// MessageText:
// ������ ������������, ������� ������ ���������
#define MSG_CONFIG_ERROR                 ((DWORD)0xC0020004L)

// MessageId: MSG_NOT_CONNECTED
// MessageText:
// ������ (����� ���� ��� �����������, ���� � ����������), �������������� ������� ���������� �� ���������
#define MSG_NOT_CONNECTED                ((DWORD)0xC0020008L)

// MessageId: MSG_DEVICE_FAILURE
// MessageText:
// �������� ������, �������������� ������� ����������
#define MSG_DEVICE_FAILURE               ((DWORD)0xC002000CL)

// MessageId: MSG_SENSOR_FAILURE
// MessageText:
// �������� �������� ����������
#define MSG_SENSOR_FAILURE               ((DWORD)0xC0020010L)

// MessageId: MSG_LAST_KNOWN
// MessageText:
// ����� � �������, �������������� ������� ����������, ��������
#define MSG_LAST_KNOWN                   ((DWORD)0xC0020014L)

// MessageId: MSG_COMM_FAILURE
// MessageText:
// �� ������� ��������� � �������, �������������� ������� ����������
#define MSG_COMM_FAILURE                 ((DWORD)0xC0020018L)

// MessageId: MSG_OUT_OF_SERVICE
// MessageText:
// �� �������������� ������� (���������, �� � �.�.)
#define MSG_OUT_OF_SERVICE               ((DWORD)0xC002001CL)

// MessageId: MSG_LAST_USABLE/
// MessageText: ��� ������
// 
#define MSG_LAST_USABLE                  ((DWORD)0x80020044L)

// MessageId: MSG_SENSOR_NOT_ACCURATE
// MessageText:
// �������� ����������� ���������
#define MSG_SENSOR_NOT_ACCURATE          ((DWORD)0x80020050L)

// MessageId: MSG_EGU_ACCEEDED
// MessageText:
// �������� ����� �� �������
#define MSG_EGU_ACCEEDED                 ((DWORD)0x80020054L)

// MessageId: MSG_SUB_NORMAL
// MessageText: ��� ������
// 
#define MSG_SUB_NORMAL                   ((DWORD)0x80020058L)

// MessageId: MSG_GOOD
// MessageText:
// ������
#define MSG_GOOD                         ((DWORD)0x000200D8L)

///////////////////////////////////////////////////////////////////
// �������������� ����������� ������ �������� IDispatch, ��������� ��� ������ Microsoft Visual C++

// ����������. �� ��������� ���������� ����� ������. ���� ���� ����� ����� �������� ������ ��� ������
//  Microsoft Visual C++, ���� ��������� ����� ������������.

/////////////////////////////////////////////////////////////////////////////
enum LOCALSTRINGNUMBER
{
    SERVER_CONNECTION_ERROR = 1,   	//101 ������ ��� ����������� � ������� ������
    READING_REGISTRY_ERROR = 2,    	//102 ������ ��� ������ ������ �� �������
    CONFIG_FILE_NOT_AVAILABLE = 3, 	//103 ���� ������������ � ����� DirConfig ����������
    HELP_FILE_MISSING = 4,	   	//104 ���� ������� �����������
    DIRHELP_NOT_AVAILABLE = 5,	   	//105 ����� DirHelp ����������
    DIRSIGNAL_NOT_AVAILABLE = 6,   	//106 ����� DirSignal ����������
    DIRRESULT_NOT_AVAILABLE = 7,   	//107 ����� DirResult ����������
    DIRCORRECT_NOT_AVAILABLE = 8,  	//108 ����� DirCorrect ����������
    INSTALLLOCATION_NOT_AVAILABLE = 9,  //109 ����� InstallLocation ����������
    AUTOSCALE_CREATION_ERROR = 10,      //110 ������ ��� �������� ���������� ������ CAutoScaleXY
    UNIT_CONNECTION_ERROR = 11,         //111 ������ ��� ����������� � Unit
    PROGRAM_RUNNING_FROM_UNIT = 12,     //112 ��������� �������� ����� Unit
    PROGRAM_RUNNING_FROM_PANEL = 13,    //113 ��������� �������� �� Z-������
    PROGRAM_START_WORKING = 14,         //114 ��������� ������ ��������
    SAMPLING_FREQUENCY = 15,		//115 ������� ������������� ��� ��
    ERROR_CODE = 16,			//116 ��� ������
    WORKING_CHANNELS_MISSING = 17,      //117 ��� ������� ������� ������� ������. ��������� �� ����� ���������
    HELP_FILE_CALLING_ERROR = 18,       //118 ������ ��� ������ ������� ���������
    PROGRAM_FINISH_WORKING = 19,        //119 ��������� ��������� ���� ������
	ERROR_READ_DATA = 20,           //120 ��� ������ ������ � ������ ��������� ������
	ERROR_CALC_DATA = 21,           //121 ��� ��������� ������ ��������� ������
	END_JOB_SRV = 22,		//122 ��� ������� ������
	PROGRAM_FINISH = 23,		//123 ��������� ����� �������
	PARAM_FILE_CALLING_ERROR,       //124 ������ ��� ������� ��������� ��������� ����������
};
/////////////////////////////////////////////////////////////////////////////
// ����� �������� CDWindowslog

class CDWindowslog : public CWnd
{
protected:
	DECLARE_DYNCREATE(CDWindowslog)
public:
	CLSID const& GetClsid()
	{
		static CLSID const clsid
			= { 0xD1394A2C, 0xFEB, 0x46AC, { 0x80, 0x58, 0xB7, 0xB2, 0xBC, 0xD1, 0xAC, 0x5E } };
		return clsid;
	}
	virtual BOOL Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle,
						const RECT& rect, CWnd* pParentWnd, UINT nID, 
						CCreateContext* pContext = NULL)
	{ 
		return CreateControl(GetClsid(), lpszWindowName, dwStyle, rect, pParentWnd, nID); 
	}

    BOOL Create(LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, 
				UINT nID, CFile* pPersist = NULL, BOOL bStorage = FALSE,
				BSTR bstrLicKey = NULL)
	{ 
		return CreateControl(GetClsid(), lpszWindowName, dwStyle, rect, pParentWnd, nID,
		pPersist, bStorage, bstrLicKey); 
	}

// ��������
//public:

// ��������
public:

// _DZetWindowsLog

// Functions
//

	void AboutBox()
	{
		InvokeHelper(DISPID_ABOUTBOX, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
	}
	void AddMessage(long eventType, long eventID, LPCTSTR message)
	{
		static BYTE parms[] = VTS_I4 VTS_I4 VTS_BSTR ;
		InvokeHelper(0x1, DISPATCH_METHOD, VT_EMPTY, NULL, parms, eventType, eventID, message);
	}
	void Read()
	{
		InvokeHelper(0x2, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
	}
	void SetResultDir(LPCTSTR dirResult)
	{
		static BYTE parms[] = VTS_BSTR ;
		InvokeHelper(0x3, DISPATCH_METHOD, VT_EMPTY, NULL, parms, dirResult);
	}
	CString LocalString(long number)
	{
		CString result;
		static BYTE parms[] = VTS_I4 ;
		InvokeHelper(0x4, DISPATCH_METHOD, VT_BSTR, (void*)&result, parms, number);
		return result;
	}

// Properties
//



};
