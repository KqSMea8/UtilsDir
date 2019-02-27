#include "NamedPipes\Server\zetPipeServer.h"

CZetPipeServer::CZetPipeServer()
{

}

CZetPipeServer::CZetPipeServer(
    LPTSTR lpszPipeName_, 
    void(*pF_)(LPVOID lpInData_, DWORD lpInDataSz_, LPVOID lpOutData_, DWORD* lpOutDataSz_, LPVOID lpArg_), 
    LPVOID lpArg_,
    DWORD dwMaxPipeClientInBuffSz_,
    DWORD dwMaxPipeClientOutBuffSz_,
    DWORD dwMaxPipeClientFreeExemplyar_)
{
    // Инициализируем внутренние переменные для хранения настроек именованного канала
    m_lpszPipeName = lpszPipeName_;
    m_lpArg = lpArg_;
    m_fPipeClientHandler = pF_;

    m_dwMaxPipeClientInBuffSz  = dwMaxPipeClientInBuffSz_;
    m_dwMaxPipeClientOutBuffSz = dwMaxPipeClientOutBuffSz_;
    m_dwMaxPipeClientFreeCopys = dwMaxPipeClientFreeExemplyar_;
}

CZetPipeServer::~CZetPipeServer()
{
    Close();
}

bool CZetPipeServer::createPipeClient()
{
    // Если ошибка произошла при создании порта завершения
    if (INVALID_HANDLE_VALUE == m_hIOCompletionPort)
        return false;

    LPIPE_CLIENT_STRUCT pPipeClient = new PIPE_CLIENT_STRUCT;
    pPipeClient->pInBuff = new BYTE[m_dwMaxPipeClientInBuffSz];    // Буфер приема данных
    pPipeClient->pOutBuff = new BYTE[m_dwMaxPipeClientOutBuffSz];   // Буфер отправки данных
    pPipeClient->hPipeInst = INVALID_HANDLE_VALUE;

    // Инициализируем структуру OVERLAPPED
    memset(&pPipeClient->oOverlap, 0x00, sizeof(pPipeClient->oOverlap));
    pPipeClient->oOverlap.hEvent = CreateEvent(NULL, TRUE, TRUE, NULL);

    // Если не получилось проинициализировать Overlapped удаляемся
    if (INVALID_HANDLE_VALUE == pPipeClient->oOverlap.hEvent)
    {   
        deletePipeClient(pPipeClient);
        return false;
    }

    // Так как Windows сервис запущен от имени системы, то необходимо задать более детально права доступа
    SECURITY_DESCRIPTOR sd = {0};
    if (InitializeSecurityDescriptor(&sd, SECURITY_DESCRIPTOR_REVISION) != FALSE)
    {
        if (SetSecurityDescriptorDacl(&sd, true, 0, false) != FALSE)
        {
            SECURITY_ATTRIBUTES sa; sa.nLength = sizeof(sa); sa.lpSecurityDescriptor = &sd; sa.bInheritHandle = false;

            pPipeClient->hPipeInst = CreateNamedPipe(
                m_lpszPipeName,
                PIPE_ACCESS_DUPLEX | FILE_FLAG_OVERLAPPED,
                PIPE_TYPE_MESSAGE | PIPE_READMODE_MESSAGE | PIPE_WAIT,
                PIPE_UNLIMITED_INSTANCES,
                m_dwMaxPipeClientOutBuffSz,
                m_dwMaxPipeClientInBuffSz,
                0,
                &sa);
        }
        else
            pPipeClient->hPipeInst = INVALID_HANDLE_VALUE;
    }

    // Если не получилось создать именованный канал удаляемся
    if (INVALID_HANDLE_VALUE == pPipeClient->hPipeInst)
    {
        deletePipeClient(pPipeClient);
        return false;
    }

    // Подключение к порту завершения, если не получилось удаляемся
    if (m_hIOCompletionPort != CreateIoCompletionPort(pPipeClient->hPipeInst, m_hIOCompletionPort, (ULONG_PTR)pPipeClient, 1))
    {
        deletePipeClient(pPipeClient);
        return false;
    }

    // Подключение к именованному каналу
    BOOL bConnected = ConnectNamedPipe(pPipeClient->hPipeInst, &pPipeClient->oOverlap);

    // Проверяем статус подключения
    BOOL fSuccess = FALSE;
    switch (GetLastError())
    {
        case ERROR_IO_PENDING:
            pPipeClient->dwState = PIPE_CLIENT_STATE_CONNECTING;
            pPipeClient->bPendingIO = TRUE;
            break;
        case ERROR_PIPE_CONNECTED:
            pPipeClient->dwState = PIPE_CLIENT_STATE_CONNECTING;
            pPipeClient->bPendingIO = TRUE;

            if (!PostQueuedCompletionStatus(m_hIOCompletionPort, 0, (ULONG_PTR)pPipeClient, &pPipeClient->oOverlap))   // Событие не возникнет
            {
                deletePipeClient(pPipeClient);
                return false;
            }
        default:
            break;
    }

    // Добавляем в список клиентов
    m_lPipeClients.push_back(pPipeClient);

    return true;
}

void CZetPipeServer::deletePipeClient(LPIPE_CLIENT_STRUCT pPipeClient_)
{
    // Если указатель неправильный
    if (nullptr == pPipeClient_)
        return;

    // Если именованный канал открыт
    if (INVALID_HANDLE_VALUE != pPipeClient_->hPipeInst)
    {
        FlushFileBuffers(pPipeClient_->hPipeInst);
        DisconnectNamedPipe(pPipeClient_->hPipeInst);

        CloseHandle(pPipeClient_->hPipeInst);
    }

    // Если событие Overlapped создано
    if (INVALID_HANDLE_VALUE != pPipeClient_->oOverlap.hEvent)
        CloseHandle(pPipeClient_->oOverlap.hEvent);

    // Если входящий буфер проинициализирован
    if (nullptr != pPipeClient_->pInBuff)
	{ 
        delete[] pPipeClient_->pInBuff;
		pPipeClient_->pInBuff = nullptr;
	}

    // Если исходящий буфер проинициализирован
    if (nullptr != pPipeClient_->pOutBuff)
	{ 
        delete[] pPipeClient_->pOutBuff;
		pPipeClient_->pOutBuff = nullptr;
	}

    // Если клиент добавлен в список всех клиентов
    std::list<LPIPE_CLIENT_STRUCT>::iterator deletingItem = std::find(m_lPipeClients.begin(), m_lPipeClients.end(), pPipeClient_);
    if (deletingItem != m_lPipeClients.end())
        m_lPipeClients.erase(deletingItem);

    delete pPipeClient_;
    pPipeClient_ = nullptr;
}

void CZetPipeServer::Init()
{
    m_hIOCompletionPort = CreateIoCompletionPort(INVALID_HANDLE_VALUE, NULL, 0, 1);

    // Создаем поток для обработки IOCP
    m_pthPipeClientThread = new std::thread(IOCompletionThread, this);

    // Создаем клиентов, в последствии к нему подключатся и создастся новый клиент
    for (unsigned int i = 0; i < m_dwMaxPipeClientFreeCopys; i++)
        createPipeClient();
}

void CZetPipeServer::Close()
{
    // Останавливаем поток
    CloseHandle(m_hIOCompletionPort);
    if (m_pthPipeClientThread != nullptr)
    {
        m_pthPipeClientThread->join();
        delete m_pthPipeClientThread;
        m_pthPipeClientThread = nullptr;
    }

    // Удаляем всех клиентов
    while (m_lPipeClients.size() != 0)
        deletePipeClient(*m_lPipeClients.begin());
}

UINT CZetPipeServer::IOCompletionThread(LPVOID lpZetPipeCommServer_)
{
    CZetPipeServer* zetPipeCommServer = (CZetPipeServer*)lpZetPipeCommServer_;

    DWORD       dwTransferredBytes = 0;   // Для получения информации о текущем обрабатываемом Overlapped
    ULONG_PTR   pulpCompletionKey = 0;    // Для получения информации о текущем обрабатываемом Overlapped
    OVERLAPPED* poOverlapped = nullptr;   // Для получения информации о текущем обрабатываемом Overlapped
    DWORD       dwChanged = 0;   // Объем обновленной информации
    DWORD       dwErr = 0;       // Последняя ошибка

    BOOL fSuccess = FALSE;

    while (true)
    {
        // Определяем вызвавший именованный канал
        fSuccess = GetQueuedCompletionStatus(zetPipeCommServer->m_hIOCompletionPort, &dwTransferredBytes, &pulpCompletionKey, &poOverlapped, INFINITE);
        LPIPE_CLIENT_STRUCT currPipeClient = (LPIPE_CLIENT_STRUCT)pulpCompletionKey;

        if (!fSuccess)
        {
            if (GetLastError() == ERROR_BROKEN_PIPE)
            {
                // Если произошло отключение от именованного канала
                zetPipeCommServer->deletePipeClient(currPipeClient);
                continue;
            }
            else
                break;   // Остановка потока
        }

        // Оканчиваем выполнение операции (возникло событие о окончании выполнения операции)
        if (currPipeClient->bPendingIO != FALSE)
        {
            if (PIPE_CLIENT_STATE_CONNECTING != currPipeClient->dwState)
                fSuccess = GetOverlappedResult(currPipeClient->hPipeInst, &currPipeClient->oOverlap, &dwChanged, FALSE);

            switch (currPipeClient->dwState)
            {
                case PIPE_CLIENT_STATE_CONNECTING:
                    // Ожидаем завершения процедуры подключения
                    zetPipeCommServer->createPipeClient();

                    if (!fSuccess)
                    {
                        zetPipeCommServer->deletePipeClient(currPipeClient);
                        continue;
                    }

                    currPipeClient->dwState = PIPE_CLIENT_STATE_READING;
                    break;

                case PIPE_CLIENT_STATE_READING:
                    if (!fSuccess || dwChanged == 0)
                    {
                        zetPipeCommServer->deletePipeClient(currPipeClient);
                        continue;
                    }

                    currPipeClient->dwInBuffSz = dwChanged;
                    currPipeClient->dwState = PIPE_CLIENT_STATE_WRITING;
                    break;

                case PIPE_CLIENT_STATE_WRITING:
                    if (!fSuccess || dwChanged != currPipeClient->dwOutBuffSz)
                    {
                        zetPipeCommServer->deletePipeClient(currPipeClient);
                        continue;
                    }

                    currPipeClient->dwState = PIPE_CLIENT_STATE_READING;
                    break;

                default:
                    return 0;
            }
        }

        // Выполняем операцию

        switch (currPipeClient->dwState)
        {
            case PIPE_CLIENT_STATE_READING:
                fSuccess = ReadFile(currPipeClient->hPipeInst, currPipeClient->pInBuff, zetPipeCommServer->m_dwMaxPipeClientInBuffSz, &currPipeClient->dwInBuffSz, &currPipeClient->oOverlap);

                // Операция чтения прошла успешно
                if (fSuccess && currPipeClient->dwInBuffSz != 0)
                {
                    currPipeClient->bPendingIO = FALSE;
                    currPipeClient->dwState = PIPE_CLIENT_STATE_WRITING;
                    continue;
                }

                // Операция чтения в ожидании
                dwErr = GetLastError();
                if (!fSuccess && (dwErr == ERROR_IO_PENDING))
                {
                    currPipeClient->bPendingIO = TRUE;
                    continue;
                }

                zetPipeCommServer->deletePipeClient(currPipeClient);
                break;

            case PIPE_CLIENT_STATE_WRITING:
                // Обрабатываем данные через нашу CallBack функцию
                zetPipeCommServer->m_fPipeClientHandler(currPipeClient->pInBuff, currPipeClient->dwInBuffSz, currPipeClient->pOutBuff, &currPipeClient->dwOutBuffSz, zetPipeCommServer->m_lpArg);

                fSuccess = WriteFile(currPipeClient->hPipeInst, currPipeClient->pOutBuff, currPipeClient->dwOutBuffSz, &dwChanged, &currPipeClient->oOverlap);

                if (fSuccess && dwChanged == currPipeClient->dwOutBuffSz)
                {
                    currPipeClient->bPendingIO = FALSE;
                    currPipeClient->dwState = PIPE_CLIENT_STATE_READING;
                    continue;
                }

                dwErr = GetLastError();
                if (!fSuccess && (dwErr == ERROR_IO_PENDING))
                {
                    currPipeClient->bPendingIO = TRUE;
                    continue;
                }

                zetPipeCommServer->deletePipeClient(currPipeClient);
                break;

            default:
                return 0;
        }
    }

    return 0;
}