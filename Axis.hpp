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

#ifndef AXIS_HPP
#define AXIS_HPP

#include "Entity.hpp"
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/rotate_vector.hpp>
#include <glm/gtx/transform.hpp>

class Axis : public Entity
{
private:
    
    GLfloat g_vertex_buffer_data[3*3*2] = {
        0.0f,0.0f,0.0f,
        1.0f,0.0f,0.0f,
        0.0f,0.0f,0.0f,
        0.0f,1.0f,0.0f,
        0.0f,0.0f,0.0f,
        0.0f,0.0f,1.0f
    };
    
    GLfloat g_color_buffer_data[3*3*2] = {
        1.0f,0.0f,0.0f,
        1.0f,0.0f,0.0f,
        0.0f,1.0f,0.0f,
        0.0f,1.0f,0.0f,
        0.0f,0.0f,1.0f,
        0.0f,0.0f,1.0f
    };
    
public:

    GLuint vertexbuffer;
    GLuint colorbuffer;
    float size;
    GLuint programID;

    Axis(GLuint programID, float size);
    void Draw(glm::mat4 projection, glm::mat4 view, glm::mat4 translate, glm::mat4 orientation);
    void Draw();
};

#endif
