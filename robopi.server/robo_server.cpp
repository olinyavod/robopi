#include "robo_server.hpp"

#include <iostream>

using robopi::robo_server;

void robo_server::do_accept()
{
	acceptor_.async_accept([this](const asio::error_code error, tcp::socket socket)
	{
		std::cout << "Accept client: " << socket.remote_endpoint() << ".\n";

		connections_.push_back(socket);		
		
		this->do_accept();
	});
}
