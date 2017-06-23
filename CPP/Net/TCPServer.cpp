#include "TCPServer.h"
#include "NetInclude.h"
#include "TCPConnection.h"
#include <iostream>


TCPServer::TCPServer(IOContext* pContext, IPacketHead* pHead, uint16_t iPort)
	: TCPConnectPool(pContext, pHead)
	, m_iPort(iPort)
{
	
}


TCPServer::~TCPServer()
{
	Stop();
}

void TCPServer::Start()
{
	if (m_pAcceptor != nullptr)
	{
		Stop();
	}
	m_pAcceptor = new Acceptor(m_pContext->Context, m_iPort);
	DoAccept();
}

void TCPServer::Stop()
{
	if (m_pAcceptor != nullptr)
	{
		m_pAcceptor->acceptor.close();
		delete m_pAcceptor;
		m_pAcceptor = nullptr;
	}
}

void TCPServer::DoAccept()
{
	m_pAcceptor->acceptor.async_accept([this](const std::error_code& error, Net::ip::tcp::socket peer)
	{
		if (!error)
		{
			TCPConnectionPtr pConnect = std::make_shared<TCPConnection>(this, peer, m_pHead->HeadLen());
// 			std::cout << peer.remote_endpoint() << std::endl;
// 			static std::string str = "Hello!!!";
// 			peer.async_send(Net::buffer(str, str.size()), [](const std::error_code& error, std::size_t bytes_transferred)
// 			{
// 				std::cout << error << std::endl;
// 				std::cout << bytes_transferred << std::endl;
// 			});
		}
		else
		{
			std::cout << error << std::endl;
		}
		DoAccept();
	});
}
