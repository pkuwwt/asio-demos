#include <iostream>
#include <asio.hpp>

void print(const asio::error_code& /*e*/) {
	std::cout << "end of wait" << std::endl;
}

int main(int argc, char* argv[]) {
	asio::io_context io;
	asio::steady_timer timer(io, asio::chrono::seconds(5));
	timer.async_wait(&print);
	std::cout << "after async_wait" << std::endl;
	io.run();
	std::cout << "end of program" << std::endl;
	return 0;
}
