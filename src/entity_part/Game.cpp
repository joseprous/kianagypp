#include "Game.hpp"
#include <SDL2/SDL.h>

Game::Game()
    : EM(std::make_shared<EntityManager>()),
      inputSystem(EM,0),
      renderSystem(EM,0),
      physicsSystem(EM,17)
{
    Log("Game::Game");
}

void Game::loop()
{
    while(true){
        uint32_t ticks = SDL_GetTicks();
        //Log("Game::loop init");
        inputSystem.update(ticks);
        physicsSystem.update(ticks);
        renderSystem.update(ticks);
        //Log("Game::loop end");
    }
}
