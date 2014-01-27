#include <iostream>

#include "Entity.hpp"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/rotate_vector.hpp>
using namespace glm;

void printVec3(std::string name, glm::vec3 v)
{
    std::cout << name << ": " << v.x << "," << v.y << "," << v.z << std::endl;    
}

Entity::Entity(glm::vec3 position, glm::vec3 direction, glm::vec3 up)
{
    this->position = position;
    this->direction = glm::normalize(direction);
    this->up = glm::normalize(up);
    this->quaternion = angleAxis((float)0.0, direction);
}

void Entity::MoveForward(float distance)
{
    position = position + (direction * distance);
}

void Entity::MoveBackward(float distance)
{
    position = position + (direction * (distance * (-1)));
}

void Entity::MoveRight(float distance)
{
    position = position + cross(direction, up) * distance; 
}
void Entity::MoveLeft(float distance)
{
    position = position + cross(up, direction) * distance; 
}
void Entity::MoveUp(float distance)
{
    position = position + (up * distance);
}

void Entity::MoveDown(float distance)
{
    position = position + (up * (distance * (-1)));
}

void Entity::RotateXQ(float angle)
{
    //direction = glm::rotate(direction, angle, up);
    quaternion = glm::rotate(quaternion,angle,  up);
        //angleAxis(angle, glm::vec3(up)) * quaternion;
    direction = quaternion * direction;
}

void Entity::RotateYQ(float angle)
{
    glm::vec3 aux = cross(direction, up);
    //direction = glm::rotate(direction, angle, aux);
    //up = glm::rotate(up, angle, aux);*/
    quaternion = glm::rotate(quaternion,angle,aux);
        //angleAxis(angle, aux) * quaternion;
    direction = quaternion * direction;
    up = quaternion * up;
}

void Entity::RotateX(float angle)
{
    direction = glm::rotate(direction, angle, up);
}

void Entity::RotateY(float angle)
{
    glm::vec3 aux = cross(direction, up);
    direction = glm::rotate(direction, angle, aux);
    up = glm::rotate(up, angle, aux);
}
