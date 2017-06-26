#pragma once
#include <functional>
#include "SharePtrDef.h"
#include "IPacketHead.h"

class TCPConnectPool
{
public:
	TCPConnectPool(IOContext* pContext, IPacketHead* pHead);
	~TCPConnectPool();

public:

	std::function<void(TCPConnectionPtr, std::string*)> OnRecivePacket;

	std::function<void(TCPConnectionPtr pConnect, std::error_code ec)> OnConnectiontError;

	virtual void Stop() = 0;

	size_t CalDateLen(const char* pHead, size_t len);


protected:
	IPacketHead* m_pHead;
	IOContext*	m_pContext;
};

