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

#include <iostream>

#include "Cube.hpp"

Cube::Cube(GLuint programID, float size, glm::vec3 position)
    : Entity(position, glm::vec3(0,0,1), glm::vec3(0,1,0)),
      mAxis(programID,2)
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

/*void printMat(glm::mat4  mat){
    int i,j;
    for (j=0; j<4; j++){
        for (i=0; i<4; i++){
            std::cout << mat[i][j] << " ";
        }
        std::cout << std::endl;
    }
    std::cout << std::endl;
    }*/

void printVec3(std::string name, glm::vec3 v);


void Cube::Draw(glm::mat4 projection, glm::mat4 view)
{
    // Model matrix : an identity matrix (model will be at the origin)
    glm::mat4 Model      = glm::mat4(1.0f);  // Changes for each model !

    glm::mat4 translate = glm::translate(mPosition);
    glm::mat4 orientation =  glm::toMat4(mQuaternion);
    glm::mat4 scale = glm::scale( glm::vec3(mSize,mSize,mSize) );
    Model = translate * orientation * scale * Model;

/*    if(!this->name.compare("cube2")){
        printVec3("direction",direction);
        printVec3("up",up);
        printMat(orientation);
        }*/
//    Model = glm::translate(Model, this->position);
//    Model = glm::rotate( Model, angle, myRotationAxis );
    
    // Our ModelViewProjection : multiplication of our 3 matrices
    glm::mat4 MVP        = projection * view * Model; // Remember, matrix multiplication is the other way around


    // Get a handle for our "MVP" uniform.
    // Only at initialisation time.
    GLuint MatrixID = (GLuint)glGetUniformLocation(mProgramID, "MVP");

    // Send our transformation to the currently bound shader,
    // in the "MVP" uniform
    // For each model you render, since the MVP will be different (at least the M part)
    glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVP[0][0]);


    GLuint WinScaleID = (GLuint)glGetUniformLocation(mProgramID, "WIN_SCALE");

    glUniform2f(WinScaleID, 640, 480);

    
//    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

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
//    glDrawArrays(GL_TRIANGLES, 0, 3); // Starting from vertex 0; 3 vertices total -> 1 triangle
    glDrawArrays(GL_TRIANGLES, 0, 3*12); // Starting from vertex 0; 3 vertices total -> 1 triangle

    glDisableVertexAttribArray(0);
    glDisableVertexAttribArray(1);

//    axis.Draw(projection,view,translate,orientation);
}

void Cube::MoveForward(float distance)
{
    Entity::MoveForward(distance);
}
