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
        if(EM->has(entity,Parts::Camera)){
            auto &camera = EM->camera[entity];
            if(EM->has(entity,Parts::Position)){
                auto &pos = EM->position[entity];
                view = glm::lookAt( pos.position + camera.relativePosition, pos.position, pos.up );
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
        if(EM->has(entity,Parts::GLShaders)){
            auto &shaders = EM->glShaders[entity];

            if(EM->has(entity,Parts::GLMesh)){
                auto &mesh = EM->glMesh[entity];

                GLuint programId = SM.GetShaderProgram(shaders);
                glUseProgram(programId);

                glm::mat4 Model      = glm::mat4(1.0f);

                glm::mat4 MVP        = projection * view * Model;

                GLuint MatrixID = (GLuint)glGetUniformLocation(programId, "MVP");

                glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVP[0][0]);


                GLuint WinScaleID = (GLuint)glGetUniformLocation(programId, "WIN_SCALE");
                
                glUniform2f(WinScaleID, 640, 480);

                draw(&mesh);
            }
        }
    }
    
    SDL_GL_SwapWindow(window);
}
