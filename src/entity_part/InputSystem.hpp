#ifndef INPUTSYSTEM_HPP
#define INPUTSYSTEM_HPP

#include "EntityManager.hpp"

class InputSystem
{
private:
    
    EntityManagerSP EM;
    uint32_t mLast_ticks;
    uint32_t mPeriod;
    
public:

    InputSystem(EntityManagerSP em,uint32_t period);
    void update(uint32_t ticks);
};

#endif
