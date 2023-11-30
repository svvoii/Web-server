#include "SimpleSocket.hpp"

HDE::SimpleSocket::SimpleSocket(int domain, int service, int protocol, int port, u_long interface) {

	// Defining address structure
	this->address.sin_family = domain; // AF_INET;
	this->address.sin_port = htons(port); // htons(PORT);
	this->address.sin_addr.s_addr = htonl(interface); // INADDR_ANY;

	// Creating socket, establishing connection, getting file descriptor 
	this->socket_fd = socket(domain, service, protocol); // AF_INET = IPv4, SOCK_STREAM = TCP, 0 = IP
	if (this->socket_fd == 0) {
		
		perror("In socket");
		exit(EXIT_FAILURE);
	}

	// 
	this->connection = connectToNetwork();
}
