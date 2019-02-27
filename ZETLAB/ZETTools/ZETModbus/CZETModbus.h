#pragma once

#include "IZETModbus.h"

class CZETModbus : public IZETModbus
{
public:
	CZETModbus(void);
	virtual ~CZETModbus(void) = 0;
					
	virtual BYTE	ReadCoilStatus(BYTE dev_addr, WORD addr, WORD* pBuff, unsigned int flags_num);			//�������� �������� ���������� ��������� ������
	virtual BYTE	ReadDiscreteInputs(BYTE dev_addr, WORD addr, WORD* pBuff, unsigned int inputs_num);		//�������� �������� ���������� ���������� ������
	virtual BYTE	ReadHoldingRegisters(BYTE dev_addr, WORD addr, WORD* pBuff, unsigned int& regs_num);	//�������� �������� ���������� ��������� ��������
	virtual BYTE	ReadInputRegisters(BYTE dev_addr, WORD addr, WORD* pBuff, unsigned int& regs_num);		//�������� �������� ���������� ��������� �����
	
	virtual BYTE	ForceSingleCoil(BYTE dev_addr, WORD addr, bool value);									//�������� �������� � ������� �����
	virtual BYTE	PresetSingleRegister(BYTE dev_addr, WORD addr, WORD value);								//�������� �������� � ������� �����
	virtual BYTE	ForceMultipleCoils(BYTE dev_addr, WORD addr, WORD* pBuff, unsigned int flags_num);		//�������� ������ � ��������� ��������� ������
	virtual BYTE	PresetMultipleRegisters(BYTE dev_addr, WORD addr, WORD* pBuff, unsigned int &regs_num);	//�������� ������ � ��������� ��������� �����

	void			SetConnectionParams(unsigned int br, unsigned int parity);								//����������������� ������ �� ��������� ���������� ����� ���� �����������
	BYTE			GetHierarchicalChain(BYTE dev_addr, BYTE* pBuff);										//��������� ������������� ������� ��� ��������� ����

protected:
	virtual void	Recv(BYTE* pBuff, unsigned int num_bytes) = 0;						//�������� ������
	virtual void	Send(BYTE* pBuff, unsigned int num_bytes) = 0;						//��������� ������

	void			SendCommandPDU(BYTE dev_addr, BYTE* pPDU);							//��������� ������� 
	BYTE			GetResponsePDU(BYTE* pPDU, BYTE& PDU_len);							//�������� �����
	WORD			CheckSumm(BYTE* pBuff, unsigned int len);							//��������� ����������� �����
};