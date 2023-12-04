#include "HeaderSockets.hpp"
#include "Servers/TestServer.hpp"
#include "Servers/HttpServer.hpp"

int	main() {

	/*
	std::cout << "Starting server..." << std::endl;

	std::cout << "Binding socket..." << std::endl;
	BindingSocket	bindingSocket(AF_INET, SOCK_STREAM, 0, 8081, INADDR_ANY);

	std::cout << "Listening to the network..." << std::endl;
	ListeningSocket	listeningSocket(AF_INET, SOCK_STREAM, 0, 8080, INADDR_ANY, 10);

	std::cout << "Success!" << std::endl;
	*/

	//TestServer	server;
	
	// All arguments are for TESTING PURPOSES, declared in `TestServer.hpp`
	HttpServer server(AF_INET, SOCK_STREAM, PROTOCOL, PORT, INADDR_ANY, BACKLOG);
}