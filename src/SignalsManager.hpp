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
#include <map>
#include <boost/optional.hpp>

class SignalsManager 
{     
public:

    void send(Signals signal);

    
    bool receive(Signals signal);

    void sendf(Signals signal, float value);
    void sendb(Signals signal, bool value);

    boost::optional<float> receivef(Signals signal);
    boost::optional<bool> receiveb(Signals signal);
    
private:
    std::set<Signals> signals;
    std::map<Signals,float> valuesf;
    std::map<Signals,bool> valuesb;
};

typedef std::shared_ptr<SignalsManager> SignalsManagerSP;


#endif // SIGNALSMANAGER_HPP
