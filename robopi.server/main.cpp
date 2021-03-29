#include <iostream>

#include <asio.hpp>

#include "server_discover.hpp"

const std::uint16_t DISCOVER_PORT = 30000;

std::uint16_t server_port = 0;

std::size_t get_server_info(std::uint8_t* data, std::size_t max_length)
{
	const auto message_size = sizeof(server_port);
	
	std::memcpy(data, &server_port, message_size);
	
	return message_size;
}

int main(int argc, char *argv[])
{
	try 
	{
		if (argc > 1)
			std::sscanf(argv[1], "%d", &server_port);			
						
		asio::io_context io_context;

		robopi::server_discover discover(io_context, DISCOVER_PORT, get_server_info);
		
		discover.run();

		std::cout << "Start listen on " << server_port << " port.\n";
		io_context.run();
	
		return 0;
	}
	catch(std::exception& ex)
	{
		std::cerr << "Exception: " << ex.what() << "\n";
		
		return 1;
	}
}