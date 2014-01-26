#ifndef CAMERA_HPP
#define CAMERA_HPP

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/rotate_vector.hpp>

class Camera
{
private:
    
    glm::vec3 position;
    glm::vec3 direction;
    glm::vec3 up;

public:
    Camera(glm::vec3 position, glm::vec3 direction, glm::vec3 up);
    glm::mat4 GetViewMatrix();
    void MoveForward(float distance);
    void MoveBackward(float distance);
    void MoveLeft(float distance);
    void MoveRight(float distance);
    void MoveUp(float distance);
    void MoveDown(float distance);
    void RotateX(float angle);
    void RotateY(float angle);
};
#endif
