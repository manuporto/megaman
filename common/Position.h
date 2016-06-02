#ifndef POSITION_H
#define POSITION_H

#include <vector>
#include <string>

class Clonable {
    public:
        virtual Clonable* clone() const = 0;
        virtual ~Clonable() {}
};

class Position : public Clonable {
    private:
        int x, y;
        int direction;

    public:
        Position(const int x, const int y);
        explicit Position(const std::vector<int>& position);
        void move(int x_amount, int y_amount);
        void forward();
        void backward();
        void reset();
        bool operator==(const Position& other) const;
        std::vector<int> get_position() const;
        std::string str() const;
        Position* clone() const;
        ~Position();
};

#endif //POSITION_H
