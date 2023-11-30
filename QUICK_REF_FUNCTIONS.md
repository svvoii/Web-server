## ALLOWED EXTERNAL FUNCTIONS  
QUICK GUIDE REFERENCE, EXPLANATION, USAGE EXAMPLES

[1. execve](#execve)  
[2. dup](#dup)  
[3. dup2](#dup2)  
[4. pipe](#pipe)  
[5. strerror](#strerror)  
[6. gai_strerror](#gai_strerror)  
[7. errno](#errno)  
[8. fork](#fork)  
[9. socketpair](#socketpair)  
[10. htons](#htons)  
[11. htonl](#htonl)  
[12. ntohs](#ntohs)  
[13. ntohl](#ntohl)  
[14. select](#select)  
[15. poll](#poll)  
[15.1 struct pollfd](#struct-pollfd)  
[16. epoll (epoll_create, epoll_ctl, epoll_wait)](#epoll)  
[17. kqueue (kqueue, kevent)](#kqueue)   
[17.1. struct kevent](#struct-kevent)  
[18. socket](#socket)  
[18.1 struct sockaddr](#struct-sockaddr)  
[19. accept](#accept)  
[20. listen](#listen)  
[21. send](#send)  
[22. recv](#recv)  
[23. chdir](#chdir)  
[24. bind](#bind)  
[25. connect](#connect)  
[25.1 inet_addr](#inet_addr)  
[26. getaddrinfo](#getaddrinfo)  
[27. freeaddrinfo](#freeaddrinfo)  
[28. setsockopt](#setsockopt)  
[29. getsockname](#getsockname)  
[30. getprotobyname](#getprotobyname)  
[31. fcntl](#fcntl)  
[32. open](#open)  
[33. close](#close)  
[34. read](#read)  
[35. write](#write)  
[36. waitpid](#waitpid)  
[37. kill](#kill)  
[38. signal](#signal)  
[39. access](#access)  
[40. stat](#stat)  
[41. opendir](#opendir)  
[42. readdir](#readdir)  
[43. closedir](#closedir)  

## execve
01 < < < < < < < < < < < < < < < < < < < EXECVE > > > > > > > > > > > > > > > > > > > >

`execve` is a system call in C that is used to execute a program.  
It replaces the current process image with a new process image specified by the path of the executable file.  

`execve(const char *pathname, char *const argv[], char *const envp[])`  

1) `pathname` is the path of the executable file.  
2) `argv[]` is an array of strings that represent the command-line arguments passed  
to the new program. The first argument should be the name of the executable itself.  
3) `envp[]` is an array of strings that represent the environment variables (as `env` cmd).  

When the `execve` is called, the current process is replaced by the new program specified by the `pathname` argument.   

<details>
<summary>EXAMPLE:</summary>

```c
#include <unistd.h>

int main() {
	char *const argv[] = { "ls", "-l", NULL };
	char *const envp[] = { NULL };

	execve("/bin/ls", argv, envp);

	// This line will only be reached if the execve function fails
	perror("execve");
	return 1;
}
```
</details>


## dup
02 < < < < < < < < < < < < < < < < < < < DUP > > > > > > > > > > > > > > > > > > > >

`dup` function in C is used to duplicate a file descriptor.  
It creates a copy of the original file descriptor passed as an argument.  

`int dup(int oldfd)` this will return a duplicate file descriptor of the oldfd.

<details>
<summary>EXAMPLE:</summary>

```c
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
```
</details>
  

## dup2
03 < < < < < < < < < < < < < < < < < < < DUP2 > > > > > > > > > > > > > > > > > > > >

`dup2` function in C is used to duplicate a file descriptor. 
It creates a copy of the original file descriptor passed as an argument.  

`int dup2(int oldfd, int newfd)` this will return a duplicate file descriptor of the oldfd.  
(-1) is returned if an error occurs.  

<details>
<summary>EXAMPLE:</summary>

```c
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
```
</details>


## pipe
04 < < < < < < < < < < < < < < < < < < < PIPE > > > > > > > > > > > > > > > > > > > >

`pipe` function in C is used to create a pipe, which is a unidirectional communication
data channel between two processes.  
It allows one process to write data to the pipe, and the other process to read that data from the pipe.  

`pipe` function takes a single argument, which is an array of two integers.  
It creates a pipe and stores the file descriptors of the read and write ends of the
pipe in the array.  
The read end of the pipe is used for reading data from the pipe, and the write end of
the pipe is used for writing data to the pipe.  

`int pipe(int pipefd[2])`  
Returns 0 on success, or -1 on error.

<details>
<summary>EXAMPLE:</summary>

```c
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

int main() {
	int pipefd[2];
	if (pipe(pipefd) == -1) {
		perror("pipe");
		return 1;
	}

	pid_t pid = fork();
	if (pid == -1) {
		perror("fork");
		return 1;
	}

	if (pid == 0) {
		// Child process
		close(pipefd[0]); // Close the read end of the pipe

		// Write data to the pipe
		char message[] = "Hello, parent process!";
		write(pipefd[1], message, sizeof(message));

		close(pipefd[1]); // Close the write end of the pipe
		exit(0);
	} else {
		// Parent process
		close(pipefd[1]); // Close the write end of the pipe

		// Read data from the pipe
		char buffer[100];
		ssize_t bytesRead = read(pipefd[0], buffer, sizeof(buffer));
		if (bytesRead == -1) {
			perror("read");
			return 1;
		}

		// Print the data received from the child process
		printf("Received message from child: %s\n", buffer);

		close(pipefd[0]); // Close the read end of the pipe
		wait(NULL); // Wait for the child process to exit
	}

	return 0;
}
```
</details>


## strerror
05 < < < < < < < < < < < < < < < < < < < STRERROR > > > > > > > > > > > > > > > > > > > >
 
`strerror` function in C is used to retrieve a string representation of the error code number.  
It takes an integer argument and returns a pointer to a string that describes the error code.  

`char *strerror(int errnum)`  
it returns a pointer to a string that describes the error corresponding to the error code number passed as an argument.   
The string returned by `strerror` is typically a system-specific error message.  

<details>
<summary>EXAMPLE:</summary>

```c
#include <stdio.h>
#include <string.h>
#include <errno.h>

int main() {
	FILE *file = fopen("nonexistent.txt", "r");
	if (file == NULL) {
		printf("Failed to open file: %s\n", strerror(errno));
		return 1;
	}

	// File operations...

	fclose(file);
	return 0;
}
```
</details>


## gai_strerror
06 < < < < < < < < < < < < < < < < < < < GAI_STRERROR > > > > > > > > > > > > > > > > > > > >

`gai_strerror` function is a POSIX function used to retrieve a string representation 
of an error code related to network address and service translation.  
It takes an error code number as an argument and returns a pointer to a string that describes the error.  

`char *gai_strerror(int errcode)`  
it returns a pointer to a string that describes the error corresponding to the error code number passed as an argument.  
The string returned by `gai_strerror` is typically a system-specific error message.  

<details>
<summary>EXAMPLE:</summary>

```c
#include <stdio.h>
#include <netdb.h>

int main() {
	struct addrinfo hints, *result;
	int error;

	// Set up hints structure
	memset(&hints, 0, sizeof(hints));
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;

	// Call getaddrinfo with invalid hostname to trigger an error
	error = getaddrinfo("invalidhostname", "80", &hints, &result);
	if (error != 0) {
		printf("getaddrinfo failed: %s\n", gai_strerror(error));
		return 1;
	}

	// Process the result...

	freeaddrinfo(result);
	return 0;
}
```
</details>


## errno
07 < < < < < < < < < < < < < < < < < < < ERRNO > > > > > > > > > > > > > > > > > > > >

`errno` is a global variable in C that is used to indicate an error condition.  
It indicates the last error that occurred during a system call or library function.  
`errno` is defined in the `errno.h` header file.  

<details>
<summary>EXAMPLE:</summary>

```c
#include <stdio.h>
#include <errno.h>

int main() {
	FILE *file = fopen("nonexistent.txt", "r");
	if (file == NULL) {
		printf("Failed to open file: %s\n", strerror(errno));
		return 1;
	}

	// File operations...

	fclose(file);
	return 0;
}
```
</details>


## fork
08 < < < < < < < < < < < < < < < < < < < FORK > > > > > > > > > > > > > > > > > > > >
 
`fork` function in C is used to create a new process by duplicating the existing process.  
It creates a child process that is an exact copy of the parent process, including the 
code, data, and file descriptors.  

`pid_t fork(void)`  
it returns the process ID of the child process in the parent process, and 0 in the child process.   
If an error occurs, it returns -1.  

<details>
<summary>EXAMPLE:</summary>

```c
#include <stdio.h>
#include <unistd.h>

int main() {
	pid_t pid = fork();
	if (pid == -1) {
		perror("fork");
		return 1;
	}

	if (pid == 0) {
		// Child process
		printf("Hello from the child process!\n");
	} else {
		// Parent process
		printf("Hello from the parent process! Child PID: %d\n", pid);
	}

	return 0;
}
```
</details>


## socketpair
09 < < < < < < < < < < < < < < < < < < < SOCKETPAIR > > > > > > > > > > > > > > > > > > > >

`socketpair` function in C is used to create a pair of connected sockets that can be
used to communicate between two processes.  
It creates a pair of sockets that are bidirectionally connected, allowing data to be transferred between the two processes.  

`int socketpair(int domain, int type, int protocol, int sv[2])`  
1) `domain` specifies the communication domain for the sockets. It can be either `AF_UNIX` or `AF_LOCAL` for local communication, or `AF_INET` for network communication.
2) `type` specifies the type of socket, such as `SOCK_STREAM` for reliable TCP sockets or `SOCK_DGRAM` for UDP (datagram) sockets.
3) `protocol` specifies the protocol to be used with the socket. It can be 0 to use the default protocol for the given domain and type.
4) `sv[2]` is an array of two integers that will store the file descriptors of the two sockets created by the `socketpair` function.

<details>
<summary>EXAMPLE:</summary>

```c
#include <stdio.h>
#include <sys/socket.h>
#include <unistd.h>

int main() {
	int sockets[2];
	if (socketpair(AF_UNIX, SOCK_STREAM, 0, sockets) == -1) {
		perror("socketpair");
		return 1;
	}

	pid_t pid = fork();
	if (pid == -1) {
		perror("fork");
		return 1;
	}

	if (pid == 0) {
		// Child process
		close(sockets[0]); // Close the parent's socket

		// Use sockets[1] for communication with the parent process
		// ...

		close(sockets[1]); // Close the child's socket
	} else {
		// Parent process
		close(sockets[1]); // Close the child's socket

		// Use sockets[0] for communication with the child process
		// ...

		close(sockets[0]); // Close the parent's socket
	}

	return 0;
}
```
The example above creates a pair of connected sockets, and then forks a child process.  
The parent process closes the child's socket, and the child process closes the parent's socket.  
This allows the parent and child processes to communicate with each other using the sockets. 
</details>


## htons
10 < < < < < < < < < < < < < < < < < < < HTONS > > > > > > > > > > > > > > > > > > > >

`htons` function in C is used to convert a 16-bit integer (short) from host byte order
to network byte order. `htons` stands for "host to network short".

`uint16_t htons(uint16_t hostshort)`  
it returns the converted value.

<details>
<summary>EXAMPLE:</summary>

```c
#include <stdio.h>
#include <arpa/inet.h>

int main() {
	uint16_t host_short = 3000;
	uint16_t network_short = htons(host_short);

	printf("Host byte order: %u\n", host_short);
	printf("Network byte order: %u\n", network_short);

	return 0;
}
```
The output of the program is:  
Host byte order: 3000  
Network byte order: 10752  
</details>


## htonl
11 < < < < < < < < < < < < < < < < < < < HTONL > > > > > > > > > > > > > > > > > > > >

`htonl` function in C is used to convert a 32-bit integer (long) from host byte order
to network byte order. `htonl` stands for "host to network long".  

`uint32_t htonl(uint32_t hostlong)`  
it returns the converted value.

<details>
<summary>EXAMPLE:</summary>

```c
#include <stdio.h>
#include <arpa/inet.h>

int main() {
	uint32_t host_long = 3000000;
	uint32_t network_long = htonl(host_long);

	printf("Host byte order: %u\n", host_long);
	printf("Network byte order: %u\n", network_long);

	return 0;
}
```
The output of the program is:  
Host byte order: 3000000  
Network byte order: 50331648  
</details>


## ntohs
12 < < < < < < < < < < < < < < < < < < < NTOHS > > > > > > > > > > > > > > > > > > > >

`ntohs` function in C is used to convert a 16-bit integer (short) from network byte order
to host byte order. `ntohs` stands for "network to host short".

`uint16_t ntohs(uint16_t netshort)`  
it returns the converted value.

<details>
<summary>EXAMPLE:</summary>

```c
#include <stdio.h>
#include <arpa/inet.h>

int main() {
	uint16_t network_short = 10752;
	uint16_t host_short = ntohs(network_short);

	printf("Network byte order: %u\n", network_short);
	printf("Host byte order: %u\n", host_short);

	return 0;
}
```
The output of the program is:  
Network byte order: 10752  
Host byte order: 3000  
</details>


## ntohl
13 < < < < < < < < < < < < < < < < < < < NTOHL > > > > > > > > > > > > > > > > > > > >

`ntohl` function in C is used to convert a 32-bit integer (long) from network byte order
to host byte order. `ntohl` stands for "network to host long".  

`uint32_t ntohl(uint32_t netlong)`  
it returns the converted value.  

<details>
<summary>EXAMPLE:</summary>

```c
#include <stdio.h>
#include <arpa/inet.h>

int main() {
	uint32_t network_long = 50331648;
	uint32_t host_long = ntohl(network_long);

	printf("Network byte order: %u\n", network_long);
	printf("Host byte order: %u\n", host_long);

	return 0;
}
```
The output of the program is:  
Network byte order: 50331648  
Host byte order: 3000000  
</details>


## select
14 < < < < < < < < < < < < < < < < < < < SELECT > > > > > > > > > > > > > > > > > > > >

`select` function in C is used to monitor multiple file descriptors.  
It allows a program to monitor multiple file descriptors to see if any of them are ready for reading, writing,
or if an exceptional condition (error) has occurred.  

`int select(int nfds, fd_set *readfds, fd_set *writefds, fd_set *exceptfds, struct timeval *timeout)`  
Returns the number of file descriptors that are ready for reading, writing, or have an exceptional condition.  
It returns 0 if the timeout expires before any events occur, or -1 if an error occurs.
1) `nfds` specifies the highest-numbered file descriptor in any of the three sets, plus 1.
2) `readfds` is a set of file descriptors to monitor for reading.
3) `writefds` is a set of file descriptors to monitor for writing.
4) `exceptfds` is a set of file descriptors to monitor for exceptional conditions.
5) `timeout` specifies the maximum amount of time to wait for an event to occur. If it is NULL, the `select` function will block indefinitely until an event occurs.

<details>
<summary>EXAMPLE:</summary>

```c
#include <stdio.h>
#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>

int main() {
	fd_set rfds;
	struct timeval tv;
	int retval;

	// Watch stdin (fd 0) to see when it has input.
	FD_ZERO(&rfds);
	FD_SET(0, &rfds);

	// Wait up to five seconds.
	tv.tv_sec = 5;
	tv.tv_usec = 0;

	retval = select(1, &rfds, NULL, NULL, &tv);
	if (retval == -1)
		perror("select");
	else if (retval)
		printf("Data is available now.\n");
	else
		printf("No data within five seconds.\n");

	return 0;
}
```
The example above monitors the standard input (file descriptor 0) for five seconds to
see if any data is available to be read..  
</details>


## poll
15 < < < < < < < < < < < < < < < < < < < POLL > > > > > > > > > > > > > > > > > > > >

`poll` function in C is used to monitor multiple file descriptors to see if they are
ready for reading, writing, or if an exceptional condition (error) has occurred.  
It is similar to the `select` function, but it is more flexible and scalable, it
doesn't require recalculating the maximum file descriptor value, and can monitor more
than `FD_SETSIZE` (1024) file descriptors.  

`int poll(struct pollfd *fds, nfds_t nfds, int timeout)`  
Returns the number of file descriptors that are ready for reading, writing, or have an exceptional condition.  
It returns 0 if the timeout expires before any events occur, or -1 if an error occurs.
1) `fds` is an array of `struct pollfd` structures that contain the file descriptors to monitor and the events to monitor for (see example of the `struct pollfd` bellow in 15.1).
2) `nfds` is the number of file descriptors in the `fds` array.
3) `timeout` specifies the maximum amount of time to wait for an event to occur. If it is -1, the `poll` function will block indefinitely until an event occurs.

<details>
<summary>EXAMPLE:</summary>

```c
#include <stdio.h>
#include <poll.h>
#include <unistd.h>

int main() {
	struct pollfd fds[1];
	int timeout_msecs = 5000;
	int ret;

	// Monitor stdin (fd 0) for input
	fds[0].fd = 0;
	fds[0].events = POLLIN;

	ret = poll(fds, 1, timeout_msecs);
	if (ret == -1) {
		perror("poll");
		return 1;
	}

	if (fds[0].revents & POLLIN) {
		printf("stdin is ready for reading\n");
	} else {
		printf("No data within five seconds.\n");
	}

	return 0;
}
```
The example above monitors the standard input (file descriptor 0) for five seconds to see if any data is available to be read.  
</details>


## struct pollfd
15.1 < < < < < < < < < < < < < < < < < < < STRUCT POLLFD > > > > > > > > > > > > > > > > > > > >

`struct pollfd` is a structure used by the `poll` function to specify the file descriptors to monitor and the events to monitor for.  

```c
struct pollfd {  
	int		fd;			// File descriptor to monitor  
	short	events;		// Events to monitor for (POLLIN, POLLOUT, POLLERR)  
	short	revents;	// Events that occurred (POLLIN, POLLOUT, POLLERR)  
};
```  
- `fd` is the file descriptor to monitor.  
- `events` is a bit mask specifying the events to monitor for. It can be one or more of  the following values:  
	- `POLLIN` - Data available to be read  
	- `POLLOUT` - Data can be written  
	- `POLLERR` - Error condition  
- `revents` is a bit mask specifying the events that occurred.  

<details>
<summary>EXAMPLE:</summary>

```c
#include <stdio.h>
#include <poll.h>
#include <unistd.h>

int main() {
	struct pollfd fds[2];

	// Monitor stdin (fd 0) for input
	fds[0].fd = 0;
	fds[0].events = POLLIN;

	// Monitor stdout (fd 1) for being ready to write
	fds[1].fd = 1;
	fds[1].events = POLLOUT;

	int ret = poll(fds, 2, 5000);
	if (ret == -1) {
		perror("poll");
		return 1;
	}

	if (fds[0].revents & POLLIN) {
		printf("stdin is ready for reading\n");
	}

	if (fds[1].revents & POLLOUT) {
		printf("stdout is ready for writing\n");
	}

	return 0;
}
```
The example above monitors the standard input (file descriptor 0) for input and the standard output (file descriptor 1) for being ready to write.  
</details>


## epoll
16 < < < < < < < < < < < < < < < < < < < EPOLL > > > > > > > > > > > > > > > > > > > >

`epoll` is an interface in Linux for monitoring multiple file descriptors (I/O multiplexing).  
It is similar to the `select` and `poll` functions, but it is more efficient for large numbers of file descriptors.  

INTERFACE FUNCTIONS:  
`int epoll_create(int size)`  
Returns a file descriptor for the new epoll instance, or -1 if an error occurs.

`int epoll_ctl(int epfd, int op, int fd, struct epoll_event *event)`
1) `epfd` is the file descriptor returned by `epoll_create`.
2) `op` specifies the operation to perform. It can be one of the following values:
	- `EPOLL_CTL_ADD` - Add a file descriptor to the epoll instance.
	- `EPOLL_CTL_MOD` - Change the events to monitor for a file descriptor.
	- `EPOLL_CTL_DEL` - Remove a file descriptor from the epoll instance.
3) `fd` is the file descriptor to add, modify, or remove.
4) `event` is a pointer to a `struct epoll_event` structure that specifies the events to monitor for the file descriptor.

`int epoll_wait(int epfd, struct epoll_event *events, int maxevents, int timeout)`
Returns the number of events that occurred, or -1 if an error occurs.
1) `epfd` is the file descriptor returned by `epoll_create`.
2) `events` is an array of `struct epoll_event` structures that will store the events that occurred.
3) `maxevents` is the maximum number of events to store in the `events` array.
4) `timeout` specifies the maximum amount of time to wait for an event to occur. If it is -1, the `epoll_wait` function will block indefinitely until an event occurs.

<details>
<summary>EXAMPLE:</summary>

```c
#include <stdio.h>
#include <sys/epoll.h>
#include <unistd.h>

int main() {
	int epfd = epoll_create1(0);
	if (epfd == -1) {
		perror("epoll_create1");
		return 1;
	}

	struct epoll_event event;
	event.events = EPOLLIN;
	event.data.fd = 0;

	if (epoll_ctl(epfd, EPOLL_CTL_ADD, 0, &event) == -1) {
		perror("epoll_ctl");
		return 1;
	}

	struct epoll_event events[10];
	int num_events = epoll_wait(epfd, events, 10, 5000);
	if (num_events == -1) {
		perror("epoll_wait");
		return 1;
	}

	for (int i = 0; i < num_events; i++) {
		if (events[i].events & EPOLLIN) {
			printf("stdin is ready for reading\n");
		}
	}

	return 0;
}
```
The example above ctreats an epoll instance, adds the standard input (file descriptor 0) to the interest list with `EPOLLIN` event, and then `epoll_wait` is used to wait for `EPOLLIN` event.  
If the standard input is ready for reading, it prints a respective message.  
</details>


## kqueue
17 < < < < < < < < < < < < < < < < < < < KQUEUE > > > > > > > > > > > > > > > > > > > >

`kqueue` is an interface in BSD-based operating systems like FreeBSD, OpenBSD, and macOS
used for monitoring multiple file descriptors (I/O multiplexing).  
It is similar to the `epoll` interface in Linux.  

INTERFACE FUNCTIONS:  
`int kqueue(void)`  
it returns a file descriptor for the new kqueue instance, or -1 if an error occurs.

`int kevent(int kq, const struct kevent *changelist, int nchanges, struct kevent *eventlist, int nevents, const struct timespec *timeout)`  
is used to register events to the queue or retrieve pending events from the queue.  
Returns the number of events that occurred, or -1 if an error occurs.  
1) `kq` is the file descriptor returned by `kqueue`.
2) `changelist` is an array of `struct kevent` structures that specifies the changes to make to the kqueue instance (see example of the `struct kevent` bellow in 17.1).
3) `nchanges` is the number of changes to make to the kqueue instance.
4) `eventlist` is an array of `struct kevent` structures that will store the events that occurred.
5) `nevents` is the maximum number of events to store in the `eventlist` array.
6) `timeout` specifies the maximum amount of time to wait for an event to occur. If it is NULL, the `kevent` function will block indefinitely until an event occurs.

<details>
<summary>EXAMPLE:</summary>

```c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/event.h>
#include <sys/time.h>
#include <unistd.h>

int main() {
	int kq = kqueue();
	if (kq == -1) {
		perror("kqueue");
		return 1;
	}

	struct kevent change;
	EV_SET(&change, STDIN_FILENO, EVFILT_READ, EV_ADD, 0, 0, NULL);

	struct kevent event;
	while (1) {
		int ret = kevent(kq, &change, 1, &event, 1, NULL);
		if (ret == -1) {
			perror("kevent");
			return 1;
		}

		if (event.flags & EV_EOF) {
			printf("EOF detected on stdin\n");
			break;
		}

		if (event.filter == EVFILT_READ) {
			char buf[1024];
			ssize_t n = read(STDIN_FILENO, buf, sizeof(buf) - 1);
			if (n > 0) {
				buf[n] = '\0';
				printf("Read: %s", buf);
			}
		}
	}

	return 0;
}
```
The example above creates a kqueue instance, adds the standard input (file descriptor 0) to the interest list with `EVFILT_READ` event, and then `kevent` is used to wait for `EVFILT_READ` event.  
If the standard input is ready for reading, it reads the data from the standard input and prints it to the standard output.  
</details>


## struct kevent
17.1 < < < < < < < < < < < < < < < < < < < STRUCT KEVENT > > > > > > > > > > > > > > > > > > > >

`struct kevent` is a structure used by the `kqueue` interface to specify the events to monitor for and the events that occurred.

```c
struct kevent {
	uintptr_t	ident;	// Identifier for this event
	short		filter;	// Filter for event
	u_short		flags;	// Action flags for kqueue
	u_int		fflags;	// Filter flag value
	intptr_t	data;	// Filter data value
	void		*udata;	// Opaque user data identifier
};
```
- `ident` is an identifier for this event. It can be a file descriptor, process ID, or user-defined identifier.
- `filter` is the filter for this event. It can be one of the following values:
	- `EVFILT_READ` - Read data available
	- `EVFILT_WRITE` - Write data available
	- `EVFILT_AIO` - Asynchronous I/O
	- `EVFILT_VNODE` - File system events
	- `EVFILT_PROC` - Process events
	- `EVFILT_SIGNAL` - Signal events
	- `EVFILT_TIMER` - Timer events
	- `EVFILT_USER` - User-defined events
- `flags` is a bit mask specifying the action to perform. It can be one or more of the following values:
	- `EV_ADD` - Add the event to the kqueue instance.
	- `EV_DELETE` - Remove the event from the kqueue instance.
	- `EV_ENABLE` - Enable the event.
	- `EV_DISABLE` - Disable the event.
	- `EV_ONESHOT` - Only deliver the event once.
	- `EV_CLEAR` - Clear the event state after delivery.
	- `EV_RECEIPT` - Return the event in the `udata` field of the `struct kevent` structure.
- `fflags` is a filter flag value. It depends on the filter specified by the `filter` field.
- `data` is a filter data value. It depends on the filter specified by the `filter` field.
- `udata` is an opaque user data identifier. It is returned in the `udata` field of the `struct kevent` structure.

<details>
<summary>EXAMPLE:</summary>

```c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/event.h>
#include <sys/time.h>
#include <unistd.h>

int main() {
	int kq = kqueue();
	if (kq == -1) {
		perror("kqueue");
		return 1;
	}

	struct kevent change;
	EV_SET(&change, STDIN_FILENO, EVFILT_READ, EV_ADD, 0, 0, NULL);

	struct kevent event;
	while (1) {
		int ret = kevent(kq, &change, 1, &event, 1, NULL);
		if (ret == -1) {
			perror("kevent");
			return 1;
		}

		if (event.flags & EV_EOF) {
			printf("EOF detected on stdin\n");
			break;
		}

		if (event.filter == EVFILT_READ) {
			char buf[1024];
			ssize_t n = read(STDIN_FILENO, buf, sizeof(buf) - 1);
			if (n > 0) {
				buf[n] = '\0';
				printf("Read: %s", buf);
			}
		}
	}

	return 0;
}
```
The example above creates a kqueue instance, adds the standard input (file descriptor 0) to the interest list with `EVFILT_READ` event using `struct kevent`, and then `kevent` is used in a loop to wait for `EVFILT_READ` event.  
If the standard input is ready for reading, it reads the input and prints it to the standard output.  
If the `EV_EOF` flag is set in the `flags` field of the `struct kevent` structure, it means that the end of file was reached on the file descriptor.  
</details>


## socket
18 < < < < < < < < < < < < < < < < < < < SOCKET > > > > > > > > > > > > > > > > > > > >

`socket` function in C is used to create a socket, which is an endpoint for communication between two processes.  
It is a fundamental function for network communication, used in conjunction with other functions like `bind`, `connect`, `listen`, and `accept`.  

`int socket(int domain, int type, int protocol)`  
it returns a file descriptor for the new socket, or -1 if an error occurs.  
1) `domain` specifies the communication domain for the socket. It can be either `AF_UNIX` or `AF_LOCAL` for local communication, or `AF_INET` for IPv4 network communication.
2) `type` specifies the type of socket, such as `SOCK_STREAM` for reliable TCP sockets or `SOCK_DGRAM` for UDP (datagram) sockets.
3) `protocol` specifies the protocol to be used with the socket. It can be 0 to use the default protocol for the given domain and type.

<details>
<summary>EXAMPLE:</summary>

```c
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

int main() {
	int sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if (sockfd < 0) {
		perror("socket");
		return 1;
	}

	struct sockaddr_in serv_addr;
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_port = htons(80);
	serv_addr.sin_addr.s_addr = INADDR_ANY;

	if (bind(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0) {
		perror("bind");
		return 1;
	}

	listen(sockfd, 5);

	struct sockaddr_in cli_addr;
	socklen_t clilen = sizeof(cli_addr);

	int newsockfd = accept(sockfd, (struct sockaddr *) &cli_addr, &clilen);
	if (newsockfd < 0) {
		perror("accept");
		return 1;
	}

	// Now newsockfd can be used to communicate with the client

	return 0;
}
```
The example above creates a TCP socket, binds it to available IP address on port 80, and listens for incoming connections.  
When a client connects, a new socket is created for communication with the client.  
</details>


## struct sockaddr
18.1 < < < < < < < < < < < < < < < < < < < STRUCT SOCKADDR > > > > > > > > > > > > > > > > > > > >

`struct sockaddr` and `struct sockaddr_in` are structures used in socket programming for handling internet addresses.

`struct sockaddr` is a generic socket address structure, used by functions like `bind`, `connect`, `accept`, and `getsockname`.

```c
struct sockaddr {
	unsigned short	sa_family;		// Address family (AF_INET or AF_UNIX)
	char			sa_data[14];	// Protocol-specific address
};
```
- `sa_family` is the address family. It can be either `AF_INET` for IPv4 addresses or `AF_UNIX` for local addresses.
- `sa_data` is the protocol-specific address. It can be a 14-byte IPv4 address or a 14-byte path to a local socket.

`struct sockaddr_in` is a structure used to specify an IPv4 address.
```c
struct sockaddr_in {
	short			sin_family;		// Address family (AF_INET)
	unsigned short	sin_port;		// Port number
	struct in_addr	sin_addr;		// Internet address
	char 			sin_zero[8];	// Unused
};
```
- `sin_family` is the address family. It is always set to `AF_INET`.
- `sin_port` is the port number.
- `sin_addr` is the internet address.
- `sin_zero` is unused and should be set to all zeros.

<details>
<summary>EXAMPLE:</summary>

```c
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

int main() {
	int sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if (sockfd < 0) {
		perror("socket");
		return 1;
	}

	struct sockaddr_in serv_addr;
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_port = htons(80);
	serv_addr.sin_addr.s_addr = INADDR_ANY;

	if (bind(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0) {
		perror("bind");
		return 1;
	}

	listen(sockfd, 5);

	struct sockaddr_in cli_addr;
	socklen_t clilen = sizeof(cli_addr);

	int newsockfd = accept(sockfd, (struct sockaddr *) &cli_addr, &clilen);
	if (newsockfd < 0) {
		perror("accept");
		return 1;
	}

	// Now newsockfd can be used to communicate with the client

	return 0;
}
```
The example above creates a TCP socket, binds it to available IP address on port 80, and listens for incoming connections.  
When a client connects, a new socket is created for communication with the client.  
</details>


## accept
19 < < < < < < < < < < < < < < < < < < < ACCEPT > > > > > > > > > > > > > > > > > > > >

`accept` function in C is used in socket programming to accept a new connection on a
socket. It is typically used in servers that listen for incoming client connections.

`int accept(int sockfd, struct sockaddr *addr, socklen_t *addrlen)` it returns a file
descriptor for the new socket, or -1 if an error occurs.
- `sockfd` is the file descriptor returned by `socket`.
- `addr` is a pointer to a `struct sockaddr` structure that will store the address of
the client that connects to the socket.
- `addrlen` is a pointer to a `socklen_t` variable that will store the size of the
`struct sockaddr` structure.

see example 18 [(socket)](#socket) and 18.1 [(struct sockaddr)](#struct-sockaddr)


## listen
20 < < < < < < < < < < < < < < < < < < < LISTEN > > > > > > > > > > > > > > > > > > > >

`listen` function in C is used in socket programming to mark a socket as a passive socket, which is a socket that will be used to accept incoming connection requests using the `accept` function.  

`int listen(int sockfd, int backlog)`  
it returns 0 on success, or -1 if an error occurs.  
1) `sockfd` is the file descriptor returned by `socket`.
2) `backlog` is the maximum number of pending connections that can be queued up before connections are refused.

<details>
<summary>EXAMPLE:</summary>

```c
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

int main() {
	int sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if (sockfd < 0) {
		perror("socket");
		return 1;
	}

	struct sockaddr_in serv_addr;
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_port = htons(80);
	serv_addr.sin_addr.s_addr = INADDR_ANY;

	if (bind(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0) {
		perror("bind");
		return 1;
	}

	if (listen(sockfd, 5) < 0) {
		perror("listen");
		return 1;
	}

	// Now sockfd can be used to accept connections

	return 0;
}
```
The example above creates a TCP socket, binds it to all available IP addresses on port 80, and sets it to listen for incoming connections.  
`listen` function is used to mark the socket as a passive socket with a backlog of 5, which means that up to 5 clients can be waiting for a connection at any given time.
</details>


## send
21 < < < < < < < < < < < < < < < < < < < SEND > > > > > > > > > > > > > > > > > > > >

`send` function in C is used in socket programming to send data on a socket.  
It is typically used in clients that send data to a server.  

`ssize_t send(int sockfd, const void *buf, size_t len, int flags)`  
it returns the number of bytes sent, or -1 if an error occurs.
1) `sockfd` is the file descriptor returned by `socket`.
2) `buf` is a pointer to the data to send.
3) `len` is the size of the data to send.
4) `flags` specifies the type of message transmission. It can be 0 or more of the following values OR-ed together:
	- `MSG_DONTROUTE` - Send without using a routing table.
	- `MSG_DONTWAIT` - Send without blocking.
	- `MSG_EOR` - Indicate end of record.
	- `MSG_OOB` - Send out-of-band data.

<details>
<summary>EXAMPLE:</summary>

```c
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>

int main() {
	int sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if (sockfd < 0) {
		perror("socket");
		return 1;
	}

	struct sockaddr_in serv_addr;
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_port = htons(80);
	serv_addr.sin_addr.s_addr = inet_addr("93.184.216.34"); // example.com

	if (connect(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0) {
		perror("connect");
		return 1;
	}

	const char *message = "Hello, World!";
	if (send(sockfd, message, strlen(message), 0) < 0) {
		perror("send");
		return 1;
	}

	// Now sockfd can be used to receive data from the server, see example 22

	return 0;
}
```
The example above creates a TCP socket and connects to the server at IP address 93.184.216.34 (example.com) on port 80.  
It then sends the message "Hello, World!" to the server.  
If the send operation is successful, the socket can be used to receive data from the server.  
</details>


## recv
22 < < < < < < < < < < < < < < < < < < < RECV > > > > > > > > > > > > > > > > > > > >

`recv` function in C is used in socket programming to receive data from a socket.  
It is typically used in servers that receive data from clients.  

`ssize_t recv(int sockfd, void *buf, size_t len, int flags)`  
it returns the number of bytes received, or -1 if an error occurs.  
1) `sockfd` is the file descriptor returned by `socket`.
2) `buf` is a pointer to the buffer where the data will be stored.
3) `len` is the size of the buffer.
4) `flags` specifies the type of message reception. It can be 0 or more of the following values OR-ed together:
	- `MSG_DONTWAIT` - Receive without blocking.
	- `MSG_OOB` - Receive out-of-band data.

<details>
<summary>EXAMPLE:</summary>

```c
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>

int main() {
	int sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if (sockfd < 0) {
		perror("socket");
		return 1;
	}

	struct sockaddr_in serv_addr;
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_port = htons(80);
	serv_addr.sin_addr.s_addr = inet_addr("93.184.216.34"); // example.com

	if (connect(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0) {
		perror("connect");
		return 1;
	}

	const char *message = "Hello, World!";
	if (send(sockfd, message, strlen(message), 0) < 0) {
		perror("send");
		return 1;
	}

	char buffer[256];
	int n = recv(sockfd, buffer, sizeof(buffer) - 1, 0);
	if (n < 0) {
		perror("recv");
		return 1;
	}

	buffer[n] = '\0';
	printf("Received: %s\n", buffer);

	return 0;
}
```
The example above creates a TCP socket and connects to the server at IP address 93.184.216.34 (example.com) on port 80.  
It then sends the message "Hello, World!" to the server and receives a response from the server.  
The `recv` function is used to receive the response from a server.  
The response is stored in a buffer and printed to the standard output.  
</details>


## chdir
23 < < < < < < < < < < < < < < < < < < < CHDIR > > > > > > > > > > > > > > > > > > > >

`chdir` function in C is used to change the current working directory.  

`int chdir(const char *path)`  
it returns 0 on success, or -1 if an error occurs.  
1) `path` is a pointer to a string containing the path of the directory to change to.

<details>
<summary>EXAMPLE:</summary>

```c
#include <unistd.h>
#include <stdio.h>

int main() {
	if (chdir("/tmp") < 0) {
		perror("chdir");
		return 1;
	}

	char cwd[1024];
	if (getcwd(cwd, sizeof(cwd)) != NULL) {
		printf("Current working dir: %s\n", cwd);
	} else {
		perror("getcwd");
		return 1;
	}

	return 0;
}
```
The example above changes the current working directory to `/tmp` and then prints the current working directory to the standard output.
</details>


## bind
24 < < < < < < < < < < < < < < < < < < < BIND > > > > > > > > > > > > > > > > > > > >

`bind` function in C is used to associate a socket with a local address.  
It is used in conjunction with the `socket` function to create a socket.  

`int bind(int sockfd, const struct sockaddr *addr, socklen_t addrlen)`  
it returns 0 on success, or -1 if an error occurs.  
1) `sockfd` is the file descriptor returned by `socket`.  
2) `addr` is a pointer to a `struct sockaddr` structure that contains the local address to associate with the socket.  
3) `addrlen` is the size of the `struct sockaddr` structure.  

<details>
<summary>EXAMPLE:</summary>

```c
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

int main() {
	int sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if (sockfd < 0) {
		perror("socket");
		return 1;
	}

	struct sockaddr_in serv_addr;
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_port = htons(80);
	serv_addr.sin_addr.s_addr = INADDR_ANY;

	if (bind(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0) {
		perror("bind");
		return 1;
	}

	// Now sockfd can be used to listen for connections

	return 0;
}
```
The example above creates a TCP socket and binds it to all available IP addresses on port 80.  

also see examples 18 [(socket)](#socket) and 18.1 [(struct sockaddr)](#struct-sockaddr)
</details>


## connect
25 < < < < < < < < < < < < < < < < < < < CONNECT > > > > > > > > > > > > > > > > > > > >

`connect` function in C is used in socket programming to connect to a remote host.  
It is typically used in clients that connect to a server.  

`int connect(int sockfd, const struct sockaddr *addr, socklen_t addrlen)`  
it returns 0 on success, or -1 if an error occurs.  
1) `sockfd` is the file descriptor returned by `socket`.  
2) `addr` is a pointer to a `struct sockaddr` structure that contains the address of the  
remote host to connect to.  
3) `addrlen` is the size of the `struct sockaddr` structure.  

<details>
<summary>EXAMPLE:</summary>

```c
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

int main() {
	int sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if (sockfd < 0) {
		perror("socket");
		return 1;
	}

	struct sockaddr_in serv_addr;
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_port = htons(80);
	serv_addr.sin_addr.s_addr = inet_addr("93.184.216.34"); // example.com

	if (connect(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0) {
		perror("connect");
		return 1;
	}

	// Now sockfd can be used to communicate with the server

	return 0;
}
```
The example above creates a TCP socket and connects to the address 93.184.216.34 (example.com) on port 80.  
If the connection is successful, the socket can be used to communicate with the server.  
</details>


## inet_addr
25.1 < < < < < < < < < < < < < < < < < < < INET_ADDR > > > > > > > > > > > > > > > > > > > >

`inet_addr` function in C is used to convert an IPv4 address in dotted-decimal notation string format to its binary representation that can be used as an argument for functions like `connect` and `bind`.  

`in_addr_t inet_addr(const char *cp)`  
it returns the binary representation of the IPv4 address, or -1 if an error occurs.
1) `cp` is a pointer to a string containing the IPv4 address in dotted-decimal notation.

<details>
<summary>EXAMPLE:</summary>

```c
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

int main() {
	const char *ip_str = "93.184.216.34"; // example.com
	in_addr_t ip = inet_addr(ip_str);

	if (ip == INADDR_NONE) {
		printf("Invalid IP address: %s\n", ip_str);
		return 1;
	}

	printf("IP in network byte order: %u\n", ip);

	return 0;
}
```
`in_addr_t` is a typedef for `uint32_t` and is defined in the `netinet/in.h` header file.  
The output of the program is: `IP in network byte order: 570666757`  
</details>


## getaddrinfo
26 < < < < < < < < < < < < < < < < < < < GET ADDR INFO > > > > > > > > > > > > > > > > > > > >

`getaddrinfo` function in C is used to resolve the network address (both service and protocol) into a list of address structures that can be used later for socket creation and binding.  
It is a modern replacement for the `gethostbyname` and `gethostbyaddr` functions.  

`int getaddrinfo(const char *node, const char *service, const struct addrinfo *hints, struct addrinfo **res)`  
it returns 0 on success, or a non-zero error code if an error occurs.
1) `node` is a pointer to a string containing the network address (either a hostname or an IP address) to resolve.
2) `service` is a pointer to a string containing the service name or port number to resolve.
3) `hints` is a pointer to a `struct addrinfo` structure that specifies the preferred socket type and protocol.
4) `res` is a pointer to a pointer to a `struct addrinfo` structure that will store the result of the address resolution.

```c
struct addrinfo {
	int				ai_flags;		// Input flags
	int				ai_family;		// Address family
	int				ai_socktype;	// Socket type
	int				ai_protocol;	// Protocol
	socklen_t		ai_addrlen;		// Length of address
	struct sockaddr	*ai_addr;		// Address
	char 			*ai_canonname;	// Canonical name
	struct addrinfo	*ai_next;		// Next node
};
```
- `ai_flags` is a bit mask specifying input flags. It can be one or more of the following values OR-ed together:
	- `AI_ADDRCONFIG` - Only return IPv4 addresses if an IPv4 address is configured.
	- `AI_ALL` - Return all matching addresses.
	- `AI_CANONNAME` - Fill in the `ai_canonname` field of the `struct addrinfo` structure.
	- `AI_NUMERICHOST` - Return only numeric host addresses.
	- `AI_NUMERICSERV` - Return only numeric service addresses.
	- `AI_PASSIVE` - Return an address that can be used to bind to a socket for accepting incoming connections.
	- `AI_V4MAPPED` - If no IPv6 addresses are found, return IPv4-mapped IPv6 addresses.
- `ai_family` is the address family. It can be either `AF_INET` for IPv4 addresses or `AF_INET6` for IPv6 addresses.
- `ai_socktype` is the socket type. It can be either `SOCK_STREAM` for TCP sockets or `SOCK_DGRAM` for UDP sockets.
- `ai_protocol` is the protocol. It can be either `IPPROTO_TCP` for TCP sockets or `IPPROTO_UDP` for UDP sockets.
- `ai_addrlen` is the length of the `ai_addr` field.
- `ai_addr` is a pointer to a `struct sockaddr` structure that contains the address.
- `ai_canonname` is a pointer to a string containing the canonical name of the host.
- `ai_next` is a pointer to the next node in a linked list of `struct addrinfo` structures.

<details>
<summary>EXAMPLE:</summary>

```c
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>

int main() {
	struct addrinfo hints, *res;
	int status;

	memset(&hints, 0, sizeof hints);
	hints.ai_family = AF_UNSPEC; // AF_INET or AF_INET6 to force version
	hints.ai_socktype = SOCK_STREAM;

	if ((status = getaddrinfo("www.example.com", "80", &hints, &res)) != 0) {
		fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(status));
		return 1;
	}

	// res now points to a linked list of 1 or more struct addrinfos

	// ... do everything until you don't need res anymore ....

	freeaddrinfo(res); // free the linked list

	return 0;
}
```
The example above resolves the address of the host www.example.com on port 80.  
The `hints` structure is used to specify that we are looking for a stream (TCP) socket.  
If tha call to `getaddrinfo` is successful, the `res` pointer will point to a linked list of `struct addrinfo` structures that contain the resolved addresses.  
</details>


## freeaddrinfo
27 < < < < < < < < < < < < < < < < < < < FREE ADDR INFO > > > > > > > > > > > > > > > > > > > >

`freeaddrinfo` function in C is used to free the dynamically allocated memory of the linked list of `struct addrinfo` structures returned by the `getaddrinfo` function.  

`void freeaddrinfo(struct addrinfo *res)`  
1) `res` is a pointer to the first node in the linked list of `struct addrinfo` structures.

see example 26 [(getaddrinfo)](#getaddrinfo)


## setsockopt
28 < < < < < < < < < < < < < < < < < < < SETSOCKOPT > > > > > > > > > > > > > > > > > > > >

`setsockopt` function in C is used to set socket options.  
It is typically used in conjunction with the `socket` function to set socket options before binding the socket.  
Options may exist at multiple protocol levels, but they are always present at the uppermost socket level.  

`int setsockopt(int sockfd, int level, int optname, const void *optval, socklen_t optlen)`  
it returns 0 on success, or -1 if an error occurs.
1) `sockfd` is the file descriptor returned by `socket`.
2) `level` specifies the protocol level at which the option resides. It can be one of the following values:
	- `SOL_SOCKET` - Socket level
	- `IPPROTO_IP` - IP level
	- `IPPROTO_IPV6` - IPv6 level
	- `IPPROTO_TCP` - TCP level
	- `IPPROTO_UDP` - UDP level
3) `optname` specifies the option to set. It can be one of the following values:
	- `SO_ACCEPTCONN` - Accept connections on socket
	- `SO_BROADCAST` - Allow transmission of broadcast messages
	- `SO_DEBUG` - Record debugging information
	- `SO_DONTROUTE` - Bypass routing table
	- `SO_KEEPALIVE` - Keep connection alive
	- `SO_LINGER` - Linger on close if data is present
	- `SO_OOBINLINE` - Receive out-of-band data in band
	- `SO_RCVBUF` - Receive buffer size
	- `SO_RCVLOWAT` - Receive low-water mark
	- `SO_RCVTIMEO` - Receive timeout
	- `SO_REUSEADDR` - Allow reuse of local addresses
	- `SO_SNDBUF` - Send buffer size
	- `SO_SNDLOWAT` - Send low-water mark
	- `SO_SNDTIMEO` - Send timeout
	- `SO_TYPE` - Get socket type
4) `optval` is a pointer to a buffer containing the value of the option to set.
5) `optlen` is the size of the buffer.

<details>
<summary>EXAMPLE:</summary>

```c
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>

int main() {
	int sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if (sockfd < 0) {
		perror("socket");
		return 1;
	}

	int enable = 1;
	if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &enable, sizeof(int)) < 0) {
		perror("setsockopt");
		return 1;
	}

	// Now the socket can be used with the SO_REUSEADDR option enabled

	return 0;
}
```
The example above creates a TCP socket and sets the `SO_REUSEADDR` option on the socket.  
This option allows the socket to reuse the address and port that is in the `TIME_WAIT` state.  
This can be useful in a server that has been restarted and needs to bind to the same address and port.  
</details>


## getsockname
29 < < < < < < < < < < < < < < < < < < < GETSOCKNAME > > > > > > > > > > > > > > > > > > > >

`getsockname` function in C is used to retrieve the local name (address and port) of a socket.  
This is useful when you want to know the address and port assigned to a socket by the system when you did not bind the socket manually.  

`int getsockname(int sockfd, struct sockaddr *addr, socklen_t *addrlen)`  
it returns 0 on success, or -1 if an error occurs.  
1) `sockfd` is the file descriptor returned by `socket`.  
2) `addr` is a pointer to a `struct sockaddr` structure that will store the local address.  
3) `addrlen` is a pointer to a `socklen_t` variable that will store the size of the `struct sockaddr` structure.

<details>
<summary>EXAMPLE:</summary>

```c
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

int main() {
	int sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if (sockfd < 0) {
		perror("socket");
		return 1;
	}

	struct sockaddr_in serv_addr;
	socklen_t addrlen = sizeof(serv_addr);

	if (getsockname(sockfd, (struct sockaddr *) &serv_addr, &addrlen) < 0) {
		perror("getsockname");
		return 1;
	}

	printf("Local IP address is: %s\n", inet_ntoa(serv_addr.sin_addr));
	printf("Local port is: %d\n", (int) ntohs(serv_addr.sin_port));

	return 0;
}
```
The example above creates a TCP socket and retrieves the local address and port assigned to the socket by the system.  
- `getsockname` function is used to retrieve the local address and port assigned to a socket.  
- `inet_ntoa` function is used to convert the binary representation of the IP address to a string in dotted-decimal notation.  
- `ntohs` function is used to convert the port number from network byte order to host byte order.  
</details>


## getprotobyname
30 < < < < < < < < < < < < < < < < < < < GETPROTOBYNAME > > > > > > > > > > > > > > > > > > > >

`getprotobyname` function in C is used to retrieve the protocol information corresponding to a protocol name.  
It is typically used in conjunction with the `socket` function to specify the protocol to use when creating a socket.  

`struct protoent *getprotobyname(const char *name)`  
it returns a pointer to a `struct protoent` structure on success, or NULL if an error occurs.
1) `name` is a pointer to a string containing the protocol name.

```c
struct protoent {
		char	*p_name;		// Official protocol name
		char	**p_aliases;	// Alias list
		int		p_proto;		// Protocol number
};
```
- `p_name` is a pointer to a string containing the official protocol name.
- `p_aliases` is a pointer to a NULL-terminated list of alternate names.
- `p_proto` is the protocol number.

<details>
<summary>EXAMPLE:</summary>

```c
#include <stdio.h>
#include <netdb.h>

int main() {
	struct protoent *pe;

	pe = getprotobyname("tcp");
	if (pe) {
		printf("Name: %s, Protocol number: %d\n", pe->p_name, pe->p_proto);
	} else {
		printf("Protocol not found\n");
	}

	return 0;
}
```
The example above retrieves the protocol information for the TCP protocol.
The `getprotobyname` function is used to retrieve the protocol information for the TCP protocol.
The `struct protoent` structure contains the protocol name and protocol number.
</details>


## fcntl
31 < < < < < < < < < < < < < < < < < < < FCNTL > > > > > > > > > > > > > > > > > > > >

`fcntl` function in C is used to perform various operations on files or file descriptors.  
It can be used to gat or set the file attributes, control locks on files, and more.  

`int fcntl(int fd, int cmd, ...)`  
it returns the result of the operation, or -1 if an error occurs.  
1) `fd` is the file descriptor to operate on.  
2) `cmd` is the operation to perform. It can be one of the following values:  
	- `F_DUPFD` - Duplicate file descriptor  
	- `F_GETFD` - Get file descriptor flags  
	- `F_SETFD` - Set file descriptor flags  
	- `F_GETFL` - Get file status flags  
	- `F_SETFL` - Set file status flags  
	- `F_GETLK` - Get record locking information  
	- `F_SETLK` - Set record locking information  
	- `F_SETLKW` - Set record locking information; wait if blocked  
	- `F_GETOWN` - Get process or process group ID to receive SIGURG signals  
	- `F_SETOWN` - Set process or process group ID to receive SIGURG signals  
	- `F_GETLK64` - Get record locking information  
	- `F_SETLK64` - Set record locking information  
	- `F_SETLKW64` - Set record locking information; wait if blocked  
	- `F_SETOWN_EX` - Set process or process group ID to receive SIGURG signals  
	- `F_GETOWN_EX` - Get process or process group ID to receive SIGURG signals  
	- `F_GETOWNER_UIDS` - Get owner UID and filesystem UID  
	- `F_OFD_GETLK` - Get record locking information  
	- `F_OFD_SETLK` - Set record locking information  
	- `F_OFD_SETLKW` - Set record locking information; wait if blocked  
	- `F_DUPFD_CLOEXEC` - Duplicate file descriptor with close-on-exit set  
	- `F_SETLEASE` - Set a lease  
	- `F_GETLEASE` - Get a lease  
	- `F_NOTIFY` - Request notifications on a directory  
	- `F_SETPIPE_SZ` - Set pipe page size array  
	- `F_GETPIPE_SZ` - Get pipe page size array  
	- `F_ADD_SEALS` - Add seals  
	- `F_GET_SEALS` - Get seals  
	- `F_GET_RW_HINT` - Get read/write hints  
	- `F_SET_RW_HINT` - Set read/write hints  
	- `F_GET_FILE_RW_HINT` - Get file read/write hints  
	- `F_SET_FILE_RW_HINT` - Set file read/write hints  

<details>
<summary>EXAMPLE:</summary>

```c
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>

int main() {
	int fd = open("test.txt", O_RDONLY);
	if (fd < 0) {
		perror("open");
		return 1;
	}

	int flags = fcntl(fd, F_GETFL);
	if (flags < 0) {
		perror("fcntl");
		return 1;
	}

	printf("File status flags: %o\n", flags);

	close(fd);

	return 0;
}
```
The example above opens a file and retrieves the file status flags using the `fcntl` function.  
The flags are printed in octal format.  
</details>


## open
32 < < < < < < < < < < < < < < < < < < < OPEN > > > > > > > > > > > > > > > > > > > >

`open` function in C is used to open a file and return a file descriptor, which is an
integer that uniquely identifies an open file of the process.  

`int open(const char *pathname, int flags, mode_t mode)`  
it returns a file descriptor on success, or -1 if an error occurs.  
1) `pathname` is a pointer to a string containing the path of the file to open.
2) `flags` specifies the flags to use when opening the file. It can be one or more of the following values OR-ed together:
	- `O_RDONLY` - Open for reading only
	- `O_WRONLY` - Open for writing only
	- `O_RDWR` - Open for reading and writing
	- `O_NONBLOCK` - Do not block on open
	- `O_APPEND` - Append on each write
	- `O_CREAT` - Create file if it does not exist
	- `O_TRUNC` - Truncate size to 0
	- `O_EXCL` - Error if `O_CREAT` and the file exists
	- `O_SHLOCK` - Atomically obtain a shared lock
	- `O_EXLOCK` - Atomically obtain an exclusive lock
	- `O_NOFOLLOW` - Do not follow symlinks
	- `O_SYMLINK` - Allow open of symlinks
	- `O_EVTONLY` - Descriptor requested for event notifications only
	- `O_CLOEXEC` - Mark as close-on-exec
3) `mode` specifies the permissions to use when creating the file. It is only used when `O_CREAT` is specified in the `flags` argument. It can be one or more of the following values OR-ed together:
	- `S_IRWXU` - Read, write, execute by owner
	- `S_IRUSR` - Read by owner
	- `S_IWUSR` - Write by owner
	- `S_IXUSR` - Execute by owner
	- `S_IRWXG` - Read, write, execute by group
	- `S_IRGRP` - Read by group
	- `S_IWGRP` - Write by group
	- `S_IXGRP` - Execute by group
	- `S_IRWXO` - Read, write, execute by others
	- `S_IROTH` - Read by others
	- `S_IWOTH` - Write by others
	- `S_IXOTH` - Execute by others

<details>
<summary>EXAMPLE:</summary>

```c
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>

int main() {
	int fd = open("test.txt", O_RDONLY);
	if (fd < 0) {
		perror("open");
		return 1;
	}

	// Do something with the file

	if (close(fd) < 0) {
		perror("close");
		return 1;
	}

	return 0;
}
```
The example above opens a file and then closes it using the `close` function.  
</details>


## close
33 < < < < < < < < < < < < < < < < < < < CLOSE > > > > > > > > > > > > > > > > > > > >

`close` function in C is used to close a file descriptor. It is typically used in  
conjunction with the `open` function to close a file after it has been opened.  
 
`int close(int fd)`  
it returns 0 on success, or -1 if an error occurs.  
1) `fd` is the file descriptor to close.  
 
see example 32 [(open)](#open)


## read
34 < < < < < < < < < < < < < < < < < < < READ > > > > > > > > > > > > > > > > > > > >

`read` function in C is used to read data from a file descriptor. It is typically used
in conjunction with the `open` function to read data from a file after it has been opened.

`ssize_t read(int fd, void *buf, size_t count)`  
it returns the number of bytes read, or -1 if an error occurs.
1) `fd` is the file descriptor to read from.
2) `buf` is a pointer to the buffer where the data will be stored.
3) `count` is the size of the buffer.

<details>
<summary>EXAMPLE:</summary>

```c
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>

int main() {
	int fd = open("test.txt", O_RDONLY);
	if (fd < 0) {
		perror("open");
		return 1;
	}

	char buffer[256];
	int n = read(fd, buffer, sizeof(buffer) - 1);
	if (n < 0) {
		perror("read");
		return 1;
	}

	buffer[n] = '\0';
	printf("Read: %s\n", buffer);

	if (close(fd) < 0) {
		perror("close");
		return 1;
	}

	return 0;
}
```
The example above opens a file and reads its contents using the `read` function.  
The contents of the file are stored in a buffer and printed to the standard output.  
</details>


## write
35 < < < < < < < < < < < < < < < < < < < WRITE > > > > > > > > > > > > > > > > > > > >

`write` function in C is used to write data from a buffer to a file descriptor.  
It is typically used in conjunction with the `open` function to write data to a file
after it has been opened.  

`ssize_t write(int fd, const void *buf, size_t count)`  
it returns the number of bytes written, or -1 if an error occurs.  
1) `fd` is the file descriptor to write to.  
2) `buf` is a pointer to the buffer containing the data to write.  
3) `count` is the size of the buffer.  

<details>
<summary>EXAMPLE:</summary>

```c
#include <stdio.h>
#include <fcntl.h>

int main() {
	int fd = open("test.txt", O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (fd < 0) {
		perror("open");
		return 1;
	}

	const char *message = "Hello, World!";
	if (write(fd, message, strlen(message)) < 0) {
		perror("write");
		return 1;
	}

	if (close(fd) < 0) {
		perror("close");
		return 1;
	}

	return 0;
}
```
The example above opens a file and writes the message "Hello, World!" to it using the `write` function.  
</details>


## waitpid
36 < < < < < < < < < < < < < < < < < < < WAITPID > > > > > > > > > > > > > > > > > > > >

`waitpid` function in C is used to wait for a child process to change state.  
It is typically used in conjunction with the `fork` function to wait for the child process
to terminate before continuing.

`pid_t waitpid(pid_t pid, int *status, int options)`  
it returns the process ID of the child process that terminated, or -1 if an error occurs.  
1) `pid` is the process ID of the child process to wait for. It can be one of the following values:
	- `> 0` - Wait for the child process with the specified PID.
	- `0` - Wait for any child process in the same process group as the current process.
	- `-1` - Wait for any child process.
	- `< -1` - Wait for any child process whose process group ID is equal to the absolute value of `pid`.
2) `status` is a pointer to an integer that will store the exit status of the child process.
3) `options` specifies the behavior of the `waitpid` function. It can be one or more of the following values OR-ed together:
	- `WCONTINUED` - Wait for a continued child process.
	- `WNOHANG` - Return immediately if no child process has exited.
	- `WUNTRACED` - Wait for a stopped child process.

<details>
<summary>EXAMPLE:</summary>

```c
#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

int main() {
	pid_t pid = fork();
	if (pid < 0) {
		perror("fork");
		return 1;
	}

	if (pid == 0) {
		// Child process
		printf("Child process\n");
	} else {
		// Parent process
		printf("Parent process\n");

		int status;
		if (waitpid(pid, &status, 0) < 0) {
			perror("waitpid");
			return 1;
		}

		if (WIFEXITED(status)) {
			printf("Child process exited with status: %d\n", WEXITSTATUS(status));
		} else if (WIFSIGNALED(status)) {
			printf("Child process was terminated by signal: %d\n", WTERMSIG(status));
		} else if (WIFSTOPPED(status)) {
			printf("Child process was stopped by signal: %d\n", WSTOPSIG(status));
		}
	}

	return 0;
}
```
The example above forks a child process and waits for it to terminate using the `waitpid` function.  
- `WIFEXITED` macro is used to check if the child process terminated normally.  
- `WEXITSTATUS` macro is used to retrieve the exit status of the child process.  
- `WIFSIGNALED` macro is used to check if the child process was terminated by a signal.  
- `WTERMSIG` macro is used to retrieve the signal number that terminated the child process.  
- `WIFSTOPPED` macro is used to check if the child process was stopped by a signal.  
- `WSTOPSIG` macro is used to retrieve the signal number that stopped the child process.  
</details>


## kill
37 < < < < < < < < < < < < < < < < < < < KILL > > > > > > > > > > > > > > > > > > > >

`kill` function in C is used to send a signal to a process or a group of processes.  
It can be used to send various signals to a process, such as `SIGKILL`, `SIGSTOP`, `SIGTERM`, etc.  

`int kill(pid_t pid, int sig)`  
it returns 0 on success, or -1 if an error occurs.  
1) `pid` is the process ID of the process to send the signal to. It can be one of the following values:  
	- `> 0` - Send the signal to the process with the specified PID.  
	- `0` - Send the signal to all processes in the same process group as the current process.  
	- `-1` - Send the signal to all processes except the system processes.  
	- `< -1` - Send the signal to all processes whose process group ID is equal to the absolute value of `pid`.  
2) `sig` is the signal to send. It can be one of the following values:  
	- `SIGABRT` - Abort signal  
	- `SIGALRM` - Alarm clock  
	- `SIGBUS` - Access to an undefined portion of a memory object  
	- `SIGCHLD` - Child process terminated, stopped, or continued  
	- `SIGCONT` - Continue executing, if stopped  
	- `SIGFPE` - Erroneous arithmetic operation  
	- `SIGHUP` - Hangup  
	- `SIGILL` - Illegal instruction  
	- `SIGINT` - Terminal interrupt signal  
	- `SIGKILL` - Kill (cannot be caught or ignored)  
	- `SIGPIPE` - Write on a pipe with no one to read it  
	- `SIGQUIT` - Terminal quit signal  
	- `SIGSEGV` - Invalid memory reference  
	- `SIGSTOP` - Stop executing (cannot be caught or ignored)  
	- `SIGTERM` - Termination signal  
	- `SIGTSTP` - Terminal stop signal  
	- `SIGTTIN` - Background process attempting read  
	- `SIGTTOU` - Background process attempting write  
	- `SIGUSR1` - User-defined signal 1  
	- `SIGUSR2` - User-defined signal 2  
	- `SIGPOLL` - Pollable event  
	- `SIGPROF` - Profiling timer expired  
	- `SIGSYS` - Bad system call  
	- `SIGTRAP` - Trace/breakpoint trap  
	- `SIGURG` - High bandwidth data is available at a socket  
	- `SIGVTALRM` - Virtual timer expired  
	- `SIGXCPU` - CPU time limit exceeded  
	- `SIGXFSZ` - File size limit exceeded  

<details>
<summary>EXAMPLE:</summary>

```c
#include <stdio.h>
#include <signal.h>
#include <unistd.h>

int main() {
	pid_t pid = fork();
	if (pid < 0) {
		perror("fork");
		return 1;
	}

	if (pid == 0) {
		// Child process
		printf("Child process\n");
		pause(); // Wait for any signal
	} else {
		// Parent process
		printf("Parent process\n");
		sleep(1); // Let the child process print its message
		if (kill(pid, SIGTERM) < 0) {
			perror("kill");
			return 1;
		}
	}

	return 0;
}
```
The example above forks a child process and sends the `SIGTERM` signal 
to the child process using the `kill` function.
</details>


## signal
38 < < < < < < < < < < < < < < < < < < < SIGNAL > > > > > > > > > > > > > > > > > > > >

`signal` function in C is used to change the action taken by a process on receipt of a signal.  
It can be used to define a custom handler function that will be called when the process 
receives a specific signal.  

`void (*signal(int sig, void (*func)(int)))(int)`  
it returns a pointer to the previous handler function, or `SIG_ERR` if an error occurs.  
1) `sig` is the signal to handle. It can be one of the following values:  
	- `SIGABRT` - Abort signal  
	- `SIGALRM` - Alarm clock  
	- `SIGBUS` - Access to an undefined portion of a memory object  
	- `SIGCHLD` - Child process terminated, stopped, or continued  
	- `SIGCONT` - Continue executing, if stopped  
	- `SIGFPE` - Erroneous arithmetic operation  
	- `SIGHUP` - Hangup  
	- `SIGILL` - Illegal instruction  
	- `SIGINT` - Terminal interrupt signal  
	- `SIGKILL` - Kill (cannot be caught or ignored)  
	- `SIGPIPE` - Write on a pipe with no one to read it  
	- `SIGQUIT` - Terminal quit signal  
	- `SIGSEGV` - Invalid memory reference  
	- `SIGSTOP` - Stop executing (cannot be caught or ignored)  
	- `SIGTERM` - Termination signal  
	- `SIGTSTP` - Terminal stop signal  
	- `SIGTTIN` - Background process attempting read  
	- `SIGTTOU` - Background process attempting write  
	- `SIGUSR1` - User-defined signal 1  
	- `SIGUSR2` - User-defined signal 2  
	- `SIGPOLL` - Pollable event  
	- `SIGPROF` - Profiling timer expired  
	- `SIGSYS` - Bad system call  
	- `SIGTRAP` - Trace/breakpoint trap  
	- `SIGURG` - High bandwidth data is available at a socket  
	- `SIGVTALRM` - Virtual timer expired  
	- `SIGXCPU` - CPU time limit exceeded  
	- `SIGXFSZ` - File size limit exceeded  
2) `func` is a pointer to the handler function to call when the process receives the signal.

<details>
<summary>EXAMPLE:</summary>

```c
#include <stdio.h>
#include <signal.h>
#include <unistd.h>

void sigint_handler(int sig_num) {
	printf("Received SIGINT, exiting...\n");
	exit(0);
}

int main() {
	signal(SIGINT, sigint_handler);

	printf("Press Ctrl+C to exit...\n");

	while(1) {
		sleep(1);
	}

	return 0;
}
```
The example above defines a custom handler function for the `SIGINT` signal 
using the `signal` function.  
`signal_handler` function is set to be called when the process receives the `SIGINT` signal,
(usually when the user presses Ctrl+C).  
The program then enters an infinite loop, and can be terminated by pressing Ctrl+C.  
</details>


## access
39 < < < < < < < < < < < < < < < < < < < ACCESS > > > > > > > > > > > > > > > > > > > >

`access` function in C is used to check the file accessability.  
It checks whether the process can access the file specified by the pathname, 
according to the value of `mode`.  
It is typically used to check if a file exists before opening it.  

`int access(const char *pathname, int mode)`  
it returns 0 if the file exists and the
process can access it, or -1 if an error occurs.
1) `pathname` is a pointer to a string containing the path of the file to check.
2) `mode` specifies the accessibility checks to perform. It can be one or more of the following values OR-ed together:
	- `F_OK` - Check if the file exists.
	- `R_OK` - Check if the file exists and the process can read it.
	- `W_OK` - Check if the file exists and the process can write to it.
	- `X_OK` - Check if the file exists and the process can execute it.

<details>
<summary>EXAMPLE:</summary>

```c
#include <stdio.h>
#include <unistd.h>

int main() {
	const char *pathname = "test.txt";

	if (access(pathname, F_OK) == 0) {
		printf("File exists.\n");
	} else {
		printf("File does not exist.\n");
	}

	if (access(pathname, R_OK) == 0) {
		printf("File is readable.\n");
	} else {
		printf("File is not readable.\n");
	}

	if (access(pathname, W_OK) == 0) {
		printf("File is writable.\n");
	} else {
		printf("File is not writable.\n");
	}

	if (access(pathname, X_OK) == 0) {
		printf("File is executable.\n");
	} else {
		printf("File is not executable.\n");
	}

	return 0;
}
```
The example above checks if the file test.txt exists and if the process can read, write,
and execute it using the `access` function.
</details>


## stat
40 < < < < < < < < < < < < < < < < < < < STAT > > > > > > > > > > > > > > > > > > > >

`stat` function in C is used to get the status of a file or a file system object.  
It retrieves information about the file pointed to by `pathname` and stores it in the
`struct stat` structure.  

`int stat(const char *pathname, struct stat *buf)` it returns 0 on success, or -1 if an error occurs.
1) `pathname` is a pointer to a string containing the path of the file to check.
2) `buf` is a pointer to a `struct stat` structure that will store the status of the file.

```c
struct stat {
    dev_t		st_dev;		// ID of device containing file
    ino_t		st_ino;		// inode number
    mode_t		st_mode;	// protection
    nlink_t		st_nlink;	// number of hard links
    uid_t		st_uid;		// user ID of owner
    gid_t		st_gid;		// group ID of owner
    dev_t		st_rdev;	// device ID (if special file)
    off_t		st_size;	// total size, in bytes
    blksize_t	st_blksize;	// blocksize for filesystem I/O
    blkcnt_t	st_blocks;	// number of 512B blocks allocated
    time_t		st_atime;	// time of last access
    time_t		st_mtime;	// time of last modification
    time_t		st_ctime;	// time of last status change
};
```
- `st_dev` is the ID of the device containing the file.
- `st_ino` is the inode number.
- `st_mode` is the protection.
- `st_nlink` is the number of hard links.
- `st_uid` is the user ID of the owner.
- `st_gid` is the group ID of the owner.
- `st_rdev` is the device ID (if the file is a special file).
- `st_size` is the total size, in bytes.
- `st_blksize` is the blocksize for filesystem I/O.
- `st_blocks` is the number of 512B blocks allocated.
- `st_atime` is the time of last access.
- `st_mtime` is the time of last modification.
- `st_ctime` is the time of last status change.

<details>
<summary>EXAMPLE:</summary>

```c
#include <stdio.h>
#include <sys/stat.h>
#include <time.h>

int main() {
	struct stat fileStat;
	if(stat("test.txt",&fileStat) < 0) {
		perror("stat");
		return 1;
	}

	printf("File Size: %ld bytes\n",fileStat.st_size);
	printf("File Permissions: ");
	printf( (S_ISDIR(fileStat.st_mode)) ? "d" : "-");
	printf( (fileStat.st_mode & S_IRUSR) ? "r" : "-");
	printf( (fileStat.st_mode & S_IWUSR) ? "w" : "-");
	printf( (fileStat.st_mode & S_IXUSR) ? "x" : "-");
	printf( (fileStat.st_mode & S_IRGRP) ? "r" : "-");
	printf( (fileStat.st_mode & S_IWGRP) ? "w" : "-");
	printf( (fileStat.st_mode & S_IXGRP) ? "x" : "-");
	printf( (fileStat.st_mode & S_IROTH) ? "r" : "-");
	printf( (fileStat.st_mode & S_IWOTH) ? "w" : "-");
	printf( (fileStat.st_mode & S_IXOTH) ? "x" : "-");
	printf("\nLast modified time: %s", ctime(&fileStat.st_mtime));

	return 0;
}
```
The example above gets the status of the file test.txt using the `stat` function.  
The size of the file, its permissions, and the last modified time.  
</details>


## opendir
41 < < < < < < < < < < < < < < < < < < < OPENDIR > > > > > > > > > > > > > > > > > > > >

`opendir` function in C is used to open a directory stream corresponding to
a given directory, so that the entries in the directory can be read.

`DIR *opendir(const char *name)` it returns a pointer to a `DIR` structure on success,
or NULL if an error occurs.
1) `name` is a pointer to a string containing the path of the directory to open.

```c
struct dirent {
	ino_t			d_ino;			// inode number
	off_t 			d_off;			// offset to the next dirent
	unsigned short	d_reclen;		// length of this record
	unsigned char	d_type;			// type of file
	char			d_name[256];	// filename
};
```
- `d_ino` is the inode number.
- `d_off` is the offset to the next `dirent` structure.
- `d_reclen` is the length of this record.
- `d_type` is the type of file.
- `d_name` is the filename.

<details>
<summary>EXAMPLE:</summary>

```c
#include <stdio.h>
#include <dirent.h>

int main() {
	DIR *dir;
	struct dirent *ent;

	dir = opendir(".");
	if (dir == NULL) {
		perror("opendir");
		return 1;
	}

	while ((ent = readdir(dir)) != NULL) {
		printf("%s\n", ent->d_name);
	}

	closedir(dir);

	return 0;
}
```
The example of `opendir` function is used to open the current directory.  
The entries in the directory are then read one-by-one using the `readdir` function,
and their names are printed.  
Finally, the directory stream is closed using the `closedir` function.  
</details>


## readdir
42 < < < < < < < < < < < < < < < < < < < READDIR > > > > > > > > > > > > > > > > > > > >

`readdir` function in C is used to read the next entry from a directory stream.  
It is typically used in conjunction with the `opendir` function to read the entries
in a directory.  

`struct dirent *readdir(DIR *dirp)` it returns a pointer to a `struct dirent` structure  
on success, or NULL if the end of the directory stream is reached or an error occurs.  
1) `dirp` is a pointer to a `DIR` structure returned by the `opendir` function.  

see example 41 [(opendir)](#opendir)


## closedir
43 < < < < < < < < < < < < < < < < < < < CLOSEDIR > > > > > > > > > > > > > > > > > > > >

`closedir` function in C is used to close a directory stream.  
It is typically used in conjunction with the `opendir` function to close a directory
after it has been opened.  

`int closedir(DIR *dirp)`  
it returns 0 on success, or -1 if an error occurs.  
1) `dirp` is a pointer to a `DIR` structure returned by the `opendir` function.  

see example 41 [(opendir)](#opendir)
