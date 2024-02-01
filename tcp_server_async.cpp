
#include <ctime>
#include <iostream>
#include <string>
#include <asio.hpp>
#include <memory>

using asio::ip::tcp;

std::string make_daytime_string() {
	using namespace std;
	time_t now = time(0);
	return ctime(&now);
}


class TCPConnection : public std::enable_shared_from_this<TCPConnection> {
public:
	typedef std::shared_ptr<TCPConnection> pointer;
	static pointer create(asio::io_context& io) {
		return pointer(new TCPConnection(io));
	}
	tcp::socket& socket() {
		return _socket;
	}
	void start() {
		_message = make_daytime_string();
		asio::async_write(_socket, asio::buffer(_message),
				std::bind(&TCPConnection::handle_write, shared_from_this(),
					std::placeholders::_1,
					std::placeholders::_2
				));
		std::cout << "async_write() returned immediately" << std::endl;
	}
private:
	TCPConnection(asio::io_context& io) : _socket(io) {
	}
	void handle_write(const asio::error_code& error, size_t bytes_transfered) {
		if (!error) {
			std::cout << "data sent: " << bytes_transfered << " bytes" << std::endl;
		} else {
			std::cerr << "ERROR: " << error << std::endl;
		}
	}
	tcp::socket _socket;
	std::string _message;
};

class TCPServer {
public:
	TCPServer(asio::io_context& io) : _io(io), _acceptor(io, tcp::endpoint(tcp::v4(), 13)) {
		start_accept();
	}
private:
	void start_accept() {
		auto new_connection = TCPConnection::create(_io);
		_acceptor.async_accept(new_connection->socket(),
				std::bind(&TCPServer::handle_accept, this,
					new_connection,
					std::placeholders::_1
				));
	}
	void handle_accept(TCPConnection::pointer new_connection, const asio::error_code& error) {
		if (!error) {
			new_connection->start();
		}
		start_accept();
	}
	asio::io_context& _io;
	tcp::acceptor _acceptor;
};

int main(int argc, char* argv[]) {
	try {
		asio::io_context io;
		TCPServer server(io);
		io.run();
	} catch (std::exception& e) {
		std::cerr << e.what() << std::endl;
	}
}
