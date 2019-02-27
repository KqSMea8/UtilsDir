#pragma once

//*************************************************************************************************
// структура которая передается в GrammaViewer из TrendViewer
struct TrendViewerCopyData
{
	//  размеры структуры и данных
	size_t					Size_of_FullStruct;		// Размер всей структуры
	size_t					Size_of_Stuct;			// Размер структуры без поля с данными и дескриптором
	size_t					Size_of_Elements;		// Размер в байтах массива с количеством элементов в канале
	size_t					Size_of_Data__ForChans;	// Размер в байтах массива с данными для каналов
	size_t					Size_of_Descr_ForChans;	// Размер в байтах массива с дескрипторами для каналов
	size_t					Size_of_Data;			// Размер в байтах массива с данными
	size_t					Size_of_Descriptor;		// Размер в байтах массива с дескрипторами
	// *********************************************************************************************
	unsigned				Field;					// Номер поля для анализа
	size_t					CountChannels;			// Количество каналов
	// *********************************************************************************************
	size_t *				ElementsInEachChannel;	// Количество элементов в каждом канале
	size_t *				SizeDataEachChannel;	// Размер дескриптора для каждого канала
	size_t *				SizeDescriptorEachChannel;	// Размер дескриптора для каждого канала
	BYTE *					Data;					// Массив с данными для анализа
	BYTE *					Descriptor;				// Дескриптор для передаваемой структуры
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
// структура которая передается в GrammaViewer из ZetTrends
struct GrammaViewerCopyDataNumbers
{
	size_t					Size_of_FullStruct;		// размер всей структуры
	size_t					Size_of_Stuct;			// размер структуры без поля с данными
	size_t					CountElements;			// Общее количество элементов в массивах
	size_t					Size_of_DataArray;		// размер всей структуры
	size_t					Size_of_TimeArray;		// размер всей структуры
	float *					DataArray;				// массив с данными
	double *				TimeArray;				// массив со временем
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