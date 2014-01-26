#include "Camera.hpp"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/rotate_vector.hpp>
using namespace glm;


Camera::Camera(glm::vec3 position, glm::vec3 direction, glm::vec3 up)
{
    this->position = position;
    this->direction = glm::normalize(direction);
    this->up = glm::normalize(up);
}
glm::mat4 Camera::GetViewMatrix()
{
    return glm::lookAt( position, position + direction, up );
}

void Camera::MoveForward(float distance)
{
    position = position + (direction * distance);
}

void Camera::MoveBackward(float distance)
{
    position = position + (direction * (distance * (-1)));
}

void Camera::MoveRight(float distance)
{
    position = position + cross(direction, up) * distance; 
}
void Camera::MoveLeft(float distance)
{
    position = position + cross(up, direction) * distance;     
}
void Camera::MoveUp(float distance)
{
    position = position + (up * distance);
}

void Camera::MoveDown(float distance)
{
    position = position + (up * (distance * (-1)));
}

void Camera::RotateX(float angle)
{
    direction = glm::rotate(direction, angle, up);
}

void Camera::RotateY(float angle)
{
    glm::vec3 aux = cross(direction,up);
    direction = glm::rotate(direction, angle, aux);
    up = glm::rotate(up, angle, aux);    
}
