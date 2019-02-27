#pragma once
#include <Struct_grid.h>
#include <pugixml/include/pugiautolink.hpp>
#include <list>
// ������������ ������ ��� ������ xml
namespace LoadSaveGridSetting
{
enum Errors
{
	ERROR_LOAD_FILE = -1,
	ERROR_NO_CONFIG = -2,
};
};

class CLoadSaveGridGL
{
public:
	CLoadSaveGridGL();
	~CLoadSaveGridGL();

public:
	/// <summary>���������� ��������� GridGL � ���� � ���� xml.</summary>
	/// <returns> ���������� true ��� �������� ����������</returns>
	/// <param name="File">���� � �����</param>
	/// <param name="GridGLParams">��������� �����</param>
	/// <param name="Comments">������ ������������, ������������ � ����</param>
	/// <param name="CommentsCount">���������� ������������ ������������</param>
	bool SaveGridParams(_In_ CString File, _In_  grid *GridGLParams,
						_In_opt_ CString *Comments = nullptr, 
						_In_opt_ size_t   CommentsCount = 0);
	/// <summary>�������� ��������� GridGL �� xml �����.</summary>
	/// <returns> ���������� S_OK ��� �������� ���������� �����, � ��������� ������ ��� ������ �� LoadSaveGridSetting::Errors.</returns>
	/// <param name="File">���� � �����</param>
	/// <param name="GridGLParams">��������� �����</param>
	/// <param name="Comments">������ ������������ � �����</param>
	long LoadGridParams(_In_ CString File, _Out_ grid *GridGLParams, 
						_Out_opt_ std::list<CString> &Comments = std::list<CString>());
};

