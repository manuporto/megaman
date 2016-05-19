#include <vector>

#include "Stage.h"

StageIDProtected::StageIDProtected() : stage_id(0) {}

void StageIDProtected::set_id(char stage_id) {
    Lock l(this->m);
    this->stage_id = stage_id;
}

char StageIDProtected::operator()() {
    Lock l(this->m);
    return this->stage_id;
}

StageIDProtected::~StageIDProtected() {}

Stage::Stage(char id, std::vector<Player*>& players) :
        id(id), players(players) {}

StageInfo* Stage::get_stage_info() {
    StageFactory factory;
    StageInfo* info = factory(this->id);
    //TODO Actualizar vector de enemigos
    //TODO Actualizar vector de objetos
    return info;
}

Stage::~Stage() {
    for (unsigned int i = 0; i < this->enemies.size(); ++i)
        delete this->enemies[i];
    for (unsigned int i = 0; i < this->objects.size(); ++i)
        delete this->objects[i];
}
