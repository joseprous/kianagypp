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

typedef size_t Entity;
typedef int32_t Part_t;

enum class Parts : Part_t
{
    Position,
    Mesh,
    GLMesh,
    Collision,
    GLShaders,
    Camera
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
