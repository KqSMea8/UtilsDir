#pragma once
#include <open62541\open62541.h>

#include <string>

class COpcUaFunctionHelper
{
public:
	static std::wstring getNodeClassDescription(_In_ const UA_NodeClass nodeClass);
	static std::wstring getNodeIdTypeDescription(_In_ const UA_NodeIdType nodeIdType);
	static std::wstring getStateDescription(_In_ const UA_ServerState state);
	static std::wstring getDataTypeDescription(_In_ const uint16_t dataType);
	static std::wstring getValueRankDescription(_In_ const int32_t valueRank);
	static std::wstring getValue(_In_ const UA_Variant& value);
		
	static bool w2c(_In_ const wchar_t* pSrc, _In_ const uint32_t srcLength, _In_ char* pDst, _In_ const uint32_t dstLength);
	static bool c2w(_In_ const char* pSrc, _In_ const uint32_t srcLength, _In_ wchar_t* pDst, _In_ const uint32_t dstLength);

protected:
	COpcUaFunctionHelper();
	~COpcUaFunctionHelper();
};

