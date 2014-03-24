#ifndef PHYSICSSYSTEM_HPP
#define PHYSICSSYSTEM_HPP

#include "System.hpp"
#include "EntityManager.hpp"

class PhysicsSystem : public System
{
private:
    
    std::shared_ptr<btDiscreteDynamicsWorld> dynamicsWorld;

public:

    PhysicsSystem(EntityManagerSP em,uint32_t period)
        :System(em,period){}

    void init() override;
    void update() override;
    using System::update;
};

#endif
