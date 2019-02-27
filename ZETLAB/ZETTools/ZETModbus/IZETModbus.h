#pragma once

#include <objbase.h>

#define MAX_PDU_SIZE			253  					//������������ ������ ������ Modbus (PDU) � ������ 
#define MIN_PDU_SIZE			1  						//�����������  ������ ������ Modbus (PDU) � ������
#define MAX_ADU_SIZE			(MAX_PDU_SIZE + 3)  	//������������ ������ ������ Modbus (ADU) � ������ 

//������, ������������ � ������ �����������
#define MODBUS_ERR_UNKNOWN_ADDR		0x02	//������ ����������� ����� ��� ������� � ��������� ����������
#define MODBUS_ERR_DEV_BUSY			0x06	//���������� � ��������� ������ ������

interface IZETModbus
{
	virtual BYTE ReadCoilStatus(BYTE dev_addr, WORD addr, WORD* pBuff, unsigned int flags_num) = 0;			//�������� �������� ���������� ��������� ������
	virtual BYTE ReadDiscreteInputs(BYTE dev_addr, WORD addr, WORD* pBuff, unsigned int inputs_num) = 0;	//�������� �������� ���������� ���������� ������
	virtual BYTE ReadHoldingRegisters(BYTE dev_addr, WORD addr, WORD* pBuff, unsigned int& regs_num) = 0;	//�������� �������� ���������� ��������� ��������
	virtual BYTE ReadInputRegisters(BYTE dev_addr, WORD addr, WORD* pBuff, unsigned int& regs_num) = 0;		//�������� �������� ���������� ��������� �����
	
	virtual BYTE ForceSingleCoil(BYTE dev_addr, WORD addr, bool value) = 0;									//�������� �������� � ������� �����
	virtual BYTE PresetSingleRegister(BYTE dev_addr, WORD addr, WORD value) = 0;							//�������� �������� � ������� �����
	virtual BYTE ForceMultipleCoils(BYTE dev_addr, WORD addr, WORD* pBuff, unsigned int flags_num) = 0;		//�������� ������ � ��������� ��������� ������
	virtual BYTE PresetMultipleRegisters(BYTE dev_addr, WORD addr, WORD* pBuff, unsigned int &regs_num) = 0;//�������� ������ � ��������� ��������� �����
};

#include <Exception/Exception.h>

class ModbusException : public Exception
{
public:
	ModbusException(unsigned int err) : Exception(err) {};
};