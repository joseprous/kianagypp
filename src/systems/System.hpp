#ifndef SYSTEM_HPP
#define SYSTEM_HPP

#include "EntityManager.hpp"

class System
{
protected:
    EntityManagerSP EM;
    uint32_t mLast_ticks;
    uint32_t mPeriod;
    
public:

    System(EntityManagerSP em,uint32_t period)
        :EM(em),
         mLast_ticks(0),
         mPeriod(period)
    {
    }

    virtual ~System();
    
    virtual void init() = 0;
    
    virtual void update() = 0;
    virtual void update(uint32_t ticks);
};

#endif
