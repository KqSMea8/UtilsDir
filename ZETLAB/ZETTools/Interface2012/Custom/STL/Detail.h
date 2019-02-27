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
		void CalcTexCoors(int picWidth, int picHeight);	//	������ ������� ���������� ��������� ��� ������
        void CalcTexCoors(int picWidth, int picHeight, double angle); //	������ ������� ���������� ��������� ��� ������ � ���������
        void CalcTexCoordHMME(int picWidth, int picHeight); //	������ ������� ���������� ��������� ��� ������ (�������� �������� ��������� �� ������)
        void CalcTexCoordHMME(int picWidth, int picHeight, double angle); //	������ ������� ���������� ��������� ��� ������ � ��������� (�������� �������� ��������� �� ������)
        void CalcMultiTexCoordHMME(int picWidth, int picHeight);
		void Clear();
		void Draw(facet *pFacet, int QntPlanes); //���������� ������ ��� ��������� ��������
		void Draw_texture(facet *pFacet, int QntPlanes, int *PicSize, bool Shift=true); //���������� ������ � ��������� ������� �������� ������� � �������
        void Draw_texture_VBO(GLuint VBO, facet *pFacet, int QntPlanes, int *PicSize, bool Shift);
		void Draw_HMME(facet *pFacet, int QntPlanes, int *PicSize); //����������� ���� �������� �� ������
        void Draw_multitexture(facet *pFacet, int QntPlanes);
		int GetQntPlanes();
		facet* ReadFacet();
		void SetRotate(float Rotete); //������� ������ �� ��� Z
        float GetRotate();
		void SetRotateX(float Rotete); //������� ������ �� ��� X
		void SetScale(float ScX, float ScY, float ScZ); //��������� �������� ������
		void SetOffset(float X, float Y, float Z); //������� ������ ������ � ���������� (X,Y,Z)
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