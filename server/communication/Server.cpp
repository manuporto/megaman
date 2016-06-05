#include <unistd.h>

#include "Server.h"
#include "common/communication/AddrInfo.h"
#include "Acceptor.h"

#define SLEEP_TIME_MICROSECONDS 1000000

void Server::configure_server_socket(const char* port) {
    ServerAddrInfo info(port);
    this->socket(info());
    this->socket.bind_and_listen(info());
}

Server::Server(const char* port)
    : quit_server(false), match(communicators) {
    this->configure_server_socket(port);
    this->start();
}

void Server::wait_for_players() {
    Acceptor acceptor(this->socket, this->match);

    while (!this->quit_server &&
           !this->match.is_full() && !this->match.has_started())
        usleep(SLEEP_TIME_MICROSECONDS);

    acceptor.shutdown();
}

void Server::start_game() { this->match.start_stage(); }

void Server::run() {
    this->wait_for_players();
    if (!this->quit_server) this->start_game();
}

void Server::shutdown() {
    this->quit_server = true;
    for (unsigned int i = 0; i < this->communicators.size(); ++i)
        this->communicators[i]->shutdown();
}

Server::~Server() {
    for (unsigned int i = 0; i < this->communicators.size(); ++i)
        delete this->communicators[i];
}
