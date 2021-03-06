#ifndef BOSSCHAMBER_H
#define BOSSCHAMBER_H

#include <string>
#include <vector>
#include <map>

#include "server/communication/EventQueue.h"
#include "Player.h"
#include "Boss.h"
#include "GameObjectHandler.h"

class ServerCommunicator;
class Match;

class BossChamber : public GameObjectHandler {
    private:
        Match* match;
        Boss* boss;
        std::map<std::string, Player*> players;
        EventQueue& events;

        void execute_events();
        void execute_action(Player* player,
                            const char action_id, const bool pressed);
        void acknowledge_deceased();
        void acknowledge_updates();
        Player* player_with_name(const std::string& name);
        bool players_are_dead();

    public:
        BossChamber(Match* match,
                    std::vector<ServerCommunicator*>& communicators,
                    EventQueue& events,
                    const char boss_id);
        void run(bool* exit);
        bool beated();
        void reward_players();
        ~BossChamber();
};

class BossChamberError : public SystemError {
public:
    explicit BossChamberError(const std::string error_msg) throw();
};

#endif //BOSSCHAMBER_H
