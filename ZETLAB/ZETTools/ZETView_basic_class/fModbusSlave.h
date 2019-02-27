//---------------------------------------------------------------------------
#ifndef TModbusSlaveH
#define TModbusSlaveH
//---------------------------------------------------------------------------
#include "stdio.h"
#include "winsock.h"
#include "iphlpapi.h"
#include "afxwin.h"
#include "afxtempl.h"
#include <fstream>
#include <iostream>

using namespace std;

//---------------------------------------------------------------------------
#define    OPC_QUALITY_BAD             0x00
#define    OPC_QUALITY_UNCERTAIN       0x40
#define    OPC_QUALITY_GOOD            0xC0

enum TDataQuality {dqUncertain = OPC_QUALITY_UNCERTAIN, dqGood = OPC_QUALITY_GOOD, dqBad = OPC_QUALITY_BAD};

struct TSocketDef
{
	HANDLE      ThreadHandle;
	DWORD       ThreadId;
	SOCKET      SocketHandle;
	sockaddr_in Addr;	
	int         IP;
	INT_PTR     ID;
	bool        Enabled;
	SYSTEMTIME  TimeOfRecv;
	char	    RECV_BUF[256];    
    char	    SEND_BUF[256]; 
};

class TModbusSlave
{
public:
	char	 fAddrBuff[240]; // 120 слов
public:
	TModbusSlave();
	~TModbusSlave();
	void	 ServerStart();
	void	 ServerStop();
	void	 WriteBuff(int aIndex, char aVal);
	char     ReadBuff(int aIndex);
	int  	 SetIntTag(int aAddr, int aVal);
	int  	 GetIntTag(int aAddr);
	int 	 SetFloatTag(int aAddr, float aVal);
	float	 GetFloatTag(int aAddr);
	int 	 SetDoubleTag(int aAddr, double aVal);
	double   GetDoubleTag(int aAddr);
};

extern TModbusSlave *mb;

#endif
