#ifndef INPUTSYSTEM_HPP
#define INPUTSYSTEM_HPP

#include "System.hpp"
#include "EntityManager.hpp"

class InputSystem : public System
{
public:

    InputSystem(EntityManagerSP em,uint32_t period)
        :System(em,period){}

    void init() override;
    void update() override;

    using System::update;
};

#endif
