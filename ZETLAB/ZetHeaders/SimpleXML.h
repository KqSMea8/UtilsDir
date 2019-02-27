// (C) Ильназ Низаметдинов, ЗАО "ЭТМС"
// Все права защищены.
//
// Класс для работы с XML-файлами.
// Выступает в роли посредника между приложением и встроенной технологией MSXML

#pragma once

typedef IXMLDOMNodePtr hXml;

class SimpleXML
{
public:
	SimpleXML();
	SimpleXML( CString fileName );
	~SimpleXML();

	// Открывает xml-файл
	BOOL Load( CString fileName );

	// Сохраняет данные в xml-файл
	BOOL Save( CString fileName );

	// Создаем xml с корневым каталогом rootName
	hXml Create( CString rootName );

	// Возвращает имя xml-элемента
	CString GetName( hXml el );

	// Возвращает корневой элемент документа
	hXml GetRootNode();

	// Читает значение элемента el, сохраняет в value
	CString GetValue( hXml el );

	// Устанавливаем значение value, элемента el
	BOOL SetValue( hXml el, CString strValue );
	BOOL SetValue( hXml el, bool boolValue );
	BOOL SetValue( hXml el, int intValue );
	BOOL SetValue( hXml el, double dblValue );

	// Читает атрибут attribute, элемента el, сохраняет значение в value
	BOOL GetAttributeValue( hXml el, CString attribute, VARIANT& value );

	// Устанавливаем значение value, атрибута attribute, элемента el
	BOOL SetAttributeValue( hXml el, CString attribute, VARIANT value );

	// Возвращает дочерний элемент
	hXml GetChildNode( hXml el );

	// Возвращает элемент, слудеющий за el
	hXml GetNextNode( hXml el );

	// Возвращает родитель элемента
	hXml GetParent( hXml el );

	// Добавляет элемент в xml-документ
	hXml AddElement( CString name, hXml parent );

	// Возвращает элемент с именем name
	hXml GetNode( CString name, hXml parent );

private:
	// Указатель на открытый/созданный xml-документ
	IXMLDOMDocumentPtr file;
};
