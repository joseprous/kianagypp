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

#ifndef COLLISIONPART_HPP
#define COLLISIONPART_HPP

#include <memory>
#include <bullet/btBulletDynamicsCommon.h>

#include "map/Brush.hpp"

struct CollisionPart
{
    std::shared_ptr<btConvexHullShape> convexHullShape;
    std::shared_ptr<btDefaultMotionState> groundMotionState;
    std::shared_ptr<btRigidBody> groundRigidBody;
};

CollisionPart create_collision_part(const Mesh &mesh);

#endif
