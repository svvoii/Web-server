#include "../includes/HttpRequest.hpp"

/*
*/
HttpRequest::HttpRequest(const std::string& buffer) {

	std::cout << MAGENTA;
	std::cout << "\tHttpRequest constructor called." << std::endl;
	std::cout << "Here is the request from the browser:";
	std::cout << RESET << std::endl;
	std::cout << buffer << std::endl;

	std::istringstream	ss(buffer);
	std::string			line;

	std::cout << MAGENTA << "Parsing HTTP request ..." << RESET << std::endl;
	//std::cout << std::endl;

	// Parsing each line into map by the first space
	//std::cout << CYAN << "..parseLine().." << RESET << std::endl;
	while (std::getline(ss, line) && !line.empty() && line != "\r") {

		line = trim(line);
		//std::cout << "trimmed line: [" << line << "]" << std::endl;
		//std::flush(std::cout);

		_parseHeaders(line);
	}
	std::cout << std::endl;

	// Extracting the request line, method, uri, http version
	_extractRequestLine();

	// Extracting the body of the request
	while (std::getline(ss, line)) {
		_bodyBuffer += line;
	}
}

HttpRequest::HttpRequest(int socketFd) 
	: _buffRequest(""), _method(NONE), _uriPath(""), _httpVersion(""), _bodyBuffer("") {

	std::cout << MAGENTA << "\tHttpRequest constructor called." << RESET << std::endl;
	std::cout << CYAN << "Reading request from the browser.." << RESET << std::endl;

	memset(_buff, 0, BUF_SIZE);

	// Reading the request from the browser/client
	// int bytes_read = read(socketFd, _buff, BUF_SIZE);
	int bytes_read = recv(socketFd, _buff, BUF_SIZE, 0);
	if (bytes_read < 0) {
		std::cerr << RED << "\t[-] Error receiving data from the web-browser.. recv() failed." << RESET << std::endl;
		exit(EXIT_FAILURE);
	}
	else if (bytes_read == 0) {
		std::cout << RED << "\t[-] Connection closed by the web-browser." << RESET << std::endl;
		exit(EXIT_FAILURE);
	}
	else {
		std::cout << GREEN << "\tData received from the web-browser and added to the `_buffer`, bytesRead: [" << bytes_read << "]." << RESET << std::endl;
		_buffRequest.assign(_buff, bytes_read);
	}	

	std::cout << BLUE << "Buffer contents:" << RESET << std::endl;
	std::cout << _buffRequest << std::endl;
	//std::cout << std::endl;
	//std::cout << BLUE << "Buffer contents in HEX:" << RESET << std::endl;
	//_printInHEX(_buff, bytes_read);

	_parsing();

}

HttpRequest::~HttpRequest() {
	
	std::cout << RED << "\t[~] HttpRequest destructor called." << RESET << std::endl;
}

void	HttpRequest::_parsing() {
	std::cout << MAGENTA << "Parsing HTTP request ..." << RESET << std::endl;

	std::istringstream	ss(_buffRequest);
	std::string			line;

	while (std::getline(ss, line) && !line.empty() && line != "\r") {

		line = trim(line);

		_parseHeaders(line);
	}
	std::cout << std::endl;

	// Extracting the request line, METHOD, URI/PATH, HTTP version
	_extractRequestLine();

	// Extracting the body of the request
	while (std::getline(ss, line)) {
		_bodyBuffer += line;
	}

}

void	HttpRequest::_parseHeaders(const std::string& line) {

	//std::cout << CYAN << "in parseLine().." << RESET << std::endl;

	std::istringstream	ss(line);
	std::string			key;
	std::string			value;

	std::getline(ss, key, ' ');
	std::getline(ss, value);
	
	key = trim(key);
	value = trim(value);

	if (key.length() != 0 || value.length() != 0) {
		_headers.insert(std::make_pair(key, value));
	}

}

/*
** using typedef on the map iterator to make it easier to read
*/
typedef std::map<std::string, std::string>::iterator MapIterator;
void	HttpRequest::_extractRequestLine() {

	for (MapIterator it = _headers.begin(); it != _headers.end(); it++) {

		// Checking if the first word of the line is a METHOD (enum requestMethod), NONE = 0
		if (isMethod(it->first)) {
			_method = isMethod(it->first);

			std::istringstream	ss(it->second);
			std::string			word;

			// Get the URI path
			std::getline(ss, word, ' ');
			_uriPath = word;

			// Get the HTTP version
			std::getline(ss, word, ' ');
			_httpVersion = word;

			// Removing request line from headers map
			_headers.erase(it);

			break ;
		}
	}
}

// Getters
std::string	HttpRequest::getMethod() {
	
	if (_method == GET) {
		return "GET";
	}
	else if (_method == POST) {
		return "POST";
	}
	else if (_method == PUT) {
		return "PUT";
	}
	else if (_method == DELETE) {
		return "DELETE";
	}
	else if (_method == OPTIONS) {
		return "OPTIONS";
	}
	else {
		return "NONE";
	}
}

std::string	HttpRequest::getUri() {
	return _uriPath;
}

std::string	HttpRequest::getHttpVersion() {
	return _httpVersion;
}

std::string	HttpRequest::getBodyBuffer() {
	return _bodyBuffer;
}

std::map<std::string, std::string>	HttpRequest::getHeaders() {
	return _headers;
}

/*
** Clean parsing helpers.
**
** `trim()` removes unnecessary spaces and new line characters etc.
*/
std::string	HttpRequest::trim(const std::string& str) {

	std::string	trimmed;

	trimmed = str;

	trimmed.erase(trimmed.find_last_not_of('\r') + 1);
	trimmed.erase(trimmed.find_last_not_of('\n') + 1);
	trimmed.erase(0, trimmed.find_first_not_of('\t'));
	trimmed.erase(trimmed.find_last_not_of('\t') + 1);
	trimmed.erase(0, trimmed.find_first_not_of(' '));
	trimmed.erase(trimmed.find_last_not_of(' ') + 1);
	trimmed.erase(trimmed.find_last_not_of(':') + 1);

	return trimmed;
}

enum requestMethod	HttpRequest::isMethod(const std::string& str) {

	if (str == "GET") {
		return GET;
	}
	else if (str == "POST") {
		return POST;
	}
	else if (str == "PUT") {
		return PUT;
	}
	else if (str == "DELETE") {
		return DELETE;
	}
	else if (str == "OPTIONS") {
		return OPTIONS;
	}
	else {
		return NONE;
	}
}

void	HttpRequest::_printInHEX(char *buff, int len) {

	std::cout << "[";
	for (int i = 0; i < len; i++) {
		//std::cout << std::hex << (int)buff[i] << " ";
		std::cout << std::hex << std::setw(2) << std::setfill('0') << (int)buff[i] << " ";
	}
	std::cout << "]" << std::endl;
}
