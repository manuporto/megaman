#include <vector>
#include <string>
#include <iostream>

#include "ServerCommunicator.h"

ServerReceiver::ServerReceiver(Socket& peer,
                               ReceivedPacketsProtected& packets)
        : Receiver(peer, packets) {
    this->start();
}

void ServerReceiver::receive_packet(const char id) {
    switch (id) {
        // Solo cases para los paquetes que pueden ser recibidos
        case NEW_PLAYER: {
            char name[NAME_LENGTH + 1];
            name[NAME_LENGTH] = '\0';
            this->socket.receive(name, sizeof(char) * NAME_LENGTH);
            this->packets.push(new NewPlayer(name));
            break;
        } case STAGE_PICK: {
            char stage_id;
            this->socket.receive(&stage_id, sizeof(char));
            this->packets.push(new StagePick(stage_id));
            break;
        } default:
            // Si el ID es desconocido, desecha el paquete
            break;
    }
}

ServerReceiver::~ServerReceiver() {}

NameWaiter::NameWaiter(Player* player,
                       ReceivedPacketsProtected& packets_received)
        : player(player), packets_received(packets_received) {
    this->start();
}

void NameWaiter::run() {
    while (this->packets_received.is_empty(NEW_PLAYER)) {}
    NewPlayer* packet = (NewPlayer*)this->packets_received.pop(NEW_PLAYER);
    this->player->set_name(packet->get_name());
    delete packet;
}

NameWaiter::~NameWaiter() { this->join(); }

ServerCommunicator::ServerCommunicator(Player* player, int fd)
    : player(player),
      peer(fd),
      receiver(peer, packets_received) {}

void ServerCommunicator::send_new_player_notification(const std::string& name) {
    std::cout << "Sending player name: "<< name << std::endl;
    this->packets_to_send.push(new NewPlayer(name));
    Sender s(this->peer, this->packets_to_send);
}

void ServerCommunicator::send_stage_pick(const char stage_id) {
    this->packets_to_send.push(new StagePick(stage_id));
    Sender s(this->peer, this->packets_to_send);
}

void ServerCommunicator::receive_name() {
    NameWaiter waiter(this->player, this->packets_received);
}

const std::string& ServerCommunicator::name() {
    return this->player->get_name();
}

void ServerCommunicator::send_stage_info(StageInfo* info) {
    std::vector<Position*> positions;

    //TODO arreglar memory leak de todos estos vectores

    positions = info->get_met_positions();
    for (unsigned int i = 0; i < positions.size(); ++i)
        this->packets_to_send.push
                (new StageElement(MET, positions[i]->clone()));
    positions = info->get_bumby_positions();
    for (unsigned int i = 0; i < positions.size(); ++i)
        this->packets_to_send.push
                (new StageElement(BUMBY, positions[i]->clone()));
    positions = info->get_sniper_positions();
    for (unsigned int i = 0; i < positions.size(); ++i)
        this->packets_to_send.push
                (new StageElement(SNIPER, positions[i]->clone()));
    positions = info->get_jumping_sniper_positions();
    for (unsigned int i = 0; i < positions.size(); ++i)
        this->packets_to_send.push
                (new StageElement(JUMPING_SNIPER, positions[i]->clone()));
    positions = info->get_block_positions();
    for (unsigned int i = 0; i < positions.size(); ++i)
        this->packets_to_send.push
                (new StageElement(BLOCK, positions[i]->clone()));
    positions = info->get_stairs_positions();
    for (unsigned int i = 0; i < positions.size(); ++i)
        this->packets_to_send.push
                (new StageElement(STAIRS, positions[i]->clone()));
    positions = info->get_spike_positions();
    for (unsigned int i = 0; i < positions.size(); ++i)
        this->packets_to_send.push
                (new StageElement(SPIKE, positions[i]->clone()));
    positions = info->get_cliff_positions();
    for (unsigned int i = 0; i < positions.size(); ++i)
        this->packets_to_send.push
                (new StageElement(CLIFF, positions[i]->clone()));

    Sender s(this->peer, this->packets_to_send);
}

void ServerCommunicator::shutdown() {
    this->peer.shutdown();
}

ServerCommunicator::~ServerCommunicator() {
    this->peer.shutdown();
}

StageIdWaiter::StageIdWaiter(StageIDProtected& stage_id,
                             ReceivedPacketsProtected& packets_received) :
        stage_id(stage_id), packets_received(packets_received) {}

void StageIdWaiter::run() {
    while (this->packets_received.is_empty(STAGE_PICK)) {}
    StagePick* packet = (StagePick*)this->packets_received.pop(STAGE_PICK);
    this->stage_id.set_id(packet->get_stage_id());
    std::cout << "Stage id received: " << packet->get_stage_id() << std::endl;
    delete packet;
}

StageIdWaiter::~StageIdWaiter() {}

HostCommunicator::HostCommunicator(Player* player,
                                   int fd,
                                   StageIDProtected& stage_id) :
        ServerCommunicator(player, fd),
        waiter(stage_id, this->packets_received) {
    this->waiter.start();
}

void HostCommunicator::receive_stage_id() {
    this->waiter.join();
}

HostCommunicator::~HostCommunicator() {}
