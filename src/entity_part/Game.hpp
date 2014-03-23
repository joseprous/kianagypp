#ifndef GAME_HPP
#define GAME_HPP

#include "EntityManager.hpp"
#include "RendererSystem.hpp"
#include "PhysicsSystem.hpp"
#include "InputSystem.hpp"

class Game
{
public:
    
    EntityManagerSP EM;

    InputSystem inputSystem;
    RenderSystem renderSystem;
    PhysicsSystem physicsSystem;

    Game();
    [[noreturn]] void loop();
};

#endif
