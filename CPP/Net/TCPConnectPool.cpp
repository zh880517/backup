#include "TCPConnectPool.h"
#include "NetInclude.h"


TCPConnectPool::TCPConnectPool(IOContext* pContext, IPacketHead* pHead)
	: m_pContext(pContext)
	, m_pHead(pHead)
{

}


TCPConnectPool::~TCPConnectPool()
{
}

size_t TCPConnectPool::CalDateLen(const char * pHead, size_t len)
{
	return m_pHead->GetDataLen(pHead, len);
}

void TCPConnectPool::OnRecive(const TCPConnectionPtr & pConnect, std::string * pPacket)
{
	if (OnRecivePacket)
	{
		OnRecivePacket(pConnect, pPacket);
	}
}

void TCPConnectPool::OnError(const TCPConnectionPtr & pConnect, std::error_code ec)
{
	std::lock_guard<std::mutex> lock(m_Mutex);
	if (OnConnectiontError)
	{
		OnConnectiontError(pConnect, ec);
	}
}

