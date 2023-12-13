#include "../includes/HttpServer.hpp"

HttpServer::HttpServer(int domain, int service, int protocol, int port, u_long interface, int backlog) 
		: SimpleServer(domain, service, protocol, port, interface, backlog) {

	std::cout << "[" << timeStamp() << "] " << MAGENTA << "\tHttpServer constructor called." << RESET;
	std::cout << " server_socket_fd: [" << getSocket().getSocketFD() << "]" << std::endl;

	_serverSocketFd = getSocket().getSocketFD();
	_newSocketFd = -1;

	// Launching the server.. `_accept()` >>> `_handle()` >>> `_respond()`
	run();
}

HttpServer::~HttpServer() {
	
	std::cout << RED << "\t[~] HttpServer destructor called." << RESET << std::endl;
}

/*
void	HttpServer::_accept() {

	std::cout << "[" << timeStamp() << "] " << MAGENTA << "in `_accept()`.." << RESET << std::endl;
	
	struct sockaddr_in	address = getSocket().getAddress();
	int					addrlen = sizeof(address);

	_newSocketFd = accept(_serverSocketFd, (struct sockaddr *)&address, (socklen_t *)&addrlen);

	if (_newSocketFd < 0) {
		std::cerr << RED << "\t[-] Error accepting connection.. accept() failed." << RESET << std::endl;
		exit(EXIT_FAILURE);
	}

	std::cout << GREEN << "[+] Connection accepted. _newSocketFd:[" << _newSocketFd << "], " << "IP: " << inet_ntoa(address.sin_addr) << RESET << std::endl;
}
** !! ?? `ephemeral port` is a short-lived transport protocol port for Internet Protocol (IP) communications.
** It was set by the kernel on the client side, and was used by the server to send back responses.
** The client will use the same port number for the duration of the current connection.
*/

/*
** This is where the request from the web-browser is parsed.
** `HttpRequest` class is used to parse the request and 
** store the individual parts of the request in the class members.
*/
void	HttpServer::_handleRequestAndResponse() {

	std::cout << "[" << timeStamp() << "] " << MAGENTA << "in `_handle()`.. passing request from web-browser to the `HttpRequest` class." << RESET;
	std::cout << " server_socket_fd: [" << getSocket().getSocketFD() << "]" << std::endl;
	std::cout << std::endl;

	std::cout << MAGENTA << "Handling request from the client.." << RESET << std::endl;
	HttpRequest httpRequest(_newSocketFd);

	std::cout << BLUE << "Checking parsed data:" << RESET << std::endl;
	std::cout << CYAN << "Method:\t" << RESET << httpRequest.getMethod() << std::endl;
	std::cout << CYAN << "URI:   \t" << RESET << httpRequest.getUri() << std::endl;
	std::cout << CYAN << "HTTP V:\t" << RESET << httpRequest.getHttpVersion() << std::endl;
	std::cout << CYAN << "Body:" << RESET << std::endl;
	std::cout << httpRequest.getBodyBuffer() << std::endl;

	std::cout << MAGENTA << "Generating response.." << RESET << std::endl;

	HttpResponse	response(&httpRequest);

	std::string		responseString = response.getResponse();

	std::cout << BLUE << "Response string:" << RESET << std::endl;
	std::cout << responseString << std::endl;

	send(_newSocketFd, responseString.c_str(), responseString.length(), 0);
	//write(_newSocketFd, responseString.c_str(), responseString.length());

	std::cout << GREEN << "Responce message sent!" << RESET << std::endl;
}

/*
** This is the main loop of the server.
** This will allow only one connection at a time.
void	HttpServer::run() {

	std::cout << GREEN << "\tHttpServer::run() called." << RESET << std::endl;

	while (1) {

		_accept();
		_handleRequestAndResponse();

  
		// Closing connection with client
		close(_newSocketFd);
		_newSocketFd = -1;
	}
}
*/

void	HttpServer::_accept() {

	struct sockaddr_in	address = getSocket().getAddress();
	int					addrlen = sizeof(address);
	int					ftcnl_ret = 0;

	_newSocketFd = accept(_serverSocketFd, (struct sockaddr *)&address, (socklen_t *)&addrlen);
	if (_newSocketFd < 0) {
		std::cerr << RED << "\t[-] Error accepting connection.. accept() failed." << RESET << std::endl;
		return ;
	}

	std::cout << GREEN << "[+] Connection accepted. _newSocketFd:[" << _newSocketFd << "], " << "IP: " << inet_ntoa(address.sin_addr) << RESET << std::endl;

	//_clients_map.insert(std::pair<int, Client*>(_newSocketFd, new Client(_newSocketFd)));
	_addToSet(_newSocketFd, &_recv_fd_pool);

	ftcnl_ret = fcntl(_newSocketFd, F_SETFL, O_NONBLOCK);
	if (ftcnl_ret < 0) {
		std::cerr << RED << "\t[-] Error setting the socket to non-blocking mode.. fcntl() failed." << RESET << std::endl;
		_closeConnection(_newSocketFd);
		return ;
	}
	_newSocketFd = -1;
}

void	HttpServer::_handle(int fd) {

	char			buffer[BUF_SIZE] = {0};
	int				bytes_read = 0;

	memset(buffer, 0, BUF_SIZE);

	bytes_read = read(fd, buffer, BUF_SIZE);
	if (bytes_read == 0) {
		std::cout << YELLOW << "\t[!] Connection closed by the client." << RESET << std::endl;
		_closeConnection(fd);
		return ;
	}
	else if (bytes_read < 0) {
		std::cerr << RED << "\t[-] Error reading data from the client.. read() failed." << RESET << std::endl;
		_closeConnection(fd);
		return ;
	}
	else {
		// need to update time stamp on of the client object
		// converting the buffer to a string and storing it in the client object
		_requestBuffer.assign(buffer, bytes_read);
	}

	std::cout << CYAN << "Request has been received from the client, on the socked:[" << fd << "]" << RESET << std::endl;
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

	std::cout << CYAN << "Responding to the client, on the socked:[" << fd << "]" << RESET << std::endl;

	int	bytes_sent = 0;
	int	bytes_to_send = _responseBuffer.length();

	bytes_sent = send(fd, _responseBuffer.c_str(), bytes_to_send, 0);
	if (bytes_sent < 0) {
		std::cerr << RED << "\t[-] Error sending data to the client.. send() failed." << RESET << std::endl;
		_closeConnection(fd);
		return ;
	}
	else if (bytes_sent == 0) {
		std::cout << YELLOW << "\t[!] Connection closed by the client." << RESET << std::endl;
		_closeConnection(fd);
		return ;
	}
	else {
		std::cout << GREEN << "\t[+] Response has been sent to the client, on the socked:[" << fd << "]" << RESET << std::endl;
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

	std::cout << GREEN << "\tHttpServer::run() called." << RESET << std::endl;

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

				_accept();
			}
			else if (FD_ISSET(fd, &recv_set_cpy) ) { // && _clients_map.count(fd)) // means that the client is connected 

				_handle(fd);
			}
			else if (FD_ISSET(fd, &send_set_cpy) ) { // && _clients_map.count(fd))

				// the logic here shall include cgi handling depends on the cgi_state..
				_respond(fd);
			} 
		}
		// check for timeout
	}
}

void	HttpServer::_initFdSets() {

	//_serverSocketFd = getSocket().getSocketFD();
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
	std::time_t currentTime = time(NULL);
	std::string dt = std::ctime(&currentTime);
	dt.erase(dt.length() - 6);

	return dt;
}

void	HttpServer::checkErrorAndExit(int returnValue, const std::string& msg) {

	if (returnValue < 0) {
		std::cerr << RED << "\t[-] " << msg << RESET << std::endl;
		exit(EXIT_FAILURE);
	}
}
