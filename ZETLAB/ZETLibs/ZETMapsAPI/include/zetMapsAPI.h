#pragma once

// ������ ���������� ��������� HTML ��������, � ������� ������������ ������ �����.
// �������� ������ � ������ ������� ����� ��� DLL:
//     1. ������������������� ��������� HTML �������� � ������ ������� (zyapiInitMap)
//     2. �������� ���� �� ����� (zyapiAddCustomBubleOnMap)
//     3. ���������� ����� (zyapiShowMap)
//     4. ��� ������������� ������� ��������� ���� (zyapiCloseMap)

#include <string>

#ifdef ZETMAPSAPI_EXPORTS
    // ������ ������ ��������� ������ � ������� DLL, ������� ���������������� � ��������
    #define ZETMAPSAPI_API _declspec(dllexport)
#else
    // ������ ������ �� ��������� � �������� ������������ ������ DLL, ������� ���������������� � �������
    #define ZETMAPSAPI_API _declspec(dllimport)
#endif

class ZETMAPSAPI_API CZetMapsAPI
{
private:
    class Impl;

    Impl *m_iImpl = nullptr;
public:
    CZetMapsAPI();
    ~CZetMapsAPI();

    // ������� ��� ������������� ����������
    // ������� ��������: ����� �� �������� ����� ������������� �������������� HTML ��������, ��� ������ ��������, ���� ����� ����������� � ���������� �� ���������
    // �������� �������� - ����� �� �������� ������������� ��������, ������ �������� - ������
    std::wstring Init(std::wstring htmlPath_ = L"");

    // ������� ��� ���������� ����� �� �����
    // ������� ���������:
    //                    latitude_    - ������
    //                    longitude_   - �������
    //                    hintText_    - ����������� �����
    //                    captionText_ - �����, ������������ ����� � ������
    void AddCustomBubble(double latitude_, double longitude_,
        std::wstring hintText_, std::wstring captionText_);

    // ������� ��� ���������� ����� �� �����
    // ������� ���������:
    //                    latitudeStart_    - ������ ������ �����
    //                    longitudeStart_   - ������� ������ �����
    //                    latitudeEnd_      - ������ ����� �����
    //                    longitudeEnd_     - ������� ����� �����
    //                    hintText_    - ����������� �����
    void AddCustomLine(double latitudeStart_, double longitudeStart_,
        double latitudeEnd_, double longitudeEnd_, std::wstring hintText_);
    void AddCustomLines(double* latitude_, double* longitude_, unsigned long coordCount_, std::wstring hintText_);

    // ���������� �������������� �����
    void Show();

    // ���������������� ���������
    void Close();
};