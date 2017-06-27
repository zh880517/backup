#pragma once
#include "TCPConnectPool.h"


class TCPServer : public TCPConnectPool
{
public:
	TCPServer(IPacketHead* pHead, uint16_t iPort, uint32_t iThreadNum = 1);
	~TCPServer();

	void Start();

	virtual void Stop() override;

private:

	void DoAccept();
private:
	uint16_t	m_iPort;
	uint32_t	m_iThreadNum;
	Acceptor*	m_pAcceptor;
	IOContext*  m_pContext;
	std::vector<std::thread*> m_Threads;
};

