#include "CZETModBus.h"

#include <tchar.h>
#include <process.h>

CZETModbus::CZETModbus()
{
}

CZETModbus::~CZETModbus(void)
{
}

BYTE CZETModbus::ReadCoilStatus(BYTE dev_addr, WORD addr, WORD* pBuff, unsigned int flags_num)
{
	throw ModbusException(0); //(!!!) Данный метод не реализован
	return 0x00;
}

BYTE CZETModbus::ReadDiscreteInputs(BYTE dev_addr, WORD addr, WORD* pBuff, unsigned int inputs_num)
{
	throw ModbusException(0); //(!!!) Данный метод не реализован
	return 0x00;
}

BYTE CZETModbus::ReadHoldingRegisters(BYTE dev_addr, WORD addr, WORD* pBuff, unsigned int& regs_num)
{
	BYTE Command[5];
	BYTE Response[MAX_PDU_SIZE];
	BYTE response_len;
	BYTE err = 0x00;

	if(regs_num > (MAX_PDU_SIZE - 2)/2 || regs_num < 1)
		throw ModbusException(9);
	
	Command[0] = 0x03;
	Command[1] = addr >> 8;
	Command[2] = addr & 0xFF;
	Command[3] = regs_num >> 8;
	Command[4] = regs_num & 0xFF;

	SendCommandPDU(dev_addr, Command);

	if(GetResponsePDU(Response, response_len) == dev_addr)
	{
		if((Response[0] & 0x7F) != 0x03)
			throw ModbusException(5);
		
		if(!(Response[0] & 0x80))
		{
			regs_num = Response[1]/2;
			for(unsigned int idx(0); idx < regs_num; idx++)
			{
				//Переворачиваем байты в каждом слове
				pBuff[idx] = (Response[idx*2 + 2]  << 8) | Response[idx*2 + 3];
			}
		}
		else
		{
			err = Response[1];
			if(!err) //Устройство не доступно
				err = 1;
		}
	}
	else
	{
		unsigned char buff[1024];
		Recv(buff, 1024);
		throw ModbusException(4);
	}
	
	return err;
}

BYTE CZETModbus::ReadInputRegisters(BYTE dev_addr, WORD addr, WORD* pBuff, unsigned int& regs_num)
{
	BYTE Command[5];
	BYTE Response[MAX_PDU_SIZE];
	BYTE response_len;
	BYTE err = 0x00;

	if(regs_num > (MAX_PDU_SIZE - 2)/2 || regs_num < 1)
		throw ModbusException(9);
	
	Command[0] = 0x04;
	Command[1] = addr >> 8;
	Command[2] = addr & 0xFF;
	Command[3] = regs_num >> 8;
	Command[4] = regs_num & 0xFF;

	SendCommandPDU(dev_addr, Command);

	if(GetResponsePDU(Response, response_len) == dev_addr)
	{
		if((Response[0] & 0x7F) != 0x04)
			throw ModbusException(5);
		
		if(!(Response[0] & 0x80))
		{
			regs_num = Response[1]/2;
			for(unsigned int idx(0); idx < regs_num; idx++)
			{
				//Переворачиваем байты в каждом слове
				pBuff[idx] = (Response[idx*2 + 2]  << 8) | Response[idx*2 + 3];
			}
		}
		else
			err = Response[1];
	}
	else
	{
		unsigned char buff[1024];
		Recv(buff, 1024);
		throw ModbusException(4);
	}
	
	return err;
}

BYTE CZETModbus::ForceSingleCoil(BYTE dev_addr, WORD addr, bool value)
{
	throw ModbusException(0); //(!!!) Данный метод не реализован
	return 0x00;
}

BYTE CZETModbus::PresetSingleRegister(BYTE dev_addr, WORD addr, WORD value)
{
	BYTE Command[5];
	BYTE Response[MAX_PDU_SIZE];
	BYTE response_len;
	BYTE err = 0x00;

	Command[0] = 0x06;
	Command[1] = addr >> 8;
	Command[2] = addr & 0xFF;
	Command[3] = value >> 8;
	Command[4] = value & 0xFF;

	SendCommandPDU(dev_addr, Command);

	if(GetResponsePDU(Response, response_len) == dev_addr)
	{
		if((Response[0] & 0x7F) != 0x06)
			throw ModbusException(5);
		
		if(!(Response[0] & 0x80))
		{
			if(!memcmp(Command, Response, 5))
				throw ModbusException(6);
		}
		else
			err = Response[1];
	}
	else
	{
		unsigned char buff[1024];
		Recv(buff, 1024);
		throw ModbusException(4);
	}
	
	return err;
}

BYTE CZETModbus::ForceMultipleCoils(BYTE dev_addr, WORD addr, WORD* pBuff, unsigned int flags_num)
{
	throw ModbusException(0); //(!!!) Данный метод не реализован
	return 0x00;
}

BYTE CZETModbus::PresetMultipleRegisters(BYTE dev_addr, WORD addr, WORD* pBuff, unsigned int &regs_num)
{
	BYTE Command[MAX_PDU_SIZE];
	BYTE Response[MAX_PDU_SIZE];
	BYTE response_len;
	BYTE err = 0x00;

	if(regs_num > (MAX_PDU_SIZE - 2)/2 || regs_num < 1)
		throw ModbusException(9);
	
	Command[0] = 0x10;
	Command[1] = addr >> 8;
	Command[2] = addr & 0xFF;
	Command[3] = regs_num >> 8;
	Command[4] = regs_num & 0xFF;
	Command[5] = regs_num*2 & 0xFF;

	memcpy_s(&Command[6], regs_num*2, pBuff, regs_num*2);

	SendCommandPDU(dev_addr, Command);

	if(GetResponsePDU(Response, response_len) == dev_addr)
	{
		if((Response[0] & 0x7F) != 0x10)
			throw ModbusException(5);
		
		if(!(Response[0] & 0x80))
			regs_num = (Response[3] << 8) | Response[4];
		else
			err = Response[1];
	}
	else
	{
		unsigned char buff[1024];
		Recv(buff, 1024);
		throw ModbusException(4);
	}
	
	return err;
}

void CZETModbus::SetConnectionParams(unsigned int br, unsigned int parity)
{
	BYTE command_buff[9];
	
	command_buff[0] = 100;
	memcpy_s(&command_buff[1], sizeof(br), &br, sizeof(br));
	memcpy_s(&command_buff[5], sizeof(parity), &parity, sizeof(parity));

	SendCommandPDU(0, command_buff);
}

BYTE CZETModbus::GetHierarchicalChain(BYTE dev_addr, BYTE* pBuff)
{
	BYTE command[5] = {0x64, 0x00, 0xFF, 0x00, 0xff};
	BYTE Response[MAX_PDU_SIZE];
	BYTE response_len;

	if(dev_addr == 3)
	{
		int r(8);
		r++;
	}

	SendCommandPDU(dev_addr, command);
	if(GetResponsePDU(Response, response_len) == dev_addr)
	{
		if((Response[0] & 0x7F) != command[0])
			throw ModbusException(5);
		
		if(Response[0] & 0x80)
			throw ModbusException(5);

		if(Response[2])
			if(pBuff)
				memcpy_s(pBuff, Response[2], &Response[3], Response[2]);
	}
	else
	{
		unsigned char buff[1024];
		Recv(buff, 1024);
		throw ModbusException(4);
	}

	return Response[2];
}

void CZETModbus::SendCommandPDU(BYTE dev_addr, BYTE* pPDU)
{
	unsigned int	pdu_len(5);
	BYTE			ADU[MAX_ADU_SIZE] = {dev_addr};
	
	switch(pPDU[0])
	{
		case 0x03 : break;
		case 0x04 : break;
		case 0x10 : pdu_len = 6 + pPDU[5]; break;
		case 0x64 : pdu_len = 5; break;
		default : throw ModbusException(1);
	}

	if(pdu_len <= MAX_PDU_SIZE && pdu_len >= MIN_PDU_SIZE)
	{
		WORD			check_sum;
		
		memcpy_s(&ADU[1], pdu_len, pPDU, pdu_len);
		check_sum = CheckSumm(ADU, pdu_len + 1);

		ADU[pdu_len + 1] = (check_sum & 0xFF00) >> 8;
		ADU[pdu_len + 2] = check_sum & 0x00FF;

		/*
		pdu_len += 3;
		BYTE *pCur(ADU);
		while(pdu_len--)
			Send(pCur++, 1);
		*/
		Send(ADU, pdu_len + 3);
	}
	else
		throw ModbusException(2);
}

BYTE CZETModbus::GetResponsePDU(BYTE* pPDU, BYTE& PDU_len)
{
	WORD check_sum;
	BYTE buff[MAX_ADU_SIZE];
	
	memset(buff, 0x00, MAX_ADU_SIZE);
	Recv(buff, 4);

	if(!(buff[1] & 0x80))
	{
		switch(buff[1])
		{
			case 0x03 :
			case 0x04 : PDU_len = buff[2] + 2; break;
			case 0x10 : PDU_len = 5; break;
			case 0x64 : 
			{
				switch(buff[2])
				{
					case 0x00:
					{
						PDU_len = buff[3] + 3;
						break;
					}
					default: throw ModbusException(3);
				}
				break;
			}
			default : throw ModbusException(3);
		}
	}
	else
		PDU_len = 2;

	if(PDU_len > MAX_PDU_SIZE || PDU_len < MIN_PDU_SIZE)
		throw ModbusException(3);

	Recv(&buff[4], PDU_len - 1);

	check_sum = (buff[PDU_len + 1] << 8) | buff[PDU_len + 2];
	if(check_sum != CheckSumm(buff, PDU_len + 1))
		throw ModbusException(16);
	
	memcpy_s(pPDU, PDU_len, &buff[1], PDU_len);
	
	return buff[0];
}

WORD CZETModbus::CheckSumm(BYTE* pBuff, unsigned int len)
{
	WORD CRC = 0xFFFF;
	int idx;

	while(len--)
	{
		CRC ^= *pBuff++;
		for(idx = 0; idx < 8; idx++)
			CRC = (CRC & 0x01) ? (CRC >> 1) ^ 0xA001 : CRC >> 1;
	}
	return CRC >> 8 | CRC << 8;
}