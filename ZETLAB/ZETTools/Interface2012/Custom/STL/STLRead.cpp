#include "stdafx.h"
#include <Interface2012\Custom\STL\STLRead.h>

/*
Примерный вид текстового (ASCII) STL-файла:

solid [part name]
facet normal 0.576759 -0.415057 -0.703617
outer loop
vertex 2.22934 -0.992723 -0.862826
vertex 2.29245 -0.871852 -0.8824
vertex 2.41037 -0.777999 -0.841105
endloop
endfacet
...
endsolid [part name]
*/

/*
Структура бинарного STL-файла

1 байт Header[80]
4 байта NumberOfTriangles 

4 байта NormalVector[3]
4 байта Vertex1[3]
4 байта Vertex2[3]
4 байта Vertex3[3]
2 байта AttributeByteCount
*/

/*
При создании объекта необходимо указать имя файла в конструкторе
Вызвать метод ReadFile() для чтения указанного файла и GetQntOfFacets() для объекта класса, чтобы выяснить, сколько памяти выделять под структуру данных facet
Создать структуру facet для хранения данных
Вызвать метод GetData(facet *Buffer, int Size) для заполнения структуры.
*/
//------------------------------------------------------------------------------
 CSTLRead::CSTLRead(char *FileName)
	 :Error(0)
	 , pFile(nullptr)
 {

	if (fopen_s(&pFile, FileName, "rb") != 0)
		Error=-1;
	int EndFlag = 0;
	char String[SIZE_OF_ARRAY] = { "" };
	EndFlag=fscanf_s(pFile, "%s", String);
	QntOfFacet = 0;
	if(String[0] == 's')
	while (EndFlag!=EOF)
	{
		EndFlag = fscanf_s(pFile, "%s", String);
		if(String[0]=='f')
			QntOfFacet++;
	}
	else
	{
		bfHeader pHeader[1];
		fseek(pFile, 0, SEEK_SET);
		fread(pHeader, sizeof(bfHeader), 1, pFile);
		QntOfFacet=pHeader->QntOfFacet;
	}
	fseek(pFile, 0, SEEK_SET);
 };
 //------------------------------------------------------------------------------
 CSTLRead::~CSTLRead()
 {
	  fclose(pFile);
	  free(tFacet);
#ifdef _ACTIVEX_ZIS
	  if (TempPtr != NULL)
	  free(TempPtr);
#endif
	  pFile = nullptr;
	  tFacet = nullptr;
	  TempPtr = nullptr;
 };
 //------------------------------------------------------------------------------
bool CSTLRead::ReadDataBin()
{
	bfHeader pHeader[1];
	binfacet pFacet[2];
	size_t Size;
	int QntPlanes=0, Counter;
	MaxX = MaxY = MaxZ = MinX = MinY = MinZ=0;
	bool FirstReadFlag=false;

	fseek(pFile, 0, SEEK_SET);

	fread(pHeader, sizeof(bfHeader), 1, pFile);
	QntPlanes=pHeader->QntOfFacet;
	tFacet=(facet*)calloc(QntPlanes, sizeof(facet));
	Size=_msize(tFacet);

	for(Counter = 0; Counter<QntPlanes; Counter++){
		if(!feof(pFile))
		{
			fread(pFacet, sizeof(binfacet), 1, pFile);
			tFacet[Counter].Normal.X = pFacet->Normal.X;
			tFacet[Counter].Normal.Y = pFacet->Normal.Y;
			tFacet[Counter].Normal.Z = pFacet->Normal.Z;
			tFacet[Counter].Vertex[0].X = pFacet->Vertex1.X;
			tFacet[Counter].Vertex[0].Y = pFacet->Vertex1.Y;
			tFacet[Counter].Vertex[0].Z = pFacet->Vertex1.Z;
			if(FirstReadFlag == false){
				MaxX = tFacet[Counter].Vertex[0].X;
				MaxY = tFacet[Counter].Vertex[0].Y;
				MaxZ = tFacet[Counter].Vertex[0].Z;
				MinX = tFacet[Counter].Vertex[0].X;
				MinY = tFacet[Counter].Vertex[0].Y;
				MinZ = tFacet[Counter].Vertex[0].Z;
				FirstReadFlag = true;
			}
			if(tFacet[Counter].Vertex[0].X > MaxX)	MaxX = tFacet[Counter].Vertex[0].X;
			if(tFacet[Counter].Vertex[0].Y > MaxY)	MaxY = tFacet[Counter].Vertex[0].Y;
			if(tFacet[Counter].Vertex[0].Z > MaxZ)	MaxZ = tFacet[Counter].Vertex[0].Z;
			if(tFacet[Counter].Vertex[0].X < MinX)	MinX = tFacet[Counter].Vertex[0].X;
			if(tFacet[Counter].Vertex[0].Y < MinY)	MinY = tFacet[Counter].Vertex[0].Y;
			if(tFacet[Counter].Vertex[0].Z < MinZ)	MinZ = tFacet[Counter].Vertex[0].Z;
			tFacet[Counter].Vertex[1].X = pFacet->Vertex2.X;
			tFacet[Counter].Vertex[1].Y = pFacet->Vertex2.Y;
			tFacet[Counter].Vertex[1].Z = pFacet->Vertex2.Z;
			if(tFacet[Counter].Vertex[1].X > MaxX)	MaxX = tFacet[Counter].Vertex[1].X;
			if(tFacet[Counter].Vertex[1].Y > MaxY)	MaxY = tFacet[Counter].Vertex[1].Y;
			if(tFacet[Counter].Vertex[1].Z > MaxZ)	MaxZ = tFacet[Counter].Vertex[1].Z;
			if(tFacet[Counter].Vertex[1].X < MinX)	MinX = tFacet[Counter].Vertex[1].X;
			if(tFacet[Counter].Vertex[1].Y < MinY)	MinY = tFacet[Counter].Vertex[1].Y;
			if(tFacet[Counter].Vertex[1].Z < MinZ)	MinZ = tFacet[Counter].Vertex[1].Z;
			tFacet[Counter].Vertex[2].X = pFacet->Vertex3.X;
			tFacet[Counter].Vertex[2].Y = pFacet->Vertex3.Y;
			tFacet[Counter].Vertex[2].Z = pFacet->Vertex3.Z;
			if(tFacet[Counter].Vertex[2].X > MaxX)	MaxX = tFacet[Counter].Vertex[2].X;
			if(tFacet[Counter].Vertex[2].Y > MaxY)	MaxY = tFacet[Counter].Vertex[2].Y;
			if(tFacet[Counter].Vertex[2].Z > MaxZ)	MaxZ = tFacet[Counter].Vertex[2].Z;
			if(tFacet[Counter].Vertex[2].X < MinX)	MinX = tFacet[Counter].Vertex[2].X;
			if(tFacet[Counter].Vertex[2].Y < MinY)	MinY = tFacet[Counter].Vertex[2].Y;
			if(tFacet[Counter].Vertex[2].Z < MinZ)	MinZ = tFacet[Counter].Vertex[2].Z;
		}
	};
	
	QntOfFacet = QntPlanes;
	Width = MaxX - MinX;
	Height = MaxY - MinY;
	Depth = MaxZ - MinZ;
	MiddleX = (float)((MaxX + MinX) / 2.0);
	MiddleY = (float)((MaxY + MinY) / 2.0);
	MiddleZ = (float)((MaxZ + MinZ) / 2.0);

	return true;

};
//------------------------------------------------------------------------------
bool CSTLRead::ReadData(){
	int QntPlanes(0), Counter(0), Counter1(0), EndFlag(0);
	char c(0), String[SIZE_OF_ARRAY] = "";
	facet pFacet[1];
	size_t Size;
	bool FirstReadFlag(false);

	tFacet = (facet*)calloc(QntPlanes, sizeof(facet));

	Size = _msize(tFacet);

	if(fscanf_s(pFile, "%s", String)==NULL) return false;
	if(String[0]!='s') return false;
	MaxX = MaxY = MaxZ = MinX = MinY = MinZ = 0;

	while (EndFlag!=EOF)
	{
		EndFlag=fscanf_s(pFile, "%s", String);
		if(String[0]=='f')
		{
			QntPlanes++;
			tFacet=(facet*)realloc(tFacet, Size+QntPlanes*sizeof(facet));
			String[0]='\0';
			fscanf_s(pFile, "%s%f%f%f", String, &pFacet[0].Normal.X, &pFacet[0].Normal.Y, &pFacet[0].Normal.Z);
		}
		if(String[0]=='v'){
			fscanf_s(pFile, "%f%f%f", &pFacet[0].Vertex[Counter].X, &pFacet[0].Vertex[Counter].Y, &pFacet[0].Vertex[Counter].Z);
			if(FirstReadFlag==false){
				MaxX=pFacet[0].Vertex[Counter].X;
				MaxY=pFacet[0].Vertex[Counter].Y;
				MaxZ=pFacet[0].Vertex[Counter].Z;
				MinX=pFacet[0].Vertex[Counter].X;
				MinY=pFacet[0].Vertex[Counter].Y;
				MinZ=pFacet[0].Vertex[Counter].Z;
				FirstReadFlag=true;
			}
			if(pFacet[0].Vertex[Counter].X>MaxX) MaxX=pFacet[0].Vertex[Counter].X;
			if(pFacet[0].Vertex[Counter].X<MinX) MinX=pFacet[0].Vertex[Counter].X;
			if(pFacet[0].Vertex[Counter].Y>MaxY) MaxY=pFacet[0].Vertex[Counter].Y;
			if(pFacet[0].Vertex[Counter].Y<MinY) MinY=pFacet[0].Vertex[Counter].Y;
			if(pFacet[0].Vertex[Counter].Z>MaxZ) MaxZ=pFacet[0].Vertex[Counter].Z;
			if(pFacet[0].Vertex[Counter].Z<MinZ) MinZ=pFacet[0].Vertex[Counter].Z;
			Counter++;
		}
		if(String[0]=='e' && String[3]=='f')
		{
			Counter=0;
			memcpy(tFacet+QntPlanes-1, pFacet, sizeof(pFacet));
		}
	};

	QntOfFacet = QntPlanes;
	Width = MaxX - MinX;
	Height = MaxY - MinY;
	Depth = MaxZ - MinZ;
	MiddleX = (float)((MaxX + MinX) / 2.0);
	MiddleY = (float)((MaxY + MinY) / 2.0);
	MiddleZ = (float)((MaxZ + MinZ) / 2.0);

	return true;
 };
//------------------------------------------------------------------------------
void CSTLRead::GetData(facet *Buffer, int Size){
	int Counter;

	for(Counter = 0; Counter < Size; Counter++){
		memcpy(Buffer + Counter, tFacet + Counter, sizeof(facet));
	}

};
//------------------------------------------------------------------------------
void CSTLRead::ReadFile(){
	bool rc;

	rc=ReadData();
	if(!rc) ReadDataBin();
}
//------------------------------------------------------------------------------
int CSTLRead::GetQntOfFacets(){
	return QntOfFacet;
}
//------------------------------------------------------------------------------
float CSTLRead::GetDepth(){
	return Depth;
};
//------------------------------------------------------------------------------
float CSTLRead::GetHeight(){
	return Height;
};
//------------------------------------------------------------------------------
float CSTLRead::GetWidth(){
	return Width;
};
//------------------------------------------------------------------------------
float CSTLRead::GetMiddleX(){
	return MiddleX;
};
//------------------------------------------------------------------------------
float CSTLRead::GetMiddleY(){
	return MiddleY;
};
//------------------------------------------------------------------------------
float CSTLRead::GetMiddleZ(){
	return MiddleZ;
};
//------------------------------------------------------------------------------
facet* CSTLRead::GetFacetPtr()
{
	return tFacet;
}
//------------------------------------------------------------------------------