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

#ifndef SIGNALSMANAGER_HPP
#define SIGNALSMANAGER_HPP

#include <memory>
#include <set>
#include "Signals.hpp"


class SignalsManager 
{     
public:

    void send(Signals signal);

    template<typename T>
    void send(Signals signal, T value);
    
    bool receive(Signals signal);

    template<typename T>
    bool receive(Signals signal, T &value);

private:
    std::set<Signals> signals;
};

typedef std::shared_ptr<SignalsManager> SignalsManagerSP;


template<typename T>
void SignalsManager::send(Signals signal, T value)
{
    
}

template<typename T>
bool SignalsManager::receive(Signals signal, T &value)
{
    return false;
}


#endif // SIGNALSMANAGER_HPP
