#pragma once

namespace NamedPipe
{
	// ������ ������ ������� ��� ��������� ����� � �������
	enum ReestrCommands
	{
		zlsChangeSignalsDirCommand = 0x01,		// ������� ������� �������� ���������� ������ ��������
		zlsChangeCompressDirCommand = 0x02,		// ������� ������� �������� ���������� ������ ������ ��������
		zlsChangeResultDirCommand = 0x03,		// ������� ������� �������� ���������� ������������ ������ �����������
		zlsChangeConfigDirCommand = 0x04,		// ������� ������� �������� ���������� ������������ ������ ������������
		zlsChangeSuffixDirCommand = 0x05,		// ������� ������� �������� ���������� ���������� � ����� ��������
		zlsChangeLanguageCommand = 0x06			// ������� ������� �������� ���� �������� ZETLab
	};
}