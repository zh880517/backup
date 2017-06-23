#pragma once
#include <memory>
#include <string>
#include "ThreadQueue.h"

class TCPConnection;
class TCPSession;
struct IOContext;
struct Acceptor;
class TCPConnectPool;

using TCPConnectionPtr = std::shared_ptr<TCPConnection>;
using TCPSessionPtr = std::shared_ptr<TCPSession>;


using NetPacket = std::shared_ptr<std::string>;
using PacketQueue = ThreadQueue<NetPacket>;