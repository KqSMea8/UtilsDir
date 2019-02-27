#pragma once

#include <open62541\open62541.h>
#include <ZETSimple\include\singleton\singleton.h>
#include <ZETSimple\include\singleton\singleton_auto_link.h>

#include <map>
#include <stdint.h>

class COpcUaNodeIdStorageImpl
{
public:
	COpcUaNodeIdStorageImpl();
	~COpcUaNodeIdStorageImpl();

	uint64_t put(_In_ const UA_NodeId& nodeId);
	uint64_t get(_In_ const UA_NodeId& nodeId);
	void get(_In_ const uint64_t id, _Out_ UA_NodeId& nodeId);
	void clear();

protected:
	std::map<uint64_t, UA_NodeId> m_nodes;

private:
	
};

typedef CSingleton<COpcUaNodeIdStorageImpl> COpcUaNodeIdStorage;