#include "RendererSystem.hpp"

#include <iostream>
#include <fstream>
#include <algorithm>
#include <string>

RenderSystem::RenderSystem(EntityManagerSP em, uint32_t period)
    :EM(em),
     mLast_ticks(0),
     mPeriod(period)
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

GLuint LoadShaders(const char * vertex_file_path,const char * fragment_file_path,const char * geometry_file_path)
{
    // Create the shaders
    GLuint VertexShaderID = glCreateShader(GL_VERTEX_SHADER);
    GLuint FragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);
    GLuint GeometryShaderID = glCreateShader(GL_GEOMETRY_SHADER);

    // Read the Vertex Shader code from the file
    std::string VertexShaderCode;
    std::ifstream VertexShaderStream(vertex_file_path, std::ios::in);
    if(VertexShaderStream.is_open())
    {
        std::string Line = "";
        while(getline(VertexShaderStream, Line))
            VertexShaderCode += "\n" + Line;
        VertexShaderStream.close();
    }

    // Read the Fragment Shader code from the file
    std::string FragmentShaderCode;
    std::ifstream FragmentShaderStream(fragment_file_path, std::ios::in);
    if(FragmentShaderStream.is_open()){
        std::string Line = "";
        while(getline(FragmentShaderStream, Line))
            FragmentShaderCode += "\n" + Line;
        FragmentShaderStream.close();
    }

    std::string GeometryShaderCode;
    std::ifstream GeometryShaderStream(geometry_file_path, std::ios::in);
    if(GeometryShaderStream.is_open()){
        std::string Line = "";
        while(getline(GeometryShaderStream, Line))
            GeometryShaderCode += "\n" + Line;
        GeometryShaderStream.close();
    }
    

    GLint Result = GL_FALSE;
    int InfoLogLength;

    // Compile Vertex Shader
    printf("Compiling shader : %s\n", vertex_file_path);
    char const * VertexSourcePointer = VertexShaderCode.c_str();
    glShaderSource(VertexShaderID, 1, &VertexSourcePointer , NULL);
    glCompileShader(VertexShaderID);

    // Check Vertex Shader
    glGetShaderiv(VertexShaderID, GL_COMPILE_STATUS, &Result);
    glGetShaderiv(VertexShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
    std::vector<char> VertexShaderErrorMessage((size_t)InfoLogLength);
    glGetShaderInfoLog(VertexShaderID, InfoLogLength, NULL, &VertexShaderErrorMessage[0]);
    fprintf(stdout, "%s\n", &VertexShaderErrorMessage[0]);

    // Compile Fragment Shader
    printf("Compiling shader : %s\n", fragment_file_path);
    char const * FragmentSourcePointer = FragmentShaderCode.c_str();
    glShaderSource(FragmentShaderID, 1, &FragmentSourcePointer , NULL);
    glCompileShader(FragmentShaderID);

    // Check Fragment Shader
    glGetShaderiv(FragmentShaderID, GL_COMPILE_STATUS, &Result);
    glGetShaderiv(FragmentShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
    std::vector<char> FragmentShaderErrorMessage((size_t)InfoLogLength);
    glGetShaderInfoLog(FragmentShaderID, InfoLogLength, NULL, &FragmentShaderErrorMessage[0]);
    fprintf(stdout, "%s\n", &FragmentShaderErrorMessage[0]);

    // Compile Geometry Shader
    printf("Compiling shader : %s\n", fragment_file_path);
    char const * GeometrySourcePointer = GeometryShaderCode.c_str();
    glShaderSource(GeometryShaderID, 1, &GeometrySourcePointer , NULL);
    glCompileShader(GeometryShaderID);

    // Check Geometry Shader
    glGetShaderiv(GeometryShaderID, GL_COMPILE_STATUS, &Result);
    glGetShaderiv(GeometryShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
    std::vector<char> GeometryShaderErrorMessage((size_t)InfoLogLength);
    glGetShaderInfoLog(GeometryShaderID, InfoLogLength, NULL, &GeometryShaderErrorMessage[0]);
    fprintf(stdout, "%s\n", &GeometryShaderErrorMessage[0]);
    
    // Link the program
    fprintf(stdout, "Linking program\n");
    GLuint ProgramID = glCreateProgram();
    glAttachShader(ProgramID, VertexShaderID);
    glAttachShader(ProgramID, FragmentShaderID);
    glAttachShader(ProgramID, GeometryShaderID);
    glLinkProgram(ProgramID);

    // Check the program
    glGetProgramiv(ProgramID, GL_LINK_STATUS, &Result);
    glGetProgramiv(ProgramID, GL_INFO_LOG_LENGTH, &InfoLogLength);
    std::vector<char> ProgramErrorMessage( std::max((size_t)InfoLogLength, size_t(1)) );
    glGetProgramInfoLog(ProgramID, InfoLogLength, NULL, &ProgramErrorMessage[0]);
    fprintf(stdout, "%s\n", &ProgramErrorMessage[0]);

    glDeleteShader(VertexShaderID);
    glDeleteShader(FragmentShaderID);
    glDeleteShader(GeometryShaderID);

    return ProgramID;
}

GLuint GetShaderProgram(const OpenGLShadersPart &shaders, std::map<std::string, GLuint> &mGLPrograms)
{
    if(mGLPrograms.count(shaders.key)==0){
        mGLPrograms[shaders.key] =
            LoadShaders(shaders.vertexShaderPath.c_str(),
                        shaders.fragmentShaderPath.c_str(),
                        shaders.geometryShaderPath.c_str()
                );
    }
    return mGLPrograms[shaders.key];
}


void RenderSystem::update(uint32_t ticks)
{
    if(ticks <= mLast_ticks + mPeriod) return;
        
    glm::mat4 view;

    for(size_t entity = 0; entity < EM->size; entity++){
        if(EM->has(entity,Parts::Camera)){
            auto &camera = EM->camera[entity];
            if(EM->has(entity,Parts::Position)){
                auto &pos = EM->position[entity];
                view = glm::lookAt( pos.position + camera.relativePosition, pos.position, pos.up );                
            }
        }
    }

    glClearColor(0.0f,0.0f,0.5f,1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    
    glm::mat4 projection = glm::perspective(45.0f, 4.0f / 3.0f, 0.1f, 10000.0f);
    for(size_t entity = 0; entity < EM->size; entity++){
        if(EM->has(entity,Parts::GLShaders)){
            auto &shaders = EM->glShaders[entity];

            if(EM->has(entity,Parts::GLMesh)){
                auto &mesh = EM->glMesh[entity];

                GLuint programId = GetShaderProgram(shaders,mGLPrograms);
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

    mLast_ticks = ticks;
}
