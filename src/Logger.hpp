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

#ifndef LOGGER_HPP
#define LOGGER_HPP

#include <string>
#include <iostream>

#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>

/*void LogM(glm::mat4  mat){
    int i,j;
    for (j=0; j<4; j++){
        for (i=0; i<4; i++){
            printf("%f ",mat[i][j]);
        }
        printf("\n");
    }
    }*/




template<typename T>
void Log(T message)
{
    std::cout << message << std::endl;
}

template<> void Log<glm::vec3>(glm::vec3 v);
template<> void Log<glm::quat>(glm::quat q);
template<> void Log<glm::mat4>(glm::mat4 m);


template<typename T0, typename T1>
void Log(T0 message1, T1 message2)
{
    std::cout << message1 << " " << message2 << " " << std::endl;
}

template<typename T0,typename T1,typename T2>
void Log(T0 message1, T1 message2, T2 message3)
{
    std::cout << message1 << " "  << message2 << " "  << message3 << std::endl;
}


#endif
