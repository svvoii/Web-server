#include "ListeningSocket.hpp"

ListeningSocket::ListeningSocket(
	int domain, 
	int service, 
	int protocol, 
	int port, 
	u_long interface,
	int backlog)
	: BindingSocket(domain, service, protocol, port, interface) {

	this->_backlog = backlog;
	this->_listening = 0;

	startListenToNetwork();
	testConnection(this->_listening);
}

ListeningSocket::~ListeningSocket() {
}

void	ListeningSocket::startListenToNetwork() {

	this->_listening = listen(getSocketFD(), _backlog);
}

