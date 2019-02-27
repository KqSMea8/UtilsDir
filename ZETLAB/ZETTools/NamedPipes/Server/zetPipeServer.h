// Класс для обмена данными между программами через именованные каналы (сервер)
// Версия 2.1.1 @ 2017/04/13
//
// Класс принимает данные от клиентов по именованным каналам и обрабатывает их с помощью определенной CallBack функции
// Принцип действия: При инициализации создается порт завершения (IOCompletionPort), и один поток для обработки Overlapped от всех событий от созданных именованных каналов.
//                   Создается один именовнный канал. При подключении к нему, создается ещё один новый канал. При отключении от именованного канала экземпляр удаляется.
//
// Использование:
// - Проинициализировать экземпляр класса: 
//   CZetPipeServer pipeComm(*Имя именованного канала*, 
//                               *Функция обработчик полученной команды по именованному каналу*,
//                               *Аргумент сторонний*);
//   CZetPipeServer pipeComm(L"\\\\.\\pipe\\testMyPipes", f, arg);

// - Вызвать функцию для запуска работы именованного канала:           Init()

// - При завершении работы с командами по именованному каналу вызвать: Close()

#ifndef zetpipecommand_server
#define zetpipecommand_server

#include <Windows.h>
#include <string>
#include <list>
#include <future>

typedef struct
{
    OVERLAPPED   oOverlap;        // Структура OVERLAPPED для работы с именованными каналами
    HANDLE       hPipeInst;       // Хендл именованного канала
    LPVOID       pInBuff;         // Буфер с принятыми по именованному каналу данными
    DWORD        dwInBuffSz;      // Размер принятых данных по именованному каналу
    LPVOID       pOutBuff;        // Буфер с ответными данными по именованному каналу
    DWORD        dwOutBuffSz;     // Размер отправляемых данных по именованному каналу
    DWORD        dwState;         // Текущее состояние именованного канала
    BOOL         bPendingIO;      // В режиме ожидания подключения
} PIPE_CLIENT_STRUCT, *LPIPE_CLIENT_STRUCT;

class CZetPipeServer
{
// Константы
private:
    static const DWORD PIPE_CLIENT_DEFAULT_IO_BUFFER_SIZE = 4096;   // Максимальный размер по умолчанию буфера для приемо передачи по именованному каналу
    static const DWORD PIPE_CLIENT_DEFAULT_FREE_COPY = 1;           // Максимальное количество свободных экземпляров именованных каналов

    static const DWORD PIPE_CLIENT_STATE_CONNECTING = 0;   // Состояние именованного канала в поиске клиента
    static const DWORD PIPE_CLIENT_STATE_READING = 1;      // Состояние именованного канала в чтении данных
    static const DWORD PIPE_CLIENT_STATE_WRITING = 2;      // Состояние именованного канала в записи данных

// Данные
private:
    HANDLE m_hIOCompletionPort = INVALID_HANDLE_VALUE;   // Дескриптор порта завершения (очередь событий Overlapped)
    DWORD  m_dwMaxPipeClientInBuffSz  = 0;               // Размер буфера под входящие данные
    DWORD  m_dwMaxPipeClientOutBuffSz = 0;               // Размер буфера под исходящие данные
    DWORD  m_dwMaxPipeClientFreeCopys = 0;               // Максимальное количество зарезервированных свободных екземпляров именованных каналов
    std::list<LPIPE_CLIENT_STRUCT> m_lPipeClients;       // Список экземпляров созданных именованных каналов

protected:
    LPTSTR m_lpszPipeName = L"";      // Имя именованного канала для передачи команд

    LPVOID       m_lpArg = nullptr;   // Сторонний аргумент передаваемый в функцию обработчик
    // Обработчик полученной команды
    void(*m_fPipeClientHandler)(LPVOID lpInCommData_, DWORD dwInCommDataSz_, LPVOID lpOutCommData_, DWORD* pdwOutCommDataSz_, LPVOID lpArg_);

    std::thread* m_pthPipeClientThread = nullptr;   // Поток отслеживания событий по работе с именованным каналом

// Функции
private:
    bool createPipeClient();                                  // Создает новый экземпляр именованного канала для ожидания подключения клиента
    void deletePipeClient(LPIPE_CLIENT_STRUCT pipeClient_);   // Удаляет экземпляр именованного канала

public:
    CZetPipeServer();

    CZetPipeServer(
        LPTSTR pipeName_,                                   // Имя именованного канала для работы с командами
        void(*pF_)(LPVOID lpInData_, DWORD lpInDataSz_,   // Функция обработчик полученных данных по именованному каналу
                    LPVOID lpOutData_, DWORD* lpOutDataSz_, LPVOID lpArg_),
        LPVOID lpArg_,                                        // Аргумент передаваемый в функцию обработчик
        DWORD  dwMaxPipeClientInBuffSz_      = PIPE_CLIENT_DEFAULT_IO_BUFFER_SIZE,   // Размер входного буфера
        DWORD  dwMaxPipeClientOutBuffSz_     = PIPE_CLIENT_DEFAULT_IO_BUFFER_SIZE,   // Размер выходного буфера
        DWORD  dwMaxPipeClientFreeExemplyar_ = PIPE_CLIENT_DEFAULT_FREE_COPY);       // Количество зарезервированных свободных экземпляров именованных каналов

    ~CZetPipeServer();

    void Init();    // Функция инициализации класса для работы с именованными каналами
    void Close();   // Функция деинициализации класса для работы с именованными каналами

protected:
    static UINT IOCompletionThread(LPVOID pParam);   // Потоковая функция обработки событий из очереди порта завершения
};

#endif
