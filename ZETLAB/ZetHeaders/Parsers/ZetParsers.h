#ifdef ZETPARSERS_EXPORTS
#define ZETPARSERS_API __declspec(dllexport)
#else
#define ZETPARSERS_API __declspec(dllimport)
#endif

#include <vector>
#include <deque>
#include <string>
#include <zet_schema1.h>
#include <Zet7xxxDefinitions.h>

class CPage;

namespace zetparser7000
{
using namespace std;

typedef pair < CPage*, unsigned >	Page;
typedef deque<Page>::iterator		Iter;
typedef vector<wstring>				vstr;

template class ZETPARSERS_API deque < Page > ;
template class ZETPARSERS_API deque < long >;

class ZETPARSERS_API CZetParser7000
{
public:
	/** ����������� �� ��������� */
	CZetParser7000();
	/** ���������� */
	~CZetParser7000();
	/** ������� ���������� ���������� �������
	* @return ���������� �������
	*/
	int GetListPages();
	/** ���������� ������ � ����������������
	* @return ������
	*/
	deque<long> GetIdentificators();
	/** ���������� ���������� �����
	* @return ���������� �����
	* @param id ������������� ���������
	* @param num ���������� ����� (���� ���� ����������)
	*/
	long GetNumFields(long id, unsigned num = 0);
	/** ������� � ������� �������������� ������������� ��������
	* @return ��� ������
	* @param pDev ��������� �� ��������� ����������
	* @param sDev ������ ��������� ����������
	* @param pDef ��������� �� ����� ����������
	* @param sDef ������ ���� ����������
	*/
	int Parse(BYTE* pDev, size_t sDev, BYTE* pDef, size_t sDef);
	/** ���������� �������� ���������
	* @return �������� ���������
	* @param id ������������� ���������
	* @param num ���������� ����� (���� ���� ����������)
	*/
	wstring GetName(long id, unsigned num = 0);
	/** ���������� ���������� ����������� ���������
	* @return ���������� ����������� ���������
	* @param id ������������� ���������
	* @param num ���������� ����� (���� ���� ����������)
	*/
	wstring GetDescription(long id, unsigned num = 0);
	/** ���������� �������� ����
	* @return ����������� ����� ����
	* @param id ������������� ���������
	* @param fld ����� ����
	* @param num ���������� ����� (���� ���� ����������)
	*/
	wstring GetFieldName(long id, long fld, unsigned num = 0);
	/** ���������� ��������� ������������� �������� �����
	* @return ��������
	* @param id ������������� ���������
	* @param fld ����� ����
	* @param val ������ ��� ������ ����� (��������, ���� ���������)
	* @param num ���������� ����� (���� ���� ����������)
	*/
	wstring GetValue(long id, long fld, vstr& val, unsigned num = 0);
	/** ���������� ������ ����
	* @return ������ ����
	* @param id ������������� ���������
	* @param fld ����� ����
	* @param num ���������� ����� (���� ���� ����������)
	*/
	size_t GetFieldSize(long id, long fld, unsigned num = 0);
	/** ��������� �������� � ���������
	* @return ���������� ��������� �������/��������
	* @param ������������� ���������
	* @param ���������� ����� ���������
	* @param �������� �������
	*/
	bool SetValue(long id, long fld, wstring& val, unsigned num = 0);
	/** ���������� ��� ���������
	* @return ��� ���������
	* @param id ������������� ���������
	* @param fld ���������� ����� ����
	* @param num ���������� ����� (���� ���� ����������)
	*/
	long GetTypeOfArg(long id, long fld, unsigned num = 0);
	/** ����� ��������� ������� */
	void ResetParser();
	/** ������� ���������� ���� ���������
	* @return ����
	* @param id ������������� ���������
	* @param fld ���������� ����� ����
	* @param num ���������� ����� (���� ���� ����������)
	*/
	bool IsEditable(long id, long fld, unsigned num = 0);
protected:
	/** ������ �� "����������" */
	deque<Page> m_Page;
	/** ������ � ������� ��������������� �������� */
	deque<long> m_ID;
	/** ������� ������ ��������� �� id
	* @return �������� (���� ��������� �� ������� �� �������� ����� end())
	* @param id ������������� ���������
	* @param num ���������� ����� (���� ���� ����������)
	*/
	Iter GetIterator(long id, unsigned num = 0);
	/** ������� ����������� ����� ������� � ������
	* @param pDev ��������� �� ���������
	* @param pDef ��������� �� ���������
	* @pram type ��� ���������
	* @param access 
	*/
	void AddPage(STRUCT_HEAD* pDev, STRUCT_HEAD* pDef, STRUCT_TYPE type, long access);
};
}

namespace zetparser018
{
using namespace std;
using namespace zet;
using namespace schema1;

enum ZetParserMode
{
	PROGRAMM = 0,
	DEVICE
};

class ZETPARSERS_API CZetParser018
{
public:
	/** ����������� �� ��������� */
	CZetParser018();
	/** ���������� */
	~CZetParser018();
	/** ������� �������������� ������������
	* @return ��� ������
	* @param pbuf ����� �� ������
	* @param sz ������ ������
	*/
	int Parser(BYTE* pSchm, size_t sz1, BYTE* pPrm, size_t sz2);
	/** ������� ���������� ���������� "�������"
	* @return ���������� "�������"
	*/
	int GetListPages();
	/** ���������� ���������� ����� �� "��������"
	* @return ���������� �����
	*/
	int GetListFields();
	/** ���������� ��� �������� / -1 ���� ����� �� ������� ������
	* @return ��� ��������
	* @param num ���������� ����� ����
	*/
	long GetFiledControl(unsigned num);
	/** ���������� ������ ���� ��� ���
	* @return 1 ���� HIDDEN
	* @return 0 ���� �� HIDDEN ��� �����������
	*/
	bool IsFieldHidden(unsigned num);
	/** ���������� �������� ���� ����������� ��� ���
	* @return true ���� INACCURACY
	* @return false ���� �� INACCURACY ��� �����������
	*/
	bool IsFieldInaccuracy(unsigned num);
	/** ���������� �������� ���� METADATA ��� ���
	* @return 1 ���� METADATA
	* @return 0 ���� �� METADATA ��� �����������
	*/
	bool IsFieldMetadata(unsigned num);
	/** ���������� �������� "��������"
	* @return ��������
	*/
	wstring GetPageName();
	/** ���������� ��� ����
	* @return BaseType, ������������ ���� CHAR S8, U8, S16, U16, S32, U32, S64, U64, F32, F64, METADATA, UNDEFINED
	* @param num ���������� ����� ����
	*/
	BaseType GetFieldType(unsigned num);
	/** ���������� �������� ����
	* @return �������� ����
	* @param num ���������� ����� ����
	*/
	wstring GetFieldName(unsigned num);
	/** ���������� ����������� ����
	* @return �����������
	* @param num ���������� ����� ����
	*/
	wstring GetFiledDescription(unsigned num);
	/** ���������� ��������� �������� �������
	* @return ��������
	* @param num ���������� ����� ����
	*/
	wstring GetFieldDefault(unsigned num);
	/** ���������� Offset  
	* @return uint32_t
	* @param num ���������� ����� ����
	*/
	uint32_t GetFieldOffset(int num);
	/** ����������
	* @return
	* @param num ���������� ����� ����
	*/
	wstring GetFieldGroup(unsigned num);
	/** ���������� �������� ���������
	* @return ��������
	*/
	wstring GetStructName();
	/** ���������� ����������� ���������
	* @return ��������
	*/
	wstring GetStructDescription();
	/** ���������� title ���������
	* @return ��������
	*/
	wstring GetStructTitle();
	/** ����� ��������� ������� */
	void ResetParser();
	/** ��������� ������ ������ ���������
	* @param Mode �����
	*/
	void SetMode(ZetParserMode Mode);
	/** ������� ���������� ������� Header ��������
	* @return ZetHeader*
	*/
	ZetHeader* GetHeader();
	/** ������� ���������� ������� ����� ������
	* @return ������� ����� ������
	*/
	ZetParserMode GetMode();
	/** ���������� ������� �������� � ����
	* @return ���������� �������� �������
	* @param ���������� ����� ����
	*/
	wstring GetCurrentVal(unsigned num);
	/** ���������� �������� CountPow2 ��� ����
	* @return CountPow2
	* @param num ���������� ����� ����
	*/
	int GetFieldCountPow2(unsigned num);
	/** ������ ������ �������� � ����
	* @param num ���������� �����
	* @param val �������� ������� 
	*/
	void WriteValue(unsigned num, wstring& val);
	/** ���������� ���������� ����� � �����
	* @return ���������� �����
	* @param num ���������� ����� ����
	*/
	long GetStringListCount(unsigned num);
	/** ���������� ���������� ����� � ��������� �����
	* @return ���������� ��������� �����
	* @param num ���������� ����� ����
	*/
	long GetListCount(unsigned num);
	/** ���������� ������ � ������
	* @return ������ � ������
	* @param num ���������� �����
	*/
	wstring GetList(unsigned num);
	/** ���������� ������ �� ���������� ������
	* @return ������ �� ���������� ������
	* @param num ���������� �����
	*/
	wstring GetStringList(unsigned num);
	/** ���������� ���������� ��������� � ������� ������� ����
	* @return ����� ��������� �������
	* @param num ���������� ����� ����
	* @param bCheckArray ��������� ������� ���������� �����
	*/
	int GetFieldArraySize(unsigned num, bool bCheckArray = false, int HeaderSize = 0);
	/** ���������� ������ �� ��������� �������� ������� � �������� ����
	* @return ������ �� �������� ����
	* @param num ���������� ����� ����, ���������� ����� �������� �������
	* @param bCheckArray ��������� ������� ���������� �����
	*/
	wstring GetFieldArrayVal(unsigned fieldnum, int arraynum, bool bCheckArray = false);
	/** ���� ���� �������� ������ �� ���������� offset �������� ����� �������
	* @return offset ��������
	* @param num ���������� ����� ����, ���������� ����� �������� �������
	*/
	uint32_t GetArrayElementOffset(unsigned fieldnum, int arraynum);
	/** ���������� �������� ��������� �� �����������
	* @return ��������
	* @param _pSchm - ���� �����, _size - �� ������
	*/
	wstring GetStructNameFromDescriptor(_In_ const BYTE* _pSchm, _In_ const size_t _size);
	/** ���������� ���������� ��������� �� �����������
	* @return ��������
	* @param _pSchm - ���� �����, _size - �� ������
	*/
	wstring GetStructDescriptionFromDescriptor(_In_ const BYTE* _pSchm, _In_ const size_t _size);
protected:
	/** �������������� ������ � ��������
	* @return ��������
	* @param val ������ �� ���������
	*/
	template <typename T>
	T SetNewValue(const wstring& val);
	/** ������� ��������� ������� ��������
	* @param f ��������� ������ Field
	*/
	void SetVal(Field f);
	/** ��������� ��������� ��������
	* @param f ��������� ������ Field
	*/
	void SetDefVal(Field f);
	/** ��������� ������� �������� */
	void SetValues();
	/** �������������� ������ �� char � wchar_t 
	* @return ��������������� ������
	* @param str ������� ������
	*/
	wstring GetWstring(const std::string& str);
	/** �������������� ������ �� wchar_t � char
	* @return ��������������� ������
	* @param str ������� ������
	*/
	std::string GetString(const wstring& str);
	/** ������ � ������ */
	vector<Field> fields;
	/** ������ ��� �������� ������ */
	binary m_data;
	/** ������ ��� �������� ���������� */
	binary m_prop;
	/** ������ ��� �������� ������ ���������� */
	binary m_propOld;
	/** ��������� ������ */
	Structure m_struct;
	/** ������ ��� �������� ����� */
	Reader m_Reader;
	/** ���� ����������� �� ����� ������ ������� */
	ZetParserMode m_Mode;
	/** ��������� �� ��������� ������ */
	ZetHeader* m_pHeader;
	/** ��������� �� ��������� ������ */
	ZetFooter* m_pFooter;
	/** �������� � ����� ���������� */
	size_t m_PropShift;
	/** �������� ��������� */
	wstring m_ProgName;
	/** �������� ������ ��� ������ ������� ������ */
	wstring SetCurrentValue(BYTE* addr, BaseType type);
	/** ����� ������ ������� */
	size_t m_ModeShift;
	/** ������� �������� ����� */
	deque<wstring> m_CurValue;
};
}
