#include "../includes/ServersManager.hpp"

ServersManager::ServersManager() {

	std::cout << MAGENTA << "\tServersManager default constructor called" << RESET << std::endl;

	_numberOfServers = _config.getNumberOfServers();

	_servers.reserve(_numberOfServers);

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

		_addToSet(_servers[i].getServerFd(), &_recv_fd_pool);

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

	_initFdSets();

	while (1) {

		recv_fd_pool_copy = _recv_fd_pool;
		send_fd_pool_copy = _send_fd_pool;

		int		activity = select(_max_fd + 1, &recv_fd_pool_copy, &send_fd_pool_copy, NULL, NULL);

		checkErrorAndExit(activity, "select() failed");

		for (int i = 0; i < _numberOfServers; i++) {

			if (FD_ISSET(_servers[i].getServerFd(), &recv_fd_pool_copy)) {

				_accept(_servers[i].getServerFd());
			}
		}

		for (int i = 0; i < _numberOfServers; i++) {

			if (FD_ISSET(_servers[i].getServerFd(), &send_fd_pool_copy)) {

				_respond(_servers[i].getServerFd());
			}
		}

		for (int i = 0; i < _numberOfServers; i++) {

			if (FD_ISSET(_servers[i].getServerFd(), &recv_fd_pool_copy)) {

				_handle(_servers[i].getServerFd());
			}
		}
	}
}
