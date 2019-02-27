#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <winsock.h>
#include <iphlpapi.h>
#include <afxwin.h>
#include <afxtempl.h>
//#include <string>
//using namespace std;

#define OPC_QUALITY_GOOD		0xc0
#define OPC_QUALITY_BAD			0x00
#define OPC_QUALITY_UNCERTAIN	0x40

enum TDataQuality {dqUncertain = OPC_QUALITY_UNCERTAIN, dqGood = OPC_QUALITY_GOOD, dqBad = OPC_QUALITY_BAD};
enum TDevStatus   {dsDisconnected = 0, dsStarted = 1, dsConnected = 2};
enum TDataType    {dtNone = 0, dtInt = 1, dtFloat = 2, dtDouble = 3};
enum TAccessType  {atR = 0, atW = 1,  atRW = 2};

struct TDataInf
{ 
	SYSTEMTIME   TimeStamp;	
	TDataType    Type; 
	TAccessType  Access;
	TDataQuality Quality;
	bool         Dynamic;
};

struct TIntData
{
	int      Val;
	TDataInf Inf;
};

struct TFloatData
{
	float    Val;
	TDataInf Inf;
};

struct TDoubleData
{
	double   Val;
	TDataInf Inf;
};

class CZetbusClient
{
public:	
	TIntData     IntIn[512];
	TFloatData   FloatIn[512];
	TDoubleData  DoubleIn[512];
	TIntData     IntOut[512];
	TFloatData   FloatOut[512];
	TDoubleData  DoubleOut[512];	

	int			 IntInCount;
	int			 FloatInCount;
	int			 DoubleInCount;
	int			 IntOutCount;
	int			 FloatOutCount;	
	int			 DoubleOutCount;
	
	int			 TimeOut;	
	int			 PullCount;
	int			 ReqTime;

	SOCKET		 fSck;
	CString		 fIP;
	int			 fPort;
	bool		 fConnected;
	TDevStatus   fStatus;	
	WORD		 fTransactId;
	char		 fDeviceId;
	WORD		 fProtocolId;
	char		 fTransactLen;
	WORD		 fDataLen;
	WORD		 fPackageLen;
	SYSTEMTIME	 StartTime;

	char		 fData_req[255];
	char		 fData_ans[255];

	int			 Connect();
	void		 OnConnect();
	void		 Disconnect();	
	void		 OnDisconnect();
	void		 InitData();
	int			 SendRequest(int aLen);
	int          GetAnswer();
    
	void		 SetTransactId(WORD aTransactId);
	void		 SetProtocolId(WORD aProtocolId);	
	void		 SetTransactLen(char aTransactLen); 
	void		 SetDeviceId(char aDeviceId);
	void		 SetFuncCode(char aFuncCode);
	void		 SetParmId(char aParmId);	
	void		 SetResCode(char aResCode);
	void		 SetTimeStamp(SYSTEMTIME aTimeStamp);
	void		 SetQuality(TDataQuality aQuality);
	void		 SetDynamic(bool aDynamic);
	void		 SetDataType(TDataType aDataType);	
	void		 SetDataIndex(WORD aDataIndex);
	void		 SetIntData(int aData);
	void		 SetDoubleData(double aData);
	void		 SetFloatData(float aData);	

	WORD		 GetTransactId(char* aBuff);
	WORD		 GetProtocolId(char* aBuff);	
	char		 GetTransactLen(char* aBuff); 
	char		 GetDeviceId(char* aBuff);
	char		 GetFuncCode(char* aBuff);	
	char		 GetParmId(char* aBuff);
	char	     GetResCode(char* aBuff);
	SYSTEMTIME   GetTimeStamp(char* aBuff);
	TDataQuality GetQuality(char* aBuff);
	bool         GetDinamic(char* aBuff);
	TDataType    GetDataType(char* aBuff);	
	WORD         GetDataIndex(char* aBuff);
	int          GetIntData(char* aBuff);
	double       GetDoubleData(char* aBuff);
	float        GetFloatData(char* aBuff);	

public:	
	CZetbusClient();
	~CZetbusClient(); 

	int			 Start(CString aIP, int aDeviceId);
	void		 Stop();
	bool		 IsConnected();	
	
	TIntData     GetIntIn(int aIndex);
	TFloatData   GetFloatIn(int aIndex);
	TDoubleData  GetDoubleIn(int aIndex);		
	void         SetIntOut(int aIndex, int aVal,       SYSTEMTIME aTimeStamp, TDataQuality aQual);
	void         SetFloatOut(int aIndex, float aVal,   SYSTEMTIME aTimeStamp, TDataQuality aQual);
	void         SetDoubleOut(int aIndex, double aVal, SYSTEMTIME aTimeStamp, TDataQuality aQual);

	void		 ReadIntIn(int aIndex);
	void		 ReadFloatIn(int aIndex);
	void		 ReadDoubleIn(int aIndex);
	void		 WriteIntOut(int aIndex);
	void		 WriteFloatOut(int aIndex);
	void		 WriteDoubleOut(int aIndex);

	void		 SetIntInCount(int aIntInCount);
	void		 SetFloatInCount(int aFloatInCount);
	void		 SetDoubleInCount(int aDoubleInCount);
	void		 SetIntOutCount(int aIntOutCount);
	void		 SetFloatOutCount(int aFloatOutCount);
	void		 SetDoubleOutCount(int aDoubleOutCount);	

	int  		 GetIntInCount();
	int 		 GetFloatInCount();
	int 		 GetDoubleInCount();
	int 		 GetIntOutCount();
	int 		 GetFloatOutCount();
	int 		 GetDoubleOutCount();
};

extern CZetbusClient *ZetbusClient;