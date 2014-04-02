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

#include "RendererSystem.hpp"

#include <iostream>
#include <fstream>
#include <algorithm>
#include <string>

void RenderSystem::init()
{
    glewExperimental = GL_TRUE; 
    glewInit();

    // Enable depth test
    glEnable(GL_DEPTH_TEST);
    // Accept fragment if it closer to the camera than the former one
    glDepthFunc(GL_LESS);

    GLuint VertexArrayID;
    glGenVertexArrays(1, &VertexArrayID);
    glBindVertexArray(VertexArrayID);
    
    Log("RenderSystem::RenderSystem");
}

void draw(OpenGLMeshPart *part)
{
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, part->vertexbuffer);
    glVertexAttribPointer(
        0,                  // attribute 0. No particular reason for 0, but must match the layout in the shader.
        3,                  // size
        GL_FLOAT,           // type
        GL_FALSE,           // normalized?
        0,                  // stride
        (void*)0            // array buffer offset
        );


    glEnableVertexAttribArray(1);
    glBindBuffer(GL_ARRAY_BUFFER, part->colorbuffer);
    glVertexAttribPointer(
        1,                                // attribute. No particular reason for 1, but must match the layout in the shader.
        3,                                // size
        GL_FLOAT,                         // type
        GL_FALSE,                         // normalized?
        0,                                // stride
        (void*)0                          // array buffer offset
        );
    // Index buffer
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, part->elementbuffer);

    // Draw the triangles !
    glDrawElements(
        GL_TRIANGLES,      // mode
        (GLsizei)part->element_buffer_data.size(),    // count
        GL_UNSIGNED_INT,   // type
        (void*)0           // element array buffer offset
        );

    glDisableVertexAttribArray(0);
    glDisableVertexAttribArray(1);
}

bool RenderSystem::getCamera(glm::mat4 &view)
{
    for(Entity entity : EM->entities){
        if(auto camera = EM->getCamera(entity)){
            if(auto pos = EM->getPosition(entity)){
                glm::vec3 camera_pos = pos->position + glm::vec3(0,-5,2);
                glm::vec3 mDirection = glm::normalize(pos->position - camera_pos);
                view = glm::lookAt( camera_pos, camera_pos + mDirection, glm::vec3(0,0,1) );
                return true;
            }
        }
    }
    return false;
}


void RenderSystem::update()
{
    glm::mat4 view;
    if(!getCamera(view)) return;

    glClearColor(0.0f,0.0f,0.5f,1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    
    glm::mat4 projection = glm::perspective(45.0f, 4.0f / 3.0f, 0.1f, 10000.0f);
    for(Entity entity : EM->entities){
        if(auto shaders = EM->getGLShaders(entity)){
            if(auto mesh = EM->getGLMesh(entity)){
                GLuint programId = SM.GetShaderProgram(*shaders);
                glUseProgram(programId);

                glm::mat4 Model      = glm::mat4(1.0f);

                if(auto position = EM->getPosition(entity)){
                    glm::mat4 translate = glm::translate(position->position);
                    //glm::mat4 orientation =  glm::toMat4(mQuaternion);
                    glm::mat4 scale = glm::scale( glm::vec3(0.5,0.5,0.5) );
                    Model = translate/* * orientation*/ * scale  * Model;
                }
                
                
                glm::mat4 MVP        = projection * view * Model;

                GLuint MatrixID = (GLuint)glGetUniformLocation(programId, "MVP");

                glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVP[0][0]);


                GLuint WinScaleID = (GLuint)glGetUniformLocation(programId, "WIN_SCALE");
                
                glUniform2f(WinScaleID, 640, 480);

                draw(&(*mesh));
            }
        }
    }
    
    SDL_GL_SwapWindow(window);
}
