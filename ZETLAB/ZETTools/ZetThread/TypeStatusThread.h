//------------------------------------------------------------------------------
#pragma once
//------------------------------------------------------------------------------
// ����������� ������ ������ ������
enum TypeStatusThread : BYTE
{
	notStart,		// �� �������
	running,		// �������
	working,		// �������� (���-�� ������)
	dataCopy,		// �������� ������
	dataTime,		// �������� � �������� ������
	dataRead,		// ������ ������
	dataProcessing,	// ������������ ������

	waitObject,		// ������� �����-�� �������
	waitSynshroGUI,	// ������� ������� ������������� GUI
	waitMode,		// ������ ����� ������
	sleeping,		// �������
	suspend,		// ����������
	ending,			// �������� ������
	numFileUse,		// ����� ������� NumFileUse

	dataProcessingAver,
};
//------------------------------------------------------------------------------