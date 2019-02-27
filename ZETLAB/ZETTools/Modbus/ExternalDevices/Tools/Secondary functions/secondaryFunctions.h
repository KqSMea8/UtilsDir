// ��������������� ������� ��� ��������������� ������, �������������� � �.�. 
// ������� ����� ����������� ��� ������ � ���������, ���������� ������������
#pragma once
#include <afx.h>
#include <vector>

// ��������������� ������ � ��������
void SF_convertCStringToChar(CString str_, char* charStr_, long charSz_);

// ��������������� float �������� � ������
CString SF_convertFloatValToCString(float val_);

// ��������������� long �������� � ������
CString SF_convertLongValToCString(long val_);

// ��������� ������ COM ������ � �����������
// comPortNames_ - ������ ���� COM ������
// comPortFriendlyNames_ - ������ ������������� ���� COM ������
// comPortInstanceIds_ - ������ ���������� ��������������� COM ������
void SF_getCommPortsList(std::vector<CString>& comPortNames_, std::vector<CString>& comPortFriendlyNames_,
    std::vector<CString>& comPortInstanceIds_);