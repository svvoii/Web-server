#include "SimpleServer.hpp"

SimpleServer::SimpleServer(int domain, int service, int protocol, 
		int port, u_long interface, int backlog) 
		: _socket(domain, 
					service, 
					protocol, 
					port, 
					interface, 
					backlog) {

	std::cout << MAGENTA << "\tSimpleServer constructor called." << RESET << std::endl;

	//_socket = new ListeningSocket(domain, service, protocol, port, interface, backlog);
}

SimpleServer::~SimpleServer() {

	std::cout << RED << "\t[~] SimpleServer destructor called." << RESET << std::endl;
}

ListeningSocket &	SimpleServer::getSocket() {

	return _socket;
}
