#include "StdAfx.h"
#include "SimpleXML.h"

#define fail(_1)				(FAILED(_1)  || S_OK != _1)
#define fail_or_fail(_1, _2)	((FAILED(_1) || S_OK != _1) || !_2)
#define null(_1)				(NULL == _1)
#define null_or_null(_1, _2)	(NULL == _1  || NULL == _2)
#define fail_or_null(_1, _2)	((FAILED(_1) || S_OK != _1) || NULL == _2)

SimpleXML::SimpleXML() : file(NULL)
{
}

SimpleXML::SimpleXML( CString cstrFileName )
{
	if (!Load( cstrFileName ))
		return;
}

SimpleXML::~SimpleXML()
{
}

BOOL SimpleXML::Load( CString cstrFileName )
{
	HRESULT hr = file.CreateInstance( __uuidof( DOMDocument ) );

	if (fail_or_null( hr, file ))
		return FALSE;

	VARIANT_BOOL bSuccess = false;
	hr = file->load( CComVariant( cstrFileName ), &bSuccess );

	if (fail_or_fail( hr, bSuccess ))
	{
		IXMLDOMParseErrorPtr errPtr;
		file->get_parseError(&errPtr);
		BSTR bstrErr;
		errPtr->get_reason(&bstrErr);

		TRACE(_T("Error:\n"));
		long code, line, pos;
		errPtr->get_errorCode(&code);
		errPtr->get_line(&line);
		errPtr->get_linepos(&pos);
		TRACE(_T("Code = 0x%x\n"), code);
		TRACE(_T("Source = Line : %ld; Char : %ld\n"), line, pos);
		TRACE(_T("Error Description = %s\n"), CStringA(bstrErr));
		return FALSE;
	}

	return TRUE;
}

BOOL SimpleXML::LoadIndirect( BSTR bstrText )
{
	HRESULT hr = file.CreateInstance( __uuidof( DOMDocument ) );

	if (fail_or_null( hr, file ))
		return FALSE;

	VARIANT_BOOL bSuccess = false;
	hr = file->loadXML( bstrText, &bSuccess );

	if (fail_or_fail( hr, bSuccess ))
		return FALSE;

	return TRUE;
}

BOOL SimpleXML::Save( CString cstrFileName )
{
	if (null( file ))
		return FALSE;

	// По умолчанию документ сохраняется без всякого форматирования,
	// без отступов, без переводов каретки и т.п.
	// Исправим это.

	// Документ для форматирования
	IXMLDOMDocumentPtr pure_xml;
	HRESULT hr = pure_xml.CreateInstance( __uuidof( DOMDocument ) );

	if (fail_or_null( hr, pure_xml ))
		return FALSE;

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

	if (fail_or_fail( hr, bSuccess ))
		return FALSE;

	// Форматируем, с отступами и прочим
	BSTR bstrFormatted;
	hr = file->transformNode( pure_xml, &bstrFormatted );

	if (fail( hr ))
		return FALSE;

	// Загружаем отформатированные данные обратно в исходный файл
	hr = file->loadXML( bstrFormatted, &bSuccess );

	if (fail_or_fail( hr, bSuccess ))
		return FALSE;

	// Изменяем кодировку на UTF-8
	hXml p_header;
	hr = file->get_firstChild( &p_header );

	if (fail_or_null( hr, p_header ))
		return FALSE;

	IXMLDOMNamedNodeMapPtr attrMap;
	hr = p_header->get_attributes( &attrMap );

	if (fail_or_null( hr, attrMap ))
		return FALSE;

	hXml p_attr_encode;
	hr = attrMap->getNamedItem( CComBSTR( L"encoding" ), &p_attr_encode );

	if (fail_or_null( hr, p_attr_encode ))
		return FALSE;

	hr = p_attr_encode->put_nodeValue( CComVariant( L"UTF-8" ) );

	if (fail( hr ))
		return FALSE;

	// Сохраняем
	hr = file->save( CComVariant( cstrFileName ) );

	if (fail( hr ))
		return FALSE;

	return TRUE;
}

hXml SimpleXML::Create( CString cstrRootName )
{
	HRESULT hr = file.CreateInstance( __uuidof( DOMDocument ) );

	if (fail_or_null( hr, file ))
		return NULL;

	IXMLDOMProcessingInstructionPtr p_node;
	file->createProcessingInstruction( L"xml", L" version='1.0' encoding='UTF-8'", &p_node );

	if (fail_or_null( hr, p_node ))
		return NULL;

	IXMLDOMNodePtr p_header_node;
	hr = file->appendChild( p_node, &p_header_node );

	if (fail_or_null( hr, p_header_node ))
		return NULL;

	// Добавляем корневой каталог
	// P.S. XML может содержать только ОДИН корневой элемент
	return AddElement( cstrRootName, file );
}

hXml SimpleXML::GetRootNode()
{
	if (null( file ))
		return NULL;

	// Получаем указатель на элемент
	hXml p_node;
	HRESULT hr = file->get_lastChild( &p_node );

	if (fail_or_null( hr, p_node ))
		return NULL;

	return p_node;
}

hXml SimpleXML::GetParent( hXml el )
{
	if (null_or_null( file, el ))
		return NULL;

	// Получаем указатель на родитель
	hXml p_parent_node;
	HRESULT hr = el->get_parentNode( &p_parent_node );

	if (fail_or_null( hr, p_parent_node ))
		return NULL;

	return p_parent_node;
}

hXml SimpleXML::GetChildNode( hXml el )
{
	if (null_or_null( file, el ))
		return NULL;

	// Получаем указатель на первый дочерний элемент
	hXml p_first_child_node;
	HRESULT hr = el->get_firstChild( &p_first_child_node );

	if (fail_or_null( hr, p_first_child_node ))
		return NULL;

	return p_first_child_node;
}

hXml SimpleXML::GetNextNode( hXml el )
{
	if (null_or_null( file, el ))
		return NULL;

	// Получаем указатель на следующий элемент
	hXml p_next_node;
	HRESULT hr = el->get_nextSibling( &p_next_node );

	if (fail_or_null( hr, p_next_node ))
		return NULL;

	return p_next_node;
}

hXml SimpleXML::AddElement( CString cstrName, hXml parent )
{
	if (null_or_null( file, parent ))
		return NULL;

	IXMLDOMNodePtr p_node;
	HRESULT hr = file->createNode( CComVariant( NODE_ELEMENT ), CComBSTR( cstrName ), NULL, &p_node );

	if (fail_or_null( hr, p_node ))
		return NULL;

	IXMLDOMNodePtr p_insert_node;
	hr = parent->appendChild( p_node, &p_insert_node );

	if (fail_or_null( hr, p_insert_node ))
		return NULL;

	return p_insert_node;
}

BOOL SimpleXML::RemoveElement( CString cstrName, hXml parent )
{
	if (null_or_null( file, parent ))
		return FALSE;

	hXml p_node = GetNode( cstrName, parent );

	if (null( p_node ))
		return FALSE;

	IXMLDOMNodePtr p_removed_node;
	HRESULT hr = parent->removeChild( p_node, &p_removed_node );

	if (fail_or_null( hr, p_removed_node ))
		return FALSE;

	return TRUE;
}

BOOL SimpleXML::RemoveElement( hXml el, hXml parent )
{
	if (null_or_null( file, parent ))
		return FALSE;

	if (null( el ))
		return FALSE;

	IXMLDOMNodePtr p_removed_node;
	HRESULT hr = parent->removeChild( el, &p_removed_node );

	if (fail_or_null( hr, p_removed_node ))
		return FALSE;

	return TRUE;
}

CString SimpleXML::GetName( hXml el )
{
	if (null_or_null( file, el ))
        return L"";
		//return FALSE;

	// Получаем имя элемента
	CComBSTR p_name;
	HRESULT hr = el->get_nodeName( &p_name );

	if (fail( hr ))
		//return FALSE;
        return L"";

	return CString( p_name );
}

hXml SimpleXML::GetNode( CString cstrName, hXml parent )
{
	if (null_or_null( file, parent ))
	    return NULL;

	hXml el = GetChildNode( parent );

	while (el && cstrName != GetName( el ))
	{
		el = GetNextNode( el );
	}

	return el;
}

CString SimpleXML::GetValue( hXml el )
{
	if (null_or_null( file, el ))
		//return NULL;
        return L"";

	// Получаем значение элемента
	VARIANT value;
	HRESULT hr = el->get_nodeTypedValue( &value );

	if (fail( hr ))
		//return NULL;
        return L"";

	return CString( value.bstrVal );
}

BOOL SimpleXML::SetValue( hXml el, CString cstrValue )
{
	if (null_or_null( file, el ))
		return FALSE;

	// Устанавливаем значение элемета
	VARIANT value;
	value.vt	  = VT_BSTR;
	value.bstrVal = cstrValue.AllocSysString();

	HRESULT hr = el->put_nodeTypedValue( value );

	if (fail( hr ))
		return FALSE;

	return TRUE;
}

BOOL SimpleXML::SetValue( hXml el, bool boolValue )
{
	if (null_or_null( file, el ))
		return FALSE;

	// Устанавливаем значение элемента
	VARIANT value;
	value.vt	  = VT_BOOL;
	value.boolVal = boolValue ? VARIANT_TRUE : VARIANT_FALSE;

	HRESULT hr = el->put_nodeTypedValue( value );

	if (fail( hr ))
		return FALSE;

	return TRUE;
}

BOOL SimpleXML::SetValue( hXml el, int intValue )
{
	if (null_or_null( file, el ))
		return FALSE;

	// Устанавливаем значение элемета
	VARIANT value;
	value.vt	 = VT_I4;
	value.intVal = intValue;

	HRESULT hr = el->put_nodeTypedValue( value );

	if (fail( hr ))
		return FALSE;

	return TRUE;
}

BOOL SimpleXML::SetValue( hXml el, double dblValue )
{
	if (null_or_null( file, el ))
		return FALSE;

	// Устанавливаем значение элемета
	VARIANT value;
	value.vt	 = VT_R8;
	value.dblVal = dblValue;

	HRESULT hr = el->put_nodeTypedValue( value );

	if (fail( hr ))
		return FALSE;

	return TRUE;
}

BOOL SimpleXML::GetAttributeValue( hXml el, CString cstrAttribute, VARIANT& value )
{
	if (null_or_null( file, el ))
		return FALSE;

	// Получаем указатель на атрибут
	IXMLDOMElementPtr p_node_el = el;
	HRESULT hr = p_node_el->getAttribute( CComBSTR( cstrAttribute ), &value );

	if (fail( hr ))
		return FALSE;

	return TRUE;
}

BOOL SimpleXML::SetAttributeValue( hXml el, CString cstrAttribute, VARIANT value )
{
	if (null_or_null( file, el ))
		return FALSE;

	// Получаем указатель на атрибут
	IXMLDOMElementPtr p_node_el = el;
	HRESULT hr = p_node_el->setAttribute( CComBSTR( cstrAttribute ), value );

	if (fail( hr ))
		return FALSE;

	return TRUE;
}
