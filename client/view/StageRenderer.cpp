#include <string>
#include <SDL2pp/SDL2pp.hh>

#include "TileRenderer.h"
#include "common/StageParser.h"
#include "StageRenderer.h"

StageRenderer::StageRenderer(SDL2pp::Renderer *renderer) : renderer(renderer)
        , tiles(renderer), met(renderer) {
    background = new SDL2pp::Texture(*renderer, "resources/background.png");
}

void StageRenderer::render() {
    tile_renderers_t tile_renderers = tiles.get_renderers();

    // Render stage
    StageParserInfo info;
    for (auto const &iterator: info) {
        std::string tile_type = iterator.first;
        StageParserPositions positions = iterator.second;
        for (size_t i = 0; i < positions.size(); ++i){
            (tiles.*(tile_renderers[tile_type]))(positions[i][0],
                                                 positions[i][1]);
        }
    }
    renderer->Copy(*background);
    (tiles.*(tile_renderers["block"]))(0, renderer->GetOutputHeight() - 40);
    (tiles.*(tile_renderers["block"]))(50, renderer->GetOutputHeight() - 40);
    (tiles.*(tile_renderers["block"]))(100, renderer->GetOutputHeight() - 40);
    (tiles.*(tile_renderers["stairs"]))(0, renderer->GetOutputHeight() - 90);
//    tiles.render(0, renderer->GetOutputHeight() - 50);
//    tiles.render(50, renderer->GetOutputHeight() - 50);
//    tiles.render(100, renderer->GetOutputHeight() - 50);

    // Render actors (players, enemies, proyectiles)
    met.render(100, renderer->GetOutputHeight() - 100, 0);
}

StageRenderer::~StageRenderer() {
    delete background;
}
