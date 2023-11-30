#ifndef HEADERSOCKETS_HPP
#define HEADERSOCKETS_HPP


// Parent class (abstract)
#include "Sockets/SimpleSocket.hpp"

// Child class of SimpleSocket for server side socket
#include "Sockets/BindingSocket.hpp"
// Child class of BindingSocket
#include "Sockets/ListeningSocket.hpp"

// Child class of SimpleSocket for client side socket
#include "Sockets/ConnectingSocket.hpp"


#endif
