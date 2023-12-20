#include "../includes/Server.hpp"

Server::Server(t_serverData *serverData) 
	: serverSocket(
		AF_INET, 
		SOCK_STREAM, 
		PROTOCOL, 
		serverData->port, 
		INADDR_ANY, 
		BACKLOG ) {

	std::cout << MAGENTA << "\tServer constructor called" << RESET << std::endl;

	_serverFd = serverSocket.getSocketFD();

	_port = serverData->port;
	_serverName = serverData->serverName;
	_uploadpath = serverData->uploadPath;
	_locations_map = serverData->locations_map;

}

Server::~Server() {

	std::cout << RED << "\tServer destructor called" << RESET << std::endl;
}

int		Server::getServerFd() const {

	return _serverFd;
}
