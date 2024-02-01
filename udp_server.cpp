
#include <ctime>
#include <iostream>
#include <string>
#include <asio.hpp>

using asio::ip::udp;

std::string make_daytime_string() {
	using namespace std;
	time_t now = time(0);
	return ctime(&now);
}

int main(int argc, char* argv[]) {
	try {
		asio::io_context io;
		// udp Port 13, IP v4
		udp::socket socket(io, udp::endpoint(udp::v4(), 13));
		for (;;) {
			char buf[1] = {0};
			udp::endpoint remote_endpoint;
			socket.receive_from(asio::buffer(buf), remote_endpoint);
			auto message = make_daytime_string();
			asio::error_code ignored_error;
			socket.send_to(asio::buffer(message), remote_endpoint, 0, ignored_error);
			std::cout << "Send message to " << remote_endpoint.address() << ":" << remote_endpoint.port() << std::endl;
		}
	} catch (std::exception& e) {
		std::cerr << e.what() << std::endl;
	}
}

