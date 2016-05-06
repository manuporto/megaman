#include "serverAcceptor.h"
#include "common/commonThread.h"
#include "common/commonSocket.h"
#include "QuitProtected.h"
#include "serverMatch.h"

#define PLAYERS_MAX 4

Acceptor::Acceptor(Socket& server, Match& match, QuitProtected& quit) :
	server(server), match(match), quit(quit) {}

void Acceptor::run() {
	int peers_fd;
	for (unsigned int i = 0; !this->quit() && i < PLAYERS_MAX && this->match.host_waiting(); ++i) {
		try {
			peers_fd = this->server.accept();
		} catch (const SocketError &e) {
			continue;
		}
		
		this->match.add_player(peers_fd);
	}
}

Acceptor::~Acceptor() {}