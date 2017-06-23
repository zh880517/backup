#pragma once
#include "NetInclude.h"
#include "SharePtrDef.h"

#include <memory>
#include <string>


class TCPConnection : public std::enable_shared_from_this<TCPConnection>
{
public:
	TCPConnection(TCPConnectPool* connPool, Net::ip::tcp::socket& socket, uint32_t buffLen);
	~TCPConnection();

public: 

	void TryRecive();

	void Close();

	void SetPacketQueue(PacketQueue* queue);

	void TrySend();

protected:

	
	void ReadHead();

	void ReadBody();

	void Send();

private:
	Net::ip::tcp::socket m_Socket;
	TCPConnectPool*		 m_ConnPool;

	bool			m_bReading = false;

	char*			m_pBuffer = nullptr;
	size_t			m_iBufferLen = 0;
	size_t			m_iReadBytes = 0;
	std::string*	m_pCurRecivePacket = nullptr;

	size_t			m_iWriteBytes = 0;
	NetPacket		m_pCurSendPacket = nullptr;
	PacketQueue*	m_Packets = nullptr;
	std::mutex		m_SendMutex;

};

