#include <errno.h>
#include <string.h>

#include <stdbool.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <netinet/in.h>

#include "commonSocket.h"

#define LISTEN_LIMIT 10
#define ERROR_CODE -1
#define SocketError_BUFFER_LEN 256

SocketError::SocketError() throw() {
	int _errno = errno;

	const char *m = strerror(_errno);
	strncpy(error_msg, m, SocketError_BUFFER_LEN);
	
	error_msg[SocketError_BUFFER_LEN - 1] = '\0'; 
}

const char* SocketError::what() const throw() {
	return error_msg;
}

SocketError::~SocketError() throw() {}

Socket::Socket() : fd(0) {}

Socket::Socket(int fd) : fd(fd) {}

void Socket::operator()(struct addrinfo* info) {
	this->fd = socket(info->ai_family, info->ai_socktype, info->ai_protocol);
	if (this->fd == ERROR_CODE)
		throw SocketError();
}

Socket::~Socket() {
	close(this->fd);
}

void Socket::bind_and_listen(struct addrinfo* info) {
	if (bind(this->fd, info->ai_addr, info->ai_addrlen) == ERROR_CODE)
		throw SocketError();

	if (listen(this->fd, LISTEN_LIMIT) == ERROR_CODE)
		throw SocketError();
}

void Socket::connect(struct addrinfo* info) {
	if (::connect(this->fd, info->ai_addr, info->ai_addrlen) == ERROR_CODE)
		throw SocketError();
}

int Socket::accept() {
	int new_fd = ::accept(this->fd, NULL, NULL);
	if (new_fd == ERROR_CODE)
		throw SocketError();
	return new_fd;
}

void Socket::send(const char* buffer, size_t size) {
	int sent, sent_now;
	sent = sent_now  = 0;
	bool valid_socket = true;

	while (sent < (signed)size && valid_socket) {
		sent_now = ::send(this->fd, &buffer[sent], size - sent, MSG_NOSIGNAL);
		
		if (!sent_now || sent_now == ERROR_CODE)
			valid_socket = false;
		else
			sent += sent_now;
	}
	
	if (!valid_socket) throw SocketError();
}

void Socket::receive(char* buffer, size_t size) {
	int received, received_now;
	received = received_now = 0;
	bool valid_socket = true;

	while (received < (signed)size && valid_socket) {
		received_now = recv(this->fd, &buffer[received], 
							size - received, MSG_NOSIGNAL);
      
		if (!received_now || received_now == ERROR_CODE)
			valid_socket = false;
		else
			received += received_now;
	}

	if (!valid_socket) throw SocketError();
}

void Socket::shutdown() {
	if (::shutdown(this->fd, SHUT_RDWR) == ERROR_CODE)
		throw SocketError();
}
