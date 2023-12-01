#include "TestServer.hpp"

TestServer::TestServer() 
	: SimpleServer(
					DOMAIN,
					SERVICE, 
					PROTOCOL, 
					PORT, 
					INTERFACE, 
					BACKLOG) {
	run();
}

TestServer::~TestServer() {

}

void	TestServer::_accept() {

	//std::cout << "TestServer::_accept()" << std::endl;

	struct sockaddr_in	address = getSocket()->getAddress();
	int					addrlen = sizeof(address);

	_new_socket = accept(getSocket()->getSocketFD(), 
							(struct sockaddr *)&address, 
							(socklen_t*)&addrlen);

	if (_new_socket < 0) {
		std::cerr << "accept() failed" << std::endl;
		exit(EXIT_FAILURE);
	}

	std::cout << "Connection accepted" << std::endl;
	std::cout << "IP: " << inet_ntoa(address.sin_addr) << std::endl;

	read(_new_socket, _buffer, BUFSIZE);

}

void	TestServer::_handle() {

	std::cout << "TestServer::_handle().. Here is your _buffer:" << std::endl;

	std::cout << _buffer << std::endl;
}

void	TestServer::_respond() {

	std::cout << "TestServer::_respond()" << std::endl;

	const char *hello = "Wake up, Neo...\nThe Matrix has you...\nFollow the white rabbit.\nKnock, knock, Neo.";

	write(_new_socket, hello, strlen(hello));

	std::cout << "Hello message sent" << std::endl;
	std::cout << "Closing connection" << std::endl;

	close(_new_socket);
}


void	TestServer::run() {
	
	std::cout << "TestServer::run()" << std::endl;

	while (true) {

		_accept();
		_handle();
		_respond();
	}
}
