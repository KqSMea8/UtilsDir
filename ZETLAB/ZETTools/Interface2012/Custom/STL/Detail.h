#pragma once
#include <Interface2012\Custom\STL\STLRead.h>
#include <Interface2012\Custom\STL\DDetail.h>

class CDetail
{
	public:
		static int counter;
		int stl_number;
		CDetail();
		CDetail(const CDetail&);
		~CDetail();
		CDetail(char *FileName, float OffsetX, float OffsetY, float OffsetZ, float Rotate, float ScaleX, float ScaleY, float ScaleZ);
		CDetail(long number, float OffsetX, float OffsetY, float OffsetZ, float Rotate, float ScaleX, float ScaleY, float ScaleZ);
		void Resize(float OffsetX, float OffsetY, float OffsetZ, float Rotate, float ScaleX, float ScaleY, float ScaleZ);
		void CalcTexCoors(int picWidth, int picHeight);	//	расчЄт плоских текстурных координат дл€ модели
        void CalcTexCoors(int picWidth, int picHeight, double angle); //	расчЄт плоских текстурных координат дл€ модели с поворотом
        void CalcTexCoordHMME(int picWidth, int picHeight); //	расчЄт плоских текстурных координат дл€ модели (нат€нуть текстуру полностью на объект)
        void CalcTexCoordHMME(int picWidth, int picHeight, double angle); //	расчЄт плоских текстурных координат дл€ модели с поворотом (нат€нуть текстуру полностью на объект)
        void CalcMultiTexCoordHMME(int picWidth, int picHeight);
		void Clear();
		void Draw(facet *pFacet, int QntPlanes); //прорисовка детали без наложени€ текстуры
		void Draw_texture(facet *pFacet, int QntPlanes, int *PicSize, bool Shift=true); //прорисовка делали с наложение большой текстуры пиксель в пиксель
        void Draw_texture_VBO(GLuint VBO, facet *pFacet, int QntPlanes, int *PicSize, bool Shift);
		void Draw_HMME(facet *pFacet, int QntPlanes, int *PicSize); //нат€гивание всей текстуры на объект
        void Draw_multitexture(facet *pFacet, int QntPlanes);
		int GetQntPlanes();
		facet* ReadFacet();
		void SetRotate(float Rotete); //поворот фигуры по оси Z
        float GetRotate();
		void SetRotateX(float Rotete); //поворот фигуры по оси X
		void SetScale(float ScX, float ScY, float ScZ); //изменение масштаба модели
		void SetOffset(float X, float Y, float Z); //перенос центра модели в координаты (X,Y,Z)
		void GetOffset(float &X, float &Y, float &Z);
		//float GetOffsetZ(char Axis) { return DDetail.GetOffset('Z'); }
		void SetOffsetZ(float Z);
		float Width, Height, Depth;
	private:
		float MiddleX, MiddleY, MiddleZ;
		int QntPlanes, index;
		facet *pFacet;
		bool Removable, Seleted;
		float StartPointX, StartPointY;
		CDDetail DDetail;
};