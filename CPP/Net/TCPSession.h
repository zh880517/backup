#pragma once
#include <memory>

class TCPConnection;
using TCPConnectionPtr = std::shared_ptr<TCPConnection>;

class TCPSession
{
public:
	TCPSession();
	~TCPSession();
};

