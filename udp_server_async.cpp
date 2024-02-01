
#include <ctime>
#include <iostream>
#include <string>
#include <asio.hpp>
#include <memory>

using asio::ip::udp;

std::string make_daytime_string() {
	using namespace std;
	time_t now = time(0);
	return ctime(&now);
}


class UDPServer {
public:
	UDPServer(asio::io_context& io) : _io(io), _socket (io, udp::endpoint(udp::v4(), 13)) {
		start_receive();
	}
private:
	void start_receive() {
		_socket.async_receive_from(asio::buffer(_recv_buf),
				_remote_endpoint,
				std::bind(&UDPServer::handle_receive, this,
					std::placeholders::_1,
					std::placeholders::_2
				));
	}
	void handle_receive(const asio::error_code& error, std::size_t bytes_transfered) {
		if (!error) {
			auto message = std::make_shared<std::string>(make_daytime_string());
			_socket.async_send_to(asio::buffer(*message), _remote_endpoint,
					std::bind(&UDPServer::handle_send, this, message,
						std::placeholders::_1,
						std::placeholders::_2
						));
		}
		start_receive();
	}
	void handle_send(std::shared_ptr<std::string> message, const asio::error_code&, std::size_t bytes_transfered) {
	}
	asio::io_context& _io;
	udp::endpoint _remote_endpoint;
	udp::socket _socket;
	char _recv_buf[1] = {0};
};

int main(int argc, char* argv[]) {
	try {
		asio::io_context io;
		UDPServer server(io);
		io.run();
	} catch (std::exception& e) {
		std::cerr << e.what() << std::endl;
	}
}
