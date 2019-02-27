#pragma once
#include <vector>
#include <chaninfo.h>
//*************************************************************************************************
#define MINIMUM_GROUPS_COUNT  0	// ���������� ����� � ������� ����� ���������� ������
//*************************************************************************************************
struct MergeInfoStuct			// ��������� ���������� ���������� � ����� ��������� ���� �����������
{
	bool bMerged;				// ���� ��� ���� ���� ����������
	unsigned MergeStartIndex;	// ������ � ������� � �������� ���������� �����������
	unsigned MergeLastIndex;		// ������ � ������� �� ������� ������������� �����������
	CString MergeName;			// �������� �����������
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
struct FilteredChannelStruct : public _CHANNEL_STRUCT	// ��������� ������ � ��������������� �����������
{
	long ChannelID;							// ������ ������
	bool bEnabled;							// true - ����� �� ��� ������������ \ false - ����� ��� ������������ 
	CString Name;							// �������� ������
	CString Conversion;						// ������� ���������
	std::vector<MergeInfoStuct> MergeInfo;	// ���������� �� ������������ ������
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
	// ��������� ����� ������ �� �� �������
	void SetGroupName(CString NewName, _In_ unsigned GroupIndex)
	{
		if (MergeInfo.size() > GroupIndex)
		{
			MergeInfo[GroupIndex].MergeName = NewName;
		}
	}
	//*********************************************************************************************
	// ���������� ��� ������ �� �� �������
	CString GetGroupName		(_In_ unsigned GroupIndex)
	{
		return MergeInfo[GroupIndex].MergeName;		
	}
	//*********************************************************************************************
	// ���������� ������ � �������� ���������� ����������� � ������
	unsigned GetGroupStartIndex	(_In_ unsigned GroupIndex)
	{
		return MergeInfo[GroupIndex].MergeStartIndex;
	}
	//*********************************************************************************************
	// ���������� ������ �� ������� ������������� ����������� � ������
	unsigned GetGroupLastIndex	(_In_ unsigned GroupIndex)
	{
		return MergeInfo[GroupIndex].MergeLastIndex;
	}
	//*********************************************************************************************
	// ��������� ��� ��� ������ �����
	bool IsGroupNameEmpty		(_In_ unsigned GroupIndex)
	{
		return MergeInfo[GroupIndex].MergeName.IsEmpty();
	}
	//*********************************************************************************************
	// ���������� ��� ��������� �� ������� ������ (0 - ��� ������, ��� ��� ������ 0 - ����� ������)
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
enum FilterTypes					// �������� ���������� �������
{
	Filter_Disabled	= 1,			// ��� ����������
	Filter_By_Conversion,			// ������ �� �������� ���������
};
//*********************************************************************************************
enum SortTypes						// �������� ���������� �������
{
	Sort_Disabled = 0,				// ��� ����������
	Sort_By_Name,					// ����������� �� ������ �������
	Sort_By_Conversion				// ����������� �� �������� ���������
};
//*************************************************************************************************
class CChannelsFilter
{
public:
	CChannelsFilter();
	~CChannelsFilter();
public:	//*****************************************************************************************
	// ������� ���������� � ������������ �������
	void SetChannelsList(_In_ const std::vector<FilteredChannelStruct>	&ChannelsList);
	//*********************************************************************************************
	/* �������� ������ ��������������� ������� �������� ��������� ���� ���������� � ����������
	* @param FilterType - ��� ���������� �������
	* @param SortType   - ��� ����������
	* @param Conversion - ������ ����������������� ������ ���������
	* @return ��������������� ������*/
	std::vector<FilteredChannelStruct> GetChannelsList(_In_opt_ unsigned	Group_Count = MINIMUM_GROUPS_COUNT,
													   _In_opt_ SortTypes	SortType	= Sort_Disabled,
													   _In_opt_ FilterTypes	FilterType  = Filter_Disabled,
													   _In_opt_ std::vector<CString> Conversion = std::vector<CString>());
	//*********************************************************************************************
	/* ������� ���������� ������� � ������� �� �������
	* @param Vect - ������ ��� ����������
	* @param FilterType - ��� ���������� �������
	* @param Conversion - ������ ����������������� ������ ���������
	* @return ��������������� ������*/
	void FilterList	(_Inout_ std::vector<FilteredChannelStruct> &Vect,
					 _In_ FilterTypes FilterType,
					 _In_opt_ std::vector<CString> Conversion = std::vector<CString>());		
	//*********************************************************************************************
	/* ������� ���������� ������� � ������� �� �������
	* @param Vect - ������ ��� ����������
	* @param SortType   - ��� ����������
	* @return ��������������� ������*/
	std::vector<FilteredChannelStruct> SortList		(_In_ std::vector<FilteredChannelStruct> Vect, 
													 _In_ SortTypes SortType);
	//*********************************************************************************************
	/* ������� ����������� ������� � ������� �� �������, 
	   ��������� ��� ����������� ����� ��������� ���� ������ ����� ������� ��������������
	* @param DefaultVector - ������ ��� �����������
	* @param Group_Count   - ���������� �����, � ������� ����� ���������� ������	
	* @return ��������������� ������*/
	void GroupByNames(_Inout_ std::vector<FilteredChannelStruct> &DefaultVector, 
					  _In_opt_ unsigned Group_Count = MINIMUM_GROUPS_COUNT);
	//*********************************************************************************************
	/* ������� ������� �� ������� ��������������� ��������
	* @param DefaultVector - ������� � ������� ����� ������� �������� ��������*/
	void DeleteDisabled(_Inout_ std::vector<FilteredChannelStruct> &DefaultVector);
private://*****************************************************************************************
	std::vector<FilteredChannelStruct> m_ChannelsStructs;	// �������� ������ �� �������
};	//*********************************************************************************************
//*********************************************************************************************
/* ������� ��������� ����� ������� �� ��������� ���������� ��������, �������� ������� ���������� �� "�"
* @param Vect - ������ � ������� ����� �������� �����
* @param Symbols_Count - ������������ ���������� �������� � ������ �����(� ������ "�")*/
void ShortenChannelNames(_Inout_ std::vector<CString> &Vect, _In_ unsigned Symbols_Count);