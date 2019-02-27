//----------------------------------------------------------------------------------------------------------------------
// ������� ������ � ���������� ������
#pragma once
#include <stdint.h>
//----------------------------------------------------------------------------------------------------------------------
#pragma pack(push, 2)
typedef struct _COMPRESSED
{
	float coef_k;		// ����������� ��������������
	float damping_h;	// ���������
	float frequency_w0; // ����������� �������
	float frequency_w1; // ����� ����������� �������
	short bit_offset;	// ��������, ��� ��������� ������� ����
}COMPRESSED, *PCOMPRESSED;
#pragma pack(pop)
//----------------------------------------------------------------------------------------------------------------------
int32_t getDecompressSize(const uint16_t sizeCode, _In_ const uint32_t inSize);
//----------------------------------------------------------------------------------------------------------------------
int32_t decompress(_In_ const COMPRESSED parameters, _In_ const uint32_t val0, _In_ const uint16_t sizeCode,
	_In_ uint8_t* pDataIn, _In_ const uint32_t inSize, _Out_ float* pDataOut, _In_ const uint32_t outSize);
//----------------------------------------------------------------------------------------------------------------------