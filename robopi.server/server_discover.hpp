#pragma once

#include <asio.hpp>

namespace robopi
{
	using asio::ip::udp;

	typedef std::size_t (*get_server_info_handler)(std::uint8_t* data, std::size_t max_length);

	class server_discover
	{
	private:

		enum commands : std::uint8_t
		{
			auth = 0xAC
		};
		
		static const std::size_t max_length = 1024;
		udp::socket sock_;
		get_server_info_handler get_server_info_;

	public:
		server_discover(asio::io_context& io_context,
			const std::uint16_t port,
			get_server_info_handler get_server_info)
			: sock_(io_context, udp::endpoint(udp::v4(), port))
			, get_server_info_(get_server_info)
		{
		
		}

		void run();

		void stop();

	private:
		void handle_receive_from(const udp::endpoint& sender_endpoint, const std::uint8_t* data, const asio::error_code& error, std::size_t bytes_recvd);
	};
}

