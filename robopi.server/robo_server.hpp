#pragma once

#include <list>

#include <asio.hpp>

using asio::ip::tcp;

namespace robopi {
	class robo_server
	{
	private:
		tcp::acceptor acceptor_;
		std::list<tcp::socket> connections_;

		void do_accept();
	public:
		robo_server(asio::io_context& io_context, std::uint8_t port)
			: acceptor_(io_context, tcp::endpoint(tcp::v4(), port))
		{
				
		}

		void run()
		{
			do_accept();			
		}

		tcp::endpoint local_endpoint() const
		{
			return acceptor_.local_endpoint();
		}	
	};
}

