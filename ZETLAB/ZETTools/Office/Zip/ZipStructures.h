#pragma once
#pragma pack(push, 1)		// выравнивание структур по 1 байту
#include "zlib.h"

typedef struct _LocalFileHeader
{
	unsigned long signature;			//!< 4 байта: сигнатура ZIP-Файла (0x04034b50)
	unsigned short extract_version;		//!< 2 байта: версия файла
	unsigned short bit_flag;			//!< 2 байта: битовый флаг
	unsigned short compression_method;	//!< 2 байта: метод сжатия
	unsigned short modification_time;	//!< 2 байта: время последней модификации файла
	unsigned short modification_date;	//!< 2 байта: дата последней модификации файла
	unsigned long crc_32;				//!< 4 байта: контрольная сумма
	unsigned long compressed_size;		//!< 4 байта: размер сжатого файла
	unsigned long uncompressed_size;	//!< 4 байта: размер исходного файла
	unsigned short file_name_length;	//!< 2 байта: длина названия файла
	unsigned short extra_field_length;	//!< 2 байта: длина дополнительного поля
	//! общий размер: 30 байт
} LocalFileHeader, *pLocalFileHeader;

typedef struct _DataDescriptor
{
	unsigned long signature;			//!< 4 байта: 
	unsigned long crc_32;				//!< 4 байта: 
	unsigned long compressed_size;		//!< 4 байта: 
	unsigned long uncompressed_size;	//!< 4 байта: 
	//! общий размер 16 байт
} DataDescriptor, *pDataDescriptor;

typedef struct _CentralDirFileHeader
{
	unsigned long signature;			//!< 4 байта:
	unsigned short version_made;		//!< 2 байта:
	unsigned short version_extract;		//!< 2 байта:
	unsigned short bit_flag;			//!< 2 байта:
	unsigned short compression_method;	//!< 2 байта:
	unsigned short modify_time;			//!< 2 байта:
	unsigned short modify_date;			//!< 2 байта:
	unsigned long crc_32;				//!< 4 байта:
	unsigned long compressed_size;		//!< 4 байта:
	unsigned long uncompressed_size;	//!< 4 байта:
	unsigned short file_name_length;	//!< 2 байта:
	unsigned short extra_field_length;	//!< 2 байта:
	unsigned short file_comment_length;	//!< 2 байта:
	unsigned short disk_number;			//!< 2 байта:
	unsigned short internal_attributes;	//!< 2 байта:
	unsigned long external_attributes;	//!< 4 байта:
	unsigned long relative_offset;		//!< 4 байта: позиция соответствующей структуры LocalFileHeader относительно начала файла
	//! общий размер 46 байт
} CentralDirFileHeader, *pCentralDirFileHeader;

typedef struct _CentralDirEndRecord
{
	unsigned long signature;			//!< 4 байта: m_End.cdRecord.signature; V607 Ownerless expression 'm_End.cdRecord.signature'.
	unsigned short disk_number;			//!< 2 байта:
	unsigned short disk_start;			//!< 2 байта:
	unsigned short records_number;		//!< 2 байта:
	unsigned short total_number;		//!< 2 байта:
	unsigned long central_dir_size;		//!< 4 байта:
	unsigned long central_dir_offset;	//!< 4 байта:
	unsigned short comment_length;		//!< 2 байта:
	_CentralDirEndRecord()
		: signature(0)
		, disk_number(0)
		, disk_start(0)
		, records_number(0)
		, total_number(0)
		, central_dir_size(0)
		, central_dir_offset(0)
		, comment_length(0)
	{}

	//! общий размер 20 байт
} CentralDirEndRecord, *pCentralDirEndRecord;

typedef struct _LocalFile
{
	LocalFileHeader zhead;
	DataDescriptor zdesc;
	char* pFileName;
	char* pExtraField;
	Byte* pCompData;
	Byte* pUncmData;
} LocalFile, *pLocalFile;

typedef struct _CentralDir
{
	CentralDirFileHeader cdHead;
	char* pFileName;
	char* pExtraField;
	char* pFileComment;
} CentralDir, *pCentralDir;

typedef struct _EndRecord
{
	CentralDirEndRecord cdRecord;
	char* pComment;
	_EndRecord()
		: pComment(nullptr)
	{}
} EndRecord, *pEndRecord;

#pragma pack(pop)

