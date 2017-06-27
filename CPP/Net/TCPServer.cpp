#include "TCPServer.h"
#include "NetInclude.h"
#include "TCPConnection.h"
#include <iostream>


TCPServer::TCPServer(IPacketHead* pHead, uint16_t iPort, uint32_t iThreadNum)
	: TCPConnectPool(pHead)
	, m_iPort(iPort)
	, m_iThreadNum(iThreadNum)
{
	m_pContext = new IOContext;
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
	for (uint32_t i=0; i<m_iThreadNum; ++i)
	{
		std::thread* pThread = new std::thread([this](IOContext* pCon) 
		{
			while (true)
			{
				try
				{
					pCon->Context.run();
					break;
				}
				catch (std::exception& e)
				{
					std::cout << e.what() << std::endl;
				}
				catch (...)
				{

				}
			}
		}, m_pContext);
		m_Threads.push_back(pThread);
	}
	DoAccept();
}

void TCPServer::Stop()
{
	if (m_pAcceptor != nullptr)
	{
		m_pAcceptor->acceptor.close();
		delete m_pAcceptor;
		m_pAcceptor = nullptr;
		for (auto pThread : m_Threads)
		{
			pThread->join();
			delete pThread;
		}
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
