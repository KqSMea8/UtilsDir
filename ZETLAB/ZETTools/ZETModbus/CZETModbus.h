#pragma once

#include "IZETModbus.h"

class CZETModbus : public IZETModbus
{
public:
	CZETModbus(void);
	virtual ~CZETModbus(void) = 0;
					
	virtual BYTE	ReadCoilStatus(BYTE dev_addr, WORD addr, WORD* pBuff, unsigned int flags_num);			//Получить значения нескольких регистров флагов
	virtual BYTE	ReadDiscreteInputs(BYTE dev_addr, WORD addr, WORD* pBuff, unsigned int inputs_num);		//Получить значения нескольких дискретных входов
	virtual BYTE	ReadHoldingRegisters(BYTE dev_addr, WORD addr, WORD* pBuff, unsigned int& regs_num);	//Получить значения нескольких регистров хранения
	virtual BYTE	ReadInputRegisters(BYTE dev_addr, WORD addr, WORD* pBuff, unsigned int& regs_num);		//Получить значения нескольких регистров ввода
	
	virtual BYTE	ForceSingleCoil(BYTE dev_addr, WORD addr, bool value);									//Записать значение в регистр флага
	virtual BYTE	PresetSingleRegister(BYTE dev_addr, WORD addr, WORD value);								//Записать значение в регистр ввода
	virtual BYTE	ForceMultipleCoils(BYTE dev_addr, WORD addr, WORD* pBuff, unsigned int flags_num);		//Записать данные в несколько регистров флагов
	virtual BYTE	PresetMultipleRegisters(BYTE dev_addr, WORD addr, WORD* pBuff, unsigned int &regs_num);	//Записать данные в несколько регистров ввода

	void			SetConnectionParams(unsigned int br, unsigned int parity);								//Широковещательный запрос на изменение параметров связи всем подчиненным
	BYTE			GetHierarchicalChain(BYTE dev_addr, BYTE* pBuff);										//Получение иерархической цепочки для заданного узла

protected:
	virtual void	Recv(BYTE* pBuff, unsigned int num_bytes) = 0;						//Получить данные
	virtual void	Send(BYTE* pBuff, unsigned int num_bytes) = 0;						//Отправить данные

	void			SendCommandPDU(BYTE dev_addr, BYTE* pPDU);							//Отправить команду 
	BYTE			GetResponsePDU(BYTE* pPDU, BYTE& PDU_len);							//Получить ответ
	WORD			CheckSumm(BYTE* pBuff, unsigned int len);							//Вычислить контральную сумму
};