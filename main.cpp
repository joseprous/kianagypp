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
#include "Map.hpp"
#include "Map_Driver.hpp"
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

Map load_map(int argc, char **argv)
{
    Map_Driver driver;
    for (int i = 1; i < argc; ++i){
        if (argv[i] == std::string ("-p"))
            driver.trace_parsing = true;
        else if (argv[i] == std::string ("-s"))
            driver.trace_scanning = true;
        else
            driver.parse (argv[i]);                
    }
    return driver.map;
}

void draw_map(Map &map, GLuint programID, glm::mat4 projection, glm::mat4 view)
{
    glm::mat4 Model      = glm::mat4(1.0f);  // Changes for each model !

    glm::mat4 MVP        = projection * view * Model; // Remember, matrix multiplication is the other way around


    GLuint MatrixID = glGetUniformLocation(programID, "MVP");

    glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVP[0][0]);


    GLuint WinScaleID = glGetUniformLocation(programID, "WIN_SCALE");

    glUniform2f(WinScaleID, 640, 480);

    for(brush b : map.entities[0].brushes){
        //for(poly p : b.polys){
//    poly p=map.entities[0].brushes[0].polys[0];
            glEnableVertexAttribArray(0);
            glBindBuffer(GL_ARRAY_BUFFER, b.vertexbuffer);
            glVertexAttribPointer(
                0,                  // attribute 0. No particular reason for 0, but must match the layout in the shader.
                3,                  // size
                GL_FLOAT,           // type
                GL_FALSE,           // normalized?
                0,                  // stride
                (void*)0            // array buffer offset
                );


            glEnableVertexAttribArray(1);
            glBindBuffer(GL_ARRAY_BUFFER, b.colorbuffer);
            glVertexAttribPointer(
                1,                                // attribute. No particular reason for 1, but must match the layout in the shader.
                3,                                // size
                GL_FLOAT,                         // type
                GL_FALSE,                         // normalized?
                0,                                // stride
                (void*)0                          // array buffer offset
                );
            // Index buffer
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, b.elementbuffer);
 
            // Draw the triangles !
            glDrawElements(
                GL_TRIANGLES,      // mode
                b.element_buffer_data.size(),    // count
                GL_UNSIGNED_INT,   // type
                (void*)0           // element array buffer offset
                );
            
            //glDrawArrays(GL_TRIANGLE_FAN, 0, p.num);
            
            glDisableVertexAttribArray(0);
            glDisableVertexAttribArray(1);
            //        }
    }
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

    Map map = load_map(argc,argv);

    map.entities[0].load_brushes();
    
    // Create and compile our GLSL program from the shaders
    GLuint programID = LoadShaders( "shaders/simple.vert", "shaders/simple.frag", "shaders/wireframe.geom" );

    Cube cube1(programID, 1, glm::vec3(0,0,0));
    Cube cube2(programID, 1, glm::vec3(0,2,2));
    Cube cube3(programID, 5, glm::vec3(10,0,0));
    
    cube2.name = "cube2";

//    cube1.RotateX(45);
    //cube2.RotateY(90);
    cube3.RotateX(90);
    
    Camera camera(glm::vec3(1432,1232,240), glm::vec3(1,0,0),glm::vec3(0,0,1));
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
                    camera.MoveRight(10);
                    break;
                case SDLK_a:  
                    camera.MoveLeft(10);
                    break;
                case SDLK_w:    
                    camera.MoveForward(10);
                    break;
                case SDLK_s:    
                    camera.MoveBackward(10);
                    break;
                case SDLK_q:    
                    camera.MoveUp(1);
                    break;
                case SDLK_e:    
                    camera.MoveDown(1);
                    break;
                case SDLK_z:    
                    cube2.RotateX(-10);
                    break;
                case SDLK_x:    
                    cube2.RotateX(10);
                    break;
                case SDLK_c:    
                    cube2.RotateY(-10);
                    break;
                case SDLK_v:    
                    cube2.RotateY(10);
                    break;
                case SDLK_l:
                    cube2.MoveRight(1);
                    break;
                case SDLK_h:  
                    cube2.MoveLeft(1);
                    break;
                case SDLK_k:    
                    cube2.MoveForward(1);
                    break;
                case SDLK_j:    
                    cube2.MoveBackward(1);
                    break;
                case SDLK_LEFT:
                    camera.RotateX(-10);
                    break;
                case SDLK_RIGHT:
                    camera.RotateX(10);                    
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
                //std::cout << "xrel:" << windowEvent.motion.xrel << std::endl;
                //std::cout << "yrel:" << windowEvent.motion.yrel << std::endl;

                float angleX = windowEvent.motion.xrel * (-1);
                float angleY = windowEvent.motion.yrel * (-1);
                //std::cout << "angleX:" << angleX << std::endl;
                //std::cout << "angleY:" << angleY << std::endl;
                //std::cout << std::endl;
                
                camera.RotateX(angleX);
                camera.RotateY(angleY);
                break;
            }
        }
        glClearColor(0.0f,0.0f,0.5f,1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glUseProgram(programID);

        //  glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

        // Projection matrix : 45° Field of View, 4:3 ratio, display range : 0.1 unit <-> 100 units
        glm::mat4 Projection = glm::perspective(45.0f, 4.0f / 3.0f, 0.1f, 10000.0f);
        // Camera matrix
        glm::mat4 View       = camera.GetViewMatrix();

        //cube1.Draw(Projection, View);
        //cube2.Draw(Projection, View);
        //cube3.Draw(Projection, View);

        draw_map(map,programID,Projection, View);

        SDL_GL_SwapWindow(window);
        if(SDL_GetTicks()>gticks2+100){
            cube1.RotateX(1);
//            cube2.RotateXQ(90);
            gticks2=SDL_GetTicks();
        }
    }
	SDL_Quit();

	return 0;
}
