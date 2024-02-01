#include <functional>
#include <iostream>
#include <asio.hpp>

void print(const asio::error_code& /*e*/, asio::steady_timer* t, int* count) {
	if (*count < 5) {
		std::cout << *count << std::endl;
		++(*count);
		t->expires_at(t->expiry() + asio::chrono::seconds(1));
		t->async_wait(std::bind(print, std::placeholders::_1, t, count));
	}
}

int main(int argc, char* argv[]) {
	asio::io_context io;
	int count = 0;
	asio::steady_timer timer(io, asio::chrono::seconds(1));
	timer.async_wait(std::bind(print, std::placeholders::_1, &timer, &count));
	io.run();
	std::cout << "Final count is " << count << std::endl;
	return 0;
}
