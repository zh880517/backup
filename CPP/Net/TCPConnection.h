#pragma once
#include "NetInclude.h"
#include <memory>
class TCPConnection : public std::enable_shared_from_this<TCPConnection>
{
public:
	TCPConnection(Net::ip::tcp::socket socket, uint32_t buffLen);
	~TCPConnection();

	std::function<void(std::error_code)> OnDisconnection;
	std::function<void(std::string*)> OnNewPacket;
	std::function<size_t(const char*, size_t)> GetDataLen;

public: 
	
	void Close();

protected:
	
	void ReadHead();

	void ReadBody();

private:
	char*			m_pBuffer = nullptr;
	size_t			m_iBufferLen = 0;
	size_t			m_iReadBytes = 0;
	std::string*	m_pCurPacket = nullptr;
	Net::ip::tcp::socket m_Socket;

};

