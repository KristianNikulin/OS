all:
	g++ server.cpp -o server -lzmq
	g++ client.cpp -o client -lzmq -pthread

server:
	g++ server.cpp -o server -lzmq

client:
	g++ client.cpp -o client -lzmq -pthread

clean:
	rm -rf client server logins.txt