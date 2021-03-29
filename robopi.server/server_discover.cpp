#include "server_discover.hpp"

#include <iostream>

using robopi::server_discover;

void server_discover::run()
{
	auto* data = new std::uint8_t[max_length];
	auto* sender_endpoint = new udp::endpoint();
		
	sock_.async_receive_from(asio::buffer(data, max_length),
		*sender_endpoint,
		[this, data, sender_endpoint](const asio::error_code& err, const std::size_t len)
		{
			this->handle_receive_from(*sender_endpoint, data, err, len);

			delete[] data;
			delete sender_endpoint;
		});
}

void server_discover::handle_receive_from(const udp::endpoint& sender_endpoint, const std::uint8_t* data, const asio::error_code& error, std::size_t bytes_recvd)
{
	std::cout << "Received message from: " << sender_endpoint << ".\n";
	
	if (bytes_recvd > max_length)
		std::cerr << "Received bytes more " << max_length << " is " << bytes_recvd << ".\n";

	if (data[0] == auth)
	{
		std::cout << "Received auth command.\n";
		auto* send_data = new std::uint8_t[max_length];
		auto len = get_server_info_(send_data, max_length);
				
		sock_.async_send_to(asio::buffer(send_data, len),
			sender_endpoint, 
			[this, send_data](const asio::error_code&, std::size_t)
			{
				delete[] send_data;
			});
	}
	
	run();
}

void server_discover::stop()
{
	sock_.cancel();
}