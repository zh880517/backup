#include "TCPClient.h"
#include "NetInclude.h"
#include "TCPSession.h"
#include "TCPConnection.h"
#include <iostream>

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
	TCPConnectionPtr pConnect = std::make_shared<TCPConnection>(this, Net::ip::tcp::socket(m_pContext->Context), m_pHead->HeadLen());
	pConnect->SetSession(pSession);
	pConnect->Socket().async_connect(ep, [this, pConnect](const std::error_code& error) 
	{
		if (error)
		{
			OnConnectiontError(pConnect, error);
			return;
		}
		auto pSess = pConnect->Session();
		if (pSess != nullptr)
		{
			pSess->SetConnection(pConnect);
			if (OnConnected)
			{
				OnConnected(pSess);
			}
		}
	});
}
