
#include <iostream>
#include <asio.hpp>
#include <functional>


class Printer {
public:
	Printer(asio::io_context& io) :
		strand(asio::make_strand(io)),
		timer1(io, asio::chrono::seconds(1)),
		timer2(io, asio::chrono::seconds(1)),
		count(0) {
			next1();
			next2();
	}
	~Printer() {
		std::cout << "Final count is " << count << std::endl;
	}
	void next1() {
		timer1.async_wait(asio::bind_executor(strand, std::bind(&Printer::print1, this)));
	}
	void next2() {
		timer2.async_wait(asio::bind_executor(strand, std::bind(&Printer::print2, this)));
	}
	void print1() {
		if (count < 10) {
			std::cout << "Timer 1 " << count << std::endl;
			++count;
			timer1.expires_at(timer1.expiry() + asio::chrono::seconds(1));
			next1();
		}
	}
	void print2() {
		if (count < 10) {
			std::cout << "Timer 2 " << count << std::endl;
			++count;
			timer2.expires_at(timer2.expiry() + asio::chrono::seconds(1));
			next2();
		}
	}
private:
	// make sure only one handler executed at the same time
	asio::strand<asio::io_context::executor_type> strand;
	asio::steady_timer timer1;
	asio::steady_timer timer2;
	int count;
};

int main(int argc, char* argv[]) {
	asio::io_context io;
	Printer printer(io);
	asio::thread t(std::bind(static_cast<size_t (asio::io_context::*)()>(&asio::io_context::run), &io));
	io.run();
	t.join();
	return 0;
}
