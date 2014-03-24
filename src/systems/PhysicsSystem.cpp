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

#include "PhysicsSystem.hpp"

void PhysicsSystem::init()
{
    auto broadphase = std::make_shared<btDbvtBroadphase>();
        
    auto collisionConfiguration = std::make_shared<btDefaultCollisionConfiguration>();
    auto dispatcher = std::make_shared<btCollisionDispatcher>(collisionConfiguration.get());

    auto solver = std::make_shared<btSequentialImpulseConstraintSolver>();

    dynamicsWorld = std::make_shared<btDiscreteDynamicsWorld>(dispatcher.get(),broadphase.get(),solver.get(),collisionConfiguration.get());

    dynamicsWorld->setGravity(btVector3(0,0,-10));

    Log("PhysicsSystem::PhysicsSystem");
}

void PhysicsSystem::update()
{
    
}
