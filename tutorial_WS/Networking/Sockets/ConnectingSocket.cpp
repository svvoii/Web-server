#include "ConnectingSocket.hpp"

/*
** This class is similar to BindingSocket class except that it uses
** `connect()` instead of `bind()` in the `connectToNetwork()` method.
** This represents a client side socket.
**
** In the constructor of this class we call the constructor of the
** parent class and pass the arguments to it.
*/
ConnectingSocket::ConnectingSocket(int domain, int service, int protocol, int port, u_long interface)
	: SimpleSocket(domain, service, protocol, port, interface) {

	int	connection = 0;

	// Establish connection, bind the socket to a port, check for error
	connection = connectToNetwork(this->getSocketFD(), this->getAddress());
	setConnection(connection); // ???
	testConnection(this->getConnection());
	//testConnection(connection);
}

ConnectingSocket::~ConnectingSocket() {
}

/*
** This method is used to connect the socket to a port.
** It is a virtual method in the parent class and must be implemented here.
**
** The return from `connectToNetwork()` will be assigned to the `connection` variable
** in the parent class constructor.. `connection = connectToNetwork(socket_fd, address);`
*/
int	ConnectingSocket::connectToNetwork(int socket_fd, struct sockaddr_in address) {

	int	connectStatus = 0;

	connectStatus = connect(socket_fd, (struct sockaddr *)&address, sizeof(address));
	return (connectStatus);
	// the error handling is done in the constructor above with `testConnection()`
}
