#include "TCPConnection.h"


TCPConnection::TCPConnection(Net::ip::tcp::socket socket, uint32_t buffLen)
	:m_Socket(std::move(socket)),m_iBufferLen(buffLen)
{
	m_pBuffer = new char[buffLen];
	ReadHead();
}

TCPConnection::~TCPConnection()
{
	if (m_pBuffer != nullptr)
		delete[] m_pBuffer;
}

void TCPConnection::Close()
{
	std::error_code ec;
	m_Socket.shutdown(Net::socket_base::shutdown_both, ec);
}

void TCPConnection::ReadHead()
{
	auto self(shared_from_this());
	m_Socket.async_receive(Net::buffer(m_pBuffer + m_iReadBytes, m_iBufferLen - m_iReadBytes), [this, self](std::error_code er, size_t bytes_transferred) 
	{
		if (er)
		{
			m_iReadBytes += bytes_transferred;
			if (m_iReadBytes < m_iBufferLen)
			{
				ReadHead();
				return;
			}
			size_t len = GetDataLen(m_pBuffer, m_iBufferLen);
			m_pCurPacket = new std::string(len + m_iBufferLen, (char)0);
			m_pCurPacket->append(m_pBuffer, m_iBufferLen);
			m_pCurPacket->resize(len + m_iBufferLen);

			return;
		}
		OnDisconnection(er);
	});
}

void TCPConnection::ReadBody()
{
	auto self(shared_from_this());
	m_Socket.async_receive(Net::buffer(m_pCurPacket->data() + m_iReadBytes, m_pCurPacket->size() - m_iReadBytes), [this, self](std::error_code er, size_t bytes_transferred)
	{
		if (er)
		{
			m_iReadBytes += bytes_transferred;
			if (m_iReadBytes < m_pCurPacket->size())
			{
				ReadBody();
				return;
			}
			m_iReadBytes = 0;
			OnNewPacket(m_pCurPacket);
			m_pCurPacket = nullptr;
			ReadHead();
			return;
		}
		OnDisconnection(er);
	});
}
