#ifndef circ_buff_basic_zet_logger
#define circ_buff_basic_zet_logger

#include <zet_struct1.h>
#include <ZetErrorCodes.h>

#define ZETLOGGER_MAXIMUM_STRUCTURE_LENGTH 16384	// Максимальная длина структуры
#define ZETLOGGER_CIRC_BUFF_SIZE 65536	// Память выделена из расчета записи данных в файл по ZETLOGGER_CIRC_BUFF_SIZE/2 байт

// Структура описывающая циклический буфер для хранения данных
struct CircBuffBasicZetLoggerStruct
{
	char _buffer[ZETLOGGER_CIRC_BUFF_SIZE];		// Физический буфер циклического буфера для хранения структур

	long _posWriteCircBuff;		// Указатель на позицию записи циклического буфера
	long _posReadCircBuff;		// Указатель на позицию чтения циклического буфера
};

// Данный класс реализует циклический буфер хранения данных в структурах
class CCircBuffBasicZetLogger
{
private:
	CircBuffBasicZetLoggerStruct* _circBuffStruct;	// Указатель на структуру описывающую циклический буфер (сделано через указатели для использования Shared Memory)

public:
	CCircBuffBasicZetLogger();
	CCircBuffBasicZetLogger(CircBuffBasicZetLoggerStruct* circBuffStruct_);
	~CCircBuffBasicZetLogger();
	
	// Проверка проинициализированности класса (классу дали указатель на структуру с которой он должен работать)
	// Возвращаемые значения: true - класс проинициализирован; false - класс не проинициализирован
	bool isValid();
	// Сброс указателей циклического буфера в начальное положение
	void reset();

	// Запись данных в циклический буфер
	// Возвращаемое значение: >=0 - количество записанных данных; <0 - буфер не проинициализирован
	long writeStructures(char* structures_, long size_);
	// Чтение данных из циклического буфера
	// Возвращаемое значение: >0 - количество считанных данных, <0 - буфер не проинициализирован
	long readStructures(char* structures_, long size_);
	// Чтение данных из циклического буфера во временный буфер (без сдвига указателя чтения)
	// Возвращаемое значение:  >0 - количество считанных данных, <0 - буфер не проинициализирован
	long readTempStructures(char* structures_, long size_);
	
	// Определение свободного места циклического буфера под запись данных
	// Возвращаемое значение: размер свободного места циклического буфера под запись данных
	long getSizeWrite();
	// Определение размера хранящихся данных в циклическом буфере для чтения
	// Возвращаемое значение: размер хранящихся данных
	long getSizeRead();
};

#endif