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

#include <SDL2/SDL.h>
#include <iostream>
#include <GL/glew.h>

#include "Game.hpp"
#include "map/Map.hpp"

#include "CreatePlayer.hpp"
#include "CreateBrush.hpp"

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
    Uint32 other_flags = 0;

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

    Map map = load_map(argv[1]);

    float scale = 0.01f;
    map.entities[0].load_brushes(scale);
    
    glm::vec3 start_pos(1600*scale,1232*scale,600*scale);

    Game game;
    game.renderSystem.window = window;
    /*Entity player = */createPlayer(game.EM,start_pos);
    //game.physicsSystem.dynamicsWorld->addRigidBody(game.EM->collision[player].rigidBody.get());
    Entity cube = createCube(game.EM,start_pos/* - glm::vec3(1,0,-20)*/);
    game.physicsSystem.dynamicsWorld->addRigidBody(game.EM->collision[cube].rigidBody.get());
    
    for(const brush &b : map.entities[0].mBrushes){
        Entity b1 = createBrush(game.EM,b);
        game.physicsSystem.dynamicsWorld->addRigidBody(game.EM->collision[b1].rigidBody.get());
    }

    game.loop();
    std::cout << "Num brushes: " << map.entities[0].mBrushes.size() << std::endl;
    int vertices = 0;
    for(const brush &b : map.entities[0].mBrushes){
        for(const poly &p : b.getMesh()){
            vertices += p.vertexes.size();
        }
    }
    std::cout << "num vertices: " << vertices << std::endl;
}
