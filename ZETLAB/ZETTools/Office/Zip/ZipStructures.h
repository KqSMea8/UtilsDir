#pragma once
#pragma pack(push, 1)		// ������������ �������� �� 1 �����
#include "zlib.h"

typedef struct _LocalFileHeader
{
	unsigned long signature;			//!< 4 �����: ��������� ZIP-����� (0x04034b50)
	unsigned short extract_version;		//!< 2 �����: ������ �����
	unsigned short bit_flag;			//!< 2 �����: ������� ����
	unsigned short compression_method;	//!< 2 �����: ����� ������
	unsigned short modification_time;	//!< 2 �����: ����� ��������� ����������� �����
	unsigned short modification_date;	//!< 2 �����: ���� ��������� ����������� �����
	unsigned long crc_32;				//!< 4 �����: ����������� �����
	unsigned long compressed_size;		//!< 4 �����: ������ ������� �����
	unsigned long uncompressed_size;	//!< 4 �����: ������ ��������� �����
	unsigned short file_name_length;	//!< 2 �����: ����� �������� �����
	unsigned short extra_field_length;	//!< 2 �����: ����� ��������������� ����
	//! ����� ������: 30 ����
} LocalFileHeader, *pLocalFileHeader;

typedef struct _DataDescriptor
{
	unsigned long signature;			//!< 4 �����: 
	unsigned long crc_32;				//!< 4 �����: 
	unsigned long compressed_size;		//!< 4 �����: 
	unsigned long uncompressed_size;	//!< 4 �����: 
	//! ����� ������ 16 ����
} DataDescriptor, *pDataDescriptor;

typedef struct _CentralDirFileHeader
{
	unsigned long signature;			//!< 4 �����:
	unsigned short version_made;		//!< 2 �����:
	unsigned short version_extract;		//!< 2 �����:
	unsigned short bit_flag;			//!< 2 �����:
	unsigned short compression_method;	//!< 2 �����:
	unsigned short modify_time;			//!< 2 �����:
	unsigned short modify_date;			//!< 2 �����:
	unsigned long crc_32;				//!< 4 �����:
	unsigned long compressed_size;		//!< 4 �����:
	unsigned long uncompressed_size;	//!< 4 �����:
	unsigned short file_name_length;	//!< 2 �����:
	unsigned short extra_field_length;	//!< 2 �����:
	unsigned short file_comment_length;	//!< 2 �����:
	unsigned short disk_number;			//!< 2 �����:
	unsigned short internal_attributes;	//!< 2 �����:
	unsigned long external_attributes;	//!< 4 �����:
	unsigned long relative_offset;		//!< 4 �����: ������� ��������������� ��������� LocalFileHeader ������������ ������ �����
	//! ����� ������ 46 ����
} CentralDirFileHeader, *pCentralDirFileHeader;

typedef struct _CentralDirEndRecord
{
	unsigned long signature;			//!< 4 �����: m_End.cdRecord.signature; V607 Ownerless expression 'm_End.cdRecord.signature'.
	unsigned short disk_number;			//!< 2 �����:
	unsigned short disk_start;			//!< 2 �����:
	unsigned short records_number;		//!< 2 �����:
	unsigned short total_number;		//!< 2 �����:
	unsigned long central_dir_size;		//!< 4 �����:
	unsigned long central_dir_offset;	//!< 4 �����:
	unsigned short comment_length;		//!< 2 �����:
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

	//! ����� ������ 20 ����
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

