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

#ifndef IQMPART_HPP
#define IQMPART_HPP


#include "iqm/util.h"
#include "iqm/geom.h"
#include "iqm/iqm.h"
#include <GL/glew.h>
#include <vector>
#include <string>

struct IQMPart
{
    std::vector<GLuint> textures;
    GLuint notexture = 0;
    std::vector<Matrix3x4> baseframe; 
    std::vector<Matrix3x4> inversebaseframe;
    std::vector<Matrix3x4> outframe;
    std::vector<Matrix3x4> frames;
    std::vector<uchar> buf;
    iqmheader hdr;
    GLuint ebo = 0, vbo = 0, ubo = 0;
    GLint ubosize = 0, bonematsoffset = 0;
};

IQMPart create_iqm(std::string model_name);

#endif
