#include <iostream>
#include <asio.hpp>

int main(int argc, char* argv[]) {
	asio::io_context io;
	std::cout << "Sleep for 2 seconds..." << std::endl;
	asio::steady_timer timer(io, asio::chrono::seconds(2));
	timer.wait();
	std::cout << "Sleep finished" << std::endl;
	return 0;
}
