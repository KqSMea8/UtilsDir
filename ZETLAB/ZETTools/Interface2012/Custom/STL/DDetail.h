#pragma once
//#include "stdafx.h"
#include <Interface2012\Custom\STL\STLRead.h>
//#include <GL/glew.h>
#include "GL/GLAux.h"
//#include "GL/freeglut_std.h"
//#include <Interface2012\Custom\FunctionForAll.h>
#include <vector>

class CDDetail{
	public:
		CDDetail();
		~CDDetail();
		void Draw(facet *pFacet, int QntPlanes);
		void Draw_texture(facet *pFacet, int QntPlanes, int *PicSize, bool Stift);
        void Draw_texture_VBO(GLuint VBO, facet *pFacet, int QntPlanes, int *PicSize, bool Shift);
		void Draw_HMME(facet *pFacet, int QntPlanes, int *PicSize);
        void Draw_multitexture(facet *pFacet, int QntPlanes);
		void SetOffset(float X, float Y, float Z);
		void GetOffset(float &X, float &Y, float &Z);
		void SetRotate(float Rotate);
        float GetRotate();
		void SetRotateX(float RoteteX);
		void SetScale(float ScaleX, float ScaleY, float ScaleZ);
		void GetScale(float &X, float &Y, float &Z);
		void SetDimension(float Width, float Height, float Depth);
		void CalcDetailArray(facet *pFacet, int QntPlanes);
        void CalcMultiTextArray(facet *pFacet, int QntPlanes);
		void Clear();
		float MiddleX, MiddleY, MiddleZ;
		float MinX, MinY, MinZ;
		float MaxX, MaxY, MaxZ;
	private:
		float mat_dif[3], mat_amb[3], mat_spec[3];
		float shininess, Rotate, RotateX, OffsetX, OffsetY, OffsetZ, ScaleX, ScaleY, ScaleZ, Height, Width, Depth;
		int index;
		std::vector<float> pVertexs;
		std::vector<float> pNormals;
		std::vector<float> pTexCoords;
        std::vector<float> pMultiTexCoords;
	protected:
};