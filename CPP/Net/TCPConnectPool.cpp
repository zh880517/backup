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
