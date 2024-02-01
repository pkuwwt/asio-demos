OUT := $(patsubst %.cpp,%,$(wildcard *.cpp))

all: ${OUT}

asio:
	git clone https://mirror.ghproxy.com/https://github.com/chriskohlhoff/asio
	cd asio/asio
	make && make install

%:%.cpp
	g++ -std=c++11 $< -o $@

clean:
	@rm -rf ${OUT}
