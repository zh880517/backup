#include "TCPSession.h"
#include "TCPConnection.h"


TCPSession::TCPSession()
{
}


TCPSession::~TCPSession()
{
}

void TCPSession::DisConnect()
{
}

void TCPSession::SetConnection(TCPConnectionPtr& pConn)
{
	m_pConnection = pConn;
	if (m_pConnection)
	{
		m_pConnection->TryRecive();
		m_pConnection->SetPacketQueue(&m_PacketQueue);
		m_pConnection->TrySend();
	}
}
