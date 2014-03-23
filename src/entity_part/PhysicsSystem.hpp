#ifndef PHYSICSSYSTEM_HPP
#define PHYSICSSYSTEM_HPP

#include "EntityManager.hpp"

class PhysicsSystem
{
private:
    
    std::shared_ptr<btDiscreteDynamicsWorld> dynamicsWorld;

    EntityManagerSP EM;
    uint32_t mLast_ticks;
    uint32_t mPeriod;
public:

    PhysicsSystem(EntityManagerSP em,uint32_t period);
    void update(uint32_t ticks);
};

#endif
