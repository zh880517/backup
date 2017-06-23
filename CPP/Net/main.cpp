#include <iostream>
#include <experimental/timer>
#include <experimental/net>
#include <experimental/__net_ts/ts/timer.hpp>
#include <experimental/__net_ts/ts/net.hpp>

#include <tuple>

using namespace std::experimental::net;

void Handler_Timer(std::error_code ec)
{
	std::cout << ec << std::endl;
}

class Server
{
public:
	Server(io_context& io, short port)
		:m_Acceptor(io, ip::tcp::endpoint(ip::tcp::v4(), port))
	{
		DoAccept();
	}

	void Close()
	{
		m_Acceptor.close();
	}

private: 
	void DoAccept()
	{
		m_Acceptor.async_accept([this](const std::error_code& error, ip::tcp::socket peer)
		{
			if (!error)
			{
				std::cout << peer.remote_endpoint()<<std::endl;
				static std::string str = "Hello!!!";
				peer.async_send(buffer(str, str.size()), [](const std::error_code& error, std::size_t bytes_transferred) 
				{
					std::cout << error << std::endl;
					std::cout << bytes_transferred << std::endl;
				});
			}
			else
			{
				std::cout << error << std::endl;
			}
			DoAccept();
		});
	}

private:
	ip::tcp::acceptor m_Acceptor;
};

union MyUnion
{
	uint64_t value;
	char data[8];
};

int main()
{
	std::string str = "123456";
	char* data = &(str.at(0));
	ip::address addr = ip::make_address("0.0.0.0");
	io_context c;
	ip::tcp::socket so(c);
	//steady_timer t(c);
	//t.expires_after(chrono::seconds(5));
	//t.wait();
	//t.async_wait(Handler_Timer);
	Server sr(c, 2048);
	c.run();
	return 0;
}