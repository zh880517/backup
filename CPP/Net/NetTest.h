#pragma once
#include <map>
#include "IPacketHead.h"
#include "TCPServer.h"
#include "TCPClient.h"

class ShortHead : public IPacketHead
{
public:

	virtual size_t HeadLen() override;

	virtual size_t GetDataLen(const char * pHead, size_t iHeadLen) override;

};


class ServerTest
{
public:
	ServerTest(IOContext* pContex);
	~ServerTest() = default;

protected:
	void OnRecive(TCPConnectionPtr pConn, std::string* pPacket);

	void OnError(TCPConnectionPtr pConnect, std::error_code ec);

private:
	TCPServer m_Server;
	ShortHead m_Head;
	NetPacket m_SendPacket;
	//std::map<TCPConnectionPtr, TCPSession*> m_Session;
};
struct ClientRecord
{
	uint32_t SendNum = 0;
	uint32_t Index = 0;
};
class ClientTest
{
public:
	ClientTest(IOContext* pContex);
	~ClientTest();

protected:

	void OnConnected(TCPSession* pSession);

	void OnRecive(TCPConnectionPtr pConn, std::string* pPacket);

	void OnError(TCPConnectionPtr pConnect, std::error_code ec);

private:
	TCPClient m_Client;
	ShortHead m_Head;
	NetPacket m_SendPacket;
	std::mutex m_Mutex;
	std::map<TCPSession*, ClientRecord> m_Sessions;
	const static uint32_t MaxClient = 20000;
};


