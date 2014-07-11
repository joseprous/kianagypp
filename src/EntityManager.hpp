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
#include <unordered_map>
#include <boost/optional.hpp>

#include "Logger.hpp"

#include "parts/PositionPart.hpp"
#include "parts/MeshPart.hpp"
#include "parts/OpenGLMeshPart.hpp"
#include "parts/CollisionPart.hpp"
#include "parts/OpenGLShadersPart.hpp"
#include "parts/CameraPart.hpp"
#include "parts/SignalsPart.hpp"
#include "parts/MovementPart.hpp"
#include "parts/IQMPart.hpp"

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
    Movement,
    IQM
};

template <typename T>
class PartContainer
{
private:

    std::unordered_map<Entity,T> data;

public:

    T & operator [](Entity entity)
        {
            return data[entity];
        }
};

class EntityManager
{
private:

    std::unordered_map<Entity,std::bitset<32>> partMask; 

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
    PartContainer<IQMPart> iqm;

    std::set<Entity> entities;
   
    Entity addEntity();
    void removeEntity(Entity entity);
    void attach(Entity entity, std::initializer_list<Parts> parts);
    void detach(Entity entity, std::initializer_list<Parts> parts);

    bool has(Entity entity, Parts part);

    boost::optional<PositionPart &> getPosition(Entity entity);
    boost::optional<OpenGLMeshPart &> getGLMesh(Entity entity);
    boost::optional<CollisionPart &> getCollision(Entity entity);
    boost::optional<OpenGLShadersPart &> getGLShaders(Entity entity);
    boost::optional<CameraPart &> getCamera(Entity entity);
    boost::optional<SignalsPart &> getSignals(Entity entity);
    boost::optional<MovementPart &> getMovement(Entity entity);
};

typedef std::shared_ptr<EntityManager> EntityManagerSP;

//Entity createPlayer(EntityManagerSP em, glm::vec3 position);
//Entity createBrush(EntityManagerSP em, const brush &b);

#endif
