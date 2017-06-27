#pragma once
#include <memory>
#include <mutex>

#include "SharePtrDef.h"
class TCPConnection;
using TCPConnectionPtr = std::shared_ptr<TCPConnection>;

class TCPSession
{
public:
	TCPSession();
	~TCPSession();

	void DisConnect();

	bool IsConnection();

	void SetConnection(const TCPConnectionPtr& pConn);

	void SendPacket(const NetPacket& packet);

	void SendPackets(const std::list<NetPacket>& packets);

private:
	TCPConnectionPtr m_pConnection;
	PacketQueue		 m_PacketQueue;
	std::mutex		 m_Mutex;
};

