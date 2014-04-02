#include "CollisionPart.hpp"

CollisionPart create_collision_part(const Mesh &mesh)
{
    CollisionPart collision;
    collision.convexHullShape = std::make_shared<btConvexHullShape>();
    for(const poly &p : mesh){
        for(const Vertex &v : p.vertexes){
            collision.convexHullShape->addPoint(btVector3((float)v.pos.x,(float)v.pos.y,(float)v.pos.z));
        }
    }
    collision.groundMotionState = std::make_shared<btDefaultMotionState>();

    btRigidBody::btRigidBodyConstructionInfo groundRigidBodyCI(0,collision.groundMotionState.get(),collision.convexHullShape.get(),btVector3(0,0,0));

    collision.groundRigidBody = std::make_shared<btRigidBody>(groundRigidBodyCI);
    return collision;
}
