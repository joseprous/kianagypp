#include "Game.hpp"
#include <SDL2/SDL.h>

Game::Game()
    : EM(std::make_shared<EntityManager>()),
      inputSystem(EM,0),
      renderSystem(EM,0),
      physicsSystem(EM,17)
{
    inputSystem.init();
    renderSystem.init();
    physicsSystem.init();
    Log("Game::Game");
}

void Game::loop()
{
    Log("Game::loop start");
    while(true){
        uint32_t ticks = SDL_GetTicks();
        inputSystem.update(ticks);
        physicsSystem.update(ticks);
        renderSystem.update(ticks);
    }
}
