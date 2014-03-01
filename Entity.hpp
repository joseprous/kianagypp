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

#ifndef ENTITY_HPP
#define ENTITY_HPP

#include <string>
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>

class Entity
{
protected:
    
    glm::vec3 position;
    glm::vec3 direction;
    glm::vec3 up;
    glm::quat quaternion;
    
public:

    std::string name;

    Entity(glm::vec3 position, glm::vec3 direction, glm::vec3 up);

    virtual void MoveForward(float distance);
    virtual void MoveBackward(float distance);
    virtual void MoveLeft(float distance);
    virtual void MoveRight(float distance);
    virtual void MoveUp(float distance);
    virtual void MoveDown(float distance);
    virtual void RotateX(float angle);
    virtual void RotateY(float angle);
    virtual void Move(glm::vec3 dir, float distance);

};
#endif
