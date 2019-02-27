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
	// Идентификатор запроса (только для TCP)
	u16 mbTransaction;

public:
	Modbus();
	Modbus( ModbusProtocol Protocol, ModbusType Type = MODBUS_MASTER );
	~Modbus();

methods:
	String getName( DWORD Port );
	String int2String( DWORD num );

interfaces:

	// Открывает COM-порт
	// @ CString	Portname	- имя порта
	// $ Возвращает дескриптор порта
	HANDLE	open					( String PortName );

	// Открывает COM-порт
	// @ DWORD	Port	- номер порта
	// $ Возвращает дескриптор порта
	HANDLE	open					( DWORD Port );

	// Открывает TCP-соединение
	// @ CString	HostName	- ip-адрес/имя хоста
	// @ DWORD		Port		- номер порта
	// $ Возвращает дескриптор открытого соединения
	SOCKET	open					( String HostName, DWORD Port );

	// Закрывает COM-порт
	void	close					();

	// Настройка COM-порта
	// @ DWORD	BaudRate	- скорость
	// @ BYTE	ByteSize	- битов данных
	// @ BYTE	StopBits	- количество стоп-битов
	// $ Возвращает false в случае ошибки
	bool	setup					( DWORD BaudRate, BYTE ByteSize, BYTE StopBits );

	// Запись данных в порт
	// @ LPCVOID	Buffer	- данные для записи
	// @ DWORD		Length	- длина данных, в байтах
	// @ SOCKET		Client	- сокет клиента (только для  TCP)
	// $ Возвращает количество записанных байт
	DWORD	write					( LPCVOID Buffer, DWORD Length, SOCKET Client = INVALID_SOCKET );

	// Чтение данных из порта
	// @ LPVOID		Buffer	- буфер, куда запишутся прочитанные данные
	// @ DWORD		Length	- максимальная длина буфера, в байтах
	// @ SOCKET		Client	- сокет клиента (только для  TCP)
	// $ Возвращает число прочитанных байт
	DWORD	read					( LPVOID Buffer, DWORD Length, SOCKET Client = INVALID_SOCKET );

	// Получение дескриптора порта
	// $ Возвращает дескриптор порта
	HANDLE	getHandle				();

	// Получение дескриптора сокета
	// $ Возвращает дескриптор сокета
	SOCKET	getSocket				();

	// Получение имени порта
	// $ Возвращает имя порта
	String	getPortName				();

	// Получение скорости на порту
	// $ Возвращает скорость порта
	DWORD	getPortSpeed			();

	// Получение ip-адреса
	// $ Возвращает ip-адрес
	String	getIP					();

	// Вытаскивает из пакета зону с контрольной суммой
	// @ const u8	*ADU		- пришедший пакет
	// @ u8			ADULength	- длина пришедшего пакета
	// $ Возвращает полученную контрольную сумму
	u16	getCRC16					( const u8 *ADU, u8 ADULength );

	// Вычисляет контрольную сумму для заданного пакета
	// @ const u8	*ADU	- пакет, для которого нужно вычислить контрольную сумму
	// @ u8			Length	- длина пакета, для которого вычисляется CRC
	// $ Возвращает вычисленную контрольную сумму
	u16	CRC16						( const u8 *ADU, u8 Length );

	// Получает ADU из MBAP + ADU
	// @ u8		*ADU		- пакет вместе с MBAP, из которого вытаскивается ADU
	//						  результат запишется в тот же буфер
	// @ u8		ADULength	- длина входящего ADU-пакета
	// $ Возвращает длину ADU-пакета
	u8	getADU						( u8 *ADU, u8 ADULength );

	// Получает PDU из ADU
	// @ u8		*PDU		- буфер, куда запишется PDU
	// @ u8		*ADU		- полученный ADU-пакет
	// @ u8		ADULength	- длина ADU-пакета
	// $ Возвращает длину PDU-пакета
	u8	getPDU						( u8 *PDU, u8 *ADU, u8 ADULength );

	// Получает данные из PDU
	// @ u8		*Data		- буфер, куда запишутся данные
	// @ u8		*PDU		- полученный PDU-пакет
	// @ u8		PDULength	- длина PDU-пакета
	// $ Возвращает длину данных
	u8	getData						( u8 *Data, u8 *PDU, u8 PDULength );

	// Получает код функции из PDU-пакета
	// @ u8	*PDU	- PDU-пакет, из которого вытаскивается код функции
	// $ Возвращает код функции
	ModbusFunction	getFunctionCode	( const u8 *PDU );

	// Получает адрес устройства
	// @ const u8	*ADU	- ADU-пакет
	// $ Возвращает адрес устройства
	u8	getAddress					( const u8 *ADU );

	// Получает адрес регистра
	// @ const u8	*PDU	- PDU-пакет
	// $ Возвращает адрес первого регистра
	u16	getStartAddress				( const u8 *PDU );

	// Получает количество регистров
	// @ const u8	*PDU	- PDU-пакет
	// $ Возвращает количество регистров
	u16	getNumberFlags				( const u8 *PDU );

	// Составляет PDU-пакет
	// @ u8				*PDU		- буфер, куда запишется полученный PDU-пакет
	// @ ModbusFunction Func		- код функции
	// @ const u8		*Data		- данные
	// @ u8				DataLength	- длина буфера с данными
	// $ Возвращает длину составленного PDU-пакета
	u8	makePDU						( u8 *PDU, ModbusFunction Func, const u8 *Data, u8 DataLength );
	u8	makePDU2					( u8 *PDU, ModbusFunction Func, const u8 *Data, u8 DataLength );

	// Составляет ADU-пакет
	// @ u8				*ADU		- буфер, куда запишется полученный ADU-пакет
	// @ u8				SlaveAddress- адрес slave-устройства
	// @ const u8		*PDU		- PDU-пакет, на основе которого составляется ADU-пакет
	// @ u8				PDULength	- длина PDU-пакета
	// $ Возвращает длину составленного ADU-пакета
	u8	makeADU						( u8 *ADU, u8 SlaveAddress, const u8 *PDU, u8 PDULength );

	// Делает запрос MODBUS_READ_COILS (0x01)
	// @ u8		SlaveAddress	- адрес ведомого устройства
	// @ u16	StartAddress	- адрес первого флага
	// @ u16	qCoils			- количество флагов
	// $ Возвращает количество байт в запросе
	u8	readCoils					( u8 SlaveAddress, u16 StartAddress, u16 qCoils );

	// Делает запрос MODBUS_READ_DISCRETE_INPUTS (0x02)
	// @ u8		SlaveAddress	- адрес ведомого устройства
	// @ u16	StartAddress	- адрес первого флага
	// @ u16	qInputs			- количество флагов
	// $ Возвращает количество байт в запросе
	u8	readDiscreteInputs			( u8 SlaveAddress, u16 StartAddress, u16 qInputs );

	// Делает запрос MODBUS_READ_HOLDING_REGISTERS (0x03)
	// @ u8		SlaveAddress	- адрес ведомого устройства
	// @ u16	StartAddress	- адрес первого регистра
	// @ u16	nRegisters		- количество регистров
	// $ Возвращает количество байт в запросе
	u8	readHoldingRegisters		( u8 SlaveAddress, u16 StartAddress, u16 nRegisters );

	// Делает запрос MODBUS_READ_INPUT_REGISTERS (0x04)
	// @ u8		SlaveAddress	- адрес ведомого устройства
	// @ u16	StartAddress	- адрес первого регистра
	// @ u16	nRegisters		- количество регистров
	// $ Возвращает количество байт в запросе
	u8	readInputRegisters			( u8 SlaveAddress, u16 StartAddress, u16 nRegisters );

	VARIANT	getValue( u8 *ADU, u8 ADULength, VARTYPE type );
};

}
}
}
