#ifndef GAME_HPP
#define GAME_HPP

#include "EntityManager.hpp"
#include "systems/RendererSystem.hpp"
#include "systems/PhysicsSystem.hpp"
#include "systems/InputSystem.hpp"

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
