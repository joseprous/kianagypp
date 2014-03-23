#ifndef RENDERERSYSTEM_HPP
#define RENDERERSYSTEM_HPP

#include "EntityManager.hpp"

#include <tuple>
#include <map>
#include <SDL2/SDL.h>

class RenderSystem
{
private:
    
    std::map<std::string, GLuint> mGLPrograms;

    EntityManagerSP EM;
    uint32_t mLast_ticks;
    uint32_t mPeriod;
public:

    SDL_Window* window;

    RenderSystem(EntityManagerSP em,uint32_t period);
    void update(uint32_t ticks);
};

#endif
