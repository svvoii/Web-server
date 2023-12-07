#ifndef HTTPRESPONSE_HPP
#define HTTPRESPONSE_HPP

#include "HttpRequest.hpp"

class HttpResponse {
	private:

		std::string		_response;

		void			_generateResponse(HttpRequest *httpRequest);

	public:

		HttpResponse(HttpRequest *httpRequest);
		~HttpResponse();

		std::string		getResponse();
};

#endif
