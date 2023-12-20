/*
** This class is set to create and manage the servers.
** The `Config` class will be used to get the data from the config file,
** and pass it to each server instance created.
**
** I/O MULTIPLEXING will be used to handle the connections to 
** each server with the help of the `select()` function, as well as
** the `fd_set` structure, and `fcntl()` function to set the socket
** to non-blocking mode.
*/
#ifndef SERVERSMANAGER_HPP
#define SERVERSMANAGER_HPP

#include "../../Config/Config.hpp"
#include "Server.hpp"
#include "HttpRequest.hpp"
#include "HttpResponse.hpp"

#include <arpa/inet.h> // inet_ntoa
#include <fcntl.h> // fcntl
#include <map> // std::map
#include <ctime> // std::time_t

class ServersManager {
	private:

		Config							_config;
		int								_numberOfServers;
		std::vector<Server>				_servers;

		fd_set							_recv_fd_pool; // To store the socket FDs of the clients
		fd_set							_send_fd_pool; // To store the socket FDs of the clients
		int								_max_fd; // To store the max socket FD

		void							_initFdSets();
		void							_addToSet(int fd, fd_set *set);
		void							_removeFromSet(int fd, fd_set *set);
		void							_closeConnection(int fd);

		void							_accept(int serverIndex, int fd);
		void							_handle(int serverIndex, int fd);
		void							_respond(int serverIndex, int fd);

	public:

		ServersManager();
		~ServersManager();

		// Setters
		void					setServers();

		// Main logic to run the servers, receive, handle and respond to the requests
		std::string				timeStamp();
		void					checkErrorAndExit(int returnValue, const std::string& msg);
		void					run();

};


#endif
