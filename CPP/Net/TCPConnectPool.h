#pragma once
#include "SharePtrDef.h"
#include "IPacketHead.h"

class TCPConnectPool
{
public:
	TCPConnectPool(IOContext* pContext, IPacketHead* pHead);
	~TCPConnectPool();


public:

	virtual void Stop() = 0;

	void OnConnectiontError(TCPConnection* pConnect, std::error_code ec);

	void OnRecivePacket(TCPConnection* pConnect, std::string* pPacket);

	size_t CalDateLen(const char* pHead, size_t len);


protected:
	IPacketHead* m_pHead;
	IOContext*	m_pContext;
};

