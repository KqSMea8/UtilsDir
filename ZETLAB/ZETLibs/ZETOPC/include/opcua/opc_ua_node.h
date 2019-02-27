#pragma once

#include <open62541\open62541.h>

#include <list>
#include <memory>

class COpcUaNode
{
public:
	COpcUaNode(_In_ const UA_NodeId& nodeId);
	~COpcUaNode();

	bool isObjectsNode() const;

	void getNodeId(_Out_ UA_NodeId& nodeId) const;
	void setNodeClass(_In_ const UA_NodeClass nodeClass);
	UA_NodeClass getNodeClass() const;
	void setBrowseName(_In_ const std::wstring sBrowseName);
	std::wstring getBrowseName() const;
	void setDisplayName(_In_ const std::wstring sDisplayName);
	std::wstring getDisplayName() const;
	void setDescription(_In_ const std::wstring sDescription);
	std::wstring getDescription() const;
	void setDataType(_In_ const uint16_t dataType);
	uint16_t getDataType() const;
	void setValueRank(_In_ const int32_t valueRank);
	int32_t getValueRank() const;

	uint32_t getNamespaceIndex() const;

	uint64_t getId() const;
	const COpcUaNode* getNode(_In_ const uint64_t id) const;

	std::list<std::shared_ptr<COpcUaNode>> m_nodes;

protected:
	UA_NodeClass m_nodeClass;
	std::wstring m_sBrowseName;
	std::wstring m_sDisplayName;
	std::wstring m_sDescription;
	uint16_t m_dataType;
	int32_t m_valueRank;

	uint16_t m_namespaceIndex;
	UA_NodeId m_nodeId;
};

