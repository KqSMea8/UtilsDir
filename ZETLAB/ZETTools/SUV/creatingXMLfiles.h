#pragma once
#include <pugixml/include/pugiautolink.hpp>

using namespace pugi;

class creatingXMLfiles
{
public:
	creatingXMLfiles();
	~creatingXMLfiles();

	//----------- Методы --------------
public:
	bool			saveDocument();
	void			setRootNode( xml_node rootNode ){ m_rootNode = rootNode; };
	xml_node		getRootNode(){ return m_rootNode; };

	//Создание документа
	void			createDocument(CString _XmlFileName);

	xml_node		addRootNode(CString nodeNameRoot);
	void			addChildNode(CString nodeNameRoot);

	template <class Tnode, class Tattr >
	void addNode(CString nodeName, Tnode value, CString attr = L"", Tattr valueAtrr = CString)
	{
		xml_node childNode = m_rootNode.append_child(nodeName);

		childNode.text().set(value);

		if (!attr.IsEmpty())
			childNode.append_attribute(attr) = valueAtrr;
	}

	void			addComment(CString comment);
	//Чтение
	bool			openDocument(CString _XmlFileName);
	xml_node		readRootNode(CString nodeNameRoot);
	xml_node		readChildNode(CString nodeNameRoot);


	CString			readNodeAsCString(CString nodeName, CString attr = _T(""), int type = 0){ return readNode(nodeName, attr, type); }
	float			readNodeAsFloat(CString nodeName, CString attr = _T(""), int type = 0){ return (float)_wtof(readNode(nodeName, attr, type)); }
	UINT			readNodeAsUINT(CString nodeName, CString attr = _T(""), int type = 0){ return (UINT)_wtof(readNode(nodeName, attr, type)); }

	CString			getAttrAsCString(){ return m_cAttr_value; }
	float			getAttrAsFloat(){ return m_fAttr_value; }
	int				getAttrAsUINT(){ return m_iAttr_value; }

private:
	void			generateHeader();
	CString			readNode(CString nodeName, CString attr =_T(""), int type = 0);
//----------- Поля --------------
public:
private:
	xml_node		m_rootNode;
	xml_document	m_hFile;
	CString			m_XmlFileName;
	CString			m_cAttr_value;
	int				m_iAttr_value;
	float			m_fAttr_value;
};