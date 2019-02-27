// (C) ������ ������������, ��� "����"
// ��� ����� ��������.
//
// ����� ��� ������ � XML-�������.
// ��������� � ���� ���������� ����� ����������� � ���������� ����������� MSXML

#pragma once
#include <comdef.h>
#include <atlcomcli.h>
#include <atlstr.h>
#include <comdef.h>
#include <atlcomcli.h>

typedef IXMLDOMNodePtr hXml;

class SimpleXML
{
public:
	SimpleXML();
	SimpleXML( CString cstrFileName );
	~SimpleXML();

	// ��������� xml-����
	BOOL Load( CString cstrFileName );

	// ������ ������� ������ ��� xml-����
	BOOL LoadIndirect( BSTR bstrText );

	// ��������� ������ � xml-����
	BOOL Save( CString cstrFileName );

	// ������� xml � �������� ��������� rootName
	hXml Create( CString cstrRootName );

	// ���������� �������� ������� ���������
	hXml GetRootNode();

	// ���������� �������� ��������
	hXml GetParent( hXml el );

	// ���������� �������� �������
	hXml GetChildNode( hXml el );

	// ���������� �������, ��������� �� el
	hXml GetNextNode( hXml el );

	// ��������� ������� � xml-��������
	hXml AddElement( CString cstrName, hXml parent );

	// ������� ������� �� xml-���������
	BOOL RemoveElement( CString cstrName, hXml parent );
	BOOL RemoveElement( hXml el, hXml parent );

	// ���������� ��� xml-��������
	CString GetName( hXml el );

	// ���������� ������� � ������ name
	hXml GetNode( CString cstrName, hXml parent );

	// ���������� ��������� �������� �������� el
	CString GetValue( hXml el );

	// ������������� �������� value, �������� el
	BOOL SetValue( hXml el, CString cstrValue );
	BOOL SetValue( hXml el, bool boolValue );
	BOOL SetValue( hXml el, int intValue );
	BOOL SetValue( hXml el, double dblValue );

	// ������ ������� attribute, �������� el, ��������� �������� � value
	BOOL GetAttributeValue( hXml el, CString cstrAttribute, VARIANT& value );

	// ������������� �������� value, �������� attribute, �������� el
	BOOL SetAttributeValue( hXml el, CString cstrAttribute, VARIANT value );

private:
	// ��������� �� ��������/��������� xml-��������
	IXMLDOMDocumentPtr file;
};
