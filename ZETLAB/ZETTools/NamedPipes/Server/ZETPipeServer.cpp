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
    // �������������� ���������� ���������� ��� �������� �������� ������������ ������
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
    // ���� ������ ��������� ��� �������� ����� ����������
    if (INVALID_HANDLE_VALUE == m_hIOCompletionPort)
        return false;

    LPIPE_CLIENT_STRUCT pPipeClient = new PIPE_CLIENT_STRUCT;
    pPipeClient->pInBuff = new BYTE[m_dwMaxPipeClientInBuffSz];    // ����� ������ ������
    pPipeClient->pOutBuff = new BYTE[m_dwMaxPipeClientOutBuffSz];   // ����� �������� ������
    pPipeClient->hPipeInst = INVALID_HANDLE_VALUE;

    // �������������� ��������� OVERLAPPED
    memset(&pPipeClient->oOverlap, 0x00, sizeof(pPipeClient->oOverlap));
    pPipeClient->oOverlap.hEvent = CreateEvent(NULL, TRUE, TRUE, NULL);

    // ���� �� ���������� ������������������� Overlapped ���������
    if (INVALID_HANDLE_VALUE == pPipeClient->oOverlap.hEvent)
    {   
        deletePipeClient(pPipeClient);
        return false;
    }

    // ��� ��� Windows ������ ������� �� ����� �������, �� ���������� ������ ����� �������� ����� �������
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

    // ���� �� ���������� ������� ����������� ����� ���������
    if (INVALID_HANDLE_VALUE == pPipeClient->hPipeInst)
    {
        deletePipeClient(pPipeClient);
        return false;
    }

    // ����������� � ����� ����������, ���� �� ���������� ���������
    if (m_hIOCompletionPort != CreateIoCompletionPort(pPipeClient->hPipeInst, m_hIOCompletionPort, (ULONG_PTR)pPipeClient, 1))
    {
        deletePipeClient(pPipeClient);
        return false;
    }

    // ����������� � ������������ ������
    BOOL bConnected = ConnectNamedPipe(pPipeClient->hPipeInst, &pPipeClient->oOverlap);

    // ��������� ������ �����������
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

            if (!PostQueuedCompletionStatus(m_hIOCompletionPort, 0, (ULONG_PTR)pPipeClient, &pPipeClient->oOverlap))   // ������� �� ���������
            {
                deletePipeClient(pPipeClient);
                return false;
            }
        default:
            break;
    }

    // ��������� � ������ ��������
    m_lPipeClients.push_back(pPipeClient);

    return true;
}

void CZetPipeServer::deletePipeClient(LPIPE_CLIENT_STRUCT pPipeClient_)
{
    // ���� ��������� ������������
    if (nullptr == pPipeClient_)
        return;

    // ���� ����������� ����� ������
    if (INVALID_HANDLE_VALUE != pPipeClient_->hPipeInst)
    {
        FlushFileBuffers(pPipeClient_->hPipeInst);
        DisconnectNamedPipe(pPipeClient_->hPipeInst);

        CloseHandle(pPipeClient_->hPipeInst);
    }

    // ���� ������� Overlapped �������
    if (INVALID_HANDLE_VALUE != pPipeClient_->oOverlap.hEvent)
        CloseHandle(pPipeClient_->oOverlap.hEvent);

    // ���� �������� ����� ������������������
    if (nullptr != pPipeClient_->pInBuff)
	{ 
        delete[] pPipeClient_->pInBuff;
		pPipeClient_->pInBuff = nullptr;
	}

    // ���� ��������� ����� ������������������
    if (nullptr != pPipeClient_->pOutBuff)
	{ 
        delete[] pPipeClient_->pOutBuff;
		pPipeClient_->pOutBuff = nullptr;
	}

    // ���� ������ �������� � ������ ���� ��������
    std::list<LPIPE_CLIENT_STRUCT>::iterator deletingItem = std::find(m_lPipeClients.begin(), m_lPipeClients.end(), pPipeClient_);
    if (deletingItem != m_lPipeClients.end())
        m_lPipeClients.erase(deletingItem);

    delete pPipeClient_;
    pPipeClient_ = nullptr;
}

void CZetPipeServer::Init()
{
    m_hIOCompletionPort = CreateIoCompletionPort(INVALID_HANDLE_VALUE, NULL, 0, 1);

    // ������� ����� ��� ��������� IOCP
    m_pthPipeClientThread = new std::thread(IOCompletionThread, this);

    // ������� ��������, � ����������� � ���� ����������� � ��������� ����� ������
    for (unsigned int i = 0; i < m_dwMaxPipeClientFreeCopys; i++)
        createPipeClient();
}

void CZetPipeServer::Close()
{
    // ������������� �����
    CloseHandle(m_hIOCompletionPort);
    if (m_pthPipeClientThread != nullptr)
    {
        m_pthPipeClientThread->join();
        delete m_pthPipeClientThread;
        m_pthPipeClientThread = nullptr;
    }

    // ������� ���� ��������
    while (m_lPipeClients.size() != 0)
        deletePipeClient(*m_lPipeClients.begin());
}

UINT CZetPipeServer::IOCompletionThread(LPVOID lpZetPipeCommServer_)
{
    CZetPipeServer* zetPipeCommServer = (CZetPipeServer*)lpZetPipeCommServer_;

    DWORD       dwTransferredBytes = 0;   // ��� ��������� ���������� � ������� �������������� Overlapped
    ULONG_PTR   pulpCompletionKey = 0;    // ��� ��������� ���������� � ������� �������������� Overlapped
    OVERLAPPED* poOverlapped = nullptr;   // ��� ��������� ���������� � ������� �������������� Overlapped
    DWORD       dwChanged = 0;   // ����� ����������� ����������
    DWORD       dwErr = 0;       // ��������� ������

    BOOL fSuccess = FALSE;

    while (true)
    {
        // ���������� ��������� ����������� �����
        fSuccess = GetQueuedCompletionStatus(zetPipeCommServer->m_hIOCompletionPort, &dwTransferredBytes, &pulpCompletionKey, &poOverlapped, INFINITE);
        LPIPE_CLIENT_STRUCT currPipeClient = (LPIPE_CLIENT_STRUCT)pulpCompletionKey;

        if (!fSuccess)
        {
            if (GetLastError() == ERROR_BROKEN_PIPE)
            {
                // ���� ��������� ���������� �� ������������ ������
                zetPipeCommServer->deletePipeClient(currPipeClient);
                continue;
            }
            else
                break;   // ��������� ������
        }

        // ���������� ���������� �������� (�������� ������� � ��������� ���������� ��������)
        if (currPipeClient->bPendingIO != FALSE)
        {
            if (PIPE_CLIENT_STATE_CONNECTING != currPipeClient->dwState)
                fSuccess = GetOverlappedResult(currPipeClient->hPipeInst, &currPipeClient->oOverlap, &dwChanged, FALSE);

            switch (currPipeClient->dwState)
            {
                case PIPE_CLIENT_STATE_CONNECTING:
                    // ������� ���������� ��������� �����������
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

        // ��������� ��������

        switch (currPipeClient->dwState)
        {
            case PIPE_CLIENT_STATE_READING:
                fSuccess = ReadFile(currPipeClient->hPipeInst, currPipeClient->pInBuff, zetPipeCommServer->m_dwMaxPipeClientInBuffSz, &currPipeClient->dwInBuffSz, &currPipeClient->oOverlap);

                // �������� ������ ������ �������
                if (fSuccess && currPipeClient->dwInBuffSz != 0)
                {
                    currPipeClient->bPendingIO = FALSE;
                    currPipeClient->dwState = PIPE_CLIENT_STATE_WRITING;
                    continue;
                }

                // �������� ������ � ��������
                dwErr = GetLastError();
                if (!fSuccess && (dwErr == ERROR_IO_PENDING))
                {
                    currPipeClient->bPendingIO = TRUE;
                    continue;
                }

                zetPipeCommServer->deletePipeClient(currPipeClient);
                break;

            case PIPE_CLIENT_STATE_WRITING:
                // ������������ ������ ����� ���� CallBack �������
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