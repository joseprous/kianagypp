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

#ifndef MAP_HPP
#define MAP_HPP

#include <vector>
#include <string>
#include <glm/glm.hpp>
#include <GL/glew.h>
#include "Physics.hpp"
#include "Brush.hpp"

struct header
{
    std::string variable;
    std::string value;
};

class entity
{
private:
    
public:
    
    entity();
    entity(std::vector<header> headers, std::vector<rawbrush> rawbrushes);

    std::vector<header> mHeaders;
    std::vector<rawbrush> mRawbrushes;
    std::vector<brush> mBrushes;

    void load_brushes(dynamicsWorldSP dynamicsWorld, float scale);
};

class Map
{
public:
    
    void draw(GLuint programID, glm::mat4 projection, glm::mat4 view);
    
    std::vector<entity> entities;    
};

Map load_map(std::string map_file);

#endif
