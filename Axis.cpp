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

#include "Axis.hpp"

Axis::Axis(GLuint programID, float size)
    : Entity(glm::vec3(0,0,0), glm::vec3(0,0,1), glm::vec3(0,1,0))
{
    mSize = size;
    mProgramID = programID;
    
    glGenBuffers(1, &mVertexbuffer);
    glBindBuffer(GL_ARRAY_BUFFER, mVertexbuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_data), g_vertex_buffer_data, GL_STATIC_DRAW);
    
    glGenBuffers(1, &mColorbuffer);
    glBindBuffer(GL_ARRAY_BUFFER, mColorbuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(g_color_buffer_data), g_color_buffer_data, GL_STATIC_DRAW);

}

void Axis::Draw()
{
    // 1rst attribute buffer : vertices
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, mVertexbuffer);
    glVertexAttribPointer(
        0,                  // attribute 0. No particular reason for 0, but must match the layout in the shader.
        3,                  // size
        GL_FLOAT,           // type
        GL_FALSE,           // normalized?
        0,                  // stride
        (void*)0            // array buffer offset
        );
        
    glEnableVertexAttribArray(1);
    glBindBuffer(GL_ARRAY_BUFFER, mColorbuffer);
    glVertexAttribPointer(
        1,                                // attribute. No particular reason for 1, but must match the layout in the shader.
        3,                                // size
        GL_FLOAT,                         // type
        GL_FALSE,                         // normalized?
        0,                                // stride
        (void*)0                          // array buffer offset
        );

    // Draw the triangle !
    glDrawArrays(GL_LINES, 0, 2*3); // Starting from vertex 0; 3 vertices total -> 1 triangle

    glDisableVertexAttribArray(0);
    glDisableVertexAttribArray(1);    
}

void Axis::Draw(glm::mat4 projection, glm::mat4 view, glm::mat4 translate, glm::mat4 orientation)
{
    // Model matrix : an identity matrix (model will be at the origin)
    glm::mat4 Model      = glm::mat4(1.0f);  // Changes for each model !

    //   glm::mat4 translate = glm::translate(this->position);
    //glm::mat4 orientation =  glm::toMat4(quaternion);
    glm::mat4 scale = glm::scale( glm::vec3(mSize,mSize,mSize) );
    Model = translate  * orientation * scale * Model;
    
    // Our ModelViewProjection : multiplication of our 3 matrices
    glm::mat4 MVP        = projection * view * Model; // Remember, matrix multiplication is the other way around


    // Get a handle for our "MVP" uniform.
    // Only at initialisation time.
    GLuint MatrixID = (GLuint)glGetUniformLocation(mProgramID, "MVP");

    // Send our transformation to the currently bound shader,
    // in the "MVP" uniform
    // For each model you render, since the MVP will be different (at least the M part)
    glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVP[0][0]);

    Draw();
}

void Axis::MoveForward(float distance)
{
    Entity::MoveForward(distance);
}
