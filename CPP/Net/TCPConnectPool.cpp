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

void TCPConnectPool::OnConnectiontError(TCPConnection * pConnect, std::error_code ec)
{
}

void TCPConnectPool::OnRecivePacket(TCPConnection * pConnect, std::string * pPacket)
{
}

size_t TCPConnectPool::CalDateLen(const char * pHead, size_t len)
{
	return m_pHead->GetDataLen(pHead, len);
}
