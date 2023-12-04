#ifndef HEADERSOCKETS_HPP
#define HEADERSOCKETS_HPP

// SOME COLORS FOR DEBUGGING
#define RED "\033[1;31m"
#define GREEN "\033[1;32m"
#define YELLOW "\033[1;33m"
#define BLUE "\033[1;34m"
#define MAGENTA "\033[1;35m"
#define CYAN "\033[1;36m"
#define RESET "\033[0m"


// Parent class (abstract)
#include "Sockets/SimpleSocket.hpp"

// Child class of SimpleSocket for server side socket
#include "Sockets/BindingSocket.hpp"
// Child class of BindingSocket
#include "Sockets/ListeningSocket.hpp"

// Child class of SimpleSocket for client side socket
#include "Sockets/ConnectingSocket.hpp"


#endif
