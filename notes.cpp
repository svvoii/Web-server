/*
** HTTP server project in C++.
** Started on 2023-11-22 by sbocanci.
**
** This file contains my notes on the way to HTTP server in C++.
**
** My Notion page:
** https://www.notion.so/Web-Server-7fd3b9bf0dc14fd7b0222ffb596f50e8
**
*/

/*
** Understandng some basic concepts and requirements of this project.
**
** Remark from the subject:
###################################################################################
** "Please read the RFC and do some tests with telnet and NGINX before
** starting this project."
###################################################################################
** 
** `RFC` stands for `Request For Comments` and it is a document that
** contains technical details about the Internet, including the TCP/IP
** protocol suite and the HTTP and HTTPS application protocols.
**
** These documents outline the syntax, semantics, and behavior of HTTP requests 
** and responses, along with other related aspects like headers, status codes, 
** caching, and security.
**
** Telnet is a network protocol that allows you to establish a command-line 
** connection to a remote server. It is often used for testing network services 
** and troubleshooting. In the context of an HTTP server project, you can use 
** Telnet to manually send HTTP requests to the server and observe the responses.
**
** NGINX is a popular open-source web server that can also be used as a reverse
** proxy, load balancer, and HTTP cache. In the context of an HTTP server 
** NGINX can be used as a reverse proxy to forward HTTP requests to the server.
**
** The remark from the subject prompts to refer to the RFC and perform some tests
** with Telnet and NGINX by using publicly available web servers.
**
** RFCs are available on the IETF website. The RFCs that define the HTTP protocol
** at the links below:
** https://tools.ietf.org/html/rfc7230
** https://tools.ietf.org/html/rfc7231
** https://tools.ietf.org/html/rfc7232
** https://tools.ietf.org/html/rfc7233
** https://tools.ietf.org/html/rfc7234
** https://tools.ietf.org/html/rfc7235
**
** Example of a test with Telnet:
** https://www.digitalocean.com/community/tutorials/how-to-test-a-remote-port-connection-using-telnet
** It can be used to manually send HTTP requests to publicly available web servers
** and observe the responses.
** In the terminal: (tellnet <host / IP> <port>)
** `telnet www.example.com 80`
** `<host / IP>` is the hostname or IP address of the server.
** `<port>` is the port number of the server. For HTTP, usually port 80.
** Once connected to the server via Telnet, manual HTTP requests can be sent.
** (..request line, headers, empty line to indicate the end of the headers..)
** (see below)
**
** Example of a test with NGINX:
** Once NGINX is installed, it can be configured as a reverse proxy to forward
** HTTP requests to publicly available web servers.
** `nginx.conf` is the NGINX configuration file, which can be edited to configure
** for IP address or hostname and port number of the HTTP server to be tested.
** Once NGINX is running, HTTP requests can be sent to NGINX, which will forward
** them to the HTTP server.
** This will allow to send the requests and observe the responses.
**
*/

/*###################################################################################*/
/* STEPS TO INSTALL TELNET AND NGINX */
/*###################################################################################*/
/*
** Steps to setup and test HTTP server with Telnet and NGINX:
**
** 1. Install Telnet: If you don't have Telnet installed on your system, 
** you'll need to install it. The installation process varies depending on your 
** operating system. For example, on Ubuntu, you can install Telnet by running 
** `sudo apt-get install telnet`.
**
** 2. Start your HTTP server: Make sure your HTTP server is running and listening 
** on a specific port (e.g., port 8080).
**
** 3. Open a terminal and run Telnet: Open a terminal and run the Telnet command 
** followed by the IP address or hostname of your server and the port number. 
** For example, `telnet localhost 8080`.
** 
** 4. Send an HTTP request: Once connected to the server via Telnet, you can 
** manually send an HTTP request by typing the request line, headers, and an 
** empty line to indicate the end of the headers. For example:
** (for the example bellow use: `telnet www.example.com 80` and copy-paste the request)
```
GET / HTTP/1.1
Host: www.example.com
User-Agent: MyTestClient/1.0

```
** 5. Observe the server response: After sending the request, the server should 
** respond with an HTTP response. You can observe the response headers and the 
** content returned by the server.
** 
** Testing with NGINX:
**
** If you have NGINX installed on your system, you can also test your HTTP server 
** by configuring NGINX as a reverse proxy. 
** Here's a high-level overview of the steps:
**
** 1. Install NGINX. For Ubuntu: `sudo apt-get install nginx`.
** 
** 2. Configure NGINX as a reverse proxy. 
** Edit the NGINX configuration file (`nginx.conf`) and configure it to act as a 
** reverse proxy for your HTTP server. You'll need to specify the IP address or 
** hostname and the port number of your HTTP server.
** 
** 3. Start NGINX. On Ubuntu: `sudo service nginx start`.
** 
** 4. Send HTTP requests to NGINX. Once NGINX is running, you can send HTTP requests 
** to NGINX, which will forward them to your HTTP server. 
** Tools like cURL or a web browser can be used to send requests to NGINX and 
** observe the responses.
** 
**
** RFC: https://www.ietf.org/standards/
** NGINX: https://nginx.org/en/
**
*/

/*###################################################################################*/
/* EXAMPLE OF A REQUEST AND RESPONCE USING TELNET */
/*###################################################################################*/
/*
%> telnet www.example.com 80

Trying 93.184.216.34...
Connected to www.example.com.
Escape character is '^]'.
GET / HTTP/1.1
Host: www.example.com
User-Agent: MyTestClient/1.0

HTTP/1.1 200 OK
Age: 468797
Cache-Control: max-age=604800
Content-Type: text/html; charset=UTF-8
Date: Wed, 22 Nov 2023 15:35:06 GMT
Etag: "3147526947+ident"
Expires: Wed, 29 Nov 2023 15:35:06 GMT
Last-Modified: Thu, 17 Oct 2019 07:18:26 GMT
Server: ECS (bsb/279B)
Vary: Accept-Encoding
X-Cache: HIT
Content-Length: 1256

<!doctype html>
<html>
<head>
    <title>Example Domain</title>

    <meta charset="utf-8" />
    <meta http-equiv="Content-type" content="text/html; charset=utf-8" />
    <meta name="viewport" content="width=device-width, initial-scale=1" />
    <style type="text/css">
    body {
        background-color: #f0f0f2;
        margin: 0;
        padding: 0;
        font-family: -apple-system, system-ui, BlinkMacSystemFont, "Segoe UI", "Open Sans", "Helvetica Neue", Helvetica, Arial, sans-serif;
        
    }
    div {
        width: 600px;
        margin: 5em auto;
        padding: 2em;
        background-color: #fdfdff;
        border-radius: 0.5em;
        box-shadow: 2px 3px 7px 2px rgba(0,0,0,0.02);
    }
    a:link, a:visited {
        color: #38488f;
        text-decoration: none;
    }
    @media (max-width: 700px) {
        div {
            margin: 0 auto;
            width: auto;
        }
    }
    </style>    
</head>

<body>
<div>
    <h1>Example Domain</h1>
    <p>This domain is for use in illustrative examples in documents. You may use this
    domain in literature without prior coordination or asking for permission.</p>
    <p><a href="https://www.iana.org/domains/example">More information...</a></p>
</div>
</body>
</html>
^]
telnet>
*/

/*
Based on the Telnet session above, here's an explanation of the response from the server:

1. `HTTP/1.1 200 OK`: This line indicates that the server responded with an 
HTTP status code of `200 OK`, which means the request was successful.

2. Response Headers:
   - `Age: 468797`: This header indicates the age of the response in seconds.
   - `Cache-Control: max-age=604800`: This header specifies the caching behavior 
		for the response. In this case, it indicates that the response can be 
		cached by the client for a maximum of 604800 seconds (7 days).
   - `Content-Type: text/html; charset=UTF-8`: 
   		This header specifies the MIME type of the response content, 
		which is `text/html`, and the character encoding used, which is `UTF-8`.
   - `Date: Wed, 22 Nov 2023 15:35:06 GMT`: 
   		This header indicates the date and time when the response was generated.
   - `Etag: "3147526947+ident"`: This header provides an entity tag for the response, 
   		which can be used for caching and conditional requests.
   - `Expires: Wed, 29 Nov 2023 15:35:06 GMT`: This header specifies the expiration 
   		date and time for the response.
   - `Last-Modified: Thu, 17 Oct 2019 07:18:26 GMT`: 
   		This header indicates the last modification date and time of the requested resource.
   - `Server: ECS (bsb/279B)`: This header specifies the server software and its version.
   - `Vary: Accept-Encoding`: This header indicates that the response may vary 
   		based on the `Accept-Encoding` header sent by the client.
   - `X-Cache: HIT`: This header indicates that the response was served from the cache.
   - `Content-Length: 1256`: This header specifies the length of the response 
   		content in bytes.

3. Response Body: The remaining lines represent the HTML content of the response. 
	It includes an HTML document with a title, some text, and a link to more information. 
	This content is displayed in a web browser when accessing the URL `www.example.com`.

4. `^]`: This character (`^]`) is the escape character for Telnet. 
	It indicates that you can exit the Telnet session by typing `Ctrl+]`.

In summary, the server responded with an HTTP status code of `200 OK`, 
indicating a successful request. The response included various headers providing information 
about caching, content type, server details, and more. 
The response body contained an HTML document with some text and a link.

Please note that the response shown here is specific to the `www.example.com` 
domain and may vary for different websites or servers.
*/
/*###################################################################################*/
/*###################################################################################*/


/*###################################################################################*/
/* CONFIGURATION FILE REQUIREMENTS */
/*###################################################################################*/
/*
You can get some inspiration from the ’server’ part of NGINX
configuration file.

In the configuration file, you should be able to:
• Choose the port and host of each ’server’.
• Setup the server_names or not.
• The first server for a host:port will be the default for this host:port (that means
it will answer to all the requests that don’t belong to an other server).
• Setup default error pages.
• Limit client body size.
• Setup routes with one or multiple of the following rules/configuration (routes wont
be using regexp):
	- Define a list of accepted HTTP methods for the route.
	- Define a HTTP redirection.
	- Define a directory or a file from where the file should be searched (for example,
if url /kapouet is rooted to /tmp/www, url /kapouet/pouic/toto/pouet is
/tmp/www/pouic/toto/pouet).
	- Turn on or off directory listing.
	- Set a default file to answer if the request is a directory.
	- Execute CGI based on certain file extension (for example .php).
	- Make it work with POST and GET methods.
	- Make the route able to accept uploaded files and configure where they should
be saved.
Because you won’t call the CGI directly, use the full path as PATH_INFO.
		> Just remember that, for chunked request, your server needs to unchunk
it, the CGI will expect EOF as end of the body.
		> Same things for the output of the CGI. If no content_length is returned
from the CGI, EOF will mark the end of the returned data.
		> Your program should call the CGI with the file requested as first argument.
		> The CGI should be run in the correct directory for relative path file access.
		> Your server should work with one CGI (php-CGI, Python, and so forth).

The important thing is resilience. Server should never die.

*/

/*

Here's an example of the server part of an NGINX configuration file:
```
server {
    listen 80;
    server_name example.com;

    location / {
        proxy_pass http://localhost:8080;
        proxy_set_header Host $host;
        proxy_set_header X-Real-IP $remote_addr;
    }
}
```
In this example:

	`listen 80;` specifies that NGINX should listen on port 80 for incoming HTTP requests.
	`server_name example.com;` defines the server name or domain for which this 
		configuration applies. Replace example.com with your actual domain name.
	`location /` defines the location or URL path for which this configuration block 
		applies. In this case, it applies to all requests under the root path /.
	`proxy_pass http://localhost:8080;` specifies that NGINX should forward the requests 
		to the specified backend server at http://localhost:8080. 
		Replace localhost:8080 with the actual address and port of your backend server.
	`proxy_set_header` directives are used to set the request headers that will be sent 
		to the backend server. In this example, we set the Host and 
		`X-Real-IP` headers to the original values from the client request.

This is a basic example of an NGINX configuration for a reverse proxy setup. 
NGINX can be further configured with additional directives and settings to meet your 
specific requirements.

For more detailed information about NGINX configuration see:
`config_example.conf` file in the root of this repository.

*/

/*###################################################################################*/
/* WHAT IS NGINX [ en-gin-ex ] */
/*###################################################################################*/
/*
**
*/

/*###################################################################################*/
/* WHAT IS CGI [ see-gee-eye ] */
/*###################################################################################*/
/*
**
*/

/*###################################################################################*/
/* ALLOWED EXTERNAL FUNCTIONS, EXPLANATION */
/*###################################################################################*/
/*
** execve, dup, dup2, pipe, strerror, gai_strerror, errno, fork, socketpair, htons,
** htonl, ntohs, ntohl, select, poll, epoll (epoll_create, epoll_ctl, epoll_wait),
** kqueue (kqueue, kevent), socket, accept, listen, send, recv, chdir bind, connect,
** getaddrinfo, freeaddrinfo, setsockopt, getsockname, getprotobyname, fcntl, close,
** read, write, waitpid, kill, signal, access, stat, open, opendir, readdir, closedir.
**
** 1) < < < < < < EXECVE > > > > > >
** `execve` is a system call in C that is used to execute a program. It replaces the
** current process image with a new process image specified by the path of the 
** executable file.
** `execve(const char *pathname, char *const argv[], char *const envp[])`
** `pathname` is the path of the executable file.
** `argv[]` is an array of strings that represent the command-line arguments passed
** to the new program. The first argument should be the name of the executable itself.
** `envp[]` is an array of strings that represent the environment variables (as `env` cmd).
** When the `execve` is called, the current process is replaced by the new program specified
** by the `pathname` argument. 
** EXAMPLE:
#include <unistd.h>

int main() {
	char *const argv[] = { "ls", "-l", NULL };
	char *const envp[] = { NULL };

	execve("/bin/ls", argv, envp);

	// This line will only be reached if the execve function fails
	perror("execve");
	return 1;
}
**
** 2) < < < < < < DUP > > > > > >
** `dup` function in C is used to duplicate a file descriptor. It creates a copy of the
** original file descriptor passed as an argument.
** `int dup(int oldfd)` this will return a duplicate file descriptor of the oldfd.
** EXAMPLE:
#include <unistd.h>
#include <fcntl.h>

int main() {
	int fd = open("file.txt", O_RDONLY);
	if (fd == -1) {
		perror("open");
		return 1;
	}

	int new_fd = dup(fd);
	if (new_fd == -1) {
		perror("dup");
		return 1;
	}

	// Use the new file descriptor to read from the file
	char buffer[100];
	ssize_t bytesRead = read(new_fd, buffer, sizeof(buffer));
	if (bytesRead == -1) {
		perror("read");
		return 1;
	}

	// Print the contents of the file
	write(STDOUT_FILENO, buffer, bytesRead);

	close(fd);
	close(new_fd);

	return 0;
}
**
** 3) < < < < < < DUP2 > > > > > >
** `dup2` function in C is used to duplicate a file descriptor. It creates a copy of the
** original file descriptor passed as an argument.
** `int dup2(int oldfd, int newfd)` this will return a duplicate file descriptor of the oldfd.
** -1 is returned if an error occurs.
** EXAMPLE:
#include <unistd.h>
#include <fcntl.h>

int main() {
	int fd = open("file.txt", O_RDONLY);
	if (fd == -1) {
		perror("open");
		return 1;
	}

	int new_fd = dup2(fd, 3);
	if (new_fd == -1) {
		perror("dup2");
		return 1;
	}

	// Use the new file descriptor to read from the file
	char buffer[100];
	ssize_t bytesRead = read(new_fd, buffer, sizeof(buffer));
	if (bytesRead == -1) {
		perror("read");
		return 1;
	}

	// Print the contents of the file
	write(STDOUT_FILENO, buffer, bytesRead);

	close(fd);
	close(new_fd);

	return 0;
}
*/

