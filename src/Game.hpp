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

#ifndef GAME_HPP
#define GAME_HPP

#include "EntityManager.hpp"
#include "systems/RendererSystem.hpp"
#include "systems/PhysicsSystem.hpp"
#include "systems/InputSystem.hpp"
#include "systems/GameSystem.hpp"

#include "SignalsManager.hpp"
#include "AccumulatorsManager.hpp"

class Game
{
public:
    
    EntityManagerSP EM;
    SignalsManagerSP SigM;
    AccumulatorsManagerSP AccM;
    
    InputSystem inputSystem;
    GameSystem gameSystem;
    PhysicsSystem physicsSystem;
    RenderSystem renderSystem;

    Game();
    void loop();
};

#endif
