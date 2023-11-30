#ifndef SIMPLESOCKET_HPP
#define SIMPLESOCKET_HPP

#include <iostream>
#include <string>
#include <sys/socket.h> // socket(), bind(), listen(), accept()
#include <netinet/in.h> // sockaddr_in, INADDR_ANY

namespace HDE {

	class SimpleSocket {
		private:

			int					socket_fd;
			struct sockaddr_in	address;
			int					connection;
		
		public:

			SimpleSocket(int domain, int service, int protocol, int port, u_long interface);
			//SimpleSocket(int domain, int type, int protocol);

			// This must be implemented by the child class. 
			// In the implementation either `bind()` or `connect()` to be used (SERVER vs CLIENT)
			virtual int		connectToNetwork() = 0;
	};

}

#endif