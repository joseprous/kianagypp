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

#include <iostream>

#include "Entity.hpp"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/rotate_vector.hpp>
using namespace glm;

/*void printVec3(std::string name, glm::vec3 v)
{
    std::cout << name << ": " << v.x << "," << v.y << "," << v.z << std::endl;    
}
*/
Entity::Entity(glm::vec3 position, glm::vec3 direction, glm::vec3 up)
{
    mPosition = position;
    mDirection = glm::normalize(direction);
    mUp = glm::normalize(up);
    mQuaternion = angleAxis((float)0.0, direction);
}

void Entity::Move(glm::vec3 dir, float distance)
{
    glm::vec3 aux = mQuaternion * dir;
    mPosition = mPosition + (aux * distance);    
}


void Entity::MoveForward(float distance)
{
    Move(mDirection, distance);
}

void Entity::MoveBackward(float distance)
{
    Move(mDirection * (-1.0f), distance);
}

void Entity::MoveRight(float distance)
{
    Move(cross(mDirection, mUp), distance);
}

void Entity::MoveLeft(float distance)
{
    Move(cross(mUp,mDirection), distance);
}

void Entity::MoveUp(float distance)
{
    Move(mUp, distance);
}

void Entity::MoveDown(float distance)
{
    Move(mUp * (-1.0f), distance);
}

void Entity::RotateX(float angle)
{
    mQuaternion = glm::rotate(mQuaternion,angle,  mUp);
}

void Entity::RotateY(float angle)
{
    glm::vec3 aux = cross(mDirection, mUp);
    mQuaternion = glm::rotate(mQuaternion,angle,aux);
}

