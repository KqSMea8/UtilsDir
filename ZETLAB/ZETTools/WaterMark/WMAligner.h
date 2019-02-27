#pragma once

#ifndef wmaligner_h
#define wmaligner_h


#include <vector>
#include <list>
#include <algorithm>

#include "MappingOpener.h"
#include "WMInfo.h"

#include <boost/shared_ptr.hpp>
#include <boost/make_shared.hpp>
#include <WaterMark/WaterMarkFloat.h>


const static unsigned long nan_[2]={0xffffffff, 0x7fffffff};

#define FIVEHOURS (3600 * 5)
#define TENHOURS (3600 * 10)
#define BADQUALITY 0
#define NEWFILEPREFIX _T("x")


enum syncro_type
{
	stNone,
	stInsert,
	stCut
};

enum unsynchro_type
{
	utData,
	utTime
};



class Unsynchro
{

public:

	Unsynchro::Unsynchro()
		: from(0)
		, count(0)
		, sync_type(stNone)
	{}

	bool Unsynchro::operator > (const Unsynchro &uns)
	{
		bool ret = false;

		if(uns.unsync_type == utData && unsync_type == utTime)
			ret = true;
		else
			if(uns.unsync_type == utTime && unsync_type == utData)
				ret = false;
			else
				if(uns.from < from)
					ret = true;

		return ret;
	}

	bool Unsynchro::operator < (const Unsynchro &uns)
	{
		return !(operator>(uns));
	}

	LONGLONG from;
	LONGLONG count;
	syncro_type sync_type;
	unsynchro_type unsync_type;
};




class WMAligner
{
public:

	WMAligner::WMAligner(const std::vector<CString> &namesContainer, long frq)
	{
		std::for_each(namesContainer.begin(), namesContainer.end(), [&] (const CString &fName)	// для всех файлов
		{
			std::list <Unsynchro> unsList;

			float *readBuf = NULL;

			boost::shared_ptr <MappingOpener <float> > shp_map = boost::make_shared <MappingOpener <float> > (fName, &readBuf, 0);

			long fSize = shp_map->GetSize();
			long sample_count = fSize / sizeof(float);

			long lAdress = -(WMFSIZE / 2), lRTC;
			unsigned char ucQuality;
			long oldRTC = 0, oldAdress = 0;
			long oldOldRtc = 0;														// значение времени в отсчете перед предыдущим.


			while(lAdress < sample_count &&											//	пока адрес меньше количества элементов и мы находим корректную ватермарку.
					WMF_FindPPS(readBuf + lAdress + WMFSIZE / 2, 
								sample_count - (lAdress + WMFSIZE / 2), 
								&lAdress, &lRTC, &ucQuality) 
								== 0
				)
			{
				if(lAdress != 0)
					lAdress += oldAdress + WMFSIZE / 2;

				TRACE(_T("\nAdress = %d; RTC = %d; QualitySynchro = %d, QualityPower = %d; QualityData = %d."), lAdress, lRTC, long(ucQuality & 0x03), long((ucQuality >> SIZEOFQUALITY) & 0x03), long((ucQuality >> 2 * SIZEOFQUALITY) & 0x03));

				if(oldRTC > 0 && ((ucQuality >> 2 * SIZEOFQUALITY) & 0x03) > BADQUALITY && ((lRTC - oldRTC) != 1  || lAdress - oldAdress != frq) && (oldOldRtc == 0 || abs(oldRTC - oldOldRtc) <= FIVEHOURS))	
				{
					long tmpAdr, tmpRTC;
					unsigned char tmpQ;

					if(WMF_FindPPS(readBuf + lAdress + WMFSIZE / 2, sample_count - (lAdress + WMFSIZE / 2), &tmpAdr, &tmpRTC, &tmpQ) == 0)	// проверка на случайное не адекватное значение данных в ватермарке
					{
						if(((tmpQ >> 2 * SIZEOFQUALITY) & 0x03) > BADQUALITY && tmpRTC - lRTC >= 1 && abs(tmpRTC - lRTC) <= FIVEHOURS)															// для проверки считываем следующую ватермарку и смотрим её параметры.
							CalcUnsynchro(unsList, lAdress, oldAdress, lRTC, oldRTC, frq, sample_count);
					}
				}

				oldOldRtc = oldRTC;
				oldRTC = lRTC;
				oldAdress = lAdress;
			}


			if(!unsList.empty())	// если в файле найдены не синхронизированные части
			{

				unsList.sort();

				ResolveCollision(unsList);

				ReWriteFile(fName, unsList, readBuf, sample_count, utData);	// синхронизация по данным

				shp_map.reset();								//закрываем старый

				readBuf = NULL;

				DeleteMoveFiles(fName);

				shp_map = boost::make_shared <MappingOpener <float> > (fName, &readBuf, 0);

				sample_count = shp_map->GetSize()  / sizeof(float);

				ReWriteFile(fName, unsList, readBuf, sample_count, utTime);	// синхронизация по времени

				shp_map.reset();								//закрываем старый

				DeleteMoveFiles(fName);
			}

			shp_map.reset();
		});
	}

private:



	void WMAligner::CalcUnsynchro(std::list <Unsynchro> &unsList, long lAdress, long oldAdress, long lRTC, long oldRTC, long frq, long sample_count)
	{
		Unsynchro u_elem_data;
		Unsynchro u_elem_time;

		long Adr_corrector = 0;		//  нужен на случай если пропущена ватермарка, но данных столько сколько нужно

		if(lAdress - oldAdress > frq)
		{
			u_elem_data.sync_type = stCut;
			u_elem_data.count = lAdress - oldAdress - frq;
			u_elem_data.from = oldAdress + frq;
			u_elem_data.unsync_type = utData;
			Adr_corrector = -(long)(u_elem_data.count);
		}
		else 
		if(lAdress - oldAdress < frq)
		{
			u_elem_data.sync_type = stInsert;
			u_elem_data.count = frq - (lAdress - oldAdress);
			u_elem_data.from = lAdress;
			u_elem_data.unsync_type = utData;
			Adr_corrector = (long)u_elem_data.count;
		}


		if((lRTC - oldRTC) > (lAdress - oldAdress + Adr_corrector) / frq)
		{
			u_elem_time.sync_type = stInsert;
			u_elem_time.from = lAdress;
			u_elem_time.count = (LONGLONG)((LONGLONG)(lRTC - oldRTC - 1) * (LONGLONG)frq);
			u_elem_time.unsync_type = utTime;
		}
		else
		if ((lRTC - oldRTC) < (lAdress - oldAdress + Adr_corrector) / frq)
		{
			u_elem_time.sync_type = stCut;
			u_elem_time.count = (LONGLONG)((LONGLONG)abs(lRTC - oldRTC - 1) * (LONGLONG)frq);
			u_elem_time.from = lAdress - u_elem_time.count;
			u_elem_time.unsync_type = utTime;
		}

		if(u_elem_data.count && u_elem_data.from && u_elem_time.count && u_elem_time.from)	// если рассинхронизация и по данным и по времени
		{
			GlueRanges(u_elem_data, u_elem_time);

			u_elem_data.sync_type = stNone;
		}


		if(u_elem_data.sync_type != stNone)
		{
			CheckOverload(u_elem_data, sample_count, frq);
			unsList.push_back(u_elem_data);
		}

			
		if(u_elem_time.sync_type != stNone)
		{
			CheckOverload(u_elem_time, sample_count, frq);
			unsList.push_back(u_elem_time);
		}
	}



	void WMAligner::CheckOverload(Unsynchro &elem, long sample_count, long frq)
	{
		if(elem.from < 0)
		{
			elem.count += elem.from;
			elem.from = 0;

			if(elem.count < 0)
				elem.count = 0;
		}
			

		if(elem.sync_type == stCut && elem.from + elem.count > sample_count)
			elem.count = sample_count - elem.from;

		if(elem.count  > frq * FIVEHOURS)
			elem.count = frq * FIVEHOURS;
	}



	void WMAligner::ResolveCollision(std::list <Unsynchro> &unsList)
	{
		for(auto i_it = unsList.begin(), end_i = unsList.end(); i_it != end_i; ++i_it)
		{
			for(auto j_it = unsList.begin(), end_j = unsList.end(); j_it != end_j; ++j_it)
			{
				if(i_it == j_it) continue;	// не сравнивать с самим собой.

				if(i_it->from >= j_it->from &&	//если область i-того элемента входит в область j-того
					(
					(i_it->sync_type == stInsert	&& i_it->from				<= j_it->from + j_it->count) || 
					(i_it->sync_type == stCut		&& i_it->from + i_it->count <= j_it->from + j_it->count)
					) 
					)	
				{
					GlueRanges(*i_it, *j_it);

					i_it->sync_type = stNone;			
				}
			}
		}
	}



	void WMAligner::ReWriteFile(const CString &fName, const std::list <Unsynchro> &unsList, float *readBuf, long sample_count, const unsynchro_type uns_type)
	{
		HANDLE hOutput = INVALID_HANDLE_VALUE;

		hOutput = CreateFile( fName + NEWFILEPREFIX, GENERIC_WRITE, FILE_SHARE_READ, 0, CREATE_ALWAYS, 0, NULL);

		if(hOutput != INVALID_HANDLE_VALUE)
		{
			WriteCorrect(hOutput, unsList, readBuf, sample_count, uns_type);

			CloseHandle(hOutput);
		}
	}



	void WMAligner::WriteCorrect(HANDLE hOutput, const std::list <Unsynchro> &unsList, float *readBuf, long sample_count, const unsynchro_type uns_type)
	{
		DWORD counter = 0;
		DWORD written = 0;
		BOOL result = 0;

		float notANumber =(float)( *( double* )nan_);

		for(auto us = unsList.begin(), endit = unsList.end(); us != endit; ++us)
		{

			if(uns_type != us->unsync_type)	// если не подходит тип рассинхронизации
				continue;

			if(sample_count < counter + (us->from - counter))	// если по каким то причинам фрагмент выходит за пределы исходного файла.
				continue;


			if(us->sync_type == stInsert)
			{
				result = WriteFile(hOutput, readBuf + counter,  ((DWORD)us->from - counter) * sizeof(float), &written, 0);

				float *nan_buf = NULL;

				//исправлена из-за исправления ошибки найденной 2014.04.17 pvs-studio
//				nan_buf = new float[us->count];

//				if(!nan_buf)
//					return;
				
				try
				{
					nan_buf = new float[(UINT)us->count];		
				}
				catch (CMemoryException*)
				{
					return;		
				}

				memset(nan_buf, *(int*)&notANumber, (size_t)us->count);

				result = WriteFile(hOutput, nan_buf, (DWORD)us->count * sizeof(float), &written, 0);

				delete []nan_buf;

				counter = (DWORD)us->from;
			}


			if(us->sync_type == stCut)
			{
				if((us->from - counter - 1) > 0)
				{
				//	result = WriteFile(hOutput, readBuf + counter,  (us->from - counter - 1) * sizeof(float), &written, 0) ;	// вместо одного отсчета втыкаем НаН, чтоб знать что здесь были не корректные данные.
				//	result = WriteFile(hOutput, &nan_, sizeof(float), &written, 0);
					WriteFile(hOutput, readBuf + counter,  ((DWORD)us->from - counter - 1) * sizeof(float), &written, 0) ;	// вместо одного отсчета втыкаем НаН, чтоб знать что здесь были не корректные данные.
					WriteFile(hOutput, &nan_, sizeof(float), &written, 0);
				}				

				counter = (DWORD)(us->from + us->count);
			}
				
		}

		if(counter < (DWORD)sample_count)
			result = WriteFile(hOutput, readBuf + counter,  (sample_count - counter) * sizeof(float), &written, 0);
	}



	void WMAligner::GlueRanges(Unsynchro &uns_slave, Unsynchro &uns_master)
	{
		if(uns_slave.sync_type == stInsert && uns_master.sync_type == stInsert)
			uns_master.count += uns_slave.count;
		else
			if(uns_slave.sync_type == stInsert && uns_master.sync_type == stCut)
			{
				uns_master.from += uns_slave.count;
				uns_master.count -= uns_slave.count;
			}
			else
				if(uns_slave.sync_type == stCut && uns_master.sync_type == stInsert)
					uns_master.count -= uns_slave.count;
				else
					if(uns_slave.sync_type == stCut && uns_master.sync_type == stCut)
					{
						uns_master.from -= uns_slave.count;
						uns_master.count += uns_slave.count;
					}
	}



	void DeleteMoveFiles(CString fName)
	{
		DeleteFile(fName);								// удаляем старый
		MoveFile(fName + NEWFILEPREFIX, fName);			// переименовываем новый
	}

};




#endif	//wmaligner_h