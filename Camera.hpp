#ifndef CAMERA_HPP
#define CAMERA_HPP

#include "Entity.hpp"

class Camera : public Entity
{

public:
    Camera(glm::vec3 position, glm::vec3 direction, glm::vec3 up);
    glm::mat4 GetViewMatrix();
};
#endif
