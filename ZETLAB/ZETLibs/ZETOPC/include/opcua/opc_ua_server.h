#pragma once

#include "opc_ua_node.h"

#include <open62541\open62541.h>

#include <windows.h>
#include <string>
#include <vector>
#include <memory>

class COpcUaServer
{
public:
#pragma region Constructors and destructor
	COpcUaServer();
	~COpcUaServer();
#pragma endregion

#pragma region Methods
	void start();
	void stop();
	void restart();

	void addNamespace(_In_ const std::wstring sNamespace);
	const COpcUaNode* addObjectNode(_In_ const uint16_t namespaceIndex, _In_ const UA_NodeIdType nodeIdType,
		_In_ const UA_Variant& nodeIdId, _In_ const uint64_t parentNode, _In_ const std::wstring& sBrowseName,
		_In_ const std::wstring& sDisplayName, _In_ const std::wstring& sDescription);
	const COpcUaNode* addVariableNode(_In_ const uint16_t namespaceIndex, _In_ const UA_NodeIdType nodeIdType,
		_In_ const UA_Variant& nodeIdId, _In_ const uint64_t parentNode, _In_ const std::wstring& sBrowseName,
		_In_ const std::wstring& sDisplayName, _In_ const std::wstring& sDescription, _In_ const uint16_t dataType);

	const COpcUaNode* getObjectsNode() const;
	const COpcUaNode* getNode(_In_ const uint64_t id) const;
	const COpcUaNode* getNode(_In_ const UA_NodeId& nodeId) const;

	UA_ServerState getState() const;
		
	UA_DateTimeStruct getCurrentTime() const;
	void getNodeValue(_In_ const uint64_t id, _Out_ UA_Variant& value);

#pragma endregion

#pragma region Properties
	void setPort(_In_ const uint16_t port);
	uint16_t getPort() const;
	void setApplicationName(_In_ const std::wstring sApplicationName);
	std::wstring getApplicationName() const;
	void setApplicationUri(_In_ const std::wstring sApplicationUri);
	std::wstring getApplicationUri() const;
	void setProductUri(_In_ const std::wstring sProductUri);
	std::wstring getProductUri() const;

	void getNamespaces(_Out_ std::vector<std::wstring>& namespaces) const;
	void getNodeClasses(_Out_ std::vector<UA_NodeClass>& nodeClasses) const;
	void getNodeIdTypes(_Out_ std::vector<UA_NodeIdType>& nodeidTypes) const;
	void getDataTypes(_Out_ std::vector<uint16_t>& dataTypes) const;
#pragma endregion

protected:
	uint16_t m_port;
	std::wstring m_sApplicationName;
	std::wstring m_sApplicationUri;
	std::wstring m_sProductUri;

private:
	static DWORD WINAPI serverThread(LPVOID pParam);
	DWORD threadFunction();

	void initServerConfig(_Inout_ UA_ServerConfig& config);
	void clearServerConfig();
	void browse();
	void browse(_In_ UA_NodeId& nodeId, _In_ COpcUaNode* pNode);

	void updateNode(_In_ COpcUaNode* pNode);
	void updateNodeClass(_In_ COpcUaNode* pNode);
	void updateBrowseName(_In_ COpcUaNode* pNode);
	void updateDisplayName(_In_ COpcUaNode* pNode);
	void updateDescription(_In_ COpcUaNode* pNode);
	void updateDataType(_In_ COpcUaNode* pNode);
	void updateValueRank(_In_ COpcUaNode* pNode);

	UA_Server* m_pServer;
	UA_LocalizedText* m_pApplicationName;
	UA_String* m_pApplicationUri;
	UA_String* m_pProductUri;

	std::shared_ptr<COpcUaNode> m_objects;

	HANDLE m_hThread;
	bool m_bThread;
};

