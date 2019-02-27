#include "stdafx.h"
#include "fZetbusClient.h"

  CZetbusClient *ZetbusClient;
//--------------------------------------------------------------------------//
  HANDLE hSendThread;
  HANDLE hGetThread;

  bool SendDataThreadEnabled = false;
  bool GetDataThreadEnabled  = false;

  UINT SendDataThread(LPVOID lpParam);
  UINT GetDataThread(LPVOID lpParam);

//--------------------------------------------------------------------------//
  void ByteReverse(char& X, char& Y)
  {
	  X=X^Y;
	  Y=X^Y;
	  X=X^Y;
  }

//--------------------------------------------------------------------------//

/////////////////////////////////////////////////////////////////////////////
//	CONSTRUCTOR  ////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////

//--------------------------------------------------------------------------//
 
const char PARM_WRITE       = 0x01;
const char PARM_READ        = 0x02;

const char FUNC_PARM        = 0x11;
const char FUNC_SETTINGS    = 0xD1;
const char RES_OK			= 0x00;
const char RES_ERROR		= 0x00;

const WORD ZETBUS_PROTOCOL  = 0x000A;
const WORD TRANSACT_LEN     = 31;

const char TRANSACT_ID_INC  = 0;
const char PROTOCOL_ID_INC  = 2;
const char TRANSACT_LEN_INC = 4;
const char DEVICE_ID_INC    = 5;
const char FUNC_CODE_INC    = 6;
const char PARM_ID_INC      = 7;
const char RES_CODE_INC     = 8;
const char TIME_STAMP_INC   = 9;
const char QUALITY_INC      = 23;
const char DYNAMIC_INC      = 24;
const char DATA_TYPE_INC    = 25;
const char DATA_INDEX_INC   = 26;
const char DATA_INC         = 28;

//---------------------------------------------------------------------------
CZetbusClient::CZetbusClient()
{
	//SendTo = new CSendTo;	

	IntInCount     = 0;
	FloatInCount   = 0;	
	DoubleInCount  = 0;
	IntOutCount    = 0;
	FloatOutCount  = 0;		
	DoubleOutCount = 0;

	TimeOut		   = 0;
	PullCount	   = 0;
	ReqTime		   = 0;

	fTransactId	   = 0;	
	fProtocolId	   = ZETBUS_PROTOCOL;
	fTransactLen   = TRANSACT_LEN;	
	
	fConnected	   = true;	
}

//--------------------------------------------------------------------------
CZetbusClient::~CZetbusClient()
{  
	SendDataThreadEnabled = false;
	GetDataThreadEnabled  = false;
	//delete SendTo;
}

//-------------------------------------------------------------------------//

/////////////////////////////////////////////////////////////////////////////
//	CONNECT  ////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
//-------------------------------------------------------------------------//
int CZetbusClient::Start(CString aIP, int aDeviceId)
{
	WSADATA      Wsa;
	if(WSAStartup(0x0101,&Wsa)) 
	{
		//SendTo->MsgLog("cannot init winsock");  
		return -1;
	}

	fIP	      = aIP;
	fDeviceId = aDeviceId;
	fPort	  = 502;	

	GetSystemTime(&StartTime);

	InitData();
	
	SendDataThreadEnabled = true;
	GetDataThreadEnabled  = true;

	hSendThread = CreateThread( NULL, 0, (LPTHREAD_START_ROUTINE)SendDataThread, 0, 0, NULL);
	hGetThread  = CreateThread( NULL, 0, (LPTHREAD_START_ROUTINE)GetDataThread, 0, 0, NULL);

	return 1;
}

//---------------------------------------------------------------------------
void CZetbusClient::Stop()
{
	Disconnect();
}

//---------------------------------------------------------------------------
int CZetbusClient::Connect()
{	
	sockaddr_in  addr;	

	addr.sin_family		= PF_INET;
	addr.sin_addr.s_addr= INADDR_ANY;
	addr.sin_port		= htons(fPort);  

	fSck = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP); 	
	if (fSck==SOCKET_ERROR)
	{
		closesocket(fSck);
		fSck=0;
		//SendTo->MsgLog("create socket error");
		return -1;
	}

	const char   optval   = 1;
	if(setsockopt(fSck, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(optval)) == SOCKET_ERROR)
	{
		//SendTo->MsgLog("setsockopt error");	
		return -1;
	}
	    
    sockaddr_in  addr_srv;

	addr_srv.sin_family = PF_INET;
	addr_srv.sin_port   = htons(fPort);
	addr_srv.sin_addr.S_un.S_addr = inet_addr(CStringA(fIP).GetBuffer());

	if (connect(fSck, (sockaddr *)&addr_srv, sizeof(addr_srv)) == SOCKET_ERROR)
	{
		closesocket(fSck);
		fSck=0;
		//SendTo->MsgLog("cannot connect socket");
		return -1;
	}
	fConnected = true;	
	return 1;
}


//---------------------------------------------------------------------------
void CZetbusClient::Disconnect()
{
	SendDataThreadEnabled = false;
	GetDataThreadEnabled  = false;
	TerminateThread(hSendThread, 0);
	TerminateThread(hGetThread, 0);
	OnDisconnect();
}

//---------------------------------------------------------------------------
void CZetbusClient::OnDisconnect()
{
	closesocket(fSck);
	fSck=0;
	InitData();
	//SendTo->MsgLog("socket closed");
	fConnected = false;
}

//-------------------------------------------------------------------------//

/////////////////////////////////////////////////////////////////////////////
//	SEND DATA  //////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
//-------------------------------------------------------------------------//

  UINT SendDataThread(LPVOID lpParam)
  {
	  LARGE_INTEGER tmp;
	  QueryPerformanceFrequency(&tmp);
	  __int64 f0 = tmp.QuadPart;
	  __int64 t0=0, t1=0;
	  
	  while(SendDataThreadEnabled)
	  {
		  bool wait = true;
		  QueryPerformanceCounter(&tmp);
		  t0 = tmp.QuadPart;
		  t0 = (t0/(f0/1000));

		  if(ZetbusClient->IsConnected())
		  {			 		  
			  for (int i=1; i<=ZetbusClient->GetIntOutCount(); i++)    ZetbusClient->WriteIntOut(i);			 
			  for (int i=1; i<=ZetbusClient->GetFloatOutCount(); i++)  ZetbusClient->WriteFloatOut(i);			  
			  for (int i=1; i<=ZetbusClient->GetDoubleOutCount(); i++) ZetbusClient->WriteDoubleOut(i);
		  }
		  else
		  {
			  ZetbusClient->Connect();
		  }	  
		  while(wait)
		  {
			  QueryPerformanceCounter(&tmp);
			  t1 = tmp.QuadPart; t1 = (t1/(f0/1000));
			  if ((t1 - t0) >= 1000) wait = false;
		  }
	  }
	  return 0;
 }

//--- SET PACKAGE ----------------------------------------------------------//
void   CZetbusClient::WriteIntOut(int aIndex)
{
	SetTransactId(fTransactId);
	SetProtocolId(fProtocolId);	
	SetTransactLen(fTransactLen);
	SetDeviceId(fDeviceId);
	SetFuncCode(FUNC_PARM);
	SetParmId(PARM_WRITE);
	SetResCode(RES_OK);	
	SetTimeStamp(IntOut[aIndex-1].Inf.TimeStamp);
	SetQuality(IntOut[aIndex-1].Inf.Quality);
	SetDynamic(IntOut[aIndex].Inf.Dynamic);
	SetDataType(IntOut[aIndex-1].Inf.Type);	
	SetDataIndex(aIndex-1);
	SetIntData(IntOut[aIndex-1].Val);	
	SendRequest(36);	
}

//---------------------------------------------------------------------------
void   CZetbusClient::WriteFloatOut(int aIndex)
{
	SetTransactId(fTransactId);
	SetProtocolId(fProtocolId);	
	SetTransactLen(fTransactLen);
	SetDeviceId(fDeviceId);
	SetFuncCode(FUNC_PARM);
	SetParmId(PARM_WRITE);
	SetResCode(RES_OK);	
	SetTimeStamp(FloatOut[aIndex-1].Inf.TimeStamp);
	SetQuality(FloatOut[aIndex-1].Inf.Quality);
	SetDynamic(FloatOut[aIndex].Inf.Dynamic);
	SetDataType(FloatOut[aIndex-1].Inf.Type);	
	SetDataIndex(aIndex-1);
	SetFloatData(FloatOut[aIndex-1].Val);	
	SendRequest(36);	
}

//---------------------------------------------------------------------------
void   CZetbusClient::WriteDoubleOut(int aIndex)
{
	SetTransactId(fTransactId);
	SetProtocolId(fProtocolId);	
	SetTransactLen(fTransactLen);
	SetDeviceId(fDeviceId);
	SetFuncCode(FUNC_PARM);
	SetParmId(PARM_WRITE);
	SetResCode(RES_OK);	
	SetTimeStamp(DoubleOut[aIndex-1].Inf.TimeStamp);
	SetQuality(DoubleOut[aIndex-1].Inf.Quality);
	SetDataType(DoubleOut[aIndex-1].Inf.Type);	
	SetDataIndex(aIndex-1);
	SetDoubleData(DoubleOut[aIndex-1].Val);	
	SendRequest(36);	
}

//--- TCP/IP --------------------------------------------------------------//
void CZetbusClient::SetTransactId(WORD aTransactId)
{	
	WORD* ptr = (WORD*)(fData_req + TRANSACT_ID_INC); 
	ptr[0] = aTransactId;	
	if (fTransactId==65535) fTransactId = 0;
	fTransactId++;	
}

//---------------------------------------------------------------------------
void CZetbusClient::SetProtocolId(WORD aProtocolId)
{
	WORD* ptr = (WORD*)(fData_req + PROTOCOL_ID_INC); 
	ptr[0] = aProtocolId;
}

//---------------------------------------------------------------------------
void CZetbusClient::SetTransactLen(char aTransactLen)
{
	fData_req[TRANSACT_LEN_INC] = aTransactLen;	
}

//---------------------------------------------------------------------------
void CZetbusClient::SetDeviceId(char aDeviceId)
{
	fData_req[DEVICE_ID_INC] = aDeviceId;	
}

//--- Zetbus ----------------------------------------------------------------
void CZetbusClient::SetFuncCode(char aFuncCode)
{
	fData_req[FUNC_CODE_INC] = aFuncCode;	
}

//---------------------------------------------------------------------------
void CZetbusClient::SetParmId(char aParmId)
{
    fData_req[PARM_ID_INC] = aParmId;	
}

//---------------------------------------------------------------------------
void CZetbusClient::SetResCode(char aResCode)
{
	fData_req[RES_CODE_INC] = aResCode;	
}

//---------------------------------------------------------------------------
void CZetbusClient::SetTimeStamp(SYSTEMTIME aTimeStamp)
{
	WORD Year         = aTimeStamp.wYear;
	WORD Month        = aTimeStamp.wMonth;	
	WORD Day          = aTimeStamp.wDay;
	WORD Hour         = aTimeStamp.wHour;
	WORD Minute       = aTimeStamp.wMinute;
	WORD Second       = aTimeStamp.wSecond;
	WORD Milliseconds = aTimeStamp.wMilliseconds;
		
	WORD* ptr = (WORD*)(fData_req+TIME_STAMP_INC); 
	ptr[0] = Year;
	ptr[1] = Month;
	ptr[2] = Day;
	ptr[3] = Hour;
	ptr[4] = Minute;
	ptr[5] = Second;
	ptr[6] = Milliseconds;	
}
//---------------------------------------------------------------------------
void CZetbusClient::SetQuality(TDataQuality aQuality)
{
	fData_req[QUALITY_INC] = aQuality;	
}

void CZetbusClient::SetDynamic(bool aDynamic)
{	
	if (aDynamic) fData_req[DYNAMIC_INC] = 1; 
	else fData_req[DYNAMIC_INC] = 0;		
}

//---------------------------------------------------------------------------
void CZetbusClient::SetDataType(TDataType aDataType)
{
	WORD* ptr = (WORD*)(fData_req+DATA_TYPE_INC); 
	ptr[0] = aDataType;	
}

//---------------------------------------------------------------------------
void CZetbusClient::SetDataIndex(WORD aDataIndex)
{
	WORD* ptr = (WORD*)(fData_req+DATA_INDEX_INC); 
	ptr[0] = aDataIndex;	
}

//---------------------------------------------------------------------------
void CZetbusClient::SetIntData(int aData)
{
	int* ptr = (int*)(fData_req + DATA_INC); 
	ptr[0] = aData;	
	ByteReverse(fData_req[35],fData_req[36]);
	ByteReverse(fData_req[37],fData_req[38]);	
}

//---------------------------------------------------------------------------
void CZetbusClient::SetFloatData(float aData)
{
	float* ptr = (float*)(fData_req + DATA_INC); 
	ptr[0] = aData;	
	ByteReverse(fData_req[35],fData_req[36]);
	ByteReverse(fData_req[37],fData_req[38]);	
}

//---------------------------------------------------------------------------
void CZetbusClient::SetDoubleData(double aData)
{
	double* ptr = (double*)(fData_req + DATA_INC); 
	ptr[0] = aData;	
	ByteReverse(fData_req[35],fData_req[36]);
	ByteReverse(fData_req[37],fData_req[38]);
	ByteReverse(fData_req[39],fData_req[40]);
	ByteReverse(fData_req[41],fData_req[42]);	
}

//-------------------------------------------------------------------------//
int CZetbusClient::SendRequest(int aLen)
{
	if(send(fSck, fData_req, aLen, 0) == SOCKET_ERROR)
	{
		//SendTo->MsgLog("server not respond.");
		OnDisconnect();
		return -1;
	}	
	
	//SendTo->DataLog(fData_req, 36);	

	return 1;
}

//-------------------------------------------------------------------------//

/////////////////////////////////////////////////////////////////////////////
//	GET DATA  ///////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
//-------------------------------------------------------------------------//

  UINT GetDataThread(LPVOID lpParam)
  {
	  while(GetDataThreadEnabled)
	  {
		 ZetbusClient->GetAnswer();
	  }
	  return 0;
  }

//--- GET PACKEGE ---------------------------------------------------------//
int CZetbusClient::GetAnswer()
{
	int res = recv( fSck, fData_ans, 256, 0 ); 
	if (res==SOCKET_ERROR)
	{
		//SendTo->MsgLog("server not respond.");
		OnDisconnect();
		return -1;	
	}		
	
	//SendTo->DataLog(fData_ans,31);	
	
	WORD TransactId	     = GetTransactId(fData_ans);
	WORD ProtocolId	     = GetProtocolId(fData_ans);
	char TransactLen	 = GetTransactLen(fData_ans);
	char DeviceId	     = GetDeviceId(fData_ans);
	char ResCode	     = GetResCode(fData_ans);
	SYSTEMTIME TimeStamp = GetTimeStamp(fData_ans);
	TDataQuality Quality = GetQuality(fData_ans);
	bool Dynamic         = GetDinamic(fData_ans);
	TDataType DataType   = GetDataType(fData_ans);	
	WORD DataIndex       = GetDataIndex(fData_ans);
	
	switch (DataType)
	{
		case dtInt:
		{
			int IntData					   = GetIntData(fData_ans);
			IntIn[DataIndex].Val           = IntData;
			IntIn[DataIndex].Inf.Quality   = Quality;
			IntIn[DataIndex].Inf.TimeStamp = TimeStamp;
			IntIn[DataIndex].Inf.Dynamic   = Dynamic;
			break;
		}
		case dtFloat:
		{
			float FloatData				     = GetFloatData(fData_ans);
			FloatIn[DataIndex].Val           = FloatData;
			FloatIn[DataIndex].Inf.Quality   = Quality;
			FloatIn[DataIndex].Inf.TimeStamp = TimeStamp;
			FloatIn[DataIndex].Inf.Dynamic   = Dynamic;
			break;
		}
		case dtDouble:
		{
			double DoubleData                 = GetDoubleData(fData_ans);
			DoubleIn[DataIndex].Val           = DoubleData;
			DoubleIn[DataIndex].Inf.Quality   = Quality;
			DoubleIn[DataIndex].Inf.TimeStamp = TimeStamp;
			DoubleIn[DataIndex].Inf.Dynamic   = Dynamic;
			break;
		}		
	}	
	return 1;
}

//--- TCP/IP --------------------------------------------------------------//
WORD CZetbusClient::GetTransactId(char* aBuff)
{
	WORD* ptr = (WORD*)(aBuff + TRANSACT_ID_INC); 
	WORD TransactId = ptr[0];		
	return TransactId;
}

//---------------------------------------------------------------------------
WORD CZetbusClient::GetProtocolId(char* aBuff)	
{
	WORD* ptr =(WORD*)(aBuff + PROTOCOL_ID_INC); 
	WORD ProtocolId = ptr[0];		
	return ProtocolId;
}

//---------------------------------------------------------------------------
char CZetbusClient::GetTransactLen(char* aBuff) 
{ 
	char TransactLen = aBuff[TRANSACT_LEN_INC];	
	return TransactLen;
}

//---------------------------------------------------------------------------
char CZetbusClient::GetDeviceId(char* aBuff)
{
	char DeviceId = aBuff[DEVICE_ID_INC];		
	return DeviceId;
}

//--- Zetbus ----------------------------------------------------------------
char CZetbusClient::GetFuncCode(char* aBuff)
{
	char FuncCode = aBuff[FUNC_CODE_INC];		
	return FuncCode;
}

//---------------------------------------------------------------------------
char CZetbusClient::GetParmId(char* aBuff)
{
	char ParmId = aBuff[PARM_ID_INC];		
	return ParmId;
}

//---------------------------------------------------------------------------
char CZetbusClient::GetResCode(char* aBuff)
{
	char ResCode = aBuff[0];		
	return ResCode;
}

//---------------------------------------------------------------------------
SYSTEMTIME CZetbusClient::GetTimeStamp(char* aBuff)
{
	WORD* ptr = (WORD*)(aBuff); 

	SYSTEMTIME TimeStamp;

	TimeStamp.wYear         = ptr[0];
	TimeStamp.wMonth        = ptr[1];
	TimeStamp.wDay          = ptr[2];
	TimeStamp.wHour         = ptr[3];
	TimeStamp.wMinute       = ptr[4];
	TimeStamp.wSecond       = ptr[5];
	TimeStamp.wMilliseconds = ptr[6];
	
	return TimeStamp;
}

//---------------------------------------------------------------------------
TDataQuality CZetbusClient::GetQuality(char* aBuff)
{	
	TDataQuality Quality;

	const char QUALITY_GOOD = 0xc0;
	const char QUALITY_BAD = 0x00;
	const char QUALITY_UNCERTAIN = 0x40;

	switch (aBuff[QUALITY_INC])
	{
		case QUALITY_UNCERTAIN: Quality = dqUncertain;	break;			
		case QUALITY_GOOD:      Quality = dqGood;		break;
		case QUALITY_BAD:       Quality = dqBad;		break;
		default:				Quality = dqUncertain;
	}	
	return Quality;
}

//---------------------------------------------------------------------------
bool CZetbusClient::GetDinamic(char* aBuff)
{	
	if (aBuff[DYNAMIC_INC]==1) return true;
	else return false;	
}

//---------------------------------------------------------------------------
TDataType CZetbusClient::GetDataType(char* aBuff)
{
	WORD* ptr = (WORD*)(aBuff + DATA_TYPE_INC); 
	
	TDataType DataType;	

	switch (ptr[0])
	{
		case 0x01: DataType = dtInt;    break;
		case 0x02: DataType = dtFloat;  break;
		case 0x03: DataType = dtDouble;	break;
		default:   DataType = dtNone;
	}		

	return DataType;	
}

//---------------------------------------------------------------------------
WORD CZetbusClient::GetDataIndex(char* aBuff)
{
	WORD* ptr = (WORD*)(aBuff + DATA_INDEX_INC); 
	WORD DataIndex = ptr[0];		
	return DataIndex;
}

//---------------------------------------------------------------------------
int CZetbusClient::GetIntData(char* aBuff)
{
	char buf[4];
	for(int i=0; i<4; i++) buf[i]=aBuff[i+ DATA_INC];
	ByteReverse(buf[0],buf[1]);
	ByteReverse(buf[2],buf[3]);
	int* ptr = (int*)(buf); 
	int IntData = ptr[0];		
	return IntData;
}

//---------------------------------------------------------------------------
float CZetbusClient::GetFloatData(char* aBuff)
{
	char buf[4];
	for(int i=0; i<4; i++) buf[i]=aBuff[i+ DATA_INC];
	ByteReverse(buf[0],buf[1]);
	ByteReverse(buf[2],buf[3]);
	float* ptr = (float*)(buf); 
	float FloatData = ptr[0];		
	return FloatData;
}

//---------------------------------------------------------------------------
double CZetbusClient::GetDoubleData(char* aBuff)	
{
	char buf[8];
	for(int i=0; i<8; i++) buf[i]=aBuff[i+ DATA_INC];
	ByteReverse(buf[0],buf[1]);
	ByteReverse(buf[2],buf[3]);
	ByteReverse(buf[4],buf[5]);
	ByteReverse(buf[6],buf[7]);
	double* ptr = (double*)(buf); 
	double DoubleData = ptr[0];		
	return DoubleData;
}

//-------------------------------------------------------------------------//

/////////////////////////////////////////////////////////////////////////////
//	DATA  ///////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
//-------------------------------------------------------------------------//
void CZetbusClient::InitData() 
{
	for (int i = 0; i<512; i++)
	{
		IntIn[i].Val     = 0;
		FloatIn[i].Val   = 0;
		DoubleIn[i].Val  = 0;
		IntOut[i].Val    = 0;
		FloatOut[i].Val  = 0;
		DoubleOut[i].Val = 0;
	}

	for (int i = 0; i<512; i++)
	{
		IntIn[i].Inf.Type     = dtInt;
		FloatIn[i].Inf.Type   = dtFloat;
		DoubleIn[i].Inf.Type  = dtDouble;
		IntOut[i].Inf.Type    = dtInt;
		FloatOut[i].Inf.Type  = dtFloat;
		DoubleOut[i].Inf.Type      = dtDouble;
	}
	for (int i = 0; i<512; i++)
	{
		IntIn[i].Inf.Dynamic     = false;
		FloatIn[i].Inf.Dynamic   = false;
		DoubleIn[i].Inf.Dynamic  = false;
		IntOut[i].Inf.Dynamic    = false;
		FloatOut[i].Inf.Dynamic  = false;
		DoubleOut[i].Inf.Dynamic = false;
	}
	for (int i = 0; i<512; i++)
	{
		IntIn[i].Inf.Quality     = dqUncertain;
		FloatIn[i].Inf.Quality   = dqUncertain;
		DoubleIn[i].Inf.Quality  = dqUncertain;
		IntOut[i].Inf.Quality    = dqUncertain;
		FloatOut[i].Inf.Quality  = dqUncertain;
		DoubleOut[i].Inf.Quality = dqUncertain;
	}
	for (int i = 0; i<512; i++)
	{
		IntIn[i].Inf.TimeStamp     = StartTime;
		FloatIn[i].Inf.TimeStamp   = StartTime;
		DoubleIn[i].Inf.TimeStamp  = StartTime;
		IntOut[i].Inf.TimeStamp    = StartTime;
		FloatOut[i].Inf.TimeStamp  = StartTime;
		DoubleOut[i].Inf.TimeStamp = StartTime;
	}	
}

//---------------------------------------------------------------------------
void   CZetbusClient::SetIntOut(int aIndex, int aValue, SYSTEMTIME aTimeStamp, TDataQuality aQuality)
{
	IntOut[aIndex-1].Val = aValue;
	IntOut[aIndex-1].Inf.TimeStamp = aTimeStamp;
	IntOut[aIndex-1].Inf.Quality = aQuality;	
}

//---------------------------------------------------------------------------
void   CZetbusClient::SetFloatOut(int aIndex, float aValue, SYSTEMTIME aTimeStamp, TDataQuality aQuality)
{
	FloatOut[aIndex-1].Val = aValue;
	FloatOut[aIndex-1].Inf.TimeStamp = aTimeStamp;
	FloatOut[aIndex-1].Inf.Quality = aQuality;	
}

//---------------------------------------------------------------------------
void   CZetbusClient::SetDoubleOut(int aIndex, double aValue, SYSTEMTIME aTimeStamp, TDataQuality aQuality)
{
	DoubleOut[aIndex-1].Val = aValue;
	DoubleOut[aIndex-1].Inf.TimeStamp = aTimeStamp;
	DoubleOut[aIndex-1].Inf.Quality = aQuality;	
}

//---------------------------------------------------------------------------
TIntData  CZetbusClient::GetIntIn(int aIndex)
{
	return IntIn[aIndex-1];
}

//---------------------------------------------------------------------------
TFloatData  CZetbusClient::GetFloatIn(int aIndex)
{
	return FloatIn[aIndex-1];
}

//---------------------------------------------------------------------------
TDoubleData CZetbusClient::GetDoubleIn(int aIndex)
{
	return DoubleIn[aIndex-1];
}

//---------------------------------------------------------------------------
void CZetbusClient::SetIntInCount(int aIntInCount)
{
	IntInCount = aIntInCount;
}

//---------------------------------------------------------------------------
void CZetbusClient:: SetFloatInCount(int aFloatInCount)
{
	FloatInCount = aFloatInCount;
}

//---------------------------------------------------------------------------
void CZetbusClient:: SetDoubleInCount(int aDoubleInCount)
{
	DoubleInCount = aDoubleInCount;
}

//---------------------------------------------------------------------------
void CZetbusClient:: SetIntOutCount(int aIntOutCount)
{
	IntOutCount = aIntOutCount;
}

//---------------------------------------------------------------------------
void CZetbusClient:: SetFloatOutCount(int aFloatOutCount)
{
	FloatOutCount = aFloatOutCount;
}

//---------------------------------------------------------------------------
void CZetbusClient:: SetDoubleOutCount(int aDoubleOutCount)
{
	DoubleOutCount = aDoubleOutCount;
}

//---------------------------------------------------------------------------
int CZetbusClient::GetIntInCount()
{
	return IntInCount;	
}

//---------------------------------------------------------------------------
int CZetbusClient::GetFloatInCount()
{
	return FloatInCount;	
}

//---------------------------------------------------------------------------
int CZetbusClient::GetDoubleInCount()
{
	return DoubleInCount;	
}

//---------------------------------------------------------------------------
int CZetbusClient::GetIntOutCount()
{
	return IntOutCount;
}

//---------------------------------------------------------------------------
int CZetbusClient::GetFloatOutCount()
{
	return FloatOutCount;
}

//---------------------------------------------------------------------------
int CZetbusClient::GetDoubleOutCount()
{
	return DoubleOutCount;
}

//---------------------------------------------------------------------------
bool CZetbusClient::IsConnected()
{
	return fConnected;
}

//-------------------------------------------------------------------------//
/////////////////////////////////////////////////////////////////////////////

