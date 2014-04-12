#include "EntityManager.hpp"

#include <glm/gtx/quaternion.hpp>
#include <glm/gtc/constants.hpp>
#include "map/Brush.hpp"

poly createPoly(std::vector<glm::dvec3> points)
{
//    Log("start createPoly");
    poly p;
    for(glm::dvec3 point : points ){
        p.vertexes.push_back({point});
    }

    /*  for(auto v : p.vertexes){
        Log(v.pos.x);
        Log(v.pos.y);
        Log(v.pos.z);
        Log("");
        }*/

    //Log("end createPoly");
    
    return p;
}

Mesh getCubeMesh()
{
    Mesh mesh;


    mesh.push_back(createPoly({glm::dvec3(-1,1,-1),
                    glm::dvec3(-1,-1,-1),
                    glm::dvec3(1,-1,-1),
                    glm::dvec3(1,1,-1)}));
    mesh.push_back(createPoly({glm::dvec3(-1,1,1),
                    glm::dvec3(-1,-1,1),
                    glm::dvec3(1,-1,1),
                    glm::dvec3(1,1,1)}));
    
    mesh.push_back(createPoly({glm::dvec3(-1,1,-1),
                    glm::dvec3(-1,-1,-1),
                    glm::dvec3(-1,-1,1),
                    glm::dvec3(-1,1,1)}));

    mesh.push_back(createPoly({glm::dvec3(1,1,-1),
                    glm::dvec3(1,-1,-1),
                    glm::dvec3(1,-1,1),
                    glm::dvec3(1,1,1)}));

    
    mesh.push_back(createPoly({glm::dvec3(-1,1,1),
                    glm::dvec3(-1,1,-1),
                    glm::dvec3(1,1,-1),
                    glm::dvec3(1,1,1)}));
    mesh.push_back(createPoly({glm::dvec3(-1,-1,1),
                    glm::dvec3(-1,-1,-1),
                    glm::dvec3(1,-1,-1),
                    glm::dvec3(1,-1,1)}));

    return mesh;
}

Entity createPlayer(EntityManagerSP em, glm::vec3 position)
{
    Entity player = em->addEntity();
    em->attach(player, {Parts::Position, Parts::Camera, Parts::Movement, Parts::Signals, Parts::Mesh, /*Parts::GLShaders, Parts::GLMesh*//*, Parts::Collision*/});

    em->position[player] = {position,
                            glm::vec3(0,1,0),
                            glm::vec3(0,0,1),
                            glm::angleAxis(0.0f,glm::vec3(0,0,1))};

    em->camera[player] = {glm::vec3(3,1,0)};
    
    em->movement[player] = {0.1f,0.1f,0.1f,0.1f,0.1f,0.1f,
                            0,0,0,0,0,0};

    em->glShaders[player] = {"shaders/simple.vert", "shaders/simple.frag", "shaders/wireframe.geom", "wireframe" };

    em->mesh[player] = { getCubeMesh() };
    em->glMesh[player] = create_glmesh(em->mesh[player].mesh);

//    em->collision[player] = create_collision_part(em->mesh[player].mesh, false, true);

    
    em->signals[player].signals.insert(Signals::MoveLeft);    
    em->signals[player].signals.insert(Signals::MoveRight);    
    em->signals[player].signals.insert(Signals::MoveForward);    
    em->signals[player].signals.insert(Signals::MoveBackward);    
    em->signals[player].signals.insert(Signals::MoveUp);    
    em->signals[player].signals.insert(Signals::MoveDown);
    em->signals[player].signals.insert(Signals::RotateX);    

    return player;
}

Entity createCube(EntityManagerSP em, glm::vec3 position)
{
    Entity cube = em->addEntity();
    em->attach(cube, {Parts::Position, Parts::Mesh, Parts::GLShaders, Parts::GLMesh, Parts::Collision});

    em->position[cube] = {position,
                          glm::vec3(0,1,0),
                          glm::vec3(0,0,1),
                          glm::angleAxis(0.0f,glm::vec3(0,0,1))};
    
    em->glShaders[cube] = {"shaders/simple.vert", "shaders/simple.frag", "shaders/wireframe.geom", "wireframe" };

    em->mesh[cube] = { getCubeMesh() };
    em->glMesh[cube] = create_glmesh(em->mesh[cube].mesh);

    //em->collision[cube] = create_collision_part(em->mesh[cube].mesh, true, true);

    CollisionPart collision;
    /*collision.convexHullShape = std::make_shared<btConvexHullShape>();
    for(const poly &p : em->mesh[cube].mesh){
        for(const Vertex &v : p.vertexes){
            collision.convexHullShape->addPoint(btVector3((float)v.pos.x,(float)v.pos.y,(float)v.pos.z));
        }
        }*/

    collision.boxShape = std::make_shared<btBoxShape>(btVector3(0.5f,0.5f,0.5f));
    
    auto pos = em->position[cube].position;
    auto quat = em->position[cube].quaternion;
    auto bpos = btVector3(pos.x,pos.y,pos.z);
    auto bquat = btQuaternion(quat.x, quat.y, quat.z, quat.w);
    collision.motionState = std::make_shared<btDefaultMotionState>(btTransform(bquat,bpos));

    btScalar mass = 1;
    btVector3 fallInertia(0,0,0);
    collision.boxShape->calculateLocalInertia(mass,fallInertia);
    btRigidBody::btRigidBodyConstructionInfo rigidBodyCI(mass,collision.motionState.get(),collision.boxShape.get(),fallInertia);
    
    collision.rigidBody = std::make_shared<btRigidBody>(rigidBodyCI);
    collision.changeUp = true;
    collision.move = true;

    em->collision[cube] = collision;
    
    return cube;
}

