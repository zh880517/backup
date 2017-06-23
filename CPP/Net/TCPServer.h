#pragma once
#include "TCPConnectPool.h"


class TCPServer : public TCPConnectPool
{
public:
	TCPServer(IOContext* pContext, IPacketHead* pHead, uint16_t iPort);
	~TCPServer();

	void Start();

	virtual void Stop() override;

private:

	void DoAccept();
private:
	uint16_t	m_iPort;
	Acceptor*	m_pAcceptor;
};

