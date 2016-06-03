#ifndef ENEMY_H
#define ENEMY_H

#include <vector>
#include <string>

#include "common/Position.h"
#include "Movable.h"

#define MET_NAME "Met"
#define BUMBY_NAME "Bumby"
#define SNIPER_NAME "Sniper"
#define JUMPING_SNIPER_NAME "JumpingSniper"

class Object;
class Projectile;
class MegaMan;

class Enemy : public Movable {
    private:
        const std::string name;
        int energy;

    public:
        Enemy(const std::string& name, const std::vector<int>& position,
              const int velocity, int energy);
        const std::string& get_name();
        void decrease_energy(int amount);
        bool is_dead();
        void collide_with(Enemy* enemy);
        void collide_with(Object* object);
        void collide_with(Projectile* projectile);
        void collide_with(MegaMan* mm);
        void execute_collision_with(GameObject* other);
        virtual Projectile* shoot() = 0;
        virtual void tick() = 0;
        std::string info();
        virtual ~Enemy();
};

class Met : public Enemy {
    public:
        explicit Met(const std::vector<int>& position);
        Projectile* shoot();
        void tick();
        ~Met();
};

class Bumby : public Enemy {
    public:
        explicit Bumby(const std::vector<int>& position);
        Projectile* shoot();
        void tick();
        ~Bumby();
};

class Sniper : public Enemy {
    public:
        explicit Sniper(const std::vector<int>& position);
        Projectile* shoot();
        void tick();
        ~Sniper();
};

class JumpingSniper : public Enemy {
    public:
        explicit JumpingSniper(const std::vector<int>& position);
        Projectile* shoot();
        void tick();
        ~JumpingSniper();
};

#endif //ENEMY_H
