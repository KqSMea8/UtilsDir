//------------------------------------------------------------------------------
//					��������� ������� ����������� ���� CRC
#pragma once
//------------------------------------------------------------------------------
// ������ ������� CRC32
unsigned int CRC32(unsigned int uiCRC, const void *pBuffer, unsigned int uiSize);
//------------------------------------------------------------------------------
// ������ ������� CRC16 (������� A001)
//unsigned short CRC16(unsigned short usCRC, unsigned char ucSymbol);
unsigned short CRC16(unsigned short usCRC, const void *pBuffer, unsigned int uiSize);
//------------------------------------------------------------------------------