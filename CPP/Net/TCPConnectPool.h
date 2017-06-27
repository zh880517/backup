#pragma once
#include <functional>
#include "SharePtrDef.h"
#include "IPacketHead.h"

class TCPConnectPool
{
public:
	TCPConnectPool(IPacketHead* pHead);
	~TCPConnectPool();

public:

	std::function<void(TCPConnectionPtr, std::string*)> OnRecivePacket;

	std::function<void(TCPConnectionPtr , std::error_code )> OnConnectiontError;

	virtual void Stop() = 0;

	size_t CalDateLen(const char* pHead, size_t len);

	void OnRecive(const TCPConnectionPtr& pConnect, std::string* pPacket);

	void OnError(const TCPConnectionPtr& pConnect, std::error_code ec);



protected:
	IPacketHead* m_pHead;
	std::mutex m_Mutex;
};

