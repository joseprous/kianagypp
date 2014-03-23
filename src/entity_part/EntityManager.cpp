#include "EntityManager.hpp"

#include <glm/gtx/quaternion.hpp>

Entity createPlayer(EntityManagerSP em, glm::vec3 position)
{
    Entity player = em->addEntity();
    em->attach(player, {Parts::Position, Parts::Camera});

    em->position[player] = {position, glm::vec3(0,0,1), glm::vec3(0,1,0), glm::angleAxis(0.0f,glm::vec3(0,0,1))};
    em->camera[player] = {glm::vec3(10,10,10)};

    return player;
}

OpenGLMeshPart create_glmesh(const Mesh &mesh)
{
    OpenGLMeshPart glMesh;

    GLuint elem=0;
    for(const poly &p : mesh){
        glm::dvec3 v0 = p.vertexes[0].pos;
        glm::dvec3 v;
        for (auto it = p.vertexes.begin()+2 ; it != p.vertexes.end(); ++it){
            glMesh.vertex_buffer_data.push_back((GLfloat)v0.x);
            glMesh.vertex_buffer_data.push_back((GLfloat)v0.y);
            glMesh.vertex_buffer_data.push_back((GLfloat)v0.z);
            glMesh.element_buffer_data.push_back(elem++);

            v = (it-1)->pos;
            glMesh.vertex_buffer_data.push_back((GLfloat)v.x);
            glMesh.vertex_buffer_data.push_back((GLfloat)v.y);
            glMesh.vertex_buffer_data.push_back((GLfloat)v.z);
            glMesh.element_buffer_data.push_back(elem++);

            v = it->pos;
            glMesh.vertex_buffer_data.push_back((GLfloat)v.x);
            glMesh.vertex_buffer_data.push_back((GLfloat)v.y);
            glMesh.vertex_buffer_data.push_back((GLfloat)v.z);
            glMesh.element_buffer_data.push_back(elem++);

            for(int k=0;k<9;k++){
                glMesh.color_buffer_data.push_back(1);
            }
        }
    }
    GLfloat *vertex_buffer_datap = glMesh.vertex_buffer_data.data();
    GLfloat *color_buffer_datap = glMesh.color_buffer_data.data();
    GLuint *element_buffer_datap = glMesh.element_buffer_data.data();

    glGenBuffers(1, &(glMesh.vertexbuffer));
    glBindBuffer(GL_ARRAY_BUFFER, glMesh.vertexbuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)*glMesh.vertex_buffer_data.size(), vertex_buffer_datap, GL_STATIC_DRAW);

    glGenBuffers(1, &(glMesh.colorbuffer));
    glBindBuffer(GL_ARRAY_BUFFER, glMesh.colorbuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)*glMesh.color_buffer_data.size(), color_buffer_datap, GL_STATIC_DRAW);

    glGenBuffers(1, &(glMesh.elementbuffer));
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, glMesh.elementbuffer);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLfloat)*glMesh.element_buffer_data.size(), element_buffer_datap, GL_STATIC_DRAW);
    return glMesh;
}


CollisionPart create_collision_part(const Mesh &mesh)
{
    CollisionPart collision;
    collision.convexHullShape = std::make_shared<btConvexHullShape>();
    for(const poly &p : mesh){
        for(const Vertex &v : p.vertexes){
            collision.convexHullShape->addPoint(btVector3((GLfloat)v.pos.x,(GLfloat)v.pos.y,(GLfloat)v.pos.z));
        }
    }
    collision.groundMotionState = std::make_shared<btDefaultMotionState>();

    btRigidBody::btRigidBodyConstructionInfo groundRigidBodyCI(0,collision.groundMotionState.get(),collision.convexHullShape.get(),btVector3(0,0,0));

    collision.groundRigidBody = std::make_shared<btRigidBody>(groundRigidBodyCI);
    return collision;
}

Entity createBrush(EntityManagerSP em, const brush &b)
{
    Entity brush = em->addEntity();
    em->attach(brush, {Parts::Mesh, Parts::GLShaders, Parts::GLMesh, Parts::Collision});

    em->mesh[brush] = {b.getMesh()};
    em->glShaders[brush] = {"shaders/simple.vert", "shaders/simple.frag", "shaders/wireframe.geom", "wireframe" };

    em->glMesh[brush] = create_glmesh(b.getMesh());
    em->collision[brush] = create_collision_part(b.getMesh());

    return brush;
}

Entity EntityManager::addEntity()
{
    for(Entity i=0;i<partMask.size();i++)
    {
        if(partMask[i].none()){
            return i;
        }
    }
    partMask.emplace_back();
    position.emplace_back();
    mesh.emplace_back();
    glMesh.emplace_back();
    collision.emplace_back();
    glShaders.emplace_back();
    camera.emplace_back();

    size = partMask.size();
    
    return size - 1;
}

void EntityManager::removeEntity(Entity entity)
{
    partMask[entity].reset();
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
