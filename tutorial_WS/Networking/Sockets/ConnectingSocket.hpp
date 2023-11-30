#ifndef CONNECTINGSOCKET_HPP
#define CONNECTINGSOCKET_HPP

#include "SimpleSocket.hpp"

/*
** This class will inherit from SimpleSocket class.
** It will be used to create a client side socket 
** using `connect()` in the `connectToNetwork()` method.
*/

class ConnectingSocket : public SimpleSocket {
	public:

		ConnectingSocket(int domain, int service, int protocol, int port, u_long interface);
		~ConnectingSocket();

		// This method is virtual in parent and must be implemented by the child class. 
		int		connectToNetwork(int socket_fd, struct sockaddr_in address);
};

#endif
