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

#ifndef PLAYER_HPP
#define PLAYER_HPP

#include "Entity.hpp"
#include "Cube.hpp"

class Player : public Cube
{
public:

    Player(dynamicsWorldSP dynamicsWorld, GLuint programID, float size, glm::vec3 position);

    std::shared_ptr<btBoxShape> fallShape;
    std::shared_ptr<btDefaultMotionState> fallMotionState;
    std::shared_ptr<btRigidBody> fallRigidBody;

};


#endif