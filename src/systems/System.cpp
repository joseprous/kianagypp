#include "System.hpp"

System::~System()
{
    
}

void System::update(uint32_t ticks)
{
    if(ticks <= mLast_ticks + mPeriod) return;

    update();
    
    mLast_ticks = ticks;
}
