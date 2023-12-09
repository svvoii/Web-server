#include "../includes/HttpServer.hpp"

HttpServer::HttpServer(int domain, int service, int protocol, int port, u_long interface, int backlog) 
		: SimpleServer(domain, service, protocol, port, interface, backlog) {

	std::cout << MAGENTA << "\tHttpServer constructor called." << RESET << std::endl;

	_buffRequest = "";
	_new_socket = -1;
	httpRequest = NULL;

	// Launching the server.. `_accept()` >>> `_handle()` >>> `_respond()`
	run();
}

HttpServer::~HttpServer() {
	
	std::cout << RED << "\t[~] HttpServer destructor called." << RESET << std::endl;

	// Deleting the `HttpRequest` object on proper server shutdown
	if (httpRequest != NULL) {
		delete httpRequest;
		httpRequest = NULL;
	}
}

void	HttpServer::_accept() {

	std::cout << std::endl;
	std::cout << MAGENTA << "in `_accept()`.." << RESET << std::endl;
	
	struct sockaddr_in	address = getSocket().getAddress();
	int					addrlen = sizeof(address);

	_new_socket = accept(getSocket().getSocketFD(), 
					(struct sockaddr *)&address, 
					(socklen_t *)&addrlen);

	if (_new_socket < 0) {
		std::cerr << RED << "\t[-] Error accepting connection.. accept() failed." << RESET << std::endl;
		exit(EXIT_FAILURE);
	}

	std::cout << GREEN;
	std::cout << "[+] Connection accepted." << std::endl;
	std::cout << "IP: " << inet_ntoa(address.sin_addr) << std::endl;
	std::cout << BLUE;
	// !! emepheral port* ??
	std::cout << "Port (ephemeral) ntohs[" << ntohs(address.sin_port) << "], (client side)" << std::endl;
	std::cout << "Saving the request from the web-browser in `_buffer` with `recv()`." << std::endl;
	std::cout << RESET;

	char buff[BUF_SIZE] = {0};
	int bytesRead = recv(_new_socket, buff, BUF_SIZE, 0);

	if (bytesRead < 0) {
		std::cerr << RED << "\t[-] Error receiving data from the web-browser.. recv() failed." << RESET << std::endl;
		exit(EXIT_FAILURE);
	}
	else if (bytesRead == 0) {
		std::cout << RED << "\t[-] Connection closed by the web-browser." << RESET << std::endl;
		exit(EXIT_FAILURE);
	}
	else {
		std::cout << GREEN << "Data received from the web-browser and added to the `_buffer`, bytesRead: [" << bytesRead << "]." << RESET << std::endl;
		_buffRequest.assign(buff, bytesRead);
	}
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
void	HttpServer::_handle() {

	std::cout << MAGENTA << "in `_handle()`.. passing request from web-browser to the `HttpRequest` class." << RESET << std::endl;
	std::cout << std::endl;

	// Passing the request from the web-browser to the `HttpRequest` class for parsing
	// Should be allocated on the heap in this case to use it in `_respond()` method
	httpRequest = new HttpRequest(_buffRequest);

	std::cout << BLUE << "Checking parsed data and Getters of `HttpRequest` class:" << RESET << std::endl;
	std::cout << std::endl;
	std::cout << CYAN << "Method:\t" << RESET << httpRequest->getMethod() << std::endl;
	std::cout << CYAN << "URI:   \t" << RESET << httpRequest->getUri() << std::endl;
	std::cout << CYAN << "HTTP V:\t" << RESET << httpRequest->getHttpVersion() << std::endl;

	std::cout << std::endl;
	std::cout << CYAN << "Headers:" << RESET << std::endl;
	std::map<std::string, std::string> headers = httpRequest->getHeaders();

	for (std::map<std::string, std::string>::iterator it = headers.begin(); it != headers.end(); ++it) {
		std::cout << "\tkey[" << it->first << "]:\tvalue[" << it->second << "]" << std::endl;
	}
	std::cout << std::endl;

	std::cout << CYAN << "Body:" << RESET << std::endl;
	std::cout << httpRequest->getBodyBuffer() << std::endl;
	std::cout << std::endl;

	std::map<std::string, std::string> body = httpRequest->getBody();

	for (std::map<std::string, std::string>::iterator it = body.begin(); it != body.end(); ++it) {
		std::cout << "\tkey[" << it->first << "]:\tvalue[" << it->second << "]" << std::endl;
	}

}

void	HttpServer::_respond() {

	std::cout << MAGENTA << "in `_respond()`.." << RESET << std::endl;

	// Passing the request from the web-browser to the `HttpResponse` class for generating the response
	HttpResponse	response(httpRequest);

	std::string		responseString = response.getResponse();

	/*
	std::cout << GREEN << "Response:" << RESET << std::endl;
	std::cout << responseString << std::endl;
	*/

	send(_new_socket, responseString.c_str(), responseString.length(), 0);

	std::cout << GREEN; 
	std::cout << "Responce message sent" << std::endl;
	std::cout << "Closing connection" << std::endl;
	std::cout << RESET;

	close(_new_socket);

	// Deleting the `HttpRequest` object right after the response is sent to avoid memory leaks
	delete httpRequest;
	httpRequest = NULL;
}

void	HttpServer::run() {

	std::cout << GREEN << "\tHttpServer::run() called." << RESET << std::endl;

	while (1) {

		_accept();
		_handle();
		_respond();
	}
}
