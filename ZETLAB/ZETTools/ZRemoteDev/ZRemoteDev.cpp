#include "ZRemoteDev.h"

using namespace std;

ZRemoteDev::ZRemoteDev(IZETModbus *pModBus, BYTE dev_addr) : m_pModbus(pModBus), m_DevAddr(dev_addr)
{
	if(!pModBus)
		throw ZRemoteDevException(0);
}

ZRemoteDev::~ZRemoteDev(void)
{
}

void ZRemoteDev::GetStructsInfo(std::vector<StructInfo>& structs_info)
{
	structs_info.clear();
	unsigned int cur_struct_addr(0);
	STRUCT_HEAD cur_struct_head;
	int iCommonStructCounter(0);
	do
	{
		ReadRegisters(cur_struct_addr, &cur_struct_head, sizeof(cur_struct_head)/2);
		if(cur_struct_head.size)
		{
			StructInfo info;
			info.head = cur_struct_head;
			info.addr = cur_struct_addr;
			
			structs_info.push_back(info);
			cur_struct_addr += info.head.size/2;
			if (cur_struct_head.struct_type == COMMON_STRUCT)
				iCommonStructCounter++;
			if (iCommonStructCounter == 3)
				break;
		}
	}
	while(cur_struct_head.size);
}

void ZRemoteDev::ReadStruct(WORD addr, void* pBuff, WORD size)
{
	unsigned int struct_sz;				//Размер структуры в словах

	//Читаем размер структуры
	ReadRegisters(addr, pBuff, sizeof(STRUCT_HEAD) / 2);
	struct_sz = ((pSTRUCT_HEAD) pBuff)->size;
	
	if(struct_sz > sizeof(STRUCT_HEAD) / 2 && struct_sz < MAX_STRUCT_SIZE)
	{
		if(struct_sz > size)
			struct_sz = size;

		ReadRegisters(addr, ((PBYTE) pBuff), struct_sz / 2);
	}
	else
		throw ZRemoteDevException(0); //Некорректный размер структуры 
}

void ZRemoteDev::WriteStruct(WORD addr, void* pBuff)
{
	STRUCT_HEAD	head;
	WORD		size(((pSTRUCT_HEAD) pBuff)->size);

	ReadRegisters(addr, &head, sizeof(STRUCT_HEAD)/2);

	if(head.struct_type == ((pSTRUCT_HEAD) pBuff)->struct_type)
	{
		PBYTE	pSrc((PBYTE) pBuff); pSrc += sizeof(STRUCT_HEAD);
		WORD	write_addr(addr + sizeof(STRUCT_HEAD)/2);
		WORD	we[2];
		we[0] = 0x01;
		we[1] = 0;
		
		//Устанавливаем WE
		WriteRegisters(addr + sizeof(unsigned int)/2, &we, sizeof(long)/2);

		if(size > head.size)
			size = head.size;
		size = (size - sizeof(STRUCT_HEAD))/2;

		switch(head.struct_type)
		{
			case CHANNEL_PAR_STRUCT:
			{
				//В структуре канала нельзя менять поле value
				size -= sizeof(float)/2;
				write_addr += sizeof(float)/2;
				pSrc += sizeof(float);
				break;
			}
			case ADC_PAR_STRUCT:
			{
				//В структуре АЦП нельзя менять поле freq (данное поле может изменяться только через структуру канала)
				size -= sizeof(float)/2;
				write_addr += sizeof(float)/2;
				pSrc += sizeof(float);
				break;
			}
			case DATA_BUFFER_STRUCT:
			{
				WriteRegisters(addr, pBuff, 2);
				break;
			}
		}
		WriteRegisters(write_addr, pSrc, (BYTE) size);

		try
		{
			//Сбрасываем WE
			we[0] = 0x00000000;
			WriteRegisters(addr + sizeof(unsigned int)/2, we, sizeof(long)/2);
		}
		catch(...)
		{
			//При изменении скоростных параметров устройство или перезагрузки устройства не сможет ответить
//			if(	head.struct_type == UART_PAR_STRUCT ||
//				(head.struct_type == COMMAND_STRUCT && ((PDEV_COMMAND) pBuff)->id == REBOOT_DEV))

			//При изменении скоростных параметров устройства не сможет ответить
			if(	head.struct_type == UART_PAR_STRUCT)
			{
			}
			else
				throw ZRemoteDevException(10);
		}
	}
	else
		throw ZRemoteDevException(11);
}

unsigned int ZRemoteDev::ReadRegisters(WORD reg_addr, void* pBuff, BYTE regs_num)
{
	unsigned int	ret_val(regs_num);
	BYTE			err;
	unsigned short	attempt_num(DEV_BUSY_ATTEMPT);

	while((err = m_pModbus->ReadHoldingRegisters(m_DevAddr, reg_addr, (PWORD) pBuff, ret_val)) == MODBUS_ERR_DEV_BUSY)
	{
		if(!attempt_num--)
			throw ZRemoteDevException(6);
		Sleep(DEV_BUSY_WAIT);
	}
	if(err)
		throw ZRemoteDevException(0);

	return ret_val;
}

unsigned int ZRemoteDev::WriteRegisters(WORD reg_addr, void* pBuff, BYTE regs_num)
{
	unsigned int	ret_val(regs_num);
	BYTE			err;
	unsigned short	attempt_num(DEV_BUSY_ATTEMPT);

	while((err = m_pModbus->PresetMultipleRegisters(m_DevAddr, reg_addr, (PWORD) pBuff, ret_val)) == MODBUS_ERR_DEV_BUSY)
	{
		if(!attempt_num--)
			throw ZRemoteDevException(6);
		Sleep(DEV_BUSY_WAIT);
	}
	
	if(err)
		throw ZRemoteDevException(0);

	return ret_val;
}

#if 0 // NOT USED
void ZRemoteDev::SaveFlash(void)
{
	DEV_COMMAND	commant_struct;
	WORD		commant_struct_addr(0);

	do
	{
		//Определяем адрес структуры, определяющей команду устройству
		ReadRegisters(commant_struct_addr, &commant_struct, sizeof(STRUCT_HEAD)/2);
		if(commant_struct.head.struct_type == COMMAND_STRUCT)
		{
			if(commant_struct.head.status != COMMAND_COMPLETE && commant_struct.head.status != WRONG_COMMAND)
				throw ZRemoteDevException(0); //Устройство занято выполнением другой команды

			//Посылаем команду сохранения данных
			commant_struct.id = SAVE_STRUCT;
			WriteStruct(commant_struct_addr, &commant_struct);
		}
		commant_struct_addr += commant_struct.head.size/2;
	}
	while(commant_struct.head.size);
}
#endif
