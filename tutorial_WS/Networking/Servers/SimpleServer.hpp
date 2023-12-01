#ifndef SIMPLESERVER_HPP
#define SIMPLESERVER_HPP

#include <iostream>

#include "../HeaderSockets.hpp"

class SimpleServer {
	private:

		BindingSocket	_bindingSocket;
		ListeningSocket	_listeningSocket;

	public:

		SimpleServer();
		~SimpleServer();

		// Getters
		BindingSocket	getBindingSocket() const;
		ListeningSocket	getListeningSocket() const;

		// Setters
		void	setBindingSocket(BindingSocket bindingSocket);
		void	setListeningSocket(ListeningSocket listeningSocket);

		// Methods
		void	startServer();
};


#endif
