#pragma once

#include "opccomn.h"
#include "opcda.h"
#include "OpcEnum.h"

#include <algorithm>
#include <vector>

#define OPC_DO_NOTHING	(WM_USER + 1000)
#define OPC_CHECK_WRITE	(WM_USER + 1001)
#define OPC_READ		(WM_USER + 1002)

class OPCDataCallback;

struct OpcTagInfo
{
	OpcTagInfo()
		: fullPath(_T(""))
		, wQuality(OPC_QUALITY_UNCERTAIN)
		, hWndInput(NULL)

		, previosHost(-1)
		, previosServer(-1)
		, previosTag(-1)

		, last(TRUE)
		, stop(FALSE)

		, hClient(0)
		, hServer(0)
		, toWrite(FALSE)
		, alwaysWrite(FALSE)
	{
		VariantInit( &vValue );
		//bValue		  = 0;
		//lValue		  = 0;
		//fValue		  = 0.0;
	};

	OpcTagInfo( CString fp, HWND h = NULL )
	{
		fullPath	= fp;
		VariantInit( &vValue );
		wQuality	  = OPC_QUALITY_UNCERTAIN;
		hWndInput	  = h;

		previosHost   = -1;
		previosServer = -1;
		previosTag    = -1;

		last          = FALSE;
		stop          = FALSE;

		hClient		  = 0;
		hServer		  = 0;
		
		//bValue		  = 0;
		//lValue		  = 0;
		//fValue		  = 0.0;
		toWrite		  = FALSE;
		alwaysWrite   = FALSE;
	}

	~OpcTagInfo()
	{
		VariantClear( &vValue );
	};

	// used for std::find
	bool operator==( const OpcTagInfo& t2 )
	{
		return (this->fullPath == t2.fullPath);
	};

	bool operator==( const OPCHANDLE& t2 )
	{
		return (this->hClient == t2);
	};

	// generic information
	CString		fullPath;
	VARIANT		vValue;
	WORD		wQuality;
	FILETIME	timeStamp;

	// ZETView specific information
	HWND		hWndInput;
	LONG	    previosHost;
	LONG	    previosServer;
	LONG	    previosTag;

	void		*hosts_;
	
	BOOL		last;
	BOOL		stop;

	// OPC specific information
	OPCHANDLE	hClient;
	OPCHANDLE	hServer;

	// to remove
	//BYTE		bValue;
	//LONG		lValue;
	//FLOAT		fValue;
	BOOL		toWrite;
	BOOL		alwaysWrite;
};

typedef std::vector<OpcTagInfo> OPCTagList;
typedef std::vector<OpcTagInfo*> OPCTagListP;

struct OpcTagToWrite
{
	OpcTagToWrite( OPCTagListP* tags_ , bool bAll)
	{
		tags = tags_;
		all = bAll;
	};

	void operator()( OpcTagInfo& t2 )
	{
		if (all || t2.toWrite || t2.alwaysWrite)
			tags->push_back( &t2 );
	};

	OPCTagListP* tags;
	bool all;
};

struct OpcTagClearWrite
{
	void operator()( OpcTagInfo& t2 )
	{
		t2.toWrite = FALSE;
	};
};
///////////////////////////////////////////////////////////////////////////////

//extern
struct OpcHostInfo;

struct OpcServerInfo
{
	OpcServerInfo()
		: progID(_T(""))
		, bConnected(FALSE)
		, hClient(1)
		, ipServer(nullptr)
		, ipGroup(nullptr)
		, hGroup(0)
		, ipCP(nullptr)
		, dwAdvise(0)
		, ipCallback(nullptr)
		, ipSyncIO(nullptr)
		, ipSyncIO2(nullptr)
		, pHost(nullptr)
	{
	}

	OpcServerInfo( CString p )
	{
		progID		= p;
		bConnected	= FALSE;
		hClient		= 1;
		ipServer	= nullptr;
		ipGroup		= nullptr;
		hGroup		= 0;
		ipCP		= nullptr;
		dwAdvise	= 0;
		ipCallback	= nullptr;
		ipSyncIO	= nullptr;
		ipSyncIO2	= nullptr;
		pHost		= nullptr;

	}

	~OpcServerInfo()
	{
	}

	// used for std::find
	bool operator==( const OpcServerInfo& s2 )
	{
		return (this->progID == s2.progID);
	};

	// used to delete server from specific host
	//void SetHostPointer( OpcHostInfo* host ) { pHost = host; };

	// generic information
	CString				progID;
	BOOL				bConnected;

	// OPC specific information
	OPCHANDLE			hClient;
	IOPCServer*			ipServer;
	IOPCItemMgt*		ipGroup;
	OPCHANDLE			hGroup;
	IConnectionPoint*	ipCP;
	DWORD				dwAdvise;
	OPCDataCallback*	ipCallback;
	IOPCSyncIO*			ipSyncIO;
	IOPCSyncIO2*		ipSyncIO2;

	// Server tags
	OPCTagList			tags;
	// not available tags
	OPCTagList			na_tags;

	// pointer to parent host
	OpcHostInfo*		pHost;	// used to delete server from specific host

	HRESULT             oldStatus;
};

typedef std::vector<OpcServerInfo> OPCServerList;
///////////////////////////////////////////////////////////////////////////////

struct OpcHostInfo
{
	OpcHostInfo()
		: host(_T(""))
		, hThread(INVALID_HANDLE_VALUE)
		, dwThread(0)
		, oldPing(OLE_E_NOCACHE)
	{

	}

	OpcHostInfo( CString h )
	{
		host		= h;
		hThread		= INVALID_HANDLE_VALUE;
		dwThread	= 0;
		oldPing		= OLE_E_NOCACHE;
	}

	~OpcHostInfo()
	{
		if (INVALID_HANDLE_VALUE != hThread)
		{
#pragma warning(suppress: 6258)
			::TerminateThread( hThread, 0 );
			::CloseHandle( hThread );
		}
	}

	// used for std::find
	bool operator==( const OpcHostInfo& h2 )
	{
		return (this->host == h2.host);
	};

	// generic information
	CString host;

	// Thread
	HANDLE				hThread;
	DWORD				dwThread;

	// OPC servers
	OPCServerList		servers;

    // old Ping
	HRESULT				oldPing;
};

typedef std::vector<OpcHostInfo> OPCHostList;
///////////////////////////////////////////////////////////////////////////////

UINT Opc_Start( LPVOID lParam );
void Opc_Stop();
void Opc_GetTags( OPCHostList& hosts_ );

void Opc_Initialize( OpcHostInfo* pHost );
HRESULT Opc_Connect( CString host, CString progID );
HRESULT PingRemoteHost( CString host );
HRESULT InstancePingRemoteHost( const CString& host );
void Opc_Disconnect( OpcServerInfo* pServer );

#define null_( it )\
	it.end()

void AddMessage(long eventType, long eventID, LPCTSTR message);
CString ObtainError( DWORD dwErrorMsgId );

inline void freeMemory(OPCSERVERSTATUS** pServerStatus)
{
	OPCSERVERSTATUS* p = *pServerStatus;
	if (nullptr != p)
	{
		if (nullptr != p->szVendorInfo)
			CoTaskMemFree(p->szVendorInfo);
		CoTaskMemFree(p);
		p = nullptr;
	}
}
