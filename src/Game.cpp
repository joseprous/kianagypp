/*
kianagy++
Copyright (C) 2014 José Prous

This file is part of kianagy++.

kianagy++ is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

kianagy++ is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with kianagy++.  If not, see <http://www.gnu.org/licenses/>.
*/

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
