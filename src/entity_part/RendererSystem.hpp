#ifndef RENDERERSYSTEM_HPP
#define RENDERERSYSTEM_HPP

#include "EntityManager.hpp"

#include <tuple>
#include <map>

class RendererSystem
{
private:
    
    EntityManager *EM;
    std::map<std::string, GLuint> mGLPrograms;

public:

    RendererSystem(EntityManager *em);
    void update();
};

#endif
