#pragma once
#include <experimental/net>
namespace Net = std::experimental::net;

struct IOContext
{
	Net::io_context Context;
};

struct Acceptor
{
	Acceptor(Net::io_context& io, uint16_t port):acceptor(io, Net::ip::tcp::endpoint(Net::ip::tcp::v4(), port)){}
	Net::ip::tcp::acceptor acceptor;
};