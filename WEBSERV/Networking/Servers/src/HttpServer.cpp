#include "../includes/HttpServer.hpp"

HttpServer::HttpServer(int domain, int service, int protocol, int port, u_long interface, int backlog) 
		: SimpleServer(domain, service, protocol, port, interface, backlog) {

	std::cout << MAGENTA << "\tHttpServer constructor called." << RESET;
	std::cout << " server_socket_fd: [" << getSocket().getSocketFD() << "]" << std::endl;

	_newSocketFd = -1;
	//httpRequest = NULL;

	// Launching the server.. `_accept()` >>> `_handle()` >>> `_respond()`
	run();
}

HttpServer::~HttpServer() {
	
	std::cout << RED << "\t[~] HttpServer destructor called." << RESET << std::endl;

	/*
	// Deleting the `HttpRequest` object on proper server shutdown
	if (httpRequest != NULL) {
		delete httpRequest;
		httpRequest = NULL;
	}
	*/
}

void	HttpServer::_accept() {

	std::cout << std::endl;
	std::cout << MAGENTA << "in `_accept()`.." << RESET << std::endl;
	
	struct sockaddr_in	address = getSocket().getAddress();
	int					addrlen = sizeof(address);

	_newSocketFd = accept(getSocket().getSocketFD(), (struct sockaddr *)&address, (socklen_t *)&addrlen);

	if (_newSocketFd < 0) {
		std::cerr << RED << "\t[-] Error accepting connection.. accept() failed." << RESET << std::endl;
		exit(EXIT_FAILURE);
	}

	std::cout << GREEN << "[+] Connection accepted. _newSocketFd:[" << _newSocketFd << "], " << "IP: " << inet_ntoa(address.sin_addr) << RESET << std::endl;
}
/*
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

	std::cout << MAGENTA << "in `_handle()`.. passing request from web-browser to the `HttpRequest` class." << RESET;
	std::cout << " server_socket_fd: [" << getSocket().getSocketFD() << "]" << std::endl;
	std::cout << std::endl;

	std::cout << MAGENTA << "Handling request from the client.." << RESET << std::endl;
	HttpRequest httpRequest(_newSocketFd);

	std::cout << BLUE << "Checking parsed data:" << RESET << std::endl;
	//std::cout << std::endl;
	std::cout << CYAN << "Method:\t" << RESET << httpRequest.getMethod() << std::endl;
	std::cout << CYAN << "URI:   \t" << RESET << httpRequest.getUri() << std::endl;
	std::cout << CYAN << "HTTP V:\t" << RESET << httpRequest.getHttpVersion() << std::endl;
	std::cout << CYAN << "Body:" << RESET << std::endl;
	std::cout << httpRequest.getBodyBuffer() << std::endl;
	std::cout << std::endl;

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
*/
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

/*
** This is the main loop of the server with the `select()` function.
**
** Currently not working properly.. need to fix it.
*/