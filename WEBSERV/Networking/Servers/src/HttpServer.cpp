#include "../includes/HttpServer.hpp"

HttpServer::HttpServer(int domain, int service, int protocol, int port, u_long interface, int backlog) 
		: SimpleServer(domain, service, protocol, port, interface, backlog) {

	_serverSocketFd = getSocket().getSocketFD();

	std::cout << timeStamp() << MAGENTA << " HttpServer constructor called." << RESET;
	std::cout << " server_socket_fd: [" << _serverSocketFd << "]" << std::endl;

	// Launching the server.. `_accept()` >>> `_handle()` >>> `_respond()`
	run();
}

HttpServer::~HttpServer() {
	
	std::cout << RED << "\t[~] HttpServer destructor called." << RESET << std::endl;
}

void	HttpServer::_accept(int fd) {
	//std::cout << CYAN << "..in _accept().." << RESET << std::endl;

	struct sockaddr_in	address = getSocket().getAddress();
	int					addrlen = sizeof(address);
	int					ftcnl_ret = 0;

	fd = accept(_serverSocketFd, (struct sockaddr *)&address, (socklen_t *)&addrlen);
	if (fd < 0) {
		std::cerr << RED << "\t[-] Error accepting connection.. accept() failed." << RESET << std::endl;
		return ;
	}

	std::cout << timeStamp() << GREEN << "[+] Client connection accepted on fd:[" << fd << "], " << "IP:[" << inet_ntoa(address.sin_addr) << "], client side port:[" << address.sin_port << "]" << RESET << std::endl;

	// Adding the client socket FD to the recv set_fd pool
	_addToSet(fd, &_recv_fd_pool);

	// Setting the client socket to non-blocking mode so that `read()` or `recv()` won't block the server execution
	ftcnl_ret = fcntl(fd, F_SETFL, O_NONBLOCK);
	if (ftcnl_ret < 0) {
		std::cerr << RED << "\t[-] Error setting the socket to non-blocking mode.. fcntl() failed." << RESET << std::endl;
		_closeConnection(fd);
		return ;
	}
}

void	HttpServer::_handle(int fd) {
	//std::cout << CYAN << "..in _handle().." << RESET << std::endl;

	char			buffer[BUF_SIZE] = {0};
	int				bytes_read = 0;

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

	_requestBuffer.assign(buffer, bytes_read);

	std::cout << CYAN << " Request has been received from the client, on the socked:[" << fd << "]" << RESET << std::endl;

	HttpRequest 	httpRequest(_requestBuffer);
	//std::cout << requestBuffer << std::endl;

	// next step is to build the response but not send it yet
	HttpResponse	response(&httpRequest);
	_responseBuffer = response.getResponse();

	// if there is a body in the request, cgi should be set to true and handled differently
	// if (httpRequest.getBodyBuffer().length() > 0)
	// 	//cgi_state = true;
	// ...

	// at last we remove the client fd from the recv set and add it to the send set to send the response
	_removeFromSet(fd, &_recv_fd_pool);
	_addToSet(fd, &_send_fd_pool);
}

void	HttpServer::_respond(int fd) {
	//std::cout << CYAN << "..in _respond().." << RESET << std::endl;

	int	bytes_sent = 0;
	int	bytes_to_send = _responseBuffer.length();

	bytes_sent = send(fd, _responseBuffer.c_str(), bytes_to_send, 0);

	std::cout << timeStamp();

	if (bytes_sent < 0) {
		std::cerr << RED << "[-] Error sending data to the client.. send() failed." << RESET << std::endl;
		_closeConnection(fd);
		return ;
	}
	else if (bytes_sent == 0) {
		std::cout << YELLOW << "[!] Connection closed by the client. ";
		_closeConnection(fd);
		return ;
	}
	else {
		std::cout << GREEN << "[+] Response has been sent to the client, socket:[" << fd << "]" << RESET << std::endl;
	}

	_removeFromSet(fd, &_send_fd_pool);
	_addToSet(fd, &_recv_fd_pool);
}

/*
** This is the main loop of the server with the `select()` function.
**
** Currently not working properly.. need to fix it.
*/
void	HttpServer::run() {

	std::cout << timeStamp() << GREEN << " LAUNCHING SERVER.. HttpServer::run().." << RESET << std::endl;

	fd_set			recv_set_cpy;
	fd_set			send_set_cpy;
	int				select_ret;

	_max_fd = 0;
	_initFdSets();

	while (true) {

		recv_set_cpy = _recv_fd_pool;
		send_set_cpy = _send_fd_pool;

		select_ret = select(_max_fd + 1, &recv_set_cpy, &send_set_cpy, NULL, NULL);
		checkErrorAndExit(select_ret, "select() failed. Exiting..");

		for (int fd = 3; fd <= _max_fd; fd++) {

			if (FD_ISSET(fd, &recv_set_cpy) && fd == _serverSocketFd) {

				_accept(fd);
			}
			else if (FD_ISSET(fd, &recv_set_cpy) ) { // && _clients_map.count(fd)) // means that the client is connected 

				_handle(fd);
			}
			else if (FD_ISSET(fd, &send_set_cpy) ) { // && _clients_map.count(fd))

				_respond(fd);

				// the logic here shall also include cgi handling.. 
			} 
		}
		// check for timeout ??
	}
}

void	HttpServer::_initFdSets() {

	FD_ZERO(&_recv_fd_pool);
	FD_ZERO(&_send_fd_pool);
	int	fcntl_ret = 0;

	// Adding the server socket FD to the set
	// Would require to change the logic here if there will be more than one server socket

	fcntl_ret = fcntl(_serverSocketFd, F_SETFL, O_NONBLOCK);
	checkErrorAndExit(fcntl_ret, "fcntl() failed. Exiting..");

	_addToSet(_serverSocketFd, &_recv_fd_pool);

	_max_fd = _serverSocketFd; 
}

void	HttpServer::_addToSet(int fd, fd_set *set) {

	if (fd > _max_fd) {
		_max_fd = fd;
	}
	FD_SET(fd, set);
}

void	HttpServer::_removeFromSet(int fd, fd_set *set) {

	if (fd == _max_fd) {
		_max_fd--;
	}
	FD_CLR(fd, set);
}

void	HttpServer::_closeConnection(int fd) {

	std::cout << YELLOW << "[!] Closing connection fd [" << fd << "] on the server side." << RESET << std::endl;

	if (FD_ISSET(fd, &_recv_fd_pool)) {
		_removeFromSet(fd, &_recv_fd_pool);
	}
	if (FD_ISSET(fd, &_send_fd_pool)) {
		_removeFromSet(fd, &_send_fd_pool);
	}
	close(fd);
	//_clients_map.erase(fd);
}

std::string	HttpServer::timeStamp() {

	std::time_t currentTime = std::time(NULL);
    std::tm* now = std::localtime(&currentTime);
    char buffer[64];
    std::strftime(buffer, sizeof(buffer), "[%d/%m/%Y %H:%M:%S]", now);

	return std::string(buffer);
}

void	HttpServer::checkErrorAndExit(int returnValue, const std::string& msg) {

	if (returnValue < 0) {
		std::cerr << RED << "\t[-] " << msg << RESET << std::endl;
		exit(EXIT_FAILURE);
	}
}
