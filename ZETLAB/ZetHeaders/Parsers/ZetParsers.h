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
	/** конструктор по умолчанию */
	CZetParser7000();
	/** деструктор */
	~CZetParser7000();
	/** функция возвращает количество страниц
	* @return количество страниц
	*/
	int GetListPages();
	/** возвражает вестор с идентификаторами
	* @return вектор
	*/
	deque<long> GetIdentificators();
	/** возвращает количество полей
	* @return количество полей
	* @param id идентификатор структуры
	* @param num порядковый номер (если есть повторения)
	*/
	long GetNumFields(long id, unsigned num = 0);
	/** функция в которой осуществляется распарсивание структур
	* @return код ошибки
	* @param pDev указатель на структуру параметров
	* @param sDev размер струткуры параметров
	* @param pDef указатель на схему параметров
	* @param sDef размер схме параметров
	*/
	int Parse(BYTE* pDev, size_t sDev, BYTE* pDef, size_t sDef);
	/** Возвращает название параметра
	* @return название параметра
	* @param id идентификатор структуры
	* @param num порядковый номер (если есть повторения)
	*/
	wstring GetName(long id, unsigned num = 0);
	/** Возвращает возвращает обозначение параметра
	* @return возвращает обозначение параметра
	* @param id идентификатор структуры
	* @param num порядковый номер (если есть повторения)
	*/
	wstring GetDescription(long id, unsigned num = 0);
	/** Возвращает название поля
	* @return возвращение именя поля
	* @param id идентификатор структуры
	* @param fld номер поля
	* @param num порядковый номер (если есть повторения)
	*/
	wstring GetFieldName(long id, long fld, unsigned num = 0);
	/** Возвращает текстовое представление значений полей
	* @return значение
	* @param id идентификатор структуры
	* @param fld номер поля
	* @param val вектор для списка полей (например, если комбобокс)
	* @param num порядковый номер (если есть повторения)
	*/
	wstring GetValue(long id, long fld, vstr& val, unsigned num = 0);
	/** Возвращает размер поля
	* @return размер поля
	* @param id идентификатор структуры
	* @param fld номер поля
	* @param num порядковый номер (если есть повторения)
	*/
	size_t GetFieldSize(long id, long fld, unsigned num = 0);
	/** установка значения в структуру
	* @return возвращает результат успешно/неупешно
	* @param идентификатор структуры
	* @param порядковый номер структуры
	* @param значение строкой
	*/
	bool SetValue(long id, long fld, wstring& val, unsigned num = 0);
	/** Возвращает тип параметра
	* @return тип параметра
	* @param id идентификатор структуры
	* @param fld порядковый номер поля
	* @param num порядковый номер (если есть повторения)
	*/
	long GetTypeOfArg(long id, long fld, unsigned num = 0);
	/** Сброс состояния парсера */
	void ResetParser();
	/** Функция возвращает флаг изменений
	* @return флаг
	* @param id идентификатор структуры
	* @param fld порядковый номер поля
	* @param num порядковый номер (если есть повторения)
	*/
	bool IsEditable(long id, long fld, unsigned num = 0);
protected:
	/** Вектор со "страницами" */
	deque<Page> m_Page;
	/** Вектор с набором идентификаторов структур */
	deque<long> m_ID;
	/** Функция поиска структуры по id
	* @return итератор (если структура не найдена то итератор равен end())
	* @param id идентификатор структуры
	* @param num порядковый номер (если есть повторения)
	*/
	Iter GetIterator(long id, unsigned num = 0);
	/** Функция добавляющая новую странцу в вектор
	* @param pDev указатель на структуру
	* @param pDef указатель на структуру
	* @pram type тип структуры
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
	/** конструктор по умолчанию */
	CZetParser018();
	/** деструктор */
	~CZetParser018();
	/** функция осуществляющая распаривание
	* @return код ошибки
	* @param pbuf буфер со схемой
	* @param sz размер буфера
	*/
	int Parser(BYTE* pSchm, size_t sz1, BYTE* pPrm, size_t sz2);
	/** функция возвращает количество "страниц"
	* @return количество "страниц"
	*/
	int GetListPages();
	/** возвращает количество полей на "странице"
	* @return количество полей
	*/
	int GetListFields();
	/** возвращает тип контрола / -1 если выход за пределы списка
	* @return тип контрола
	* @param num порядковый номер поля
	*/
	long GetFiledControl(unsigned num);
	/** определяет скрыто поле или нет
	* @return 1 если HIDDEN
	* @return 0 если не HIDDEN или отсутствует
	*/
	bool IsFieldHidden(unsigned num);
	/** определяет содержит поле погрешность или нет
	* @return true если INACCURACY
	* @return false если не INACCURACY или отсутствует
	*/
	bool IsFieldInaccuracy(unsigned num);
	/** определяет является поле METADATA или нет
	* @return 1 если METADATA
	* @return 0 если не METADATA или отсутствует
	*/
	bool IsFieldMetadata(unsigned num);
	/** возвращает название "страницы"
	* @return название
	*/
	wstring GetPageName();
	/** возвращает тип поля
	* @return BaseType, возвращаемые типы CHAR S8, U8, S16, U16, S32, U32, S64, U64, F32, F64, METADATA, UNDEFINED
	* @param num порядковый номер поля
	*/
	BaseType GetFieldType(unsigned num);
	/** возвращает название поля
	* @return название поля
	* @param num порядковый номер поля
	*/
	wstring GetFieldName(unsigned num);
	/** возвращает обозначение поля
	* @return обозначение
	* @param num порядковый номер поля
	*/
	wstring GetFiledDescription(unsigned num);
	/** возвращает дефолтное значение строкой
	* @return значение
	* @param num порядковый номер поля
	*/
	wstring GetFieldDefault(unsigned num);
	/** возвращает Offset  
	* @return uint32_t
	* @param num порядковый номер поля
	*/
	uint32_t GetFieldOffset(int num);
	/** возвращает
	* @return
	* @param num порядковый номер поля
	*/
	wstring GetFieldGroup(unsigned num);
	/** возвращает название структуры
	* @return название
	*/
	wstring GetStructName();
	/** возвращает обозначение структуры
	* @return название
	*/
	wstring GetStructDescription();
	/** возвращает title структуры
	* @return название
	*/
	wstring GetStructTitle();
	/** Сброс состояния парсера */
	void ResetParser();
	/** Установка режима работы программы
	* @param Mode режим
	*/
	void SetMode(ZetParserMode Mode);
	/** Функция возвращает текущую Header сруктуру
	* @return ZetHeader*
	*/
	ZetHeader* GetHeader();
	/** Функция возвращает текущий режим работы
	* @return текущий режим работы
	*/
	ZetParserMode GetMode();
	/** Возвращает текущее значение в поле
	* @return возвращает значение строкой
	* @param порядковый номер поля
	*/
	wstring GetCurrentVal(unsigned num);
	/** Возвращает значение CountPow2 для поля
	* @return CountPow2
	* @param num порядковый номер поля
	*/
	int GetFieldCountPow2(unsigned num);
	/** Запись нового значения в поле
	* @param num порядковый номер
	* @param val значение строкой 
	*/
	void WriteValue(unsigned num, wstring& val);
	/** Возвращает количество полей в листе
	* @return количество полей
	* @param num порядковый номер поля
	*/
	long GetStringListCount(unsigned num);
	/** Возвращает количество полей в строковом листе
	* @return количество строковых полей
	* @param num порядковый номер поля
	*/
	long GetListCount(unsigned num);
	/** Возвращает строку в полями
	* @return строка с полями
	* @param num порядковый номер
	*/
	wstring GetList(unsigned num);
	/** Возвращает строку со строковыми полями
	* @return строка со строковыми полями
	* @param num порядковый номер
	*/
	wstring GetStringList(unsigned num);
	/** Возвращает количество элементов в массиве данного поля
	* @return число элементов массива
	* @param num порядковый номер поля
	* @param bCheckArray проверяем массивы переменной длины
	*/
	int GetFieldArraySize(unsigned num, bool bCheckArray = false, int HeaderSize = 0);
	/** Возвращает строку со значением элемента массива в заданном поле
	* @return строка со значение поля
	* @param num порядковый номер поля, порядковый номер элемента массива
	* @param bCheckArray проверяем массивы переменной длины
	*/
	wstring GetFieldArrayVal(unsigned fieldnum, int arraynum, bool bCheckArray = false);
	/** Если поле содержит массив то возвращает offset элемента этого массива
	* @return offset элемента
	* @param num порядковый номер поля, порядковый номер элемента массива
	*/
	uint32_t GetArrayElementOffset(unsigned fieldnum, int arraynum);
	/** возвращает название структуры из дескриптора
	* @return название
	* @param _pSchm - сама схема, _size - ее размер
	*/
	wstring GetStructNameFromDescriptor(_In_ const BYTE* _pSchm, _In_ const size_t _size);
	/** возвращает дескриптор структуры из дескриптора
	* @return название
	* @param _pSchm - сама схема, _size - ее размер
	*/
	wstring GetStructDescriptionFromDescriptor(_In_ const BYTE* _pSchm, _In_ const size_t _size);
protected:
	/** Преобразование строки в значение
	* @return значение
	* @param val строка со значением
	*/
	template <typename T>
	T SetNewValue(const wstring& val);
	/** Функция установки текущих значений
	* @param f экземпляр класса Field
	*/
	void SetVal(Field f);
	/** Установка дефолтных значений
	* @param f экземпляр класса Field
	*/
	void SetDefVal(Field f);
	/** Установка текущих значений */
	void SetValues();
	/** Преобразование строки из char в wchar_t 
	* @return преобразованная строка
	* @param str входная строка
	*/
	wstring GetWstring(const std::string& str);
	/** Преобразование строки из wchar_t в char
	* @return преобразованная строка
	* @param str входная строка
	*/
	std::string GetString(const wstring& str);
	/** Вектор с полями */
	vector<Field> fields;
	/** Вектор для входного буфера */
	binary m_data;
	/** Вектор для хранения параметров */
	binary m_prop;
	/** Вектор для хранения старых параметров */
	binary m_propOld;
	/** Структура буфера */
	Structure m_struct;
	/** Объект для открытия схемы */
	Reader m_Reader;
	/** Флаг указывающий на режим работы парсера */
	ZetParserMode m_Mode;
	/** Указатель на структуру хедера */
	ZetHeader* m_pHeader;
	/** Указатель на структуру футера */
	ZetFooter* m_pFooter;
	/** Смещение в файле параметров */
	size_t m_PropShift;
	/** Название программы */
	wstring m_ProgName;
	/** Смещение разное для разных режимов работы */
	wstring SetCurrentValue(BYTE* addr, BaseType type);
	/** режим работы парсера */
	size_t m_ModeShift;
	/** Текущие значения полей */
	deque<wstring> m_CurValue;
};
}
