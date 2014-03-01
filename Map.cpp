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

entity::entity(std::vector<header> headers, std::vector<rawbrush> rawbrushes)
{
    this->headers = headers;
    this->rawbrushes = rawbrushes;
}

void entity::load_brushes(dynamicsWorldSP dynamicsWorld)
{
    for(rawbrush b : this->rawbrushes)
    {
        brush aux;
        aux.load(dynamicsWorld,b);
        this->brushes.push_back(aux);        
    }
}
void Map::draw(GLuint programID, glm::mat4 projection, glm::mat4 view)
{
    glm::mat4 Model      = glm::mat4(1.0f);  // Changes for each model !

    glm::mat4 MVP        = projection * view * Model; // Remember, matrix multiplication is the other way around


    GLuint MatrixID = glGetUniformLocation(programID, "MVP");

    glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVP[0][0]);


    GLuint WinScaleID = glGetUniformLocation(programID, "WIN_SCALE");

    glUniform2f(WinScaleID, 640, 480);

    for(brush b : entities[0].brushes){
        b.draw(programID, projection, view);
    }    
}

Map load_map(std::string map_file)
{
    Map_Driver driver;
    driver.parse (map_file.c_str());                

    return driver.map;
}
