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
#include "Player.hpp"

Player::Player(dynamicsWorldSP dynamicsWorld, GLuint programID, float size, glm::vec3 position)
    : Cube(programID,size,position)
{
    fallShape = std::make_shared<btBoxShape>(btVector3(size,size,size));
    fallMotionState = std::make_shared<btDefaultMotionState>(btTransform(btQuaternion(quaternion.x, quaternion.y, quaternion.z, quaternion.w),btVector3(position.x,position.y,position.z)));

    btScalar mass = 1;
    btVector3 fallInertia(0,0,0);
    fallShape->calculateLocalInertia(mass,fallInertia);
    btRigidBody::btRigidBodyConstructionInfo fallRigidBodyCI(mass,fallMotionState.get(),fallShape.get(),fallInertia);

    fallRigidBody = std::make_shared<btRigidBody>(fallRigidBodyCI);
    dynamicsWorld->addRigidBody(fallRigidBody.get());

}
