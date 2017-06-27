#include "NetTest.h"
#include "TCPSession.h"
#include "TCPConnection.h"
#include <iostream>

size_t ShortHead::HeadLen()
{
	return 2;
}

size_t ShortHead::GetDataLen(const char * pHead, size_t iHeadLen)
{
	size_t iLen = (pHead[0] &0xFF | pHead[1] << 8);
	return iLen;
}

ServerTest::ServerTest()
	:m_Server(&m_Head, 12345, 8)
{
	m_Server.OnRecivePacket = std::bind(&ServerTest::OnRecive, this, std::placeholders::_1, std::placeholders::_2);
	m_Server.OnConnectiontError = std::bind(&ServerTest::OnError, this, std::placeholders::_1, std::placeholders::_2);

	m_SendPacket = std::make_shared<std::string>();
	m_SendPacket->push_back((uint8_t)1024);
	m_SendPacket->push_back((uint8_t)(1024 >> 8));
	for (int i=0; i< 1024; ++i)
	{
		m_SendPacket->push_back((uint8_t)i);
	}

	m_Server.Start();
}

void ServerTest::OnRecive(TCPConnectionPtr pConn, std::string * pPacket)
{
	TCPSession* pSession = pConn->Session();
	if (pSession == nullptr)
	{
		pSession = new TCPSession();
		pSession->SetConnection(pConn);
	}
	//std::cout << *pPacket << std::endl;
	delete pPacket;
	pSession->SendPacket(m_SendPacket);
}

void ServerTest::OnError(TCPConnectionPtr pConnect, std::error_code ec)
{
	if (ec.value() != 10009)
	{
		std::cout << ec << std::endl;
	}
	TCPSession* pSession = pConnect->Session();
	pConnect->SetSession(nullptr);
	if (pSession != nullptr)
	{
		delete pSession;
	}
}

ClientTest::ClientTest(IOContext * pContex)
	:m_Client(pContex, &m_Head)
{
	m_Client.OnRecivePacket = std::bind(&ClientTest::OnRecive, this, std::placeholders::_1, std::placeholders::_2);
	m_Client.OnConnectiontError = std::bind(&ClientTest::OnError, this, std::placeholders::_1, std::placeholders::_2);
	m_Client.OnConnected = std::bind(&ClientTest::OnConnected, this, std::placeholders::_1);

	m_SendPacket = std::make_shared<std::string>();
	m_SendPacket->push_back((uint8_t)1024);
	m_SendPacket->push_back((uint8_t)(1024 >> 8));
	for (int i = 0; i < 1024; ++i)
	{
		m_SendPacket->push_back((uint8_t)i);
	}

	for (size_t i=0; i< MaxClient; ++i)
	{
		TCPSession* pSession = new TCPSession;
		m_Client.Connect("127.0.0.1", 12345, pSession);
		ClientRecord record;
		record.Index = i;
		m_Sessions.insert(std::make_pair(pSession, record));
		std::this_thread::sleep_for(std::chrono::microseconds(10));
	}
}

ClientTest::~ClientTest()
{
}

void ClientTest::OnConnected(TCPSession * pSession)
{
	pSession->SendPacket(m_SendPacket);
	m_Sessions[pSession].SendNum++;
}

void ClientTest::OnRecive(TCPConnectionPtr pConn, std::string * pPacket)
{
	delete pPacket;
	auto pSession = pConn->Session();
	if (pSession == nullptr)
	{
		std::cout << "Error !!!" << std::endl;
		return;
	}
	auto it = m_Sessions.find(pSession);
	if (it != m_Sessions.end())
	{
		if (it->second.SendNum > 100)
		{
			pSession->DisConnect();
			return;
		}
		pSession->SendPacket(m_SendPacket);
		it->second.SendNum++;
		pSession->SendPacket(m_SendPacket);
		it->second.SendNum++;
		pSession->SendPacket(m_SendPacket);
		it->second.SendNum++;
	}

}

void ClientTest::OnError(TCPConnectionPtr pConnect, std::error_code ec)
{
	if (ec.value() != 10009)
	{
		//std::cout << ec << std::endl;
	}
	auto pSession = pConnect->Session();
	if (pSession == nullptr)
		return;
	
	auto it = m_Sessions.find(pSession);
	if (it != m_Sessions.end() /*&& it->second.SendNum < 103*/)
	{
		std:: cout << "Close Client : Send Num = " << it->second.SendNum << " Index = "<< it->second.Index << ec << std::endl;
	}
	pConnect->SetSession(nullptr);
	delete pSession;
	m_Sessions.erase(pSession);
}
