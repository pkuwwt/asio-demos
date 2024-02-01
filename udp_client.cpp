
#include <iostream>
#include <asio.hpp>

using asio::ip::udp;

int main(int argc, char* argv[]) {
	try {
		if (argc != 2) {
			std::cerr << "USAGE: " << argv[0] << " <host>" << std::endl;
			return 1;
		}
		asio::io_context io;
		udp::resolver resolver(io);
		udp::endpoint recv_endpoint = *resolver.resolve(udp::v4(), argv[1], "daytime").begin();
		udp::socket socket(io);
		socket.open(udp::v4());
		char send_buf[1] = {0};
		socket.send_to(asio::buffer(send_buf), recv_endpoint);
		char recv_buf[128];
		udp::endpoint send_endpoint;
		size_t len = socket.receive_from(asio::buffer(recv_buf), send_endpoint);
		std::cout.write(recv_buf, len);
	} catch (std::exception& e) {
		std::cerr << e.what() << std::endl;
	}
	return 0;
}
