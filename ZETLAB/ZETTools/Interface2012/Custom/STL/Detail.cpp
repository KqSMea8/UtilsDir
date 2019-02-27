#include "stdafx.h"
#include <Interface2012\Custom\STL\Detail.h>
#include <Interface2012\Custom\FunctionForAll.h>
//------------------------------------------------------------------------------
CDetail::CDetail(char *FileName, float OffsetX, float OffsetY, float OffsetZ, float Rotate, float ScaleX, float ScaleY, float ScaleZ)
{
	CSTLRead Object = CSTLRead (FileName);
	if (Object.Error != 0)
		exit(1);
	counter++;
	
	Object.ReadFile();
	QntPlanes = Object.GetQntOfFacets();
	pFacet = (facet*)calloc(QntPlanes, sizeof(facet));
	//TRACE (L"Buy memory %i\n", counter);
	Object.GetData(pFacet, QntPlanes);
	//получение размеров исходной модели
	Depth = Object.GetDepth();
	Height = Object.GetHeight();
	Width = Object.GetWidth();
	
	//получение центра исходной модели
	MiddleX = Object.GetMiddleX();
	MiddleY = Object.GetMiddleY();
	MiddleZ = Object.GetMiddleZ();

	//получаем координаты крайней левой и крайней правой
	//точки stl модели
	DDetail.MinX = Object.MinX;
	DDetail.MinY = Object.MinY;
	DDetail.MaxX = Object.MaxX;
	DDetail.MaxY = Object.MaxY;

	//вычисление центра реальной модели (в масштабе)
	DDetail.MiddleX = MiddleX;
	DDetail.MiddleY = MiddleY;
	DDetail.MiddleZ = MiddleZ;
	DDetail.SetOffset(OffsetX, OffsetY, OffsetZ);
	DDetail.SetDimension(Width, Height, Depth);
	
	if(Rotate == 90 || Rotate == 270) DDetail.SetScale(ScaleX/Height, ScaleY/Width, ScaleZ/Depth);
	if (Rotate == 0 || Rotate == 180) DDetail.SetScale(ScaleX/Width, ScaleY/Height, ScaleZ/Depth);
	
	DDetail.SetRotate(Rotate);
	DDetail.SetRotateX(0);

	CalcTexCoors(1000, 1000);

};
//------------------------------------------------------------------------------
CDetail::CDetail(long number, float OffsetX, float OffsetY, float OffsetZ, float Rotate, float ScaleX, float ScaleY, float ScaleZ)
{
	//TRACE("CREATE CDetail number %d\ttotal %d\n", number, counter+1);
#ifdef _ACTIVEX_ZIS
	counter++;
	stl_number = number;
	QntPlanes = ZetIntSrc->get_GetQntOfFacets(number);
	pFacet = nullptr;
	pFacet = (facet*)calloc(QntPlanes, sizeof(facet));
	//TRACE (L"Buy memory %i\n", counter);
	ZetIntSrc->get_Data((LONG*)pFacet, QntPlanes, number);

	Depth = ZetIntSrc->get_Depth(number);
	Height = ZetIntSrc->get_Height(number);
	Width = ZetIntSrc->get_Width(number);

	MiddleX = ZetIntSrc->get_Middle(number, 0);
	MiddleY = ZetIntSrc->get_Middle(number, 1);
	MiddleZ = ZetIntSrc->get_Middle(number, 2);

	DDetail.MinX = ZetIntSrc->get_MinMax(number, 0, 0);
	DDetail.MinY = ZetIntSrc->get_MinMax(number, 1, 0);
	DDetail.MaxX = ZetIntSrc->get_MinMax(number, 0, 1);
	DDetail.MaxY = ZetIntSrc->get_MinMax(number, 1, 1);

	DDetail.MiddleX = MiddleX;
	DDetail.MiddleY = MiddleY;
	DDetail.MiddleZ = MiddleZ;

	DDetail.SetOffset(OffsetX, OffsetY, OffsetZ);
	DDetail.SetDimension(Width, Height, Depth);

	if(Rotate==90 || Rotate==270) DDetail.SetScale(ScaleX/Height, ScaleY/Width, ScaleZ/Depth);
	if (Rotate==0 || Rotate==180) DDetail.SetScale(ScaleX/Width, ScaleY/Height, ScaleZ/Depth);

	DDetail.SetRotate(Rotate);
	DDetail.SetRotateX(0);

	CalcTexCoors(1000, 1000);
	Removable=Removable;
#endif
};
//------------------------------------------------------------------------------
void CDetail::Resize(float OffsetX, float OffsetY, float OffsetZ, float Rotate,
	float ScaleX, float ScaleY, float ScaleZ)
{
	DDetail.SetOffset(OffsetX, OffsetY, OffsetZ);
	if (Rotate == 90 || Rotate == 270) DDetail.SetScale(ScaleX / Height, ScaleY / Width, ScaleZ / Depth);
	if (Rotate == 0 || Rotate == 180) DDetail.SetScale(ScaleX / Width, ScaleY / Height, ScaleZ / Depth);
}
//------------------------------------------------------------------------------
CDetail::CDetail(const CDetail& source)
{
	TRACE("COPY CDetail number %d\ttotal %d\n", source.stl_number, counter+1);
}
//------------------------------------------------------------------------------
CDetail::CDetail(){

};
//------------------------------------------------------------------------------
CDetail::~CDetail()
{
//	counter--;
//	TRACE("DELETE CDetail number %d\ttotal %d\n", stl_number, counter);
#ifdef _ACTIVEX_ZIS
#else
	if (pFacet)
	{
		free(pFacet);
		pFacet = nullptr;
	}
	DDetail.Clear();
#endif
}
//------------------------------------------------------------------------------
int CDetail::counter = 0;
//------------------------------------------------------------------------------
void CDetail::Clear()
{
	counter--;
	TRACE (L"counter = %i\n", counter);
	if (pFacet != NULL)
	{
		free(pFacet);
		pFacet = nullptr;
	}
	DDetail.Clear();
}
//------------------------------------------------------------------------------
void CDetail::CalcTexCoors(int picWidth, int picHeight)
{
	//вычисление плоских текстурных координат
	float x(0.f), y(0.f);
    float scX = 1.0f, scY = 1.0f, scZ = 1.0f;
    float mdX = 0.0f, mdY = 0.0f, mdZ = 0.0f;
    float ofX = 0.0f, ofY = 0.0f, ofZ = 0.0f;
    mdX = DDetail.MiddleX;
    mdY = DDetail.MiddleY;
    mdZ = DDetail.MiddleZ;
    DDetail.GetOffset(ofX, ofY, ofZ);
    DDetail.GetScale(scX, scY, scZ);
    for (int triang = 0; triang < QntPlanes; triang++)
    {
        for(int vertex = 0; vertex < 3; vertex++)
        {
            x = pFacet[triang].Vertex[vertex].X - mdX;
            y = pFacet[triang].Vertex[vertex].Y - mdY;
            pFacet[triang].Texture[vertex].S = (x*scX + ofX) / picWidth;
            pFacet[triang].Texture[vertex].T = 1 - (y*scY + ofY) / picHeight;
        }
    }
    DDetail.Clear();
    DDetail.CalcDetailArray(pFacet, QntPlanes);
}
//------------------------------------------------------------------------------
void CDetail::CalcTexCoors(int picWidth, int picHeight, double angle)
{
    //вычисление плоских текстурных координат с учетом угла поворота
	float x(0.f), y(0.f), xa(0.f), ya(0.f);
    float scX = 1.0f, scY = 1.0f, scZ = 1.0f;
    float mdX = 0.0f, mdY = 0.0f, mdZ = 0.0f;
    float ofX = 0.0f, ofY = 0.0f, ofZ = 0.0f;
    mdX = DDetail.MiddleX;
    mdY = DDetail.MiddleY;
    mdZ = DDetail.MiddleZ;
    angle = angle*3.14/180;
    DDetail.GetOffset(ofX, ofY, ofZ);
    DDetail.GetScale(scX, scY, scZ);
    for (int triang = 0; triang < QntPlanes; triang++)
    {
        for(int vertex = 0; vertex < 3; vertex++)
        {
            x = (pFacet[triang].Vertex[vertex].X - mdX)*cos(angle) - (pFacet[triang].Vertex[vertex].Y - mdY)*sin(angle);
            y = (pFacet[triang].Vertex[vertex].X - mdX)*sin(angle) + (pFacet[triang].Vertex[vertex].Y - mdY)*cos(angle);
            pFacet[triang].Texture[vertex].S = (x*scX + ofX) / picWidth;
            pFacet[triang].Texture[vertex].T = 1 - (y*scY + ofY) / picHeight;

        }
    }
    DDetail.Clear();
    DDetail.CalcDetailArray(pFacet, QntPlanes);
}
//------------------------------------------------------------------------------
void CDetail::CalcTexCoordHMME(int picWidth, int picHeight)
{
	float x(0.f), y(0.f);
    float scX = 1.0f, scY = 1.0f, scZ = 1.0f;
    float mdX = 0.0f, mdY = 0.0f, mdZ = 0.0f;
    float ofX = 0.0f, ofY = 0.0f, ofZ = 0.0f;
    mdX = DDetail.MiddleX;
    mdY = DDetail.MiddleY;
    mdZ = DDetail.MiddleZ;
    DDetail.GetOffset(ofX, ofY, ofZ);
    DDetail.GetScale(scX, scY, scZ);
    for (int triang = 0; triang < QntPlanes; triang++)
    {
        for(int vertex = 0; vertex < 3; vertex++)
        {
            x = pFacet[triang].Vertex[vertex].X - DDetail.MinX;
            y = pFacet[triang].Vertex[vertex].Y - DDetail.MinY;
            pFacet[triang].Texture[vertex].S = 1 - (x*picWidth/Width)/picWidth;
            pFacet[triang].Texture[vertex].T = (y*picHeight/Height)/picHeight;
        }
    }
    DDetail.Clear();
    DDetail.CalcDetailArray(pFacet, QntPlanes);
}
//------------------------------------------------------------------------------
void CDetail::CalcTexCoordHMME(int picWidth, int picHeight, double angle)
{
	float x(0.f), y(0.f);
    float scX = 1.0f, scY = 1.0f, scZ = 1.0f;
    float mdX = 0.0f, mdY = 0.0f, mdZ = 0.0f;
    float ofX = 0.0f, ofY = 0.0f, ofZ = 0.0f;
    mdX = DDetail.MiddleX;
    mdY = DDetail.MiddleY;
    mdZ = DDetail.MiddleZ;
    angle = angle*3.14/180;
    DDetail.GetOffset(ofX, ofY, ofZ);
    DDetail.GetScale(scX, scY, scZ);
    for (int triang = 0; triang < QntPlanes; triang++)
    {
        for(int vertex = 0; vertex < 3; vertex++)
        {
            x = (pFacet[triang].Vertex[vertex].X - DDetail.MinX)*cos(angle) - (pFacet[triang].Vertex[vertex].Y - DDetail.MinY)*sin(angle);
            y = (pFacet[triang].Vertex[vertex].X - DDetail.MinX)*sin(angle) + (pFacet[triang].Vertex[vertex].Y - DDetail.MinY)*cos(angle);
            pFacet[triang].Texture[vertex].S = 1 - (x*picWidth/Width)/picWidth;
            pFacet[triang].Texture[vertex].T = (y*picHeight/Height)/picHeight;
        }
    }
    DDetail.Clear();
    DDetail.CalcDetailArray(pFacet, QntPlanes);
}
//------------------------------------------------------------------------------
void CDetail::CalcMultiTexCoordHMME(int picWidth, int picHeight)
{
	float x(0.f), y(0.f);
    float scX = 1.0f, scY = 1.0f, scZ = 1.0f;
    float mdX = 0.0f, mdY = 0.0f, mdZ = 0.0f;
    float ofX = 0.0f, ofY = 0.0f, ofZ = 0.0f;
    mdX = DDetail.MiddleX;
    mdY = DDetail.MiddleY;
    mdZ = DDetail.MiddleZ;
    DDetail.GetOffset(ofX, ofY, ofZ);
    DDetail.GetScale(scX, scY, scZ);
    for (int triang = 0; triang < QntPlanes; triang++)
    {
        for(int vertex = 0; vertex < 3; vertex++)
        {
            x = pFacet[triang].Vertex[vertex].X - DDetail.MinX;
            y = pFacet[triang].Vertex[vertex].Y - DDetail.MinY;
            pFacet[triang].Texture[vertex].S = 1 - (x*picWidth/Width)/picWidth;
            pFacet[triang].Texture[vertex].T = (y*picHeight/Height)/picHeight;
        }
    }
    DDetail.CalcMultiTextArray(pFacet, QntPlanes);
}
//------------------------------------------------------------------------------
void CDetail::Draw(facet *pFacet, int QntPlanes)
{
	DDetail.Draw(pFacet, QntPlanes);
};
//------------------------------------------------------------------------------
void CDetail::Draw_texture(facet *pFacet, int QntPlanes, int *PicSize, bool Shift)
{
	DDetail.Draw_texture(pFacet, QntPlanes, PicSize, Shift);
};
//------------------------------------------------------------------------------
void CDetail::Draw_texture_VBO(GLuint VBO, facet *pFacet, int QntPlanes, int *PicSize, bool Shift)
{
    //DDetail.Draw_texture_VBO(VBO, pFacet, QntPlanes, PicSize, Shift);
}
//------------------------------------------------------------------------------
void CDetail::Draw_HMME(facet *pFacet, int QntPlanes, int *PicSize)
{
	DDetail.Draw_HMME(pFacet, QntPlanes, PicSize);
};
//------------------------------------------------------------------------------
void CDetail::Draw_multitexture(facet *pFacet, int QntPlanes)
{
    DDetail.Draw_multitexture(pFacet, QntPlanes);
}
//------------------------------------------------------------------------------
int CDetail::GetQntPlanes(){
	return QntPlanes;
};
//------------------------------------------------------------------------------
facet* CDetail::ReadFacet(){
	return pFacet;
};
//------------------------------------------------------------------------------
void CDetail::SetRotate(float Rotete){
	DDetail.SetRotate(Rotete);
};
//------------------------------------------------------------------------------
float CDetail::GetRotate()
{
    return DDetail.GetRotate();
}
//------------------------------------------------------------------------------
void CDetail::SetRotateX(float RoteteX){
	DDetail.SetRotateX(RoteteX);
};
//------------------------------------------------------------------------------
void CDetail::SetOffset(float X, float Y, float Z){
	DDetail.SetOffset(X, Y, Z);
};
//------------------------------------------------------------------------------
void CDetail::GetOffset(float &X, float &Y, float &Z){
	 DDetail.GetOffset(X, Y, Z);
};
//------------------------------------------------------------------------------
void CDetail::SetScale(float ScX, float ScY, float ScZ){
	DDetail.SetScale(ScX/Width, ScY/Height, ScZ/Depth);
};
//------------------------------------------------------------------------------
void CDetail::SetOffsetZ( float Z )
{
	float tX(0.f), tY(0.f), tZ(0.f);
	DDetail.GetOffset(tX, tY, tZ);
	DDetail.SetOffset(tX, tY, Z);
}
//------------------------------------------------------------------------------