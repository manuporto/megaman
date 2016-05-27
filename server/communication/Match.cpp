#include <string>
#include <vector>
#include <iostream>

#include "Match.h"

#define PLAYERS_MAX 4

Match::Match(std::vector<ServerCommunicator*>& communicators)
    : communicators(communicators) {}

bool Match::has_started() {
    Lock l(this->m);
    return this->stage_id() != 0;
}

bool Match::is_full() {
    Lock l(this->m);
    return this->communicators.size() >= PLAYERS_MAX;
}

bool Match::has_host() { return this->communicators.size() > 0; }

void Match::send_team_to_new_player(ServerCommunicator* new_comm) {
    for (unsigned int i = 0; i < this->communicators.size() - 1; ++i)
        new_comm->send_new_player_notification(this->communicators[i]->name());
}

void Match::send_new_player_to_team(ServerCommunicator* new_comm) {
    for (unsigned int i = 0; i < this->communicators.size() - 1; ++i)
        this->communicators[i]->send_new_player_notification(new_comm->name());
}

void Match::send_stage_pick_to_team(const char stage_id) {
    for (unsigned int i = 0; i < this->communicators.size(); ++i)
        this->communicators[i]->send_stage_pick(stage_id);
}

void Match::send_stage_info(StageInfo* info) {
    for (unsigned int i = 0; i < this->communicators.size(); ++i)
        this->communicators[i]->send_stage_info(info);
}

void Match::add_player(int fd) {
    Lock l(this->m);

    if (this->communicators.size() >= PLAYERS_MAX)
        throw MatchError("Mega Man Co-op match is full");
    if (this->stage_id() != 0)
        throw MatchError("Mega Man Co-op match has already started");

    Player* new_player = new Player();

    if (!this->has_host()) {
        HostCommunicator* hc = new HostCommunicator(new_player,
                                                    fd,
                                                    this->stage_id);
        this->communicators.push_back(hc);
        this->game.new_player(new_player);
        hc->receive_name();

    } else {
        ServerCommunicator* c = new ServerCommunicator(new_player, fd);
        this->communicators.push_back(c);
        this->game.new_player(new_player);
        c->receive_name();

        this->send_team_to_new_player(c);
        this->send_new_player_to_team(c);
    }
}

void Match::start_stage() {
    std::cout << "Stage started" << std::endl;
    ((HostCommunicator*)this->communicators[0])->receive_stage_id();
    this->send_stage_pick_to_team(this->stage_id());

    StageInfo* info = StageFactory::stage_info(this->stage_id());

    this->send_stage_info(info);
    this->game.set_stage(info);

    //TODO ...

    delete info;
}

Match::~Match() {}

MatchError::MatchError(const std::string error_msg) throw()
        : error_msg(error_msg) {}

const char* MatchError::what() const throw() { return error_msg.c_str(); }

MatchError::~MatchError() throw() {}
