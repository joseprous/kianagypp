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

#ifndef BRUSH_HPP
#define BRUSH_HPP

#include <vector>
#include <string>
#include <glm/glm.hpp>
#include <GL/glew.h>
#include "Physics.hpp"

struct line{
    glm::dvec3 dir;
    glm::dvec3 point;
};

//ax+by+cz+d=0
struct plane{
	double a,b,c,d;
	glm::dvec3 normal;
};

struct rawplane
{
    glm::dvec3 point0;
    glm::dvec3 point1;
    glm::dvec3 point2;
    std::string texture_name;
    std::vector<double> texture_data;
};

struct rawbrush
{
    std::vector<rawplane> rawplanes;
};

struct poly
{
    std::vector<glm::dvec3> vertexes;

    glm::dvec3 normal;
    std::string tex;
    glm::dvec3 center;
    int num;
};

class brush
{
public:
    
    void load(dynamicsWorldSP dynamicsWorld, rawbrush &rb);
    void draw(GLuint programID, glm::mat4 projection, glm::mat4 view);

private:
    
    std::vector<poly> polys;
    std::vector<GLfloat> vertex_buffer_data;
    std::vector<GLfloat> color_buffer_data;
    std::vector<GLuint> element_buffer_data;    
    GLuint vertexbuffer;
    GLuint colorbuffer;
    GLuint elementbuffer;
    int num;

    std::shared_ptr<btConvexHullShape> convexHullShape;
    std::shared_ptr<btDefaultMotionState> groundMotionState;
    std::shared_ptr<btRigidBody> groundRigidBody;
    
    void create_buffers();
    void order_vertexes();
    void remove_extra_vertexes(std::vector<plane> &planes);
    void add_vertexes_to_polys(std::vector<plane> &planes);
    void create_planes_from_points(rawbrush &b, std::vector<plane> &planes);

};

#endif

