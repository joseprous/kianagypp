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

    void MoveForward(float distance);
    void MoveBackward(float distance);
    void MoveLeft(float distance);
    void MoveRight(float distance);
    void MoveUp(float distance);
    void MoveDown(float distance);
    void RotateXQ(float angle);
    void RotateYQ(float angle);
    void RotateX(float angle);
    void RotateY(float angle);
};
#endif
