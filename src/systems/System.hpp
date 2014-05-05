/*
kianagy++
Copyright (C) 2014 José Prous

This file is part of kianagy++.

kianagy++ is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

kianagy++ is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with kianagy++.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef SYSTEM_HPP
#define SYSTEM_HPP

#include "EntityManager.hpp"
#include "SignalsManager.hpp"
#include "AccumulatorsManager.hpp"

class System
{
protected:

    std::string mSystemName;
    EntityManagerSP EM;
    SignalsManagerSP SigM;
    AccumulatorsManagerSP AccM;

    uint32_t mLast_ticks;
    uint32_t mPeriod;
    
public:

    System(std::string systemName, EntityManagerSP em, SignalsManagerSP sigm, AccumulatorsManagerSP accm, uint32_t period)
        :mSystemName(systemName),
         EM(em),
         SigM(sigm),
         AccM(accm),
         mLast_ticks(0),
         mPeriod(period)
        {
            AccM->add(mSystemName);
        }

    virtual ~System();
    
    virtual void init() = 0;
    
    virtual void update() = 0;
    virtual void update(uint32_t ticks);
};

#endif
