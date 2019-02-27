#pragma once

//*************************************************************************************************
// ��������� ������� ���������� � GrammaViewer �� TrendViewer
struct TrendViewerCopyData
{
	//  ������� ��������� � ������
	size_t					Size_of_FullStruct;		// ������ ���� ���������
	size_t					Size_of_Stuct;			// ������ ��������� ��� ���� � ������� � ������������
	size_t					Size_of_Elements;		// ������ � ������ ������� � ����������� ��������� � ������
	size_t					Size_of_Data__ForChans;	// ������ � ������ ������� � ������� ��� �������
	size_t					Size_of_Descr_ForChans;	// ������ � ������ ������� � ������������� ��� �������
	size_t					Size_of_Data;			// ������ � ������ ������� � �������
	size_t					Size_of_Descriptor;		// ������ � ������ ������� � �������������
	// *********************************************************************************************
	unsigned				Field;					// ����� ���� ��� �������
	size_t					CountChannels;			// ���������� �������
	// *********************************************************************************************
	size_t *				ElementsInEachChannel;	// ���������� ��������� � ������ ������
	size_t *				SizeDataEachChannel;	// ������ ����������� ��� ������� ������
	size_t *				SizeDescriptorEachChannel;	// ������ ����������� ��� ������� ������
	BYTE *					Data;					// ������ � ������� ��� �������
	BYTE *					Descriptor;				// ���������� ��� ������������ ���������
	// *********************************************************************************************
	TrendViewerCopyData()
		: Size_of_FullStruct		(sizeof(TrendViewerCopyData))
		, Size_of_Stuct				(NULL)
		, Size_of_Elements			(NULL)
		, Size_of_Data__ForChans	(NULL)
		, Size_of_Descr_ForChans	(NULL)
		, Size_of_Data				(NULL)
		, Size_of_Descriptor		(NULL)
		, CountChannels				(NULL)
		, Field						(NULL)
		, ElementsInEachChannel		(nullptr)
		, SizeDataEachChannel		(nullptr)
		, SizeDescriptorEachChannel	(nullptr)
		, Data						(nullptr)
		, Descriptor				(nullptr)
	{}
	// *********************************************************************************************
	~TrendViewerCopyData()
	{
		DeleteAll();
	}
	void TrendViewerCopyData::DeleteAll()
	{
		if (ElementsInEachChannel)
		{ 
			delete[] ElementsInEachChannel;
			ElementsInEachChannel = nullptr;
		}

		if (SizeDataEachChannel)
		{ 
			delete[] SizeDataEachChannel;
			SizeDataEachChannel = nullptr;
		}

		if (SizeDescriptorEachChannel)
		{ 
			delete[] SizeDescriptorEachChannel;
			SizeDescriptorEachChannel = nullptr;
		}

		if (Data)
		{ 
			delete[] Data;
			Data = nullptr;
		}

		if (Descriptor)
		{ 
			delete[] Descriptor;
			Descriptor = nullptr;
		}
	}
}; 
//*************************************************************************************************
// ��������� ������� ���������� � GrammaViewer �� ZetTrends
struct GrammaViewerCopyDataNumbers
{
	size_t					Size_of_FullStruct;		// ������ ���� ���������
	size_t					Size_of_Stuct;			// ������ ��������� ��� ���� � �������
	size_t					CountElements;			// ����� ���������� ��������� � ��������
	size_t					Size_of_DataArray;		// ������ ���� ���������
	size_t					Size_of_TimeArray;		// ������ ���� ���������
	float *					DataArray;				// ������ � �������
	double *				TimeArray;				// ������ �� ��������
	GrammaViewerCopyDataNumbers()
		: Size_of_FullStruct	(0)
		, Size_of_Stuct			(0)
		, CountElements			(0)
		, Size_of_DataArray		(0)
		, Size_of_TimeArray		(0)
		, DataArray				(nullptr)
		, TimeArray				(nullptr)
	{}
	~GrammaViewerCopyDataNumbers()
	{
		DeleteAll();
	}
	void GrammaViewerCopyDataNumbers::DeleteAll()
	{
		if (DataArray)
		{ 
			delete[] DataArray;
			DataArray = nullptr;
		}
		if (TimeArray)
		{ 
			delete[] TimeArray;
			TimeArray = nullptr;
		}
	}
};
//*************************************************************************************************