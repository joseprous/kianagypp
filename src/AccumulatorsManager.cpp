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

#include "AccumulatorsManager.hpp"
#include <iomanip>

AccSP AccumulatorsManager::add(std::string name)
{
    auto acc = std::make_shared<Acc>();
    accumulators[name] = acc;
    return acc;
}

/*AccSP AccumulatorsManager::get(std::string name)
{
    return accumulators[name];
    }*/
void AccumulatorsManager::update(std::string name, long value)
{
    if(firsts.count(name) == 0){
        firsts[name] = value;
    }else{
        (*(accumulators[name]))(value);
    }
}

void AccumulatorsManager::print_statistics()
{
    std::cout << "statistics in microseconds:" << std::endl;
    for(auto a : accumulators){
        std::cout << a.first << ":" << std::endl; 
        std::cout << "count:  " << std::setw(8) << boost::accumulators::count(*(a.second)) << std::endl;
        std::cout << "first:  " << std::setw(8) << firsts[a.first] << std::endl;
        std::cout << "mean:   " << std::setw(8) << (unsigned)boost::accumulators::mean(*(a.second)) << std::endl;
        std::cout << "median: " << std::setw(8) << (unsigned)boost::accumulators::median(*(a.second)) << std::endl;
        std::cout << "max:    " << std::setw(8) << boost::accumulators::max(*(a.second)) << std::endl;
        std::cout << "min:    " << std::setw(8) << boost::accumulators::min(*(a.second)) << std::endl;
        std::cout << std::endl; 
    }
}
