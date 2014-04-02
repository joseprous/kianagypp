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

#include "EntityManager.hpp"

Entity EntityManager::addEntity()
{
    Entity entity;
    if(available_entities.empty()){
        if(entities.empty()){
            entity = 0;
        }else{
            entity = *entities.rbegin() + 1;
        }
    }else{
        auto it = available_entities.begin();
        entity = *it;
        available_entities.erase(it);
    }
    entities.insert(entity);
    
    return entity;
}

void EntityManager::removeEntity(Entity entity)
{
    available_entities.insert(entity);
    entities.erase(entity);
}

void EntityManager::attach(Entity entity, std::initializer_list<Parts> parts)
{
    for(Parts part : parts){
        partMask[entity].set(static_cast<size_t>(part));
    }
}

void EntityManager::detach(Entity entity, std::initializer_list<Parts> parts)
{
    for(Parts part : parts){
        partMask[entity].reset(static_cast<size_t>(part));
    }
}

bool EntityManager::has(Entity entity, Parts part)
{
    return partMask[entity][static_cast<size_t>(part)];
}

boost::optional<PositionPart &> EntityManager::getPosition(Entity entity)
{
    if(has(entity,Parts::Position)){
        return boost::optional<PositionPart &>(position[entity]);
    }else{
        return boost::optional<PositionPart &>();
    }
}

boost::optional<OpenGLMeshPart &> EntityManager::getGLMesh(Entity entity)
{
    if(has(entity,Parts::GLMesh)){
        return boost::optional<OpenGLMeshPart &>(glMesh[entity]);
    }else{
        return boost::optional<OpenGLMeshPart &>();
    }
}

boost::optional<CollisionPart &> EntityManager::getCollision(Entity entity)
{
    if(has(entity,Parts::Collision)){
        return boost::optional<CollisionPart &>(collision[entity]);
    }else{
        return boost::optional<CollisionPart &>();
    }
}

boost::optional<OpenGLShadersPart &> EntityManager::getGLShaders(Entity entity)
{
    if(has(entity,Parts::GLShaders)){
        return boost::optional<OpenGLShadersPart &>(glShaders[entity]);
    }else{
        return boost::optional<OpenGLShadersPart &>();
    }
}

boost::optional<CameraPart &> EntityManager::getCamera(Entity entity)
{
    if(has(entity,Parts::Camera)){
        return boost::optional<CameraPart &>(camera[entity]);
    }else{
        return boost::optional<CameraPart &>();
    }
}

boost::optional<SignalsPart &> EntityManager::getSignals(Entity entity)
{
    if(has(entity,Parts::Signals)){
        return boost::optional<SignalsPart &>(signals[entity]);
    }else{
        return boost::optional<SignalsPart &>();
    }
}

boost::optional<MovementPart &> EntityManager::getMovement(Entity entity)
{
    if(has(entity,Parts::Movement)){
        return boost::optional<MovementPart &>(movement[entity]);
    }else{
        return boost::optional<MovementPart &>();
    }
}
