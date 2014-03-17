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

void entity::load_brushes(dynamicsWorldSP dynamicsWorld, float scale)
{
    for(rawbrush b : mRawbrushes)
    {
        brush aux;
        aux.load(dynamicsWorld,b,scale);
        mBrushes.push_back(aux);        
    }
}
void Map::draw(GLuint programID, const glm::mat4 &projection, const glm::mat4 &view)
{
    glm::mat4 Model      = glm::mat4(1.0f);  // Changes for each model !

    glm::mat4 MVP        = projection * view * Model; // Remember, matrix multiplication is the other way around


    GLuint MatrixID = (GLuint)glGetUniformLocation(programID, "MVP");

    glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVP[0][0]);


    GLuint WinScaleID = (GLuint)glGetUniformLocation(programID, "WIN_SCALE");

    glUniform2f(WinScaleID, 640, 480);

    for(brush b : entities[0].mBrushes){
        b.draw();
    }    
}

Map load_map(const std::string &map_file)
{
    Map_Driver driver;
    driver.parse (map_file.c_str());                

    return driver.map;
}
