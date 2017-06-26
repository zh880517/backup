#include "TCPClient.h"
#include "NetInclude.h"
#include "TCPSession.h"
#include "TCPConnection.h"

TCPClient::TCPClient(IOContext* pContext, IPacketHead* pHead)
	: TCPConnectPool(pContext, pHead)
{
}


TCPClient::~TCPClient()
{
}

void TCPClient::Stop()
{
}

void TCPClient::Connect(const std::string & strIP, uint16_t iPort, TCPSession * pSession)
{
	Net::ip::tcp::endpoint ep(Net::ip::make_address(strIP), iPort);
	Net::ip::tcp::socket newSocket(m_pContext->Context);
	TCPConnectionPtr pConnect = std::make_shared<TCPConnection>(this, newSocket, m_pHead->HeadLen());
	pConnect->SetSession(pSession);
	newSocket.async_connect(ep, [this, &pConnect, pSession](const std::error_code& error) 
	{
		if (error)
			OnConnectiontError(pConnect, error);
		pSession->SetConnection(pConnect);
	});
}
