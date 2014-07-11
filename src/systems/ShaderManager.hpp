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

#ifndef SHADERMANAGER_HPP
#define SHADERMANAGER_HPP

#include <GL/glew.h>
#include "parts/OpenGLShadersPart.hpp"
#include <map>

class ShaderManager
{
private:
   
public:
    std::map<std::string, GLuint> mGLPrograms;

    GLuint LoadShaders(const char * vertex_file_path,const char * fragment_file_path,const char * geometry_file_path);

    
    GLuint GetShaderProgram(const OpenGLShadersPart &shaders);


};

#endif
