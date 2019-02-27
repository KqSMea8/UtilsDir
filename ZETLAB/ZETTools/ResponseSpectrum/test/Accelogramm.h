//! ������� ��� ������ ����������� �� ������ � ������� ��.
//!	�������� ������� ���������� �� ��������� 
//! https://ru.wikipedia.org/wiki/��������������_�������������_�_�����_(1978_���)
/** ���� ��������� ��������� ��, ������� � ����� ���� ����� ���� ������������ ��� ������������� �������
	��������������� ������� � ������� ��������� ������� ���������� ��������������. 	��������� ���� � 
	����������� ��������� MS DOS ������� ������ ��������, ������������� �� ������������ ���������� 
	�������� 999. � ������ ������, ������� � 11-��� �������, ����������� 15 �������� (�� ���� ������� �� 
	�����). � ������ ������ ��� �������� ������ ����������� ����� ���������� ����� �� ������� ��������. 
	��������� ������ � ���������� ����� ������� (� ������ ������ � ��������� �������) 0.02�, � ��� 
	������������ ��� ���������� ������������: ������ (���������) � ����������� �� � �������� ���������� 
	����� ��������� (� ����� g); ������ � ������������ �������������� � ���������� ������. ������� �����
	�������� ��������� �� ���������� ����� ������������� �������������� ����� �������, ����� � ������ ���
	������� ( � �������� 7�70) ����� ���� �������� ����� ���������� ����������. **/

#pragma once

#include <stdio.h>

long ReadFileAA(const char* filename, float** pAccel, long &size, long &freq)
{
	long acsize = 0;	//	���������� �����
	long filesize = 0;	//	������ �������� �����
	float rm = 0.;		//	�������������� ���������
	float step = 0.;	//	��� �� �������
	float rma = 0.;		//	������� ��������� RMA
	char* buffer = NULL;//	������ �� ���������� ������� �� �����

	// ������ ������ �� ����� � ������
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

	// ������ �������� ��������� ������������
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

	// ����������� ������ � ������ ����� � ��������� �������
	*pAccel = new float[acsize];
	size = acsize;
	readed = 0;
	cpos = cbeg;
	bool bReading = true;
	while (bReading)
	{
		cpos = strstr(cpos, ")");		// ������ ������
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

	// ������������ ������
	for (long i = 0; i < acsize; i++)
		(*pAccel)[i] *= rma * rm / 999.0f;

	// ������� �������������
	freq = (long)(1 / step + 0.5f);

	delete[] buffer;
	return 0;
}