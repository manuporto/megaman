#include <string>
#include <utility>
#include <vector>

#include "server/communication/InfoMaker.h"
#include "GameObjectHandler.h"
#include "GameObjectSetter.h"
#include "Enemy.h"
#include "Boss.h"

void GameObjectHandler::set(const std::string& info) {
    GameObjectSetter setter(info, this);
}

void GameObjectHandler::add_game_object(GameObject* object) {
    this->objects.push_back(object);
    this->object_id[object] = this->objects.size();
}

const std::string GameObjectHandler::status() {
    InfoMaker info;
    std::pair<std::string, std::string> status;
    for (unsigned int i = 0; i < this->objects.size(); ++i) {
        status = this->objects[i]->info(this->object_id[this->objects[i]]);
        info.add(status.first, status.second);
    }
    return info.str();
}

void GameObjectHandler::tick() {
    for (unsigned int i = 0; i < this->objects.size(); ++i)
        this->objects[i]->tick();
}

void GameObjectHandler::check_collisions() {
    GameObject* current_object;
    for (unsigned int i = 0; i < this->objects.size(); ++i) {
        current_object = this->objects[i];
        for (unsigned int j = 0; j < this->objects.size(); ++j) {
            if (i == j) continue;
            if (current_object->collided_with(this->objects[j]))
                current_object->execute_collision_with(this->objects[j]);
        }
    }
}

std::vector<int> GameObjectHandler::get_rid_of_corpses() {
    std::vector<int> deceased;
    GameObject* dead_obj;
    for (unsigned int i = 0; i < this->objects.size(); ++i) {
        if (this->objects[i]->is_dead()) {
            dead_obj = this->objects[i];

            deceased.push_back(this->object_id[dead_obj]);

            this->object_id.erase(dead_obj);
            this->objects.erase(this->objects.begin() + i);

            if (!dead_obj->is_megaman()) delete dead_obj;
        }
    }
    return deceased;
}

void GameObjectHandler::create_new_projectiles() {
//    Shooter* shooter;
    for (unsigned int i = 0; i < this->objects.size(); ++i) {
//        if (this->objects[i]->shoots_per_tick()) {
//            shooter = (Shooter*)this->objects[i];
//            shooter->shoot(this);
//        }
        if (this->objects[i]->is_enemy())
            ((Enemy*)this->objects[i])->shoot(this);
        if (this->objects[i]->is_boss())
            ((Boss*)this->objects[i])->shoot(this);
    }
}

std::vector<FloatUpdate*> GameObjectHandler::updates() {
    std::vector<FloatUpdate*> updates;
    for (unsigned int i = 0; i < this->objects.size(); ++i)
        if (this->objects[i]->it_moved())
            updates.push_back(this->objects[i]->update
                    (this->object_id[this->objects[i]]));
    return updates;
}

std::vector<float> GameObjectHandler::closest_enemy_for_megaman
        (const std::vector<float>& position) {
    std::vector<float> result;
    result[X_COORD_POS] = 0;
    result[Y_COORD_POS] = 0;
    for (unsigned int i = 0; i < this->objects.size(); ++i)
        if (this->objects[i]->is_enemy() || this->objects[i]->is_boss())
            return this->objects[i]->get_position();
    return result;
}

std::vector<float> GameObjectHandler::closest_megaman
        (const std::vector<float>& position) {
    std::vector<float> result;
    result[X_COORD_POS] = 0;
    result[Y_COORD_POS] = 0;
    for (unsigned int i = 0; i < this->objects.size(); ++i)
        if (this->objects[i]->is_megaman())
            return this->objects[i]->get_position();
    return result;
}

GameObjectHandler::~GameObjectHandler() {
    for (unsigned int i = 0; i < this->objects.size(); ++i)
        if (!this->objects[i]->is_megaman()) delete this->objects[i];
}
