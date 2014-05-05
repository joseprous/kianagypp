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

#include "System.hpp"
#include <chrono>

System::~System()
{
    
}

void System::update(uint32_t ticks)
{
    if(ticks <= mLast_ticks + mPeriod) return;
  

    auto t1 = std::chrono::high_resolution_clock::now();
    update();
    auto t2 = std::chrono::high_resolution_clock::now();
    auto time_span = std::chrono::duration_cast<std::chrono::microseconds>(t2 - t1);

    (*(AccM->get(mSystemName)))(time_span.count());

    mLast_ticks = ticks;
}
