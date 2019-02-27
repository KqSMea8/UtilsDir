#pragma once
#include <Windows.h>
#include <winsock.h>
// #include <ws2tcpip.h>
#include "io/modbus/frl_modbus_defines.h"
#include "io/comm_ports/frl_comm_ports_serial_fh.h"

namespace frl{ namespace io{ namespace modbus{

#define interfaces	public
#define methods		private

class Modbus
{
private:
	ModbusProtocol	mbProtocol;
	ModbusType		mbState;
	String			portName;
	DWORD			speed;
	String			ip;
	union
	{
		HANDLE hSerial;
		SOCKET hSocket;
	};
	// ������������� ������� (������ ��� TCP)
	u16 mbTransaction;

public:
	Modbus();
	Modbus( ModbusProtocol Protocol, ModbusType Type = MODBUS_MASTER );
	~Modbus();

methods:
	String getName( DWORD Port );
	String int2String( DWORD num );

interfaces:

	// ��������� COM-����
	// @ CString	Portname	- ��� �����
	// $ ���������� ���������� �����
	HANDLE	open					( String PortName );

	// ��������� COM-����
	// @ DWORD	Port	- ����� �����
	// $ ���������� ���������� �����
	HANDLE	open					( DWORD Port );

	// ��������� TCP-����������
	// @ CString	HostName	- ip-�����/��� �����
	// @ DWORD		Port		- ����� �����
	// $ ���������� ���������� ��������� ����������
	SOCKET	open					( String HostName, DWORD Port );

	// ��������� COM-����
	void	close					();

	// ��������� COM-�����
	// @ DWORD	BaudRate	- ��������
	// @ BYTE	ByteSize	- ����� ������
	// @ BYTE	StopBits	- ���������� ����-�����
	// $ ���������� false � ������ ������
	bool	setup					( DWORD BaudRate, BYTE ByteSize, BYTE StopBits );

	// ������ ������ � ����
	// @ LPCVOID	Buffer	- ������ ��� ������
	// @ DWORD		Length	- ����� ������, � ������
	// @ SOCKET		Client	- ����� ������� (������ ���  TCP)
	// $ ���������� ���������� ���������� ����
	DWORD	write					( LPCVOID Buffer, DWORD Length, SOCKET Client = INVALID_SOCKET );

	// ������ ������ �� �����
	// @ LPVOID		Buffer	- �����, ���� ��������� ����������� ������
	// @ DWORD		Length	- ������������ ����� ������, � ������
	// @ SOCKET		Client	- ����� ������� (������ ���  TCP)
	// $ ���������� ����� ����������� ����
	DWORD	read					( LPVOID Buffer, DWORD Length, SOCKET Client = INVALID_SOCKET );

	// ��������� ����������� �����
	// $ ���������� ���������� �����
	HANDLE	getHandle				();

	// ��������� ����������� ������
	// $ ���������� ���������� ������
	SOCKET	getSocket				();

	// ��������� ����� �����
	// $ ���������� ��� �����
	String	getPortName				();

	// ��������� �������� �� �����
	// $ ���������� �������� �����
	DWORD	getPortSpeed			();

	// ��������� ip-������
	// $ ���������� ip-�����
	String	getIP					();

	// ����������� �� ������ ���� � ����������� ������
	// @ const u8	*ADU		- ��������� �����
	// @ u8			ADULength	- ����� ���������� ������
	// $ ���������� ���������� ����������� �����
	u16	getCRC16					( const u8 *ADU, u8 ADULength );

	// ��������� ����������� ����� ��� ��������� ������
	// @ const u8	*ADU	- �����, ��� �������� ����� ��������� ����������� �����
	// @ u8			Length	- ����� ������, ��� �������� ����������� CRC
	// $ ���������� ����������� ����������� �����
	u16	CRC16						( const u8 *ADU, u8 Length );

	// �������� ADU �� MBAP + ADU
	// @ u8		*ADU		- ����� ������ � MBAP, �� �������� ������������� ADU
	//						  ��������� ��������� � ��� �� �����
	// @ u8		ADULength	- ����� ��������� ADU-������
	// $ ���������� ����� ADU-������
	u8	getADU						( u8 *ADU, u8 ADULength );

	// �������� PDU �� ADU
	// @ u8		*PDU		- �����, ���� ��������� PDU
	// @ u8		*ADU		- ���������� ADU-�����
	// @ u8		ADULength	- ����� ADU-������
	// $ ���������� ����� PDU-������
	u8	getPDU						( u8 *PDU, u8 *ADU, u8 ADULength );

	// �������� ������ �� PDU
	// @ u8		*Data		- �����, ���� ��������� ������
	// @ u8		*PDU		- ���������� PDU-�����
	// @ u8		PDULength	- ����� PDU-������
	// $ ���������� ����� ������
	u8	getData						( u8 *Data, u8 *PDU, u8 PDULength );

	// �������� ��� ������� �� PDU-������
	// @ u8	*PDU	- PDU-�����, �� �������� ������������� ��� �������
	// $ ���������� ��� �������
	ModbusFunction	getFunctionCode	( const u8 *PDU );

	// �������� ����� ����������
	// @ const u8	*ADU	- ADU-�����
	// $ ���������� ����� ����������
	u8	getAddress					( const u8 *ADU );

	// �������� ����� ��������
	// @ const u8	*PDU	- PDU-�����
	// $ ���������� ����� ������� ��������
	u16	getStartAddress				( const u8 *PDU );

	// �������� ���������� ���������
	// @ const u8	*PDU	- PDU-�����
	// $ ���������� ���������� ���������
	u16	getNumberFlags				( const u8 *PDU );

	// ���������� PDU-�����
	// @ u8				*PDU		- �����, ���� ��������� ���������� PDU-�����
	// @ ModbusFunction Func		- ��� �������
	// @ const u8		*Data		- ������
	// @ u8				DataLength	- ����� ������ � �������
	// $ ���������� ����� ������������� PDU-������
	u8	makePDU						( u8 *PDU, ModbusFunction Func, const u8 *Data, u8 DataLength );
	u8	makePDU2					( u8 *PDU, ModbusFunction Func, const u8 *Data, u8 DataLength );

	// ���������� ADU-�����
	// @ u8				*ADU		- �����, ���� ��������� ���������� ADU-�����
	// @ u8				SlaveAddress- ����� slave-����������
	// @ const u8		*PDU		- PDU-�����, �� ������ �������� ������������ ADU-�����
	// @ u8				PDULength	- ����� PDU-������
	// $ ���������� ����� ������������� ADU-������
	u8	makeADU						( u8 *ADU, u8 SlaveAddress, const u8 *PDU, u8 PDULength );

	// ������ ������ MODBUS_READ_COILS (0x01)
	// @ u8		SlaveAddress	- ����� �������� ����������
	// @ u16	StartAddress	- ����� ������� �����
	// @ u16	qCoils			- ���������� ������
	// $ ���������� ���������� ���� � �������
	u8	readCoils					( u8 SlaveAddress, u16 StartAddress, u16 qCoils );

	// ������ ������ MODBUS_READ_DISCRETE_INPUTS (0x02)
	// @ u8		SlaveAddress	- ����� �������� ����������
	// @ u16	StartAddress	- ����� ������� �����
	// @ u16	qInputs			- ���������� ������
	// $ ���������� ���������� ���� � �������
	u8	readDiscreteInputs			( u8 SlaveAddress, u16 StartAddress, u16 qInputs );

	// ������ ������ MODBUS_READ_HOLDING_REGISTERS (0x03)
	// @ u8		SlaveAddress	- ����� �������� ����������
	// @ u16	StartAddress	- ����� ������� ��������
	// @ u16	nRegisters		- ���������� ���������
	// $ ���������� ���������� ���� � �������
	u8	readHoldingRegisters		( u8 SlaveAddress, u16 StartAddress, u16 nRegisters );

	// ������ ������ MODBUS_READ_INPUT_REGISTERS (0x04)
	// @ u8		SlaveAddress	- ����� �������� ����������
	// @ u16	StartAddress	- ����� ������� ��������
	// @ u16	nRegisters		- ���������� ���������
	// $ ���������� ���������� ���� � �������
	u8	readInputRegisters			( u8 SlaveAddress, u16 StartAddress, u16 nRegisters );

	VARIANT	getValue( u8 *ADU, u8 ADULength, VARTYPE type );
};

}
}
}
