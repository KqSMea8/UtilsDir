#pragma once

#include <objbase.h>

#define MAX_PDU_SIZE			253  					//Максимальный размер пакета Modbus (PDU) в байтах 
#define MIN_PDU_SIZE			1  						//Минимальный  размер пакета Modbus (PDU) в байтах
#define MAX_ADU_SIZE			(MAX_PDU_SIZE + 3)  	//Максимальный размер пакета Modbus (ADU) в байтах 

//Ошибки, возвращаемые в ответе устройством
#define MODBUS_ERR_UNKNOWN_ADDR		0x02	//Указан неизвестный адрес при доступе к регистрам устройства
#define MODBUS_ERR_DEV_BUSY			0x06	//Устройство в настоящий момент занято

interface IZETModbus
{
	virtual BYTE ReadCoilStatus(BYTE dev_addr, WORD addr, WORD* pBuff, unsigned int flags_num) = 0;			//Получить значения нескольких регистров флагов
	virtual BYTE ReadDiscreteInputs(BYTE dev_addr, WORD addr, WORD* pBuff, unsigned int inputs_num) = 0;	//Получить значения нескольких дискретных входов
	virtual BYTE ReadHoldingRegisters(BYTE dev_addr, WORD addr, WORD* pBuff, unsigned int& regs_num) = 0;	//Получить значения нескольких регистров хранения
	virtual BYTE ReadInputRegisters(BYTE dev_addr, WORD addr, WORD* pBuff, unsigned int& regs_num) = 0;		//Получить значения нескольких регистров ввода
	
	virtual BYTE ForceSingleCoil(BYTE dev_addr, WORD addr, bool value) = 0;									//Записать значение в регистр флага
	virtual BYTE PresetSingleRegister(BYTE dev_addr, WORD addr, WORD value) = 0;							//Записать значение в регистр ввода
	virtual BYTE ForceMultipleCoils(BYTE dev_addr, WORD addr, WORD* pBuff, unsigned int flags_num) = 0;		//Записать данные в несколько регистров флагов
	virtual BYTE PresetMultipleRegisters(BYTE dev_addr, WORD addr, WORD* pBuff, unsigned int &regs_num) = 0;//Записать данные в несколько регистров ввода
};

#include <Exception/Exception.h>

class ModbusException : public Exception
{
public:
	ModbusException(unsigned int err) : Exception(err) {};
};