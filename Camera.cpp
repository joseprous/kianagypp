#include "Camera.hpp"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/rotate_vector.hpp>
using namespace glm;


Camera::Camera(glm::vec3 position, glm::vec3 direction, glm::vec3 up)
    : Entity(position,direction,up)
{
    
}
glm::mat4 Camera::GetViewMatrix()
{
    return glm::lookAt( position, position + direction, up );
}

void Camera::RotateX(float angle)
{
    direction = glm::rotate(direction, angle, up);
}

void Camera::RotateY(float angle)
{
    glm::vec3 aux = cross(direction, up);
    direction = glm::rotate(direction, angle, aux);
    up = glm::rotate(up, angle, aux);
}
