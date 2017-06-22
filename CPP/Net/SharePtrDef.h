#pragma once
#include <memory>

class TCPConnection;
class TCPSession;

using TCPConnectionPtr = std::shared_ptr<TCPConnection>;
using TCPSessionPtr = std::shared_ptr<TCPSession>;