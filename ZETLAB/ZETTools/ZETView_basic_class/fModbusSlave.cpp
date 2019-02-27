//---------------------------------------------------------------------------v.1012
#include "stdafx.h"
#include "fModbusSlave.h"

//---------------------------------------------------------------------------
  TModbusSlave* mb(nullptr);
  TSocketDef*   fServer;
  CPtrArray     fClientList; 
  bool m_bQuit = false;

//--------------------------------------------------------------------------- 
  VOID WAIT_CONNECTION(LPVOID lParam); 
  VOID WAIT_REQUEST(LPVOID lParam); 
  void ON_GET_REQUEST(TSocketDef* aClDef);

//---------------------------------------------------------------------------
  UINT_PTR   nTimerID; 

  void  OnTimer()
  {
	  SYSTEMTIME CurrentTime;
	  GetLocalTime(&CurrentTime);
	  if(CurrentTime.wSecond > 55) return;
	  TSocketDef *cl_def;
	  for(int i = 0; i < fClientList.GetSize(); i++)
	  {
		  cl_def = (TSocketDef*)fClientList.GetAt(i);
		  if(abs(CurrentTime.wSecond - cl_def->TimeOfRecv.wSecond)>3) 
		  {
			  if (cl_def->Enabled) closesocket(cl_def->SocketHandle);
		  }
	  }	
  }

//--------------------------------------------------------------------------- 
  void ByteReverse(char& X, char& Y)
  {
	  X=X^Y;	
	  Y=X^Y;
	  X=X^Y; 
  }

//---------------------------------------------------------------------------
  //ofstream  log("log1.txt");

  void ToTimeLog()
  {
	 /*SYSTEMTIME CurrentTime;
	 GetLocalTime(&CurrentTime);	 
	 log<<"["<<CurrentTime.wMinute<< ".";
     if (CurrentTime.wSecond <10) log<<"0";
     log<<CurrentTime.wSecond<< ".";
	 if (CurrentTime.wMilliseconds <10) log<<"00";
	 else if (CurrentTime.wMilliseconds <100) log<<"0";
	 log<<CurrentTime.wMilliseconds<<"]";*/
  }

  void ToMsgLog(TSocketDef *aDef, char* aStr)
  {
	 /*ToTimeLog();
	 if (aDef!=NULL) log<<"["<<aDef->ThreadHandle << "_" << aDef->ThreadId <<"]";
	 log<<aStr<<"\n";*/	 
  }

  void ToDataLog(TSocketDef *aDef, char* aStr, char* aBuff, int aSize)
  {
	 /*ToTimeLog();
	 if (aDef!=NULL) log<<"["<<aDef->ThreadHandle << "_" << aDef->ThreadId <<"]";
     log<<aStr;
	 for (int i=0; i<aSize; i++) log<<(int)aBuff[i]<<" ";          
	 log<<"\n";*/
  }
 
   

//---------------------------------------------------------------------------

  TModbusSlave::TModbusSlave()
//	  : m_bStop(false)
  {
  }

	TModbusSlave::~TModbusSlave()
	  {
	  }

void TModbusSlave::ServerStart()
{
	if (fServer!= NULL) 
	{
		ToMsgLog(NULL, " server not stoped, can't start new server");
		return;	
	}		
	
	fServer = new TSocketDef;	

	WSADATA Wsa;	
	if(WSAStartup(0x0101,&Wsa))
	{
		ToMsgLog(NULL, " wsastartup error");
		return;
	}	
	
	sockaddr_in Addr;
	Addr.sin_family=PF_INET;
	Addr.sin_addr.s_addr=INADDR_ANY;
	Addr.sin_port=htons(502);
	
	fServer->SocketHandle = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	
	const char optval = 1;	
	if(setsockopt(fServer->SocketHandle, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(optval))==SOCKET_ERROR)
	{
		ToMsgLog(NULL, " setsockopt");	
		return;
	}	
	if (bind(fServer->SocketHandle, (sockaddr*)&Addr, sizeof(Addr))==SOCKET_ERROR)
	{
		closesocket(fServer->SocketHandle);
		fServer->SocketHandle=0;
		ToMsgLog(NULL, " bind error");
		return;
	}	
	if (listen(fServer->SocketHandle, SOMAXCONN)==SOCKET_ERROR)
	{
		closesocket(fServer->SocketHandle);
		fServer->SocketHandle=0;
		ToMsgLog(NULL, " listen error");
		return;
	}	

	for(int i = 0; i < 240; i++) fAddrBuff[i] = 0;

	fServer->ThreadHandle = CreateThread( NULL, 0, (LPTHREAD_START_ROUTINE)WAIT_CONNECTION, 0, 0, &fServer->ThreadId);

	nTimerID = SetTimer(NULL, 1, 3000, (TIMERPROC)OnTimer);	
} 

//---------------------------------------------------------------------------
void TModbusSlave::ServerStop()
{
	TSocketDef *cl_def;
//	fServer->SocketHandle = 0;
	closesocket(fServer->SocketHandle);
	for(int i = 0; i < fClientList.GetSize(); i++)
	{
		cl_def = (TSocketDef*)fClientList.GetAt(i);
	    cl_def->Enabled = false;
		closesocket(cl_def->SocketHandle);
	}
	fServer->SocketHandle = 0;
	//fServer->SocketHandle = 0;
	
	//if (cl_def->ThreadHandle != NULL)
	//	TerminateProcess(cl_def->ThreadHandle,NO_ERROR); // убрать процесс

	m_bQuit = true;
	KillTimer(NULL, 1);


	delete fServer;
	fServer = nullptr;
}

//---------------------------------------------------------------------------

VOID WAIT_CONNECTION(LPVOID lParam)
{	
	ToMsgLog(fServer, " server started, waiting connection...");	   
	while(fServer->SocketHandle!=0 || m_bQuit == false)
	{			
		TSocketDef* cl_def = new TSocketDef; 		
				   
		cl_def->SocketHandle = accept(fServer->SocketHandle,(sockaddr*)&cl_def->Addr, 0);
		
		if (fServer->SocketHandle!=0) 
		{
			cl_def->IP           = cl_def->Addr.sin_addr.S_un.S_addr;
			cl_def->ID           = fClientList.GetSize();
			cl_def->Enabled      = true;	
			if (m_bQuit)
				break;
			cl_def->ThreadHandle = CreateThread( NULL, 0, (LPTHREAD_START_ROUTINE)WAIT_REQUEST, &cl_def->ID, 0, &cl_def->ThreadId);
			fClientList.Add(cl_def);	
		}
	} 	
	ToMsgLog(fServer, " server stoped");
}

//---------------------------------------------------------------------------
VOID WAIT_REQUEST(LPVOID lParam)
{	
	TSocketDef *cl_def = (TSocketDef*)fClientList.GetAt(((int*)lParam)[0]);
	ToMsgLog(cl_def, " client connected");
	while(cl_def->Enabled)
	{
		int res = recv( cl_def->SocketHandle,cl_def->RECV_BUF, 256, 0 ); 
		if (res!=SOCKET_ERROR)
		{
			GetLocalTime(&cl_def->TimeOfRecv);
			ToDataLog(cl_def, " <---", cl_def->RECV_BUF, res);	
			ON_GET_REQUEST(cl_def);		
		}
		else
		{	 
			ToMsgLog(cl_def, " <--- socket error");			
			cl_def->Enabled = false;	       
		}
	} 
	ToMsgLog(cl_def, " client disconeccted");			    
	closesocket(cl_def->SocketHandle);
}

//---------------------------------------------------------------------------
void ON_GET_REQUEST(TSocketDef* aClDef)
{		
	switch (aClDef->RECV_BUF[7])//Func_Code
	{
	case(0x04): //Read Input Registers
		{
			aClDef->SEND_BUF[0]  = aClDef->RECV_BUF[0];      // ID
			aClDef->SEND_BUF[1]  = aClDef->RECV_BUF[1];      // ID
			aClDef->SEND_BUF[2]  = aClDef->RECV_BUF[2];      // MB
			aClDef->SEND_BUF[3]  = aClDef->RECV_BUF[3];      // MB
			aClDef->SEND_BUF[4]  = 0;                        // Len
			aClDef->SEND_BUF[5]  = aClDef->RECV_BUF[11]*2+3; // Len(byte)
			aClDef->SEND_BUF[6]  = aClDef->RECV_BUF[6];      // Dev_Addr
			aClDef->SEND_BUF[7]  = aClDef->RECV_BUF[7];      // Func_Code
			aClDef->SEND_BUF[8]  = aClDef->RECV_BUF[11];     // Len(word)
			if (mb != NULL) 
			for (int i = 0; i < aClDef->RECV_BUF[11]*2; i++)  // Value
				aClDef->SEND_BUF[9 + i ] = mb->fAddrBuff[aClDef->RECV_BUF[9]*2 + i];
			send(aClDef->SocketHandle, aClDef->SEND_BUF, aClDef->RECV_BUF[11]*2 + 9, 0);
			ToDataLog(aClDef, " --->", aClDef->SEND_BUF, aClDef->RECV_BUF[11]*2 + 9);  
			break;
		}
		case(0x03): //Read Holding Registers
		{
			aClDef->SEND_BUF[0]  = aClDef->RECV_BUF[0];      // ID
			aClDef->SEND_BUF[1]  = aClDef->RECV_BUF[1];      // ID
			aClDef->SEND_BUF[2]  = aClDef->RECV_BUF[2];      // MB
			aClDef->SEND_BUF[3]  = aClDef->RECV_BUF[3];      // MB
			aClDef->SEND_BUF[4]  = 0;                        // Len
			aClDef->SEND_BUF[5]  = aClDef->RECV_BUF[11]*2+3; // Len(byte)
			aClDef->SEND_BUF[6]  = aClDef->RECV_BUF[6];      // Dev_Addr
			aClDef->SEND_BUF[7]  = aClDef->RECV_BUF[7];      // Func_Code
			aClDef->SEND_BUF[8]  = aClDef->RECV_BUF[11];     // Len(word)
			if (mb != NULL)
			for (int i = 0; i < aClDef->RECV_BUF[11]*2; i++) // Value
				aClDef->SEND_BUF[9 + i ] = mb->fAddrBuff[aClDef->RECV_BUF[9]*2 + i];
			send(aClDef->SocketHandle, aClDef->SEND_BUF, aClDef->RECV_BUF[11]*2 + 9, 0);
			ToDataLog(aClDef, " --->", aClDef->SEND_BUF, aClDef->RECV_BUF[11]*2 + 9); 
			break;
		}
		case(0x06): //Write Holding Registers int
		{
			if (mb != NULL) mb->fAddrBuff[aClDef->RECV_BUF[9]*2] = aClDef->RECV_BUF[10];
			if (mb != NULL) mb->fAddrBuff[aClDef->RECV_BUF[9]*2+1] = aClDef->RECV_BUF[11];
			break;
		}
		case(0x10): //Write Holding Registers float, double
		{		
			for(int i = 0; i < aClDef->RECV_BUF[12]; i++)
			{
				if (mb != NULL) mb->fAddrBuff[aClDef->RECV_BUF[9]*2+i] = aClDef->RECV_BUF[13+i];				
			}
			break;
		}
	}	 	
}
//---------------------------------------------------------------------------
void TModbusSlave::WriteBuff(int aIndex, char aVal)
{
	
}
//---------------------------------------------------------------------------
char TModbusSlave::ReadBuff(int aIndex)
{
	return 0;
}
//---------------------------------------------------------------------------
int TModbusSlave::SetIntTag(int aAddr, int aVal)
{	
	int Addr = aAddr - 1;
	int* ptr = (int*)(fAddrBuff + Addr*2); ptr[0] = aVal;
	ByteReverse(fAddrBuff[Addr*2+0],fAddrBuff[Addr*2+1]);
	return aAddr + 1;
}
//---------------------------------------------------------------------------
int TModbusSlave::GetIntTag(int aAddr)
{
	int Addr = aAddr - 1;
	char buf[2];
	for (int i = 0; i<2; i++) buf[i] = fAddrBuff[Addr*2+i];
    ByteReverse(buf[0],buf[1]);
	short* ptr = (short*)(buf); 	
	return ptr[0];
}
//---------------------------------------------------------------------------
int TModbusSlave::SetFloatTag(int aAddr, float aVal)
{
    int Addr = aAddr - 1;
	float* ptr = (float*)(fAddrBuff + Addr*2); ptr[0] = aVal;
	ByteReverse(fAddrBuff[Addr*2+0],fAddrBuff[Addr*2+1]);
	ByteReverse(fAddrBuff[Addr*2+2],fAddrBuff[Addr*2+3]);
	return aAddr + 2;
}
//---------------------------------------------------------------------------
float TModbusSlave::GetFloatTag(int aAddr)
{
	int Addr = aAddr - 1;
	char buf[4];
	for (int i = 0; i<4; i++) buf[i] = fAddrBuff[Addr*2+i];
    ByteReverse(buf[0],buf[1]);
	ByteReverse(buf[2],buf[3]);
	float* ptr = (float*)(buf); 
	return ptr[0];
}
//---------------------------------------------------------------------------
int TModbusSlave::SetDoubleTag(int aAddr, double aVal)   
{
    int Addr = aAddr - 1;
	double* ptr = (double*)(fAddrBuff + Addr*2); ptr[0] = aVal;
	ByteReverse(fAddrBuff[Addr*2+0],fAddrBuff[Addr*2+1]);
	ByteReverse(fAddrBuff[Addr*2+2],fAddrBuff[Addr*2+3]);	
	ByteReverse(fAddrBuff[Addr*2+4],fAddrBuff[Addr*2+5]);
	ByteReverse(fAddrBuff[Addr*2+6],fAddrBuff[Addr*2+7]);
	return aAddr + 4;
}
//---------------------------------------------------------------------------
double TModbusSlave::GetDoubleTag(int aAddr)
{
	int Addr = aAddr - 1;
	char buf[8];
	for (int i = 0; i<8; i++) buf[i] = fAddrBuff[Addr*2+i];
    ByteReverse(buf[0],buf[1]);
	ByteReverse(buf[2],buf[3]);
	ByteReverse(buf[4],buf[5]);
	ByteReverse(buf[6],buf[7]);
	double* ptr = (double*)(buf); 
	return ptr[0];
}

/////////////////////////////////////////////////////////////////////////////


 


