#include "../includes/HttpRequest.hpp"

HttpRequest::HttpRequest(const std::string& buffer) {

	std::cout << MAGENTA;
	std::cout << "\tHttpRequest constructor called." << std::endl;
	std::cout << "Here is the request from the browser:";
	std::cout << RESET << std::endl;
	std::cout << buffer << std::endl;

	std::istringstream	ss(buffer);
	std::string			line;

	std::cout << MAGENTA << "Parsing the request ..." << RESET << std::endl;
	std::cout << std::endl;

	// Parsing each line into map by the first space
	std::cout << CYAN << "..parseLine().." << RESET << std::endl;
	while (std::getline(ss, line) && !line.empty() && line != "\r") {

		line = trim(line);
		//std::cout << "trimmed line: [" << line << "]" << std::endl;
		//std::flush(std::cout);

		parseHeaders(line);
	}
	std::cout << std::endl;

	// Extracting the request line, method, uri, http version
	extractRequestLine();

	// Extracting the body of the request
	while (std::getline(ss, line)) {
		_bodyBuffer += line;
	}
	
	if (_bodyBuffer.length() != 0) {
		parseBody();
	}
}

HttpRequest::~HttpRequest() {
	
	std::cout << RED << "\t[~] HttpRequest destructor called." << RESET << std::endl;
}

void	HttpRequest::parseHeaders(const std::string& line) {

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

/*
** using typedef on the map iterator to make it easier to read
*/
typedef std::map<std::string, std::string>::iterator MapIterator;
void	HttpRequest::extractRequestLine() {

	std::cout << CYAN << "in prseRequestLine().." << RESET << std::endl;
	std::cout << std::endl;

	for (MapIterator it = _headers.begin(); it != _headers.end(); it++) {

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

void	HttpRequest::parseBody() {

	std::cout << CYAN << "in parseBody().." << RESET << std::endl;
	
	std::string		contentLength = _headers["Content-Length"];
	std::string		contentType = _headers["Content-Type"];

	std::cout << "\tcontentLength:[" << contentLength << "]" << std::endl;
	std::cout << "\tcontentType:  [" << contentType << "]" << std::endl;

	if (contentType == "application/x-www-form-urlencoded" 
		|| contentType == "multipart/form-data") {
		parseFormUrlData(_bodyBuffer);
	}
	else if (contentType == "application/json") {
		parseJsonData(_bodyBuffer);
	}
	else if (contentType == "text/xml" || contentType == "application/xml") {
		parseXmlData(_bodyBuffer);
	}
	else if (contentType == "text/plain") {
		// No parsing needed _bodyBuffer is a plain text already
		std::cout << "\tNo parsing needed _bodyBuffer is a plain text already" << std::endl;
	}
	else {
		std::cout << RED << "Error: Unknown Content-Type" << RESET << std::endl;
	}	

}

/*
If you want to make a POST request with data in the body, use this:

```bash
curl -X POST -d "key1=value1&key2=value2" http://localhost:8080/
```
In this command:
- `-X POST` specifies the HTTP method to use.
- `-d "key1=value1&key2=value2"` includes the data to send with the request. 
	This data is in the format of a query string.
- `http://localhost:8080/` is the URL to send the request to.

When this request is received by the server, 
the data will be in the body of the HTTP request. 
The server can then parse this data as needed. 
In your `HttpRequest` class, this would be handled by the 
`parseFormUrlData` method bellow 
if the `Content-Type` header is `application/x-www-form-urlencoded`.
*/
void	HttpRequest::parseFormUrlData(const std::string& data) {

	std::cout << CYAN << "in parseFormUrlData().." << RESET << std::endl;

	std::istringstream	ss(data);
	std::string			line;

	while (std::getline(ss, line, '&')) {

		std::istringstream	ssLine(line);
		std::string			key;
		std::string			value;

		std::getline(ssLine, key, '=');
		std::getline(ssLine, value);

		key = trim(key);
		value = trim(value);

		_body.insert(std::make_pair(key, value));
	}
}

/*
** This example of JSON parsing will not work for most cases
** with nested objects and arrays.
** It is just a simple example for testing purposes.
*/
void	HttpRequest::parseJsonData(const std::string& data) {

	std::cout << CYAN << "in parseJsonData().." << RESET << std::endl;

	std::istringstream	ss(data);
	std::string			line;

	while (std::getline(ss, line, ',')) {

		std::istringstream	ssLine(line);
		std::string			key;
		std::string			value;

		std::getline(ssLine, key, ':');
		std::getline(ssLine, value);

		key = trim(key);
		value = trim(value);

		_body.insert(std::make_pair(key, value));
	}

}

/*
** This example of XML parsing will NOT work for most cases.
** It is just a simple example for testing purposes.
*/
void	HttpRequest::parseXmlData(const std::string& data) {

	std::cout << CYAN << "in parseXmlData().." << RESET << std::endl;

	std::istringstream	ss(data);
	std::string			line;

	while (std::getline(ss, line)) {

		std::istringstream	ssLine(line);
		std::string			key;
		std::string			value;

		std::getline(ssLine, key, '>');
		std::getline(ssLine, value, '<');

		key = trim(key);
		value = trim(value);

		if (key.length() != 0 || value.length() != 0) {
			_body.insert(std::make_pair(key, value));	
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

std::map<std::string, std::string>	HttpRequest::getBody() {
	return _body;
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
