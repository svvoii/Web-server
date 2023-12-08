#include "HttpRequest.hpp"

HttpRequest::HttpRequest(const std::string& buffer) {

	std::cout << MAGENTA;
	std::cout << "\tHttpRequest constructor called." << std::endl;
	std::cout << "Here is the request from the browser:";
	std::cout << RESET << std::endl;
	std::cout << buffer << std::endl;

	std::istringstream	ss(buffer);
	std::string			line;
	std::string			body;

	std::cout << MAGENTA << "Parsing the request ..." << RESET << std::endl;
	std::cout << std::endl;

	// Parsing each line into map by the first space
	std::cout << CYAN << "..parseLine().." << RESET << std::endl;
	while (std::getline(ss, line) && line != "\n\r") {

		line = trim(line);
		//std::cout << "trimmed line: [" << line << "]" << std::endl;
		//std::flush(std::cout);

		parseLine(line);
	}
	std::cout << std::endl;

	// Extracting the request line, method, uri, http version
	parseRequestLine();

	// Extracting the body of the request
	// TODO: Check if the request has a body
}

HttpRequest::~HttpRequest() {
	
	std::cout << RED << "\t[~] HttpRequest destructor called." << RESET << std::endl;
}

typedef std::map<std::string, std::string>::iterator MapIterator;
void	HttpRequest::parseRequestLine() {

	std::cout << CYAN << "in prseRequestLine().." << RESET << std::endl;
	//std::cout << "line:\t" << line << std::endl;
	std::cout << std::endl;

	for (MapIterator it = _headers.begin(); it != _headers.end(); it++) {
		/*
		std::cout << "\tkey:  [" << it->first << "]" << std::endl;
		std::cout << "\tvalue:[" << it->second << "]" << std::endl;
		*/
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

			_headers.erase(it);

			break ;
		}
	}
}

void	HttpRequest::parseLine(const std::string& line) {

	//std::cout << CYAN << "in parseLine().." << RESET << std::endl;

	std::istringstream	ss(line);
	std::string			key;
	std::string			value;

	std::getline(ss, key, ' ');
	std::getline(ss, value);
	
	key = trim(key);
	value = trim(value);

	//std::cout << "\tkey:  [" << key << "]" << std::endl;
	//std::cout << "\tvalue:[" << value << "]" << std::endl;

	if (key.length() != 0 || value.length() != 0) {
		_headers.insert(std::make_pair(key, value));
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
	else {
		return NONE;
	}
}
