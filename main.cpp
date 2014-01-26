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

#include "Camera.hpp"
#include "Cube.hpp"

using namespace glm;


GLuint LoadShaders(const char * vertex_file_path,const char * fragment_file_path)
{
    // Create the shaders
    GLuint VertexShaderID = glCreateShader(GL_VERTEX_SHADER);
    GLuint FragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);

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

    // Link the program
    fprintf(stdout, "Linking program\n");
    GLuint ProgramID = glCreateProgram();
    glAttachShader(ProgramID, VertexShaderID);
    glAttachShader(ProgramID, FragmentShaderID);
    glLinkProgram(ProgramID);

    // Check the program
    glGetProgramiv(ProgramID, GL_LINK_STATUS, &Result);
    glGetProgramiv(ProgramID, GL_INFO_LOG_LENGTH, &InfoLogLength);
    std::vector<char> ProgramErrorMessage( max(InfoLogLength, int(1)) );
    glGetProgramInfoLog(ProgramID, InfoLogLength, NULL, &ProgramErrorMessage[0]);
    fprintf(stdout, "%s\n", &ProgramErrorMessage[0]);

    glDeleteShader(VertexShaderID);
    glDeleteShader(FragmentShaderID);

    return ProgramID;
}

SDL_Window* init_sdl()
{
	if (SDL_Init(SDL_INIT_EVERYTHING) != 0){
		std::cout << "SDL_Init Error: " << SDL_GetError() << std::endl;
        exit(-1);
	}

    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
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
    SDL_Window* window;
    
    window = init_sdl();

    // Enable depth test
    glEnable(GL_DEPTH_TEST);
    // Accept fragment if it closer to the camera than the former one
    glDepthFunc(GL_LESS);

    GLuint VertexArrayID;
    glGenVertexArrays(1, &VertexArrayID);
    glBindVertexArray(VertexArrayID);

    Cube cube1(1,glm::vec3(0,0,0));
    Cube cube2(1,glm::vec3(0,2,2));
    Cube cube3(5,glm::vec3(10,0,0));

    // Create and compile our GLSL program from the shaders
    GLuint programID = LoadShaders( "shaders/simple.vert", "shaders/simple.frag" );


    Camera camera(glm::vec3(-5,0,0), glm::vec3(1,0,0),glm::vec3(0,1,0));
    
    SDL_Event windowEvent;
    Uint32 gticks2;
    bool quit;
    float angle = 0;
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
                    camera.MoveRight(1);
                    break;
                case SDLK_a:  
                    camera.MoveLeft(1);
                    break;
                case SDLK_w:    
                    camera.MoveForward(1);
                    break;
                case SDLK_s:    
                    camera.MoveBackward(1);
                    break;
                case SDLK_q:    
                    camera.MoveUp(1);
                    break;
                case SDLK_e:    
                    camera.MoveDown(1);
                    break;
                }
                break;
            case SDL_KEYUP:
                break;
            case SDL_MOUSEMOTION:
                //rotcam(event.motion.xrel,event.motion.yrel);
                camera.RotateX(windowEvent.motion.xrel * (-1));
                camera.RotateY(windowEvent.motion.yrel * (-1));
                break;
            }
        }

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glUseProgram(programID);

//        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

        // Projection matrix : 45° Field of View, 4:3 ratio, display range : 0.1 unit <-> 100 units
        glm::mat4 Projection = glm::perspective(45.0f, 4.0f / 3.0f, 0.1f, 100.0f);
        // Camera matrix
        glm::mat4 View       = camera.GetViewMatrix();

        cube1.Draw(programID, Projection, View, 0);
        cube2.Draw(programID, Projection, View, angle);
        cube3.Draw(programID, Projection, View, angle);


        SDL_GL_SwapWindow(window);
        if(SDL_GetTicks()>gticks2+100){
            angle = ((int)angle + 1) % 360;
            gticks2=SDL_GetTicks();
        }
    }
	SDL_Quit();

	return 0;
}
