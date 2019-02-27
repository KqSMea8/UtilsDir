 // ***** Sample.mc *****
 // This is the header section.
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
//
//      R - is a reserved bit
//
//      Facility - is the facility code
//
//      Code - is the facility's status code
//
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


//
// MessageId: MSG_CONFIG_ERROR
//
// MessageText:
//
//  �訡�� ���䨣��樨, ����୮ ������ ��ࠬ����. %r
//  %1 
//
#define MSG_CONFIG_ERROR                 ((DWORD)0xC0020004L)

//
// MessageId: MSG_NOT_CONNECTED
//
// MessageText:
//
//  ����� (����� ���� ��� �ணࠬ���, ⠪ � �������), ���ᯥ稢��騩 ����稥 ���ଠ樨 �� ������祭. %r
//  %1
//
#define MSG_NOT_CONNECTED                ((DWORD)0xC0020008L)

//
// MessageId: MSG_DEVICE_FAILURE
//
// MessageText:
//
//  �ᯮ�祭 ����� ���ᯥ稢��騩 ����稥 ���ଠ樨. %r
//  %1
//
#define MSG_DEVICE_FAILURE               ((DWORD)0xC002000CL)

//
// MessageId: MSG_SENSOR_FAILURE
//
// MessageText:
//
//  �ᯮ�祭 ���筨� ���ଠ樨. %r
//  %1
//
#define MSG_SENSOR_FAILURE               ((DWORD)0xC0020010L)

//
// MessageId: MSG_LAST_KNOWN
//
// MessageText:
//
//  C��� � ���㫥�, ���ᯥ稢��騬 ����稥 ���ଠ樨, ����ﭠ. %r
//  %1
//
#define MSG_LAST_KNOWN                   ((DWORD)0xC0020014L)

//
// MessageId: MSG_COMM_FAILURE
//
// MessageText:
//
//  �� 㤠���� �易���� � ���㫥�, ���ᯥ稢��騬 ����稥 ���ଠ樨. %r
//  %1
//
#define MSG_COMM_FAILURE                 ((DWORD)0xC0020018L)

//
// MessageId: MSG_OUT_OF_SERVICE
//
// MessageText:
//
//  �� �����ন������ (���ᨥ� �ணࠬ��, ���ᨥ� �� � �.�.). %r
//  %1
//
#define MSG_OUT_OF_SERVICE               ((DWORD)0xC002001CL)

//
// MessageId: MSG_LAST_USABLE
//
// MessageText:
//
//  %1
//
#define MSG_LAST_USABLE                  ((DWORD)0x80020044L)

//
// MessageId: MSG_SENSOR_NOT_ACCURATE
//
// MessageText:
//
//  ���祭�� �����⢥��� ���������. %r
//  %1
//
#define MSG_SENSOR_NOT_ACCURATE          ((DWORD)0x80020050L)

//
// MessageId: MSG_EGU_ACCEEDED
//
// MessageText:
//
//  ���祭�� ��諮 �� �।���. %r
//  %1
//
#define MSG_EGU_ACCEEDED                 ((DWORD)0x80020054L)

//
// MessageId: MSG_SUB_NORMAL
//
// MessageText:
//
//  %1
//
#define MSG_SUB_NORMAL                   ((DWORD)0x80020058L)

//
// MessageId: MSG_GOOD
//
// MessageText:
//
//  ����. %r
//  %1
//
#define MSG_GOOD                         ((DWORD)0x000200D8L)

