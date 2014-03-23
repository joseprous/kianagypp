#include "PhysicsSystem.hpp"

PhysicsSystem::PhysicsSystem(EntityManagerSP em,uint32_t period)
    :EM(em),
     mLast_ticks(0),
     mPeriod(period)
{
    auto broadphase = std::make_shared<btDbvtBroadphase>();
        
    auto collisionConfiguration = std::make_shared<btDefaultCollisionConfiguration>();
    auto dispatcher = std::make_shared<btCollisionDispatcher>(collisionConfiguration.get());

    auto solver = std::make_shared<btSequentialImpulseConstraintSolver>();

    dynamicsWorld = std::make_shared<btDiscreteDynamicsWorld>(dispatcher.get(),broadphase.get(),solver.get(),collisionConfiguration.get());

    dynamicsWorld->setGravity(btVector3(0,0,-10));

    Log("PhysicsSystem::PhysicsSystem");
}

void PhysicsSystem::update(uint32_t ticks)
{
    if(ticks <= mLast_ticks + mPeriod) return;

    mLast_ticks = ticks;
}
