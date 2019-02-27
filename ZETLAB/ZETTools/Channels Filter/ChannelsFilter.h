#pragma once
#include <vector>
#include <chaninfo.h>
//*************************************************************************************************
#define MINIMUM_GROUPS_COUNT  0	// количество групп в которые будут объединены каналы
//*************************************************************************************************
struct MergeInfoStuct			// структура содержащая информацию с каким элементом было обьединение
{
	bool bMerged;				// флаг что поле было обьединено
	unsigned MergeStartIndex;	// индекс в массиве с которого происходит обьединение
	unsigned MergeLastIndex;		// индекс в массиве на котором заканчивается обьединение
	CString MergeName;			// название обьединения
	MergeInfoStuct()
	{
		Clear();
	}
	void Clear()
	{
		bMerged = false;
		MergeStartIndex = NULL;
		MergeLastIndex = NULL;
		MergeName.Empty();
	}
	MergeInfoStuct & operator = (const MergeInfoStuct & val)
	{
		bMerged			= val.bMerged;
		MergeStartIndex = val.MergeStartIndex;
		MergeLastIndex	= val.MergeLastIndex;
		MergeName		= val.MergeName;

		return *this;
	}
	bool operator == (const MergeInfoStuct &val) const
	{
		return bMerged			== val.bMerged &&
			   MergeStartIndex	== val.MergeStartIndex &&
			   MergeLastIndex	== val.MergeLastIndex &&
			   MergeName		== val.MergeName;
	}
};
//*************************************************************************************************
struct FilteredChannelStruct : public _CHANNEL_STRUCT	// структура канала с отфильтрованной информацией
{
	long ChannelID;							// Индекс канала
	bool bEnabled;							// true - канал не был отфильтрован \ false - канал был отфильтрован 
	CString Name;							// Название канала
	CString Conversion;						// единицы измерения
	std::vector<MergeInfoStuct> MergeInfo;	// информация об объединенных именах
	FilteredChannelStruct()
		: ChannelID			(-1)
		, bEnabled			(true)
		, Name				(_T(""))
		, Conversion		(_T(""))
	{}
	FilteredChannelStruct(_In_ CString Name, _In_ CString Conversion)
		: ChannelID			(-1)
		, bEnabled			(true)
		, Name				(Name)
		, Conversion		(Conversion)
	{}
	FilteredChannelStruct(const _CHANNEL_STRUCT &Val)
		: _CHANNEL_STRUCT  	(Val)
		, ChannelID			(-1)
		, bEnabled			(true)
		, Name				(Val.name)
		, Conversion		(Val.measure)
	{}	
	FilteredChannelStruct & operator = (const FilteredChannelStruct & val)
	{
		*((_CHANNEL_STRUCT*)this) = *((_CHANNEL_STRUCT*)&val);
		ChannelID	= val.ChannelID;
		bEnabled	= val.bEnabled;
		Name		= val.Name;
		Conversion	= val.Conversion;
		MergeInfo	= val.MergeInfo;

		return *this;
	}
	//*********************************************************************************************
	// установка имени группы по ее индексу
	void SetGroupName(CString NewName, _In_ unsigned GroupIndex)
	{
		if (MergeInfo.size() > GroupIndex)
		{
			MergeInfo[GroupIndex].MergeName = NewName;
		}
	}
	//*********************************************************************************************
	// возвращает имя группы по ее индексу
	CString GetGroupName		(_In_ unsigned GroupIndex)
	{
		return MergeInfo[GroupIndex].MergeName;		
	}
	//*********************************************************************************************
	// возвращает индекс с которого начинается объединение в группу
	unsigned GetGroupStartIndex	(_In_ unsigned GroupIndex)
	{
		return MergeInfo[GroupIndex].MergeStartIndex;
	}
	//*********************************************************************************************
	// возвращает индекс на котором заканчивается объединение в группу
	unsigned GetGroupLastIndex	(_In_ unsigned GroupIndex)
	{
		return MergeInfo[GroupIndex].MergeLastIndex;
	}
	//*********************************************************************************************
	// проверяет что имя группы пусто
	bool IsGroupNameEmpty		(_In_ unsigned GroupIndex)
	{
		return MergeInfo[GroupIndex].MergeName.IsEmpty();
	}
	//*********************************************************************************************
	// возвращает имя структуры по индексу группы (0 - имя канала, все что больше 0 - имена группы)
	CString GetName(_In_opt_ unsigned GroupIndex = 0)
	{
		if (GroupIndex == 0)
			return Name;
		else
		{
			GroupIndex--;
			if (MergeInfo.size() > GroupIndex)
			{
				if (IsGroupNameEmpty(GroupIndex))
					return GetName(GroupIndex);
				else
					return MergeInfo[GroupIndex].MergeName;
			}
		}
		return CString();
	}
	//*********************************************************************************************
};
//*********************************************************************************************
enum FilterTypes					// варианты фильтрации каналов
{
	Filter_Disabled	= 1,			// без фильтрации
	Filter_By_Conversion,			// фильтр по единицам измерения
};
//*********************************************************************************************
enum SortTypes						// варианты сортировки каналов
{
	Sort_Disabled = 0,				// без сортировки
	Sort_By_Name,					// сортировать по именам каналов
	Sort_By_Conversion				// сортировать по единицам измерения
};
//*************************************************************************************************
class CChannelsFilter
{
public:
	CChannelsFilter();
	~CChannelsFilter();
public:	//*****************************************************************************************
	// заносит информацию о существующих каналах
	void SetChannelsList(_In_ const std::vector<FilteredChannelStruct>	&ChannelsList);
	//*********************************************************************************************
	/* получаем список отфильтрованных каналов согласно заданному типу фильтрации и сортировки
	* @param FilterType - тип фильтрации каналов
	* @param SortType   - тип сортировки
	* @param Conversion - массив отфильтровываемых единиц измерения
	* @return Отфильтрованный массив*/
	std::vector<FilteredChannelStruct> GetChannelsList(_In_opt_ unsigned	Group_Count = MINIMUM_GROUPS_COUNT,
													   _In_opt_ SortTypes	SortType	= Sort_Disabled,
													   _In_opt_ FilterTypes	FilterType  = Filter_Disabled,
													   _In_opt_ std::vector<CString> Conversion = std::vector<CString>());
	//*********************************************************************************************
	/* Функция фильтрации массива с данными по каналам
	* @param Vect - вектор для сортировки
	* @param FilterType - тип фильтрации каналов
	* @param Conversion - массив отфильтровываемых единиц измерения
	* @return Отфильтрованный массив*/
	void FilterList	(_Inout_ std::vector<FilteredChannelStruct> &Vect,
					 _In_ FilterTypes FilterType,
					 _In_opt_ std::vector<CString> Conversion = std::vector<CString>());		
	//*********************************************************************************************
	/* Функция сортировки массива с данными по каналам
	* @param Vect - вектор для сортировки
	* @param SortType   - тип сортировки
	* @return отсортированный массив*/
	std::vector<FilteredChannelStruct> SortList		(_In_ std::vector<FilteredChannelStruct> Vect, 
													 _In_ SortTypes SortType);
	//*********************************************************************************************
	/* Функция группировки массива с данными по каналам, 
	   наилучший вид группировки будет достигнут если данные будут заранее отсортированны
	* @param DefaultVector - вектор для группировки
	* @param Group_Count   - количество групп, в которое будут объединены каналы	
	* @return сгруппированный массив*/
	void GroupByNames(_Inout_ std::vector<FilteredChannelStruct> &DefaultVector, 
					  _In_opt_ unsigned Group_Count = MINIMUM_GROUPS_COUNT);
	//*********************************************************************************************
	/* Функция удаляет из вектора отфильтрованные элементы
	* @param DefaultVector - вектора в котором будут удалены ненужные элементы*/
	void DeleteDisabled(_Inout_ std::vector<FilteredChannelStruct> &DefaultVector);
private://*****************************************************************************************
	std::vector<FilteredChannelStruct> m_ChannelsStructs;	// исходные данные по каналам
};	//*********************************************************************************************
//*********************************************************************************************
/* Функция сокращает имена каналов до заданного количества символов, убранные символы заменяются на "…"
* @param Vect - вектор в котором будут заменены имена
* @param Symbols_Count - максимальное количество символов в каждом имени(с учетом "…")*/
void ShortenChannelNames(_Inout_ std::vector<CString> &Vect, _In_ unsigned Symbols_Count);