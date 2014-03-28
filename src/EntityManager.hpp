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

#ifndef ENTITYMANAGER_HPP
#define ENTITYMANAGER_HPP

#include <cstdint>
#include <bitset>
#include <type_traits>
#include <initializer_list>
#include <map>
#include <set>

#include "Logger.hpp"

#include "parts/PositionPart.hpp"
#include "parts/MeshPart.hpp"
#include "parts/OpenGLMeshPart.hpp"
#include "parts/CollisionPart.hpp"
#include "parts/OpenGLShadersPart.hpp"
#include "parts/CameraPart.hpp"
#include "parts/SignalsPart.hpp"
#include "parts/MovementPart.hpp"

typedef size_t Entity;
typedef int32_t Part_t;

enum class Parts : Part_t
{
    Position,
    Mesh,
    GLMesh,
    Collision,
    GLShaders,
    Camera,
    Signals,
    Movement
};

template <typename T>
class PartContainer
{
private:

    std::map<Entity,T> data;

public:

    T & operator [](Entity entity)
        {
            return data[entity];
        }
};

class EntityManager
{
private:

    std::map<Entity,std::bitset<32>> partMask; 

    std::set<Entity> available_entities;

public:
    PartContainer<PositionPart> position;
    PartContainer<MeshPart> mesh;
    PartContainer<OpenGLMeshPart> glMesh;
    PartContainer<CollisionPart> collision;
    PartContainer<OpenGLShadersPart> glShaders;
    PartContainer<CameraPart> camera;
    PartContainer<SignalsPart> signals;
    PartContainer<MovementPart> movement;

    std::set<Entity> entities;
   
    Entity addEntity();
    void removeEntity(Entity entity);
    void attach(Entity entity, std::initializer_list<Parts> parts);
    void detach(Entity entity, std::initializer_list<Parts> parts);
    bool has(Entity entity, Parts part);
};

typedef std::shared_ptr<EntityManager> EntityManagerSP;

Entity createPlayer(EntityManagerSP em, glm::vec3 position);
Entity createBrush(EntityManagerSP em, const brush &b);


#endif
