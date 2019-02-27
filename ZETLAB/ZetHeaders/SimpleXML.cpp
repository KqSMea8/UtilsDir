#include "StdAfx.h"
#include "SimpleXML.h"

SimpleXML::SimpleXML()
{
	file = NULL;
}

SimpleXML::SimpleXML( CString fileName )
{
	if (!Load( fileName )) return;
}

SimpleXML::~SimpleXML()
{
}

BOOL SimpleXML::Load( CString fileName )
{
	HRESULT hr = file.CreateInstance( __uuidof( DOMDocument ) );

	if (FAILED( hr ) || S_OK != hr) return FALSE;
	if (NULL == file) return FALSE;

	VARIANT_BOOL bSuccess = false;
	hr = file->load( CComVariant( fileName ), &bSuccess );

	if (FAILED( hr ) || S_OK != hr) return FALSE;
	if (!bSuccess) return FALSE;

	return TRUE;
}

BOOL SimpleXML::Save( CString fileName )
{
	if (NULL == file) return FALSE;

	// По умолчанию документ сохраняется без всякого форматирования,
	// без отступов, без переводов каретки и т.п.
	// Исправим это.

	// Документ для форматирования
	IXMLDOMDocumentPtr pure_xml;
	HRESULT hr = pure_xml.CreateInstance( __uuidof( DOMDocument ) );

	if (FAILED( hr ) || S_OK != hr) return FALSE;
	if (NULL == pure_xml) return FALSE;

	VARIANT_BOOL bSuccess = false;
	hr = pure_xml->loadXML(
		L"<?xml version=\"1.0\" encoding=\"utf-8\"?>" \
		L"<xsl:stylesheet xmlns:xsl=\"http://www.w3.org/1999/XSL/Transform\" version=\"1.0\">" \
			L"<xsl:output method=\"xml\" indent=\"yes\"/>" \
			L"<xsl:template match=\"@* | node()\">" \
				L"<xsl:copy>\r\n" \
					L"<xsl:choose>" \
						L"<xsl:when test=\"text()\">" \
							L"<xsl:apply-templates select=\"@* | node()\"/>" \
						L"</xsl:when>" \
						L"<xsl:when test=\"*\">" \
							L"<xsl:apply-templates select=\"@* | node()\"/>" \
						L"</xsl:when>" \
						L"<xsl:otherwise>" \
							L"<xsl:apply-templates select=\"@*\"/>" \
						L"</xsl:otherwise>" \
					L"</xsl:choose>" \
				L"</xsl:copy>\r\n" \
			L"</xsl:template>" \
		L"</xsl:stylesheet>", &bSuccess );

	if (FAILED( hr ) || S_OK != hr) return FALSE;
	if (!bSuccess) return FALSE;

	// Форматируем, с отступами и прочим
	BSTR bstrFormatted;
	hr = file->transformNode( pure_xml, &bstrFormatted );

	if (FAILED( hr ) || S_OK != hr) return FALSE;

	// Загружаем отформатированные данные обратно в исходный файл
	hr = file->loadXML( bstrFormatted, &bSuccess );

	if (FAILED( hr ) || S_OK != hr) return FALSE;
	if (!bSuccess) return FALSE;

	// Изменяем кодировку на UTF-8
	hXml p_header;
	hr = file->get_firstChild( &p_header );

	if (FAILED( hr ) || S_OK != hr) return FALSE;
	if (NULL == p_header) return FALSE;

	IXMLDOMNamedNodeMapPtr attrMap;
	hr = p_header->get_attributes( &attrMap );

	if (FAILED( hr ) || S_OK != hr) return FALSE;
	if (NULL == attrMap) return FALSE;

	hXml p_attr_encode;
	hr = attrMap->getNamedItem( CComBSTR( L"encoding" ), &p_attr_encode );

	if (FAILED( hr ) || S_OK != hr) return FALSE;
	if (NULL == p_attr_encode) return FALSE;

	hr = p_attr_encode->put_nodeValue( CComVariant( L"UTF-8" ) );

	if (FAILED( hr ) || S_OK != hr) return FALSE;

	// Сохраняем
	hr = file->save( CComVariant( fileName ) );

	if (FAILED( hr ) || S_OK != hr) return FALSE;

	return TRUE;
}

hXml SimpleXML::Create( CString rootName )
{
	HRESULT hr = file.CreateInstance( __uuidof( DOMDocument ) );

	if (FAILED( hr ) || S_OK != hr) return FALSE;
	if (NULL == file) return FALSE;

	IXMLDOMProcessingInstructionPtr p_node;
	file->createProcessingInstruction( L"xml", L" version='1.0' encoding='UTF-8'", &p_node );

	if (FAILED( hr ) || S_OK != hr) return FALSE;
	if (NULL == p_node) return FALSE;

	IXMLDOMNodePtr p_header_node;
	hr = file->appendChild( p_node, &p_header_node );

	if (FAILED( hr ) || S_OK != hr) return FALSE;
	if (NULL == p_header_node) return FALSE;

	// Добавляем корневой каталог
	// P.S. XML может содержать только ОДИН корневой элемент

	return AddElement( rootName, file );
}

CString SimpleXML::GetName( hXml el )
{
	if (NULL == file) return NULL;
	if (NULL == el) return NULL;

	// Получаем имя элемента
	CComBSTR p_name;
	HRESULT hr = el->get_nodeName( &p_name );

	if (FAILED( hr ) || S_OK != hr) return NULL;

	return CString( p_name );
}

hXml SimpleXML::GetRootNode()
{
	if (NULL == file) return FALSE;

	// Получаем указатель на элемент
	hXml p_node;
	HRESULT hr = file->get_lastChild( &p_node );

	if (FAILED( hr ) || S_OK != hr) return NULL;
	if (NULL == p_node) return NULL;

	return p_node;
}

CString SimpleXML::GetValue( hXml el )
{
	if (NULL == file) return NULL;
	if (NULL == el) return NULL;

	// Получаем значение элемента
	VARIANT value;
	HRESULT hr = el->get_nodeTypedValue( &value );

	if (FAILED( hr ) || S_OK != hr) return NULL;

	return CString( value.bstrVal );
}

BOOL SimpleXML::SetValue( hXml el, CString strValue )
{
	if (NULL == file) return FALSE;
	if (NULL == el) return FALSE;

	// Устанавливаем значение элемета
	VARIANT value;
	value.vt	  = VT_BSTR;
	value.bstrVal = strValue.AllocSysString();
	HRESULT hr = el->put_nodeTypedValue( value );

	if (FAILED( hr ) || S_OK != hr) return FALSE;

	return TRUE;
}

BOOL SimpleXML::SetValue( hXml el, bool boolValue )
{
	if (NULL == file) return FALSE;
	if (NULL == el) return FALSE;

	// Устанавливаем значение элемета
	VARIANT value;
	value.vt	  = VT_BOOL;
	value.boolVal = boolValue;
	HRESULT hr = el->put_nodeTypedValue( value );

	if (FAILED( hr ) || S_OK != hr) return FALSE;

	return TRUE;
}

BOOL SimpleXML::SetValue( hXml el, int intValue )
{
	if (NULL == file) return FALSE;
	if (NULL == el) return FALSE;

	// Устанавливаем значение элемета
	VARIANT value;
	value.vt	 = VT_I4;
	value.intVal = intValue;
	HRESULT hr = el->put_nodeTypedValue( value );

	if (FAILED( hr ) || S_OK != hr) return FALSE;

	return TRUE;
}

BOOL SimpleXML::SetValue( hXml el, double dblValue )
{
	if (NULL == file) return FALSE;
	if (NULL == el) return FALSE;

	// Устанавливаем значение элемета
	VARIANT value;
	value.vt	 = VT_R8;
	value.dblVal = dblValue;
	HRESULT hr = el->put_nodeTypedValue( value );

	if (FAILED( hr ) || S_OK != hr) return FALSE;

	return TRUE;
}

BOOL SimpleXML::GetAttributeValue( hXml el, CString attribute, VARIANT& value )
{
	if (NULL == file) return FALSE;
	if (NULL == el) return FALSE;

	// Получаем указатель на атрибут
	IXMLDOMElementPtr p_node_el = el;
	HRESULT hr = p_node_el->getAttribute( CComBSTR( attribute ), &value );

	if (FAILED( hr ) || S_OK != hr) return FALSE;

	return TRUE;
}

BOOL SimpleXML::SetAttributeValue( hXml el, CString attribute, VARIANT value )
{
	if (NULL == file) return FALSE;
	if (NULL == el) return FALSE;

	// Получаем указатель на атрибут
	IXMLDOMElementPtr p_node_el = el;
	HRESULT hr = p_node_el->setAttribute( CComBSTR( attribute ), value );

	if (FAILED( hr ) || S_OK != hr) return FALSE;

	return TRUE;
}

hXml SimpleXML::GetChildNode( hXml el )
{
	if (NULL == file) return NULL;
	if (NULL == el) return NULL;

	// Получаем указатель на первый дочерний элемент
	hXml p_first_child_node;
	HRESULT hr = el->get_firstChild( &p_first_child_node );

	if (FAILED( hr ) || S_OK != hr) return NULL;
	if (NULL == p_first_child_node) return NULL;

	return p_first_child_node;
}

hXml SimpleXML::GetNextNode( hXml el )
{
	if (NULL == file) return NULL;
	if (NULL == el) return NULL;

	// Получаем указатель на следующий элемент
	hXml p_next_node;
	HRESULT hr = el->get_nextSibling( &p_next_node );

	if (FAILED( hr ) || S_OK != hr) return NULL;
	if (NULL == p_next_node) return NULL;

	return p_next_node;
}

hXml SimpleXML::GetParent( hXml el )
{
	if (NULL == file) return NULL;
	if (NULL == el) return NULL;

	// Получаем указатель на родитель
	hXml p_parent_node;
	HRESULT hr = el->get_parentNode( &p_parent_node );

	if (FAILED( hr ) || S_OK != hr) return NULL;
	if (NULL == p_parent_node) return NULL;

	return p_parent_node;
}

hXml SimpleXML::AddElement( CString name, hXml parent )
{
	if (NULL == file) return NULL;
	if (NULL == parent) return NULL;

	IXMLDOMNodePtr p_node;
	HRESULT hr = file->createNode( CComVariant( NODE_ELEMENT ), CComBSTR( name ), NULL, &p_node );

	if (FAILED( hr ) || S_OK != hr) return NULL;
	if (NULL == p_node) return NULL;

	IXMLDOMNodePtr p_insert_node;
	hr = parent->appendChild( p_node, &p_insert_node );

	if (FAILED( hr ) || S_OK != hr) return NULL;
	if (NULL == p_insert_node) return NULL;

	return p_insert_node;
}

hXml SimpleXML::GetNode( CString name, hXml parent )
{
	if (NULL == file) return NULL;
	if (NULL == parent) return NULL;

	hXml el = GetChildNode( parent );

	while (el && name != GetName( el ))
	{
		el = GetNextNode( el );
	}

	return el;
}
