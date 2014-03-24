#ifndef RENDERERSYSTEM_HPP
#define RENDERERSYSTEM_HPP

#include "EntityManager.hpp"
#include "System.hpp"
#include "ShaderManager.hpp"

#include <tuple>
#include <map>
#include <SDL2/SDL.h>

class RenderSystem : public System
{
private:
    
    ShaderManager SM;

    bool getCamera(glm::mat4 &view);

public:

    SDL_Window* window;

    RenderSystem(EntityManagerSP em,uint32_t period)
        :System(em,period){}
    
    void init() override;
    void update() override;
    using System::update;
};

#endif
