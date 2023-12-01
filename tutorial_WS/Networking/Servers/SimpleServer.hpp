#ifndef SIMPLESERVER_HPP
#define SIMPLESERVER_HPP

#include <iostream>

#include "../HeaderSockets.hpp"

class SimpleServer {
	private:

		ListeningSocket *	_socket;

		virtual void		_accept() = 0;
		virtual void		_handle() = 0;
		virtual void		_respond() = 0;

	public:

		SimpleServer(int domain, int service, int protocol, 
				int port, u_long interface, int backlog);
		~SimpleServer();

		// Getters
		ListeningSocket *	getSocket();

		virtual void		run() = 0;

};


#endif
