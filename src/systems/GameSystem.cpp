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

#include "GameSystem.hpp"
#include <glm/gtx/vector_angle.hpp>

void GameSystem::init()
{
    Log("GameSystem::GameSystem");
}

void GameSystem::update()
{
   
    if(auto move = SigM->receiveb(Signals::MoveRight))
    {
        for(Entity entity : EM->entities){
            if(auto signals = EM->getSignals(entity)){
                if(signals->signals.count(Signals::MoveRight) > 0){
                    if(*move){
                        EM->movement[entity].right = EM->movement[entity].speedRight;
                    }else{
                        EM->movement[entity].right = 0;
                    }
                }                
            }
        }
    }

    if(auto move = SigM->receiveb(Signals::MoveLeft))
    {
        for(Entity entity : EM->entities){
            if(auto signals = EM->getSignals(entity)){
                if(signals->signals.count(Signals::MoveLeft) > 0){
                    if(*move){
                        EM->movement[entity].left = EM->movement[entity].speedLeft;
                    }else{
                        EM->movement[entity].left = 0;
                    }
                }
            }
        }
    }
    

    if(auto move = SigM->receiveb(Signals::MoveForward))
    {
        for(Entity entity : EM->entities){
            if(auto signals = EM->getSignals(entity)){
                if(signals->signals.count(Signals::MoveForward) > 0){
                    if(*move){
                        EM->movement[entity].forward = EM->movement[entity].speedForward;
                    }else{
                        EM->movement[entity].forward = 0;
                    }
                }
            }
        }
    }
    
    if(auto move = SigM->receiveb(Signals::MoveBackward))
    {
        for(Entity entity : EM->entities){
            if(auto signals = EM->getSignals(entity)){
                if(signals->signals.count(Signals::MoveBackward) > 0){
                    if(*move){
                        EM->movement[entity].backward = EM->movement[entity].speedBackward;
                    }else{
                        EM->movement[entity].backward = 0;
                    }
                }
            }
        }
    }
    
    if(auto move = SigM->receiveb(Signals::MoveUp))
    {
        for(Entity entity : EM->entities){
            if(auto signals = EM->getSignals(entity)){
                if(signals->signals.count(Signals::MoveUp) > 0){
                    if(*move){
                        EM->movement[entity].up = EM->movement[entity].speedUp;
                    }else{
                        EM->movement[entity].up = 0;
                    }
                }
            }
        }
    }
    
    if(auto move = SigM->receiveb(Signals::MoveDown))
    {
        for(Entity entity : EM->entities){
            if(auto signals = EM->getSignals(entity)){
                if(signals->signals.count(Signals::MoveDown) > 0){
                    if(*move){
                        EM->movement[entity].down = EM->movement[entity].speedDown;
                    }else{
                        EM->movement[entity].down = 0;
                    }
                }
            }
        }
    }
    
    if(auto angleX = SigM->receivef(Signals::RotateX))
    {
        for(Entity entity : EM->entities){
            if(auto signals = EM->getSignals(entity)){
                if(signals->signals.count(Signals::RotateX) > 0){
                    auto rot = glm::angleAxis(*angleX,EM->position[entity].up);
                    EM->position[entity].quaternion = EM->position[entity].quaternion * rot;
                }
            }
        }
    }
}
