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
    broadphase = std::make_shared<btDbvtBroadphase>();
    
    collisionConfiguration = std::make_shared<btDefaultCollisionConfiguration>();
    dispatcher = std::make_shared<btCollisionDispatcher>(collisionConfiguration.get());

    solver = std::make_shared<btSequentialImpulseConstraintSolver>();

    dynamicsWorld = std::make_shared<btDiscreteDynamicsWorld>(dispatcher.get(),broadphase.get(),solver.get(),collisionConfiguration.get());

    dynamicsWorld->setGravity(btVector3(0,0,-10));

    Log("PhysicsSystem::PhysicsSystem");
}

void move(PositionPart &pos, const glm::vec3 &dir, float distance)
{
    if(distance < 0.00001 && distance > (-0.00001)) return; 
    glm::vec3 aux = pos.quaternion * dir;
    pos.position = pos.position + (aux * distance);    
}

void PhysicsSystem::update()
{
    //Log("PhysicsSystem::update");
    dynamicsWorld->stepSimulation(1/60.f,10);

    for(Entity entity : EM->entities){
        if(auto mov = EM->getMovement(entity)){
            /*if(auto coll = EM->getCollision(entity)){
                
              }else{*/
                if(auto pos = EM->getPosition(entity)){
                    move(*pos, glm::cross(pos->up, pos->direction), mov->left);
                    move(*pos, glm::cross(pos->direction, pos->up), mov->right);
                    move(*pos, pos->direction, mov->forward);
                    move(*pos, pos->direction, -mov->backward);
                    move(*pos, pos->up, mov->up);
                    move(*pos, pos->up, -mov->down);
                }
                //}
        }
        if(auto coll = EM->getCollision(entity)){
            if(coll->move){
                if(auto position = EM->getPosition(entity)){
                    btTransform trans;
                    coll->rigidBody->getMotionState()->getWorldTransform(trans);
                    auto pos = trans.getOrigin();

                    position->position.x = pos.getX();
                    position->position.y = pos.getY();
                    position->position.z = pos.getZ();

                    //Log(position->position);
                    if(coll->changeUp){
                        auto rot = trans.getRotation();
                        auto axis = rot.getAxis ();
                        auto angle = rot.getAngle();
                        glm::quat q = glm::angleAxis (glm::degrees(angle), glm::vec3(axis.getX(),axis.getY(),axis.getZ()));
                        position->quaternion = q;
                    }
                }
            }
        }
    }

}
