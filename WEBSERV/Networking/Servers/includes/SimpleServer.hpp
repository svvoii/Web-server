#ifndef SIMPLESERVER_HPP
#define SIMPLESERVER_HPP

#include <iostream>

#include "../../Sockets/HeaderSockets.hpp"

class SimpleServer {
	private:

		//ListeningSocket *	_socket;
		ListeningSocket		_socket;

		// Must be implemented by the child class.
		virtual void		_accept() = 0;
		virtual void		_handle(int fd) = 0;
		virtual void		_respond(int fd) = 0;

		// alternative to handle() and respond()
		virtual void		_handleRequestAndResponse() = 0;

	public:

		SimpleServer(int domain, int service, int protocol, 
				int port, u_long interface, int backlog);
		~SimpleServer();

		// Getters
		ListeningSocket &	getSocket();

		virtual void		run() = 0;

};


#endif
