//----------------------------------------------------------------------------------------------------------------------
// Функции сжатия и распаковки данных
#pragma once
#include <stdint.h>
//----------------------------------------------------------------------------------------------------------------------
#pragma pack(push, 2)
typedef struct _COMPRESSED
{
	float coef_k;		// коэффициент преобразования
	float damping_h;	// затухание
	float frequency_w0; // собственная частота
	float frequency_w1; // новая собственная частота
	short bit_offset;	// смещение, для получения полного кода
}COMPRESSED, *PCOMPRESSED;
#pragma pack(pop)
//----------------------------------------------------------------------------------------------------------------------
int32_t getDecompressSize(const uint16_t sizeCode, _In_ const uint32_t inSize);
//----------------------------------------------------------------------------------------------------------------------
int32_t decompress(_In_ const COMPRESSED parameters, _In_ const uint32_t val0, _In_ const uint16_t sizeCode,
	_In_ uint8_t* pDataIn, _In_ const uint32_t inSize, _Out_ float* pDataOut, _In_ const uint32_t outSize);
//----------------------------------------------------------------------------------------------------------------------