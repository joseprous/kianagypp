#include "EntityManager.hpp"

#include <glm/gtx/quaternion.hpp>

Entity createBrush(EntityManagerSP em, const brush &b)
{
    Entity brush = em->addEntity();
    em->attach(brush, {Parts::Mesh, Parts::GLShaders, Parts::GLMesh, Parts::Collision});

    em->mesh[brush] = {b.getMesh()};
    em->glShaders[brush] = {"shaders/simple.vert", "shaders/simple.frag", "shaders/wireframe.geom", "wireframe" };

    em->glMesh[brush] = create_glmesh(b.getMesh());
    em->collision[brush] = create_collision_part(b.getMesh(), false, false);

    return brush;
}
