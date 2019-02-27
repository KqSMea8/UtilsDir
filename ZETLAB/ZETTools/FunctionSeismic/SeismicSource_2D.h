//------------------------------------------------------------------------------
//	���� CSeismicSource_2D.h
//	������ �� 12.07.2012
//	����������, ��� "����", �������� �.�.
//------------------------------------------------------------------------------
// ����� ������������ ��� ����������� ��������� ������������� ����������� ��
// ����������� ��������� ������� ���������������. �������� ������� �������������
// ������������� (���). ������� ����������� ���������� � ������������ �
// ���������� ��������-����� ��������� ����������� �������� ���� �������. ��� �
// ���� ������ ����� ���������.
// ��� ���������� ���������� ��������, ������� ������������ �� �����������
// ��������. ��������� �������������� �� ������� ����� ���������� �������.
//------------------------------------------------------------------------------
#pragma once
//------------------------------------------------------------------------------
#include "afx.h"
#include <FunctionSeismic\Seismic.h>
//------------------------------------------------------------------------------
class CSeismicSource_2D
{
private:
	// �������� ������ ----------------------------------------
	int Number;			// ���-�� ��������������� � ������
	float Vp;			// �������� ��������������� P-����, ��/���
	float dVp;			// ����������� �������� P-����, ��/���
	float dT;			// ����������� ����� ���������� (���� ��� ����), ���
	// ������� ������ ���������������
	Point2D *pGeog;		// �������������� ����������: x - latitude, y - longitude
	Point2D *pCart;		// ��������� ����������
	DATE *pTp;			// ������� ���������� P-���� 
	float *pDist2;		// ������������ �������� ��������� �� ���������
	float *pUncertDist;	// �� �����������
	// �������� ������ ---------------------------------------
	Point2D SourceGeog;	// �������������� ���������� ���������
	Point2D SourceCart;	// ��������� ���������� ���������
	DATE SourceTime;	// ����� �������
	float UncertR;		// ������ ��������������� ���������, ��
	float UncertT;		// ����������� ������� �������, ���

	// ��������������� ���������� ----------------------------
	int NumberHyperbole;// ���������� ��������
	float AccCoord;		// �������� ����������� ��������� ���������, ��
	// ������ ������� � ��������� �� ��� ������ �����
	float _FunctionAndGrad(Point2D *pP, Point2D *pGrad, bool needFunction);
	Point2D GeogCenter2D;	// �������������� ���������� ����������� ����� 2D,
	// �.�. ����� ������� ���������� ��������� ����������� �����

	// ��������� ������� ������ ------------------------------
	// ������� ����������� ��������� (�������������� � ����������)
//	void ConvertGeographyInCartesian();
//	void ConvertCartesianInGeography();

	// ������� ������� �������� � ��������� ������� ������������� �������������
	float CalcFunction(Point2D *pP);
	void  CalcGrad(Point2D *pP, Point2D *pGrad);
	float CalcFunctionAndGrad(Point2D *pP, Point2D *pGrad);

	void  CalcSourceTime();

	// ������ ������� ���������������
	void CalcUncertaintyRadius();

	// ��������������� �������
	Point2D SetFirstPoint();
	bool CalcDistance();
//	bool SetCenterFor2D();
	long ConjGrad_FletcherReeves();
	bool FindMin(Point2D *pP0, float *pValP0, Point2D *pS,
				 Point2D *pP1, float *pValP1, float accuracy);
protected:
public:
	CSeismicSource_2D();
	virtual ~CSeismicSource_2D();
	bool Create(int number, float dt, float vp, float dvp);

	// ������� ����������� ��������� (�������������� � ����������)
	void ConvertGeographyInCartesian();
	void ConvertCartesianInGeography();
	bool SetCenterFor2D();

	// ������� ������� (�������������� ����������, ������� ����������)
	long GetNumber() { return Number; }
	bool SetNumber(int num);
	float GetVp() { return Vp; }
	bool SetVp(float val);
	float GetUncertaintyVp() { return dVp; }
	bool SetUncertaintyVp(float val);
	float GetLatitude(int num);
	float GetLongitude(int num);
	bool SetLatitude(int num, float val);
	bool SetLongitude(int num, float val);
	DATE GetTp(int num);
	bool SetTp(int num, DATE val);
	Point2D* GetpCartPoint() { return pCart; }		// ��������� ����������

	// ������� ������ (������� � ��������� �����������)
	long  FindSource();
	float GetSourceLatitude() { return SourceGeog.y; }
	float GetSourceLongitude() { return SourceGeog.x; }
	DATE  GetSourceTime() { return SourceTime; }
	float GetUncertaintyRadius() { return UncertR; }
	float GetUncertaintyTime() { return UncertT; }
};
//------------------------------------------------------------------------------