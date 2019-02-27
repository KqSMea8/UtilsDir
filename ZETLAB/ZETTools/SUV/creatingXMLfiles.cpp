#include "stdafx.h"
#include "creatingXMLfiles.h"


creatingXMLfiles::creatingXMLfiles()
{
	m_cAttr_value =	_T("");
	m_iAttr_value = 0;
	m_fAttr_value = 0;
};


creatingXMLfiles::~creatingXMLfiles()
{
}

void creatingXMLfiles::createDocument(CString _XmlFileName)
{
	m_XmlFileName = _XmlFileName;

	generateHeader();
}
void creatingXMLfiles::generateHeader()
{
	auto declarationNode = m_hFile.append_child(node_declaration);
	declarationNode.append_attribute(L"version") = L"1.0";
	declarationNode.append_attribute(L"encoding") = L"ccs=UTF-8";
}
xml_node creatingXMLfiles::addRootNode(CString nodeNameRoot)
{
	return m_rootNode = m_hFile.append_child(nodeNameRoot);
}
void creatingXMLfiles::addChildNode(CString nodeNameRoot)
{
	m_rootNode = m_rootNode.append_child(nodeNameRoot);
}

void creatingXMLfiles::addComment(CString comment)
{
	xml_node nComment = m_rootNode.append_child(node_comment);

	nComment.set_value(comment);
}

bool creatingXMLfiles::openDocument(CString _XmlFileName)
{
	m_XmlFileName = _XmlFileName;

	return m_hFile.load_file(_XmlFileName, format_default, encoding_utf8);

}
xml_node creatingXMLfiles::readRootNode(CString nodeNameRoot)
{
	return m_rootNode = m_hFile.child(nodeNameRoot);
}
xml_node creatingXMLfiles::readChildNode(CString nodeNameRoot)
{
	return m_rootNode = m_rootNode.child(nodeNameRoot);
}
CString creatingXMLfiles::readNode(CString nodeName, CString attr,int type)
{
	xml_node node;
	for (node = m_rootNode.first_child(); node; node = node.next_sibling())
		if (node.name() == nodeName)
		{
			if (!attr.IsEmpty())
			{
				switch (type)
				{
				case 0: m_cAttr_value = node.attribute(attr).as_string(); break;

				case 1: m_fAttr_value = node.attribute(attr).as_float(); break;

				case 2: m_iAttr_value = node.attribute(attr).as_int(); break;

				default:
					break;
				}
				
			}

			return node.text().get();
		}

	return L"";
}
bool creatingXMLfiles::saveDocument()
{
	return m_hFile.save_file(m_XmlFileName, PUGIXML_TEXT("  "));
}
