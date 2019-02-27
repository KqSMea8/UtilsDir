#include "stdafx.h"
#include "MODBUS.h"

////////////////////////////////////////////////////////////////////////////////////
long MODBUS::InitCom(wchar_t * PortName, long baudrate)
{
	DCB dcb;
	COMMTIMEOUTS ComPortTimeouts;		// таймауты работы с COM портом
	int portSuccess = 0;
	int myerr;

	intmod.hCom = CreateFile(PortName, GENERIC_READ|GENERIC_WRITE, 0, 0, OPEN_EXISTING,	0, 0);
	if(intmod.hCom == INVALID_HANDLE_VALUE)
	{
		//Handle Error
		TRACE("CreateFile failed with error %d\n", GetLastError());
		intmod.hCom = INVALID_HANDLE_VALUE;
		return -1;
	}
	//Get port status
	portSuccess = GetCommState(intmod.hCom, &dcb);
	if(!portSuccess)
	{
		TRACE("Unable to retrieve COM port status:errorCode %d\n", GetLastError());
		intmod.hCom = INVALID_HANDLE_VALUE;
		return -2;
	}
	intmod.baud = baudrate;
	//Fill in DCB structure
	dcb.DCBlength = sizeof(DCB);
	dcb.BaudRate = baudrate;
	dcb.fBinary = TRUE;
	dcb.Parity = NOPARITY;
	dcb.ByteSize = 8;
	dcb.StopBits = ONESTOPBIT;
	portSuccess = SetCommState(intmod.hCom, &dcb);
	if(!portSuccess)
	{
		//Handle error
		TRACE("Unable to set COM port status:errorCode %d\n",GetLastError());
		intmod.hCom = INVALID_HANDLE_VALUE;
		return -3;
	}
	ComPortTimeouts.ReadIntervalTimeout = 1;
	ComPortTimeouts.ReadTotalTimeoutMultiplier = 1;
	ComPortTimeouts.ReadTotalTimeoutConstant = 1;
	ComPortTimeouts.WriteTotalTimeoutConstant = 1;
	ComPortTimeouts.WriteTotalTimeoutMultiplier = 1;
	myerr = SetCommTimeouts(intmod.hCom, &ComPortTimeouts);

	intmod.SendInd = 1;

	char ch=0;
	hThreadR = AfxBeginThread(ThreadProcSendM, (LPVOID)&intmod, THREAD_PRIORITY_NORMAL);
//	hThreadR = AfxBeginThread(ThreadProcReceiveM, (LPVOID)&intmod, THREAD_PRIORITY_NORMAL);

	return 0;
}
//////////////////////////////////////////////////////////////////////////////
long MODBUS::CloseCom()
{
	intmod.SendInd = 0;
	if(intmod.hCom != INVALID_HANDLE_VALUE) CloseHandle(intmod.hCom);
	intmod.hCom = INVALID_HANDLE_VALUE;
	return 0;
}
////////////////////////////////////////////////////////////////////////////////////////////
long MODBUS::SendPaket(Paket *spack)
{
	intmod.QueSendPaket[intmod.pointWriteQueSend] = *spack;
	intmod.pointWriteQueSend++;
	if(intmod.pointWriteQueSend >= SIZEQUE) intmod.pointWriteQueSend = 0;
	if(intmod.pointWriteQueSend == intmod.pointReadQueSend) return -1;
	return 0;
}
////////////////////////////////////////////////////////////////////////////////////////////////
long MODBUS::ReceivePaket(Paket *rpack)
{
	if(intmod.pointReadQueRec == intmod.pointWriteQueRec) return -1;
	*rpack = intmod.QueRecPaket[intmod.pointReadQueRec];
	intmod.pointReadQueRec++;
	if(intmod.pointReadQueRec >= SIZEQUE) intmod.pointReadQueRec = 0;
	return 0;
}
/////////////////////////////////////////////////////////////////////////////////////////////////
UINT ThreadProcSendM(LPVOID pParam)
{
	char sendbuf[300];
	DWORD length, bytesSend;
	InterMod *inmd;
	inmd = (InterMod*)pParam;
	struct _timeb  teksend,tek;
	double diff,delta;
	static char ost[MAXLENGTHRECBUF*2];
	DWORD bytesRec;
	length = MAXLENGTHRECBUF;
	static long index = 0;
	static long RS_ind = 0;
	char cVal;
	char *ppoint;
	Paket Rec;
	static unsigned short RS_CLC;
	unsigned short RS_CRC;
	long RS_pointer;
	long timeout = 0;
	long timepack = 0;
	long myerr, RS_endian;

//	FT_GetStatus();

	inmd->BlockSend = 0;
	while(inmd->SendInd != 0)
	{
		if(inmd->BlockSend == 0)
			if(inmd->pointReadQueSend != inmd->pointWriteQueSend)
			{
				length = Convert2ByteBuf(&inmd->QueSendPaket[inmd->pointReadQueSend], sendbuf,300);
				myerr = WriteFile(inmd->hCom, sendbuf, length, &bytesSend, NULL);
				_ftime64_s(&teksend);
				delta = 1./inmd->baud*11*(length+3)+0.02+1;			//???????????????????????????????????
				Rec = inmd->QueSendPaket[inmd->pointReadQueSend];
				inmd->pointReadQueSend++;
				if(inmd->pointReadQueSend >= SIZEQUE) inmd->pointReadQueSend = 0;
				inmd->BlockSend = 1;
				TRACE("Send %d\n",length);
			}
		length = 1;
		ReadFile(inmd->hCom, &cVal, length, &bytesRec, NULL);
		if(bytesRec > 0)
		{
//			TRACE("rec data %2x, RS_ind %d, pmb->BlockSend %d\n", (cVal&0xff), RS_ind, inmd->BlockSend);
			switch(RS_ind)
			{
				case 0:		//node
					RS_CLC = 0xFFFF;
					RS_CLC = Crc16(&cVal, RS_CLC);
					Rec.node = 0xff & cVal;
					RS_ind++;
					break;
				case 1:							//number of function
					RS_CLC = Crc16(&cVal, RS_CLC);
					Rec.command = 0xff & cVal;
					RS_ind++;
					if((cVal==Read_Coil_Status)||(cVal==Read_Discrete_Inputs)||(cVal==Read_Holding_Registers)||(cVal==Read_Input_Registers))
						RS_ind += 4;
					break;
				case 2:							//Adres of table
					RS_CLC = Crc16(&cVal, RS_CLC);
					Rec.adtable = cVal<<8;
					RS_ind++;
					break;
				case 3:
					RS_CLC = Crc16(&cVal, RS_CLC);
					Rec.adtable += 0xff & cVal;
					RS_ind++;
					break;
				case 4:							//quantity of elements
					RS_CLC = Crc16(&cVal, RS_CLC);
					Rec.quantity = cVal<<8;
					RS_ind++;
					break;
				case 5:
					RS_CLC = Crc16(&cVal, RS_CLC);
					Rec.quantity += 0xff & cVal;
					RS_ind++;
//					if((Rec.command != Force_Multiple_Coils)&(Rec.command != Preset_Multiple_Registers))
					RS_ind += 2;
					break;
				case 6:							//number of bytes to send
					RS_CLC = Crc16(&cVal, RS_CLC);
					Rec.size = (0xff & cVal);
					RS_pointer = 0;
					RS_ind++;
					RS_endian = 1;
					if(Rec.size == 0)
						RS_ind++;
					break;
				case 7:							//get data
					RS_CLC = Crc16(&cVal, RS_CLC);
					ppoint = (char*)Rec.data;
					ppoint[RS_pointer+RS_endian] = cVal;
					RS_endian = -RS_endian;
					RS_pointer++;
					if(RS_pointer >= Rec.size)
						RS_ind++;
					break;
				case 8:
					RS_CRC = 0xff & cVal;
					RS_ind++;
					break;
				case 9:
					RS_CRC += cVal<<8;
					RS_ind++;
					RS_ind = 0;
//					RS_CLC = RS_CLC & 0xffff;
//					RS_CRC = RS_CRC & 0xffff;
					Rec.enable = 1;
					if(RS_CRC != RS_CLC)	// error CRC-16
						Rec.enable = -1;
					if(Rec.enable == 1)
					{
//						Rec.adtable = inmd->QueRecPaket[inmd->pointWriteQueRec].adtable;
//						Rec.quantity = inmd->QueRecPaket[inmd->pointWriteQueRec].quantity;
						inmd->QueRecPaket[inmd->pointWriteQueRec] = Rec;
						inmd->pointWriteQueRec++;
						if(inmd->pointWriteQueRec >= SIZEQUE) inmd->pointWriteQueRec = 0;

					}
					Sleep(10);
					inmd->BlockSend = 0;
					TRACE("Rec.enable %d\n", Rec.enable);
					break;
			}
			timeout = 0;
			timepack = 0;
			_ftime64_s(&teksend);
		}
		else
		{
			if(inmd->BlockSend != 0)
			{
				_ftime64_s(&tek);
				diff = tek.time - teksend.time + tek.millitm /1000. - teksend.millitm/1000.;
				if(diff > delta)
				{
					TRACE("BlockSend=0\n");
					inmd->BlockSend = 0;
				}
			}
		}
	}
	return 0;
}
///////////////////////////////////////////////////////////////////////////////////////////////////
MODBUS::MODBUS(void)
{
	intmod.hCom = INVALID_HANDLE_VALUE;
	intmod.pointReadQueSend = 0;		// указатель чтения из очереди передачи 
	intmod.pointWriteQueSend = 0;		// указатель записи из очереди передачи
	intmod.pointReadQueRec = 0;			// указатель чтения из очереди приема
	intmod.pointWriteQueRec = 0;		// указатель записи из очереди приема
	intmod.SendInd = 1;
	intmod.BlockSend = 0;
	intmod.Master = MASTER;
}
////////////////////////////////////////////////////////////////////////////////////////////////
MODBUS::~MODBUS(void)
{
}
/////////////////////////////////////////////////////
unsigned short Crc16( char *pcBlock, unsigned short RS_crc)
{
    long i;
 
// 	RS_CLC ^= *pcBlock << 8;
	RS_crc ^= (*pcBlock) & 0xff;
 
 	for( i = 0; i < 8; i++ )
	{
		RS_crc = (RS_crc & 0x01) ? (RS_crc >> 1) ^ 0xA001 : RS_crc >> 1;
	}
 		//RS_CLC = RS_CLC & 0x8000 ? ( RS_CLC << 1 ) ^ 0x8005 : RS_CLC << 1;

    return RS_crc;
}
//////////////////////////////////////////////////////////////////////////////////
long Convert2ByteBuf(Paket * Paket, char * SendBuf, long maxlen)
{
	long i, index;
	char *pointer;
	unsigned short RS_crc;

	if(maxlen <= 0) return -1;
	index = 0;
	SendBuf[index] = (char)Paket->node;
	index++;
	if(index >= maxlen) return -1;
	SendBuf[index] = (char)Paket->command;
	index++;
	if(index >= maxlen) return -1;
	SendBuf[index] = Paket->adtable>>8;
	index++;
	if(index >= maxlen) return -1;
	SendBuf[index] = Paket->adtable & 0xff;
	index++;
	if(index >= maxlen) return -1;
	SendBuf[index] = Paket->quantity>>8;
	index++;
	if(index >= maxlen) return -1;
	SendBuf[index] = Paket->quantity & 0xff;
	index++;
	if(index >= maxlen) return -1;
	if((Paket->command == Force_Multiple_Coils) || (Paket->command == Preset_Multiple_Registers))
	{
		SendBuf[index] = Paket->size & 0xff;
		index++;
		if(index >= maxlen) return -1;
		pointer = (char*)Paket->data;
		for(i = 0; i < (Paket->size & 0xff); i = i+2)
		{
			SendBuf[index] = pointer[i+1];
			SendBuf[index+1] = pointer[i];
			index++;
			index++;
			if(index >= maxlen) return -1;
		}
	}
	RS_crc = 0xffff;
	for(i = 0; i < index; i++)
	{
		RS_crc =  Crc16(&SendBuf[i],RS_crc);
	}
	SendBuf[index] = (char)(0xff & RS_crc);
	index++;
	if(index >= maxlen) return -1;
	SendBuf[index] = (char)((RS_crc >> 8) & 0xff);
	index++;
	if(index >= maxlen) return -1;
	return index;
}
//////////////////////////////////////////////////////////////////////////////////
long MODBUS::PaketIntMod(Paket * Reply, NodeInter *Nodes)
{
	int adress;
	int i;

	adress = Reply->node;		//должна быть функция добычи адреса
//
//		если адрес новый, то создается новая структура и производится сортировка по возрастающей
//
	if((adress < 1)||(adress >= NUMNODES))	// таких адресов не существует
	{
		return -1;
	}
	if((Reply->command == Read_Coil_Status)||
		(Reply->command == Read_Discrete_Inputs)||
		(Reply->command == Read_Holding_Registers)||
		(Reply->command == Read_Input_Registers)) // идут данные 
	{
		if(Nodes[adress].bufer == 0)
		{
			Nodes[adress].bufer = (float*)malloc(SIZEBUF*sizeof(float));
			Nodes[adress].posreader = 0;
			Nodes[adress].poswriter = 0;
			Nodes[adress].curtime = 0;
		}

		if(Reply->size >= 2)	//заполняет произвольное место в структуре
		{
			int start, end;
			char *point,*rec;
			start = Reply->adtable*2; 
			end = sizeof(CHANNEL_PAR);
			if(start >= 0)
			if(start + Reply->size <= sizeof(CHANNEL_PAR))
			{
				point = (char*)&Nodes[adress];
				rec = (char*)Reply->data;
				for(i = 0; i < Reply->size; i++)
				{
					point[i+start] = rec[i];
				}
				Nodes[adress].livetick = 300;			//повышаем уровень жизни
			}
			if(Reply->adtable != 0)
				TRACE("CommandInterp Reply.size %d\n", Reply->size);
		}
		if(Reply->adtable == 0)		// был запрос по созданию нового элемента
		{
			long qwe123;
			qwe123 = sizeof(CHANNEL_PAR);
			if(Nodes[adress].size == sizeof(CHANNEL_PAR))
			{
				Nodes[adress].livestat = LIVESTATUS;
				Reply->adtable = (sizeof(long) + sizeof(Nodes[adress].value))/2;
				Reply->command = Read_Holding_Registers;
				Reply->enable = 0;
				Reply->quantity = (sizeof(CHANNEL_PAR)-Reply->adtable*2)/2;
				Reply->size = Reply->quantity*2;
				return 1;
			}
		}

		if(Reply->adtable == (sizeof(long) + sizeof(Nodes[adress].value))/2)	// передача оставшейся структуры
		{
			Nodes[adress].livestat |= LIVEPARAM;
		}

		if(Reply->adtable == (sizeof(long))/2)			// Передача данных value
		if(Nodes[adress].bufer != 0)
		{
			short *point;
			point = (short*)&Nodes[adress].bufer[Nodes[adress].poswriter];
			for(i = 0; i < Reply->size/2; i += 2)
			{
				point[i] = Reply->data[i];
				point[i+1] = Reply->data[i+1];
				Nodes[adress].poswriter++;
				if(Nodes[adress].poswriter >= SIZEBUF)
				{
					Nodes[adress].poswriter = 0;
					point = (short*)Nodes[adress].bufer;
				}
			}
			Nodes[adress].livetick = 300;			//повышаем уровень жизни
		}
//		TRACE("Data adres %d %f\n",adres,Reply.fmera[0]);
	}
	return 0;
}


