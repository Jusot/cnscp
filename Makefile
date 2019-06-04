all: ./cnscp/client.cpp ./cnscp/merchant.cpp ./cnscp/bank.cpp | ./bin
	g++ -g -Wall -std=c++17 ./cnscp/client.cpp -o ./bin/client -lpthread
	g++ -g -Wall -std=c++17 ./cnscp/merchant.cpp -o ./bin/merchant -lpthread
	g++ -g -Wall -std=c++17 ./cnscp/bank.cpp -o ./bin/bank -lpthread

./bin:
	mkdir bin

.PHONY: clean
clean:
	rm -rf ./bin/*
