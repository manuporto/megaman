#include <stdio.h>

#include "Server.h"
#include "common/AddrInfo.h"
#include "Acceptor.h"
#include "QuitProtected.h"
#include "Match.h"

void Server::configure_server_socket(const char* port) {
	ServerAddrInfo info(port);
	this->server(info());
	this->server.bind_and_listen(info());
}

void Server::configure_factories() {

}

Server::Server(Socket& server, const char* port) :
server(server) {
	configure_server_socket(port);
	configure_factories();
}

void Server::operator()() {
	QuitProtected quit;
	Acceptor acceptor(this->server, this->match, quit);
	acceptor.start();
	while(!quit())
		if (getchar() == 'q') quit.switch_to_true();
	this->server.shutdown();
	acceptor.join();
}

Server::~Server() {}
