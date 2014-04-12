#include "CollisionPart.hpp"

CollisionPart create_collision_part(const Mesh &mesh, bool changeUp, bool move)
{
    CollisionPart collision;
    collision.convexHullShape = std::make_shared<btConvexHullShape>();
    for(const poly &p : mesh){
        for(const Vertex &v : p.vertexes){
            collision.convexHullShape->addPoint(btVector3((float)v.pos.x,(float)v.pos.y,(float)v.pos.z));
        }
    }
    
    collision.motionState = std::make_shared<btDefaultMotionState>();

    btRigidBody::btRigidBodyConstructionInfo rigidBodyCI(0,collision.motionState.get(),collision.convexHullShape.get(),btVector3(0,0,0));

    collision.rigidBody = std::make_shared<btRigidBody>(rigidBodyCI);
    collision.changeUp = changeUp;
    collision.move = move;
    return collision;
}
