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

#ifndef RENDERERSYSTEM_HPP
#define RENDERERSYSTEM_HPP

#include "EntityManager.hpp"
#include "System.hpp"
#include "ShaderManager.hpp"
#include "AccumulatorsManager.hpp"

#include <tuple>
#include <map>
#include <SDL2/SDL.h>

class RenderSystem : public System
{
private:
    
    ShaderManager SM;

    bool getCamera(glm::mat4 &view);

public:

    SDL_Window* window;

    RenderSystem(EntityManagerSP em, SignalsManagerSP sigm, AccumulatorsManagerSP accm, uint32_t period)
        :System("RenderSystem",em,sigm,accm,period){}
    
    void init() override;
    void update() override;
    using System::update;
};

#endif
