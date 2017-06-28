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
	Net::ip::tcp::acceptor::keep_alive option(true);
	m_pAcceptor->acceptor.set_option(option);
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
		if (error)
		{
			std::cout << error << std::endl;
			return;
		}
		TCPConnectionPtr pConnect = std::make_shared<TCPConnection>(this, peer, m_pHead->HeadLen());
		pConnect->TryRecive();
		DoAccept();
	});
}
