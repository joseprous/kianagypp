#ifndef COLLISIONPART_HPP
#define COLLISIONPART_HPP

struct CollisionPart
{
    std::shared_ptr<btConvexHullShape> convexHullShape;
    std::shared_ptr<btDefaultMotionState> groundMotionState;
    std::shared_ptr<btRigidBody> groundRigidBody;
};

#endif
