#include "stdafx.h"
#include "ZipFile.h"
#include "windows.h"

#pragma comment (lib, "zdll.lib")

CZipFile::CZipFile()
	: m_iBufferSize(NULL)
	, m_pBuffer(nullptr)
{

}

CZipFile::~CZipFile()
{
	Clear();
	CleanBuffer();
}

CZipFile::CZipFile( tstring filename )
{
	m_iBufferSize = NULL;
	m_pBuffer = NULL;
}

int CZipFile::ReadZip( tstring filename )
{
	m_sFileName = filename;

	int ret = ReadBuffer();
	if ( ret != ZIP_OK )
		return ret;

	ReadZipFromFront();

	CleanBuffer();

	UncompressData();

	return ZIP_OK;
}

int CZipFile::ReadZipFromFront()
{
	unsigned int pos = 0;
	while ( pos < m_iBufferSize )
	{
		unsigned long  signature = *(long*)(&m_pBuffer[pos]);
		int retval = ZIP_ERROR_NO_FILE_HEADER;
		if ( signature == 0x04034B50 )
			retval = ReadLocalFile(pos);
		if ( signature == 0x02014B50 )
			retval = ReadCentralDirectory(pos);
		if ( signature == 0x06054B50 )
			retval = ReadEndRecord(pos);
		if ( retval != ZIP_OK )
			pos++;
	}

	return ZIP_OK;
}

int CZipFile::WriteZip( tstring filename )
{
	m_sFileName = filename;

	CompressData();

	WriteZipFromFront();

	int ret = WriteBuffer();
	CleanBuffer();
	if ( ret != ZIP_OK )
		return ret;

	return ZIP_OK;
}

int CZipFile::WriteZipFromFront()
{
	unsigned int pos = 0;

	CleanBuffer();
	m_iBufferSize = CalculateTotalCompressSize();
	m_pBuffer = new Byte[m_iBufferSize];

	std::list<LocalFile>::iterator itFile;
	for (itFile = m_lFiles.begin(); itFile != m_lFiles.end(); ++itFile)
	{
		memcpy_s(&m_pBuffer[pos], sizeof(LocalFileHeader), &itFile->zhead, sizeof(LocalFileHeader));
		pos += sizeof(LocalFileHeader);
		memcpy_s(&m_pBuffer[pos], itFile->zhead.file_name_length, itFile->pFileName, itFile->zhead.file_name_length);
		pos += itFile->zhead.file_name_length;
		memcpy_s(&m_pBuffer[pos], itFile->zhead.extra_field_length, itFile->pExtraField, itFile->zhead.extra_field_length);
		pos += itFile->zhead.extra_field_length;
		memcpy_s(&m_pBuffer[pos], itFile->zdesc.compressed_size, itFile->pCompData, itFile->zdesc.compressed_size);
		pos += itFile->zdesc.compressed_size;
		if ( itFile->zdesc.signature == 0x08074B50 )
		{
			memcpy_s(&m_pBuffer[pos], sizeof(DataDescriptor), &itFile->zdesc, sizeof(DataDescriptor));
			pos += sizeof(DataDescriptor);
		}
	}

	std::list<CentralDir>::iterator itDir;
	for (itDir = m_lDirs.begin(); itDir != m_lDirs.end(); ++itDir)
	{
		memcpy_s(&m_pBuffer[pos], sizeof(CentralDirFileHeader), &itDir->cdHead, sizeof(CentralDirFileHeader));
		pos += sizeof(CentralDirFileHeader);
		memcpy_s(&m_pBuffer[pos], itDir->cdHead.file_name_length, itDir->pFileName, itDir->cdHead.file_name_length);
		pos += itDir->cdHead.file_name_length;
		memcpy_s(&m_pBuffer[pos], itDir->cdHead.extra_field_length, itDir->pExtraField, itDir->cdHead.extra_field_length);
		pos += itDir->cdHead.extra_field_length;
		memcpy_s(&m_pBuffer[pos], itDir->cdHead.file_comment_length, itDir->pFileComment, itDir->cdHead.file_comment_length);
		pos += itDir->cdHead.file_comment_length;
	}

	memcpy_s(&m_pBuffer[pos], sizeof(CentralDirEndRecord), &m_End.cdRecord, sizeof(CentralDirEndRecord));
	pos += sizeof(CentralDirEndRecord);
	memcpy_s(&m_pBuffer[pos], m_End.cdRecord.comment_length, m_End.pComment, m_End.cdRecord.comment_length);
	pos += m_End.cdRecord.comment_length;

	return pos;
}

void CZipFile::Clear()
{
	std::list<LocalFile>::iterator itFile;
	for (itFile = m_lFiles.begin(); itFile != m_lFiles.end(); ++itFile)
	{
		delete[] itFile->pFileName;
		itFile->pFileName = nullptr;
		delete[] itFile->pExtraField;
		itFile->pExtraField = nullptr;
		delete[] itFile->pCompData;
		itFile->pCompData = nullptr;
		delete[] itFile->pUncmData;
		itFile->pUncmData = nullptr;
		itFile->zhead.signature = 0;
		itFile->zdesc.signature = 0;
	}
	if ( !m_lFiles.empty() )
		m_lFiles.clear();

	std::list<CentralDir>::iterator itDir;
	for (itDir = m_lDirs.begin(); itDir != m_lDirs.end(); ++itDir)
	{
		delete[] itDir->pFileName;
		itDir->pFileName = nullptr;
		delete[] itDir->pExtraField;
		itDir->pExtraField = nullptr;
		delete[] itDir->pFileComment;
		itDir->pFileComment = nullptr;
		itDir->cdHead.signature = 0;
	}
	if ( !m_lDirs.empty() )
		m_lDirs.clear();

	delete[] m_End.pComment;
	m_End.pComment = nullptr;
}

int CZipFile::ReadBuffer()
{
	if ( m_sFileName.length() == 0 )
		return ZIP_ERROR_BAD_FILE_NAME;

	FILE* fid(nullptr);

	errno_t err = _wfopen_s(&fid, m_sFileName.data(), L"rb");
	if ( err != 0 )
	{
		return err;
	}

	fseek(fid, 0, SEEK_END);
	unsigned int size = ftell(fid);
	fseek(fid, 0, SEEK_SET);

	CleanBuffer();

	m_pBuffer = new Byte[size];
	m_iBufferSize = fread(m_pBuffer, 1, size, fid);

	fclose(fid);
	fid = NULL;

	if ( m_iBufferSize != size )
	{
	}

	return ZIP_OK;
}

int CZipFile::WriteBuffer()
{
	if ( m_sFileName.length() == 0 )
		return ZIP_ERROR_BAD_FILE_NAME;

	FILE* fid(nullptr);

	errno_t err = _wfopen_s(&fid, m_sFileName.data(), L"wb");
	if ((err == 0) && (fid != nullptr))
	{
		unsigned int size = fwrite(m_pBuffer, 1, m_iBufferSize, fid);
		if (size != m_iBufferSize)
		{
		}
		fclose(fid);
		fid = nullptr;
		return ZIP_OK;
	}
	else
		return err;

}

void CZipFile::CleanBuffer()
{
	if ( m_pBuffer != NULL )
	{
		delete[] m_pBuffer;
		m_iBufferSize = 0;
		m_pBuffer = nullptr;
	}
}

int CZipFile::ReadLocalFile(unsigned int &position)
{
	unsigned long signature = *(long*)(&m_pBuffer[position]);
	if ( signature != 0x04034B50 )
		return ZIP_ERROR_BAD_FILE_HEADER_FORMAT;

	LocalFile newFile;
	int newPos;

	//! читаем заголовок сжатого файла
	memcpy_s(&newFile.zhead, sizeof(LocalFileHeader), &m_pBuffer[position], sizeof(LocalFileHeader));

	//! читаем имя файла
	newFile.pFileName = new char[newFile.zhead.file_name_length + 1];
	newPos = position + sizeof(LocalFileHeader);
	memcpy_s(newFile.pFileName, newFile.zhead.file_name_length, &m_pBuffer[newPos], newFile.zhead.file_name_length);
	newFile.pFileName[newFile.zhead.file_name_length] = '\0';

	//! читаем поле дополнительных данных
	newFile.pExtraField = new char[newFile.zhead.extra_field_length + 1];
	newPos += newFile.zhead.file_name_length;
	memcpy_s(newFile.pExtraField, newFile.zhead.extra_field_length, &m_pBuffer[newPos], newFile.zhead.extra_field_length);
	newFile.pExtraField[newFile.zhead.extra_field_length] = '\0';

	// TODO : написать бы более быстрый поиск
	//! ищем сигнатуру окончания сжатых данных
	newPos += newFile.zhead.extra_field_length;
	unsigned int i = 0;
	if ( newFile.zhead.compressed_size == 0 )
	{
		for ( i = newPos; i < m_iBufferSize; i++ )
		{
			signature = *(long*)(&m_pBuffer[i]);
			if ( signature == 0x08074B50 )
				break;
		}
	}
	else
	{
		i = newPos + newFile.zhead.compressed_size;
	}

	//! читаем дескриптор сжатого файла
	memcpy_s(&newFile.zdesc, sizeof(DataDescriptor), &m_pBuffer[i], sizeof(DataDescriptor));
	//! если дескриптор имеет правильный идентификатор, то размер сжатого файла берём из него
	if ( newFile.zdesc.signature == 0x08074B50 )
	{
		//! копируем сжатые данные 
		newFile.pCompData = new Byte[newFile.zdesc.compressed_size];
		memcpy_s(newFile.pCompData, newFile.zdesc.compressed_size, &m_pBuffer[newPos], newFile.zdesc.compressed_size);
	}
	//! иначе используем заголовок файла
	else
	{
		if ( newFile.zhead.compressed_size == 0 )
			return ZIP_ERROR_BAD_FILE_HEADER_FORMAT;
		newFile.zdesc.compressed_size = newFile.zhead.compressed_size;
		newFile.zdesc.uncompressed_size = newFile.zhead.uncompressed_size;
		newFile.zdesc.crc_32 = newFile.zhead.crc_32;
		//! копируем сжатые данные 
		newFile.pCompData = new Byte[newFile.zhead.compressed_size];
		memcpy_s(newFile.pCompData, newFile.zhead.compressed_size, &m_pBuffer[newPos], newFile.zhead.compressed_size);
	}

	newFile.pUncmData = NULL;

	m_lFiles.push_back(newFile);

	if ( newFile.zdesc.signature == 0x08074B50 )
		position = newPos + newFile.zdesc.compressed_size + sizeof(DataDescriptor);
	else
		position = newPos + newFile.zhead.compressed_size;

	return ZIP_OK;
}

int CZipFile::ReadCentralDirectory(unsigned int &position)
{
	unsigned long signature = *(long*)(&m_pBuffer[position]);
	if ( signature != 0x02014B50 )
		return ZIP_ERROR_BAD_CENTRAL_DIR_FORMAT;

	CentralDir newDir;
	int newPos;

	//! читаем заголовок центральной директории
	memcpy_s(&newDir.cdHead, sizeof(CentralDirFileHeader), &m_pBuffer[position], sizeof(CentralDirFileHeader));

	//! читаем имя файла
	newDir.pFileName = new char[newDir.cdHead.file_name_length + 1];
	newPos = position + sizeof(CentralDirFileHeader);
	memcpy_s(newDir.pFileName, newDir.cdHead.file_name_length, &m_pBuffer[newPos], newDir.cdHead.file_name_length);
	newDir.pFileName[newDir.cdHead.file_name_length] = '\0';

	//! читаем дополнительное поле
	newDir.pExtraField = new char[newDir.cdHead.extra_field_length + 1];
	newPos += newDir.cdHead.file_name_length;
	memcpy_s(newDir.pExtraField, newDir.cdHead.extra_field_length, &m_pBuffer[newPos], newDir.cdHead.extra_field_length);
	newDir.pExtraField[newDir.cdHead.extra_field_length] = '\0';

	//! читаем комментарии к файлу
	newDir.pFileComment = new char[newDir.cdHead.file_comment_length + 1];
	newPos += newDir.cdHead.extra_field_length;
	memcpy_s(newDir.pFileComment, newDir.cdHead.file_comment_length, &m_pBuffer[newPos], newDir.cdHead.file_comment_length);
	newDir.pFileComment[newDir.cdHead.file_comment_length] = '\0';

	m_lDirs.push_back(newDir);
	newPos += newDir.cdHead.file_comment_length;
	position = newPos;

	return ZIP_OK;
}

int CZipFile::ReadEndRecord(unsigned int &position)
{
	unsigned long signature = *(long*)(&m_pBuffer[position]);
	if ( signature != 0x06054B50 )
		return ZIP_ERROR_BAD_END_RECORD_FORMAT;

	EndRecord endRec;

	// читаем окончание архивного zip-файла
	memcpy_s(&endRec, sizeof(CentralDirEndRecord), &m_pBuffer[position], sizeof(CentralDirEndRecord));

	// читаем комментарий в окончании файла
	endRec.pComment = new char[endRec.cdRecord.comment_length + 1];
	int newPos = position + sizeof(CentralDirEndRecord);
	memcpy_s(endRec.pComment, endRec.cdRecord.comment_length, &m_pBuffer[newPos], endRec.cdRecord.comment_length);
	endRec.pComment[endRec.cdRecord.comment_length] = '\0';

	m_End = endRec;
	position = newPos;

	return ZIP_OK;
}

int CZipFile::CalculateTotalCompressSize()
{
	unsigned int size = 0;

	std::list<LocalFile>::iterator itFile;
	for (itFile = m_lFiles.begin(); itFile != m_lFiles.end(); ++itFile)
	{
		size += sizeof(LocalFileHeader);
		size += itFile->zhead.file_name_length;
		size += itFile->zhead.extra_field_length;
		size += itFile->zdesc.compressed_size;
		if ( itFile->zdesc.signature == 0x08074B50 )
			size += sizeof(DataDescriptor);
	}

	std::list<CentralDir>::iterator itDir;
	for (itDir = m_lDirs.begin(); itDir != m_lDirs.end(); ++itDir)
	{
		size += sizeof(CentralDirFileHeader);
		size += itDir->cdHead.file_name_length;
		size += itDir->cdHead.extra_field_length;
		size += itDir->cdHead.file_comment_length;
	}

	size += sizeof(CentralDirEndRecord);
	size += m_End.cdRecord.comment_length;

	return size;
}

int CZipFile::CompressData()
{
	std::list<LocalFile>::iterator itFile;
	for (itFile = m_lFiles.begin(); itFile != m_lFiles.end(); ++itFile)
	{
		if ( itFile->pCompData != NULL )
		{ 
			delete[] itFile->pCompData;
			itFile->pCompData = nullptr;
		}

		// выделяем для сжатых данных столько же памяти, сколько и для несжатых, но в 2 раза больше
		itFile->pCompData = new Byte[itFile->zdesc.uncompressed_size * 2];

		if (itFile->zhead.compression_method > 0)
		{
			z_stream zstr;
			zstr.zalloc = Z_NULL;
			zstr.zfree = Z_NULL;
			zstr.opaque = NULL;
			int ret = deflateInit2(&zstr, Z_DEFAULT_COMPRESSION, Z_DEFLATED, -MAX_WBITS, 9, Z_DEFAULT_STRATEGY);
			if (ret != Z_OK)
			{
			}

			zstr.avail_in = itFile->zdesc.uncompressed_size;
			zstr.next_in = itFile->pUncmData;
			zstr.avail_out = itFile->zdesc.uncompressed_size;
			zstr.next_out = itFile->pCompData;
			ret = deflate(&zstr, Z_FINISH);
			if (ret != Z_OK)
			{
			}

			itFile->zdesc.compressed_size = zstr.total_out;
			itFile->zdesc.crc_32 = crc32(0L, itFile->pUncmData, itFile->zdesc.uncompressed_size);
			// если сигнатура дескриптора не соответствует правильной, то дублируем значения в заголовок файла
			if (itFile->zdesc.signature != 0x08074B50)
			{
				itFile->zhead.compressed_size = itFile->zdesc.compressed_size;
				itFile->zhead.crc_32 = itFile->zdesc.crc_32;
			}

			deflateEnd(&zstr);
		}
		else
		{
			memcpy_s(itFile->pCompData, itFile->zdesc.uncompressed_size, itFile->pUncmData, itFile->zdesc.uncompressed_size);
			itFile->zdesc.compressed_size = itFile->zdesc.uncompressed_size;
			itFile->zdesc.crc_32 = crc32(0L, itFile->pUncmData, itFile->zdesc.uncompressed_size);
			// если сигнатура дескриптора не соответствует правильной, то дублируем значения в заголовок файла
			if (itFile->zdesc.signature != 0x08074B50)
			{
				itFile->zhead.compressed_size = itFile->zdesc.compressed_size;
				itFile->zhead.crc_32 = itFile->zdesc.crc_32;
			}
		}
	}

	// пересчитаем позиции файлов в архиве
	std::list<CentralDir>::iterator itDir;
	unsigned long pos = 0;
	unsigned long dir = 0;
	for (itFile = m_lFiles.begin(), itDir = m_lDirs.begin(); itFile != m_lFiles.end(), itDir != m_lDirs.end(); ++itFile, ++itDir)
	{
		itDir->cdHead.relative_offset = pos;
		itDir->cdHead.compressed_size = itFile->zdesc.compressed_size;
		itDir->cdHead.uncompressed_size = itFile->zdesc.uncompressed_size;
		itDir->cdHead.crc_32 = itFile->zdesc.crc_32;

		pos += sizeof(LocalFileHeader);
		pos += itFile->zhead.file_name_length;
		pos += itFile->zhead.extra_field_length;
		pos += itFile->zdesc.compressed_size;
		if ( itFile->zdesc.signature == 0x08074B50 )
			pos += sizeof(DataDescriptor);

		dir += sizeof(CentralDirFileHeader);
		dir += itDir->cdHead.file_name_length;
		dir += itDir->cdHead.extra_field_length;
		dir += itDir->cdHead.file_comment_length;
	}

	m_End.cdRecord.central_dir_size = dir;
	m_End.cdRecord.central_dir_offset = pos;

	return ZIP_OK;
}

int CZipFile::UncompressData()
{
	std::list<LocalFile>::iterator itFile;
	for (itFile = m_lFiles.begin(); itFile != m_lFiles.end(); ++itFile)
	{
		if ( itFile->pUncmData != NULL )
		{ 
			delete[] itFile->pUncmData;
			itFile->pUncmData = nullptr;
		}

		itFile->pUncmData = new Byte[itFile->zdesc.uncompressed_size];

		z_stream zstr;
		zstr.zalloc = NULL;
		zstr.zfree = NULL;
		zstr.opaque = NULL;
		zstr.avail_in = 0;
		zstr.next_in = NULL;
		int ret = inflateInit2(&zstr, -MAX_WBITS);
		if ( ret != Z_OK )
		{
		}
		
		zstr.avail_in = itFile->zdesc.compressed_size;
		zstr.next_in = itFile->pCompData;
		zstr.avail_out = itFile->zdesc.uncompressed_size;
		zstr.next_out = itFile->pUncmData;
		ret = inflate(&zstr, Z_NO_FLUSH);
		if ( ret != Z_OK )
		{
		}

		inflateEnd(&zstr);
	}

	return ZIP_OK;
}

/*
You have this error:

zlib.error : Error - 3 while decompressing : incorrect header check
Which is most likely because you are trying to check headers that are not there,
e.g.your data follows RFC 1951 (deflate compressed format)
rather than RFC 1950 (zlib compressed format)
or RFC 1952 (gzip compressed format).

choosing windowBits
But zlib can decompress all those formats :

to(de - )compress deflate format, use wbits = -zlib.MAX_WBITS
to(de - )compress zlib format, use wbits = zlib.MAX_WBITS
to(de - )compress gzip format, use wbits = zlib.MAX_WBITS | 16

See documentation in http://www.zlib.net/manual.html#Advanced (section inflateInit2)

RFC 1951 это описание deflate как метода сжатия данных (алгоритма и структуры), 
а RFC 1950 это лишь кратенькое описание zlib как формата хранения\записи данных, 
сжатых тем же deflate, с добавлением 4-байтного заголовка в начале и 4-й байтной 
чексуммы adler32 в конце deflate-данных.
Соотв-но дельфийская ZLib юзает тот же deflate-алгоритм и получаемые данные отличаются 
от gzdeflate только наличием 4-байтного заголовка и чексуммы - ес-но при условии 
использования одинаковых параметров сжатия

PS: Кстати в оригинальной сишной ZLib есть возможность отключения записи заголовка 
и чексуммы, если вместо deflateInit_ юзать продвинутую deflateInit2 с параметром 
windowBits = -MAX_WBITS (отрицательное значение)
*/