#include "SimpleServer.hpp"

SimpleServer::SimpleServer(int domain, int service, int protocol, 
		int port, u_long interface, int backlog) 
		: _socket(NULL) {

	_socket = new ListeningSocket(domain, service, protocol, 
			port, interface, backlog);
}

SimpleServer::~SimpleServer() {

	if (_socket != NULL) {
		delete _socket;
		_socket = NULL;
	}
}

void	SimpleServer::deleteSocket() {

	if (_socket != NULL) {
		delete _socket;
		_socket = NULL;
	}
}

ListeningSocket *	SimpleServer::getSocket() {

	return _socket;
}

