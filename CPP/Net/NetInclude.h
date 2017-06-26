#pragma once
#include <experimental/net>
namespace Net = std::experimental::net;

struct IOContext
{
public:
	IOContext()
		:workGurad(Net::make_work_guard(Context))
	{
	}
	Net::io_context Context;
private:
	Net::executor_work_guard<Net::io_context::executor_type> workGurad;
};

struct Acceptor
{
	Acceptor(Net::io_context& io, uint16_t port):acceptor(io, Net::ip::tcp::endpoint(Net::ip::tcp::v4(), port)){}
	Net::ip::tcp::acceptor acceptor;
};