
#include <ctime>
#include <iostream>
#include <string>
#include <asio.hpp>

using asio::ip::tcp;

std::string make_daytime_string() {
	using namespace std;
	time_t now = time(0);
	return ctime(&now);
}

int main(int argc, char* argv[]) {
	try {
		asio::io_context io;
		// TCP Port 13, IP v4
		tcp::acceptor acceptor(io, tcp::endpoint(tcp::v4(), 13));
		for (;;) {
			tcp::socket socket(io);
			acceptor.accept(socket);
			auto msg = make_daytime_string();
			asio::error_code ignored_error;
			asio::write(socket, asio::buffer(msg), ignored_error);
		}
	} catch (std::exception& e) {
		std::cerr << e.what() << std::endl;
	}
}
