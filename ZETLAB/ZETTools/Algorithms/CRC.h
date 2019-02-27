//------------------------------------------------------------------------------
//					Алгоритмы расчета контрольных сумм CRC
#pragma once
//------------------------------------------------------------------------------
// Расчет функции CRC32
unsigned int CRC32(unsigned int uiCRC, const void *pBuffer, unsigned int uiSize);
//------------------------------------------------------------------------------
// Расчет функции CRC16 (полином A001)
//unsigned short CRC16(unsigned short usCRC, unsigned char ucSymbol);
unsigned short CRC16(unsigned short usCRC, const void *pBuffer, unsigned int uiSize);
//------------------------------------------------------------------------------