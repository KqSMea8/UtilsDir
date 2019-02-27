#pragma once

#ifndef mappingopener_h
#define mappingopener_h

#include <Windows.h>


template <typename T>
class MappingOpener
{
	public:

		
		/*MappingOpener::MappingOpener(const CString &fileName, T **workBuf, const long &fileOffset)
		{
			hFile = INVALID_HANDLE_VALUE, hMap = INVALID_HANDLE_VALUE;
			mapBuf = NULL;

			acceptSize = 0;


			hFile = CreateFile( fileName, GENERIC_READ, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL,OPEN_EXISTING, FILE_FLAG_SEQUENTIAL_SCAN, NULL );

			if(hFile != INVALID_HANDLE_VALUE)
			{
				hMap = HANDLE(CreateFileMapping( hFile, NULL, PAGE_READONLY, 0x00000000, 0x00000000, NULL ));
			}

			if(hMap != INVALID_HANDLE_VALUE)
			{
				mapBuf = ( ( CHAR* )( MapViewOfFile(hMap, FILE_MAP_READ, 0, 0, 0 ) ) );
			}

			if(mapBuf)
			{
				char *tPtr = mapBuf;
				*workBuf = (T*)&tPtr[fileOffset];
				acceptSize = GetFileSize(hFile, NULL) - fileOffset;
			}	
		}*/



		MappingOpener::MappingOpener(const CString &fileName, T **workBuf, const long &fileOffset, const long &mapSize = 0)
		{
			hFile = INVALID_HANDLE_VALUE, hMap = INVALID_HANDLE_VALUE;
			mapBuf = NULL;

			acceptSize = 0;

			hFile = CreateFile(fileName, GENERIC_READ, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING, FILE_FLAG_SEQUENTIAL_SCAN, NULL);

			if (hFile != INVALID_HANDLE_VALUE)
			{
				hMap = CreateFileMappingW(hFile, NULL, PAGE_READONLY, 0x00000000, 0x00000000, NULL);

				if (mapSize == 0)
				{
					if ((hMap != NULL) && (hMap != INVALID_HANDLE_VALUE))
					{
						mapBuf = (CHAR*)MapViewOfFile(hMap, FILE_MAP_READ, 0, 0, 0);
						if (mapBuf == NULL)
						{
							DWORD err_code = GetLastError();
							TRACE("%d\n", err_code);
						}
					}

					if (mapBuf)
					{
						char *tPtr = mapBuf;
						*workBuf = (T*)&tPtr[fileOffset];
						acceptSize = GetFileSize(hFile, NULL) - fileOffset;
					}
				}
				else
				{
					long lRealSize;
					SYSTEM_INFO sinf;
					GetSystemInfo(&sinf);
					long long lTemp = fileOffset / sinf.dwAllocationGranularity;
					lTemp *= sinf.dwAllocationGranularity;
					lRealSize = (long)(mapSize + fileOffset - lTemp);
					if (hMap != NULL && hMap != INVALID_HANDLE_VALUE)
					{
						mapBuf = ((CHAR*)(MapViewOfFile(hMap, FILE_MAP_READ, (DWORD)(lTemp >> 32), (DWORD)(lTemp & 0xffffffff), lRealSize)));
						DWORD a = GetLastError();
						a = 0;
					}

					if (mapBuf)
					{
						char *tPtr = mapBuf;
						*workBuf = (T*)&tPtr[fileOffset - lTemp];
						acceptSize = mapSize;
					}
				}
			}
		}



		MappingOpener::~MappingOpener()
		{
			UnmapViewOfFile( mapBuf );

			if(hMap)
				CloseHandle(hMap);

			if(hFile != INVALID_HANDLE_VALUE)
				CloseHandle(hFile);
		}



		long GetSize()
		{
			return acceptSize;
		}

private:


	HANDLE hFile, hMap;
	char *mapBuf;

	long acceptSize;

};


#endif //mappingopener_h

