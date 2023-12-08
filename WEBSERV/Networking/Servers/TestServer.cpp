#include "TestServer.hpp"

TestServer::TestServer() 
	: SimpleServer(
					DOM,
					SERVICE, 
					PROTOCOL, 
					PORT, 
					INTERFACE, 
					BACKLOG) {
	
	this->_requestType = NORM;

	run();
}

TestServer::~TestServer() {

}

void	TestServer::_accept() {

	std::cout << BLUE << "in (TestServer::_accept())" << RESET << std::endl;

	struct sockaddr_in	address = getSocket().getAddress();
	int					addrlen = sizeof(address);

	_new_socket = accept(getSocket().getSocketFD(), 
							(struct sockaddr *)&address, 
							(socklen_t*)&addrlen);

	if (_new_socket < 0) {
		std::cerr << "accept() failed" << std::endl;
		exit(EXIT_FAILURE);
	}

	std::cout << BLUE;
	std::cout << "Connection accepted" << std::endl;
	std::cout << "IP: " << inet_ntoa(address.sin_addr) << std::endl;
	std::cout << "Port ntohs[" << ntohs(address.sin_port) << "], sin_port[" << address.sin_port << "]" << std::endl;
	std::cout << RESET;

	//read(_new_socket, _buffer, BUFSIZE);
	/*
	*/
	char buff[BUFSIZE];
	int bytesRead = recv(_new_socket, buff, BUFSIZE, 0);
	if (bytesRead > 0) {
		_buffer.assign(buff, bytesRead);
	}
}

void	TestServer::_handle() {

	std::cout << BLUE << "in (TestServer::_handle()).. Here is your _buffer: (request from the web-browser)" << RESET << std::endl;

	std::cout << _buffer << std::endl;

	if (_buffer.find("GET /favicon.ico") != std::string::npos) {
		_requestType = FAVIC;
	}
}

void	TestServer::_respond() {

	std::cout << BLUE << "in (TestServer::_respond())" << RESET << std::endl;

	std::string	response = "Unknown request type..";

	if (_requestType == NORM) {
		response = "Wake up, Neo...\nThe Matrix has you...\nFollow the white rabbit.\nKnock, knock, Neo.";
		send(_new_socket, response.c_str(), response.length(), 0);
	}
	else if (_requestType == FAVIC) {
		response = "HTTP/1.1 404 Not Found\r\nContent-Length: 0\r\n\r\n";
		send(_new_socket, response.c_str(), response.length(), 0);
	}

	//write(_new_socket, hello, strlen(hello));

	std::cout << BLUE; 
	std::cout << "Responce message sent" << std::endl;
	std::cout << "Closing connection" << std::endl;
	std::cout << RESET;

	close(_new_socket);
}


void	TestServer::run() {
	
	std::cout << "TestServer::run()" << std::endl;

	while (true) {

		std::cout << CYAN << "\tWaiting for new connection... (_accept())" << RESET << std::endl;
		_accept();

		std::cout << CYAN << "\tHandling the request... (_handle())" << RESET << std::endl;
		_handle();

		std::cout << CYAN << "\tResponding to the request... (_respond())" << RESET << std::endl;
		_respond();
	}
}
