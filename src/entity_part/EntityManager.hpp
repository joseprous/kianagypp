#ifndef ENTITYMANAGER_HPP
#define ENTITYMANAGER_HPP

#include <cstdint>
#include <bitset>
#include <type_traits>
#include <initializer_list>

#include "PositionPart.hpp"
#include "MeshPart.hpp"
#include "OpenGLMeshPart.hpp"
#include "CollisionPart.hpp"
#include "OpenGLShadersPart.hpp"
#include "CameraPart.hpp"

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

class EntityManager
{
private:
    std::vector<std::bitset<32>> partMask; 
    
public:
    std::vector<PositionPart> position;
    std::vector<MeshPart> mesh;
    std::vector<OpenGLMeshPart> glMesh;
    std::vector<CollisionPart> collision;
    std::vector<OpenGLShadersPart> glShaders;
    std::vector<CameraPart> camera;

    size_t size;
    EntityManager(){size = 0;}
    Entity addEntity();
    void removeEntity(Entity entity);
    void attach(Entity entity, std::initializer_list<Parts> parts);
    void detach(Entity entity, std::initializer_list<Parts> parts);
    bool has(Entity entity, Parts part);
};

Entity createPlayer(EntityManager &em, glm::vec3 position);
Entity createBrush(EntityManager &em, const brush &b);


#endif
