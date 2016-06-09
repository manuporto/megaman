#ifndef MEGAMAN_METRENDERER_H
#define MEGAMAN_METRENDERER_H

#include <map>
#include <SDL2pp/SDL2pp.hh>
#include <string>
#include <vector>

class ActorRenderer;

typedef void (ActorRenderer::*arenderer_method_t)(int, int, int, int);

typedef std::map<std::string, arenderer_method_t> ActorRenderers;

class ActorRendererr {
public:
    ActorRendererr(SDL2pp::Renderer *renderer, SDL2pp::Texture *sprites,
                   int pos_x, int pos_y);

    void update(int pos_x, int pos_y, int dir_x, int dir_y);

    virtual void render() = 0;

    virtual ~ActorRendererr() { }

protected:
    SDL2pp::Renderer *renderer;
    SDL2pp::Texture *sprites;
    int pos_x;
    int pos_y;
    int dir_x;
    int dir_y;
};

class MetRenderer : public ActorRendererr {
    MetRenderer(SDL2pp::Renderer *renderer, SDL2pp::Texture *sprites, int pos_x,
                int pos_y);

    virtual void render();
};

class ActorRenderer {
public:
    explicit ActorRenderer(SDL2pp::Renderer *renderer);

    ActorRenderers get_renderers();

    void render_megaman(int dest_x, int dest_y, int dir_x, int dir_y);

    void renderMet(int dest_x, int dest_y, int dir_x, int dir_y);

    void render_pellet(int dest_x, int dest_y, int dir_x, int dir_y);

    virtual ~ActorRenderer();

private:
    ActorRenderers renderers;
    SDL2pp::Renderer *renderer;
    SDL2pp::Texture *sprites;
    SDL2pp::Texture *megaman_sprites;
};


#endif //MEGAMAN_METRENDERER_H
