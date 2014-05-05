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

AccSP AccumulatorsManager::add(std::string name)
{
    auto acc = std::make_shared<Acc>();
    accumulators[name] = acc;
    return acc;
}

AccSP AccumulatorsManager::get(std::string name)
{
    return accumulators[name];
}
void AccumulatorsManager::print_statistics()
{
    for(auto a : accumulators){
        std::cout << a.first << ":" << std::endl; 
        std::cout << "count:" << boost::accumulators::count(*(a.second)) << std::endl;
        std::cout << "mean:" << boost::accumulators::mean(*(a.second)) << "us" << std::endl;
        std::cout << "median:" << boost::accumulators::median(*(a.second)) << "us" << std::endl;
        std::cout << "max:" << boost::accumulators::max(*(a.second)) << "us" << std::endl;
        std::cout << "min:" << boost::accumulators::min(*(a.second)) << "us" << std::endl;
        std::cout << std::endl; 
    }
}
