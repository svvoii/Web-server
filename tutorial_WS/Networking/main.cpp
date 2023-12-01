#include "HeaderSockets.hpp"
#include "Servers/TestServer.hpp"

int	main() {

	/*
	std::cout << "Starting server..." << std::endl;

	std::cout << "Binding socket..." << std::endl;
	BindingSocket	bindingSocket(AF_INET, SOCK_STREAM, 0, 8081, INADDR_ANY);

	std::cout << "Listening to the network..." << std::endl;
	ListeningSocket	listeningSocket(AF_INET, SOCK_STREAM, 0, 8080, INADDR_ANY, 10);

	std::cout << "Success!" << std::endl;
	*/

	TestServer	server;
}