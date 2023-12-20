#include "../includes/ServersManager.hpp"

ServersManager::ServersManager() {

	std::cout << MAGENTA << "\tServersManager default constructor called" << RESET << std::endl;

	_numberOfServers = _config.getNumberOfServers();

	// This function is for testing purposes only !!!
	// The hardcoded data shall be parsed from the config file.
	_config.setServersData();

	std::cout << YELLOW << "[!] Number of servers: " << _numberOfServers << RESET << std::endl;

	//_servers.reserve(_numberOfServers);

	setServers();
}

ServersManager::~ServersManager() {

	std::cout << RED << "\tServersManager destructor called" << RESET << std::endl;
}

void	ServersManager::_initFdSets() {

	FD_ZERO(&_recv_fd_pool);
	FD_ZERO(&_send_fd_pool);
	int	fcntl_ret = 0;
	int	serverFd = 0;
	_max_fd = 0;

	for (int i = 0; i < _numberOfServers; i++) {

		serverFd = _servers[i].getServerFd();

		fcntl_ret = fcntl(serverFd, F_SETFL, O_NONBLOCK);
		checkErrorAndExit(fcntl_ret, "fcntl() failed. Exiting..");

		_addToSet(serverFd, &_recv_fd_pool);

		_max_fd = serverFd;
	}
}

void	ServersManager::_addToSet(int fd, fd_set *set) {

	if (fd > _max_fd) {
		_max_fd = fd;
	}
	FD_SET(fd, set);
}

void	ServersManager::_removeFromSet(int fd, fd_set *set) {

	if (fd == _max_fd) {
		_max_fd--;
	}
	FD_CLR(fd, set);
}

void	ServersManager::_closeConnection(int fd) {
	std::cout << YELLOW << "[!] Closing connection fd [" << fd << "] on the server side." << RESET << std::endl;

	if (FD_ISSET(fd, &_recv_fd_pool)) {
		_removeFromSet(fd, &_recv_fd_pool);
	}
	if (FD_ISSET(fd, &_send_fd_pool)) {
		_removeFromSet(fd, &_send_fd_pool);
	}
	close(fd);
}


void	ServersManager::setServers() {

	std::vector<t_serverData>	serversData = _config.getServersData();

	for (int i = 0; i < _numberOfServers; i++) {

		_servers.push_back(Server(&serversData[i]));
	}
}

std::string	ServersManager::timeStamp() {

	std::time_t currentTime = std::time(NULL);
    std::tm* now = std::localtime(&currentTime);
    char buffer[64];
    std::strftime(buffer, sizeof(buffer), "[%d/%m/%Y %H:%M:%S]", now);

	return std::string(buffer);
}

void	ServersManager::checkErrorAndExit(int returnValue, const std::string& msg) {

	if (returnValue == -1) {

		std::cerr << RED << "\t[-]" << msg << RESET << std::endl;
		exit(EXIT_FAILURE);
	}
}

void	ServersManager::run() {

	fd_set	recv_fd_pool_copy;
	fd_set	send_fd_pool_copy;
	int		select_ret = 0;
	int		serverFd = 0;

	_initFdSets();

	while (true) {

		recv_fd_pool_copy = _recv_fd_pool;
		send_fd_pool_copy = _send_fd_pool;

		select_ret = select(_max_fd + 1, &recv_fd_pool_copy, &send_fd_pool_copy, NULL, NULL);
		checkErrorAndExit(select_ret, "select() failed. Exiting..");

		for (int s = 0; s < _numberOfServers; s++) {

			serverFd = _servers[s].getServerFd();	

			for (int fd = 3; fd <= _max_fd; fd++) {

				if (FD_ISSET(fd, &recv_fd_pool_copy)) {

					// need tocheck if server index `s` is correct !!?
					_accept(s, fd);
				}
				else if (FD_ISSET(fd, &recv_fd_pool_copy)) {

					// need tocheck if server index `s` is correct !!?
					_handle(s, fd);
				}
				else if (FD_ISSET(fd, &send_fd_pool_copy)) {

					// need tocheck if server index `s` is correct !!?
					_respond(s, fd);
				}
			}
		}
		// check for timeout
	}
}

void	ServersManager::_accept(int serverIndex, int fd) {

	struct sockaddr_in	address; // ??
	socklen_t			address_len = sizeof(address);
	int					fcntl_ret = 0;
	int					serverFd = _servers[serverIndex].getServerFd();

	fd = accept(serverFd, (struct sockaddr *)&address, (socklen_t *)&address_len);
	if (fd == -1) {
		std::cerr << RED << "\t[-] Error accepting connection.. accept() failed." << RESET << std::endl;
		return ;
	}

	std::cout << timeStamp() << GREEN << " [+] New connection accepted on server [" << serverFd << "] from client [" << fd << "] with IP: " << inet_ntoa(address.sin_addr) << RESET << std::endl;

	_addToSet(fd, &_recv_fd_pool);

	fcntl_ret = fcntl(fd, F_SETFL, O_NONBLOCK);
	if (fcntl_ret == -1) {
		std::cerr << RED << "\t[-] Error setting socket to non-blocking mode.. fcntl() failed." << RESET << std::endl;
		_closeConnection(fd);
		return ;
	}

}

void	ServersManager::_handle(int serverIndex, int fd) {

	char	buffer[BUF_SIZE] = {0};
	int		bytes_read = 0;

	bytes_read = read(fd, buffer, BUF_SIZE);

	std::cout << timeStamp();

	if (bytes_read == 0) {
		std::cout << YELLOW << "[!] Connection closed by the client. ";
		_closeConnection(fd);
		return ;
	}
	else if (bytes_read < 0) {
		std::cerr << RED << "[-] Error reading data from the client.. read() failed." << RESET << std::endl;
		_closeConnection(fd);
		return ;
	}

	// check the server index
	_servers[serverIndex].requestBuffer.assign(buffer, bytes_read);

	std::cout << CYAN << "[*] Request received from client [" << fd << "]" << RESET << std::endl;

	HttpRequest 	httpRequest(_servers[serverIndex].requestBuffer);

	HttpResponse	response(&httpRequest);
	_servers[serverIndex].responseBuffer = response.getResponse();

	// might need to check if the body is not empty and handle CGI and other stuff

	_removeFromSet(fd, &_recv_fd_pool);
	_addToSet(fd, &_send_fd_pool);
}

void	ServersManager::_respond(int serverIndex, int fd) {

	int		bytes_sent = 0;
	int		bytes_to_send = _servers[serverIndex].responseBuffer.length();

	bytes_sent = send(fd, _servers[serverIndex].responseBuffer.c_str(), bytes_to_send, 0);

	std::cout << timeStamp();

	if (bytes_sent == -1) {
		std::cerr << RED << "[-] Error sending data to the client.. send() failed." << RESET << std::endl;
		_closeConnection(fd);
		return ;
	}
	else if (bytes_sent < bytes_to_send) {
		std::cout << YELLOW << "[!] Not all data has been sent to the client. " << RESET << std::endl;
		_servers[serverIndex].responseBuffer.erase(0, bytes_sent);
		return ;
	}
	else {
		std::cout << GREEN << "[+] Response sent to client, socket:[" << fd << "]" << RESET << std::endl;
	}

	_removeFromSet(fd, &_send_fd_pool);
	_addToSet(fd, &_recv_fd_pool);
}

