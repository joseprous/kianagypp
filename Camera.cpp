/*
kianagy++
Copyright (C) 2014 José Prous

This file is part of kianagy++.

kianagy++ is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

kianagy++ is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with kianagy++.  If not, see <http://www.gnu.org/licenses/>.
*/

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
    //up = cross(aux,direction);
    //up = glm::rotate(up, angle, aux);
}
