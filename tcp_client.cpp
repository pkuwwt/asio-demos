
#include "asio/io_context.hpp"
#include <iostream>
#include <asio.hpp>


using asio::ip::tcp;

int main(int argc, char* argv[]) {
	try {
		if (argc != 2) {
			std::cerr << "USAGE: " << argv[0] << " <host>" << std::endl;
			return 1;
		}
		asio::io_context io;
		tcp::resolver resolver(io);
		tcp::resolver::results_type endpoints = resolver.resolve(argv[1], "daytime");
		tcp::socket socket(io);
		asio::connect(socket, endpoints);
		for (;;) {
			char buf[128];
			asio::error_code error;
			size_t len = socket.read_some(asio::buffer(buf), error);
			if (error == asio::error::eof) {
				break; // Connection closed cleanly by peer
			} else if (error) {
				throw asio::system_error(error); // Some other error
			}
			std::cout.write(buf, len);
		}
	} catch (std::exception& e) {
		std::cerr << e.what() << std::endl;
	}
	return 0;
}
