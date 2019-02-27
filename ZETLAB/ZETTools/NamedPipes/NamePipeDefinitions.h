#pragma once

namespace NamedPipe
{
	// список команд сервису для изменения путей в реестре
	enum ReestrCommands
	{
		zlsChangeSignalsDirCommand = 0x01,		// Команда сервису изменить директорию записи сигналов
		zlsChangeCompressDirCommand = 0x02,		// Команда сервису изменить директорию записи сжатых сигналов
		zlsChangeResultDirCommand = 0x03,		// Команда сервису изменить директорию расположения файлов результатов
		zlsChangeConfigDirCommand = 0x04,		// Команда сервису изменить директорию расположения файлов конфигураций
		zlsChangeSuffixDirCommand = 0x05,		// Команда сервису изменить добавочную директорию к папке сигналов
		zlsChangeLanguageCommand = 0x06			// Команда сервису изменить язык программ ZETLab
	};
}