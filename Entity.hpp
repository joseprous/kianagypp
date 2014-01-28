#ifndef ENTITY_HPP
#define ENTITY_HPP

#include <string>
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>

class Entity
{
protected:
    
    glm::vec3 position;
    glm::vec3 direction;
    glm::vec3 up;
    glm::quat quaternion;
    
public:

    std::string name;

    Entity(glm::vec3 position, glm::vec3 direction, glm::vec3 up);

    virtual void MoveForward(float distance);
    virtual void MoveBackward(float distance);
    virtual void MoveLeft(float distance);
    virtual void MoveRight(float distance);
    virtual void MoveUp(float distance);
    virtual void MoveDown(float distance);
    virtual void RotateX(float angle);
    virtual void RotateY(float angle);
    virtual void Move(glm::vec3 dir, float distance);

};
#endif
