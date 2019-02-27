#pragma once

#include <vector>

#include <Exception/Exception.h>
#include <Zet7xxxDefinitions.h>
#include <ZETModbus/IZETModbus.h>

#define DEV_BUSY_ATTEMPT		200			//Количество попыток обращения к занятому прибору
#define DEV_BUSY_WAIT			20			//Время между попытками обращения к занятому прибору (мс)

struct StructInfo
{
	STRUCT_HEAD		head;					//Заголовок структуры
	WORD			addr;					//Адрес структуры в составе общей структуры устройства
};

class ZRemoteDevException : public Exception
{
public:
	ZRemoteDevException(unsigned int err) : Exception(err) {};
};

class ZRemoteDev
{
public:
	ZRemoteDev(IZETModbus *pModBus, BYTE dev_addr);
	virtual ~ZRemoteDev(void);

	void				GetStructsInfo(std::vector<StructInfo>& structs_info);			//Определение общего количества структур устройства
	void				ReadStruct(WORD addr, void* pBuff, WORD size);					//Прочитать структуру из устройства
	void				WriteStruct(WORD addr, void* pBuff);							//Записать структуру в устройство
	
	unsigned int		ReadRegisters(WORD reg_addr, void* pBuff, BYTE regs_num);		//Прочитать регистры из устройства
	unsigned int		WriteRegisters(WORD reg_addr, void* pBuff, BYTE regs_num);		//Установка содержимого регистров устройства

//	void				SaveFlash(void);												//Сохранить текущие настройки во внуренней flash устройства
protected:

private:
	BYTE				m_DevAddr;			//Адрес текущего устройства
	IZETModbus*			m_pModbus;			//Интерфейс текущего устройства
};
