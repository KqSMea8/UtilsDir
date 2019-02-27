#include "stdafx.h"
#include "ChannelsFilter.h"
#include <algorithm>

CChannelsFilter::CChannelsFilter()
{
}


CChannelsFilter::~CChannelsFilter()
{
}

void CChannelsFilter::SetChannelsList(_In_ const std::vector<FilteredChannelStruct> &ChannelsList)
{
	m_ChannelsStructs = ChannelsList;
}

std::vector<FilteredChannelStruct> CChannelsFilter::GetChannelsList(_In_opt_ unsigned	 Group_Count /*= MINIMUM_GROUPS_COUNT*/,
																	_In_opt_ SortTypes	 SortType	 /*= Sort_Disabled*/,
																	_In_opt_ FilterTypes FilterType	 /*= Filter_Disabled*/,
																	_In_opt_ std::vector<CString> Conversion /*= std::vector<CString>()*/)
{
	std::vector<FilteredChannelStruct> ResultVect = SortList(m_ChannelsStructs, SortType);
	FilterList(ResultVect, FilterType, Conversion);
	GroupByNames(ResultVect, Group_Count);

	return ResultVect;
}

void CChannelsFilter::FilterList(_Inout_ std::vector<FilteredChannelStruct> &Vect,
								 _In_ FilterTypes FilterType,
								 _In_opt_ std::vector<CString> Conversion /*= std::vector<CString>()*/)
{
	switch (FilterType)
	{
		case Filter_Disabled:
			// ���������� ���
			if (FilterType == Filter_Disabled)
			{
				for (auto &it : Vect)
					it.bEnabled = true;
			}
			break;
		case Filter_By_Conversion:
			// ��������������� ������ � ������������� ��������� ���������
			if (!Conversion.empty())
			{
				for (auto &it : Vect)
				{
					if (it.bEnabled)
					{
						// ���������� ���� ������ ����������� ������ ���������
						for (auto Conv : Conversion)
						{
							if (it.Conversion == Conv)
							{
								it.bEnabled = false;
								break;
							}
						}
					}
				}
			}
			break;
		default:
			break;
	}
}

bool SortByName		  (_In_ FilteredChannelStruct First, _In_ FilteredChannelStruct Second)
{
	return First.Name.MakeLower() < Second.Name.MakeLower();
}

bool SortByConvesation(_In_ FilteredChannelStruct First, _In_ FilteredChannelStruct Second)
{
	if (First.Conversion == Second.Conversion)
		return First.Name.MakeLower() < Second.Name.MakeLower();
	else
		return First.Conversion.MakeLower() < Second.Conversion.MakeLower();
}

std::vector<FilteredChannelStruct> CChannelsFilter::SortList(_In_ std::vector<FilteredChannelStruct> Vect, 
															 _In_ SortTypes SortType)
{	
	switch (SortType)
	{
		case Sort_Disabled:
			break;
		case Sort_By_Name:
			std::sort(Vect.begin(), Vect.end(), SortByName);
			break;
		case Sort_By_Conversion:
			std::sort(Vect.begin(), Vect.end(), SortByConvesation);
			break;
		default:
			break;
	}

	return Vect;
}

// ���������� ���������� ��������� ���� � �����
int GetNumberOfMatchingLetters(_In_ CString FirstStr, _In_ CString SecondStr)
{
	int Letter(0);	// ���������� ���������� �����
	int CountLetters = min(FirstStr.GetLength(), SecondStr.GetLength());
	for (; Letter <= CountLetters; Letter++)
	{
		if (FirstStr.Left(Letter) != SecondStr.Left(Letter))
		{
			Letter--;
			break;
		}
	}
	if (Letter > CountLetters)
		Letter--;

	// ��������� ���������� ������ �� ������� ������������ ������
	while (Letter > 0)
	{
		int Character = FirstStr[Letter - 1];
		if (Character >= 0 && Character <= 256)
		{
			// ���� ��� ���� ���������� �� ��������� ���������� ����������
			if (((ispunct(Character)) != 0) || (FirstStr[Letter - 1] == L' '))
				Letter--; 
			else if (isdigit(Character) != 0)
			{
				// ���� ��������� ������ �����, �� ��������� ���������� ������, ���� �� ���� �����, �� ��������� ������
				if (FirstStr.GetLength() > Letter)
				{
					int NextCharacter = FirstStr[Letter];
					if (NextCharacter >= 0 && NextCharacter <= 256)
						if (isdigit(NextCharacter) != 0)
						{
							Letter--;
							continue;
						}
				}
				if (SecondStr.GetLength() > Letter)
				{
					int NextCharacter = SecondStr[Letter];
					if (NextCharacter >= 0 && NextCharacter <= 256)
						if (isdigit(NextCharacter) != 0)
						{
							Letter--;
							continue;
						}
				}
				// ���� �� ����� �� continue �� ��������� ����
				break;
			}
			else if (isalpha(Character) != 0)
			{
				// ���� ��������� ������ �����, �� ��������� ���������� ������, ���� �� ���� �����, �� ��������� ������
				if (FirstStr.GetLength() > Letter)
				{
					int NextCharacter = FirstStr[Letter];
					if (NextCharacter >= 0 && NextCharacter <= 256)
						if (isalpha(NextCharacter) != 0)
						{
							Letter--;
							continue;
						}
				}
				if (SecondStr.GetLength() > Letter)
				{
					int NextCharacter = SecondStr[Letter];
					if (NextCharacter >= 0 && NextCharacter <= 256)
						if (isalpha(NextCharacter) != 0)
						{
							Letter--;
							continue;
						}
				}
				// ���� �� ����� �� continue �� ��������� ����
				break;
			}
			else
				break;
		}
		else
			break;
	}

	return Letter;
}

int GetNumberOfNotMatchingLetters(_In_ CString Str, _In_ int Count_Matched_Letters)
{
	int StrLength = Str.GetLength();
	if (StrLength > Count_Matched_Letters)
	{
		while (StrLength > Count_Matched_Letters)
		{			
			int Character = Str[Count_Matched_Letters];
			if (Character >= 0 && Character <= 256)
			{
				// ���� ��� ���� ���������� �� ��������� ���������� ����������
				if ((ispunct(Character)) != 0)
					Count_Matched_Letters++;
				else
					break;
			}
			else
				break;
		}

		return StrLength - Count_Matched_Letters;
	}
	else
		return 0;
}

// �������� ��������� ������
int GetNextIndex(_In_ int Index, _In_ std::vector<FilteredChannelStruct> &DefaultVector, int Group)
{
	int VectorSize = DefaultVector.size();
	if (Index >= VectorSize - 1)
		return VectorSize - 1;

	int NextIndex = Group == 0 ? Index + 1 : DefaultVector[Index].GetGroupLastIndex(Group - 1) + 1;
	for (; NextIndex < VectorSize;)
	{
		if (DefaultVector[NextIndex].bEnabled)
			break;
		else
			NextIndex++;
	}
	return NextIndex;
}

// �������� ��������� ������
int GetPrevIndex(_In_ int Index, _In_ std::vector<FilteredChannelStruct> &DefaultVector)
{
	int PrevIndex = Index - 1;
	for (; PrevIndex >= 0; PrevIndex--)
	{
		if (DefaultVector[PrevIndex].bEnabled)
			break;
	}

	if (PrevIndex < 0)
		PrevIndex = 0;
	return PrevIndex;
}

int GetMatchingLetters(_In_ int Index1, _In_ int Index2, _In_ std::vector<FilteredChannelStruct> &DefaultVector, _In_ int Group)
{
	int VectorSize = DefaultVector.size();
	if ((Index1 == Index2) || (Index2 >= VectorSize) || (Index1 >= VectorSize))
		return 0;
	else
		return GetNumberOfMatchingLetters(DefaultVector[Index1].GetName(Group), DefaultVector[Index2].GetName(Group));
}

void CChannelsFilter::GroupByNames(_Inout_ std::vector<FilteredChannelStruct> &DefaultVector,
								   _In_opt_ unsigned Group_Count /*= MINIMUM_GROUPS_COUNT*/)
{
	// ������� ���������� ��������������� �����
	for (auto &it : DefaultVector)
	{
		it.MergeInfo.clear();
		it.MergeInfo.resize(Group_Count);
	}

	int CountMatchedLetters, CountMatchedLettersNext, CountMatchedLettersNextNext;
	int VectorSize = DefaultVector.size();
	for (unsigned Group = 0; Group < Group_Count; Group++)
	{
		for (int Index = 0; Index < VectorSize - 1; Index++)
		{
			if (!DefaultVector[Index].bEnabled)
				continue;

			int NextIndex = GetNextIndex(Index, DefaultVector, Group);
			int NextNextIndex = GetNextIndex(NextIndex, DefaultVector, Group);

			// ���������� ����� �������� � ������� ������
			CountMatchedLetters = GetMatchingLetters(Index, NextIndex, DefaultVector, Group); 
			// ���������� ����� �������� �� ��������� ������
			CountMatchedLettersNext = GetMatchingLetters(NextIndex, NextNextIndex, DefaultVector, Group);
			CString UnionString;

			// ��������� ����� �� ������������ ��������� ������� � ���-����
			if (CountMatchedLetters > 0 || CountMatchedLettersNext > 0)
			{
				int StopMergeIndex;		// ������ � �������� ����� ����������� ��������� �������� �������
				int StartMergeIndex;	// ������ � �������� ������ ������������ ��������

				// ���� � ���������� ���������� ������ ��� ������� �� ������� � � �������, �������� ����������
				if (CountMatchedLetters >= CountMatchedLettersNext)
				{
					UnionString = DefaultVector[NextIndex].GetName(Group).Left(CountMatchedLetters);

					StartMergeIndex = Index;
					StopMergeIndex = NextIndex;
				}
				else
				{
					// ������� ������ ������� ����� ����� ��������� � ���������� ������
					int TempNextElement, TempNextNextElement, tMatchedLettersNext, tMatchedLettersNextNext;
					TempNextElement = NextNextIndex;
					for (;;)
					{
						TempNextElement		= GetNextIndex(TempNextElement, DefaultVector, Group);
						TempNextNextElement = GetNextIndex(TempNextElement, DefaultVector, Group);
						if (TempNextElement == TempNextNextElement)
							break;
						tMatchedLettersNext		= GetMatchingLetters(NextIndex, TempNextElement, DefaultVector, Group);
						tMatchedLettersNextNext = GetMatchingLetters(TempNextElement, TempNextNextElement, DefaultVector, Group);

						// ���� � ������� �������� ���������� ������� �� ������� � �� ��������� �� �������� �������� �� ���������
						if ((tMatchedLettersNext > 0) && (tMatchedLettersNext > tMatchedLettersNextNext) && (tMatchedLettersNext == CountMatchedLettersNext))
						{
							NextNextIndex = TempNextNextElement;
						}
						else
							break;
					}

					CountMatchedLetters = CountMatchedLettersNext;
					UnionString = DefaultVector[NextIndex].GetName(Group).Left(CountMatchedLetters);

					StartMergeIndex = NextIndex;
					StopMergeIndex = NextNextIndex - 1;

					// ������� ������� ������ �� ����������
					for (; Index < NextIndex; Index++)
					{
						DefaultVector[Index].MergeInfo[Group].bMerged = false;
						DefaultVector[Index].MergeInfo[Group].MergeStartIndex = Index;
						DefaultVector[Index].MergeInfo[Group].MergeLastIndex = Index;
					}
				}

				// �������� �� ���� ��������� ��������� ���� �� ����� ����������
				for (; StopMergeIndex < VectorSize;)
				{
					if (!DefaultVector[StopMergeIndex].bEnabled)
						continue;

					NextIndex	  = GetNextIndex(StopMergeIndex, DefaultVector, Group);
					NextNextIndex = GetNextIndex(NextIndex, DefaultVector, Group); 

					// ���������� ����� �������� �� ��������� ������
					CountMatchedLettersNext = GetMatchingLetters(StopMergeIndex, NextIndex, DefaultVector, Group); 
					// ���������� ����� �������� �� ��������� ������
					CountMatchedLettersNextNext = GetMatchingLetters(NextIndex, NextNextIndex, DefaultVector, Group); 

					if (CountMatchedLettersNext < CountMatchedLettersNextNext)
						break;

					// ���� ���������� ��������� �������� � ������� ��������� ������ ��� �� ���������
					if (CountMatchedLetters < CountMatchedLettersNext)
						break;

					if (CountMatchedLetters != CountMatchedLettersNext)
					{
						if (Group != 0 /*&& NextIndex != StopMergeIndex*/)
							StopMergeIndex = GetPrevIndex(NextIndex, DefaultVector);
						break;
					}

					StopMergeIndex = NextIndex;
					if (StopMergeIndex == VectorSize - 1)
						break;
				}

				Index = StartMergeIndex;
				for (; Index < StopMergeIndex + 1; Index++)
				{
					if (Index >= VectorSize)
					{
						Index = VectorSize;
						break;
					}

					if (!DefaultVector[Index].bEnabled)
						continue;
					// ������ ��������� �������� �����		
					DefaultVector[Index].MergeInfo[Group].bMerged			= true;
					DefaultVector[Index].MergeInfo[Group].MergeName			= UnionString;
					DefaultVector[Index].MergeInfo[Group].MergeStartIndex	= StartMergeIndex;
					DefaultVector[Index].MergeInfo[Group].MergeLastIndex	= StopMergeIndex;
				}
				Index--;		
				continue;
			}			
			else
			{
				// ������� ������� ������ �� ����������
				DefaultVector[Index].MergeInfo[Group].bMerged = false;
				DefaultVector[Index].MergeInfo[Group].MergeStartIndex = Index;
				DefaultVector[Index].MergeInfo[Group].MergeLastIndex = Index;
			}
		}

		bool bNameExist(false);
		// ��������� ��� ����� � ������ ����
		for (auto it : DefaultVector)
		{
			if (!it.IsGroupNameEmpty(Group))
			{
				bNameExist = true;
				break;
			}
		}

		if (!bNameExist)
			break;
	}

	// ������� ������������� ������
	for (auto &it : DefaultVector)
	{
		if (!it.bEnabled)
			continue;

		for (unsigned Group = 0; Group < Group_Count - 1; Group++)
		{
			// ���� ���� ����������� � ������
			if (it.MergeInfo[Group].bMerged && it.MergeInfo[Group + 1].bMerged)
			{
				// ���� ������� �����
				if (it.MergeInfo[Group].MergeName == it.MergeInfo[Group + 1].MergeName)
				{
					for (unsigned GroupInd = Group; GroupInd < Group_Count - 1; GroupInd++)
					{	
						// ���� ��������� ������ �� ���������� �� ��� ������ �������� ��������� ������������, ������� ��
						if (!it.MergeInfo[GroupInd + 1].bMerged)
						{
							it.MergeInfo[GroupInd].Clear();
							break;
						}						

						it.MergeInfo[GroupInd] = it.MergeInfo[GroupInd + 1];

						// ���� ��������� ������ �������� ��������� �� ������� ��
						if (GroupInd + 1 == Group_Count - 1)
						{
							it.MergeInfo[GroupInd + 1].Clear();
							break;
						}
					}
				}
			}
			else
				break;
		}
	}
}

void CChannelsFilter::DeleteDisabled(_Inout_ std::vector<FilteredChannelStruct> &DefaultVector)
{
	unsigned Index(0);		// ������� ��������������� ������
	// ������� ���������� ��������
	for (auto ChannelInfo = DefaultVector.begin(); ChannelInfo != DefaultVector.end();)
	{
		if (!ChannelInfo->bEnabled)
		{
			// ��� �������� �������� ����������� � ������ �� �������� ������ ����� ��������� ���������� �� ������

			// ������� � ������ �������� ������� ����������� � ��������� ������ ���������� �����������
			for (auto ChannelNew = DefaultVector.begin(); ChannelNew != ChannelInfo; ++ChannelNew)
			{
				if (ChannelNew->bEnabled)
				{
					for (int Group = 0, GroupCount = ChannelInfo->MergeInfo.size(); Group < GroupCount; Group++)
					{
						if (ChannelNew->MergeInfo[Group].MergeLastIndex >= Index)
							ChannelNew->MergeInfo[Group].MergeLastIndex--;
					}
				}
			}
			
			// ���� ��������� ����� ��������� ��������� �������
			for (auto ChannelNew = ChannelInfo + 1; ChannelNew != DefaultVector.end(); ++ChannelNew)
			{
				if (ChannelNew->bEnabled)
				{
					for (int Group = 0, GroupCount = ChannelInfo->MergeInfo.size(); Group < GroupCount; Group++)
					{
						if (ChannelNew->MergeInfo[Group].MergeStartIndex == Index)
							ChannelNew->MergeInfo[Group].MergeStartIndex++;
						else if (ChannelNew->MergeInfo[Group].MergeStartIndex > Index)
							ChannelNew->MergeInfo[Group].MergeStartIndex--;
						ChannelNew->MergeInfo[Group].MergeLastIndex--;
					}
				}
			}
			ChannelInfo = DefaultVector.erase(ChannelInfo);
		}
		else
		{
			++ChannelInfo;
			Index++;
		}
	}
}

CString GetNameFromVector(_In_ std::vector<CString> &Vect, _In_ int Index)
{
	int VectSize = Vect.size();
	if (Index < VectSize)
		return Vect[Index];
	else
		return CString();
}

void ShortenChannelNames(_Inout_ std::vector<CString> &Vect, _In_ unsigned Symbols_Count)
{
	int CountMatchedLetters;			// ���������� ����� �������� � ������� �������
	int CountMatchedLettersNext;		// ���������� ����� �������� �� ��������� ��������
	int CountMatchedLettersNextNext;	// ���������� ����� �������� �� ��������� ��������
	
	int CountNotMatchedLetters;			// ���������� �� ��������� �������� � ������� �������     ��� ������ ����������� 
	
	for (int Index = 0, VectorSize = Vect.size(); Index < VectorSize /*- 1*/; Index++)
	{
		// ���� �������� ���������� �������� ������ 0, �� ������� ������
		if (Symbols_Count == 0)
			Vect[Index].Empty();
		else if (Vect[Index].GetLength() > (int)Symbols_Count)
		{
			int NextIndex = Index + 1;
			int NextNextIndex = NextIndex + 1;

			// ���������� ����� �������� � ������� ������
			CountMatchedLetters		= GetNumberOfMatchingLetters(GetNameFromVector(Vect, Index), GetNameFromVector(Vect, NextIndex));
			// ���������� ����� �������� �� ��������� ������
			CountMatchedLettersNext = GetNumberOfMatchingLetters(GetNameFromVector(Vect, NextIndex), GetNameFromVector(Vect, NextNextIndex));
			CString UnionString(L"");

			// ��������� ����� �� ������������ ��������� ������� � ���-����
			if (CountMatchedLetters > 0 || CountMatchedLettersNext > 0)
			{
				int StopMergeIndex;		// ������ � �������� ����� ����������� ��������� �������� �������
				int StartMergeIndex;	// ������ � �������� ������ ������������ ��������

				// ���� � ���������� ���������� ������ ��� ������� �� ������� � � �������, �������� ����������
				if (CountMatchedLetters >= CountMatchedLettersNext)
				{
					StartMergeIndex = Index;
					StopMergeIndex = NextIndex;
				}
				else
				{
					// ������� ������ ������� ����� ����� ��������� � ���������� ������
					int TempNextElement, TempNextNextElement, tMatchedLettersNext, tMatchedLettersNextNext;
					TempNextElement = NextNextIndex;
					for (;;)
					{
						TempNextElement = TempNextElement + 1;
						TempNextNextElement = TempNextElement + 1;
						if (TempNextElement == TempNextNextElement)
							break;
						tMatchedLettersNext		= GetNumberOfMatchingLetters(GetNameFromVector(Vect, NextIndex), 
																			 GetNameFromVector(Vect, TempNextElement));
						tMatchedLettersNextNext = GetNumberOfMatchingLetters(GetNameFromVector(Vect, TempNextElement), 
																			 GetNameFromVector(Vect, TempNextNextElement));

						// ���� � ������� �������� ���������� ������� �� ������� � �� ��������� �� �������� �������� �� ���������
						if ((tMatchedLettersNext > 0) &&
							(tMatchedLettersNext > tMatchedLettersNextNext) &&
							(tMatchedLettersNext == CountMatchedLettersNext))
						{
							NextNextIndex = TempNextNextElement;
						}
						else
							break;
					}

					CountMatchedLetters = CountMatchedLettersNext;

					StartMergeIndex = NextIndex;
					StopMergeIndex = NextNextIndex - 1;

					// ������� ������� ������ �� ����������
					for (; Index < NextIndex; Index++)
					{
						// ���� �� ������� ����� ����� � ��������� �������, �� ������� �� �������� ������ ��������
						// ���� ������ �� ������ ������� ��� �������, � ����� �������� ������� �� ���������
						int MiddleWordIndex;
						while ((MiddleWordIndex = Vect[Index].GetLength() / 2) >= 0)
						{
							Vect[Index].Delete(MiddleWordIndex, 1);

							if (Vect[Index].GetLength() < (int)Symbols_Count)
							{
								Vect[Index].Insert(MiddleWordIndex, L"�");
								break;
							}
						}
					}
				}

				// �������� �� ���� ��������� ��������� ���� �� ����� ����������
				for (; StopMergeIndex < VectorSize;)
				{
					NextIndex = StopMergeIndex + 1;
					NextNextIndex = NextIndex + 1;

					// ���������� ����� �������� �� ��������� ������
					CountMatchedLettersNext		= GetNumberOfMatchingLetters(GetNameFromVector(Vect, StopMergeIndex), 
																			 GetNameFromVector(Vect, NextIndex));
					// ���������� ����� �������� �� ��������� ������
					CountMatchedLettersNextNext = GetNumberOfMatchingLetters(GetNameFromVector(Vect, NextIndex), 
																			 GetNameFromVector(Vect, NextNextIndex));

					if (CountMatchedLettersNext < CountMatchedLettersNextNext)
						break;

					// ���� ���������� ��������� �������� � ������� ��������� ������ ��� �� ���������
					if (CountMatchedLetters < CountMatchedLettersNext)
						break;

					if (CountMatchedLetters != CountMatchedLettersNext)
					{
						StopMergeIndex = NextIndex - 1;
						break;
					}

					StopMergeIndex = NextIndex;
					if (StopMergeIndex == VectorSize - 1)
						break;
				}
	
				// �������� ���������� �� ��������� ��������
				CountNotMatchedLetters = CountMatchedLetters > 0 ? GetNumberOfNotMatchingLetters(GetNameFromVector(Vect, Index), CountMatchedLetters) : 0;
				// ��������� ���������� ��������� ����� �� ������� ��� �����
				if (CountMatchedLetters + 1 + CountNotMatchedLetters > (int)Symbols_Count)
					CountMatchedLetters = Symbols_Count - CountNotMatchedLetters - 1;
				if (CountMatchedLetters < 0)
					CountMatchedLetters = 0;
				// ��������� ���������� ��������� ������ �� ������� ��� �����
				if (CountMatchedLetters + 1 + CountNotMatchedLetters >(int)Symbols_Count)
					CountNotMatchedLetters = Symbols_Count - CountMatchedLetters - 1;
				if (CountNotMatchedLetters < 0)
					CountNotMatchedLetters = 0;

				// ���� ����� ������������ ������ ����� ������ ��� ������, �� ��������� � ����� ��������
				if (CountMatchedLetters + 1 + CountNotMatchedLetters < (int)Symbols_Count)
					CountNotMatchedLetters = Symbols_Count - CountMatchedLetters - 1;

				Index = StartMergeIndex;
				for (; Index < StopMergeIndex + 1; Index++)
				{
					if (Index >= VectorSize)
					{
						Index = VectorSize;
						break;
					}
					
					// ������ ��������� �������� �����	
					Vect[Index] = Vect[Index].Left(CountMatchedLetters) + L"�" + Vect[Index].Right(CountNotMatchedLetters);
				}
				Index--;
				continue;
			}
			else
			{
				// ������� ������� ������ �� ����������
				// ���� �� ������� ����� ����� � ��������� �������, �� ������� �� �������� ������ ��������
				// ���� ������ �� ������ ������� ��� �������, � ����� �������� ������� �� ���������
				int MiddleWordIndex;
				while ((MiddleWordIndex = Vect[Index].GetLength() / 2) >= 0)
				{
					Vect[Index].Delete(MiddleWordIndex, 1);

					if (Vect[Index].GetLength() < (int)Symbols_Count)
					{
						Vect[Index].Insert(MiddleWordIndex, L"�");
						break;
					}
				}
			}
		}
	}
}
