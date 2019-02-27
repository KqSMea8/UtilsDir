#include "stdafx.h"
#include "CAN_BUS_7565.h"

RS_CAN_Protokol QueSendPaket[100];
RS_CAN_Protokol QueRecPaket[100];
long pointReadQueSend = 0;		// указатель чтения из очереди передачи 
long pointWriteQueSend = 0;		// указатель записи из очереди передачи
long pointReadQueRec = 0;		// указатель чтения из очереди приема
long pointWriteQueRec = 0;		// указатель записи из очереди приема

long SendInd = 1;

long TypeInter = CANBUS;		//0-CANBUS, 1-MODBUSRTU, 2-ModBusASCII, 
long Master = MASTER;
HANDLE hCom = INVALID_HANDLE_VALUE;	
CWinThread *hThreadR, *hThreadS;

long InitCom(wchar_t * PortName)
{
	DCB dcb;
	COMMTIMEOUTS ComPortTimeouts;		// таймауты работы с COM портом
	int portSuccess = 0;
	int myerr;

	hCom = CreateFile(PortName, GENERIC_READ|GENERIC_WRITE, 0, 0, OPEN_EXISTING,	0, 0);
	if(hCom == INVALID_HANDLE_VALUE)
	{
		//Handle Error
		TRACE("CreateFile failed with error %d\n", GetLastError());
		hCom = INVALID_HANDLE_VALUE;
		return -1;
	}
	//Get port status
	portSuccess = GetCommState(hCom, &dcb);
	if(!portSuccess)
	{
		TRACE("Unable to retrieve COM port status:errorCode %d\n", GetLastError());
		hCom = INVALID_HANDLE_VALUE;
		return -2;
	}
	//Fill in DCB structure
	dcb.DCBlength = sizeof(DCB);
	dcb.BaudRate = 57600;
	dcb.fBinary = TRUE;
	dcb.Parity = NOPARITY;
	dcb.ByteSize = 8;
	dcb.StopBits = TWOSTOPBITS;
	portSuccess = SetCommState(hCom, &dcb);
	if(!portSuccess)
	{
		//Handle error
		TRACE("Unable to set COM port status:errorCode %d\n",GetLastError());
		hCom = INVALID_HANDLE_VALUE;
		return -3;
	}
	ComPortTimeouts.ReadIntervalTimeout = 1;
	ComPortTimeouts.ReadTotalTimeoutMultiplier = 1;
	ComPortTimeouts.ReadTotalTimeoutConstant = 1;
	ComPortTimeouts.WriteTotalTimeoutConstant = 1;
	ComPortTimeouts.WriteTotalTimeoutMultiplier = 1;
	myerr = SetCommTimeouts(hCom, &ComPortTimeouts);

	SendInd = 1;
	hThreadS = AfxBeginThread(ThreadProcSend, 0, THREAD_PRIORITY_NORMAL);
	hThreadR = AfxBeginThread(ThreadProcReceive, 0, THREAD_PRIORITY_NORMAL);

	return 0;
}
//////////////////////////////////////////////////////////////////////////////
long CloseCom()
{
	SendInd = 0;
	if(hCom != INVALID_HANDLE_VALUE) CloseHandle(hCom);
	hCom = INVALID_HANDLE_VALUE;
	return 0;
}
//////////////////////////////////////////////////////////////////////////////////
long TFadr[]={FREQUENCY,MINLEVEL,MAXLEVEL,REFERENCE,SENSE,MEASURE,NAME1,NAME2,NAME3,NAME4,NAME5,KBOD,NEWADRESS,NUMBEROFSENSOR,STATUS,SERNUM,TYPESENSOR};

long TableFadr(long index)
{
	long retval=0;
	if(index < 0) retval = -1;
	if(index >= sizeof(TFadr)) retval = -1;
	if(retval < 0)return retval;
	retval = TFadr[index];
	return retval;
}

long Convert2CharBuf(RS_CAN_Protokol * Paket, char * SendBuf, long maxlen)
{
	int IDE;
	unsigned long com;
	unsigned char *ukaz;
	long prior;
	
	prior = Master-1;
	IDE = ((prior<<9)&0x600) + (Paket->adress&0x1ff);
	ukaz = (unsigned char*)Paket->mera.s;

	switch(Paket->type)
	{
	case TypeDatNothing:
		Paket->size = 0;
		TRACE("TypeDatNothing\n");
		sprintf_s(SendBuf,maxlen-1,"t%03x%1x%", IDE+0x400, Paket->size);
		break;
	case TypeDatShort1:
		Paket->size = 2;
		sprintf_s(SendBuf,maxlen-1,"t%03x%1x%02x%02x", IDE+0x400,Paket->size, ukaz[1],ukaz[0]);
		break;
	case TypeDatShort2:
		Paket->size = 4;
		sprintf_s(SendBuf,maxlen-1,"t%03x%1x%02x%02x%02x%02x", IDE+0x400,Paket->size, ukaz[1],ukaz[0],ukaz[3],ukaz[2]);
		break;
	case TypeDatShort3:
		Paket->size = 6;
		sprintf_s(SendBuf,maxlen-1,"t%03x%1x%02x%02x%02x%02x%02x%02x", IDE+0x400,Paket->size, ukaz[1],ukaz[0],ukaz[3],ukaz[2],ukaz[5],ukaz[4]);
		break;
	case TypeDatShort4:
		Paket->size = 8;
		sprintf_s(SendBuf,maxlen-1,"t%03x%1x%02x%02x%02x%02x%02x%02x%02x%02x", IDE+0x400,Paket->size, ukaz[1],ukaz[0],ukaz[3],ukaz[2],ukaz[5],ukaz[4],ukaz[7],ukaz[6]);
		break;
	case TypeDatLong1:
		Paket->size = 4;
		sprintf_s(SendBuf,maxlen,"t%03x%1x%02x%02x%02x%02x", IDE+0x400,Paket->size, ukaz[1],ukaz[0],ukaz[3],ukaz[2]);
		break;
	case TypeDatLong2:
		Paket->size = 8;
		sprintf_s(SendBuf,maxlen,"t%03x%1x%02x%02x%02x%02x%02x%02x%02x%02x", IDE+0x400,Paket->size, ukaz[1],ukaz[0],ukaz[3],ukaz[2],ukaz[5],ukaz[4],ukaz[7],ukaz[8]);
		break;
	case TypeDatFloat1:
		Paket->size = 4;
		sprintf_s(SendBuf,maxlen,"t%03x%1x%02x%02x%02x%02x", IDE+0x400,Paket->size, ukaz[1],ukaz[0],ukaz[3],ukaz[2]);
		break;
	case TypeDatFloat2:
		Paket->size = 8;
		sprintf_s(SendBuf,maxlen,"t%03x%1x%02x%02x%02x%02x%02x%02x%02x%02x", IDE+0x400,Paket->size, ukaz[1],ukaz[0],ukaz[3],ukaz[2],ukaz[5],ukaz[4],ukaz[7],ukaz[6]);
		break;
	case TypeDatDouble1:
		Paket->size = 8;
		sprintf_s(SendBuf,maxlen,"t%03x%1x%02x%02x%02x%02x%02x%02x%02x%02x", IDE+0x400,Paket->size, ukaz[1],ukaz[0],ukaz[3],ukaz[2],ukaz[5],ukaz[4],ukaz[7],ukaz[6]);
		break;
	case TypeDatChar1:
		Paket->size = 1;
		sprintf_s(SendBuf,maxlen,"t%03x%1x%02x", IDE+0x400,Paket->size, ukaz[0]);
		break;
	case TypeDatChar2:
		Paket->size = 2;
		sprintf_s(SendBuf,maxlen,"t%03x%1x%02x%02x", IDE+0x400,Paket->size, ukaz[1], ukaz[0]);
		break;
	case TypeDatChar3:
		Paket->size = 3;
		sprintf_s(SendBuf,maxlen,"t%03x%1x%02x%02x%02x", IDE+0x400,Paket->size, ukaz[1], ukaz[0], ukaz[2]);
		break;
	case TypeDatChar4:
		Paket->size = 4;
		sprintf_s(SendBuf,maxlen,"t%03x%1x%02x%02x%02x%02x", IDE+0x400,Paket->size, ukaz[1], ukaz[0], ukaz[3], ukaz[2]);
		break;
	case TypeDatChar5:
		Paket->size = 5;
		sprintf_s(SendBuf,maxlen,"t%03x%1x%02x%02x%02x%02x%02x", IDE+0x400,Paket->size, ukaz[1], ukaz[0], ukaz[3], ukaz[2], ukaz[4]);
		break;
	case TypeDatChar6:
		Paket->size = 6;
		sprintf_s(SendBuf,maxlen,"t%03x%1x%02x%02x%02x%02x%02x%02x", IDE+0x400,Paket->size, ukaz[1], ukaz[0], ukaz[3], ukaz[2], ukaz[5], ukaz[4]);
		break;
	case TypeDatChar7:
		Paket->size = 7;
		sprintf_s(SendBuf,maxlen,"t%03x%1x%02x%02x%02x%02x%02x%02x%02x", IDE+0x400,Paket->size, ukaz[1], ukaz[0], ukaz[3], ukaz[2], ukaz[5], ukaz[4], ukaz[6]);
		break;
	case TypeDatChar8:
		Paket->size = 8;
		sprintf_s(SendBuf,maxlen,"t%03x%1x%02x%02x%02x%02x%02x%02x%02x%02x", IDE+0x400,Paket->size, ukaz[1], ukaz[0], ukaz[3], ukaz[2], ukaz[5], ukaz[4], ukaz[7], ukaz[6]);
		break;
	case TypeComNothing:
		com = (unsigned long)Paket->fadr;
		Paket->size = 2;
		sprintf_s(SendBuf,maxlen,"t%03x%1x%04x", IDE, Paket->size, com);
		break;
	case TypeComFloat:
		com = (unsigned long)Paket->fadr;
		Paket->size = 6;
		sprintf_s(SendBuf,maxlen,"t%03x%1x%04x%02x%02x%02x%02x", IDE,Paket->size, com, ukaz[1],ukaz[0],ukaz[3],ukaz[2]);
		break;
	case TypeComChar:
		com = (unsigned long)Paket->fadr;
		Paket->size = 8;
		sprintf_s(SendBuf,maxlen,"t%03x%1x%04x%02x%02x%02x%02x%02x%02x", IDE,Paket->size, com, ukaz[1], ukaz[0], ukaz[3], ukaz[2], ukaz[5], ukaz[4], ukaz[6]);
		break;
	case TypeComLong:
		com = (unsigned long)Paket->fadr;
		Paket->size = 6;
		sprintf_s(SendBuf,maxlen,"t%03x%1x%04x%02x%02x%02x%02x", IDE,Paket->size, com, ukaz[1],ukaz[0],ukaz[3],ukaz[2]);
		break;
	}
	char enter[2];
	enter[0] = 0xd;
	enter[1] = 0;
	strcat_s(SendBuf, maxlen, enter);
	return (long)strlen(SendBuf);
}
//////////////////////////////////////////////////////////////////////////////////
int Convert2Struct(char * RecBuf, RS_CAN_Protokol * Paket)
{
	int prio;
	int pos,com,dlc,dlina;
	int ind,i,k,prom;
	char symb0 = '0';
	unsigned short kod;
	char bytes[8];
	unsigned short *sbuf;

	ind = 0;
	if(RecBuf[ind] != 't') return -1;

	ind++;
	bytes[0] = RecBuf[ind];
	bytes[1] = RecBuf[ind+1];
	bytes[2] = RecBuf[ind+2];
	bytes[3] = 0; 
	sscanf_s(bytes, "%x", &prom);
	prio = prom>>9;				//prioritet 0...3
	pos = prom&0x1ff;			//number of node 0 - system ...511 other
	ind++;
	ind++;
	ind++;
	dlc = (RecBuf[ind] - symb0)&0xf;// from 0 to 8
	dlina = (int)(strlen(RecBuf)-5)/2;
	if(dlina != dlc) return -1;
	if(dlc > 8) dlc = 8;
	if(prio < 2)		//command (2 or 3)
	{
		ind++;
		bytes[0] = RecBuf[ind];
		ind++;
		bytes[1] = RecBuf[ind];
		ind++;
		bytes[2] = RecBuf[ind];
		ind++;
		bytes[3] = RecBuf[ind];
		bytes[4] = 0;
		sscanf_s(bytes,"%x",&com);
	}
	else				//dat
	{
		com = 0;
	}
	sbuf = Paket->mera.s;
	ind++;
	for(i = ind; i < ind+dlc*2; i+= 4)
	{
		bytes[0]=RecBuf[i];
		bytes[1]=RecBuf[i+1];
		bytes[2]=RecBuf[i+2];
		bytes[3]=RecBuf[i+3];
		bytes[4] = 0;
		sscanf_s(bytes,"%x",&prom);
		kod = prom;
		k = (i-ind)/4;
		sbuf[k] = kod;
	}
	Paket->fadr = com;
	Paket->function = prio;
	Paket->adress = pos;
	Paket->size = dlc;

	return 0;
}
//////////////////////////////////////////////////////////
int PaketInterp(RS_CAN_Protokol Reply, node *Nodes)
{
	int adress;

	adress = Reply.adress;
	if((adress < 1)||(adress >= NUMNODES))	// таких адресов не существует
	{
		return -1;
	}
	if(Reply.function == 3) // идут данные 
	{
		if(Nodes[adress].bufer == 0)
		{
			Nodes[adress].bufer = (float*)malloc(SIZEBUF*sizeof(float));
			Nodes[adress].posreader = 0;
			Nodes[adress].poswriter = 0;
			Nodes[adress].curtime = 0;
		}
		if(Nodes[adress].bufer != 0)
		{
			Nodes[adress].bufer[Nodes[adress].poswriter] = Reply.mera.f[0];
			Nodes[adress].poswriter++;
			if(Nodes[adress].poswriter >= SIZEBUF) Nodes[adress].poswriter = 0;
			Nodes[adress].livetick = 300;			//повышаем уровень жизни
		}
//		TRACE("Data adres %d %f\n",adres,Reply.fmera[0]);
	}
	if(Reply.function == 1)		// ответы на запросы
	if(Reply.size > 2)
	{
		switch(Reply.fadr)
		{
		case FREQUENCY:
			Nodes[adress].frequency = Reply.mera.f[0];
			Nodes[adress].livestat |= LIVEFREQUENCY;
			break;
		case MINLEVEL:
			Nodes[adress].minlevel = Reply.mera.f[0];
			Nodes[adress].livestat |= LIVEMINLEVEL;
			break;
		case MAXLEVEL:
			Nodes[adress].maxlevel = Reply.mera.f[0];
			Nodes[adress].livestat |= LIVEMAXLEVEL;
			break;
		case REFERENCE:
			Nodes[adress].reference = Reply.mera.f[0];
			Nodes[adress].livestat |= LIVEREFERENCE;
			break;
		case MEASURE:
			memcpy(Nodes[adress].measure, Reply.mera.ch, 6);
			Nodes[adress].livestat |= LIVEMEASURE;
			break;
		case NAME1:
			memcpy(Nodes[adress].nameinfo, Reply.mera.ch, 6);
			Nodes[adress].livestat |= LIVENAME1;
			break;
		case NAME2:
			memcpy(&Nodes[adress].nameinfo[6], Reply.mera.ch, 6);
			Nodes[adress].livestat |= LIVENAME2;
			break;
		case NAME3:
			memcpy(&Nodes[adress].nameinfo[12], Reply.mera.ch, 6);
			Nodes[adress].livestat |= LIVENAME3;
			break;
		case NAME4:
			memcpy(&Nodes[adress].nameinfo[18], Reply.mera.ch, 6);
			Nodes[adress].livestat |= LIVENAME4;
			break;
		case NAME5:
			memcpy(&Nodes[adress].nameinfo[24], Reply.mera.ch, 6);
			Nodes[adress].livestat |= LIVENAME5;
			break;
		case SENSE:
			Nodes[adress].sense = Reply.mera.f[0];
			Nodes[adress].livestat |= LIVESENSE;
			break;
		case STATUS:
			Nodes[adress].status = 0xff&(long)Reply.mera.ch[0];
			Nodes[adress].typesensor = 0xff&(long)Reply.mera.ch[1];
			Nodes[adress].sernum = 0xffff&(long)Reply.mera.s[1];
			Nodes[adress].subtypesensor = 0xff&(long)Reply.mera.ch[4];
			Nodes[adress].livestat |= LIVESTATUS;
			break;
		}
//		TRACE("CommandInterp Reply.comand %d\n",Reply.comand);
	}
	return 0;
}
/////////////////////////////////////////////////////////////////////////////////////////
UINT ThreadProcSend(LPVOID pParam)
{
	char sendbuf[300];
	DWORD length, bytesSend;

	while(SendInd != 0)
	{
		if(pointReadQueSend != pointWriteQueSend)
		{
			length = Convert2CharBuf(&QueSendPaket[pointReadQueSend], sendbuf,300);
			WriteFile(hCom, sendbuf, length, &bytesSend, NULL);
			pointReadQueSend++;
			if(pointReadQueSend >= SIZEQUE) pointReadQueSend = 0;
		}
		Sleep(10);
	}
	return 0;
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
UINT ThreadProcReceive(LPVOID pParam)
{
	char recbuf[MAXLENGTHRECBUF];
	static char ost[MAXLENGTHRECBUF*2];
	DWORD length, bytesRec;
	length = MAXLENGTHRECBUF;
	static long index = 0;
	long dlina, i, numbytes,myerr;
	RS_CAN_Protokol Reply;


	while(SendInd != 0)
	{
//		if(pointReadQueSend != pointWriteQueSend)
		{
			ReadFile(hCom, recbuf, length, &bytesRec, NULL);

			if(bytesRec != 0)
			{
				memcpy(&ost[index], recbuf, bytesRec);
				dlina = index + bytesRec;
				index = 0;
				if(TypeInter == CANBUS)
				{
					for(i = 0; i < dlina; i++)
					{
						if(ost[i] == 0xd)
						{
							ost[i] = 0;
							TRACE("ReadFile %s\n",&ost[index]);
							myerr = Convert2Struct(&ost[index], &Reply);
							QueRecPaket[pointWriteQueRec] = Reply;
//							if(myerr == 0) myerr = PaketInterp(Reply, Nodes);
							index = i+1;
							pointWriteQueRec++;
							if(pointWriteQueRec >= SIZEQUE) pointWriteQueRec = 0;
						}
					}
				}
				if(TypeInter == MODBUSRTU)
				{
					if(Master == MASTER)
					{
						for(i = 0; i < dlina; i++)
						{
							if(i == index+2)	// код функции
							{
								switch(ost[index+1])
								{
								case 3:
								case 4:
									numbytes = ost[index+2];
									if(index+4+numbytes <= dlina)		//полный пакет для передачи
									{
										//.......................
										index += 4 + numbytes;
										i = index;
									}
									break;
								case 16:
								case 65:
									if(index+8 <= dlina)		//полный пакет для передачи
									{
										//.......
										index += 8;
										i = index;
									}
									break;
								}
							}
						}
					}
					else		// SLAVE
					{
					}
				}
				if(index != dlina)
					memcpy(ost, &ost[index], dlina-index);		//копировать
				index = dlina-index;
			}
		}
		Sleep(1);
	}
	return 0;
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
int SendPaket(RS_CAN_Protokol *Paket)						// поставить в очередь на передачу пакет
{
	QueSendPaket[pointWriteQueSend] = *Paket;
	pointWriteQueSend++;
	if(pointWriteQueSend >= SIZEQUE) pointWriteQueSend = 0;
	if(pointWriteQueSend == pointReadQueSend) return -1;
	return 0;
}
///////////////////////////////////////////////////////////////////////////////////////////////
int ReceivePaket(RS_CAN_Protokol *Paket )						// взять из очереди пакет
{
	if(pointReadQueRec == pointWriteQueRec) return -1;
	*Paket = QueRecPaket[pointReadQueRec];
	pointReadQueRec++;
	if(pointReadQueRec >= SIZEQUE) pointReadQueRec = 0;
	return 0;
}
///////////////////////////////////////////////////////////////////////////////////////////
int SinhroFromNode(long adres, DevPar* NodeDev);	// прочитать параметры из устройства по адресу adress и заполнить DevPar struct
int	WriteToNode(DevPar NodeDev, long adress);		// Записать информацию из DevPar struct в устройство по адресу adress
long SetMaster(long param)
{
	Master = param;
	return param;
}
