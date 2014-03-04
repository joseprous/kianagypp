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

#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <algorithm>
#include <SDL2/SDL.h>
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/rotate_vector.hpp>

#include <memory>
#include <bullet/btBulletDynamicsCommon.h>

#include "Camera.hpp"
#include "Cube.hpp"
#include "Map.hpp"
#include "Player.hpp"
using namespace glm;


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
    std::vector<char> VertexShaderErrorMessage(InfoLogLength);
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
    std::vector<char> FragmentShaderErrorMessage(InfoLogLength);
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
    std::vector<char> GeometryShaderErrorMessage(InfoLogLength);
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
    std::vector<char> ProgramErrorMessage( max(InfoLogLength, int(1)) );
    glGetProgramInfoLog(ProgramID, InfoLogLength, NULL, &ProgramErrorMessage[0]);
    fprintf(stdout, "%s\n", &ProgramErrorMessage[0]);

    glDeleteShader(VertexShaderID);
    glDeleteShader(FragmentShaderID);
    glDeleteShader(GeometryShaderID);

    return ProgramID;
}

SDL_Window* init_sdl()
{
	if (SDL_Init(SDL_INIT_EVERYTHING) != 0){
		std::cout << "SDL_Init Error: " << SDL_GetError() << std::endl;
        exit(-1);
	}

    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);

    int width = 640; int height = 480;
    SDL_Window* window;
    SDL_GLContext context;
    int other_flags = 0;

    window = SDL_CreateWindow("kianagy++", 
                               SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 
                               width, height, 
                               SDL_WINDOW_OPENGL|other_flags);
    
    context = SDL_GL_CreateContext(window);

    const unsigned char* version = glGetString(GL_VERSION);
    if (version == NULL) {
		std::cout << "Error creating GL context " << std::endl;        
        exit(-1);
    }else{
        std::cout << version << std::endl;
    }


    SDL_SetRelativeMouseMode(SDL_TRUE);
    
    glewExperimental = GL_TRUE; 
    glewInit();
    return window;
}

int main(int argc, char **argv)
{
    if(argc != 2){
        std::cout << "Usage: " << argv[0] << " map" << std::endl;
        return -1;
    }

    SDL_Window* window;
    
    window = init_sdl();

    // Enable depth test
    glEnable(GL_DEPTH_TEST);
    // Accept fragment if it closer to the camera than the former one
    glDepthFunc(GL_LESS);

    GLuint VertexArrayID;
    glGenVertexArrays(1, &VertexArrayID);
    glBindVertexArray(VertexArrayID);



    auto broadphase = std::make_shared<btDbvtBroadphase>();
        
    auto collisionConfiguration = std::make_shared<btDefaultCollisionConfiguration>();
    auto dispatcher = std::make_shared<btCollisionDispatcher>(collisionConfiguration.get());

    auto solver = std::make_shared<btSequentialImpulseConstraintSolver>();

    auto dynamicsWorld = std::make_shared<btDiscreteDynamicsWorld>(dispatcher.get(),broadphase.get(),solver.get(),collisionConfiguration.get());

    dynamicsWorld->setGravity(btVector3(0,0,-10));

    Map map = load_map(argv[1]);

    float scale = 0.01;
    map.entities[0].load_brushes(dynamicsWorld, scale);
    
    // Create and compile our GLSL program from the shaders
    GLuint programID = LoadShaders( "shaders/simple.vert", "shaders/simple.frag", "shaders/wireframe.geom" );

    glm::vec3 start_pos(1600*scale,1232*scale,600*scale);
    
    Player player1(dynamicsWorld, programID, 25*scale, start_pos);

    glm::vec3 camera_pos = start_pos + glm::vec3(0*scale,-300*scale,100*scale);
    glm::vec3 camera_dir = glm::vec3(1,0,0);
        
    Camera camera( camera_pos, camera_dir,glm::vec3(0,0,1));
    camera.lookAt(player1.getPosition() + glm::vec3(100*scale,0*scale,-200*scale));
    
    //Camera camera(glm::vec3(0,5,0), glm::vec3(1,0,0),glm::vec3(0,0,1));
    
    SDL_Event windowEvent;
    Uint32 gticks2;
    bool quit = false;
    gticks2 = SDL_GetTicks();
    while(!quit) {
        while(SDL_PollEvent(&windowEvent)) {
            SDL_Keysym keysym;
            switch (windowEvent.type){
            case SDL_QUIT:
                quit = true;
                break;
            case SDL_KEYDOWN:
                keysym = windowEvent.key.keysym;
                switch (keysym.sym) {
                case SDLK_ESCAPE:
                    quit = true;
                    break;
                case SDLK_d:
                    camera.MoveRight(10*scale);
                    break;
                case SDLK_a:  
                    camera.MoveLeft(10*scale);
                    break;
                case SDLK_w:    
                    camera.MoveForward(10*scale);
                    break;
                case SDLK_s:    
                    camera.MoveBackward(10*scale);
                    break;
                case SDLK_q:    
                    camera.MoveUp(10*scale);
                    break;
                case SDLK_e:    
                    camera.MoveDown(10*scale);
                    break;
                case SDLK_LEFT:
                    break;
                case SDLK_RIGHT:
                    break;
                case SDLK_UP:
                    break;
                case SDLK_DOWN:
                    break;
                }
                break;
            case SDL_KEYUP:
                break;
            case SDL_MOUSEMOTION:

                float angleX = windowEvent.motion.xrel * (-1);
                float angleY = windowEvent.motion.yrel * (-1);
                
                camera.RotateX(angleX);
                camera.RotateY(angleY);
                break;
            }
        }
        glClearColor(0.0f,0.0f,0.5f,1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glUseProgram(programID);

        // Projection matrix : 45° Field of View, 4:3 ratio, display range : 0.1 unit <-> 100 units
        glm::mat4 Projection = glm::perspective(45.0f, 4.0f / 3.0f, 0.1f, 10000.0f);
        // Camera matrix
        glm::mat4 View       = camera.GetViewMatrix();

        map.draw(programID, Projection, View);

        player1.Draw(Projection, View);
        
        SDL_GL_SwapWindow(window);
        if(SDL_GetTicks()>gticks2+17){
            dynamicsWorld->stepSimulation(1/60.f,10);

            
            btTransform trans;
            player1.fallRigidBody->getMotionState()->getWorldTransform(trans);
            auto pos = trans.getOrigin();
            player1.setPosition(glm::vec3(pos.getX(),pos.getY(),pos.getZ()));
            auto rot = trans.getRotation();
            auto axis = rot.getAxis ();
            auto angle = rot.getAngle();
            glm::quat q = glm::angleAxis (degrees(angle), glm::vec3(axis.getX(),axis.getY(),axis.getZ()));

            //           std::cout << "angle: " << angle << " axis: " << axis.getX() << "," << axis.getY() << "," << axis.getZ() << std::endl;
            
            player1.setOrientation(q);
            
            gticks2=SDL_GetTicks();
        }
    }
	SDL_Quit();

	return 0;
}
