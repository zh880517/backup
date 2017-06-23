#pragma once
#include <memory>
#include "SharePtrDef.h"
class TCPConnection;
using TCPConnectionPtr = std::shared_ptr<TCPConnection>;

class TCPSession
{
public:
	TCPSession();
	~TCPSession();

	void DisConnect();

	void SetConnection(TCPConnectionPtr& pConn);

private:
	TCPConnectionPtr m_pConnection;
	PacketQueue		 m_PacketQueue;
};

