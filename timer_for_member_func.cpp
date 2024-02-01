#include "asio/steady_timer.hpp"
#include <functional>
#include <iostream>
#include <asio.hpp>

class Printer {
public:
	Printer(asio::io_context& io) : timer(io, asio::chrono::seconds(1)), count(0) {
		timer.async_wait(std::bind(&Printer::print, this));
	}
	~Printer() {
		std::cout << "Final count is " << count << std::endl;
	}
	void print() {
		if (count < 5) {
			std::cout << count << std::endl;
			++count;
			timer.expires_at(timer.expiry() + asio::chrono::seconds(1));
			timer.async_wait(std::bind(&Printer::print, this));
		}
	}
private:
	asio::steady_timer timer;
	int count;
};

int main(int argc, char* argv[]) {
	asio::io_context io;
	Printer printer(io);
	io.run();
	return 0;
}
