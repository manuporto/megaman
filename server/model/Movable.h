#ifndef MOVABLE_H
#define MOVABLE_H

#include <vector>

#include "common/Position.h"
#include "GameObject.h"

class Movable : public GameObject {
    private:
        int direction;
        const int velocity;
        int current_vel_x;
        int current_vel_y;

    public:
        Movable(const std::vector<int>& position,
                const int velocity);
        Movable(const int x, const int y,
                const int direction, const int velocity);
        void reset_position();
        void change_x_movement(bool pressed, bool forward);
        void change_y_movement(bool pressed, bool forward);
        void move();
        virtual ~Movable();
};

#endif //MOVABLE_H
