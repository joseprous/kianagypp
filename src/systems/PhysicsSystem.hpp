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

#ifndef PHYSICSSYSTEM_HPP
#define PHYSICSSYSTEM_HPP

#include "System.hpp"
#include "EntityManager.hpp"

class PhysicsSystem : public System
{
private:
    
    std::shared_ptr<btDbvtBroadphase> broadphase;
    std::shared_ptr<btDefaultCollisionConfiguration> collisionConfiguration;
    std::shared_ptr<btCollisionDispatcher> dispatcher;
    std::shared_ptr<btSequentialImpulseConstraintSolver> solver;
    
public:
    std::shared_ptr<btDiscreteDynamicsWorld> dynamicsWorld;

    PhysicsSystem(EntityManagerSP em, SignalsManagerSP sigm, AccumulatorsManagerSP accm, uint32_t period)
        :System("PhysicsSystem",em, sigm, accm, period){}

    void init() override;
    void update() override;
    using System::update;
};

#endif
