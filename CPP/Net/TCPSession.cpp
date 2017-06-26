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
	std::lock_guard<std::mutex> lock(m_Mutex);
	if (m_pConnection)
	{
		m_pConnection->Close();
		m_pConnection.reset();
	}
}

bool TCPSession::IsConnection()
{
	return (bool)m_pConnection;
}

void TCPSession::SetConnection(TCPConnectionPtr& pConn)
{
	std::lock_guard<std::mutex> lock(m_Mutex);
	m_pConnection = pConn;
	if (m_pConnection)
	{
		pConn->SetSession(this);
		m_pConnection->TryRecive();
		m_pConnection->SetPacketQueue(&m_PacketQueue);
		m_pConnection->TrySend();
	}
}

void TCPSession::SendPacket(const NetPacket & packet)
{
	m_PacketQueue.Push(packet);
	if (m_pConnection)
	{
		m_pConnection->TrySend();
	}
}

void TCPSession::SendPackets(const std::list<NetPacket>& packets)
{
	m_PacketQueue.Push(packets);
	if (m_pConnection)
	{
		m_pConnection->TrySend();
	}
}
