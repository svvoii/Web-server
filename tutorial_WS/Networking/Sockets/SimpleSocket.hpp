#ifndef SIMPLESOCKET_HPP
#define SIMPLESOCKET_HPP

#include <iostream>
#include <string>
#include <cstdio> // perror()
#include <cstdlib> // exit()
#include <unistd.h> // close()
#include <sys/socket.h> // socket(), bind(), listen(), accept()
#include <netinet/in.h> // sockaddr_in, INADDR_ANY

/*
** This goingto be the parent class for TCP or UDP sockets
*/
class SimpleSocket {
	private:

		int					_socket_fd;
		struct sockaddr_in	_address;
	
	public:

		SimpleSocket(int domain, int service, int protocol, int port, u_long interface);
		~SimpleSocket();

		// Getters
		int					getSocketFD() const;
		struct sockaddr_in	getAddress() const;

		// Setters
		void				setSocketFD(int socket_fd);

		// This must be implemented by the child class. 
		// In the implementation either `bind()` or `connect()` to be used (SERVER vs CLIENT)
		virtual void	connectToNetwork(int socket_fd, struct sockaddr_in address) = 0;

		// Also can be a virtual so that the child class can specify the behavior
		void			testConnection(int itemToTest);
};

#endif