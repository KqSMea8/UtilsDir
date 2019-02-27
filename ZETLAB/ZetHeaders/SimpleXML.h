// (C) ������ ������������, ��� "����"
// ��� ����� ��������.
//
// ����� ��� ������ � XML-�������.
// ��������� � ���� ���������� ����� ����������� � ���������� ����������� MSXML

#pragma once

typedef IXMLDOMNodePtr hXml;

class SimpleXML
{
public:
	SimpleXML();
	SimpleXML( CString fileName );
	~SimpleXML();

	// ��������� xml-����
	BOOL Load( CString fileName );

	// ��������� ������ � xml-����
	BOOL Save( CString fileName );

	// ������� xml � �������� ��������� rootName
	hXml Create( CString rootName );

	// ���������� ��� xml-��������
	CString GetName( hXml el );

	// ���������� �������� ������� ���������
	hXml GetRootNode();

	// ������ �������� �������� el, ��������� � value
	CString GetValue( hXml el );

	// ������������� �������� value, �������� el
	BOOL SetValue( hXml el, CString strValue );
	BOOL SetValue( hXml el, bool boolValue );
	BOOL SetValue( hXml el, int intValue );
	BOOL SetValue( hXml el, double dblValue );

	// ������ ������� attribute, �������� el, ��������� �������� � value
	BOOL GetAttributeValue( hXml el, CString attribute, VARIANT& value );

	// ������������� �������� value, �������� attribute, �������� el
	BOOL SetAttributeValue( hXml el, CString attribute, VARIANT value );

	// ���������� �������� �������
	hXml GetChildNode( hXml el );

	// ���������� �������, ��������� �� el
	hXml GetNextNode( hXml el );

	// ���������� �������� ��������
	hXml GetParent( hXml el );

	// ��������� ������� � xml-��������
	hXml AddElement( CString name, hXml parent );

	// ���������� ������� � ������ name
	hXml GetNode( CString name, hXml parent );

private:
	// ��������� �� ��������/��������� xml-��������
	IXMLDOMDocumentPtr file;
};
