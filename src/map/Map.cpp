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

#include "Map.hpp"
#include "Map_Driver.hpp"

#include <iostream>
#include <string>

entity::entity(){}

entity::entity(const std::vector<header> &headers, const std::vector<rawbrush> &rawbrushes)
{
    mHeaders = headers;
    mRawbrushes = rawbrushes;
}

void entity::load_brushes(float scale)
{
    for(rawbrush b : mRawbrushes)
    {
        brush aux;
        aux.load(b,scale);
        mBrushes.push_back(aux);        
    }
}

Map load_map(const std::string &map_file)
{
    Map_Driver driver;
    driver.parse (map_file.c_str());                

    return driver.map;
}
