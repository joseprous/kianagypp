#ifndef MOVEPART_HPP
#define MOVEPART_HPP

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/rotate_vector.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>

struct PositionPart
{
    glm::vec3 position;
    glm::vec3 direction;
    glm::vec3 up;
    glm::quat quaternion;
};

#endif
