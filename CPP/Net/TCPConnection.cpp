#include "TCPConnection.h"
#include "TCPConnectPool.h"

TCPConnection::TCPConnection(TCPConnectPool* connPool, Net::ip::tcp::socket& socket, size_t buffLen)
	: m_Socket(std::move(socket))
	, m_ConnPool(connPool)
	, m_iBufferLen(buffLen)
{
	m_pBuffer = new char[buffLen];
}

TCPConnection::~TCPConnection()
{
	if (m_pBuffer != nullptr)
		delete[] m_pBuffer;
}


void TCPConnection::TryRecive()
{
	std::lock_guard<std::mutex> lock(m_SendMutex);
	if (m_bReading)
		return;
	m_bReading = true;
	ReadHead();
}

void TCPConnection::Close()
{
	std::error_code ec;
	m_Socket.shutdown(Net::socket_base::shutdown_both, ec);
	m_Socket.close(ec);
}

void TCPConnection::SetPacketQueue(PacketQueue * queue)
{
	std::lock_guard<std::mutex> lock(m_SendMutex);
	m_Packets = queue;
}

void TCPConnection::TrySend()
{
	std::lock_guard<std::mutex> lock(m_SendMutex);
	if (m_Packets == nullptr || m_pCurSendPacket)
		return ;
	if (m_Packets != nullptr && m_Packets->Peek(m_pCurSendPacket) && m_pCurSendPacket != nullptr)
		Send();
}

void TCPConnection::SetSession(TCPSession * pSession)
{
	std::lock_guard<std::mutex> lock(m_SendMutex);
	 m_pSession = pSession; 
	 if (pSession == nullptr)
	 {
		 m_Packets = nullptr;
	 }
}

void TCPConnection::ReadHead()
{
	auto self(shared_from_this());
	m_Socket.async_receive(Net::buffer(m_pBuffer + m_iReadBytes, m_iBufferLen - m_iReadBytes), [this, self](std::error_code er, size_t bytes_transferred) 
	{
		if (er || bytes_transferred == 0)
		{
			m_ConnPool->OnError(self, er);
			return;
		}

		m_iReadBytes += bytes_transferred;
		if (m_iReadBytes < m_iBufferLen)
		{
			ReadHead();
		}
		else
		{
			size_t len = m_ConnPool->CalDateLen(m_pBuffer, m_iBufferLen);
			m_pCurRecivePacket = new std::string(len + m_iBufferLen, (char)0);
			m_pCurRecivePacket->append(m_pBuffer, m_iBufferLen);
			m_pCurRecivePacket->resize(len + m_iBufferLen);
			ReadBody();
		}

	});
}

void TCPConnection::ReadBody()
{
	auto self(shared_from_this());
	m_Socket.async_receive(Net::buffer(&(m_pCurRecivePacket->at(0)) + m_iReadBytes, m_pCurRecivePacket->size() - m_iReadBytes), [this, self](std::error_code er, size_t bytes_transferred)
	{
		if (er || bytes_transferred == 0)
		{
			m_ConnPool->OnError(self, er);
			return;
		}

		m_iReadBytes += bytes_transferred;
		if (m_iReadBytes < m_pCurRecivePacket->size())
		{
			ReadBody();
		}
		else
		{
			m_iReadBytes = 0;
			m_ConnPool->OnRecive(self, m_pCurRecivePacket);
			m_pCurRecivePacket = nullptr;
			ReadHead();
		}
});
}

void TCPConnection::Send()
{
	auto self(shared_from_this());
	m_Socket.async_send(Net::buffer(m_pCurSendPacket->data() + m_iWriteBytes, m_pCurSendPacket->size() - m_iWriteBytes),
		[this, self](std::error_code er, size_t bytes_transferred) 
	{
		if (er || bytes_transferred == 0)
		{
			m_ConnPool->OnError(self, er);
			return;
		}
		std::lock_guard<std::mutex> lock(m_SendMutex);
		m_iWriteBytes += bytes_transferred;
		do 
		{
			if (m_iWriteBytes < m_pCurSendPacket->size())
				break;
			m_iWriteBytes = 0;
			m_pCurSendPacket.reset();
			if (m_Packets != nullptr && m_Packets->Peek(m_pCurSendPacket) && m_pCurSendPacket != nullptr)
				break;
			return;
		} while (false);
		Send();
	});
}
