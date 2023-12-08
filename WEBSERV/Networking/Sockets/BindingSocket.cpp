#include "BindingSocket.hpp"

/*
** This class is similar to ConnectingSocket class except that it uses
** `bind()` instead of `connect()` in the `connectToNetwork()` method.
** This represents a server side socket.
**
** In the constructor of this class we call the constructor of the 
** parent class and pass the arguments to it.  
*/
BindingSocket::BindingSocket(int domain, int service, int protocol, int port, u_long interface)
	: SimpleSocket(domain, service, protocol, port, interface) {

	std::cout << MAGENTA << "\t[ BindingSocket ] constructor called." << RESET << std::endl;

	_bindStatus = 0;

	connectToNetwork(this->getSocketFD(), this->getAddress());
}

BindingSocket::~BindingSocket() {

	std::cout << RED << "\t[~] BindingSocket destructor called." << RESET << std::endl;
}

// Getters
int		BindingSocket::getBindStatus() const {

	return (this->_bindStatus);
}

/*
** This method is used to bind the socket to a port on a server side.
** It is a virtual method in the parent class and must be implemented here.
*/
void	BindingSocket::connectToNetwork(int socket_fd, struct sockaddr_in address) {

	// Establishing binding connection to the network
	_bindStatus = bind(socket_fd, (struct sockaddr *)&address, sizeof(address));
	// error check
	this->testConnection(_bindStatus);
}
