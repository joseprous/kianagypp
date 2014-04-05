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

#include "Logger.hpp"

template<> void Log<glm::vec3>(glm::vec3 v)
{
    std::cout << "(" << v.x << "," << v.y << "," << v.z << ")" << std::endl;
}

template<> void Log<glm::quat>(glm::quat q)
{
    std::cout << "(" << q.x << "," << q.y << "," << q.z << "," << q.w << ")" << std::endl;
}

template<> void Log<glm::mat4>(glm::mat4 mat)
{
    int i,j;
    for (j=0; j<4; j++){
        for (i=0; i<4; i++){
            std::cout << mat[i][j] << " ";
        }
        std::cout << std::endl;
    }
}
