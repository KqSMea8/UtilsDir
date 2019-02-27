#include <stdint.h>
#include <rpc.h>

#include <ZetErrorCodes.h>
#include <ZetTools\include\ZetTools.h>

#ifdef WRITEDATA_EXPORTS
#define WRITEDATA_API __declspec(dllexport)
#else
#define WRITEDATA_API __declspec(dllimport)
#endif

//----------------------------------------------------------------------------------------------------------------------
struct wdDataStructA
{
	char sName[128];			// �������� ������
	char sComment[128];			// ����������� �� ������ (���� �� ������������)
	char sGroupName[128];		// ��������� ��� (�������������� ����������)
	char sConversion[32];		// ������� ��������� �� ������
	GUID id;					// ������������� ������
	float frequency;			// ������� ������������� (� ��)
	float minlevel;				// ����������� ���������� �������� (� ��. ���.)
	float maxlevel;				// ������������ ���������� �������� (� ��. ���.)
	float threshold;			// ����� ���������������� (� ��. ���.)
	float sense;				// ���������������� (�/��. ���.)
	float reference;			// ������� �������� (� ��. ���.) 
	float shift;				// �������� ���������� ������������ (� ��. ���.)
	int typeAdc;				// ��� ����� (�������������)
	int numberDSP;				// ����� ����������� ���������� (�������������)
	int type;					// ������ ������ (���, ���, ����������� � �.�.)
	int number;					// ���������� ����� ������
	CZetTime tTime;				// ����� ������ ������
	float* pData;				// ��������� �� ������
	uint32_t size;				// ������ ������� ������
};
//----------------------------------------------------------------------------------------------------------------------
struct wdDataStructW
{
	wchar_t sName[128];			// �������� ������
	wchar_t sComment[128];		// ����������� �� ������ (���� �� ������������)
	wchar_t sGroupName[128];	// ��������� ��� (�������������� ����������)
	wchar_t sConversion[32];	// ������� ��������� �� ������
	GUID id;					// ������������� ������
	float frequency;			// ������� ������������� (� ��)
	float minlevel;				// ����������� ���������� �������� (� ��. ���.)
	float maxlevel;				// ������������ ���������� �������� (� ��. ���.)
	float threshold;			// ����� ���������������� (� ��. ���.)
	float sense;				// ���������������� (�/��. ���.)
	float reference;			// ������� �������� (� ��. ���.) 
	float shift;				// �������� ���������� ������������ (� ��. ���.)
	int typeAdc;				// ��� ����� (�������������)
	int numberDSP;				// ����� ����������� ���������� (�������������)
	int type;					// ������ ������ (���, ���, ����������� � �.�.)
	int number;					// ���������� ����� ������
	CZetTime tTime;				// ����� ������ ������
	float* pData;				// ��������� �� ������
	uint32_t size;				// ������ ������� ������
};
//----------------------------------------------------------------------------------------------------------------------
#ifdef UNICODE
#define wdDataStruct  wdDataStructW
#else
#define wdDataStruct  wdDataStructA
#endif
//----------------------------------------------------------------------------------------------------------------------
_Check_return_
long WRITEDATA_API wdWriteAnaA(_In_ wdDataStructA* pData,	//��������� �� ������ �������� ������ ��� ������
									_In_ const uint32_t size,	//����������� ������� ��������
									_In_ LPCSTR pszPath);		//���� (����������) ��� ����������

_Check_return_
long WRITEDATA_API wdWriteAnaW(_In_ wdDataStructW* pData,	//��������� �� ������ �������� ������ ��� ������
									_In_ const uint32_t size,	//����������� ������� ��������
									_In_ LPCWSTR pszPath);		//���� (����������) ��� ����������
#ifdef UNICODE
#define wdWriteAna wdWriteAnaW
#else
#define wdWriteAna wdWriteAnaA
#endif
//----------------------------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------------------------
_Check_return_
long WRITEDATA_API wdWriteSgyA(_In_ wdDataStructA* pData,	//��������� �� ������ �������� ������ ��� ������
									_In_ const uint32_t size,	//����������� ������� ��������
									_In_ LPCSTR pszPath);		//���� (��� �����) ��� ����������

_Check_return_
long WRITEDATA_API wdWriteSgyW(_In_ wdDataStructW* pData,	//��������� �� ������ �������� ������ ��� ������
									_In_ const uint32_t size,	//����������� ������� ��������
									_In_ LPCWSTR pszPath);		//���� (��� �����) ��� ����������

#ifdef UNICODE
#define wdWriteSgy wdWriteSgyW
#else
#define wdWriteSgy wdWriteSgyA
#endif
//----------------------------------------------------------------------------------------------------------------------
_Check_return_
long WRITEDATA_API wdChangeLengthSgyA(_In_ LPCSTR pszPath,	//����(��� �����)
										_In_ const float length);	//����� ������������ �����

_Check_return_
long WRITEDATA_API wdChangeLengthSgyW(_In_ LPCWSTR pszPath,	//����(��� �����)
										_In_ const float length);	//����� ������������ �����

#ifdef UNICODE
#define wdChangeLengthSgy wdChangeLengthSgyW
#else
#define wdChangeLengthSgy wdChangeLengthSgyA
#endif
//----------------------------------------------------------------------------------------------------------------------
_Check_return_
long WRITEDATA_API wdChangeFreqSgyA(_In_ LPCSTR pszPath,	//����(��� �����)
									_In_ const float freq);	//����� ������� �������������

_Check_return_
long WRITEDATA_API wdChangeFreqSgyW(_In_ LPCWSTR pszPath,	//����(��� �����)
									_In_ const float freq);	//����� ������� �������������

#ifdef UNICODE
#define wdChangeFreqSgy wdChangeFreqSgyW
#else
#define wdChangeFreqSgy wdChangeFreqSgyA
#endif
//----------------------------------------------------------------------------------------------------------------------
_Check_return_
long WRITEDATA_API wdMergeSgyA(_In_ LPCSTR* ppszPath,	//��������� �� ������ ���� ������ ��� �����������
								_In_ const uint32_t size,	//����������� ������� ������
								_In_ LPCSTR pszPath);	//����(��� �����) ��� ����������

_Check_return_
long WRITEDATA_API wdMergeSgyW(_In_ LPCWSTR* ppszPath,	//��������� �� ������ ���� ������ ��� �����������
								_In_ const uint32_t size,	//����������� ������� ������
								_In_ LPCWSTR pszPath);	//����(��� �����) ��� ����������

#ifdef UNICODE
#define wdMergeSgy wdMergeSgyW
#else
#define wdMergeSgy wdMergeSgyA
#endif
//----------------------------------------------------------------------------------------------------------------------
_Check_return_
long WRITEDATA_API wdAverageSgyA(_In_ LPCSTR* ppszPath,	//��������� �� ������ ���� ������ ��� �����������
								 _In_ const uint32_t size,	//����������� ������� ������
								 _In_ LPCSTR pszPath);	//����(��� �����) ��� ����������

_Check_return_
long WRITEDATA_API wdAverageSgyW(_In_ LPCWSTR* ppszPath,	//��������� �� ������ ���� ������ ��� �����������
								 _In_ const uint32_t size,	//����������� ������� ������
								 _In_ LPCWSTR pszPath);	//����(��� �����) ��� ����������

#ifdef UNICODE
#define wdAverageSgy wdAverageSgyW
#else
#define wdAverageSgy wdAverageSgyA
#endif
//----------------------------------------------------------------------------------------------------------------------
_Check_return_
long WRITEDATA_API wdWriteCsvA(_In_ wdDataStructA* pData,	//��������� �� ������ �������� ������ ��� ������
									_In_ const uint32_t size,	//����������� ������� ��������
									_In_ LPCSTR pszPath);		//���� (��� �����) ��� ����������

_Check_return_
long WRITEDATA_API wdWriteCsvW(_In_ wdDataStructW* pData,	//��������� �� ������ �������� ������ ��� ������
									_In_ const uint32_t size,	//����������� ������� ��������
									_In_ LPCWSTR pszPath);		//���� (��� �����) ��� ����������
#ifdef UNICODE
#define wdWriteCsv wdWriteCsvW
#else
#define wdWriteCsv wdWriteCsvA
#endif
//----------------------------------------------------------------------------------------------------------------------
_Check_return_
long WRITEDATA_API wdSgy2AnaA(_In_ LPCSTR pszPath);			//���� (��� �����) ��� ����������

_Check_return_
long WRITEDATA_API wdSgy2AnaW(_In_ LPCWSTR pszPath);		//���� (��� �����) ��� ����������

#ifdef UNICODE
#define wdSgy2Ana wdSgy2AnaW
#else
#define wdSgy2Ana wdSgy2AnaA
#endif
//----------------------------------------------------------------------------------------------------------------------