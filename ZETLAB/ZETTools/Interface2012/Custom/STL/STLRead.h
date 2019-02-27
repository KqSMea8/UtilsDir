#pragma once
//#include "stdafx.h"
#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <stdint.h>

#define SIZE_OF_ARRAY 256

struct hdp
{
	float	X;
	float	Y;
	float	Z;
	hdp()
		: X(0.f)
		, Y(0.f)
		, Z(0.f)
	{
	}
};

struct tdp
{
	float S;
	float T;
	tdp()
		: S(0.f)
		, T(0.f)
	{
	}
};

struct facet
{
	hdp Vertex[3];
	hdp Normal;
	tdp Texture[3];
	facet()
	{
		ZeroMemory(Vertex, sizeof(Vertex));
	}
};

struct bfHeader
{
	unsigned char Header[80];
	unsigned int QntOfFacet;
	bfHeader() 
		: QntOfFacet(0)
	{
		ZeroMemory(Header, sizeof(Header));
	}
};

#pragma pack(push, 2)
struct binfacet
{
	hdp Normal;
	hdp Vertex1;
	hdp Vertex2;
	hdp Vertex3;
	unsigned short AttributeByteCount;
	binfacet()
		: AttributeByteCount(0)
	{
	}
};
#pragma pack(pop)

class CSTLRead
{
	public:
		int Error;
		CSTLRead(char *FileName);
		int GetQntOfFacets();
		void ReadFile();
		void GetData(facet *Buffer, int Size);
		float GetWidth();
		float GetHeight();
		float GetDepth();
		float GetMiddleX();
		float GetMiddleY();
		float GetMiddleZ();
		facet* GetFacetPtr();
		~CSTLRead();
		float MaxX, MaxY, MaxZ, MinX, MinY, MinZ;
		facet *TempPtr;
	private:
		FILE *pFile;
		int QntOfFacet;
		facet *tFacet;
		binfacet *bFacet;
		bool ReadData();
		bool ReadDataBin();
		float Depth, Width, Height;
		float MiddleX, MiddleY, MiddleZ;
};