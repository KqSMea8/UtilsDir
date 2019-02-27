//! Функция для чтения акселограмм из файлов в формате АА.
//!	Описание формата приводится из википедии 
//! https://ru.wikipedia.org/wiki/Акселерограмма_землетрясения_в_Иране_(1978_год)
/** Ниже приведена оцифровка АА, которая в таком виде может быть использована для динамического анализа
	сейсмостойкости объекта с помощью различных методов численного интегрирования. 	Оцифровка дана в 
	специальном текстовом MS DOS формате записи значений, нормированных по максимальной абсолютной 
	величине 999. В каждой строке, начиная с 11-той позиции, размещаются 15 значений (по пять позиций на 
	число). В начале строки для удобства чтения указывается общий порядковый номер ее первого значения. 
	Оцифровка дается с постоянным шагом времени (в данном случае с предельно большим) 0.02с, в ней 
	используется два масштабных коэффициента: первый (множитель) — преобразует ее к исходной физической 
	норме ускорений (в долях g); второй — масштабирует акселерограмму к расчетному уровню. Процесс ввода
	значений оцифровки из текстового файла целесообразно организовывать таким образом, чтобы в первых его
	строках ( в позициях 7—70) могла быть записана любая поясняющая информация. **/

#pragma once

#include <stdio.h>

long ReadFileAA(const char* filename, float** pAccel, long &size, long &freq)
{
	long acsize = 0;	//	количество точек
	long filesize = 0;	//	размер входного файла
	float rm = 0.;		//	масштабирующий множитель
	float step = 0.;	//	шаг по времени
	float rma = 0.;		//	масштаб оцифровки RMA
	char* buffer = NULL;//	буффер со считанными данными из файла

	// читаем данные из файла в буффер
	FILE* fin;
	if (fopen_s(&fin, filename, "r") != 0)
		return -1;

	fseek(fin, 0, SEEK_END);
	filesize = ftell(fin);
	fseek(fin, 0, SEEK_SET);

	buffer = new char[filesize];
	ZeroMemory(buffer, filesize);
	long readed = fread_s(buffer, filesize, sizeof(char), filesize, fin);
	fclose(fin);

	// читаем основные параметры акселограммы
	char* cpos = buffer;
	char* cbeg = buffer;
	cpos = strstr(buffer, "/-");
	for (cbeg = cpos; cbeg > buffer; cbeg--)
		if (cbeg[0] == '\n')
			break;
	acsize = atoi(cbeg + 1);

	cpos = strstr(buffer, "RM=");
	rm = (float)atof(cpos+3);
	for (cbeg = cpos; cbeg < buffer + readed; cbeg++)
		if (cbeg[0] == '\n')
			break;

	cpos = strstr(buffer, "DTA=");
	step = (float)atof(cpos + 4);

	cpos = strstr(buffer, "RMA=");
	rma = (float)atof(cpos + 4);

	// преобразуем данные в массив чисел с плавающей запятой
	*pAccel = new float[acsize];
	size = acsize;
	readed = 0;
	cpos = cbeg;
	bool bReading = true;
	while (bReading)
	{
		cpos = strstr(cpos, ")");		// начало данных
		cpos++;
		for (long i = 0; i < 15; i++)
		{
			(*pAccel)[readed++] = (float)atof(cpos);
			cpos += 4;
			if (readed == acsize)
			{
				bReading = false;
				break;
			}
		}
	}

	// масштабируем данные
	for (long i = 0; i < acsize; i++)
		(*pAccel)[i] *= rma * rm / 999.0f;

	// частота дискретизации
	freq = (long)(1 / step + 0.5f);

	delete[] buffer;
	return 0;
}