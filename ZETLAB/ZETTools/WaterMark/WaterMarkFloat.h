#ifndef _WATERMARKFLOAT 
#define _WATERMARKFLOAT

#define STARTBIT		1
#define STOPBIT			1
#define SIZEOFQUALITY	2
#define SIZEOFTIMELABEL	32
#define SIZEOFCRC		8
//������ �������:		| ��������� ��� |  ����� �������   |       �������� ������        |  ����������� �����  | �������� ���  
#define SIZEOFWMF		(STARTBIT		+ SIZEOFTIMELABEL +		4 * SIZEOFQUALITY  +			SIZEOFCRC		 + STOPBIT)
#define CRC_START_VAL	(char)0xE3

// ������� WMF_PutWatermark(...) �������� ������ WATERMARK � ������� ���� ������.
// ���������:
//		pBuffer -		����� ������ ������;
//		lSize -			������ ������ ������;
//		lRTC -			������������ ����� � �������� �� 1970 ���� (����������� �� �������������) ������������ ���� �� 2038 ����
//		ucQuality -		��������:
//								0, 1 ���� - �������� �������������, �������� �� 0 �� 3: 
//										0 - ������,
//										1 - �����,
//										2 - ������,
//										3 - �������;
//								2, 3 ���� - �������� �������, �������� �� 0 �� 3: 
//										0 - ������,
//										1 - ���� �����, 
//										2 - ���� �����,
//										3 - � ���������;
//								4, 5 ���� - �������� ��������� ������, �������� �� 0 �� 3:
//										0 - ������,
//										1 - �����,
//										2 - ������,
//										3 - �������;
//								6, 7 ���� - ������;
//		lStartWatermark - ���, ������� � �������� ���������� ��������� Watermark
// ���� ������:
//		 0 - ������� ��������� �������;
//		-1 - ������ ������ ������ ������� ������ (������ �� WATERMARK �� �����������);
//		-2 - ����� �������� ����� ������������� �������� (��������������, ������ �� WATERMARK �����������).
long WMF_PutWatermark(float* pBuffer, long lSize, long lRTC, unsigned char ucQuality, long lStartWatermark);
// ������� WMF_ClearBit(...) �������� ������� ��� ������
// ���������:
//		buffer -		����� ������ ������;
//		size -			������ ������ ������.
// ���� ������:
//		 0 - ������� ��������� �������;
//		-1 - ������ ������ ������ ��� ����� 0;
long WMF_ClearBit(float* pBuffer, long lSize);

long WMF_FindPPS(float* pBuffer, long lSize, long* lAdress, long* lRTC, unsigned char* ucQuality);
long WMF_FindFirstPPS(float* pBuffer, long lSize, long* lAdress, long* lRTC, unsigned char* ucQuality);
long WMF_FindLastPPS(float* pBuffer, long lSize, long* lAdress, long* lRTC, unsigned char* ucQuality);

#endif