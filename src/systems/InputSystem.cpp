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

#include "InputSystem.hpp"
#include <SDL2/SDL.h>

void InputSystem::init()
{
    Log("InputSystem::InputSystem");    
}

void InputSystem::update()
{
    SDL_Event windowEvent;
    while(SDL_PollEvent(&windowEvent)) {
        SDL_Keysym keysym;
        switch (windowEvent.type){
        case SDL_QUIT:
            Log("InputSystem::update SDL_QUIT");
            exit(0);
            //quit = true;
            //break;
        case SDL_KEYDOWN:
            Log("InputSystem::update SDL_KEYDOWN");
            keysym = windowEvent.key.keysym;
            switch (keysym.sym) {
            case SDLK_ESCAPE:
                exit(0);
                //quit = true;
                //break;
            case SDLK_d:
                //camera.MoveRight(10*scale);
                break;
            case SDLK_a:  
                //camera.MoveLeft(10*scale);
                break;
            case SDLK_w:    
                //camera.MoveForward(10*scale);
                break;
            case SDLK_s:    
                //camera.MoveBackward(10*scale);
                break;
            case SDLK_q:    
                //camera.MoveUp(10*scale);
                break;
            case SDLK_e:    
                //camera.MoveDown(10*scale);
                break;
            case SDLK_LEFT:
                break;
            case SDLK_RIGHT:
                break;
            case SDLK_UP:
                break;
            case SDLK_DOWN:
                break;
            }
            break;
        case SDL_KEYUP:
            break;
        case SDL_MOUSEMOTION:

            /*float angleX = windowEvent.motion.xrel * (-1);
            float angleY = windowEvent.motion.yrel * (-1);
                
            camera.RotateX(angleX);
            camera.RotateY(angleY);*/
            break;
        }
    }
}
