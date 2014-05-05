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
      SigM(std::make_shared<SignalsManager>()),
      AccM(std::make_shared<AccumulatorsManager>()),
      inputSystem(EM,SigM,AccM,0),
      gameSystem(EM,SigM,AccM,0),
      physicsSystem(EM,SigM,AccM,17),
      renderSystem(EM,SigM,AccM,0)
{
    inputSystem.init();
    gameSystem.init();
    physicsSystem.init();
    renderSystem.init();
    Log("Game::Game");
}

void Game::loop()
{
    Log("Game::loop start");
    uint32_t last_ticks = SDL_GetTicks();
    int frames = 0;
    while(true){
        if(SigM->receive(Signals::Exit)){
            break;
        }
        uint32_t ticks = SDL_GetTicks();
        inputSystem.update(ticks);
        gameSystem.update(ticks);
        physicsSystem.update(ticks);
        renderSystem.update(ticks);
        
        frames++;
        if(ticks > last_ticks + 1000){
            std::cout << "fps:" << frames << std::endl;  
            frames = 0;
            last_ticks = ticks;
        }
    }
    AccM->print_statistics();
}
