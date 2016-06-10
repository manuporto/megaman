#ifndef MEGAMAN_CAMERA_H
#define MEGAMAN_CAMERA_H

#include <extern/libSDL2pp/SDL2pp/Renderer.hh>
#include <utility>
#include <map>

#include "ActorRenderer.h"
class MegaManRenderer;
typedef std::pair<int, int> AdjustedPos;
typedef std::pair<int, int> AdjustedSize;
class Camera {
public:
    explicit Camera(SDL2pp::Renderer *renderer);
    AdjustedPos adjust_position(float x, float y);
    AdjustedSize adjust_size(int x, int y);
    void add_megaman(int id, MegaManRenderer* megaman);
    ~Camera();
private:
    void calculate_baricenter();
    SDL2pp::Renderer *renderer;
    int offset_x;
    int offset_y;
    std::map<int, MegaManRenderer*> megs;
};


#endif //MEGAMAN_CAMERA_H
