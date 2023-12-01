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
}

ListeningSocket::~ListeningSocket() {
}

// Getters
int		ListeningSocket::getBacklog() const {

	return this->_backlog;
}

int		ListeningSocket::getListening() const {

	return this->_listening;
}

void	ListeningSocket::startListenToNetwork() {

	_listening = listen(getSocketFD(), _backlog);

	testConnection(_listening);
}
