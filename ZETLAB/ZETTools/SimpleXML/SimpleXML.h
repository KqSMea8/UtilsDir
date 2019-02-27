// (C) Ильназ Низаметдинов, ЗАО "ЭТМС"
// Все права защищены.
//
// Класс для работы с XML-файлами.
// Выступает в роли посредника между приложением и встроенной технологией MSXML

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

	// Открывает xml-файл
	BOOL Load( CString cstrFileName );

	// Парсит входную строку как xml-файл
	BOOL LoadIndirect( BSTR bstrText );

	// Сохраняет данные в xml-файл
	BOOL Save( CString cstrFileName );

	// Создаем xml с корневым каталогом rootName
	hXml Create( CString cstrRootName );

	// Возвращает корневой элемент документа
	hXml GetRootNode();

	// Возвращает родитель элемента
	hXml GetParent( hXml el );

	// Возвращает дочерний элемент
	hXml GetChildNode( hXml el );

	// Возвращает элемент, слудеющий за el
	hXml GetNextNode( hXml el );

	// Добавляет элемент в xml-документ
	hXml AddElement( CString cstrName, hXml parent );

	// Удаляет элемент из xml-документа
	BOOL RemoveElement( CString cstrName, hXml parent );
	BOOL RemoveElement( hXml el, hXml parent );

	// Возвращает имя xml-элемента
	CString GetName( hXml el );

	// Возвращает элемент с именем name
	hXml GetNode( CString cstrName, hXml parent );

	// Возвращает строковое значение элемента el
	CString GetValue( hXml el );

	// Устанавливаем значение value, элемента el
	BOOL SetValue( hXml el, CString cstrValue );
	BOOL SetValue( hXml el, bool boolValue );
	BOOL SetValue( hXml el, int intValue );
	BOOL SetValue( hXml el, double dblValue );

	// Читает атрибут attribute, элемента el, сохраняет значение в value
	BOOL GetAttributeValue( hXml el, CString cstrAttribute, VARIANT& value );

	// Устанавливаем значение value, атрибута attribute, элемента el
	BOOL SetAttributeValue( hXml el, CString cstrAttribute, VARIANT value );

private:
	// Указатель на открытый/созданный xml-документ
	IXMLDOMDocumentPtr file;
};
