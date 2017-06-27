#pragma once
#include "TCPConnectPool.h"

class TCPClient : public TCPConnectPool
{
public:
	TCPClient(IOContext* pContext, IPacketHead* pHead);
	~TCPClient();

	// ͨ�� TCPConnectPool �̳�
	virtual void Stop() override;

	void Connect(const std::string& strIP, uint16_t iPort, TCPSession* pSession);

	std::function<void(TCPSession*)> OnConnected;
private:

	IOContext*	m_pContext;
};

