#include "../includes/ListeningSocket.hpp"

ListeningSocket::ListeningSocket(
	int domain, 
	int service, 
	int protocol, 
	int port, 
	u_long interface,
	int backlog)
	: BindingSocket(domain, service, protocol, port, interface) {

	std::cout << MAGENTA << "\t[ ListeningSocket ] constructor called." << RESET << std::endl;

	this->_backlog = backlog;
	this->_listening = 0;

	startListenToNetwork();
}

ListeningSocket::~ListeningSocket() {

	std::cout << RED << "\t[~] ListeningSocket destructor called." << RESET << std::endl;
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
