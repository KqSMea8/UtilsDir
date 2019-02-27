#pragma once

#include <vector>

#include <Exception/Exception.h>
#include <Zet7xxxDefinitions.h>
#include <ZETModbus/IZETModbus.h>

#define DEV_BUSY_ATTEMPT		200			//���������� ������� ��������� � �������� �������
#define DEV_BUSY_WAIT			20			//����� ����� ��������� ��������� � �������� ������� (��)

struct StructInfo
{
	STRUCT_HEAD		head;					//��������� ���������
	WORD			addr;					//����� ��������� � ������� ����� ��������� ����������
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

	void				GetStructsInfo(std::vector<StructInfo>& structs_info);			//����������� ������ ���������� �������� ����������
	void				ReadStruct(WORD addr, void* pBuff, WORD size);					//��������� ��������� �� ����������
	void				WriteStruct(WORD addr, void* pBuff);							//�������� ��������� � ����������
	
	unsigned int		ReadRegisters(WORD reg_addr, void* pBuff, BYTE regs_num);		//��������� �������� �� ����������
	unsigned int		WriteRegisters(WORD reg_addr, void* pBuff, BYTE regs_num);		//��������� ����������� ��������� ����������

//	void				SaveFlash(void);												//��������� ������� ��������� �� ��������� flash ����������
protected:

private:
	BYTE				m_DevAddr;			//����� �������� ����������
	IZETModbus*			m_pModbus;			//��������� �������� ����������
};
