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

void GameSystem::init()
{
    Log("GameSystem::GameSystem");    
}

void GameSystem::update()
{
    if(SigM->receive(Signals::Exit))
    {
        exit(0);
    }
    
    if(SigM->receive(Signals::MoveRight))
    {
        for(Entity entity : EM->entities){
            if(EM->has(entity,Parts::Signals)){
                auto &signals = EM->signals[entity];
                if(signals.signals.count(Signals::MoveRight) > 0){
                    EM->movement[entity].right = EM->movement[entity].speedRight;
                }
            }
        }
    }
    
    if(SigM->receive(Signals::StopMoveRight))
    {
        for(Entity entity : EM->entities){
            if(EM->has(entity,Parts::Signals)){
                auto &signals = EM->signals[entity];
                if(signals.signals.count(Signals::StopMoveRight) > 0){
                    EM->movement[entity].right = 0;
                }
            }
        }
    }

    if(SigM->receive(Signals::MoveLeft))
    {
        for(Entity entity : EM->entities){
            if(EM->has(entity,Parts::Signals)){
                auto &signals = EM->signals[entity];
                if(signals.signals.count(Signals::MoveLeft) > 0){
                    EM->movement[entity].left = EM->movement[entity].speedLeft;
                }
            }
        }
    }
    
    if(SigM->receive(Signals::StopMoveLeft))
    {
        for(Entity entity : EM->entities){
            if(EM->has(entity,Parts::Signals)){
                auto &signals = EM->signals[entity];
                if(signals.signals.count(Signals::StopMoveLeft) > 0){
                    EM->movement[entity].left = 0;
                }
            }
        }
    }

    if(SigM->receive(Signals::MoveForward))
    {
        for(Entity entity : EM->entities){
            if(EM->has(entity,Parts::Signals)){
                auto &signals = EM->signals[entity];
                if(signals.signals.count(Signals::MoveForward) > 0){
                    EM->movement[entity].forward = EM->movement[entity].speedForward;
                }
            }
        }
    }
    
    if(SigM->receive(Signals::StopMoveForward))
    {
        for(Entity entity : EM->entities){
            if(EM->has(entity,Parts::Signals)){
                auto &signals = EM->signals[entity];
                if(signals.signals.count(Signals::StopMoveForward) > 0){
                    EM->movement[entity].forward = 0;
                }
            }
        }
    }

    if(SigM->receive(Signals::MoveBackward))
    {
        for(Entity entity : EM->entities){
            if(EM->has(entity,Parts::Signals)){
                auto &signals = EM->signals[entity];
                if(signals.signals.count(Signals::MoveBackward) > 0){
                    EM->movement[entity].backward = EM->movement[entity].speedBackward;
                }
            }
        }
    }
    
    if(SigM->receive(Signals::StopMoveBackward))
    {
        for(Entity entity : EM->entities){
            if(EM->has(entity,Parts::Signals)){
                auto &signals = EM->signals[entity];
                if(signals.signals.count(Signals::StopMoveBackward) > 0){
                    EM->movement[entity].backward = 0;
                }
            }
        }
    }


    if(SigM->receive(Signals::MoveUp))
    {
        for(Entity entity : EM->entities){
            if(EM->has(entity,Parts::Signals)){
                auto &signals = EM->signals[entity];
                if(signals.signals.count(Signals::MoveUp) > 0){
                    EM->movement[entity].up = EM->movement[entity].speedUp;
                }
            }
        }
    }
    
    if(SigM->receive(Signals::StopMoveUp))
    {
        for(Entity entity : EM->entities){
            if(EM->has(entity,Parts::Signals)){
                auto &signals = EM->signals[entity];
                if(signals.signals.count(Signals::StopMoveUp) > 0){
                    EM->movement[entity].up = 0;
                }
            }
        }
    }

    if(SigM->receive(Signals::MoveDown))
    {
        for(Entity entity : EM->entities){
            if(EM->has(entity,Parts::Signals)){
                auto &signals = EM->signals[entity];
                if(signals.signals.count(Signals::MoveDown) > 0){
                    EM->movement[entity].down = EM->movement[entity].speedDown;
                }
            }
        }
    }
    
    if(SigM->receive(Signals::StopMoveDown))
    {
        for(Entity entity : EM->entities){
            if(EM->has(entity,Parts::Signals)){
                auto &signals = EM->signals[entity];
                if(signals.signals.count(Signals::StopMoveDown) > 0){
                    EM->movement[entity].down = 0;
                }
            }
        }
    }
    
}
