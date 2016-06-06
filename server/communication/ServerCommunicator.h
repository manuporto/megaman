#ifndef COMMUNICATOR_H
#define COMMUNICATOR_H

#include <sstream>
#include <string>
#include <server/model/Player.h>

#include "common/Thread.h"
#include "common/communication/Socket.h"
#include "common/communication/Sender.h"
#include "common/communication/Receiver.h"
#include "common/communication/Packet.h"
#include "common/communication/QuitProtected.h"
#include "server/model/Stage.h"

class NameWaiter : public Thread {
    private:
        Player& player;
        ReceivedPacketsProtected& packets_received;

    public:
        NameWaiter(Player& player, ReceivedPacketsProtected& packets_received);
        void run();
        ~NameWaiter();
};

class ServerCommunicator {
    private:
        Player player;
        Socket* peer;
        Sender sender;
        PacketsQueueProtected packets_to_send;
        Receiver receiver;
    protected:
        ReceivedPacketsProtected packets_received;

    public:
        explicit ServerCommunicator(Socket* peer);
        void send_new_player_notification(const std::string& name);
        void send_stage_pick(const char stage_id);
        void receive_name();
        const std::string& name();
        Player* get_player();
        PacketsQueueProtected* get_actions();
        void send_stage_info(const std::string& info);
        void send_tick_info(const std::string& tick_info);
        virtual void shutdown();
        virtual ~ServerCommunicator();
};

class StageIDProtected {
private:
    Mutex m;
    char stage_id;

public:
    StageIDProtected();
    void set_id(char stage_id);
    char operator()();
    ~StageIDProtected();
};

class StageIdWaiter : public Thread {
    private:
        StageIDProtected stage_id;
        ReceivedPacketsProtected& packets_received;

    public:
        explicit StageIdWaiter(ReceivedPacketsProtected& packets_received);
        void run();
        char get_stage_id();
        ~StageIdWaiter();
};

class HostCommunicator : public ServerCommunicator {
    private:
        StageIdWaiter waiter;
    public:
        explicit HostCommunicator(Socket* peer);
        char check_stage_id();
        char receive_stage_id();
        ~HostCommunicator();
};

#endif  // COMMUNICATOR_H
